#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define A_0(x, y) A_0[x * N + y]
#define A(x, y) A[x * N + y]
#define L(x, y) L[x * N + y]
#define U(x, y) U[x * N + y]

int main(int argc, char **argv)
{
    int rank, size;
    int N;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float *A_0, *L, *U, *A, *f;

    if (rank == 0)
    {
        fscanf(stdin, "%d", &N);
        A_0 = (float *)malloc(N * N * sizeof(float));
        for (int i = 0; i < N * N; ++i)
        {
            fscanf(stdin, "%f", A_0 + i);
        }
    }
    // Broadcast N
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int m = N / size; // assume N % size == 0

    // Malloc A, f
    A = (float *)malloc(m * N * sizeof(float));
    f = (float *)malloc(N * sizeof(float));

    // Malloc L, U for rank0
    if (rank == 0)
    {
        L = (float *)malloc(N * N * sizeof(float));
        U = (float *)malloc(N * N * sizeof(float));
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i == j)
                {
                    L(i, j) = 1.0;
                }
                else
                {
                    L(i, j) = 0.0;
                }
            }
        }
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                U(i, j) = 0.0;
            }
        }
    }
    // Send A_0 to all proc
    if (rank == 0)
    {
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                A(i, j) = A_0(i * size, j);
            }
        }
        for (int i = 0; i < N; ++i)
        {
            if (i % size != 0)
            {
                MPI_Send(A_0 + i * N, N, MPI_FLOAT, i % size, i / size, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        for (int i = 0; i < m; ++i)
        {
            MPI_Recv(A + i * N, N, MPI_FLOAT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // Start computing
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int v = i * size + j;
            if (rank == j)
            {
                for (int k = v; k < N; ++k)
                {
                    f[k] = A(i, k);
                }
                MPI_Bcast(f, N, MPI_FLOAT, j, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Bcast(f, N, MPI_FLOAT, j, MPI_COMM_WORLD);
            }
            if (rank <= j)
            {
                for (int k = i + 1; k < m; ++k)
                {
                    A(k, v) = A(k, v) / f[v];
                    for (int w = v + 1; w < N; ++w)
                    {
                        A(k, w) = A(k, w) - f[w] * A(k, v);
                    }
                }
            }
            else
            {
                for (int k = i; k < m; ++k)
                {
                    A(k, v) = A(k, v) / f[v];
                    for (int w = v + 1; w < N; ++w)
                    {
                        A(k, w) = A(k, w) - f[w] * A(k, v);
                    }
                }
            }
        }
    }
    // End computing

    // Start gathering
    if (rank == 0)
    {
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                A_0(i * size, j) = A(i, j);
            }
        }
        for (int i = 0; i < N; ++i)
        {
            if (i % size != 0)
            {
                MPI_Recv(A_0 + i * N, N, MPI_FLOAT, i % size, i / size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }
    else
    {
        for (int i = 0; i < m; ++i)
        {
            MPI_Send(A + i * N, N, MPI_FLOAT, 0, i, MPI_COMM_WORLD);
        }
    }
    if (rank == 0)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i > j)
                {
                    L(i, j) = A_0(i, j);
                }
                else
                {
                    U(i, j) = A_0(i, j);
                }
            }
        }
    }
    if (rank == 0)
    {
        printf("L:\n");
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                printf("%.6f\t", L(i, j));
            }
            printf("\n");
        }
        printf("U:\n");
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                printf("%.6f\t", U(i, j));
            }
            printf("\n");
        }
    }
    if (rank == 0)
    {
        free(A_0);
        free(L);
        free(U);
    }
    free(A);
    free(f);
    MPI_Finalize();
    return 0;
}