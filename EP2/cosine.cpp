

#include <cmath>

#include "cosine.h"
#include "factorial.h"


double cosine(double x, double error) {
    double cosine = 0;
    double sign = -1;
    double numerator;
    double denominator;

    double term = INFINITY;

    for (double n = 0; fabs(term) >= error; n++) {
        sign *= -1;
        numerator = pow(x, 2 * n);
        denominator = factorial(2 * n);
        term = sign * numerator / denominator;
        cosine += term;
    }

    return cosine;
}