#include <iostream>
#include <vector>
#include <pthread.h>

#include "monitor.h"

using namespace std;


pthread_mutex_t monitor; // monitor lock
vector<pthread_cond_t> pode_pegar; // variavies condicionais: uma para cada filosofo.
vector<int> esperando;  // conta quantas threads estao esperando da fila de cada
						// variavel de condicao. Utilizado para implementar o empty.
vector<int> garfos; // indica o estado de cada garfo. 1 livre, 0 pego.
int n; // quantidade de grafos.


void wait(int cv);
void signal(int cv);
bool empty(int cv);

void inicializa_monitor(int quant_filosofos) {

	n = quant_filosofos;

	for (int i = 0; i < n; i++) {
		esperando.push_back(0);

		garfos.push_back(1);

		pthread_cond_t cv;
		pthread_cond_init(&cv, NULL); // inicializa as variaveis de condicao.
		pode_pegar.push_back(cv);
	}

	pthread_mutex_init(&monitor, NULL); // inicializa o semaforo que controlo o acesso ao monitor.
}

void pega_garfos(int f) {
	
	pthread_mutex_lock(&monitor);

	/*if (garfos[f] == 0 || garfos[(f+1)%n] == 0)
		wait(f);
	else {
		garfos[f] = 0;
		garfos[(f+1)%n] = 0;
	}*/

	while (garfos[f] == 0 || garfos[(f+1)%n] == 0)
		wait(f);
	
	garfos[f] = 0;
	garfos[(f+1)%n] = 0;

	pthread_mutex_unlock(&monitor);
}

void devolve_garfos(int f) {

	pthread_mutex_lock(&monitor);

	/*if (garfos[(f-1)%n] == 1) { // esta livre o garfo f-1?
		if (!empty((f-1)%n)) // o filosofo f-1 está esperando.
			signal((f-1)%n);
	}
	else
		garfos[f] = 1;

	if (garfos[(f+2)%n] == 1) { // esta livre o garfo f+2?
		if (!empty((f+2)%n)) // o filosofo f+2 está esperando.
			signal((f+2)%n);
	}
	else
		garfos[(f+1)%n] = 1; */

	if (garfos[(f-1)%n] == 1) { // esta livre o garfo f-1?
		//if (!empty((f-1)%n)) // o filosofo f-1 está esperando.
			signal((f-1)%n);
	}	
	garfos[f] = 1;

	if (garfos[(f+2)%n] == 1) { // esta livre o garfo f+2?
		//if (!empty((f+2)%n)) // o filosofo f+1 está esperando.
			signal((f+1)%n);
	}	
	garfos[(f+1)%n] = 1;

	pthread_mutex_unlock(&monitor);
}


void wait(int cv) {
			
	//esperando[cv]++;
	pthread_cond_wait(&pode_pegar[cv], &monitor);
}

void signal(int cv) {
	
	//esperando[cv]--;
	pthread_cond_signal(&pode_pegar[cv]);
}

/*bool empty(int cv) {
	if (esperando[cv] == 0)
		return true;
	else
		return false;
}*/