#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

// Menggunakan namespace std untuk menghindari penulisan std:: berulang kali
using namespace std;

// ===================================================================
// KELAS 1: LOKASI
// ===================================================================
class Lokasi {
public:
    int id;
    string nama;
    Lokasi(int id = 0, const string& nama = "") : id(id), nama(nama) {}
};


// ===================================================================
// KELAS 2: RUTE
// ===================================================================
class Rute {
public:
    int idLokasiTujuan;
    double jarak;
    double waktu;
    double biaya;
    Rute(int tujuan, double jarak, double waktu, double biaya)
        : idLokasiTujuan(tujuan), jarak(jarak), waktu(waktu), biaya(biaya) {}

    double getBobot(const string& preferensi) const {
        if (preferensi == "waktu") return waktu;
        if (preferensi == "biaya") return biaya;
        if (preferensi == "jarak") return jarak;
        return numeric_limits<double>::infinity();
    }
};


// ===================================================================
// KELAS 3: GRAF (VERSI LENGKAP DENGAN FUNGSI CRUD)
// ===================================================================
class Graf {
private:
    unordered_map<int, Lokasi> daftarLokasi;
    unordered_map<int, vector<Rute>> adjacencyList;
    int nextLokasiId = 1;

public:
    // --- FITUR BARU: Membersihkan graf ---
    void bersihkanGraf() {
        daftarLokasi.clear();
        adjacencyList.clear();
        nextLokasiId = 1;
        cout << "INFO: Graf telah dibersihkan.\n";
    }

    // --- CREATE ---
    void tambahLokasi(const string& nama) {
        int idBaru = nextLokasiId++;
        daftarLokasi[idBaru] = Lokasi(idBaru, nama);
        adjacencyList[idBaru] = {};
        cout << "SUCCESS: Lokasi '" << nama << "' dengan ID " << idBaru << " berhasil ditambahkan.\n";
    }

    void tambahRute(int idAwal, int idTujuan, double jarak, double waktu, double biaya) {
        if (daftarLokasi.find(idAwal) == daftarLokasi.end() || daftarLokasi.find(idTujuan) == daftarLokasi.end()) {
            cout << "ERROR: Salah satu atau kedua ID lokasi tidak valid untuk menambah rute.\n";
            return;
        }
        adjacencyList[idAwal].emplace_back(idTujuan, jarak, waktu, biaya);
        // Jangan tampilkan pesan sukses di sini agar tidak membanjiri layar saat memuat file
    }

    // --- READ ---
    const Lokasi* getLokasi(int id) const {
        if (daftarLokasi.count(id)) return &daftarLokasi.at(id);
        return nullptr;
    }
    const vector<Rute>* getTetangga(int id) const {
        if (adjacencyList.count(id)) return &adjacencyList.at(id);
        return nullptr;
    }
    const unordered_map<int, Lokasi>& getSemuaLokasi() const {
        return daftarLokasi;
    }
    const unordered_map<int, vector<Rute>>& getAdjacencyList() const {
        return adjacencyList;
    }
    void tampilkanGraf() const {
        cout << "\n--- Visualisasi Graf Transportasi ---\n";
        if (daftarLokasi.empty()) {
            cout << "Graf masih kosong.\n";
        }
        for (const auto& pair : daftarLokasi) {
            const Lokasi& lokAwal = pair.second;
            cout << "[" << lokAwal.id << ": " << lokAwal.nama << "] terhubung ke:\n";
            
            const auto it = adjacencyList.find(lokAwal.id);
            if (it == adjacencyList.end() || it->second.empty()) {
                cout << "  (Tidak ada rute keluar)\n";
            } else {
                for (const Rute& rute : it->second) {
                    const Lokasi* lokTujuan = getLokasi(rute.idLokasiTujuan);
                    if (!lokTujuan) continue;
                    cout << "  -> [" << lokTujuan->id << ": " << lokTujuan->nama << "] "
                         << "(Jarak: " << rute.jarak << " km, "
                         << "Waktu: " << rute.waktu << " mnt, "
                         << "Biaya: Rp" << rute.biaya << ")\n";
                }
            }
        }
        cout << "-------------------------------------\n";
    }

