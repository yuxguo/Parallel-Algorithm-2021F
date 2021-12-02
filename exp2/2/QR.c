#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define A(x, y) A[x * N + y]
#define Q(x, y) Q[x * N + y]

int main(int argc, char **argv)
{
    // Initialization
    int N;
    float *A, *Q;
    clock_t start, end;
    FILE *fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &N);
    A = (float *)malloc(N * N * sizeof(float));
    for (int i = 0; i < N * N; ++i)
    {
        fscanf(fp, "%f", A + i);
    }
    fclose(fp);
    Q = (float *)malloc(N * N * sizeof(float));
    for (int i = 0; i < N * N; ++i)
    {
        if (i / N == i % N)
        {
            Q[i] = 1;
        }
        else
        {
            Q[i] = 0;
        }
    }
    float *aj, *qj, *ai, *qi;
    aj = (float *)malloc(N * sizeof(float));
    qj = (float *)malloc(N * sizeof(float));
    ai = (float *)malloc(N * sizeof(float));
    qi = (float *)malloc(N * sizeof(float));

    start = clock();
    // start computing
    for (int j = 0; j < N; ++j)
    {
        for (int i = j + 1; i < N; ++i)
        {
            float sq = sqrt(A(j, j) * A(j, j) + A(i, j) * A(i, j));
            float c = A(j, j) / sq;
            float s = A(i, j) / sq;
#pragma parallel for shared(A, Q, aj, qj, ai, qi, sq, c, s)
            for (int k = 0; k < N; ++k)
            {
                aj[k] = c * A(j, k) + s * A(i, k);
                qj[k] = c * Q(j, k) + s * Q(i, k);
                ai[k] = -s * A(j, k) + c * A(i, k);
                qi[k] = -s * Q(j, k) + c * Q(i, k);
            }
#pragma parallel for shared(A, Q, aj, qj, ai, qi, sq, c, s)
            for (int k = 0; k < N; ++k)
            {
                A(j, k) = aj[k];
                Q(j, k) = qj[k];
                A(i, k) = ai[k];
                Q(i, k) = qi[k];
            }
        }
    }
    end = clock();
    printf("%d, time: %.3fms\n", N, (double)(end - start) / CLOCKS_PER_SEC * 1e3);
#ifdef PRINT
    printf("Q:\n");
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%.6f\t", Q(j, i));
        }
        printf("\n");
    }
    printf("R:\n");
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%.6f\t", A(i, j));
        }
        printf("\n");
    }
#endif
    free(A);
    free(Q);
    free(aj);
    free(qj);
    free(ai);
    free(qi);
    return 0;
}