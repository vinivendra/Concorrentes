
#ifndef __EP2__factorial__
#define __EP2__factorial__

#include "gmpxx.h"

void initializeFactorial();
void updateFactorials();
mpf_class factorial(unsigned long x);

void initializePower(mpf_class x);
mpf_class power(unsigned long x);

#endif /* defined(__EP2__factorial__) */
