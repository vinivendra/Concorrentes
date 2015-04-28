 /*---------------------------------------------*
  * EP1 -  MAC0438 - Programacao Concorrente    *
  * Alunos:                                     *
  *   Helena Almeida Victoretti - 7991124       *
  *   Vinicius Vendramini - 7991103             *
  *---------------------------------------------*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include "pista.h"
#include "thread.h"

/*-----------------------------------------------------------------
  Cria as quant_ciclistas threads ciclistas que serao responsaveis
  por simular os ciclistas na pista.
*/
pthread_t *cria_threads_ciclistas()
{
	int i;
	pthread_t *tids = malloc(quant_ciclistas * sizeof(pthread_t));

	for (i = 0; i < quant_ciclistas; i++)
		if (pthread_create(&tids[i], NULL, ciclista, (void *)(i+1))) 
   		{
      		printf("\n ERROR creating thread %d\n", i+1);
      		exit(1);
   		}

   	return tids;
}

/*--------------------------------------------------------------
  Cria a thread elimina que sera responsavel a cada volta por
  eliminar o ultimo ciclista e tambem por eliminar os ciclistas
  que quebrarem.
*/
pthread_t cria_thread_elimina()
{
	pthread_t tid_e;

	if (pthread_create(&tid_e, NULL, elimina, NULL)) 
   	{
      	printf("\n ERROR creating thread elimina \n");
      	exit(1);
   	}

   	return tid_e;
}

/*-------------------------------------------------------------------
  Junta as threads criadas.
*/
void junta_threads(pthread_t *tids, pthread_t tid_e)
{
	int i;

	for (i = 0; i < quant_ciclistas; i++)
		if (pthread_join(tids[i], NULL))  
   		{
      		printf("\n ERROR joining thread \n");
      		exit(1);
   		}

   	if (pthread_join(tid_e, NULL))  
   	{
      	printf("\n ERROR joining thread \n");
      	exit(1);
   	}

}

/*-----------------------------------------------------------------
  Thread ciclista: A cada iteracao, ou seja a cada metro andado
  na pista tenta avancar uma posicao, se nao houver ciclistas na
  sua frente consegue avancar, caso contrario nao.
  Se a opcao por velocidades aleatorias estiver sendo utilizada
  a cada volta o ciclista escolhe de forma aleatoria se sua 
  velocidade sera 50 ou 25km/h. Se andar a 50km/h a cada iteracao
  pode avancar um metro, se andar a 25km/h a cada duas 
  iteracoes pode avandcar um metro.
*/
void *ciclista(void *id)
{
	int ciclista;
	int posicao_atual;
	int posicao_nova;
	int distancia;
	int velocidade;
	int um_metro;
	
	ciclista = (int)id;
	if (v)
	{
		velocidade = 0; /* se velocidade = 0, então o ciclista está andando a 25km/h, 
						   se velocidade = 1, então o ciclista está andando a 50km/h. */
		um_metro = 0;   /* se o ciclista esta andando a 25km/h então ele avanca só meio
						   metro a cada iteração. */
	}

	/* Inicializa uma semente.*/
  	srand (time(NULL));
	
	sem_wait(&mutex);
	posicao_atual = posicao_ciclista(ciclista);
	sem_post(&mutex);
	
	/* A cada volta eh execuatado uma iteracao desse while. */
	while (!acabou)
	{
		distancia = 0;

		/* A cada metro andado eh executada uma iteracao desse while. */
		while (distancia < tamanho_pista)
		{
			/* Se as velocidades são uniformes ou o ciclista está andando a 50km/h ou
			   se já andou um metro a 25km/h, então ele pode tentar avancar uma posicao. */
			if (u || velocidade || um_metro)
			{
				
				/* Como a pista eh uma variavel compartilhada por todas as
				   threads seu acesso precisa ser controlado por semaforos. */
				sem_wait(&mutex);

				/* Tenta avancar um metro na pista. Ou seja subir uma posicao */
				posicao_atual = posicao_ciclista(ciclista);
				if (posicao_atual == 0)
					posicao_nova = tamanho_pista - 1;
				else 
					posicao_nova = posicao_atual - 1;

				/* Se conseguiu avancar, retira seu identificador, da posicao
					anterior da pista. */
				if (insere_na_pista(posicao_nova, ciclista))
					retira_da_pista(posicao_atual, ciclista);

				sem_post(&mutex);

				distancia++;

				/* Espera todos os ciclistas tentarem avancar um metro. */
				barreira_metro(ciclista);
			}
			else
				um_metro = 1;
		}

		/* Escolhe a velocidade da volta seguinte. */
		if (v)
			velocidade = rand() % 2;

		/* Deu uma volta na pista. */
		volta[ciclista-1]++;

		/* Avisa que chegou na barreira, ou seja completou uma volta. */
		sem_post(&arrive[ciclista-1]); 
		/* Espera a thread elimina eliminar o ultimo colocado. */
		sem_wait(&go[ciclista-1]);

	}

	return NULL;
}


