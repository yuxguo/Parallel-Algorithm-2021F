#include <stdio.h>
#include <omp.h>

#define N 100
#define M 10

int main(void)
{
    int A[N + 1][N + 1], B[N], C[N + 1][N + 1];
    int X[N], Y[2 * N];

#pragma omp parallel for shared(A, B, C)
    for (int i = 0; i < N; ++i)
    {
        B[i] = A[i][M];
#pragma omp parallel for shared(A, B, C)
        for (int j = 0; j < N; ++j)
        {
            A[i + 1][j] = B[i] + C[i][j];
        }
    }

#pragma omp parallel for shared(A, Y)
    for (int i = 1; i < N; ++i)
    {
        Y[i] = A[2][M];
    }

#pragma omp parallel for shared(X, Y)
    for (int i = 0; i < N; ++i)
    {
        X[i] = Y[i] + 10;
    }
#pragma omp parallel for shared(A, Y)
    for (int i = 0; i < N; ++i)
    {
        Y[N + i] = A[1 + i][M];
    }
    return 0;
}
