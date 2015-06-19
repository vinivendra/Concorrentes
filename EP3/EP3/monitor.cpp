
#include "monitor.h"


void wait(garfo *g);
void signal(garfo *g);
bool empty(garfo *g);


void pega(garfo *g) {
    g->m.lock();
    wait(g);
    g->m.unlock();
}


void devolve(garfo *g) {
    g->m.lock();
    signal(g);
    g->m.unlock();
}


bool tenta(garfo *g) {
    g->m.lock();

    if (!empty(g)) {
        g->m.unlock();
        return false;
    }

    wait(g);

    g->m.unlock();
    return true;
}


void wait(garfo *g) {
    g->ocupado++;
    g->m.unlock();
    g->privateMutex.lock();
    g->m.lock();
}


void signal(garfo *g) {
    g->ocupado--;
    g->privateMutex.unlock();
}


bool empty(garfo *g) {
    return !g->ocupado;
}

