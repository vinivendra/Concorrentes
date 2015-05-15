

#include "thread.h"

#include <cstdlib>
#include <cmath>
#include <pthread.h>

#include <iostream>
#include <condition_variable>

#include "math.h"


using namespace std;


unsigned long q = 1;
double x;
double error;

vector<pthread_t> threads;

vector<double> terms;
double cosine;

vector<mutex *> arrive;

condition_variable goCV;
mutex goMutex;
bool goReady;
bool goRound;

bool done;

mutex *cosineMutex;
mutex *printArrivalMutex;

bool isFirstArrival = true;
bool shouldprintArrival = false;
unsigned long numberOfRounds;

void *threadFunction(void *id);
void printArrival(unsigned long i);

unsigned long getQ() {
    return q;
}

void setQ(unsigned long newValue) {
    q = newValue;
}

double getX() {
    return x;
}

void setX(double newValue) {
    x = newValue;
}

double getError() {
    return error;
}

void setError(double newValue) {
    error = newValue;
}

void setShouldprintArrival(bool newValue) {
    shouldprintArrival = newValue;
}

void initializeEnvironment() {
    threads.clear();

    initializeMathWithPowerBase(M_PI / 3);

    terms.clear();
    for (unsigned long i = 0; i < q; i++)
        terms.push_back(0.0);

    cosine = 0;

    goRound = true;
    goReady = true;

    done = false;

    isFirstArrival = true;
}

void startThreads() {
    for (unsigned long i = 0; i < q; i++) {
        pthread_t thread;

        if (pthread_create(&thread, nullptr, threadFunction, (void *)i))
            printf("\n ERROR creating thread %ld\n", i + 1);

        threads.push_back(thread);
    }
}


void initializeSemaphores() {

    for (unsigned long i = 0; i < q; i++) {

        mutex *a = new mutex();
        a->lock();
        arrive.push_back(a);
    }

    cosineMutex = new mutex();
    printArrivalMutex = new mutex();
}


void *threadFunction(void *id) {

    unsigned long i = (unsigned long)id;

    double sign = -1;
    double numerator;
    double denominator;

    unsigned long n;

    for (n = i; true; n += q) {
        sign = (!(n % 2)) * 2 - 1;

        numerator = power(i);
        denominator = factorial(i);
        terms[i] = sign * numerator / denominator;

        if (abs(terms[i]) < error)
            done = true;

        cosineMutex->lock();
        cosine += terms[i];
        cosineMutex->unlock();

        if (i == q - 1) {

            printArrival(i);

            for (unsigned long i = 0; i < q - 1; i++)
                arrive[i]->lock();

            updateMath();

            if (shouldprintArrival) {
                isFirstArrival = true;
                cout << "\n";
            }

            cout << "cosine: " << cosine << "\n"; // TODO: This should be
                                                  // conditional

            goRound = !goRound;
            goReady = goRound;
            goCV.notify_all();

            if (done)
                break;

        } else {
            unique_lock<mutex> lock(goMutex);

            printArrival(i);

            if (goRound) {
                arrive[i]->unlock();
                goCV.wait(lock, [] { return !goReady; });
            } else {
                arrive[i]->unlock();
                goCV.wait(lock, [] { return goReady; });
            }

            if (done)
                break;
        }
    }

    if (i == q - 1) {
        numberOfRounds = n + 1;
    }

    return 0;
}

void printArrival(unsigned long i) {
    printArrivalMutex->lock();
    if (isFirstArrival) {
        isFirstArrival = false;
        cout << "Ordem de chegada das threads: ";
    }

    cout << i << " ";
    printArrivalMutex->unlock();
}

void joinThreads() {
    for (unsigned long i = 0; i < q; i++) {
        pthread_join(threads[i], NULL);
    }
}

void printInformation() {
    cout << "=========== Results ==========\n";

    cout << "Cosine = " << cosine << "\n";

    cout << "Number of rounds = " << numberOfRounds << "\n";
}
