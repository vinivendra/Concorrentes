#include <pthread.h>
#include <iostream>
#include <vector>

#include "thread.h"
#include "monitor.h"


using namespace std;

vector<pthread_t> filosofos;
int n; // quantidade de filosofos
vector<int> pesos;
int r; // quantidade de porcoes
bool deve_ser_uniforme; // se true os filosofos comem porcoes uniformes, se false
						// os filosofos comem porcoes proporcionais a seu peso.



void *filosofo(void *id);


void set_n(int novo) {
    n = novo;
}

void set_peso(int peso) {
	pesos.push_back(peso);
}

void set_porcoes(int p) {
	r = p;
}

void set_deve_ser_uniforme(bool u) {
	deve_ser_uniforme = u;
}

void cria_threads() {

	for (int i = 0; i < n; i++) {
        pthread_t f;

        if (pthread_create(&f, nullptr, filosofo, (void *)i))
            printf("\n ERROR creating thread %d\n", i + 1);

        filosofos.push_back(f);
    }
}

void junta_threads() {
    for (int i = 0; i < n; i++) {
        pthread_join(filosofos[i], NULL);
    }
}


void *filosofo(void *id) {

	int i = (int)id;


}
        