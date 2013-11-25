#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdlib.h>
#include <limits.h>
#include <gmp.h>
#include "long.h"

#define PRIMELIMITS 10000
#define NUMBERTESTS 10

class Algorithms
{
public:
    Algorithms();
    void GeneratePrime(longz number, unsigned int numBits, unsigned int randomaiser);

private:
    unsigned long *shortPrimes;
    unsigned long maxPrimeIndex;
    bool MillerRabin(longz number, unsigned int numBits, unsigned int numTest);
    void Eratosthen();
};

#endif
