

#include <pthread.h>
#include <iostream>
#include <mutex>
#include <condition_variable>


using namespace std;


typedef struct variavel_condicional {
    mutex privateMutex;
    bool busy;
    mutex busyMutex;
    mutex m;
} variavel_condicional;


class Garfo {
    variavel_condicional cv;
    bool ocupado = false;

public:
    void pega();
    void devolve();
    bool tenta();
};
