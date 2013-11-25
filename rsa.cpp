#include "rsa.h"

RSA::RSA()
{
}

void RSA::GenerateKeys(FILE *PublicKey, FILE *PrivateKey, FILE *Randomize, unsigned int numBits)
{
    printf("Start\n");
    longz p, q, phi, pDec, qDec;

    Algorithms algo;

    char e_str[100];
    char *n_str = new char[numBits / 4 + 2];
    char *d_str = new char[numBits / 4 + 2];
    char *p_str = new char[numBits / 8 + 2];
    char *q_str = new char[numBits / 8 + 2];

    unsigned int initRandom = 0;

    longz_init(p);
    longz_init(q);
    longz_init(phi);
    longz_init(pDec);
    longz_init(qDec);
    longz_init(d);
    longz_init(e);
    longz_init(n);
    getc(stdin);

    initRandom = ((unsigned int)fgetc(Randomize) << 8) | fgetc(Randomize);
    algo.GeneratePrime(p, numBits / 2, initRandom);
    longz_get_str(p_str,16,p);

    printf("Generated p\n");

    initRandom ^= ((unsigned int)fgetc(Randomize) << 8) | fgetc(Randomize);
    algo.GeneratePrime(q, numBits / 2, initRandom);
    longz_get_str(q_str,16,q);

    printf("Generated q\n");
    getc(stdin);

    longz_mul(n,p,q);
    longz_get_str(n_str,16,n);
    longz_sub_ui(pDec,p,(unsigned long)1);
    longz_sub_ui(qDec,q,(unsigned long)1);
    longz_mul(phi,qDec,pDec);

    initRandom ^= ((unsigned int)fgetc(Randomize) << 8) | fgetc(Randomize);
    srand(initRandom);

    longz gcd;
    longz_init(gcd);

    printf("\ngen e\n");

    char *data1 = new char[1000];
    char *data2 = new char[1000];
    char *data3 = new char[1000];
    char *data4 = new char[1000];

    longz_get_str(data1,16,p);
    longz_get_str(data2,16,q);
    longz_get_str(data3,16,n);
    longz_get_str(data4,16,phi);

    printf("%x\np =%s\nq = %s\nn = %s\nphi = %s\n", numBits, data1, data2, data3, data4);

    getc(stdin);

    unsigned long e_int;
    while(true)
    {
        e_int = 1;
        e_int <<= (int)((numBits < 30 ? numBits : 30) * rand() / (RAND_MAX + 1.0));
        e_int++;
        while(true)
        {

            if((e_int >> (numBits < 30 ? numBits : 30)))
            {
                e_int = 1;
                e_int <<= (int)((numBits < 30 ? numBits : 30) * rand() / (RAND_MAX + 1.0));
                e_int++;
            }
            else
            {
                e_int <<= 1;
                e_int--;
            }

            longz_gcd_ui(gcd,phi,e_int);
            if((longz_cmp_ui(gcd,(unsigned long)1) == 0) && (longz_cmp_ui(phi, e_int) == 1))
            {
                longz_set_ui(e,e_int);
                break;
            }
        }
        printf("e = %d", e_int);
        getc(stdin);

        if(longz_invert(d, e, phi) != 0)
            break;
    }

    longz_get_str(e_str,16,e);
    longz_get_str(d_str,16,d);
    fprintf(PublicKey, "%x\n%s\n%s", numBits, n_str, e_str);
    fprintf(PrivateKey, "%x\n%s\n%s\n%s\n%s", numBits, n_str, p_str, q_str, d_str);
    printf("Stop\n");
    longz_clear(gcd);
    longz_clear(pDec);
    longz_clear(qDec);
    longz_clear(p);
    longz_clear(q);
    longz_clear(phi);
    longz_clear(d);
    longz_clear(e);
    longz_clear(n);
}

