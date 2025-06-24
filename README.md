# FP-STRUKDAT-EAS-4


| Nama  | NRP |
| ------------- | ------------- |
| Ahmad Idza Anafin  | 5027241017 |
| Zein Muhammad Hasan  | 5027241035 |
| Raya Ahmad Syarif  | 5027241041  |
| Aslam Ahmad Usman |  5027241074  |
| Mutiara Diva Jaladitha  | 5027241083  |
| Tiara Fatimah Azzahra  |  5027241090 |

<ul>
  <li><a href="#bagian1">1. Pengantar Library dan Implementasi Kelas Rute</a></li>
  <li><a href="#bagian2">2. kelas graft dan fungsi-fungsinya (1)</a></li>
  <li><a href="#bagian3">3. kelas graft dan fungsi-fungsinya (2)</a></li>
  <li><a href="#bagian4">4. Class Algoritma Pencarian Rute (Dijkstra)</a></li>
  <li><a href="#bagian5">5. Kelas Pohon Preferensi & File Manager</a></li>
  <li><a href="#bagian6">6. Kelas Sistem dan main function</a></li>
  <li><a href="#bagian7">7. Demonstrasi dan output</a></li>
</ul>

<hr>

<h2 id="bagian1">1. Pengantar Library dan Implementasi Kelas Rute</h2>
<!-- Konten bagian 1 -->
Tujuan Pembelajaran
Pada bagian ini:

- Memahami library (pustaka) apa saja yang digunakan dalam program dan peranannya.
- Mengenal konsep dan implementasi kelas Rute untuk merepresentasikan satu jalur dalam graf.

<h3>Library dalam C++</h3>
Library dalam pemrograman adalah Kumpulan kode (fungsi, kelas, modul) yang sudah ditulis dan dikemas agar bisa langsung digunakan di program kita.

```
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
```
```

Penjelasan Tiap Library
| **Library**       | **Kegunaan dalam Kode Implementasinya**                                                                                                                                  | **Contoh dari Kode Implementasinya**                                                                                                            |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `<iostream>`      | Digunakan untuk mencetak teks ke layar dan membaca input dari user.                                                                                           | `cout << "INFO: Graf telah dibersihkan.\n";`                                                                                         |
| `<string>`        | Digunakan untuk memanipulasi teks, misalnya nama lokasi.                                                                                                      | `class Lokasi { string nama; }` dan `getline(cin, nama);`                                                                            |
| `<vector>`        | Digunakan untuk membuat list dinamis seperti `adjacencyList` dan path rute.                                                                                   | `unordered_map<int, vector<Rute>> adjacencyList;` dan `vector<int> path;`                                                            |
| `<unordered_map>` | Digunakan untuk membuat dictionary agar bisa cari lokasi cepat berdasarkan `ID`.                                                                              | `unordered_map<int, Lokasi> daftarLokasi;`                                                                                           |
| `<map>`           | Digunakan untuk membuat dictionary berurut (jika mau urut berdasarkan ID). Tidak banyak dipakai di kode ini, tapi sudah di-include untuk fitur di masa depan. | Tidak digunakan langsung di kode saat ini.                                                                                           |
| `<queue>`         | Digunakan untuk antrian prioritas dalam Dijkstra.                                                                                                             | `priority_queue<Pair, vector<Pair>, greater<Pair>> pq;` di `PencariRute::cariRuteTerpendek()`                                        |
| `<limits>`        | Digunakan untuk membuat nilai infinity sebagai jarak awal.                                                                                                    | `numeric_limits<double>::infinity()` di `PencariRute::cariRuteTerpendek()`                                                           |
| `<algorithm>`     | Digunakan untuk memanipulasi list seperti hapus dan balik urutan list.                                                                                        | `remove_if(...)` di `Graf::hapusLokasi()` dan `reverse(hasil.path.begin(), hasil.path.end());` di `PencariRute::cariRuteTerpendek()` |
| `<fstream>`       | Digunakan untuk membaca dan menulis file CSV.                                                                                                                 | `ifstream fileLok(fileLokasi); ofstream fileRut(fileRute);` di `ManajerFile`                                                         |
| `<sstream>`       | Digunakan untuk memecah teks CSV menjadi bagian-bagian.                                                                                                       | `stringstream ss(line); getline(ss, idStr, ',');` di `ManajerFile::muatDariCSV()`                                                    |
| `<utility>`       | Digunakan untuk membuat pasangan nilai, berguna dalam queue prioritas Dijkstra.                                                                               | `using Pair = pair<double, int>; pq.push({0.0, idAwal});` di `PencariRute::cariRuteTerpendek()`                                      |
```

