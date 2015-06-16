#include <pthread.h>
#include <iostream>
#include <vector>
#include <unistd.h>

#include "thread.h"
#include "monitor.h"


using namespace std;

vector<pthread_t> filosofos;
int quant_f; // quantidade de filosofos
vector<int> pesos;
int r; // quantidade de porcoes
bool deve_ser_uniforme; // se true os filosofos comem porcoes uniformes, se false
						// os filosofos comem porcoes proporcionais a seu peso.

vector<int> porcoes_filosofos;
bool done;
pthread_mutex_t mutex;



void *filosofo(void *id);
float rand_12();
void destroi_threads(int f);


void set_n(int novo) {
    quant_f = novo;
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

void inicializa_ambiente() {

    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < quant_f; i++)
        porcoes_filosofos.push_back(0);
}

void cria_threads() {

	for (int i = 0; i <quant_f; i++) {
        pthread_t f;

        if (pthread_create(&f, nullptr, filosofo, (void *)i))
            printf("\n ERROR creating thread %d\n", i + 1);

        filosofos.push_back(f);
    }
}

void junta_threads() {
    for (int i = 0; i < quant_f; i++) {
        pthread_join(filosofos[i], NULL);
    }
}

// Destroi todas as threads menos a que chamou.
void destroi_threads(int f) {
    for (int i = 0; i < quant_f; i++) {
        if (i != f)
            pthread_cancel(filosofos[i]);
    }
}


void *filosofo(void *id) {

	int i = (int)id;

    while (!done) {
        sleep(rand_12()); //pensa
        
        pthread_mutex_lock(&mutex);
        cout << "Quer comer " << i << "\n";
        pthread_mutex_unlock(&mutex);
        
        pega_garfos(i);
        
        sleep(0.7); // come em tempo igual para todos

        pthread_mutex_lock(&mutex);
        porcoes_filosofos[i]++;
        r--;
        if (r <= 0) {
            done = true;
            //destroi_threads(i);
        }
        pthread_mutex_unlock(&mutex);
        
        devolve_garfos(i);

        pthread_mutex_lock(&mutex);
        cout << "Devolveu " << i << "\n";
        pthread_mutex_unlock(&mutex);
    
    }

    return NULL;

}


float rand_12() {

    float r = (float)rand() / (float)RAND_MAX + 1;
    return r;
}

void imprime_status_final() {
    
    for (int i = 0; i < quant_f; i++) {
        cout << "O filosofo " << i << " comeu " << porcoes_filosofos[i] << " porcoes.\n";
    }
}

