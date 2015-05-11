

#include <cmath>
#include <iostream>

#include "cosine.h"
#include "math.h"

using namespace std;


mpf_class cosine(mpf_class x, mpf_class error) {
    mpf_class cosine = 0;
    mpf_class sign = -1;
    mpf_class numerator;
    mpf_class denominator;

    mpf_class term = error + 1;

    initializeFactorial();
    initializePower(x);

    bool flag = 0;

    for (unsigned int n = 0; abs(term) >= error; n++) {

        sign *= -1;
        numerator = power(flag);
        denominator = factorial(flag);
        term = sign * numerator / denominator;
        cosine += term;

        if (flag) {
            updateFactorials();
            updatePowers();
        }

        flag = !flag;
    }

    return cosine;
}
