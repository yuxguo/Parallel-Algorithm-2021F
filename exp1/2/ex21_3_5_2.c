#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 1000

int main(void)
{
    int A[N], B[N], C[N], D[N];
    srand(time(NULL));
#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N; ++i)
    {
        A[i] = rand();
        B[i] = rand();
        C[i] = rand();
        D[i] = rand();
    }

#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N / 2; ++i) {
        A[i] = B[i] + C[i];
    }
#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N / 2; ++i) {
        D[i] = (A[i] + A[N - 1 - i]) / 2;
    }
#pragma omp parallel for shared(A, B, C, D)
    for (int i = N / 2; i < N; ++i) {
        A[i] = B[i] + C[i];
    }
#pragma omp parallel for shared(A, B, C, D)
    for (int i = N / 2; i < N; ++i) {
        D[i] = (A[i] + A[N - 1 - i]) / 2;
    }
    return 0;
}
