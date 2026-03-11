#include <iostream>
#include <stdio.h> 
#include <omp.h>   

using namespace std;

int main() {
    cout << "до параллельной области работает только главный поток" << endl;
    #pragma omp parallel
    {
        int thread_num = omp_get_thread_num(); 
        
        printf("я поток номер %d\n", thread_num);
    }

    cout << "параллельная область закрылась" << endl;
}