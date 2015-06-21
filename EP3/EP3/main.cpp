#include <iostream>
#include <fstream>
#include <vector>

#include "thread.h"
#include "monitor.h"

using namespace std;

void mostre_uso(const char *nome_prog);

int main(int argc, const char *argv[]) {

    int i;
    int n; // Quantidade de filosofos.
    int aux;
    vector<int> pesos;
    int r;          // Quantidade de porcoes.

    FILE *infile;

    if (argc != 4) {
        mostre_uso(argv[0]);
        return -1;
    }

    infile = fopen(argv[1], "r");
    if (infile == NULL) {
        cout << "Não consegui abrir o arquivo " << argv[1] << "\n";
        return -2;
    }
    else {
        fscanf(infile, "%d", &n);
        set_n(n);
        for (i = 0; i < n; i++)
            if (fscanf(infile, "%d", &aux) != EOF) {
                set_peso(aux);
            }
    }

    fclose(infile);

    r = atoi(argv[2]);
    set_porcoes(r);

    if (argv[3][0] == 'U' || argv[3][0] == 'u') {
        set_deve_ser_uniforme(true);
    } else if (argv[3][0] == 'P' || argv[3][0] == 'p') {
        set_deve_ser_uniforme(false);
    }

    cria_threads();
    junta_threads();
}


void mostre_uso(const char *nome_prog) {
    cout << " Uso: " << nome_prog << " arquivo R(quantidade de porcoes) "
         << "U(uniforme)|P(proprocional ao peso) \n";
}