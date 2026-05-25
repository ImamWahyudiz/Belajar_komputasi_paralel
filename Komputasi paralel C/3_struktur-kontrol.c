#include <stdio.h>

// gcc 3_struktur-kontrol.c -o 3_struktur-kontrol -lm
int main() {

    int i;
    int batas;

    printf("Masukkan batas perulangan: ");
    scanf("%d", &batas);

    if (batas > 0) {
    
        printf("Batas adalah bilangan positif\n");
    }

    for (i = 1; i <= batas; i++) {
        printf("Perulangan ke: %d\n", i);
    }

    return 0;
}