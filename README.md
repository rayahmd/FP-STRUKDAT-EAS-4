# FP-STRUKDAT-EAS-4


| Nama  | NRP |
| ------------- | ------------- |
| Zein Muhammad Hasan  | 5027241035 |
| Tiara Fatimah Azzahra  |  5027241090 |
|  |  |

<ul>
  <li><a href="#bagian1">1. Pengantar Library dan Implementasi Kelas Rute</a></li>
  <li><a href="#bagian2">2. bagian saha</a></li>
  <li><a href="#bagian3">3. bagian saha</a></li>
  <li><a href="#bagian4">4. bagian saha</a></li>
  <li><a href="#bagian5">5. bagian saha</a></li>
  <li><a href="#bagian6">6. Kelas Sistem dan main function</a></li>
  <li><a href="#bagian7">7. Demonstrasi dan output</a></li>
</ul>

<hr>

<h2 id="bagian1">1. bagian1</h2>
<!-- Konten bagian 1 -->

<h2 id="bagian2">2. bagian2</h2>
<!-- Konten bagian 2 -->

<h2 id="bagian3">3. bagian saha</h2>
<!-- Konten bagian 3 -->

<h2 id="bagian4">4. bagian saha</h2>
<!-- Konten bagian 4 -->

<h2 id="bagian5">5. bagian saha</h2>
<!-- Konten bagian 5 -->

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
