

#include <pthread.h>
#include <iostream>
#include <mutex>


using namespace std;


struct garfo {
    mutex privateMutex;
    mutex m;
    int ocupado = false;
};


void pega(garfo *g);
void devolve(garfo *g);
bool tenta(garfo *g);

