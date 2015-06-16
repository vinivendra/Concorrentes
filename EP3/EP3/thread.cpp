#include <pthread.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <unistd.h>

#include "thread.h"
#include "monitor.h"


using namespace std;

vector<pthread_t> filosofos;
int n; // quantidade de filosofos
vector<int> pesos;
int r; // quantidade de porcoes
int porcoesFaltando;
mutex comeMutex;
bool deve_ser_uniforme; // se true os filosofos comem porcoes uniformes, se
                        // false
                        // os filosofos comem porcoes proporcionais a seu peso.
vector<Garfo *> garfos;
mutex printMutex;

void *filosofo(void *id);
void come();
void pensa();


void set_n(int novo) {
    n = novo;
}

void set_peso(int peso) {
    pesos.push_back(peso);
}

void set_porcoes(int p) {
    r = p;
    porcoesFaltando = p;
}

void set_deve_ser_uniforme(bool u) {
    deve_ser_uniforme = u;
}

void cria_threads() {

    garfos.clear();
    filosofos.clear();

    for (long int i = 0; i < n; i++) {
        pthread_t f;

        if (pthread_create(&f, nullptr, filosofo, (void *)i))
            printf("\n ERROR creating thread %ld\n", i + 1);

        filosofos.push_back(f);
        garfos.push_back(new Garfo());
    }
}

void junta_threads() {
    for (int i = 0; i < n; i++) {
        pthread_join(filosofos[i], NULL);
    }
}


void *filosofo(void *id) {

    long int i = (long int)id;

    Garfo *garfo_da_esquerda = garfos[i];
    Garfo *garfo_da_direita = garfos[(i + 1) % n];

    bool acabou = false;


    while (!acabou) {
        pensa();

        while (true) {
            garfo_da_esquerda->pega();

            printMutex.lock();
            cout << id << ": pega o " << i << "\n";
            printMutex.unlock();

            if (garfo_da_direita->tenta()) {
                printMutex.lock();
                cout << id << ": pega o " << (i + 1) % n << "\n";
                printMutex.unlock();

                come();

                printMutex.lock();
                cout << id << ": come "
                     << "\n";
                cout << id << ": faltam " << porcoesFaltando << "\n";
                printMutex.unlock();

                if (porcoesFaltando <= 0) {
                    acabou = true;

                    printMutex.lock();
                    cout << id << ": acabou "
                         << "\n";
                    printMutex.unlock();
                }

                break;
            } else {
                printMutex.lock();
                cout << id << ": não rolou; devolve o " << (i + 1) % n << "\n";
                printMutex.unlock();

                // TODO: Dorme um pouquinho aleatoriamente, para evitar a
                // sincronia.

                garfo_da_esquerda->devolve();
            }
        }
    }

    return NULL;
}


void come() {
    comeMutex.lock();
    if (porcoesFaltando > 0) {
        porcoesFaltando--;
    }
    comeMutex.unlock();
}

void pensa() {
    // TODO: fazer o pensa() dormir aleatorio
    sleep(1);
}

/*

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