<h3> Kelas Rute</h3>
Kelas Rute merepresentasikan edge dalam graf transportasi — sebuah jalur dari satu lokasi ke lokasi lainnya.

Penjelasan Kelas Rute Berdasarkan Implementasi Kode 
```
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
```
Penjelasan Baris per Baris
1. Deklarasi dan Atribut
```
class Rute {
public:
    int idLokasiTujuan;
    double jarak;
    double waktu;
    double biaya;
```
Apa fungsinya?

idLokasiTujuan: ID lokasi tujuan dari rute ini.
→ Contohnya, kalau Anda membuat rute dari Lokasi 1 ke Lokasi 2, maka idLokasiTujuan = 2.
jarak: Menyimpan jarak tempuh rute dalam satuan km.
→ Digunakan kalau Anda mau mencari rute terpendek.
waktu: Menyimpan durasi perjalanan dalam menit.
→ Digunakan kalau Anda mau mencari rute tercepat.
biaya: Menyimpan ongkos perjalanan dalam rupiah.
→ Digunakan kalau Anda mau mencari rute termurah.

2. Konstruktor
```
    Rute(int tujuan, double jarak, double waktu, double biaya)
        : idLokasiTujuan(tujuan), jarak(jarak), waktu(waktu), biaya(biaya) {}
```
Apa fungsinya?
Konstruktor ini membuat objek Rute baru.

3. getBobot() — Memilih Bobot Berdasarkan Preferensi
```
    double getBobot(const string& preferensi) const {
        if (preferensi == "waktu") return waktu;
        if (preferensi == "biaya") return biaya;
        if (preferensi == "jarak") return jarak;
        return numeric_limits<double>::infinity();
    }
```
Apa fungsinya?
Fungsi getBobot() digunakan untuk:

Memilih nilai bobot yang mau dihitung saat mencari rute, sesuai preferensi pengguna.

Kalau preferensinya:
- "waktu" → kembalikan waktu.
- "biaya" → kembalikan biaya.
- "jarak" → kembalikan jarak.

Kesimpulan untuk Kelas Rute
Kelas Rute adalah struktur data utama untuk merepresentasikan satu edge (jalur) dalam graf:
- Anda bisa tahu menuju ke mana rute ini (idLokasiTujuan).
- Anda bisa tahu atribut-atributnya (jarak, waktu, biaya).
- Anda bisa memilih bobot sesuai kebutuhan lewat getBobot() — berguna banget dalam algoritma Dijkstra.

<h2 id="bagian2">2.  kelas graft dan fungsi-fungsinya (1) </h2>

Bagian ini akan menjelaskan kelas dalam graf beserta fungsi-fungsi didalamnya. 

Bagian pertama dimulai dari member-member yang terdapat dalam `private`, dimana terdapat daftar lokasi, graf, dan id  unik untuk nextLokasi agar dapat ditambah otomatis saat lokasi baru ditambah.

```c
class Graf {
private:
    unordered_map<int, Lokasi> daftarLokasi;
    unordered_map<int, vector<Rute>> adjacencyList;
    int nextLokasiId = 1;
```

Selanjutnya terdapat fitur `bersihkanGraf()` untuk membersihkan data dalam graf. Fungsi ini akan menghapus data berupa lokasi, rute, serta mengembalikan `nextLokasiId` ke nilai semula.

