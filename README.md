# Belajar Komputasi Paralel 🚀

Repositori ini berisi materi belajar dan tugas kuliah **Komputasi Paralel** (Semester 4). Fokus utama adalah implementasi pemrograman paralel menggunakan bahasa C dan **OpenMP**.

## 📁 Struktur Direktori

- `belajar_dasar_C/`: Berisi dasar-dasar pemrograman C sebagai persiapan.

- `tugas_mandiri/`: Berisi tugas-tugas mandiri terkait komputasi paralel.


## 🛠️ Persyaratan (Requirements)

Untuk menjalankan kode di repositori ini, Anda memerlukan:
- **GCC Compiler** (seperti MinGW di Windows).
- **OpenMP Library** (biasanya sudah termasuk di GCC modern).

## 🚀 Cara Menjalankan Program

### Program C Biasa
```powershell
gcc nama_file.c -o output
./output.exe
```

### Program OpenMP
```powershell
gcc nama_file.c -o output -fopenmp
./output.exe
```

## 📝 Catatan Instalasi (Troubleshooting)

Jika Anda menemui error `cannot find -lpthread` saat kompilasi OpenMP di MinGW:
1. Buka MinGW Installation Manager.
2. Instal paket `pthreads-w32`.
3. Pastikan `libpthread.a` tersedia di folder `lib` MinGW Anda.

---
**Author:** Imam Wahyudi
**Semester:** 4 - Komputasi Paralel
