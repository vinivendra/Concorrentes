

#include <vector>

#include "math.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Factorial

vector<double> factorialTable;
bool factorialFirstTime = false;


void initializeFactorial() {
    if (factorialFirstTime) {
        factorialTable = vector<double>();
    } else {
        factorialFirstTime = true;
        factorialTable.clear();
    }

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
bool powerFirstTime = false;
double powerBase = 0;


void initializePower(double x) {
    powerBase = x;

    if (powerFirstTime) {
        powerTable = vector<double>();
    } else {
        powerFirstTime = true;
        powerTable.clear();
    }

    powerTable.push_back(1.0);
}


double power(unsigned long x) {

    if (x >= powerTable.size()) {
        powerTable.push_back(power(x - 1) * powerBase);
    }

    return powerTable[x];
}

