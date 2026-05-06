#include <stdio.h>
#include <math.h>
#include <omp.h>

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

double calculate_default(int N) {
    double h = 1.0 / N;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
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

double calculate_static(int N) {
    double h = 1.0 / N;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum) schedule(static)
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

double calculate_dynamic(int N) {
    double h = 1.0 / N;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum) schedule(dynamic)
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

void print_result(const char *name, double value, double time, double serial_time, int threads) {
    double speedup = serial_time / time;
    double efficiency = speedup / threads;

    printf("%s value: %.10f\n", name, value);
    printf("%s time: %f sec\n", name, time);
    printf("%s speedup: %.4f\n", name, speedup);
    printf("%s efficiency: %.4f\n", name, efficiency);
}

int main() {
    int values[] = {100, 150, 200};
    int threads = omp_get_max_threads();

    omp_set_dynamic(0);

    for (int i = 0; i < 3; i++) {
        int N = values[i];

        printf("N = %d\n", N);
        printf("threads: %d\n", threads);

        double start = omp_get_wtime();
        double serial_value = calculate_serial(N);
        double serial_time = omp_get_wtime() - start;

        printf("serial value: %.10f\n", serial_value);
        printf("serial time: %f sec\n", serial_time);

        start = omp_get_wtime();
        double default_value = calculate_default(N);
        double default_time = omp_get_wtime() - start;
        print_result("default", default_value, default_time, serial_time, threads);

        start = omp_get_wtime();
        double static_value = calculate_static(N);
        double static_time = omp_get_wtime() - start;
        print_result("static", static_value, static_time, serial_time, threads);

        start = omp_get_wtime();
        double dynamic_value = calculate_dynamic(N);
        double dynamic_time = omp_get_wtime() - start;
        print_result("dynamic", dynamic_value, dynamic_time, serial_time, threads);

        printf("\n");
    }
}
