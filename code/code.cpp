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

// ===================================================================
// KELAS 1: LOKASI
// ===================================================================
class Lokasi {
public:
    int id;
    std::string nama;
    Lokasi(int id = 0, const std::string& nama = "") : id(id), nama(nama) {}
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

    double getBobot(const std::string& preferensi) const {
        if (preferensi == "waktu") return waktu;
        if (preferensi == "biaya") return biaya;
        if (preferensi == "jarak") return jarak;
        return std::numeric_limits<double>::infinity();
    }
};


// ===================================================================
// KELAS 3: GRAF (VERSI LENGKAP DENGAN FUNGSI CRUD)
// ===================================================================
class Graf {
private:
    std::unordered_map<int, Lokasi> daftarLokasi;
    std::unordered_map<int, std::vector<Rute>> adjacencyList;
    int nextLokasiId = 1;

public:
    // --- FITUR BARU: Membersihkan graf ---
    void bersihkanGraf() {
        daftarLokasi.clear();
        adjacencyList.clear();
        nextLokasiId = 1;
        std::cout << "INFO: Graf telah dibersihkan.\n";
    }

    // --- CREATE ---
    void tambahLokasi(const std::string& nama) {
        int idBaru = nextLokasiId++;
        daftarLokasi[idBaru] = Lokasi(idBaru, nama);
        adjacencyList[idBaru] = {};
        std::cout << "SUCCESS: Lokasi '" << nama << "' dengan ID " << idBaru << " berhasil ditambahkan.\n";
    }

