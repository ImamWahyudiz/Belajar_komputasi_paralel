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
    #include <sched.h>
    static int get_cpu_id(void) {
        return sched_getcpu();
    }
#endif

int main() {
    int N = 8;

    // Perintah untuk membagi iterasi loop ke thread yang tersedia
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int thread_id = omp_get_thread_num();
        
         // Memanggil fungsi deteksi CPU (otomatis menyesuaikan OS)
         int cpu_id = get_cpu_id();

        #pragma omp critical
        if (cpu_id >= 0) {
            printf("Iterasi ke-%d dikerjakan oleh Thread %d di CPU %d\n",
                i, thread_id, cpu_id);
        } else {
            printf("Iterasi ke-%d dikerjakan oleh Thread %d di CPU ?\n",
                i, thread_id);
        }
    }
    return 0;
}