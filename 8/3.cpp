#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    return 1.0 / (x * x + (1.0 - x) * (1.0 - x));
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;
    int N = 1000000;
    double a = 0.0;
    double b = 1.0;
    double h = (b - a) / (2.0 * N);
    double local_sum1 = 0.0;
    double local_sum2 = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    for (int k = id + 1; k <= N; k += count) {
        double x = a + (2.0 * k - 1.0) * h;
        local_sum1 += f(x);
    }

    for (int k = id + 1; k <= N - 1; k += count) {
        double x = a + 2.0 * k * h;
        local_sum2 += f(x);
    }

    MPI_Reduce(&local_sum1, &sum1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sum2, &sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (id == 0) {
        double integral = (h / 3.0) * (f(a) + f(b) + 4.0 * sum1 + 2.0 * sum2);
        double exact = acos(-1.0) / 2.0;

        printf("integral: %.12f\n", integral);
        printf("exact: %.12f\n", exact);
        printf("error: %.12f\n", fabs(integral - exact));
    }

    MPI_Finalize();
}
