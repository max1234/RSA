#include "algorithms.h"

Algorithms::Algorithms()
{
    shortPrimes = new unsigned long[PRIMELIMITS];
    Eratosthen();
}

void Algorithms::Eratosthen()
{
    unsigned long i, j, k;
    bool *prime = new bool[PRIMELIMITS];
    for(i = 0; i < PRIMELIMITS; i++)
        prime[i] = true;
    prime[0] = prime[1] = false;
    for(i = 2, k = 0; i < PRIMELIMITS; i++)
        if(prime[i])
        {
            for(j = i + i; j < PRIMELIMITS; j += i)
                prime[j] = false;
            shortPrimes[k++] = i;
        }
    shortPrimes[k] = 0;
    maxPrimeIndex = k - 1;
    free(prime);
}

void Algorithms::GeneratePrime(longz number, unsigned int numBits, unsigned int randomaiser)
{
    printf("Start\n");
    unsigned int i;
    bool isComposite;
    char *prime_str = new char[numBits + 1];

    longz buf;
    longz_init(buf);

    srand(randomaiser);
    prime_str[0] = '1';
    printf("before cycle\n");
    char *data1 = new char[1000];
    char *data2 = new char[1000];

    while(true)
    {
        prime_str[0] = '1';
        for(i = 1; i < numBits - 1; i++)
            prime_str[i] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        prime_str[numBits - 1] = '1';
        prime_str[numBits] = '\0';

        longz_set_str(number, prime_str, 2);

        printf("\nprime_str = %s\n", prime_str);
        for(isComposite = false, i = 0; i <= maxPrimeIndex; i++)
        {
            if(longz_cmp_ui(number,shortPrimes[i]) <= 0)
                break;
            longz_get_str(data1, 16, buf);
            longz_get_str(data2, 16, number);
            longz_mod_ui(buf, number, shortPrimes[i]);
            if(longz_cmp_ui(buf,(unsigned long int)0) == 0)
            {
                isComposite = true;
                break;
            }
        }
        if(isComposite)
            continue;
            if(MillerRabin(number, numBits, NUMBERTESTS))
                break;
    }
    printf("end\n");
    longz_clear(buf);
    free(prime_str);
}
bool Algorithms::MillerRabin(longz number, unsigned int numBits, unsigned int numTest)
{
    printf("Start Miller\n");
    unsigned int i, j, k;
    char *a_str;

    longz buf1, buf2, t, gcd, a, c;
    longz_init(buf1);
    longz_init(buf2);
    longz_init(t);
    longz_init(gcd);
    longz_init(a);
    longz_init(c);

    if(longz_cmp_ui(number,(unsigned long)2) == 0)
    {
        longz_clear(buf1);
        longz_clear(buf2);
        longz_clear(t);
        longz_clear(gcd);
        longz_clear(a);
        longz_clear(c);
        return true;
    }

    longz_mod_ui(buf1, number, (unsigned long)2);
    if(longz_cmp_ui(buf1,(unsigned long)0) == 0)
    {
        longz_clear(buf1);
        longz_clear(buf2);
        longz_clear(t);
        longz_clear(gcd);
        longz_clear(a);
        longz_clear(c);
        return false;
    }
    unsigned long s = 0;
    longz_sub_ui(t,number,(unsigned long)1);

    longz_mod_ui(buf1, t, (unsigned long)2);
    while(longz_cmp_ui(buf1,(unsigned long)0) == 0)
    {
        longz_cpy(buf2, t);
        longz_div_ui(t, buf1, buf2, (unsigned long)2);
        longz_mod_ui(buf1, t, (unsigned long)2);
        s++;
    }

    a_str = new char[numBits];
    for(i = 0; i < numTest; i++)
    {
        for(k = 0; k < numBits - 1; k++)
            a_str[k] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        a_str[k] = '\0';
        longz_set_str(a,a_str,2);
        longz_gcd(gcd,a,number);
        if(longz_cmp_ui(gcd,(unsigned long)1) != 0)
        {
            longz_clear(buf1);
            longz_clear(buf2);
            longz_clear(t);
            longz_clear(gcd);
            longz_clear(a);
            longz_clear(c);
            return false;
        }
        longz_powm(c, a, t, number);
        if(longz_cmp_ui(c,(unsigned long)1) == 0)
            continue;
        longz_sub_ui(buf1,number,(unsigned long)1);


        if(longz_cmp(c,buf1) == 0)
            continue;

        bool f = true;
         for(j = 0; j < s - 1; j++)
        {
            longz_cpy(buf2, c);
            longz_powm_ui(c, buf2,(unsigned long)2, number);

            if(longz_cmp(c, buf1) == 0)
                f = false;
        }

        if(f)
        {
            longz_clear(buf1);
            longz_clear(buf2);
            longz_clear(t);
            longz_clear(gcd);
            longz_clear(a);
            longz_clear(c);
            return false;
        }
    }
    longz_clear(buf1);
    longz_clear(buf2);
    longz_clear(t);
    longz_clear(gcd);
    longz_clear(a);
    longz_clear(c);

    free(a_str);

    return true;
}
