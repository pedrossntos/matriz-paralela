#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 10000

void preencherMatriz(int **matriz) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = rand() % 10;
}

void imprimirMatriz(int **matriz) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}

int main() {
    int i, j, k;
    double tempo_cpu;
    clock_t start, end;

    start = clock();
    // alocação dinâmica das matrizes
    int **A = malloc(N * sizeof(int *));
    int **B = malloc(N * sizeof(int *));
    int **C = malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(int));
        B[i] = malloc(N * sizeof(int));
        C[i] = malloc(N * sizeof(int));
    }

    srand(time(NULL)); // inicializa aleatório

    preencherMatriz(A);
    preencherMatriz(B);

    // inicialização da matriz C
    #pragma omp parallel for private(j)
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = 0;

    // multiplicação paralela
    #pragma omp parallel for private(j, k) shared(A, B, C)
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];

    imprimirMatriz(C);             
    printf("multiplicação paralela de matrizes %dx%d concluída.\n", N, N);

    // liberar memória
    for (i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    end = clock();

    tempo_cpu = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("tempo de execução: %f seg\n", tempo_cpu);

    return 0;
}
