

#include "thread.h"

#include <pthread.h>
#include "pthread_barriers.h"
#include <semaphore.h>

#include <vector>

using namespace std;


unsigned long q = 1;
vector<pthread_t> threads;
pthread_barrier_t barrier;

void *threadFunction(void *id);
void initializeBarrier();


unsigned long getQ() {
    return q;
}

void setQ(unsigned long newValue) {
    q = newValue;
}



void initializeThreads() {
    threads.clear();

    for (unsigned long i = 0; i < q; i++) {
        pthread_t thread;

        int error
            = pthread_create(&thread, nullptr, threadFunction, (void *)(i + 1));

        if (error)
            printf("\n ERROR creating thread %ld\n", i + 1);

        threads.push_back(thread);
    }
}


void initializeSemaphores() {
    pthread_barrier_init(&barrier, NULL, (unsigned int)q);
}

int round = 0;

void *threadFunction(void *id) {
    printf("hue!\n");

    while (round < q * 10) {
        printf("%d\n", round++);
        pthread_barrier_wait(&barrier);
    }

    return 0;
}


void joinThreads() {
    for (unsigned long i = 0; i < q; i++) {
        pthread_join(threads[i], NULL);
    }
}