#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// Enum untuk preferensi yang aman dari kesalahan pengetikan (type-safe)
enum class PreferensiRute { Waktu, Biaya, Jarak };

// Struct sederhana untuk paket data hasil pencarian
struct HasilRute {
    vector<int> path;
    double totalJarak = 0.0, totalWaktu = 0.0, totalBiaya = 0.0;
    bool ditemukan = false;
};

// ===================================================================
// KELAS 1: LOKASI (Data dengan Enkapsulasi)
// ===================================================================
class Lokasi {
private:
    int id;
    string nama;

public:
    Lokasi(int id_val = 0, const string& nama_val = "") : id(id_val), nama(nama_val) {}
    int getId() const { return id; }
    const string& getNama() const { return nama; }
    void setNama(const string& namaBaru) { nama = namaBaru; }
};

// ===================================================================
// KELAS 2: RUTE (Data dengan Enkapsulasi)
// ===================================================================
class Rute {
private:
    int idLokasiTujuan;
    double jarak, waktu, biaya;

public:
    Rute(int tujuan, double j, double w, double b)
        : idLokasiTujuan(tujuan), jarak(j), waktu(w), biaya(b) {}
    int getIdLokasiTujuan() const { return idLokasiTujuan; }
    double getJarak() const { return jarak; }
    double getWaktu() const { return waktu; }
    double getBiaya() const { return biaya; }
    double getBobot(PreferensiRute p) const {
        if (p == PreferensiRute::Waktu) return waktu;
        if (p == PreferensiRute::Biaya) return biaya;
        if (p == PreferensiRute::Jarak) return jarak;
        return numeric_limits<double>::infinity();
    }
};

// ===================================================================
// KELAS 3: GRAF (Otak Sistem: Data + Algoritma)
// ===================================================================
class Graf {
private:
    unordered_map<int, Lokasi> daftarLokasi;
    unordered_map<int, vector<Rute>> adjacencyList;
    int nextLokasiId = 1;

    void perbaruiNextId() {
        nextLokasiId = daftarLokasi.empty() ? 1 : max_element(daftarLokasi.begin(), daftarLokasi.end())->first + 1;
    }

public:
    // --- Manajemen Data (CRUD) ---
    void tambahLokasi(const string& nama) {
        int idBaru = nextLokasiId++;
        daftarLokasi.emplace(idBaru, Lokasi(idBaru, nama));
        cout << "SUCCESS: Lokasi '" << nama << "' (ID: " << idBaru << ") ditambahkan.\n";
    }

    void tambahLokasi(int id, const string& nama) {
        if (daftarLokasi.count(id)) return;
        daftarLokasi.emplace(id, Lokasi(id, nama));
        perbaruiNextId();
    }

    void tambahRute(int idAwal, int idTujuan, double jarak, double waktu, double biaya) {
        if (!daftarLokasi.count(idAwal) || !daftarLokasi.count(idTujuan)) {
            cerr << "ERROR: ID Lokasi tidak valid untuk menambah rute.\n"; return;
        }
        adjacencyList[idAwal].emplace_back(idTujuan, jarak, waktu, biaya);
    }
    
    // --- Akses Data (Getter) yang Aman ---
    const Lokasi* getLokasi(int id) const {
        auto it = daftarLokasi.find(id);
        return (it != daftarLokasi.end()) ? &(it->second) : nullptr;
    }
    
    vector<Lokasi> getSemuaLokasi() const {
        vector<Lokasi> semua;
        for (const auto& pair : daftarLokasi) semua.push_back(pair.second);
        sort(semua.begin(), semua.end(), [](const Lokasi& a, const Lokasi& b){ return a.getId() < b.getId(); });
        return semua;
    }

    // --- Fungsionalitas Inti ---
    void tampilkanGraf() const {
        cout << "\n--- Peta Rute Saat Ini ---\n";
        if (daftarLokasi.empty()) { cout << "Peta masih kosong.\n"; return; }
        
        for (const auto& lok : getSemuaLokasi()) {
            cout << "[" << lok.getId() << ": " << lok.getNama() << "] terhubung ke:\n";
            auto it = adjacencyList.find(lok.getId());
            if (it == adjacencyList.end() || it->second.empty()) {
                cout << "  (Tidak ada rute keluar)\n";
            } else {
                for (const Rute& rute : it->second) {
                    const Lokasi* lokTujuan = getLokasi(rute.getIdLokasiTujuan());
                    if(lokTujuan) cout << "  -> " << lokTujuan->getNama() << " (ID: " << lokTujuan->getId() << ")\n";
                }
            }
        }
        cout << "---------------------------\n";
    }

    void bersihkanGraf() {
        daftarLokasi.clear();
        adjacencyList.clear();
        nextLokasiId = 1;
        cout << "INFO: Graf telah dibersihkan.\n";
    }
    
    // --- PERUBAHAN UTAMA: Algoritma Dijkstra menjadi method dari Graf ---
    HasilRute cariRuteTerpendek(int idAwal, int idTujuan, PreferensiRute preferensi) const {
        HasilRute hasil;
        if (!getLokasi(idAwal) || !getLokasi(idTujuan)) return hasil;

        using Pair = pair<double, int>;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        unordered_map<int, double> bobotTotal;
        unordered_map<int, int> pendahulu;

        for (const auto& pair : daftarLokasi) {
            bobotTotal[pair.first] = numeric_limits<double>::infinity();
        }

        bobotTotal[idAwal] = 0.0;
        pq.push({0.0, idAwal});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == idTujuan) break;

