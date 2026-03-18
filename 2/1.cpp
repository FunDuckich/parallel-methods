#include <stdio.h>
#include <omp.h>

void check_mode() {
    if (omp_in_parallel()) {
        printf("parallel region\n");
    } else {
        printf("sequential region\n");
    }
}

int main() {
    check_mode();

    #pragma omp parallel
    {
        #pragma omp master
        {
            check_mode();
        }
    }
}