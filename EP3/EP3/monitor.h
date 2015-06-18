

#include <pthread.h>
#include <iostream>
#include <mutex>


using namespace std;


typedef struct variavel_condicional {
    mutex *privateMutex;
    mutex *m;
} variavel_condicional;


class Garfo {
    variavel_condicional cv;
    int ocupado = false;

public:
    Garfo();

    void pega();
    void devolve();
    bool tenta();
};