    // --- UPDATE ---
    void updateLokasi(int id, const string& namaBaru) {
        if (daftarLokasi.find(id) != daftarLokasi.end()) {
            daftarLokasi.at(id).nama = namaBaru;
            cout << "SUCCESS: Lokasi ID " << id << " berhasil diperbarui menjadi '" << namaBaru << "'.\n";
        } else {
            cout << "ERROR: Lokasi dengan ID " << id << " tidak ditemukan.\n";
        }
    }

    // --- DELETE ---
    bool hapusLokasi(int id) {
        if (daftarLokasi.find(id) == daftarLokasi.end()) {
            cout << "ERROR: Lokasi dengan ID " << id << " tidak ditemukan.\n";
            return false;
        }
        string namaLokasi = daftarLokasi.at(id).nama;
        daftarLokasi.erase(id);
        adjacencyList.erase(id);
        for (auto& pair : adjacencyList) {
            auto& ruteVector = pair.second;
            ruteVector.erase(
                remove_if(ruteVector.begin(), ruteVector.end(),
                                  [id](const Rute& rute) { return rute.idLokasiTujuan == id; }),
                ruteVector.end()
            );
        }
        cout << "SUCCESS: Lokasi '" << namaLokasi << "' (ID: " << id << ") dan semua rute terkait telah dihapus.\n";
        return true;
    }

    bool hapusRute(int idAwal, int idTujuan) {
        if (adjacencyList.find(idAwal) == adjacencyList.end()) {
            cout << "ERROR: Lokasi awal dengan ID " << idAwal << " tidak ditemukan.\n";
            return false;
        }
        auto& ruteVector = adjacencyList.at(idAwal);
        size_t ukuranAwal = ruteVector.size();
        ruteVector.erase(
            remove_if(ruteVector.begin(), ruteVector.end(),
                              [idTujuan](const Rute& rute) { return rute.idLokasiTujuan == idTujuan; }),
            ruteVector.end()
        );
        if (ruteVector.size() < ukuranAwal) {
            cout << "SUCCESS: Rute dari ID " << idAwal << " ke " << idTujuan << " berhasil dihapus.\n";
            return true;
        } else {
            cout << "ERROR: Rute dari ID " << idAwal << " ke " << idTujuan << " tidak ditemukan.\n";
            return false;
        }
    }

    // --- Helper functions ---
    void muatLokasi(int id, const string& nama) {
        if (daftarLokasi.find(id) == daftarLokasi.end()) {
            daftarLokasi[id] = Lokasi(id, nama);
            adjacencyList[id] = {};
        }
    }
    void setNextId(int id) { nextLokasiId = id; }
};

// ===================================================================
// KELAS 4: PENCARI RUTE (Tidak ada perubahan)
// ===================================================================
struct HasilRute {
    vector<int> path;
    double totalJarak = 0.0;
    double totalWaktu = 0.0;
    double totalBiaya = 0.0;
    bool ditemukan = false;
};

class PencariRute {
public:
    HasilRute cariRuteTerpendek(const Graf& graf, int idAwal, int idTujuan, const string& preferensi) {
        HasilRute hasil;
        if (!graf.getLokasi(idAwal) || !graf.getLokasi(idTujuan)) {
            // Pesan error sudah ditangani di fungsi menuCariRute
            return hasil;
        }
        using Pair = pair<double, int>;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        unordered_map<int, double> bobotTotal;
        unordered_map<int, int> pendahulu;
        for (const auto& pair : graf.getSemuaLokasi()) {
            bobotTotal[pair.first] = numeric_limits<double>::infinity();
        }
        bobotTotal[idAwal] = 0.0;
        pq.push({0.0, idAwal});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (u == idTujuan) break;
            const vector<Rute>* tetangga = graf.getTetangga(u);
            if (tetangga) {
                for (const Rute& rute : *tetangga) {
                    int v = rute.idLokasiTujuan;
                    double bobotRute = rute.getBobot(preferensi);
                    if (bobotTotal.count(v) && bobotTotal.at(u) + bobotRute < bobotTotal.at(v)) {
                        bobotTotal[v] = bobotTotal.at(u) + bobotRute;
                        pendahulu[v] = u;
                        pq.push({bobotTotal.at(v), v});
                    }
                }
            }
        }
        if (pendahulu.find(idTujuan) == pendahulu.end() && idAwal != idTujuan) {
             return hasil;
        }
        int saatIni = idTujuan;
        while (pendahulu.count(saatIni)) {
            hasil.path.push_back(saatIni);
            saatIni = pendahulu[saatIni];
        }
        hasil.path.push_back(idAwal);
        reverse(hasil.path.begin(), hasil.path.end());
        hasil.ditemukan = true;
        for (size_t i = 0; i < hasil.path.size() - 1; ++i) {
            int u = hasil.path[i];
            int v = hasil.path[i + 1];
            const auto* tetangga = graf.getTetangga(u);
            if (tetangga) {
                for (const auto& rute : *tetangga) {
                    if (rute.idLokasiTujuan == v) {
                        hasil.totalJarak += rute.jarak;
                        hasil.totalWaktu += rute.waktu;
                        hasil.totalBiaya += rute.biaya;
                        break;
                    }
                }
            }
        }
        return hasil;
    }
};


