#include <stdio.h>
#include <omp.h>
#include "func.h"

int main() {
    double a = 0.0;
    double b = 1000000.0; 
    int N = 100000000;
    double h = (b - a) / (2.0 * N);

    double sum1 = 0.0;
    double sum2 = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum1, sum2)
    for (int k = 1; k <= N; k++) {
        double x1 = a + (2.0 * k - 1.0) * h;
        sum1 += f(x1);

        if (k < N) {
            double x2 = a + 2.0 * k * h;
            sum2 += f(x2);
        }
    }

    double integral = (h / 3.0) * (f(a) + f(b) + 4.0 * sum1 + 2.0 * sum2);
    
    double end_time = omp_get_wtime();

    double pi_approx = integral * 2.0;

    printf("calculated integral: %.10f\n", integral);
    printf("pi approximation: %.10f\n", pi_approx);
    printf("calculation time: %f sec\n", end_time - start_time);
}