```c
void bersihkanGraf() {
    daftarLokasi.clear();
    adjacencyList.clear();
    nextLokasiId = 1;
    cout << "INFO: Graf telah dibersihkan.\n";
}
```

<h3>1. Fitur Create</h3>

Fitur yang ada pada create digunakan untuk menambahkan data-data yang diperlukan pada graf, seperti lokasi dan rute.

<h3>1.1  Menambah Lokasi</h3>

Fitur `tambahLokasi()` digunakan untuk menambah objek lokasi baru pada graf. Pertama ia akan membuat id unik yang baru untuk lokasi, lalu menyimpannya di `daftarLokasi[idBaru]` bersama dengan lokasi baru. Sementara itu `adjacencyList[idBaru]` dikosongkan agar tidak terjadi error pada saat menambah rute keluar.

```c
void tambahLokasi(const string& nama) {
    int idBaru = nextLokasiId++;
    daftarLokasi[idBaru] = Lokasi(idBaru, nama);
    adjacencyList[idBaru] = {};
    cout << "SUCCESS: Lokasi '" << nama << "' dengan ID " << idBaru << " berhasil ditambahkan.\n";
}
```

<h3>1.2 Menambah Rute</h3>

fitur `tambahRute()` digunakan untuk menambah rute baru ke adjencyList. Pertama ia akan memeriksa apakah kedua id lokasi sudah valid atau belum. Jika valid, maka fungsi akan menambahkan rute pada `adjacencyList[idAwal]` dengan `emplace_back`.

```c
void tambahRute(int idAwal, int idTujuan, double jarak, double waktu, double biaya) {
        if (daftarLokasi.find(idAwal) == daftarLokasi.end() || daftarLokasi.find(idTujuan) == daftarLokasi.end()) {
            cout << "ERROR: Salah satu atau kedua ID lokasi tidak valid untuk menambah rute.\n";
            return;
        }
        adjacencyList[idAwal].emplace_back(idTujuan, jarak, waktu, biaya);
}
```

<h3>2. Fitur Read</h3>

Fitur pada read yang digunakan untuk melihat data-data yang sudah dimasukan sebelumnya.

<h3>2.1  Mengambil Lokasi dan Rute</h3>

Fungsi `getLokasi()` dan `getTetangga()` digunakan untuk mengambil pointer ke objek lokasi dan rute berdasarkan id masing-masing objek. 

```c
const Lokasi* getLokasi(int id) const {
        if (daftarLokasi.count(id)) return &daftarLokasi.at(id);
        return nullptr;
    }
const vector<Rute>* getTetangga(int id) const {
        if (adjacencyList.count(id)) return &adjacencyList.at(id);
        return nullptr;
    }
```

<h3>2.2  Memberi Akses Lokasi dan Rute</h3>

Fungsi `getSemuaLokasi()` dan `getAdjacencyList()` digunakan untuk memberi akses untuk melihat keseluruhan isi graf: Lokasi dan rute.

```c
const unordered_map<int, Lokasi>& getSemuaLokasi() const {
        return daftarLokasi;
    }
const unordered_map<int, vector<Rute>>& getAdjacencyList() const {
        return adjacencyList;
    }
```

<h3>2.3  Menampilkan graf</h3>

Fungsi `void tampilkanGraf()` digunakan untuk menampilkan isi dari graf. Berikut fungsinya:

```c
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
```

Mekanisme dari fungsi `tampilkanGraf()` adalah sebagai berikut:

<ul>
  <li>
Pertama fungsi akan melihat apakah daftarLokasi masih kosong atau tidak.
  </li>
