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
    for (i = 1; i < size; i <<= 1) {
        if (rank % (i << 1) == 0) {
            // recive message and sum
            int recv_data;
            MPI_Recv(&recv_data, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, &status);
            my_value += recv_data;
        } else if (rank % i == 0) {
            MPI_Send(&my_value, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
            // send self val
        } else {
            // do nothing
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    printf("rank: %d, value: %d\n", rank, my_value);
    for (int i = size; i > 1; i >>= 1)
    {
        if (rank % i == 0) {
            // send message
            MPI_Send(&my_value, 1, MPI_INT, rank + (i >> 1), 0, MPI_COMM_WORLD);
        } else if (rank % (i >> 1) == 0) {
            // recv self val
            MPI_Recv(&my_value, 1, MPI_INT, rank - (i >> 1), 0, MPI_COMM_WORLD, &status);
        } else {
            // do nothing
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    printf("rank: %d, value: %d\n", rank, my_value);
    
    MPI_Finalize();
    return 0;
}