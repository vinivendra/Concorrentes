 /*---------------------------------------------*
  * EP1 -  MAC0438 - Programacao Concorrente    *
  * Alunos:                                     *
  *   Helena Almeida Victoretti - 7991124       *
  *   Vinicius Vendramini - 7991103             *
  *---------------------------------------------*/



/* Semaforo para controlar o acesso as variaveis
   compartilhadas. */
sem_t mutex;

/* Semaforos utilizados para implementar barreiras
  de sincronizacao para sincronizar os ciclistas a
  cada volta. */
sem_t *arrive;
sem_t *go;

/* Semaforos utilizados para implementar a barreira
  de sincronizacao para sincronizar ciclistas a cada
  metro andado na pista. */
sem_t **arrive_metro;


/* Threads ciclistas. */
pthread_t *tids;


/* Variaveis globais. */
int quant_ciclistas;
int *eliminado; /* indica os ciclistas que foram eliminados da prova. */
int *classificacao; /* salva a classificação final de cada ciclista. */
int acabou; /* indica que a simulação acabou. */
int *quebrado; /* indica os ciclistas que quebraram. */
int *volta; /* indica a volta que cada ciclista esta. */
int u; /* indica se as velocidades são uniformes */
int v; /* indica se as velocidades são aleatórias */
int b; /* indica se eh para executar com a opcao de debug */


void *ciclista(void *id);
void *elimina(void *arg);

void *debug();

pthread_t *cria_threads_ciclistas();
pthread_t cria_thread_elimina();
void junta_threads(pthread_t *tids, pthread_t tid_e);
void inicializa_semaforos();

void barreira_metro(int ciclista);

