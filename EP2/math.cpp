

#include <vector>

#include "math.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Factorial

vector<double> factorialTable;


void initializeFactorial() {
    factorialTable.clear();
    factorialTable.push_back(1.0);
}


double factorial(unsigned long x) {

    if (x >= factorialTable.size()) {
        factorialTable.push_back(factorial(x - 1) * x);
    }

    return factorialTable[x];
}

////////////////////////////////////////////////////////////////////////////////
// Power

vector<double> powerTable;
double powerBase = 0;


void initializePower(double x) {
    powerBase = x;
    powerTable.clear();
    powerTable.push_back(1.0);
}


double power(unsigned long x) {

    if (x >= powerTable.size()) {
        powerTable.push_back(power(x - 1) * powerBase);
    }

    return powerTable[x];
}