<li>
Selanjutnya fungsi akan menampilkan setiap dari lokasi dan semua rute keluarnya dengan cara loop ke setiap lokasi, dan menyimpannya ke `lokAwal`.
</li>
<li>
Fungsi lalu akan mengecek apakah lokasi memiliki rute keluar atau tidak. Jika lokasi memiliki rute keluar, maka fungsi akan melakukan loop ke setiap rute. Pertama loop akan akan mengambil lokasi tujuan berdasarkan `rute.idLokasiTujuan`, lalu menampilkan jarak, waktu, dan biaya dari rute tersebut.
</li>
</ul>

<h2 id="bagian3">3.  kelas graft dan fungsi-fungsinya (2) </h2>

<h3>3. Fitur Update</h3>

Tujuan: Mengganti nama lokasi berdasarkan ID-nya.

```
    void updateLokasi(int id, const string& namaBaru) {
        if (daftarLokasi.find(id) != daftarLokasi.end()) {
            daftarLokasi.at(id).nama = namaBaru;
            cout << "SUCCESS: Lokasi ID " << id << " berhasil diperbarui menjadi '" << namaBaru << "'.\n";
        } else {
            cout << "ERROR: Lokasi dengan ID " << id << " tidak ditemukan.\n";
        }
    }
```
- `if (daftarLokasi.find(id) != daftarLokasi.end())` : Mengecek apakah ID lokasi ada di `daftarLokasi`.
- `daftarLokasi.at(id).nama = namaBaru;` : Jika ada, ganti nama lokasinya.
- `cout << "SUCCESS: Lokasi ID " << id << " berhasil diperbarui...";` : Output keberhasilan atau error jika tidak ditemukan.

<h3>4. Fitur Delete</h3>

<h3>4.1  hapusLokasi</h3>

Tujuan: Menghapus lokasi beserta semua rute yang menuju ke/dari lokasi itu.

```
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
```
- `if (daftarLokasi.find(id) == daftarLokasi.end())` : Cek apakah ID ada. Kalau tidak, cetak error dan return `false`.
- `string namaLokasi = daftarLokasi.at(id).nama; daftarLokasi.erase(id); adjacencyList.erase(id);` : Hapus dari `daftarLokasi` dan semua rute keluar dari lokasi itu.
- `for (auto& pair : adjacencyList) {...}` : Hapus semua rute menuju ke ID ini dari semua lokasi lain.
- `cout << "SUCCESS: Lokasi '" << namaLokasi << "'... dan semua rute terkait telah dihapus.";` : Cetak keberhasilan.

<h3>4.2  hapusRute</h3>

Tujuan: Menghapus satu rute spesifik dari `idAwal` ke `idTujuan`.

```
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
```
- `if (adjacencyList.find(idAwal) == adjacencyList.end())` : Cek apakah lokasi awal valid.
- `auto& ruteVector = adjacencyList.at(idAwal); size_t ukuranAwal = ruteVector.size();` : Ambil daftar rute dari lokasi awal.
- `ruteVector.erase(...);` : Hapus semua elemen `rute` yang menuju ke `idTujuan`.
- `if (ruteVector.size() < ukuranAwal)` : Jika ukuran vektor berubah → berarti ada yang terhapus → tampilkan pesan sukses.

<h3>5. Helper Functions</h3>

<h3>5.1  muatLokasi</h3>

Tujuan: Digunakan untuk menambahkan lokasi baru (misal saat load data dari file).

```
void muatLokasi(int id, const string& nama) {
        if (daftarLokasi.find(id) == daftarLokasi.end()) {
            daftarLokasi[id] = Lokasi(id, nama);
            adjacencyList[id] = {};
        }
    }
```
- `if (daftarLokasi.find(id) == daftarLokasi.end()) {` : Cek dulu apakah ID sudah ada.
- `daftarLokasi[id] = Lokasi(id, nama); adjacencyList[id] = {};` : Tambahkan ke daftar lokasi dan siapkan slot adjacency-nya.

<h3>5.2  setNextId</h3>

Tujuan: Set nilai `nextLokasiId`, misal saat membaca data dari file agar ID berikutnya tidak bentrok.

