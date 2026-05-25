#include <omp.h>
#include <stdio.h>
int main() {
  int shared_var =
      100; // Semua thread akan mengakses dan BISA mengubah variabel ini
  int temp =
      -1; // Dideklarasikan di luar, namun akan di-set privat untuk tiap thread
// Menentukan atribut data sharing lewat clause pragma
#pragma omp parallel private(temp) shared(shared_var)
  {
    int thread_id = omp_get_thread_num();
    // Setiap thread mengisi 'temp' lokalnya sendiri tanpa mengganggu thread
    // lain
    temp = thread_id * 10;
// Mengamankan penulisan ke shared_var agar tertib bergantian
#pragma omp critical
    {
      // Thread memodifikasi variabel shared yang sama!
      shared_var = shared_var + 5;
      printf("[Thread %d] temp = %d | shared_var diubah menjadi = %d\n",
             thread_id, temp, shared_var);
    }
  }
  // Mengecek nilai variabel setelah keluar dari blok paralel
  printf("\n--- Di luar blok paralel ---\n");
  printf("Nilai temp asli (tetap tidak berubah): %d\n", temp);
  printf("Nilai shared_var akhir (berubah secara kumulatif): %d\n", shared_var);
  return 0;
}