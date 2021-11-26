#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;
    int my_value;
    int send_status, recive_status;
    int i;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    my_value = rank + 1;
    // assume size == 2 ^ n
    for (i = 1; i < size; i <<= 1) {
        // send message
        MPI_Send(&my_value, 1, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
        // recv message
        int recv_data;
        MPI_Recv(&recv_data, 1, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);

        MPI_Barrier(MPI_COMM_WORLD);
        my_value += recv_data;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    printf("rank: %d, value: %d\n", rank, my_value);

    MPI_Finalize();
    return 0;
}