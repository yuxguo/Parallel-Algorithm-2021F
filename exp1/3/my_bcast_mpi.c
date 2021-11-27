#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define ROOT 0

int my_bcast_mpi(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int get_color(char *name);
int get_key(int old_rank);

int main(int argc, char **argv)
{
    int rank, size;
    int my_value;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    my_value = rank;
    printf("Before bcast, rank: %d, my_value: %d\n", rank, my_value);
    my_bcast_mpi(&my_value, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    printf("After bcast, rank: %d, my_value: %d\n", rank, my_value);

    MPI_Finalize();
    return 0;
}
int my_bcast_mpi(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int world_rank, world_size;
    int node_rank, node_size;
    MPI_Comm_rank(comm, &world_rank);
    MPI_Comm_size(comm, &world_size);
    int len;
    char name[20];
    MPI_Comm node_comm;
    MPI_Get_processor_name(name, &len);
    MPI_Comm_split(comm, get_color(name), get_key(world_rank), &node_comm);

    MPI_Group world_group, first_node_group;
    MPI_Comm first_comm;
    MPI_Comm_group(comm, &world_group);
    int first_ranks[3] = {root, root, root};
    int first_node_group_num;
    if (root != 0 && root != 32)
    {
        first_ranks[1] = 0;
        first_ranks[2] = 32;
        first_node_group_num = 3;
    }
    else if (root == 0)
    {
        first_ranks[1] = 32;
        first_node_group_num = 2;
    }
    else
    {
        first_ranks[1] = 0;
        first_node_group_num = 2;
    }
    MPI_Group_incl(world_group, first_node_group_num, first_ranks, &first_node_group);
    MPI_Comm_create_group(comm, first_node_group, 0, &first_comm);
    if (first_comm != MPI_COMM_NULL)
    {
        MPI_Bcast(buffer, count, datatype, 0, first_comm);
    }
    MPI_Bcast(buffer, count, datatype, 0, node_comm);
    return 0;
}
int get_color(char *name)
{
    if (!strcmp(name, "node1"))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int get_key(int old_rank)
{
    return old_rank % 2;
}