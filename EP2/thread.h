

#ifndef __EP2__thread__
#define __EP2__thread__

unsigned long getQ();
void setQ(unsigned long newValue);

void initializeSemaphores();
void initializeThreads();
void joinThreads();

#endif /* defined(__EP2__thread__) */