    void tambahRute(int idAwal, int idTujuan, double jarak, double waktu, double biaya) {
        if (daftarLokasi.find(idAwal) == daftarLokasi.end() || daftarLokasi.find(idTujuan) == daftarLokasi.end()) {
            std::cout << "ERROR: Salah satu atau kedua ID lokasi tidak valid untuk menambah rute.\n";
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
    const std::vector<Rute>* getTetangga(int id) const {
        if (adjacencyList.count(id)) return &adjacencyList.at(id);
        return nullptr;
    }
    const std::unordered_map<int, Lokasi>& getSemuaLokasi() const {
        return daftarLokasi;
    }
    const std::unordered_map<int, std::vector<Rute>>& getAdjacencyList() const {
        return adjacencyList;
    }
    void tampilkanGraf() const {
        std::cout << "\n--- Visualisasi Graf Transportasi ---\n";
        if (daftarLokasi.empty()) {
            std::cout << "Graf masih kosong.\n";
        }
        for (const auto& pair : daftarLokasi) {
            const Lokasi& lokAwal = pair.second;
            std::cout << "[" << lokAwal.id << ": " << lokAwal.nama << "] terhubung ke:\n";
            
            const auto it = adjacencyList.find(lokAwal.id);
            if (it == adjacencyList.end() || it->second.empty()) {
                std::cout << "  (Tidak ada rute keluar)\n";
            } else {
                for (const Rute& rute : it->second) {
                    const Lokasi* lokTujuan = getLokasi(rute.idLokasiTujuan);
                    if (!lokTujuan) continue;
                    std::cout << "  -> [" << lokTujuan->id << ": " << lokTujuan->nama << "] "
                              << "(Jarak: " << rute.jarak << " km, "
                              << "Waktu: " << rute.waktu << " mnt, "
                              << "Biaya: Rp" << rute.biaya << ")\n";
                }
            }
        }
        std::cout << "-------------------------------------\n";
    }

    // --- UPDATE ---
    void updateLokasi(int id, const std::string& namaBaru) {
        if (daftarLokasi.find(id) != daftarLokasi.end()) {
            daftarLokasi.at(id).nama = namaBaru;
            std::cout << "SUCCESS: Lokasi ID " << id << " berhasil diperbarui menjadi '" << namaBaru << "'.\n";
        } else {
            std::cout << "ERROR: Lokasi dengan ID " << id << " tidak ditemukan.\n";
        }
    }

    // --- DELETE ---
    bool hapusLokasi(int id) {
        if (daftarLokasi.find(id) == daftarLokasi.end()) {
            std::cout << "ERROR: Lokasi dengan ID " << id << " tidak ditemukan.\n";
            return false;
        }
        std::string namaLokasi = daftarLokasi.at(id).nama;
        daftarLokasi.erase(id);
        adjacencyList.erase(id);
        for (auto& pair : adjacencyList) {
            auto& ruteVector = pair.second;
            ruteVector.erase(
                std::remove_if(ruteVector.begin(), ruteVector.end(),
                               [id](const Rute& rute) { return rute.idLokasiTujuan == id; }),
                ruteVector.end()
            );
        }
        std::cout << "SUCCESS: Lokasi '" << namaLokasi << "' (ID: " << id << ") dan semua rute terkait telah dihapus.\n";
        return true;
    }

    bool hapusRute(int idAwal, int idTujuan) {
        if (adjacencyList.find(idAwal) == adjacencyList.end()) {
            std::cout << "ERROR: Lokasi awal dengan ID " << idAwal << " tidak ditemukan.\n";
            return false;
        }
        auto& ruteVector = adjacencyList.at(idAwal);
        size_t ukuranAwal = ruteVector.size();
        ruteVector.erase(
            std::remove_if(ruteVector.begin(), ruteVector.end(),
                           [idTujuan](const Rute& rute) { return rute.idLokasiTujuan == idTujuan; }),
            ruteVector.end()
        );
        if (ruteVector.size() < ukuranAwal) {
            std::cout << "SUCCESS: Rute dari ID " << idAwal << " ke " << idTujuan << " berhasil dihapus.\n";
            return true;
        } else {
            std::cout << "ERROR: Rute dari ID " << idAwal << " ke " << idTujuan << " tidak ditemukan.\n";
            return false;
        }
    }

    // --- Helper functions ---
    void muatLokasi(int id, const std::string& nama) {
        if (daftarLokasi.find(id) == daftarLokasi.end()) {
            daftarLokasi[id] = Lokasi(id, nama);
            adjacencyList[id] = {};
        }
    }
    void setNextId(int id) { nextLokasiId = id; }
};

// ... (Kelas PencariRute dan PohonPreferensi tidak berubah) ...
// (Kelas ManajerFile juga tidak berubah)
// ===================================================================
// KELAS 4: PENCARI RUTE (Tidak ada perubahan)
// ===================================================================
struct HasilRute {
    std::vector<int> path;
    double totalJarak = 0.0;
    double totalWaktu = 0.0;
    double totalBiaya = 0.0;
    bool ditemukan = false;
};

class PencariRute {
public:
    HasilRute cariRuteTerpendek(const Graf& graf, int idAwal, int idTujuan, const std::string& preferensi) {
        HasilRute hasil;
        if (!graf.getLokasi(idAwal) || !graf.getLokasi(idTujuan)) {
            // Pesan error sudah ditangani di fungsi menuCariRute
            return hasil;
        }
        using Pair = std::pair<double, int>;
        std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
        std::unordered_map<int, double> bobotTotal;
        std::unordered_map<int, int> pendahulu;
        for (const auto& pair : graf.getSemuaLokasi()) {
            bobotTotal[pair.first] = std::numeric_limits<double>::infinity();
        }
        bobotTotal[idAwal] = 0.0;
        pq.push({0.0, idAwal});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (u == idTujuan) break;
            const std::vector<Rute>* tetangga = graf.getTetangga(u);
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
        std::reverse(hasil.path.begin(), hasil.path.end());
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
    std::string jalankanPohon() {
        std::cout << "\nSilakan pilih preferensi rute Anda:\n";
        std::cout << "1. Waktu Tercepat\n";
        std::cout << "2. Biaya Termurah\n";
        std::cout << "3. Jarak Terpendek\n";
        std::cout << "Pilihan Anda: " << std::flush;
        int pilihan;
        std::cin >> pilihan;
        switch(pilihan) {
            case 1: return "waktu";
            case 2: return "biaya";
            case 3: return "jarak";
            default:
                std::cout << "Pilihan tidak valid. Menggunakan preferensi default (Waktu).\n";
                return "waktu";
        }
    }
};


// ===================================================================
// KELAS 6: MANAJER FILE (Tidak ada perubahan)
// ===================================================================
class ManajerFile {
public:
    bool muatDariCSV(Graf& graf, const std::string& fileLokasi, const std::string& fileRute) {
        std::ifstream fileLok(fileLokasi);
        if (!fileLok.is_open()) {
            std::cout << "ERROR: File lokasi '" << fileLokasi << "' tidak dapat dibuka.\n";
            return false;
        }
        std::string line;
        std::getline(fileLok, line); // Lewati header
        int maxId = 0;
        while (std::getline(fileLok, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string idStr, nama;
            std::getline(ss, idStr, ',');
            std::getline(ss, nama);
            int id = std::stoi(idStr);
            graf.muatLokasi(id, nama);
            if (id > maxId) maxId = id;
        }
        graf.setNextId(maxId + 1);
        fileLok.close();
        std::ifstream fileRut(fileRute);
        if (!fileRut.is_open()) {
            std::cout << "ERROR: File rute '" << fileRute << "' tidak dapat dibuka.\n";
            return false;
        }
        std::getline(fileRut, line); // Lewati header
        while (std::getline(fileRut, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string idAwal, idTujuan, jarak, waktu, biaya;
            std::getline(ss, idAwal, ',');
            std::getline(ss, idTujuan, ',');
            std::getline(ss, jarak, ',');
            std::getline(ss, waktu, ',');
            std::getline(ss, biaya, ',');
            graf.tambahRute(std::stoi(idAwal), std::stoi(idTujuan), std::stod(jarak), std::stod(waktu), std::stod(biaya));
        }
        fileRut.close();
        return true;
    }
    
    bool simpanKeCSV(const Graf& graf, const std::string& fileLokasi, const std::string& fileRute) {
        std::ofstream fileLok(fileLokasi);
        if (!fileLok.is_open()) return false;
        fileLok << "id,nama\n";
        for(const auto& pair : graf.getSemuaLokasi()){
            fileLok << pair.second.id << "," << pair.second.nama << "\n";
        }
        fileLok.close();
        std::ofstream fileRut(fileRute);
        if(!fileRut.is_open()) return false;
        fileRut << "id_awal,id_tujuan,jarak,waktu,biaya\n";
        for(const auto& pair : graf.getAdjacencyList()){
            int idAwal = pair.first;
            for(const auto& rute : pair.second){
                fileRut << idAwal << "," << rute.idLokasiTujuan << "," << rute.jarak << "," << rute.waktu << "," << rute.biaya << "\n";
            }
        }
        fileRut.close();
        std::cout << "SUCCESS: Data berhasil disimpan ke file '" << fileLokasi << "' dan '" << fileRute << "'.\n";
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
        std::cout << "\n--- Cari Rute ---\n";
        graf.tampilkanGraf();
        if (graf.getSemuaLokasi().empty()) {
            std::cout << "Graf masih kosong. Silakan tambah lokasi dan rute terlebih dahulu.\n";
            return;
        }
        std::cout << "Masukkan ID Lokasi Awal: ";
        int idAwal;
        std::cin >> idAwal;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        std::cout << "Masukkan ID Lokasi Tujuan: ";
        int idTujuan;
        std::cin >> idTujuan;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        std::string preferensi = pohonPreferensi.jalankanPohon();
        HasilRute hasil = pencari.cariRuteTerpendek(graf, idAwal, idTujuan, preferensi);
        if (hasil.ditemukan) {
            std::cout << "\n--- HASIL REKOMENDASI RUTE ---\n";
            std::cout << "Preferensi: " << preferensi << std::endl;
            std::cout << "Rute: ";
            for (size_t i = 0; i < hasil.path.size(); ++i) {
                const Lokasi* lok = graf.getLokasi(hasil.path[i]);
                if (lok) {
                    std::cout << lok->nama << (i == hasil.path.size() - 1 ? "" : " -> ");
                }
            }
            std::cout << "\n\nDetail Perjalanan:\n";
            std::cout << "  - Total Jarak: " << hasil.totalJarak << " km\n";
            std::cout << "  - Total Waktu: " << hasil.totalWaktu << " menit\n";
            std::cout << "  - Total Biaya: Rp" << hasil.totalBiaya << "\n";
            std::cout << "---------------------------------\n";
        } else {
            const Lokasi* lokAwal = graf.getLokasi(idAwal);
            const Lokasi* lokTujuan = graf.getLokasi(idTujuan);
            if (lokAwal && lokTujuan) {
                std::cout << "\nMaaf, tidak ditemukan rute dari " << lokAwal->nama << " ke " << lokTujuan->nama << ".\n";
            } else {
                std::cout << "\nMaaf, ID lokasi awal atau tujuan tidak valid.\n";
            }
        }
    }
    
    void prosesTambahLokasi() {
        std::cout << "\n-- Tambah Lokasi Baru --\n";
        std::cout << "Masukkan nama lokasi: ";
        std::string nama;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, nama);
        graf.tambahLokasi(nama);
    }

    void prosesHapusLokasi() {
        std::cout << "\n-- Hapus Lokasi --\n";
        graf.tampilkanGraf();
        std::cout << "Masukkan ID lokasi yang akan dihapus: ";
        int id;
        std::cin >> id;
        graf.hapusLokasi(id);
    }

    void prosesUpdateLokasi() {
        std::cout << "\n-- Update Nama Lokasi --\n";
        graf.tampilkanGraf();
        std::cout << "Masukkan ID lokasi yang akan diupdate: ";
        int id;
        std::cin >> id;
        std::cout << "Masukkan nama baru untuk lokasi ID " << id << ": ";
        std::string namaBaru;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, namaBaru);
        graf.updateLokasi(id, namaBaru);
    }

    void prosesTambahRute() {
        std::cout << "\n-- Tambah Rute Baru --\n";
        graf.tampilkanGraf();
        int idAwal, idTujuan;
        double jarak, waktu, biaya;
        std::cout << "Masukkan ID lokasi awal: ";
        std::cin >> idAwal;
        std::cout << "Masukkan ID lokasi tujuan: ";
        std::cin >> idTujuan;
        std::cout << "Masukkan jarak (km): ";
        std::cin >> jarak;
        std::cout << "Masukkan waktu (menit): ";
        std::cin >> waktu;
        std::cout << "Masukkan biaya (Rp): ";
        std::cin >> biaya;
        graf.tambahRute(idAwal, idTujuan, jarak, waktu, biaya);
        std::cout << "SUCCESS: Rute dari " << idAwal << " ke " << idTujuan << " berhasil ditambahkan.\n";
    }

    void prosesHapusRute() {
        std::cout << "\n-- Hapus Rute --\n";
        graf.tampilkanGraf();
        int idAwal, idTujuan;
        std::cout << "Masukkan ID lokasi awal rute: ";
        std::cin >> idAwal;
        std::cout << "Masukkan ID lokasi tujuan rute: ";
        std::cin >> idTujuan;
        graf.hapusRute(idAwal, idTujuan);
    }

    void menuManajemenData() {
        bool kembali = false;
        while (!kembali) {
            std::cout << "\n--- Manajemen Data Graf ---\n";
            std::cout << "1. Tambah Lokasi\n";
            std::cout << "2. Hapus Lokasi\n";
            std::cout << "3. Update Nama Lokasi\n";
            std::cout << "4. Tambah Rute\n";
            std::cout << "5. Hapus Rute\n";
            std::cout << "0. Kembali ke Menu Utama\n";
            std::cout << "Pilihan: " << std::flush;
            int pilihan;
            std::cin >> pilihan;
            if (std::cin.fail()) {
                std::cout << "Input tidak valid. Harap masukkan angka.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            switch (pilihan) {
                case 1: prosesTambahLokasi(); break;
                case 2: prosesHapusLokasi(); break;
                case 3: prosesUpdateLokasi(); break;
                case 4: prosesTambahRute(); break;
                case 5: prosesHapusRute(); break;
                case 0: kembali = true; break;
                default: std::cout << "Pilihan tidak valid.\n";
            }
        }
    }
    
    // --- FITUR BARU: Fungsi untuk menangani pemuatan file ---
    void prosesMuatDataDariFile() {
        std::cout << "\n--- Muat Data dari File CSV ---\n";
        std::cout << "PERINGATAN: Data graf yang ada saat ini akan dihapus.\n";
        
        std::string namaFileLokasi, namaFileRute;
        std::cout << "Masukkan nama file lokasi (contoh: lokasi_kampus.csv): ";
        std::cin >> namaFileLokasi;
        
        std::cout << "Masukkan nama file rute (contoh: rute_kampus.csv): ";
        std::cin >> namaFileRute;

        graf.bersihkanGraf(); // Bersihkan data lama
        bool sukses = manajerFile.muatDariCSV(graf, namaFileLokasi, namaFileRute);

        if (sukses) {
            std::cout << "SUCCESS: Data baru dari '" << namaFileLokasi << "' dan '" << namaFileRute << "' berhasil dimuat.\n";
        } else {
            std::cout << "FAIL: Gagal memuat data dari file. Pastikan nama file benar dan file ada di direktori yang sama.\n";
        }
    }

public:
    void jalankan() {
        // Hapus pemuatan data otomatis saat start agar bisa dikontrol pengguna
        // manajerFile.muatDariCSV(graf, "lokasi.csv", "rute.csv");
        
        bool berjalan = true;
        while(berjalan) {
            std::cout << "\n===== Sistem Rekomendasi Rute Transportasi =====\n";
            std::cout << "1. Cari Rute Terbaik\n";
            std::cout << "2. Tampilkan Peta Rute (Graf)\n";
            std::cout << "3. Kelola Lokasi & Rute (CRUD)\n";
            std::cout << "4. Simpan Data ke File\n";
            std::cout << "5. Muat Data dari File\n";
            std::cout << "0. Keluar\n";
            std::cout << "Pilihan: " << std::flush;
            
            int pilihan;
            std::cin >> pilihan;
            if (std::cin.fail()) {
                std::cout << "Input tidak valid. Harap masukkan angka.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                default: std::cout << "Pilihan tidak valid.\n";
            }
        }
        std::cout << "Terima kasih!\n";
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
