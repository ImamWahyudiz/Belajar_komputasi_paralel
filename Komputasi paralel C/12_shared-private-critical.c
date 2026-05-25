#define _GNU_SOURCE
#include <omp.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
static int get_cpu_id(void) {
  // Panggil API secara dinamis agar kompatibel di MinGW lama
  HMODULE h = GetModuleHandleA("kernel32.dll");
  if (h) {
    typedef WORD(WINAPI * GetCurrentProcessorNumber_t)(void);
    GetCurrentProcessorNumber_t fn =
        (GetCurrentProcessorNumber_t)GetProcAddress(h, "GetCurrentProcessorNumber");
    if (fn) {
      return (int)fn();
    }
  }
  return -1;
}
#else
#include <sched.h>
static int get_cpu_id(void) { return sched_getcpu(); }
#endif

int main() {
  int total = 0;
  int N = 10; // Menggunakan N=10 agar output di terminal mudah dibaca
#pragma omp parallel for
  for (int i = 1; i <= N; i++) {
    int thread_id = omp_get_thread_num();
    int cpu_id = get_cpu_id(); // Mendapatkan ID CPU core
// Hanya satu thread yang boleh mengeksekusi blok ini secara bergantian
#pragma omp critical
    {
      total += i;
      if (cpu_id >= 0) {
        printf(
            "[Thread %d di CPU %d] Menambahkan i = %d | Total Sementara = %d\n",
            thread_id, cpu_id, i, total);
      } else {
        printf(
            "[Thread %d di CPU ?] Menambahkan i = %d | Total Sementara = %d\n",
            thread_id, i, total);
      }
    }
  }
  printf("\nTotal akhir menggunakan critical: %d\n", total);
  return 0;
}