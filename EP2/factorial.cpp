

#include "factorial.h"


double factorial(double x) {
    double factorial = 1;

    for (int i = 1; i <= x; i++) {
        factorial *= i;
    }

    return factorial;
}