#include <iostream>
#include <stdio.h>
#include <omp.h>

using namespace std;

int main() {
    int dyn_default = omp_get_dynamic();
    cout << dyn_default << endl;

    omp_set_dynamic(1);
    
    int dyn_new = omp_get_dynamic();
    cout << dyn_new << endl;

    #pragma omp parallel
    {
        #pragma omp master
        {
            int total_threads = omp_get_num_threads();
            printf("Выполняет секцию MASTER (поток 0). Фактическое число потоков: %d\n", total_threads);
        }
        
    }
}