// ===================================================================
// KELAS 5: POHON PREFERENSI (Tidak ada perubahan)
// ===================================================================
class PohonPreferensi {
public:
    string jalankanPohon() {
        cout << "\nSilakan pilih preferensi rute Anda:\n";
        cout << "1. Waktu Tercepat\n";
        cout << "2. Biaya Termurah\n";
        cout << "3. Jarak Terpendek\n";
        cout << "Pilihan Anda: " << flush;
        int pilihan;
        cin >> pilihan;
        switch(pilihan) {
            case 1: return "waktu";
            case 2: return "biaya";
            case 3: return "jarak";
            default:
                cout << "Pilihan tidak valid. Menggunakan preferensi default (Waktu).\n";
                return "waktu";
        }
    }
};


// ===================================================================
// KELAS 6: MANAJER FILE (Tidak ada perubahan)
// ===================================================================
class ManajerFile {
public:
    bool muatDariCSV(Graf& graf, const string& fileLokasi, const string& fileRute) {
        ifstream fileLok(fileLokasi);
        if (!fileLok.is_open()) {
            cout << "ERROR: File lokasi '" << fileLokasi << "' tidak dapat dibuka.\n";
            return false;
        }
        string line;
        getline(fileLok, line); // Lewati header
        int maxId = 0;
        while (getline(fileLok, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, nama;
            getline(ss, idStr, ',');
            getline(ss, nama);
            int id = stoi(idStr);
            graf.muatLokasi(id, nama);
            if (id > maxId) maxId = id;
        }
        graf.setNextId(maxId + 1);
        fileLok.close();
        ifstream fileRut(fileRute);
        if (!fileRut.is_open()) {
            cout << "ERROR: File rute '" << fileRute << "' tidak dapat dibuka.\n";
            return false;
        }
        getline(fileRut, line); // Lewati header
        while (getline(fileRut, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idAwal, idTujuan, jarak, waktu, biaya;
            getline(ss, idAwal, ',');
            getline(ss, idTujuan, ',');
            getline(ss, jarak, ',');
            getline(ss, waktu, ',');
            getline(ss, biaya, ',');
            graf.tambahRute(stoi(idAwal), stoi(idTujuan), stod(jarak), stod(waktu), stod(biaya));
        }
        fileRut.close();
        return true;
    }
    
    bool simpanKeCSV(const Graf& graf, const string& fileLokasi, const string& fileRute) {
        ofstream fileLok(fileLokasi);
        if (!fileLok.is_open()) return false;
        fileLok << "id,nama\n";
        for(const auto& pair : graf.getSemuaLokasi()){
            fileLok << pair.second.id << "," << pair.second.nama << "\n";
        }
        fileLok.close();
        ofstream fileRut(fileRute);
        if(!fileRut.is_open()) return false;
        fileRut << "id_awal,id_tujuan,jarak,waktu,biaya\n";
        for(const auto& pair : graf.getAdjacencyList()){
            int idAwal = pair.first;
            for(const auto& rute : pair.second){
                fileRut << idAwal << "," << rute.idLokasiTujuan << "," << rute.jarak << "," << rute.waktu << "," << rute.biaya << "\n";
            }
        }
        fileRut.close();
        cout << "SUCCESS: Data berhasil disimpan ke file '" << fileLokasi << "' dan '" << fileRute << "'.\n";
        return true;
    }
};


// ===================================================================
// KELAS 7: SISTEM (VERSI DENGAN MENU CRUD)
// ===================================================================
class Sistem {
private:
    Graf graf;
    PencariRute pencari;
    PohonPreferensi pohonPreferensi;
    ManajerFile manajerFile;

    // --- Fungsi UI ---
    void menuCariRute() {
        cout << "\n--- Cari Rute ---\n";
        graf.tampilkanGraf();
        if (graf.getSemuaLokasi().empty()) {
            cout << "Graf masih kosong. Silakan tambah lokasi dan rute terlebih dahulu.\n";
            return;
        }
        cout << "Masukkan ID Lokasi Awal: ";
        int idAwal;
        cin >> idAwal;
        if (cin.fail()) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Masukkan ID Lokasi Tujuan: ";
        int idTujuan;
        cin >> idTujuan;
        if (cin.fail()) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        string preferensi = pohonPreferensi.jalankanPohon();
        HasilRute hasil = pencari.cariRuteTerpendek(graf, idAwal, idTujuan, preferensi);
        if (hasil.ditemukan) {
            cout << "\n--- HASIL REKOMENDASI RUTE ---\n";
            cout << "Preferensi: " << preferensi << endl;
            cout << "Rute: ";
            for (size_t i = 0; i < hasil.path.size(); ++i) {
                const Lokasi* lok = graf.getLokasi(hasil.path[i]);
                if (lok) {
                    cout << lok->nama << (i == hasil.path.size() - 1 ? "" : " -> ");
                }
            }
            cout << "\n\nDetail Perjalanan:\n";
            cout << "  - Total Jarak: " << hasil.totalJarak << " km\n";
            cout << "  - Total Waktu: " << hasil.totalWaktu << " menit\n";
            cout << "  - Total Biaya: Rp" << hasil.totalBiaya << "\n";
            cout << "---------------------------------\n";
        } else {
            const Lokasi* lokAwal = graf.getLokasi(idAwal);
            const Lokasi* lokTujuan = graf.getLokasi(idTujuan);
            if (lokAwal && lokTujuan) {
                cout << "\nMaaf, tidak ditemukan rute dari " << lokAwal->nama << " ke " << lokTujuan->nama << ".\n";
            } else {
                cout << "\nMaaf, ID lokasi awal atau tujuan tidak valid.\n";
            }
        }
    }
    
    void prosesTambahLokasi() {
        cout << "\n-- Tambah Lokasi Baru --\n";
        cout << "Masukkan nama lokasi: ";
        string nama;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nama);
        graf.tambahLokasi(nama);
    }

    void prosesHapusLokasi() {
        cout << "\n-- Hapus Lokasi --\n";
        graf.tampilkanGraf();
        cout << "Masukkan ID lokasi yang akan dihapus: ";
        int id;
        cin >> id;
        graf.hapusLokasi(id);
    }

    void prosesUpdateLokasi() {
        cout << "\n-- Update Nama Lokasi --\n";
        graf.tampilkanGraf();
        cout << "Masukkan ID lokasi yang akan diupdate: ";
        int id;
        cin >> id;
        cout << "Masukkan nama baru untuk lokasi ID " << id << ": ";
        string namaBaru;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, namaBaru);
        graf.updateLokasi(id, namaBaru);
    }