```
void setNextId(int id) { nextLokasiId = id; }
```


<h2 id="bagian4">4. Class Algoritma Pencarian Rute (Dijkstra)</h2>
Poin ini menjelaskan implementasi salah satu algoritma pencarian rute Dijkstra pada CPP dengan menerapkan Object Oriented Programming (OOP). Pencarian rute ini memilih rute terpendek dan terbaik dimana berdasarkan beberapa parameter yaitu waktu, jarak, biaya dalam bentuk graf.

<h3>1. Alasan Menggunakan Dijkstra</h3>
  Dijkstra dipilih karena:
  
  1. Cocok untuk graf berbobot non-negatif seperti jarak, biaya, atau waktu.
  
  2. Memberikan solusi paling optimal dari satu titik ke semua titik lainnya.
     
  3. Bersifat deterministik, hasilnya pasti jika input sama.
     
  4. Waktu eksekusi efisien dengan struktur seperti priority queue (heap).

<h3>2. Komponen Utama</h3>

   1. Struktur Data Graf
      
   2. Class rute
      
   3. Rute finder
      
   4. struct hasil rute
      
<h3>3. Kekurangan</h3>

  1. Tidak mendukung bobot negatif (gunakan Bellman-Ford untuk itu).

  2. Proses bisa lambat untuk graf besar tanpa heap atau optimasi.

  3. Hanya cocok untuk kasus single-source shortest path.
<h3>4. Penjelasan kode</h3>
<pre>struct HasilRute {
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
};</pre>

<h2 id="bagian5">5. Class PreferredTree dan ManajerFile</h2>
<!-- Konten bagian 5 -->
Bab ini merinci bagian PreferredTree yang bertugas untuk memberikan output pada user berdasarkan rute yang dipilih. Terdapat tiga opsi rute yang dapat dipilih yaitu berdasarkan waktu tercepat, biaya termurah, dan jarak terpendek. Selain PreferredTree terdapat juga ManajerFile yang bertanggung jawab untuk membaca file CSV di dalam direktori code.  
<br>
<br>
<h3> 1. Analisis PreferredTree </h3>  
Class PreferredTree terdapat function JalankanPohon() yang dapat mengembalikan output berupa rute yang dapat dipilih berdasarkan input user.   

```
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
```
Function JalankanPohon menggunakan switch case yang mana akan mengembalikan nilai berupa waktu apabila user menginput 1, biaya apabila user menginput 2, dan jarak apabila user menginput 3. Namun, apabila user memberikan input yang tidak sesuai akan mengembalikan waktu karena telah diset secara default.

<h3> 2. Analisis ManajerFile </h3>  
Class ini bertugas untuk membaca dan menyimpan data Graph dari file CSV. Terdapat beberapa function yaitu MuatDariCSV dan simpanKeCSV. MuatDariCSV bertugas untuk membaca file lokasi dan rute dari CSV untuk mengisi struktur graph dan simpanKeCSV yang dapat menyimpan data Graph ke File CSV, satu untuk lokasi dan satu untuk rute.  
Berikut ini function muatDariCSV:

```
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
```
Dari function di atas dapat dilihat menerima tiga parameter yaitu graf, filelokasi, dan fileRute. fileLokasi digunakan untuk menerima parameter file CSV lokasi yang akan dibaca sedangkan fileRute untuk file CSV rute yang akan dibaca. Di dalam graf terdapat file lokasi-lokasi dan hubungannya yaitu rute. Function ini menggunakan ifstream untuk membaca file CSV kemudian disimpan ke dalam graf.

Lalu, berikut ini merupakan function simpanKeCSV:

```
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
```
Function di atas digunakan untuk menyimpan lokasi dan rute ke dalam file CSV. Cara kerjanya adalah pertama membuka fileLokasi untuk di-input data, menulis id dan nama, lalu mengiterasikan seluruh lokasi dari graf dan ditulis ke dalam file. Hal yang sama terjadi di fileRute juga.

