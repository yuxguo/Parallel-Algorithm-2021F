#include <stdio.h>
#include <omp.h>

#define N 100

int main(void)
{
    float A[N], B[N + 1], C[N + 1], D[N];

    for (int i = 0; i < N; ++i)
    {
        B[i] = C[i] * 2;
        C[i + 1] = 1 / B[i]; 
    }

#pragma omp parallel for shared(A, B)
    for (int i = 0; i < N; ++i)
    {
        A[i] = A[i] + B[i];
    }
#pragma omp parallel for shared(C, D)
    for (int i = N; i < N; ++i)
    {
        D[i] = C[i] * C[i];
    }
    return 0;
}