    void prosesTambahRute() {
        cout << "\n-- Tambah Rute Baru --\n";
        graf.tampilkanGraf();
        int idAwal, idTujuan;
        double jarak, waktu, biaya;
        cout << "Masukkan ID lokasi awal: ";
        cin >> idAwal;
        cout << "Masukkan ID lokasi tujuan: ";
        cin >> idTujuan;
        cout << "Masukkan jarak (km): ";
        cin >> jarak;
        cout << "Masukkan waktu (menit): ";
        cin >> waktu;
        cout << "Masukkan biaya (Rp): ";
        cin >> biaya;
        graf.tambahRute(idAwal, idTujuan, jarak, waktu, biaya);
        cout << "SUCCESS: Rute dari " << idAwal << " ke " << idTujuan << " berhasil ditambahkan.\n";
    }

    void prosesHapusRute() {
        cout << "\n-- Hapus Rute --\n";
        graf.tampilkanGraf();
        int idAwal, idTujuan;
        cout << "Masukkan ID lokasi awal rute: ";
        cin >> idAwal;
        cout << "Masukkan ID lokasi tujuan rute: ";
        cin >> idTujuan;
        graf.hapusRute(idAwal, idTujuan);
    }

    void menuManajemenData() {
        bool kembali = false;
        while (!kembali) {
            cout << "\n--- Manajemen Data Graf ---\n";
            cout << "1. Tambah Lokasi\n";
            cout << "2. Hapus Lokasi\n";
            cout << "3. Update Nama Lokasi\n";
            cout << "4. Tambah Rute\n";
            cout << "5. Hapus Rute\n";
            cout << "0. Kembali ke Menu Utama\n";
            cout << "Pilihan: " << flush;
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                cout << "Input tidak valid. Harap masukkan angka.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            switch (pilihan) {
                case 1: prosesTambahLokasi(); break;
                case 2: prosesHapusLokasi(); break;
                case 3: prosesUpdateLokasi(); break;
                case 4: prosesTambahRute(); break;
                case 5: prosesHapusRute(); break;
                case 0: kembali = true; break;
                default: cout << "Pilihan tidak valid.\n";
            }
        }
    }
    
    // --- FITUR BARU: Fungsi untuk menangani pemuatan file ---
    void prosesMuatDataDariFile() {
        cout << "\n--- Muat Data dari File CSV ---\n";
        cout << "PERINGATAN: Data graf yang ada saat ini akan dihapus.\n";
        
        string namaFileLokasi, namaFileRute;
        cout << "Masukkan nama file lokasi (contoh: lokasi_kampus.csv): ";
        cin >> namaFileLokasi;
        
        cout << "Masukkan nama file rute (contoh: rute_kampus.csv): ";
        cin >> namaFileRute;

        graf.bersihkanGraf(); // Bersihkan data lama
        bool sukses = manajerFile.muatDariCSV(graf, namaFileLokasi, namaFileRute);

        if (sukses) {
            cout << "SUCCESS: Data baru dari '" << namaFileLokasi << "' dan '" << namaFileRute << "' berhasil dimuat.\n";
        } else {
            cout << "FAIL: Gagal memuat data dari file. Pastikan nama file benar dan file ada di direktori yang sama.\n";
        }
    }

