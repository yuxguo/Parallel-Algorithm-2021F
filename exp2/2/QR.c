#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define A(x, y) A[x * N + y]
#define Q(x, y) Q[x * N + y]

int N;
float *A, *Q;

void my_wait(int line, int base, int *finish);
void transform(int j, int start_line, int end_line); // major_line, start_line, end_line

int main(int argc, char **argv)
{
    double start, end;
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
    int *finish = (int *)malloc(N * sizeof(int));
    int total_thread, my_thread, block, base;
    for (int i = 0; i < N; i++)
    {
        finish[i] = 0;
    }
    start = omp_get_wtime();
    // start computing
#pragma omp parallel shared(Q, A, finish) private(my_thread, total_thread, base, block)
    {
        my_thread = omp_get_thread_num();
        total_thread = omp_get_num_threads();
        block = N / total_thread;
        base = my_thread * block;
        for (int j = 0; j < base + block; ++j)
        {
            if (j < base) // need to wait other thread
            {
                my_wait(j, base, finish);
                transform(j, base, base + block);
            }
            else
            {
                transform(j, j + 1, base + block);
            }
            finish[j] = base + block;
        }
    }
    end = omp_get_wtime();
    printf("%d, time: %.3f ms\n", N, (double)(end - start) * 1e3);
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
    free(finish);
    return 0;
}
void my_wait(int line, int base, int *finish)
{
    while (1)
    {
        if (finish[line] >= base)
        {
            break;
        }
    }
}
void transform(int j, int start_line, int end_line)
{
    for (int i = start_line; i < end_line; ++i)
    {
        float sq = sqrt(A(j, j) * A(j, j) + A(i, j) * A(i, j));
        float c = A(j, j) / sq;
        float s = A(i, j) / sq;
        float ajk, aik, qjk, qik;
        for (int k = 0; k < N; ++k)
        {
            ajk = c * A(j, k) + s * A(i, k);
            qjk = c * Q(j, k) + s * Q(i, k);
            aik = -s * A(j, k) + c * A(i, k);
            qik = -s * Q(j, k) + c * Q(i, k);
            A(j, k) = ajk;
            Q(j, k) = qjk;
            A(i, k) = aik;
            Q(i, k) = qik;
        }
    }
}