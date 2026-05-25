#include <stdio.h>
#include <stdlib.h>

// gcc 6_malloc.c -o 6_malloc -lm

int main(){

int n,i ;

int *arr;

printf("Masukkan jumlah elemen: ");
scanf("%d", &n);

arr = (int*) malloc(n * sizeof(int));

if (arr == NULL) {
    printf("Gagal mengalokasikan memori\n");
    return 1;
}

for (i = 0; i < n; i++) {
   arr[i] = i*10;
   printf("Elemen ke-%d: %d\n", i, arr[i]);
}



free(arr);
return 0;
}