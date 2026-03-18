#include <stdio.h> 
#include <omp.h>   


int main() {
    printf("before the parallel region, only the main thread works\n");
    
    #pragma omp parallel
    {
        int thread_num = omp_get_thread_num(); 
        printf("I am thread number %d\n", thread_num);
    }

    printf("the parallel region is closed\n");
}