<h2 id="bagian6">6. Kelas Sistem dan main function</h2>
<!-- Konten bagian 6 -->
Bab ini merinci bagian-bagian kode yang bertanggung jawab untuk mengintegrasikan seluruh komponen program, mengelola interaksi dengan pengguna, dan menjalankan alur aplikasi secara keseluruhan. Fokus utama bab ini adalah pada Kelas Sistem yang bertindak sebagai "otak" atau "orkestrator" program, serta fungsi main() sebagai titik awal eksekusi.
<br>
<br>
<h3>1. Analisis Kelas Sistem</h3>
Kelas Sistem adalah pusat kendali dari seluruh aplikasi. Kelas ini tidak melakukan perhitungan algoritma yang rumit (seperti Dijkstra) atau manajemen data level rendah secara langsung. Sebaliknya, tugasnya adalah mendelegasikan pekerjaan ke kelas-kelas lain yang lebih spesifik.
<h3>1.1. Atribut (Private Members)</h3>

```cpp
private:
    Graf graf;
    PencariRute pencari;
    PohonPreferensi pohonPreferensi;
    ManajerFile manajerFile;
```
<ul>
  <li>
    <strong>Sistem:</strong> Di dalam Sistem, kita mendeklarasikan objek dari semua kelas utama lainnya.
  </li>
  <li>
    <strong>Graf graf:</strong> Ini adalah objek untuk menyimpan semua data lokasi dan rute. Sistem akan meminta graf untuk menampilkan data, menambah data, menghapus data, dll.
  </li>
  <li>
    <strong>PencariRute pencari:</strong> Objek ini akan digunakan setiap kali pengguna ingin mencari rute. Sistem akan memberikan data graf, titik awal, tujuan, dan preferensi kepada pencari.
  </li>
  <li>
    <strong>PohonPreferensi pohonPreferensi:</strong> Objek ini digunakan untuk berinteraksi dengan pengguna guna menentukan preferensi rute (waktu, biaya, atau jarak).
  </li>
  <li>
    <strong>ManajerFile manajerFile:</strong> Objek ini dipanggil ketika pengguna ingin memuat data dari file CSV atau menyimpan data ke file CSV.
  </li>
</ul>

<h3>1.2. Fungsi UI (Private Methods)</h3>
Ini adalah fungsi-fungsi "pembantu" yang mengelola setiap menu spesifik. Fungsi-fungsi ini bersifat private karena hanya dipanggil dari dalam Kelas Sistem itu sendiri, bukan dari luar.

```cpp
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
```
<ul>
  <li>
    <strong>Tujuan:</strong> Mengelola seluruh alur untuk fitur pencarian rute.
  </li>
  <li>
    <strong>Langkah-langkah Eksekusi:</strong>
    <ul>
      <li>Menampilkan daftar lokasi saat ini dengan memanggil <code>graf.tampilkanGraf()</code>.</li>
      <li>Meminta pengguna memasukkan ID lokasi awal dan tujuan.</li>
      <li>Melakukan validasi input sederhana untuk memastikan pengguna memasukkan angka.</li>
      <li>Memanggil <code>pohonPreferensi.jalankanPohon()</code> untuk mendapatkan pilihan pengguna (misal, "waktu").</li>
      <li><strong>Poin Kunci:</strong> Memanggil <code>pencari.cariRuteTerpendek(graf, idAwal, idTujuan, preferensi)</code>. Ini adalah momen di mana Sistem mendelegasikan tugas pencarian ke <code>PencariRute</code>.</li>
      <li>Menerima <code>HasilRute</code> dan memeriksa <code>hasil.ditemukan</code>.</li>
      <li>Jika rute ditemukan, menampilkan hasilnya dengan format yang mudah dibaca.</li>
      <li>Jika tidak, menampilkan pesan error yang informatif.</li>
    </ul>
  </li>
</ul>

```cpp
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

```

