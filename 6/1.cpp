#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    printf("process %d of %d\n", id, count);

    if (id == 0) {
        printf("C interface: MPI_Comm_rank(MPI_COMM_WORLD, &id)\n");
        printf("C++ bindings: MPI::COMM_WORLD.Get_rank()\n");
        printf("modern programs usually use the C interface from C++ code\n");
    }

    MPI_Finalize();
}
