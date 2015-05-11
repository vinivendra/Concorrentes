

#include <cmath>

#include "cosine.h"
#include "math.h"


mpf_class cosine(mpf_class x, mpf_class error) {
    mpf_class cosine = 0;
    mpf_class sign = -1;
    mpf_class numerator;
    mpf_class denominator;

    mpf_class term = error + 1;

    initializeFactorial();
    initializePower(x);

    for (unsigned int n = 0; abs(term) >= error; n++) {
        sign *= -1;
        numerator = power(2 * n);
        denominator = factorial(2 * n);
        term = sign * numerator / denominator;
        cosine += term;
    }

    return cosine;
}