public:
    void jalankan() {
        // Hapus pemuatan data otomatis saat start agar bisa dikontrol pengguna
        // manajerFile.muatDariCSV(graf, "lokasi.csv", "rute.csv");
        
        bool berjalan = true;
        while(berjalan) {
            cout << "\n===== Sistem Rekomendasi Rute Transportasi =====\n";
            cout << "1. Cari Rute Terbaik\n";
            cout << "2. Tampilkan Peta Rute (Graf)\n";
            cout << "3. Kelola Lokasi & Rute (CRUD)\n";
            cout << "4. Simpan Data ke File\n";
            cout << "5. Muat Data dari File\n";
            cout << "0. Keluar\n";
            cout << "Pilihan: " << flush;
            
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                cout << "Input tidak valid. Harap masukkan angka.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            switch(pilihan) {
                case 1: menuCariRute(); break;
                case 2: graf.tampilkanGraf(); break;
                case 3: menuManajemenData(); break;
                case 4: manajerFile.simpanKeCSV(graf, "lokasi.csv", "rute.csv"); break;
                // --- PERBAIKAN: Hubungkan ke fungsi baru ---
                case 5: prosesMuatDataDariFile(); break;
                case 0: berjalan = false; break;
                default: cout << "Pilihan tidak valid.\n";
            }
        }
        cout << "Terima kasih!\n";
    }
};

// ===================================================================
// FUNGSI UTAMA (MAIN)
// ===================================================================
int main() {
    // Optimasi ini dinonaktifkan untuk memastikan kompatibilitas
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL);

    Sistem aplikasi;
    aplikasi.jalankan();

    return 0;
}
