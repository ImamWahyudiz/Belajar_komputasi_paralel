#include <stdio.h>
#include <omp.h> // Header wajib untuk OpenMP
int main() {
 // Memulai blok paralel (Fase Fork)
 #pragma omp parallel
 {
 int id = omp_get_thread_num();
 int total = omp_get_num_threads();

    if (id % 2 == 0) {
        printf("Saya Thread Genap: %d dari total %d thread\n", id, total);
    } else {
        printf("Saya Thread Ganjil: %d dari total %d thread\n", id, total);
    }
    }
 // Akhir blok paralel (Fase Join)

 return 0;
}