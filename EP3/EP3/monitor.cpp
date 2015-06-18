
#include "monitor.h"


void wait(variavel_condicional *cv);
void signal(variavel_condicional *cv);


Garfo::Garfo() {
    this->cv.m = new mutex();
    this->cv.privateMutex = new mutex();
}


void Garfo::pega() {
    cv.m->lock();
    ocupado++;
    wait(&cv);
    cv.m->unlock();
}


void Garfo::devolve() {
    cv.m->lock();
    ocupado--;
    signal(&cv);
    cv.m->unlock();
}


bool Garfo::tenta() {
    cv.m->lock();

    if (ocupado) {
        cv.m->unlock();
        return false;
    }

    ocupado++;
    wait(&cv);
    cv.m->unlock();
    return true;
}


void wait(variavel_condicional *cv) {

    cv->m->unlock();
    cv->privateMutex->lock();
    cv->m->lock();
}


void signal(variavel_condicional *cv) {

    cv->privateMutex->unlock();
}

/* Pseudo código do monitor sem levar em considereção os pesos dos filosofos:

struct variavel_condicional {
    mutex private
    bool busy
    mutex busyMutex
    mutex m
}

monitor Garfo {
    variavel_condicional cv
    bool ocupado = false

    procedure pega() {
        m->lock()
        wait(cv)
        m->unlock()
    }

    procedure devolve() {
        m->lock()
        signal(cv)
        m->unlock()
    }

    procedure tenta() {
        m->lock()

        if busy(cv)
            m->unlock()
            return 0

        wait(cv)
        m->unlock()
    }
}


busy(cv)
    busyMutex->lock()
    bool result = cv.busy
    busyMutex->unlock()

    return result


wait(cv)

    cv.m->unlock()

        busyMutex->lock()
            cv.private.lock()
            cv.busy = true
        busyMutex->unlock()

    cv.m->lock()


signal(cv)

    cv.busy = false
    cv.private.unlock()



while true
    pensa()

    while true
        garfo_da_esquerda.pega()

        if garfo_da_direita.tenta()
            come()
            break
        else
            garfo_da_esquerda.devolve()

*/