/*----------------------------------------------------------------------
  Thread Elimina: Responsavel por eliminar o ultimo ciclista da corrida
  a cada volta. E por verificar se algum ciclista quebrou e elimina-lo
  da prova.
*/
void *elimina(void *arg)
{
	int ultimo;
	int penultimo;
	int antipenultimo;
	int posicao_ultimo;
	int volta = 0;
	int class_ciclista = quant_ciclistas-1;
	int quebra;
	int ciclista_q;
	int posicao_quebrado;
	int i;

	/* Inicializa uma semente.*/
  	srand (time(NULL));

	while (!acabou)
	{
		volta++;

		/* Espera os ciclistas darem uma volta. */
		for (i = 0; i < quant_ciclistas; i++)
			if (!eliminado[i])
				sem_wait(&arrive[i]);

		/* Elimina o ciclista que quebrou. */
		if ((volta % 4) == 0) /* A cada 4 voltas há chance de quebra. */
		{
			/* Probabilidade de 1% de quebra. Escolhe um numero aleatorio
			   entre 0 e 99, se o numero escolhido for 0, entao algum
			   ciclista vai quebrar nessa volta. */
			quebra = rand() % 100;
			if (quebra == 0 && class_ciclista > 3)
			{
				/* Escolhe aleatoriamente o ciclista que quebrou.*/
				ciclista_q = rand() % quant_ciclistas + 1;
				/* Verifica se o ciclista escolhido, ja nao foi
				   eliminado. */
				while (eliminado[ciclista_q-1] == 1)
					ciclista_q = rand() % quant_ciclistas + 1;
				
				/* Elimina o ciclista que quebrou. */
				eliminado[ciclista_q-1] = 1;

				/* Salva a classificacao final do ciclista. */
				classificacao[class_ciclista] = ciclista_q;
				quebrado[class_ciclista] = 1;
				class_ciclista--;
				
				/* Como a pista eh uma variavel compartilhada por todas as
				   threads seu acesso precisa ser controlado por semaforos. */
				sem_wait(&mutex);
				/* Retira o ciclista da pista. */
				posicao_quebrado = posicao_ciclista(ciclista_q);
				retira_da_pista(posicao_quebrado, ciclista_q);
				
				sem_post(&mutex);

				printf("O ciclista %d quebrou na volta %d. Sua posicao " 
					   "na pista era %d.\n", ciclista_q, volta, posicao_quebrado+1);

				/* Destroi a thread correspondente ao ciclista */
				if (pthread_cancel(tids[ciclista_q-1]))
				{
					printf("Erro ao destruir a thread ciclista %d.\n", ciclista_q);
      				exit(3);
				}							
			}
		}

		/* A cada duas voltas um ciclista eh eliminado */
		if (volta % 2 == 0) 
		{
			/* Como a pista eh uma variavel compartilhada por todas as
		   	   threads seu acesso precisa ser controlado por semaforos. */
			sem_wait(&mutex);
			/* Elimina do ultimo colocado */
			ultimo = ultimo_na_pista();
			penultimo = penultimo_na_pista();
			antipenultimo = antipenultimo_na_pista();
			eliminado[ultimo-1] = 1;

			/* Salva a classificacao final do ciclista. */
			classificacao[class_ciclista] = ultimo;
			class_ciclista--;

			posicao_ultimo = posicao_ciclista(ultimo);
			retira_da_pista(posicao_ultimo, ultimo);

			sem_post(&mutex);
		
			printf("O ciclista %d foi eliminado na volta %d.\n", ultimo, volta);
			if (penultimo != -1 && antipenultimo != -1)
				printf("Os ciclistas extamente a frente deste sao %d e %d.\n", penultimo, antipenultimo);
			else if (penultimo != -1)
				printf("O ciclista examente a frente deste eh %d.\n", penultimo);

			/* Destroi a thread correspondente ao ciclista */
			if (pthread_cancel(tids[ultimo-1]))
			{
				printf("Erro ao destruir a thread ciclista %d.\n", ultimo);
      			exit(3);
			}			
		}

		/* Verifica se todos os ciclistas já foram eliminados,
		   ou seja se já acabou a corrida. */
		if (class_ciclista == 0)
		{
			classificacao[0] = penultimo;
			acabou = 1;
		}
			

		/* Todos os ciclistas precisam esperar o juiz eliminar o ultimo
		  	colocado. */
		for (i = 0; i < quant_ciclistas; i++)
			if (!eliminado[i])
				sem_post(&go[i]); /* Libera os ciclistas. */
	}

	return NULL;
}

