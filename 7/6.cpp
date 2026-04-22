#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    if (count < 2) {
        if (id == 0) {
            printf("need at least 2 processes\n");
        }

        MPI_Finalize();
    } else {
        if (id == 0) {
            int workers = count - 1;
            int *values = new int[workers];
            MPI_Request *requests = new MPI_Request[workers];
            int *indexes = new int[workers];
            MPI_Status *statuses = new MPI_Status[workers];
            int completed = 0;

            for (int i = 0; i < workers; i++) {
                MPI_Irecv(&values[i], 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &requests[i]);
            }

            while (completed < workers) {
                int ready_count;

                MPI_Waitsome(workers, requests, &ready_count, indexes, statuses);

                for (int i = 0; i < ready_count; i++) {
                    int index = indexes[i];
                    int source = statuses[i].MPI_SOURCE;

                    printf("master received %d from process %d\n", values[index], source);
                    completed++;
                }
            }

            delete[] values;
            delete[] requests;
            delete[] indexes;
            delete[] statuses;
        } else {
            int value = id * id;

            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            printf("slave %d sent %d\n", id, value);
        }

        MPI_Finalize();
    }
}
