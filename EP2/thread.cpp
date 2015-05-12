

#include "thread.h"

#include <vector>
#include <cstdlib>
#include <iostream>

#include <cmath>

#include <pthread.h>
#include "pthread_barriers.h"
#include <mutex>

#include "math.h"


using namespace std;


unsigned long q = 1;
mpf_class x;
mpf_class error;

vector<pthread_t> threads;
pthread_barrier_t barrier;

vector<vector<mpf_class>> terms;
bool termsRound;

vector<mutex *> powerMutexes;
vector<mutex *> factorialMutexes;
vector<mutex *> arrive;
vector<mutex *> go;

void *threadFunction(void *id);
void initializeBarrier();


unsigned long getQ() {
    return q;
}

void setQ(unsigned long newValue) {
    q = newValue;
}

mpf_class getX() {
    return x;
}

void setX(mpf_class newValue) {
    x = newValue;
}

mpf_class getError() {
    return error;
}

void setError(mpf_class newValue) {
    error = newValue;
}

void initializeThreads() {
    threads.clear();

    initializeFactorial();
    initializePower(M_PI / 3);

    terms.clear();
    terms.push_back(vector<mpf_class>());
    terms.push_back(vector<mpf_class>());
    termsRound = false;

    for (unsigned long i = 0; i < q; i++) {
        terms[0].push_back(0);
        terms[1].push_back(0);

        pthread_t thread;

        int error = pthread_create(&thread, nullptr, threadFunction, (void *)i);

        if (error)
            printf("\n ERROR creating thread %ld\n", i + 1);

        threads.push_back(thread);
    }
}


void initializeSemaphores() {
    pthread_barrier_init(&barrier, NULL, (unsigned int)q);

    powerMutexes.clear();

    for (unsigned long i = 0; i < q; i++) {

        //        sprintf(mutexName, "powerMutexes_%ld", i);
        //        if ((mutex = sem_open(mutexName, O_CREAT, 0644, 1)) ==
        //        SEM_FAILED)
        //            printf("Falha na criação do mutex de potência número
        //            %ld!\n", i);
        //        powerMutexes.push_back(mutex);
        //
        //        sprintf(mutexName, "factorialMutexes_%ld", i);
        //        if ((mutex = sem_open(mutexName, O_CREAT, 0644, 1)) ==
        //        SEM_FAILED)
        //            printf("Falha na criação do mutex de fatorial número
        //            %ld!\n", i);
        //        factorialMutexes.push_back(mutex);

        mutex *a = new mutex();
        a->lock();
        arrive.push_back(a);

        go.push_back(new mutex());
    }
}


void *threadFunction(void *id) {
    printf("hue!\n");

    unsigned long i = (unsigned long)id;

    mpf_class cosine = 0;
    mpf_class sign = -1;
    mpf_class numerator;
    mpf_class denominator;

    terms[termsRound][i] = error + 1;
    terms[!termsRound][i] = error + 1;


    for (unsigned long n = i; abs(terms[!termsRound][i]) >= error; n += q) {
        sign = (!(n % 2)) * 2 - 1;

        // Espera a barreira estar liberada
        go[i]->lock();

        numerator = power(i);
        denominator = factorial(i);

        // Escreve no termo se o controlador terminou de ler
        terms[termsRound][i] = sign * numerator / denominator;

        if (i == q - 1) { // Controlador

            // Espera todo mundo chegar na barreira
            for (unsigned long i = 0; i < q - 1; i++)
                arrive[i]->lock();

            // Manda atualizar os números para os próximos cálculos
            updateFactorials(); // TODO: This should be done in the separate
                                // threads instead of in the controller only,
                                // and syncronized in a similar way
            updatePowers();

            // Troca a rodada
            termsRound = !termsRound;

            // Libera a barreira
            for (unsigned long i = 0; i < q; i++)
                go[i]->unlock();

            // Lê os termos já escritos
            for (unsigned long i = 0; i < q; i++)
                cosine += terms[!termsRound][i];

            // Imprime o cálculo parcial
            cout << "cosine: " << cosine << "\n";

        } else { // Thread normal
            // Avisa a barreira que chegou
            arrive[i]->unlock();
        }
    }

    return 0;
}


void joinThreads() {
    for (unsigned long i = 0; i < q; i++) {
        pthread_join(threads[i], NULL);
    }
}
