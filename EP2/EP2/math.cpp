

#include <vector>
#include <iostream>
#include <mutex>

#include "math.h"

#include "thread.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Factorial

vector<bool> factorialReady;
vector<mutex *> factorialMutexes;
vector<double> factorialTable;
unsigned long n = 0;

vector<bool> powerReady;
vector<mutex *> powerMutexes;
vector<double> powerTable;
double powerBaseSquared = 0;

bool round;


void initializeMathWithPowerBase(double x) {
    powerBaseSquared = x * x;

    n = 0;

    powerTable.clear();
    powerTable.push_back(1.0);

    powerReady.clear();
    powerReady.push_back(true);

    powerMutexes.clear();
    powerMutexes.push_back(new mutex());

    factorialTable.clear();
    factorialTable.push_back(1.0);

    factorialReady.clear();
    factorialReady.push_back(true);

    factorialMutexes.clear();
    factorialMutexes.push_back(new mutex());

    round = true;

    for (unsigned long i = 1; i < getQ(); i++) {
        powerTable.push_back(0.0);
        powerReady.push_back(false);
        powerMutexes.push_back(new mutex());

        factorialTable.push_back(0.0);
        factorialReady.push_back(false);
        factorialMutexes.push_back(new mutex());
    }
}


void updateMath() {
    round = !round;
}


double factorial(unsigned long i) {
    factorialMutexes[i]->lock();

    if (factorialReady[i] == round) {
        factorialMutexes[i]->unlock();
        return factorialTable[i];
    } else {
        if (i == 0) {
            factorialTable[i] = factorialTable[getQ() - 1] * (n + 1) * (n + 2);
            n += 2;
        } else {
            factorialTable[i] = factorial(i - 1) * (n + 1) * (n + 2);
            n += 2;
        }

        factorialReady[i] = round;
        factorialMutexes[i]->unlock();
        return factorialTable[i];
    }
}


double power(unsigned long i) {

    powerMutexes[i]->lock();

    if (powerReady[i] == round) {
        powerMutexes[i]->unlock();
        return powerTable[i];
    } else {
        if (i == 0) {
            powerTable[i] = powerTable[getQ() - 1] * powerBaseSquared;
        } else {
            powerTable[i] = power(i - 1) * powerBaseSquared;
        }

        powerReady[i] = round;
        powerMutexes[i]->unlock();
        return powerTable[i];
    }
}
