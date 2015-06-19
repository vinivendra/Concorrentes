
#include "monitor.h"


void wait(garfo *g);
void signal(garfo *g);



void pega(garfo *g) {
    g->m.lock();
    g->ocupado++;
    wait(g);
    g->m.unlock();
}


void devolve(garfo *g) {
    g->m.lock();
    g->ocupado--;
    signal(g);
    g->m.unlock();
}


bool tenta(garfo *g) {
    g->m.lock();

    if (g->ocupado) {
        g->m.unlock();
        return false;
    }

    g->ocupado++;
    wait(g);

    g->m.unlock();
    return true;
}


void wait(garfo *g) {

    g->m.unlock();
    g->privateMutex.lock();
    g->m.lock();
}


void signal(garfo *g) {

    g->privateMutex.unlock();
}

