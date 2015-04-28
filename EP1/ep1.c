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
#include <semaphore.h>
#include "pista.h"
#include "thread.h"
#include "ep1.h"


int main(int argc, char *argv[])
{
 	  pthread_t tid_e; /* Thread elimina */

	  if (argc < 3 || argc > 5)
	  {
		    mostre_uso(argv[0]);
        return -1;
	  }
	  /* Obtem os dados da entrada. */
   	tamanho_pista = atoi(argv[1]);
   	quant_ciclistas = atoi(argv[2]);
   	if (argc >= 4)
   	{
   		  if (argv[3][0] == 'u')
   			    u = 1;
   		  else 
   			    u = 0;

   		  if (argv[3][0] == 'v')
   			    v = 1;
   		  else 
   			    v = 0;

        if (argv[3][0] == 'b')
            b = 1;
        else
            b = 0;
   	}
   	else /* por default utiliza velocidades aleatorias e sem a opcao de debug. */
   	{
   		  v = 1;
   		  u = 0;
        b = 0;
   	}
    if (argc == 5)
    {
        if (argv[4][0] == 'b')
            b = 1;
        else
            b = 0;
    }

   	/* Verifica se a quantidade de ciclistas não passa de teto(d/2)*/
   	if (quant_ciclistas > tamanho_pista/2 + 1)
   		  quant_ciclistas = (int) tamanho_pista/2 + 1;

   	
    /* Inicialização da simulação. */
    pista = cria_pista();
    inicializa_corrida();
    inicializa_variaveis_globais();
    inicializa_semaforos();

   	/* Cria as threads ciclistas. */
    tids = cria_threads_ciclistas();
    /* Cria a thread elimina */
    tid_e = cria_thread_elimina();

    /* Junta as threads ciclistas. */
    junta_threads(tids, tid_e);

    imprime_classificacao_final();

    pthread_exit(NULL);

   	return 0;	   
}


/*-----------------------------------------------------
  Coloca os ciclistas de forma aleatoria na pista.
*/
void inicializa_corrida()
{
	  int i = 1;    
    int posicao;

    /* Inicializa uma semente. */
    srand(time(NULL));

    while (i <= quant_ciclistas)
    {
        posicao = rand() % quant_ciclistas;
        
        /* Se a posicao estiver vazia, entao coloca
          o ciclista e vai para o proximo. */
        if (pista[posicao][0] == -1)
        {
            pista[posicao][0] = i;
            i++;
        }
    }
}


/*-----------------------------------------------------------------------
  Inicializa as variaveis globis eliminado, classificacao, quebrado, 
  volta e acabou utilizada pelas threads.
*/
void inicializa_variaveis_globais()
{
    int i;

    eliminado = malloc(quant_ciclistas * sizeof(int));
    for (i = 0; i < quant_ciclistas; i++)
       eliminado[i] = 0;
   
    classificacao = malloc(quant_ciclistas * sizeof(int));
    for (i = 0; i < quant_ciclistas; i++)
        classificacao[i] = -1;

    quebrado = malloc(quant_ciclistas * sizeof(int));
    for (i = 0; i < quant_ciclistas; i++)
        quebrado[i] = -1;

    volta = malloc(quant_ciclistas * sizeof(int));
    for (i = 0; i < quant_ciclistas; i++)
       volta[i] = 0;

    acabou = 0;   
}


/*-----------------------------------------------------------------------
  Imprime da saida padrao a classificacao final dos ciclistas apos o 
  termino da corrida.
*/
void imprime_classificacao_final()
{
    int i;

    printf("\nA classificacao final eh:\n");
    printf("O ciclista %3d ficou em   1o lugar - medalha de ouro.\n", classificacao[0]);
    printf("O ciclista %3d ficou em   2o lugar - medalha de prata.\n", classificacao[1]);
    printf("O ciclista %3d ficou em   3o lugar - medalha de bronze.\n", classificacao[2]);
    for (i = 3; i < quant_ciclistas; i++)
    {
        printf("O ciclista %3d ficou em %3do lugar", classificacao[i], i+1);
        if (quebrado[i] == 1)
            printf(" - quebrado.\n");
        else 
            printf(".\n");
    }
}


/*--------------------------------------------------------------
  Imprime do stdout o modo de usar o programa. Os argumetos
  obrigatorios e opcionais.
*/
void mostre_uso(char *nome_prog)
{
    fprintf(stdout, " Uso: %s distancia_pista numero_de_ciclistas "
           			"[v (velocidades_aleatoria) | u (velocidades_uniforme)] "
                "[b (modo_debug)].\n", nome_prog);
}

