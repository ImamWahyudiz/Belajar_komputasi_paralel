#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Alokasi array dinamis berukuran 10 menggunakan malloc
    int *arr = (int*) malloc(10 * sizeof(int));

    // Validasi alokasi memori
    if (arr == NULL) {
        printf("Gagal mengalokasikan memori!\n");
        return 1;
    }

    // 2. Mengisinya dengan angka 1-10 menggunakan for loop
    for (int i = 0; i < 10; i++) {
        arr[i] = i + 1;
    }

    // 3. Mencetaknya menggunakan pointer (aritmatika pointer)
    printf("Isi array dinamis: ");
    for (int i = 0; i < 10; i++) {
        // Menggunakan dereference pointer *(arr + i)
        printf("%d ", *(arr + i));
    }
    printf("\n");

    // 4. Dealokasi memori menggunakan free
    free(arr);

    return 0;
}
