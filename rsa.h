#ifndef RSA_H
#define RSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <gmp.h>
#include "algorithms.h"
#include "long.h"

class RSA
{
public:
    RSA();

    void GenerateKeys(FILE *PublicKey, FILE *PrivateKey, FILE *Randomize, unsigned int numBits);
    void Encrypt(FILE *Input, FILE *Output, FILE *PublicKey);
    void Decrypt(FILE *Input, FILE *Output, FILE *PrivateKey);

private:
    longz d,e,n;
    longz M,C;

    void PrintStr(char *str, FILE *file, unsigned int numBytes);
    void ConvertStr(char *inputStr, char *outputStr, unsigned int length);
};

#endif
