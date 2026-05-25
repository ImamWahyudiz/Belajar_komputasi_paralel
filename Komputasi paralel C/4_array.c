#include <stdio.h>

// gcc 4_array.c -o 4_array -lm

int main() {
    int nilai[3] = {100, 90, 80};
    
    int matrix[2][2] = {{1, 2}, {3, 4}};

    printf("nilai pertama : %d\n",nilai[0]);
    printf("nilai kedua : %d\n",nilai[1]);
    printf("nilai ketiga : %d\n",nilai[2]);


    int i, j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
    