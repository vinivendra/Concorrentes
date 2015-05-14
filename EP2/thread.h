

#ifndef __EP2__thread__
#define __EP2__thread__

#include "gmpxx.h"
#include <vector>
#include <mutex>

unsigned long getQ();
void setQ(unsigned long newValue);
double getX();
void setX(double newValue);
double getError();
void setError(double newValue);

void initializeSemaphores();
void initializeThreads();
void joinThreads();

#endif /* defined(__EP2__thread__) */
