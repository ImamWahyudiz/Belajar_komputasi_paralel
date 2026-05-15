#include <stdio.h>

// gcc 5_pointer.c -o 5_pointer -lm

int main() {
    
    int x = 100;
    int *ptr = &x;

    printf("Nilai x: %d\n", x);
    printf("Alamat memori x: %p\n", &x);
    printf("Nilai ptr: %d\n", ptr);
    printf("Nilai yang ditunjuk ptr: %d\n", *ptr);

    *ptr = 200;
    printf("Nilai x setelah diubah melalui ptr: %d\n", x);
    
    return 0;
}