/*-------------------------------------------------------------------
  Thread Debug: Responsavel por imprimir a cada 14,4 segundos a 
  configuracao atual da pista e a volta em que cada ciclista se
  encontra. Se a opcao de debug estiver setada.
*/
float tempo = 0;

void *debug()
{
	int i;
	int posicao;

		printf("\n --- configuracao atual da corrida ---\n");
		for (i = 0; i < quant_ciclistas; i++)
			if (!eliminado[i])
			{
				posicao = posicao_ciclista(i+1);
				printf("O ciclista %d esta na volta %d. ", i+1, volta[i]);
				printf("E sua posicao na pista eh %d.\n", posicao);
			}
		printf("\n");	

	return NULL;	
}


/*--------------------------------------------------------------
	Inicializa os semaforos utilizados para controlar os acessos
	as regioes criticas. E para sincronizas os ciclistas a cada
	volta na pista.
*/
void inicializa_semaforos()
{
	int i;
	int j;

	/* Inicializa semaforo mutex com 1, reponsavel por controlar
       o acesso a regiao critica */
   	if (sem_init(&mutex, 0, 1))
   	{
   		printf("Erro ao criar o semaforo.\n");
      	exit(2);
   	}

   	/* Inicializa semaforos arrive e go com 0. Eles serao 
   	   responsaveis por sincronizar os processos a cada volta. */
   	arrive = malloc(quant_ciclistas * sizeof(sem_t));
  	for (i = 0; i < quant_ciclistas; i++)
  		if (sem_init(&arrive[i], 0, 0)) 
      	{
      		printf("Erro ao criar o semaforo.\n");
        	exit(2);
      	}

    go = malloc(quant_ciclistas * sizeof(sem_t));
  	for (i = 0; i < quant_ciclistas; i++)
  		if (sem_init(&go[i], 0, 0)) 
      	{
      		printf("Erro ao criar o semaforo.\n");
        	exit(2);
      	}

    /* Inicializa semaforos arrive_metro 0. Eles serao responsaveis 
       por sincronizar os processos a cada metro andado na pista. */
    arrive_metro = malloc(quant_ciclistas * sizeof(sem_t *));
	for (i = 0; i < quant_ciclistas; i++)
		arrive_metro[i] = malloc(quant_ciclistas * sizeof(sem_t));

	for (i = 0; i < quant_ciclistas; i++)
		for (j = 0; j < quant_ciclistas; j++)
			if (sem_init(&arrive_metro[i][j], 0, 0))
   			{
   				printf("Erro ao criar o semaforo.\n");
      			exit(2);
   			}
}


/*------------------------------------------------------------
  Essa barreira foi implementada usando uma matriz de
  semaforos. Cada coluna e cada linha representa um ciclista.
  O semaforo ij indica para o ciclista j, se o ciclista i
  ja chegou na barreira.
  Logo quando o ciclista i chegar na barreira, faz um 
  V(arrive_metro[i][j]) para todo j, ou seja avisa  todos os
  outros ciclistas que chegou na barreira.
  Para o ciclista i sair da barreira ele espera que todos os
  ciclistas tenham chego na barreira, então faz:
  P(arrive[j][i]) para todo j.
*/
void barreira_metro(int ciclista)
{
	int i;
	
	if (ciclista == 1 && b)
	{
	    tempo += 0.072;
	    if (tempo >= 14.4) {
	        tempo = 0;
            sem_wait(&mutex);	        
	        debug();
	        sem_post(&mutex);
	    }
	}

	for (i = 0; i < quant_ciclistas; i++)
		/* Se o ciclista ainda nao foi eliminado ou nao eh o proprio ciclista. */
		if (!eliminado[i] && i != (ciclista-1))
			sem_post(&arrive_metro[ciclista-1][i]);

	for (i = 0; i < quant_ciclistas; i++)
		/* Se o ciclista ainda nao foi eliminado ou nao eh o proprio ciclista. */
		if (!eliminado[i] && i != (ciclista-1))
			sem_wait(&arrive_metro[i][ciclista-1]);			
}





