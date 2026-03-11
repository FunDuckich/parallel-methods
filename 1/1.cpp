#include <iostream>

using namespace std;

int main() {
#ifdef _OPENMP
    cout << "OpenMP работает" << endl;
    
    int year = _OPENMP / 100;
    int month = _OPENMP % 100;
    
    cout << "версия стандарта OpenMP принята: год " << year << ", месяц " << month << endl;
#else 
    cout << "OpenMP не поддерживается" << endl;
#endif
}