#include <pthread.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

/* Pseudo código do monitor sem levar em considereção os pesos dos filosofos:

monitor ControleGarfos { 
	int garfo_status[n] = (n 1);
	cond garfo[n];  # uma variável de condição para cada garfo.

	procedure pega_garfos(id_filosofo) {
		if (garfo_status[id_filosofo] == 0) # tenta pegar o garfo direito.
			wait(garfo[id_filosofo]);
		else
			garfo_status[id_filoso] = 0; # pega o garfo direito.

		if (garfo_status[id_filosofo + 1] == 0) # tenta pegar o garfo esquerdo.
			wait(garfo[id_filosofo + 1]);
		else
			garfo_status[id_filosofo + 1] = 0; # pega o garfo esquerdo.
	}

	procedure devolve_garfos(id_filosofo) {
		if (empty(garfo[id_filosofo]))
			garfo_status[id_filoso] = 1;
		else 									# passagem de condição
			signal(garfo[id_filoso]);

		if (empty(garfo[id_filoso + 1]))
			garfo_status[id_filoso + 1] = 1;
		else									# passagem de condição
			signal(garfo[id_filosofo + 1]);

	}
}

*/ 
