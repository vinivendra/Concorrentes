

#include <iostream>
#include <cmath>

#include "thread.h"

#include "gmpxx.h"

using namespace std;

// TODO: Contar o numero de rodadas
// TODO: Implementar o modo de comparar valores consecutivos do cosseno.
// TODO: Marcar a ordem em que as threads chegaram na barreira
// TODO: Modo sequencial

// TODO: Colocar a biblioteca de números grandes

// TODO: Análise de desempenho

int main(int argc, const char *argv[]) {

    unsigned long precision = 300; // FIXME: 700000
    mpf_set_default_prec(precision);

    setQ(1);
    setX(M_PI / 3);
    setError(0.000000001); // FIXME: pow(1/2, precision)

    initializeSemaphores();
    initializeThreads();
    joinThreads();

    return 0;
}
