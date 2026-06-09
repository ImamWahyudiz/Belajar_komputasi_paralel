# Modul Praktikum: Analisis Nilai Mahasiswa dengan OpenMPI

Modul ini memandu Anda untuk menjalankan aplikasi komputasi paralel **Analisis Nilai Mahasiswa**. Aplikasi ini menggunakan arsitektur kluster *Master-Worker* heterogen dengan total **6 Node** terdistribusi.

## 📂 Struktur File

Di dalam direktori proyek ini terdapat:
- `analisis_nilai.c` : Kode sumber bahasa C untuk logika paralel (menampilkan tabel dan *workflow*).
- `data_nilai.csv`   : Data sumber (100 baris nilai dummy). **File ini wajib disebarkan ke semua laptop!**
- `hosts.txt`        : File konfigurasi host berisi IP masing-masing node.
- `generate_data.py` : Script (opsional) untuk membuat ulang data dummy acak.

---

## 🚀 Cara Clone Repositori

Jika Anda atau dosen penguji ingin mengunduh dan menjalankan program ini, silakan jalankan perintah berikut di dalam terminal Ubuntu/WSL:

```bash
# 1. Clone repositori ke komputer Anda
git clone https://github.com/ImamWahyudiz/Belajar_komputasi_paralel.git

# 2. Masuk ke dalam direktori tugas UAS
cd Belajar_komputasi_paralel/tugas_UAS_openMPI
```

---

## 🛠️ Persiapan Lingkungan Kluster (Tips Penting)

Berdasarkan pengujian eksekusi nyata pada 6 laptop dengan arsitektur, OS, dan merek yang berbeda-beda, berikut adalah panduan *troubleshooting* agar kluster Anda bebas dari error:

### 1. Gunakan Tailscale (Menghindari NAT WSL)
Koneksi antar-laptop yang menjalankan Ubuntu (WSL) di satu jaringan WiFi fisik biasanya akan gagal saling `ping` karena terjebak oleh sistem NAT Windows. 
**Tips:** Instal **Tailscale** di semua node WSL, lalu gunakan IP VPN Tailscale (`100.x.x.x`) di dalam file `hosts.txt`.

### 2. Atasi Error SSH Minta Password (*Permission Denied*)
MPI memerlukan otorisasi *passwordless SSH* ke semua node. Jika setelah membagikan kunci publik menggunakan `ssh-copy-id` node tujuan tetap meminta *password*, perbaiki hak akses direktori keamanan di node tujuan (Worker) tersebut:
```bash
chmod go-w ~/
chmod 700 ~/.ssh
chmod 600 ~/.ssh/authorized_keys
```

### 3. Daftarkan Hostname Master (Mencegah MPI Hanging)
Untuk menghindari proses eksekusi tertahan (nge-*hang*) akibat Worker gagal menemukan jalan pulang ke Master, tambahkan pemetaan IP Master ke *Hostname*-nya di dalam file `/etc/hosts` pada **semua komputer Worker**. Contoh:
```text
100.97.21.76  DESKTOP-B6SK0NH
```

### 4. Beda Arsitektur CPU (Menghindari `Exec format error`)
Jangan pernah mendistribusikan file hasil *compile* (`analisis_nilai` yang sudah jadi) dari Master ke Worker jika mesin kalian berbeda! (Misal: Master menggunakan Intel/AMD *x86_64*, sedangkan Worker memakai Macbook Apple Silicon *ARM64*). File tersebut tidak akan bisa dibaca dan memicu *Exec format error*.
**Tips (Compile Mandiri):** Kirim file mentah `.c` dan file CSV-nya, lalu perintahkan setiap laptop untuk melakukan *compile* sendiri dari jarak jauh menggunakan satu skrip singkat di komputer Master:
```bash
for ip in 100.116.214.85 100.85.227.48 100.70.234.121 100.121.102.75 100.127.223.99; do
  scp analisis_nilai.c data_nilai.csv mpi@$ip:/home/mpi/UAS/
  ssh mpi@$ip "cd /home/mpi/UAS && mpicc analisis_nilai.c -o analisis_nilai"
done
```

---

## 🏃‍♂️ Langkah Eksekusi (Menjalankan Program)

### Skenario 1: Uji Coba di 1 Laptop (Simulasi)
Jika Anda hanya memiliki 1 mesin dan ingin mengetes algoritma MPI ini, jalankan perintah ini (pastikan sudah melakukan `mpicc`):
```bash
mpirun -np 6 ./analisis_nilai
```

### Skenario 2: Eksekusi Kluster Nyata (6 Laptop via Tailscale)
Setelah semua persiapan di atas matang dan setiap Worker sudah memiliki file executable di foldernya, Master dapat memicu komputasi terdistribusi dengan *command* ini:

```bash
mpirun -np 6 --map-by node --mca btl_tcp_if_include tailscale0 --mca oob_tcp_if_include tailscale0 -hostfile hosts.txt ./analisis_nilai
```

**Penjelasan Parameter Sakti:**
- `-np 6`: Menjalankan tepat 6 proses (1 Master + 5 Worker).
- `--map-by node`: Memaksa pembagian beban kerja secara merata dan spesifik, yaitu persis 1 proses/tugas untuk setiap node laptop fisik.
- `--mca btl_tcp_if_include tailscale0`: Memaksa OpenMPI untuk menyalurkan seluruh lalu lintas data eksklusif melalui terowongan VPN `tailscale0` agar tidak tersesat atau diblokir oleh antarmuka jaringan yang lain.

---

## 📊 Format Output (Hasil Akhir)

Terminal komputer Master akan memunculkan:
1. Rangkuman **Nilai Tertinggi, Terendah, dan Rata-rata** global.
2. Tabel **DETAIL PEKERJAAN NODE** yang merincikan beban kerja per *Rank*, mencantumkan *Hostname* aktual laptop yang mengerjakannya, serta hasil agregasi lokal (*Max Loc & Min Loc*).
3. Penjabaran **ALUR PENGERJAAN (WORKFLOW)** yang mendeskripsikan secara kronologis proses `Scatterv`, komputasi paralel, hingga pengumpulan akhir via `Gather` & `Reduce`.