            auto it = adjacencyList.find(u);
            if (it != adjacencyList.end()) {
                for (const Rute& rute : it->second) {
                    int v = rute.getIdLokasiTujuan();
                    double bobotRute = rute.getBobot(preferensi);
                    if (bobotTotal.at(u) + bobotRute < bobotTotal.at(v)) {
                        bobotTotal[v] = bobotTotal.at(u) + bobotRute;
                        pendahulu[v] = u;
                        pq.push({bobotTotal.at(v), v});
                    }
                }
            }
        }

        if (pendahulu.find(idTujuan) == pendahulu.end() && idAwal != idTujuan) return hasil;

        // Rekonstruksi path dan hitung total
        int saatIni = idTujuan;
        while (pendahulu.count(saatIni)) {
            hasil.path.push_back(saatIni);
            saatIni = pendahulu.at(saatIni);
        }
        hasil.path.push_back(idAwal);
        reverse(hasil.path.begin(), hasil.path.end());
        hasil.ditemukan = true;

        for (size_t i = 0; i < hasil.path.size() - 1; ++i) {
            auto it = adjacencyList.find(hasil.path[i]);
            if(it != adjacencyList.end()){
                for (const auto& rute : it->second) {
                    if (rute.getIdLokasiTujuan() == hasil.path[i+1]) {
                        hasil.totalJarak += rute.getJarak();
                        hasil.totalWaktu += rute.getWaktu();
                        hasil.totalBiaya += rute.getBiaya();
                        break;
                    }
                }
            }
        }
        return hasil;
    }
};

// ===================================================================
// KELAS 4: SISTEM (Wajah Aplikasi/UI Controller)
// ===================================================================
class Sistem {
private:
    Graf graf;

    // --- Logika I/O dan UI menjadi private method di sini ---
    bool muatDariCSV(const string& fileLokasi, const string& fileRute) {
        graf.bersihkanGraf();
        // Logika memuat file (sama seperti sebelumnya, tapi kini menjadi bagian dari Sistem)
        // ... (implementasi lengkap tidak ditampilkan untuk keringkasan)
        cout << "SUCCESS: Data dari file berhasil dimuat.\n";
        return true;
    }

    bool simpanKeCSV(const string& fileLokasi, const string& fileRute) {
        // Logika menyimpan file
        // ... (implementasi lengkap tidak ditampilkan untuk keringkasan)
        cout << "SUCCESS: Data berhasil disimpan ke file.\n";
        return true;
    }

    PreferensiRute pilihPreferensi() {
        cout << "\nPilih preferensi rute:\n1. Waktu\n2. Biaya\n3. Jarak\nPilihan: ";
        int pil;
        cin >> pil;
        if (pil == 2) return PreferensiRute::Biaya;
        if (pil == 3) return PreferensiRute::Jarak;
        return PreferensiRute::Waktu; // Default
    }

    string getNamaPreferensi(PreferensiRute p) {
        if (p == PreferensiRute::Waktu) return "Waktu Tercepat";
        if (p == PreferensiRute::Biaya) return "Biaya Termurah";
        return "Jarak Terpendek";
    }

    void menuCariRute() {
        graf.tampilkanGraf();
        if (graf.getSemuaLokasi().empty()) return;
        
        cout << "Masukkan ID Lokasi Awal: "; int idAwal; cin >> idAwal;
        cout << "Masukkan ID Lokasi Tujuan: "; int idTujuan; cin >> idTujuan;
        
        PreferensiRute pref = pilihPreferensi();
        
        // Memanggil method langsung dari objek graf
        HasilRute hasil = graf.cariRuteTerpendek(idAwal, idTujuan, pref);

        if (hasil.ditemukan) {
            cout << "\n--- HASIL REKOMENDASI RUTE ---\n";
            cout << "Preferensi: " << getNamaPreferensi(pref) << "\n";
            cout << "Rute: ";
            for (size_t i = 0; i < hasil.path.size(); ++i) {
                cout << graf.getLokasi(hasil.path[i])->getNama() << (i == hasil.path.size() - 1 ? "" : " -> ");
            }
            cout << "\n\nDetail Perjalanan:\n";
            cout << "  - Total Jarak: " << hasil.totalJarak << " km\n";
            cout << "  - Total Waktu: " << hasil.totalWaktu << " menit\n";
            cout << "  - Total Biaya: Rp" << hasil.totalBiaya << "\n";
            cout << "---------------------------------\n";
        } else {
            cout << "\nMaaf, rute tidak ditemukan.\n";
        }
    }

public:
    void jalankan() {
        bool berjalan = true;
        while (berjalan) {
            cout << "\n===== Sistem Rekomendasi Rute Sederhana =====\n";
            cout << "1. Cari Rute\n";
            cout << "2. Tampilkan Peta\n";
            cout << "3. Tambah Lokasi Baru\n";
            // Fitur lain bisa ditambahkan di sini (muat/simpan file, dll)
            cout << "0. Keluar\n";
            cout << "Pilihan: ";
            int pilihan;
            cin >> pilihan;

            switch (pilihan) {
                case 1: menuCariRute(); break;
                case 2: graf.tampilkanGraf(); break;
                case 3: {
                    cout << "Masukkan nama lokasi baru: ";
                    string nama;
                    cin.ignore(); getline(cin, nama);
                    graf.tambahLokasi(nama);
                    break;
                }
                case 0: berjalan = false; break;
                default: cout << "Pilihan tidak valid.\n";
            }
        }
        cout << "Terima kasih!\n";
    }
};

// ===================================================================
// FUNGSI UTAMA
// ===================================================================
int main() {
    Sistem aplikasi;
    aplikasi.jalankan();
    return 0;
}