void RSA::Encrypt(FILE *Input, FILE *Output, FILE *PublicKey)
{
    printf("Start\n");
    unsigned int i, j;
    unsigned int numBits, inputBytes, outputBytes;

    fscanf(PublicKey,"%x\n", &numBits);

    outputBytes = (numBits % 8 ? numBits / 8 - 1 : numBits / 8);
    inputBytes = (numBits % 8 ? numBits / 8 :  numBits / 8 - 1);

    char *n_str = new char[numBits / 4 + 2];
    char *c_str = new char[inputBytes + 1];
    char *IOstring = new char[outputBytes + 1];
    char e_str[100];

    fscanf(PublicKey,"%s\n%s\n", n_str, e_str);

    longz_init(M);
    longz_init(C);
    longz_init(e);
    longz_init(n);
    longz_set_str(n,n_str,16);
    longz_set_str(e,e_str,16);

    while(!feof(Input))
    {
        for(i = 0, j = 0; i < inputBytes && !feof(Input); i++, j++)
            IOstring[i] = fgetc(Input);
        if(i)
        {
            if(!feof(Input))
                IOstring[j] = '\0';
            else if (j - 1)
                IOstring[--j] = '\0';
            else
                break;
        }
        else
            break;

        char* m_str = new char[j * 2 + 1];
        ConvertStr(IOstring, m_str, j);

        longz_set_str(M,m_str,16);
        longz_powm(C,M,e,n);
        longz_get_str(c_str,16,C);

        PrintStr(c_str, Output, outputBytes);
        free(m_str);
    }

    printf("Stop\n");

    longz_clear(e);
    longz_clear(n);
    longz_clear(M);
    longz_clear(C);
}

void RSA::Decrypt(FILE *Input, FILE *Output, FILE *PrivateKey)
{
    printf("Start\n");
    unsigned int i, j;
    unsigned int numBits, inputBytes, outputBytes;

    fscanf(PrivateKey,"%x\n", &numBits);

    inputBytes = (numBits % 8 ? numBits / 8 - 1 : numBits / 8);
    outputBytes = (numBits % 8 ? numBits / 8 :  numBits / 8 - 1);

    char *n_str = new char[numBits / 4 + 2];
    char *d_str = new char[numBits / 4 + 2];
    char *m_str = new char[outputBytes + 1];
    char *IOstring = new char[inputBytes + 1];

    fscanf(PrivateKey,"%s\n%*s\n%*s\n%s\n", n_str, d_str);

    longz_init(M);
    longz_init(C);
    longz_init(d);
    longz_init(n);
    longz_set_str(n,n_str,16);
    longz_set_str(d,d_str,16);

    while(!feof(Input))
    {
        for(i = j = 0; i < inputBytes && !feof(Input); i++, j++)
            IOstring[j] = fgetc(Input);
        if(i)
        {
            if(!feof(Input))
                IOstring[j] = '\0';
            else if (j - 1)
                IOstring[--j] = '\0';
            else
                break;
        }
        else
            break;

        char* c_str = new char[j * 2 + 1];
        ConvertStr(IOstring, c_str, j);

        longz_set_str(C,c_str,16);
        longz_powm(M,C,d,n);
        longz_get_str(m_str,16,M);
        PrintStr(m_str, Output, outputBytes);
        free(c_str);
    }
    printf("Stop\n");

    longz_clear(d);
    longz_clear(n);
    longz_clear(M);
    longz_clear(C);
}

void RSA::PrintStr(char *str, FILE *file, unsigned int numBytes)
{
    unsigned int i, tmp = numBytes * 2 - strlen(str);
    tmp -= ((numBytes * 2 - strlen(str)) % 2);
    tmp /= 2;
    for(i = 0; i < tmp; i++)
        fprintf(file,"%c", 0);
    for(i = 0; i < strlen(str);)
    {
        if(((numBytes * 2 - strlen(str)) % 2) && !i)
            tmp = 0;
        else
            tmp = str[i] - (str[i++] >= 'a' ? 'a' - 10 : '0');
        tmp <<= 4;
        tmp |= str[i] - (str[i++] >= 'a' ? 'a' - 10 : '0');
        fprintf(file,"%c", tmp);
    }
}

void RSA::ConvertStr(char *inputStr, char *outputStr, unsigned int length)
{
    char tmps[2];
    strcpy(outputStr,"");

    for(unsigned int i = 0, tmp; i < length; i++)
    {
        tmp = ((inputStr[i] >> 4) & 0xF);
        sprintf(tmps,"%01x",tmp);
        strcat(outputStr,tmps);

        tmp = (inputStr[i]  & 0xF);
        sprintf(tmps,"%01x",tmp);
        strcat(outputStr,tmps);
    }
}
