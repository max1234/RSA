#ifndef LONG_H
#define LONG_H

#include <string.h>
#include <stdio.h>
#include <limits.h>

#define LONGZSIZE 2000

typedef struct _longz_
{
    unsigned char * number;
    unsigned long length;
}

_longz;
typedef _longz longz[1];
typedef _longz *longz_ptr;

void longz_init(longz_ptr rec);
void longz_clear(longz_ptr rec);
void longz_cpy(longz_ptr rec, longz source);

void longz_set_str(longz_ptr rec, char *source, unsigned int cap);
void longz_set_ui(longz_ptr rec, unsigned long source);
void longz_get_str(char *rec, unsigned int cap, longz source);

int longz_cmp(longz op1, longz op2);
int longz_cmp_ui(longz op1, unsigned long op2);

void longz_add(longz_ptr rec, longz op1, longz op2);
void longz_add_ui(longz_ptr rec, longz op1, unsigned long op2);

int longz_sub(longz_ptr rec, longz op1, longz op2);
int longz_sub_ui(longz_ptr rec, longz op1, unsigned long op2);

void longz_mul(longz_ptr rec, longz op1, longz op2);
void longz_mul_uc(longz_ptr rec, longz op1, unsigned char op2);
void longz_mul_ui(longz_ptr rec, longz op1, unsigned long op2);

int longz_div(longz_ptr q, longz_ptr r, longz n, longz d);
int longz_div_ui(longz_ptr q, longz_ptr r, longz n, unsigned long d);

void longz_powm(longz_ptr rec, longz base, longz exp, longz mod);
void longz_powm_ui(longz_ptr rec, longz base, unsigned long exp, longz mod);

int longz_mod(longz_ptr rec, longz op1, longz op2);
int longz_mod_ui(longz_ptr rec, longz op1, unsigned long op2);

void longz_gcd(longz_ptr rec, longz op1, longz op2);
void longz_gcd_ui(longz_ptr rec, longz op1, unsigned long op2);

int longz_invert(longz_ptr rec, longz op1, longz op2);

#endif
