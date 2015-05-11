

#include <vector>
#include <iostream>

#include "math.h"

#include "thread.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Factorial

vector<mpf_class> factorialTable;
unsigned long x = 0;


void initializeFactorial() {
    factorialTable.clear();
    factorialTable.push_back(1.0);

    x = 0;

    for (unsigned long i = 1; i < getQ(); i ++) {
        factorialTable.push_back(factorialTable[i-1] * (x+1) * (x+2));
        x += 2;
    }
}


void updateFactorials() {

    factorialTable[0] = factorialTable[getQ()-1] * (x+1) * (x+2);
    x += 2;

    for (unsigned long i = 1; i < getQ(); i ++) {
        factorialTable[i] = (factorialTable[i-1] * (x+1) * (x+2));
        x += 2;
    }
}


mpf_class factorial(unsigned long i) {
    return factorialTable[i];
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
