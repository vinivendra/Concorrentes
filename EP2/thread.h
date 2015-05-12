

#ifndef __EP2__thread__
#define __EP2__thread__

#include "gmpxx.h"

unsigned long getQ();
void setQ(unsigned long newValue);
mpf_class getX();
void setX(mpf_class newValue);
mpf_class getError();
void setError(mpf_class newValue);

void initializeSemaphores();
void initializeThreads();
void joinThreads();

#endif /* defined(__EP2__thread__) */
