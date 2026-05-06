#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x, double y, double z) {
    double s = sin(x + y + z);

    return exp(-x) / (2.0 + s * s);
}

double calculate_serial(int N) {
    double h = 1.0 / N;
    double sum = 0.0;

    for (int i = 0; i <= N - 1; i++) {
        int radius = N - i;

        for (int j = -radius; j <= radius; j++) {
            int limit = (int)sqrt((double)(radius * radius - j * j));

            for (int k = -limit; k <= limit; k++) {
                sum += f(h * i, h * j, h * k);
            }
        }
    }

    return h * h * h * sum;
}

double calculate_parallel(int N, int id, int count) {
    double h = 1.0 / N;
    double local_sum = 0.0;
    double sum = 0.0;

    for (int i = id; i <= N - 1; i += count) {
        int radius = N - i;

        for (int j = -radius; j <= radius; j++) {
            int limit = (int)sqrt((double)(radius * radius - j * j));

            for (int k = -limit; k <= limit; k++) {
                local_sum += f(h * i, h * j, h * k);
            }
        }
    }

    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return h * h * h * sum;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;
    int values[] = {100, 150, 200};

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    for (int i = 0; i < 3; i++) {
        int N = values[i];
        double serial_value = 0.0;
        double serial_time = 0.0;

        if (id == 0) {
            printf("N = %d\n", N);
            printf("processes: %d\n", count);

            double start = MPI_Wtime();
            serial_value = calculate_serial(N);
            serial_time = MPI_Wtime() - start;

            printf("serial value: %.10f\n", serial_value);
            printf("serial time: %f sec\n", serial_time);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        double parallel_value = calculate_parallel(N, id, count);
        double parallel_time = MPI_Wtime() - start;

        if (id == 0) {
            double speedup = serial_time / parallel_time;
            double efficiency = speedup / count;

            printf("parallel value: %.10f\n", parallel_value);
            printf("parallel time: %f sec\n", parallel_time);
            printf("speedup: %.4f\n", speedup);
            printf("efficiency: %.4f\n", efficiency);
            printf("\n");
        }
    }

    MPI_Finalize();
}
