#include <stdio.h>
#include <math.h>
#include <mpi.h>

double get_value(int m, int n, int N) {
    double sum = 0.0;

    for (int k = 1; k <= N; k++) {
        sum += (sin(m + k) + cos(n - 2 * k)) / (1.0 + (m - n) * (m - n) + k * k);
    }

    return sum;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;
    int N = 6;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    if (count < 2) {
        if (id == 0) {
            printf("need at least 2 processes\n");
        }

        MPI_Finalize();
    } else {
        int workers = count - 1;

        if (id == 0) {
            double B[6][6];
            double *rows = new double[N * N];
            MPI_Request *requests = new MPI_Request[workers];
            int *start_rows = new int[workers];
            int *row_counts = new int[workers];
            int completed = 0;

            for (int i = 0; i < workers; i++) {
                start_rows[i] = i * N / workers;
                row_counts[i] = (i + 1) * N / workers - start_rows[i];

                MPI_Irecv(rows + start_rows[i] * N, row_counts[i] * N, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD, &requests[i]);
            }

            while (completed < workers) {
                int index;

                MPI_Waitany(workers, requests, &index, MPI_STATUS_IGNORE);

                for (int i = 0; i < row_counts[index]; i++) {
                    int row = start_rows[index] + i;

                    for (int j = 0; j < N; j++) {
                        B[j][row] = rows[row * N + j];
                    }
                }

                printf("master received rows from %d to %d\n", start_rows[index] + 1, start_rows[index] + row_counts[index]);
                completed++;
            }

            printf("matrix B:\n");

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    printf("%8.3f ", B[i][j]);
                }

                printf("\n");
            }

            delete[] rows;
            delete[] requests;
            delete[] start_rows;
            delete[] row_counts;
        } else {
            int start_row = (id - 1) * N / workers;
            int row_count = id * N / workers - start_row;
            double *rows = new double[row_count * N];

            for (int i = 0; i < row_count; i++) {
                int m = start_row + i + 1;

                for (int j = 0; j < N; j++) {
                    int n = j + 1;
                    rows[i * N + j] = get_value(m, n, N);
                }
            }

            MPI_Send(rows, row_count * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

            printf("slave %d sent rows from %d to %d\n", id, start_row + 1, start_row + row_count);

            delete[] rows;
        }

        MPI_Finalize();
    }
}
