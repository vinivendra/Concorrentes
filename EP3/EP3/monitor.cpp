#include "monitor.h"


void wait(garfo *cv);
void signal(garfo *cv);
bool empty(garfo *cv);


void pega(garfo *cv) {
    cv->m.lock();
    wait(cv);
    cv->m.unlock();
}


void devolve(garfo *cv) {
    cv->m.lock();
    signal(cv);
    cv->m.unlock();
}


bool tenta(garfo *cv) {
    cv->m.lock();

    if (!empty(cv)) {
        cv->m.unlock();
        return false;
    }

    wait(cv);

    cv->m.unlock();
    return true;
}


void wait(garfo *cv) {
    cv->ocupado++;
    cv->m.unlock();
    cv->privateMutex.lock();
    cv->m.lock();
}


void signal(garfo *cv) {
    cv->ocupado--;
    cv->privateMutex.unlock();
}


bool empty(garfo *cv) {
    return !cv->ocupado;
}
