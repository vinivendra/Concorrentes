

#include <pthread.h>
#include <iostream>
#include <mutex>
#include <condition_variable>


using namespace std;


typedef struct variavel_condicional {
    int busy;
    mutex *privateMutex;
    mutex *m;
} variavel_condicional;


class Garfo {
    variavel_condicional cv;
    bool ocupado = false;

public:
    Garfo();

    void pega();
    void devolve();
    bool tenta();
};
