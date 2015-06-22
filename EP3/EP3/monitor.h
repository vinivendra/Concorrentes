#include <pthread.h>
#include <iostream>
#include <mutex>


using namespace std;


struct garfo {
    mutex privateMutex;
    mutex m;
    int ocupado = false;
};

void pega(garfo *cv);
void devolve(garfo *cv);
bool tenta(garfo *cv);
