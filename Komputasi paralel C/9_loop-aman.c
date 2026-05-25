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
	int A[5] = {0};
	int B[5] = {10, 20, 30, 40, 50};
	int C[5] = {1, 2, 3, 4, 5};

	// Paralelisasi aman karena setiap elemen A[i] hanya bergantung pada B[i] dan C[i]
	#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		int id = omp_get_thread_num();
		int cpu_id = get_cpu_id(); // Mendapatkan ID CPU core
		A[i] = B[i] + C[i];

		#pragma omp critical
		if (cpu_id >= 0) {
			printf("Thread %d di CPU %d menghitung indeks %d: A[%d] = %d + %d = %d\n",
				id, cpu_id, i, i, B[i], C[i], A[i]);
		} else {
			printf("Thread %d di CPU ? menghitung indeks %d: A[%d] = %d + %d = %d\n",
				id, i, i, B[i], C[i], A[i]);
		}
	}

	// Menampilkan hasil akhir array A
	printf("\nHasil Akhir Array A:\n");
	for (int i = 0; i < N; i++) {
		printf("A[%d] = %d\n", i, A[i]);
	}
	return 0;
}