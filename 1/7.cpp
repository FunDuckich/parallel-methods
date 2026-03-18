#include <stdio.h>
#include <omp.h>

int main() {
    printf("default OMP_DYNAMIC value: %d\n", omp_get_dynamic());

    omp_set_dynamic(1);
    
    printf("new OMP_DYNAMIC value: %d\n", omp_get_dynamic());

    #pragma omp parallel
    {
        #pragma omp master
        {
            printf("executing MASTER section (thread 0). actual number of threads: %d\n", omp_get_num_threads());
        }
    }
}