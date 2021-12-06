#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define A_0(x, y) A_0[x * N + y]
#define B_0(x, y) B_0[x * N + y]
#define C_0(x, y) C_0[x * N + y]
#define A(x, y) A[x * b_col + y]
#define B(x, y) B[x * b_col + y]
#define C(x, y) C[x * b_col + y]
#define A_(x, y) A_[x * b_col + y]
#define B_(x, y) B_[x * b_col + y]

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        return 0;
    }
    int rank, size;
    int p_col, p_row;
    int b_col, b_row;
    int self_col, self_row;
    int N;
    double start, end;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // assume size = x ^ 2, N % x == 0

    float *A_0, *B_0, *C_0, *A, *B, *C, *A_, *B_;
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    if (rank == 0)
    {
        FILE *fp = fopen(argv[1], "r");
        fscanf(fp, "%d", &N);
        A_0 = (float *)malloc(N * N * sizeof(float));
        for (int i = 0; i < N * N; ++i)
        {
            fscanf(fp, "%f", A_0 + i);
        }
        fscanf(fp, "%d", &N);
        B_0 = (float *)malloc(N * N * sizeof(float));
        for (int i = 0; i < N * N; ++i)
        {
            fscanf(fp, "%f", B_0 + i);
        }
        C_0 = (float *)malloc(N * N * sizeof(float));
        fclose(fp);
    }
    // Broadcast N
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    p_row = (int)sqrt((double)size);
    p_col = size / p_row;

    b_row = N / p_row;
    b_col = N / p_col;

    // construct comm
    MPI_Comm grid_comm, row_comm, col_comm;
    int ndims[2] = {p_row, p_col};
    int periods[2] = {1, 1};
    int my_grid_rank;
    int my_coord[2];
    MPI_Cart_create(MPI_COMM_WORLD, 2, ndims, periods, 1, &grid_comm);
    MPI_Comm_rank(grid_comm, &my_grid_rank);
    MPI_Cart_coords(grid_comm, my_grid_rank, 2, my_coord);
    int row_flag[2] = {0, 1};
    int col_flag[2] = {1, 0};
    MPI_Cart_sub(grid_comm, row_flag, &row_comm);
    MPI_Cart_sub(grid_comm, col_flag, &col_comm);

    // printf("rank: %d, grid_rank: %d, coord: (%d, %d), ?: %d\n", rank, my_grid_rank, my_coord[0], my_coord[1], tmp);

    // Malloc A, B for per proc
    A = (float *)malloc(b_row * b_col * sizeof(float));
    B = (float *)malloc(b_row * b_col * sizeof(float));
    C = (float *)malloc(b_row * b_col * sizeof(float));

    A_ = (float *)malloc(b_row * b_col * sizeof(float));
    B_ = (float *)malloc(b_row * b_col * sizeof(float));

    // Send A_0 to all proc
    if (rank == 0)
    {
        for (int i = 0; i < b_row; ++i)
        {
            for (int j = 0; j < b_col; ++j)
            {
                A(i, j) = A_0((i + my_coord[0] * b_row), (j + my_coord[1] * b_col));
            }
        }

        for (int i = 1; i < size; ++i)
        {
            int other_coord[2];
            MPI_Cart_coords(grid_comm, i, 2, other_coord);
            for (int j = 0; j < b_row; ++j)
            {
                MPI_Send(&A_0((other_coord[0] * b_row + j), (other_coord[1] * b_col)), b_col, MPI_FLOAT, i, j, grid_comm);
            }
        }
    }
    else
    {
        for (int j = 0; j < b_row; ++j)
        {
            MPI_Recv(A + j * b_row, b_col, MPI_FLOAT, 0, j, grid_comm, MPI_STATUS_IGNORE);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Send B_0 to all proc
    if (rank == 0)
    {
        for (int i = 0; i < b_row; ++i)
        {
            for (int j = 0; j < b_col; ++j)
            {
                B(i, j) = B_0((i + my_coord[0] * b_row), (j + my_coord[1] * b_col));
            }
        }

        for (int i = 1; i < size; ++i)
        {
            int other_coord[2];
            MPI_Cart_coords(grid_comm, i, 2, other_coord);
            for (int j = 0; j < b_row; ++j)
            {
                MPI_Send(&B_0((other_coord[0] * b_row + j), (other_coord[1] * b_col)), b_col, MPI_FLOAT, i, j, grid_comm);
            }
        }
    }
    else
    {
        for (int j = 0; j < b_row; ++j)
        {
            MPI_Recv(B + j * b_row, b_col, MPI_FLOAT, 0, j, grid_comm, MPI_STATUS_IGNORE);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Start Initialization
    for (int i = 0; i < b_row; ++i)
    {
        for (int j = 0; j < b_col; ++j)
        {
            C(i, j) = 0.0;
        }
    }
    // End Initialization

    // Start computing
    for (int k = 0; k < p_col; ++k)
    {
        // for mat A
        if (my_coord[1] == k)
        {
            for (int i = 0; i < b_row; ++i)
            {
                for (int j = 0; j < b_col; ++j)
                {
                    A_(i, j) = A(i, j);
                }
            }
            MPI_Bcast(A_, b_row * b_col, MPI_FLOAT, k, row_comm);
        }
        else
        {
            MPI_Bcast(A_, b_row * b_col, MPI_FLOAT, k, row_comm);
        }

        // for mat B
        if (my_coord[0] == k)
        {
            for (int i = 0; i < b_row; ++i)
            {
                for (int j = 0; j < b_col; ++j)
                {
                    B_(i, j) = B(i, j);
                }
            }
            MPI_Bcast(B_, b_row * b_col, MPI_FLOAT, k, col_comm);
        }
        else
        {
            MPI_Bcast(B_, b_row * b_col, MPI_FLOAT, k, col_comm);
        }

        for (int ii = 0; ii < b_row; ++ii)
        {
            for (int jj = 0; jj < b_col; ++jj)
            {
                for (int kk = 0; kk < b_col; ++kk)
                {
                    C(ii, jj) += A_(ii, kk) * B_(kk, jj);
                }
            }
        }
    }
    // End computing

    // Start gathering
    if (rank == 0)
    {
        for (int i = 0; i < b_row; ++i)
        {
            for (int j = 0; j < b_col; ++j)
            {
                C_0((i + my_coord[0] * b_row), (j + my_coord[1] * b_col)) = C(i, j);
            }
        }
        for (int i = 1; i < size; ++i)
        {
            int other_coord[2];
            MPI_Cart_coords(grid_comm, i, 2, other_coord);
            for (int j = 0; j < b_row; ++j)
            {
                MPI_Recv(&C_0((other_coord[0] * b_row + j), (other_coord[1] * b_col)), b_col, MPI_FLOAT, i, j, grid_comm, MPI_STATUS_IGNORE);
            }
        }
    }
    else
    {
        for (int j = 0; j < b_row; ++j)
        {
            MPI_Send(C + j * b_col, b_row, MPI_FLOAT, 0, j, grid_comm);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // End gathering

#ifdef PRINT
    if (rank == 0)
    {
        printf("C:\n");
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                printf("%.6f\t", C_0(i, j));
            }
            printf("\n");
        }
    }
#endif
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

#ifndef PRINT
    printf("rank: %d, %.3f ms\n", rank, (double)(end - start) * 1e3);
#endif
    if (rank == 0)
    {
        free(A_0);
        free(B_0);
        free(C_0);
    }
    free(A);
    free(B);
    free(C);
    free(A_);
    free(B_);
    MPI_Finalize();
    return 0;
}