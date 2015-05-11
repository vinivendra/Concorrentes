

#include <vector>

#include "math.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Factorial

vector<mpf_class> factorialTable;


void initializeFactorial() {
    factorialTable.clear();
    factorialTable.push_back(1.0);
}


mpf_class factorial(unsigned long x) {

    if (x >= factorialTable.size()) {
        factorialTable.push_back(factorial(x - 1) * x);
    }

    return factorialTable[x];
}

////////////////////////////////////////////////////////////////////////////////
// Power

vector<mpf_class> powerTable;
mpf_class powerBase = 0;


void initializePower(mpf_class x) {
    powerBase = x;
    powerTable.clear();
    powerTable.push_back(1.0);
}


mpf_class power(unsigned long x) {

    if (x >= powerTable.size()) {
        powerTable.push_back(power(x - 1) * powerBase);
    }

    return powerTable[x];
}
