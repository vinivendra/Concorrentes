#include <pthread.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <unistd.h>

#include "thread.h"
#include "monitor.h"


using namespace std;


vector<pthread_t> filosofos;
vector<int> porcoes;
vector<int> pesos;
vector<int> maximoPorcoes;
Garfo *garfos;

int n;                  // quantidade de filosofos
int r;                  // quantidade de porcoes
bool deve_ser_uniforme; // se true os filosofos comem porcoes uniformes, se
                        // false os filosofos comem porcoes proporcionais a seu
                        // peso.

int totalPesos;
int porcoesFaltando;

mutex comeMutex;
mutex printMutex;


void *filosofo(void *id);
void come(long int i);
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

    int totalMaximoPorcoes = 0;

    garfos = (Garfo *)malloc(sizeof(Garfo) * n);
    filosofos.clear();
    porcoes.clear();
    totalPesos = 0;

    bool *porcoesCertas = (bool *)malloc(sizeof(bool));

    for (long int i = 0; i < n; i++) {
        porcoes.push_back(0);
        garfos[i] = Garfo();
        totalPesos += pesos[i];
    }

    for (long int i = 0; i < n; i++) {
        porcoesCertas[i] = false;

        float pesoRelativo = (float)pesos[i] / totalPesos;
        maximoPorcoes.push_back(pesoRelativo * r);
        totalMaximoPorcoes += maximoPorcoes[i];
    }

    for (long int i = 0; i < r - totalMaximoPorcoes; i++) {
        int randomIndex = rand() % n;
        if (porcoesCertas[randomIndex]) {
            i--;
        }
        else {
            maximoPorcoes[randomIndex]++;
            porcoesCertas[randomIndex] = true;
        }
    }

    for (long int i = 0; i < n; i++) {
        pthread_t f;

        if (pthread_create(&f, nullptr, filosofo, (void *)i))
            printf("\n ERROR creating thread %ld\n", i + 1);

        filosofos.push_back(f);
    }

    free(porcoesCertas);
}


void junta_threads() {
    for (int i = 0; i < n; i++) {
        pthread_join(filosofos[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        printf("O filosofo %d comeu %d porções.\n", i, porcoes[i]);
    }

    free(garfos);
}


void *filosofo(void *id) {

    long int i = (long int)id;

    Garfo primeiro_garfo;
    Garfo segundo_garfo;

    if (i % 2) {
        primeiro_garfo = garfos[i];
        segundo_garfo = garfos[(i + 1) % n];
    } else {
        primeiro_garfo = garfos[(i + 1) % n];
        segundo_garfo = garfos[i];
    }

    bool acabou = false;


    while (!acabou) {
        pensa();

        printMutex.lock();
        cout << id << ": pensa\n";
        printMutex.unlock();

        while (true) {
            primeiro_garfo.pega();

            printMutex.lock();
            cout << id << ": pega o " << i << "\n";
            printMutex.unlock();

            if (segundo_garfo.tenta()) {
                printMutex.lock();
                cout << id << ": pega o " << (i + 1) % n << "\n";
                printMutex.unlock();

                come(i);

                printMutex.lock();
                cout << id << ": come "
                     << "\n";
                cout << id << ": faltam " << porcoesFaltando << "\n";
                printMutex.unlock();

                if (porcoesFaltando <= 0 || porcoes[i] == maximoPorcoes[i]) {
                    acabou = true;

                    printMutex.lock();
                    cout << id << ": acabou "
                         << "\n";
                    printMutex.unlock();
                }

                primeiro_garfo.devolve();
                segundo_garfo.devolve();

                break;
            } else {
                printMutex.lock();
                cout << id << ": não rolou; devolve o " << (i + 1) % n << "\n";
                printMutex.unlock();

                primeiro_garfo.devolve();
            }
        }
    }

    return NULL;
}


void come(long int i) {
    comeMutex.lock();
    if (porcoesFaltando > 0) {
        porcoesFaltando--;
        porcoes[i]++;
    }
    comeMutex.unlock();
}

void pensa() {
    float mediaEmSegundos = 0.1;
    float variacaoEmSegundos = 0.05;

    float mediaEmMicrossegundos = mediaEmSegundos * 1000000;
    float variacaoEmMicrossegundos = variacaoEmSegundos * 1000000;

    float random = (float)rand() / RAND_MAX;
    float microssegundos = (random * mediaEmMicrossegundos)
                           + (mediaEmMicrossegundos - variacaoEmMicrossegundos);
    unsigned int tempo = microssegundos;

    usleep(tempo);
}
