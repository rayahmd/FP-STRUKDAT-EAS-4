# FP-STRUKDAT-EAS-4


| Nama  | NRP |
| ------------- | ------------- |
| Zein Muhammad Hasan  | 5027241035 |
| Aslam Ahmad Usman |  5027241074  |
| Tiara Fatimah Azzahra  |  5027241090 |
| Raya Ahmad Syarif  | 5027241041  |

<ul>
  <li><a href="#bagian1">1. Pengantar Library dan Implementasi Kelas Rute</a></li>
  <li><a href="#bagian2">2. bagian saha</a></li>
  <li><a href="#bagian3">3. bagian saha</a></li>
  <li><a href="#bagian4">4. bagian saha</a></li>
  <li><a href="#bagian5">5. Kelas Pohon Preferensi & File Manager</a></li>
  <li><a href="#bagian6">6. Kelas Sistem dan main function</a></li>
  <li><a href="#bagian7">7. Demonstrasi dan output</a></li>
</ul>

<hr>

<h2 id="bagian1">1. Pengantar Library dan Implementasi Kelas Rute</h2>
<!-- Konten bagian 1 -->
Tujuan Pembelajaran
Pada bagian ini, kita akan:

- Memahami library (pustaka) apa saja yang digunakan dalam program dan peranannya.
- Mengenal konsep dan implementasi kelas Rute untuk merepresentasikan satu jalur dalam graf.

<h3>Library dalam C++</h3>
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

using namespace std;
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

<h2 id="bagian3">3. bagian saha</h2>
<!-- Konten bagian 3 -->

<h2 id="bagian4">4. bagian saha</h2>
<!-- Konten bagian 4 -->

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
