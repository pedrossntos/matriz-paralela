#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1000 // tamanho da matriz (pode ajustar para testar desempenho)

void preencherMatriz(int matriz[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = rand() % 10; // valores de 0 a 9
}

void imprimirMatriz(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}

int main() {
    int A[N][N], B[N][N], C[N][N];
    int i, j, k;

    srand(time(NULL)); // inicializa a semente para números aleatórios

    // preenche as matrizes A e B com valores aleatórios
    preencherMatriz(A);
    preencherMatriz(B);

    // inicializa a matriz C com zeros
    #pragma omp parallel for private(j)
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = 0;

    // realiza a multiplicação de matrizes em paralelo
    #pragma omp parallel for private(j, k) shared(A, B, C)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // para visualizar a matriz resultado, use N pequeno e descomente a linha abaixo
    imprimirMatriz(C);

    printf("multiplicação paralela de matrizes %dx%d concluída.\n", N, N);

    return 0;
}
