#include <stdio.h>
#include <omp.h>

// gcc 7_openmp.c -o 7_openmp -fopenmp

int main (){
     
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        int num = omp_get_num_threads();
        
        printf("Hello dari thread %d dari total %d thread\n", id, num);
    }

    return 0;
}