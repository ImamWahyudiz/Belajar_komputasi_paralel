#define _GNU_SOURCE
#include <stdio.h>
#include <omp.h>
// Deteksi sistem operasi untuk menyertakan pustaka yang sesuai
#ifdef _WIN32
	#include <windows.h>
	static int get_cpu_id(void) {
		// Panggil API secara dinamis agar kompatibel di MinGW lama
		HMODULE h = GetModuleHandleA("kernel32.dll");
		if (h) {
			typedef WORD (WINAPI *GetCurrentProcessorNumber_t)(void);
			GetCurrentProcessorNumber_t fn =
				(GetCurrentProcessorNumber_t)GetProcAddress(h, "GetCurrentProcessorNumber");
			if (fn) {
				return (int)fn();
			}
		}
		return -1;
	}
#else
	#include <sched.h> // Pustaka untuk mendeteksi CPU Core
	static int get_cpu_id(void) {
		return sched_getcpu();
	}
#endif
int main() {
 int N = 5;
 // Inisialisasi array A dengan nilai awal indeks 0 = 10, sisanya 0
 int A[5] = {10, 0, 0, 0, 0};
 int B[5] = {0, 5, 5, 5, 5};

 // PERINGATAN: Ini tidak aman karena nilai A[i] membutuhkan nilai A[i-1] yang sedang dihitu #pragma omp parallel for
 for (int i = 1; i < N; i++) {
 int id = omp_get_thread_num();
 int cpu_id = get_cpu_id(); // Mendapatkan ID CPU core
 A[i] = A[i-1] + B[i];
 #pragma omp critical
 printf("Thread %d di CPU %d menghitung indeks %d: A[%d] = A[%d] (%d) + %d = %d\n",
 id, cpu_id, i, i, i-1, A[i-1], B[i], A[i]);
 }
 // Menampilkan hasil akhir array A
 printf("\nHasil Akhir Array A (Ekspektasi Serial: 10, 15, 20, 25, 30):\n");
 for (int i = 0; i < N; i++) {
 printf("A[%d] = %d\n", i, A[i]);
 }
 return 0;
}