<ul>
  <li>
    <strong>Fungsi:</strong> <code>prosesTambahLokasi()</code>, <code>prosesHapusLokasi()</code>, <code>prosesUpdateLokasi()</code>, <code>prosesTambahRute()</code>, <code>prosesHapusRute()</code>
    <ul>
      <li><strong>Tujuan:</strong> Menangani operasi CRUD (Create, Read, Update, Delete) untuk data graf.</li>
      <li><strong>Langkah-langkah Eksekusi (Contoh: prosesTambahRute):</strong>
        <ul>
          <li>Menampilkan graf saat ini agar pengguna tahu ID yang valid.</li>
          <li>Meminta input dari pengguna (ID awal, ID tujuan, jarak, waktu, biaya).</li>
          <li><strong>Poin Kunci:</strong> Memanggil fungsi yang sesuai dari objek <code>graf</code>, misalnya <code>graf.tambahRute(idAwal, idTujuan, ...)</code> untuk mendelegasikan tugas penambahan rute.</li>
          <li>Menampilkan pesan sukses.</li>
        </ul>
      </li>
    </ul>
  </li>

  <li>
    <strong>Fungsi:</strong> <code>menuManajemenData()</code>
    <ul>
      <li><strong>Tujuan:</strong> Menyediakan sub-menu khusus untuk operasi CRUD.</li>
      <li><strong>Langkah-langkah Eksekusi:</strong>
        <ul>
          <li>Menjalankan sebuah <code>while</code> loop untuk terus menampilkan menu manajemen data.</li>
          <li>Menggunakan <code>switch</code> statement untuk memanggil fungsi <code>proses...</code> yang sesuai berdasarkan input pengguna (misalnya <code>prosesTambahLokasi()</code>, <code>prosesHapusRute()</code>, dll).</li>
          <li>Loop akan berhenti jika pengguna memilih <code>"0. Kembali ke Menu Utama"</code>.</li>
        </ul>
      </li>
    </ul>
  </li>

  <li>
    <strong>Fungsi:</strong> <code>prosesMuatDataDariFile()</code>
    <ul>
      <li><strong>Tujuan:</strong> Mengelola proses memuat data dari file eksternal.</li>
      <li><strong>Langkah-langkah Eksekusi:</strong>
        <ul>
          <li>Memberi peringatan kepada pengguna bahwa data saat ini akan dihapus.</li>
          <li>Meminta nama file lokasi dan file rute.</li>
          <li>Memanggil <code>graf.bersihkanGraf()</code> untuk mengosongkan data lama.</li>
          <li><strong>Poin Kunci:</strong> Memanggil <code>manajerFile.muatDariCSV(graf, ...)</code> untuk mendelegasikan tugas pembacaan file dan pengisian data ke dalam objek graf.</li>
          <li>Menampilkan pesan sukses atau gagal berdasarkan hasil dari <code>manajerFile</code>.</li>
        </ul>
      </li>
    </ul>
  </li>
</ul>

<h3>1.3. Fungsi jalankan() (Public Method)</h3>
Ini adalah satu-satunya fungsi public yang utama dan menjadi pintu masuk untuk menjalankan seluruh aplikasi dari luar kelas.

```cpp
public:
    void jalankan() {
        bool berjalan = true;
        while(berjalan) {
            // ... (Menampilkan menu utama)
            int pilihan;
            cin >> pilihan;
            // ... (Validasi input)
            switch(pilihan) {
                // ... (Memanggil fungsi-fungsi private)
                case 0: berjalan = false; break;
            }
        }
    }
```

