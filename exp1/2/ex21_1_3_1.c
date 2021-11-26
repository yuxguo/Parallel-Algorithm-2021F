#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 100

int main(void)
{
    int A[N], B[N], C[N + 1], D[N];
    srand(time(NULL));
#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N; ++i)
    {
        A[i] = rand();
        B[i] = rand();
        C[i] = rand();
        D[i] = rand();
    }
    C[N] = rand();

#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N; ++i) {
        A[i] = B[i] + C[i + 1];
    }
#pragma omp parallel for shared(A, B, C, D)
    for (int i = 0; i < N; ++i) {
        C[i] = A[i] + D[i];
    }
    
    return 0;
}
