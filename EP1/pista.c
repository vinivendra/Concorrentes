 /*---------------------------------------------*
  * EP1 -  MAC0438 - Programacao Concorrente    *
  * Alunos:                                     *
  *   Helena Almeida Victoretti - 7991124       *
  *   Vinicius Vendramini - 7991103             *
  *---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "pista.h"


/*--------------------------------------------------------------
  Cria a matriz pista que irá armazenar a posiçãos ciclistas a 
  cada metro andado na pista.
*/
int **cria_pista()
{
	int i;
	int j;

	int **pista = malloc(tamanho_pista * sizeof(int *));
	for (i = 0; i < tamanho_pista; i++)
		pista[i] = malloc(CICLISTAS_METRO * sizeof(int));

	for (i = 0; i < tamanho_pista; i++)
		for (j = 0; j < CICLISTAS_METRO; j++)
			pista[i][j] = -1;

	return pista;
}

/*----------------------------------------------------------------
  Recebe uma posicao e um ciclista. Se a posicao nao esta ocupada
  com 4 ciclistas, insere o ciclista naquela posicao e retorna 1.
  Caso contrário retorna 0.
*/
int insere_na_pista(int posicao, int ciclista)
{
	int i;

	for (i = 0; i < CICLISTAS_METRO; i++)
		if (pista[posicao][i] == -1)
		{
			pista[posicao][i] = ciclista;
			return 1;
		}

	return 0;
}

/*--------------------------------------------------------------
  Recebe uma posicao e um ciclistas e remove o ciclista daquela
  posicao na pista.
*/
void retira_da_pista(int posicao, int ciclista)
{
	int i;

	for (i = 0; i < CICLISTAS_METRO; i++)
		if (pista[posicao][i] == ciclista)
			pista[posicao][i] = -1;
}

/*--------------------------------------------------------------
  Recebe o indentificador de um ciclista e retorna a posicao em
  que o ciclista esta na pista.
*/
int posicao_ciclista(int ciclista)
{
	int i;
	int j;

	for (i = 0; i< tamanho_pista; i++)
		for (j =0; j < CICLISTAS_METRO; j++)
			if (pista[i][j] == ciclista)
				return i;

	return -1;		
}

/*--------------------------------------------------------------
  Retorna o identificador do ultimo ciclista na pista.
*/
int ultimo_na_pista()
{
	int i;
	int j;

	for (i = tamanho_pista-1; i >= 0; i--)
		for (j = CICLISTAS_METRO-1; j >= 0; j--)
			if (pista[i][j] != -1)
				return pista[i][j];

	return -1;
}

/*--------------------------------------------------------------
  Retorna o identificador do penultimo ciclista na pista.
*/
int penultimo_na_pista()
{
	int i;
	int j;
	int k = 0;

	for (i = tamanho_pista-1; i >= 0; i--)
		for (j = CICLISTAS_METRO-1; j >= 0; j--)
			if (pista[i][j] != -1)
			{
				if (k == 0)
					k++;
				else
					return pista[i][j];
			}

	return -1;
}

/*--------------------------------------------------------------
  Retorna o identificador do antipenultimo ciclista na pista.
*/
int antipenultimo_na_pista()
{
	int i;
	int j;
	int k = 0;

	for (i = tamanho_pista-1; i >= 0; i--)
		for (j = CICLISTAS_METRO-1; j >= 0; j--)
			if (pista[i][j] != -1)
			{
				if (k < 2)
					k++;
				else
					return pista[i][j];
			}

	return -1;
}


/*--------------------------------------------------------------
  Imprime o estado atual dos ciclistas na pista.
*/
void imprime_pista()
{
	int i;
	int j;

	for (i = 0; i < tamanho_pista; i++)
	{
		for (j = 0; j < CICLISTAS_METRO; j++)
			printf("%d   ", pista[i][j]);
		printf("\n");
	}
}