Tujuan: Mengontrol alur utama aplikasi, menampilkan menu utama, dan menerima input dari pengguna untuk navigasi.<br>
<ul>
  <li><strong>Langkah-langkah Eksekusi:</strong>
    <ul>
      <li>Menginisialisasi <code>bool berjalan = true;</code> untuk mengontrol <code>while</code> loop.</li>
      <li><strong>Loop Utama:</strong> Selama <code>berjalan</code> masih <code>true</code>, program akan terus:
        <ul>
          <li>Menampilkan menu utama kepada pengguna (<code>1. Cari Rute</code>, <code>2. Tampilkan Peta</code>, dll).</li>
          <li>Menerima input pilihan dari pengguna.</li>
          <li>Menggunakan <code>switch</code> statement untuk mengarahkan program ke fungsi <em>private</em> yang sesuai (<code>menuCariRute()</code>, <code>graf.tampilkanGraf()</code>, <code>menuManajemenData()</code>, dll).</li>
          <li>Ketika pengguna memilih <code>"0"</code>, variabel <code>berjalan</code> akan diubah menjadi <code>false</code>, yang menyebabkan <code>while</code> loop berhenti dan program selesai.</li>
        </ul>
      </li>
    </ul>
  </li>
</ul>

<h3>2. Analisis Fungsi main()</h3>
Fungsi main() adalah titik di mana sistem operasi mulai menjalankan program C++. Dalam program ini, main() dibuat sangat sederhana dan bersih, yang merupakan praktik baik dalam Pemrograman Berorientasi Objek.

```cpp
int main() {
    Sistem aplikasi;
    aplikasi.jalankan();
    return 0;
}
```
penjelasan : <br>
<ul>
  <li>
    <strong>Sistem aplikasi;</strong>: Baris ini adalah hal pertama yang signifikan. Ini membuat sebuah instance atau objek dari Kelas <code>Sistem</code>. Saat baris ini dieksekusi, semua objek di dalam <code>Sistem</code> (yaitu <code>graf</code>, <code>pencari</code>, <code>pohonPreferensi</code>, dan <code>manajerFile</code>) juga ikut dibuat. Seluruh program kita kini terkandung dalam satu variabel bernama <code>aplikasi</code>.
  </li>
  <li>
    <strong>aplikasi.jalankan();</strong>: Baris ini memanggil satu-satunya metode publik utama dari objek <code>aplikasi</code>. Panggilan ini akan memulai <em>main loop</em> di dalam fungsi <code>jalankan()</code> dan seluruh interaksi dengan pengguna dimulai dari sini.
  </li>
  <li>
    <strong>return 0;</strong>: Menandakan bahwa program telah selesai dieksekusi dengan sukses.
  </li>
</ul>

<h2 id="bagian7">7. Demonstrasi dan output</h2>
<!-- Konten bagian 7 -->
<h3>1. Menu utama</h3>

![Image](https://github.com/user-attachments/assets/4de36d70-e857-4c8b-80b5-1f63b2ddfb6f)

<h3>2. Kelola Lokasi & Rute (CRUD)</h3>

![Image](https://github.com/user-attachments/assets/21852d87-fcc7-41e8-b4a5-cfbbd33a823c)

<h3>3. Menu load data</h3>

![Image](https://github.com/user-attachments/assets/53deeacc-ec72-4a30-8cae-8139815a3038)

<h3>4. cari rute berdasarkan waktu</h3>

![Image](https://github.com/user-attachments/assets/e61622ea-e4bb-4d68-89ee-8785374857f8)

<h3>5. cari rute berdasarkan biaya</h3>

![Image](https://github.com/user-attachments/assets/a031ee40-cc14-4404-b9ec-7aa7cc1ca208)

<h3>6. cari rute berdasarkan jarak</h3>

![Image](https://github.com/user-attachments/assets/e71bb26e-cce4-49e3-8d94-346b99e98878)

<h3>7. visualisasi graf berupa teks</h3>

![Image](https://github.com/user-attachments/assets/0cb76c88-1b22-48cc-b149-be08e4117c5f)

<h3>7. visualisasi graf dengan matplotlib.pyplot</h3>

![Image](https://github.com/user-attachments/assets/63177714-0dae-47b0-bf75-a988030ecc5c)

![Image](https://github.com/user-attachments/assets/2af32525-830f-4d8b-9699-1fe6eb4191aa)
