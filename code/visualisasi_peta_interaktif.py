import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import warnings
import os

# Mengabaikan warning yang mungkin muncul dari matplotlib terkait font
warnings.filterwarnings("ignore", category=UserWarning)

# --- FUNGSI UTAMA ---
def visualisasikan_peta():
    """
    Fungsi utama untuk menjalankan seluruh proses,
    mulai dari meminta input hingga menampilkan visualisasi.
    """
    # --- 1. MEMINTA INPUT NAMA FILE DARI PENGGUNA ---
    print("="*55)
    print("   Selamat Datang di Visualisasi Peta Graf Interaktif")
    print("="*55)
    print("Pastikan file CSV yang ingin divisualisasikan berada di folder yang sama.")
    
    file_lokasi_input = input("\n> Masukkan nama file lokasi (contoh: lokasi_surabaya.csv): ")
    file_rute_input = input("> Masukkan nama file rute (contoh: rute_surabaya.csv): ")

    # Secara otomatis menambahkan ekstensi .csv jika pengguna lupa
    if not file_lokasi_input.endswith('.csv'):
        file_lokasi_input += '.csv'
    if not file_rute_input.endswith('.csv'):
        file_rute_input += '.csv'

    # --- 2. MEMBACA DATA DARI FILE CSV ---
    try:
        print(f"\nINFO: Mencoba memuat file '{file_lokasi_input}' dan '{file_rute_input}'...")
        # Cek keberadaan file satu per satu untuk pesan error yang lebih jelas
        if not os.path.exists(file_lokasi_input):
            raise FileNotFoundError(f"File lokasi '{file_lokasi_input}' tidak ditemukan.")
        if not os.path.exists(file_rute_input):
            raise FileNotFoundError(f"File rute '{file_rute_input}' tidak ditemukan.")

        df_lokasi = pd.read_csv(file_lokasi_input)
        df_rute = pd.read_csv(file_rute_input)
        print("SUCCESS: File CSV berhasil dibaca.")

    except FileNotFoundError as e:
        print(f"ERROR: {e}")
        print("Mohon periksa kembali nama file dan pastikan file ada di direktori yang sama.")
        return # Menghentikan eksekusi jika file tidak ditemukan
    except Exception as e:
        print(f"ERROR: Terjadi kesalahan saat membaca file. {e}")
        return

    # --- 3. MEMBUAT OBJEK GRAF ---
    G = nx.DiGraph()
    print("INFO: Objek Graf (DiGraph) berhasil dibuat.")

    # --- 4. MENAMBAHKAN LOKASI SEBAGAI NODE ---
    for index, row in df_lokasi.iterrows():
        G.add_node(row['id'], label=row['nama'])
    print(f"INFO: {len(df_lokasi)} lokasi berhasil ditambahkan sebagai node.")

    # --- 5. MENAMBAHKAN RUTE SEBAGAI EDGE ---
    for index, row in df_rute.iterrows():
        G.add_edge(row['id_awal'], row['id_tujuan'], 
                   jarak=row['jarak'], 
                   waktu=row['waktu'], 
                   biaya=row['biaya'])
    print(f"INFO: {len(df_rute)} rute berhasil ditambahkan sebagai edge.")

    # --- 6. PERSIAPAN VISUALISASI ---
    pos = nx.spring_layout(G, k=0.9, iterations=50, seed=42)
    node_labels = nx.get_node_attributes(G, 'label')

    plt.figure(figsize=(20, 14))
    plt.title(f"Visualisasi Peta Rute dari file '{os.path.basename(file_rute_input)}'", size=20, pad=20)

    node_options = {"node_color": "#6495ED", "node_size": 2500, "alpha": 0.9}
    label_options = {"font_size": 10, "font_color": "black", "font_weight": "bold", "verticalalignment": "center"}
    edge_options = {"width": 1.5, "edge_color": "gray", "style": "solid", "alpha": 0.7, "arrowsize": 15, "connectionstyle": "arc3,rad=0.05"}

    # --- 7. MENGGAMBAR GRAF ---
    print("INFO: Memulai proses rendering visualisasi...")
    nx.draw_networkx_nodes(G, pos, **node_options)
    nx.draw_networkx_edges(G, pos, **edge_options)
    nx.draw_networkx_labels(G, pos, labels=node_labels, **label_options)

    # --- 8. MENAMPILKAN HASIL ---
    plt.axis('off')
    plt.tight_layout()
    print("SUCCESS: Visualisasi selesai. Menampilkan peta...")
    plt.show()


# --- ENTRY POINT SCRIPT ---
if __name__ == "__main__":
    visualisasikan_peta()
