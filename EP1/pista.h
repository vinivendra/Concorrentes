 /*---------------------------------------------*
  * EP1 -  MAC0438 - Programacao Concorrente    *
  * Alunos:                                     *
  *   Helena Almeida Victoretti - 7991124       *
  *   Vinicius Vendramini - 7991103             *
  *---------------------------------------------*/


/* Quantidade de ciclistas que podem ocupar o mesmo metro
   da pista eh 4. */
#define CICLISTAS_METRO 4

/* Variaveis globais */
int **pista;        
int tamanho_pista;  

int **cria_pista();
int insere_na_pista(int posicao, int ciclista);
void retira_da_pista(int posicao, int ciclista);
int posicao_ciclista(int ciclista);
int ultimo_na_pista();
int penultimo_na_pista();
int antipenultimo_na_pista();
void imprime_pista();

