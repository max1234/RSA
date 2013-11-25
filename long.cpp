#include "long.h"

void longz_init(longz_ptr rec)
{
    rec->length = 0;
    rec->number = new unsigned char[LONGZSIZE];
}

void longz_clear(longz_ptr rec)
{
    delete rec->number;
}

void longz_cpy(longz_ptr rec, longz source)
{
    rec->length = source->length;
    for(unsigned int i = 0; i < rec->length; i++)
        rec->number[i] = source->number[i];
}

void longz_set_str(longz_ptr rec, char *source, unsigned int cap)
{
    int i, j, k, tmp;
    if(cap == 16)
    {
        rec->length = (strlen(source) % 2 ? strlen(source) / 2 + 1 : strlen(source) / 2);
        for(i = rec->length - 1, j = strlen(source) - 1, tmp; i >= 0; i--, j -= 2)
        {
            if(j <= 0)
                tmp = 0;
            else
                tmp = source[j - 1] - (source[j - 1] >= 'a' ? 'a' - 10 : '0');
            tmp <<= 4;
            tmp |= source[j] - (source[j] >= 'a' ? 'a' - 10 : '0');
            rec->number[i] = (unsigned char)tmp;
        }
    }
    else if(cap == 2)
    {
        rec->length = (strlen(source) % 8 ? strlen(source) / 8 + 1 : strlen(source) / 8);
        for(i = rec->length - 1, j = strlen(source) - 1, tmp; i >= 0; i--, j-= 8)
        {
            for(k = 7, tmp = 0; k >= 0; k--)
                if((j - k) >= 0)
                {
                    tmp |= (source[j - k] - '0') & 1;
                    tmp <<= 1;
                }
            rec->number[i] = (unsigned char)tmp;
        }
    }
}
void longz_set_ui(longz_ptr rec, unsigned long source)
{
    unsigned long tmp = source;
    int i;
    rec->length = 0;
    while(tmp)
    {
        rec->length++;
        for(i = rec->length; i > 0; i--)
            rec->number[i] = rec->number[i - 1];
        rec->number[0] = tmp & 0xFF;
        tmp >>= 8;
    }
}
void longz_get_str(char *rec, unsigned int cap, longz source)
{
    char tmps[2];
    if(cap == 16)
    {
        strcpy(rec,"");
        for(unsigned int i = 0, tmp; i < source->length; i++)
        {
            tmp = ((source->number[i] >> 4) & 0xF);
            if(i || tmp)
            {
                sprintf(tmps,"%01x",tmp);
                strcat(rec,tmps);
            }

            tmp = (source->number[i]  & 0xF);
            sprintf(tmps,"%01x",tmp);
            strcat(rec,tmps);
        }
        sprintf(tmps,"%c",'\0');
        strcat(rec,tmps);
    }
}
int longz_cmp(longz op1, longz op2)
{
    if(op1->length > op2->length)
        return 1;
    else if(op1->length < op2->length)
        return -1;
    else
    {
        for(unsigned int i = 0; i < op1->length; i++)
        {
            if(op1->number[i] > op2->number[i])
                return 1;
            else if (op1->number[i] < op2->number[i])
                return -1;
        }
    }
    return 0;
}
int longz_cmp_ui(longz op1, unsigned long op2)
{
    unsigned long tmp = 0;
    if(op1->length > (sizeof(unsigned long) / sizeof(unsigned char)))
        return 1;

    for(unsigned int i = 0; i < op1->length; i++)
    {
        tmp <<= 8;
        tmp |= op1->number[i];
    }

    if(tmp > op2)
        return 1;
    else if (tmp < op2)
        return -1;
    else
        return 0;
}
void longz_add(longz_ptr rec, longz op1, longz op2)
{
    unsigned long length = (op1->length >= op2->length ? op1->length : op2->length);
    unsigned int tmprec = 0;
    unsigned char tmp1, tmp2;
    for(int i = op1->length - 1, j = op2->length - 1, k = length - 1; k >= 0; i--, j--, k--)
    {
        if(i >= 0)
            tmp1 = op1->number[i];
        else
            tmp1 = 0;

        if(j >= 0)
            tmp2 = op2->number[j];
        else
            tmp2 = 0;

        tmprec += tmp1 + tmp2;
        rec->number[k] = tmprec & 0xFF;
        tmprec >>= 8;
    }
    if(tmprec)
    {
        rec->length = length + 1;
        for(int i = length; i > 0; i--)
        {
            rec->number[i] = rec->number[i - 1];
        }
        rec->number[0] = tmprec;
    }
    else
        rec->length = length;
}
void longz_add_ui(longz_ptr rec, longz op1, unsigned long op2)
{
    unsigned long long tmprec = op2;
    unsigned char tmp1;
    for(int i = op1->length - 1; i >= 0; i--)
    {
        if(i >= 0)
            tmp1 = op1->number[i];
        else
            tmp1 = 0;

        tmprec += tmp1;
        rec->number[i] = tmprec & 0xFF;
        tmprec >>= 8;
    }
    rec->length = op1->length;
    while(tmprec)
    {
        rec->length++;
        for(int i = rec->length - 1; i > 0; i--)
            rec->number[i] = rec->number[i - 1];
        rec->number[0] = tmprec & 0xFF;
        tmprec >>= 8;
    }
}

int longz_sub(longz_ptr rec, longz op1, longz op2)
{
    int sign = 1;
    longz_ptr buf1 = op1, buf2 = op2;

    if(longz_cmp(op1, op2) < 0)
    {
        sign = -1;
        buf1 = op2;
        buf2 = op1;
    }
    if(longz_cmp(op1, op2) == 0)
    {
        rec->length = 0;
        return 0;
    }
    int i, j, tmprec = 0;
    unsigned char tmp1, tmp2;
    rec->length = buf1->length;

    for(i = buf1->length - 1, j = buf2->length - 1; i >= 0; i--, j--)
    {
        tmp1 = buf1->number[i];
        if(j >= 0)
            tmp2 = buf2->number[j];
        else
            tmp2 = 0;

        tmprec += tmp1 - tmp2;
        rec->number[i] = (unsigned int)tmprec & 0xFF;
        if(tmprec >> 8)
            tmprec = -1;
        else
            tmprec = 0;
    }
    if(tmprec == -1)
        return -1;
    for(i = 0; rec->number[i] == 0 && i < buf1->length; i++)
    {}
    for(j = 0; j < buf1->length - i; j++)
        rec->number[j] = rec->number[j + i];
    rec->length = buf1->length - i;
    return sign;
}
int longz_sub_ui(longz_ptr rec, longz op1, unsigned long op2)
{
    if(longz_cmp_ui(op1, op2) < 0)
        return -1;
    if(longz_cmp_ui(op1, op2) == 0)
    {
        rec->length = 0;
        return 0;
    }
    unsigned long tmp2 = op2;
    int i, j, tmprec = 0;
    unsigned char tmp1;
    for(i = op1->length - 1; i >= 0; i--)
    {
        if(i >= 0)
            tmp1 = op1->number[i];
        else
            tmp1 = 0;

        tmprec += tmp1 - (tmp2 & 0xFF);
        rec->number[i] = (unsigned int)tmprec & 0xFF;
        tmp2 >>= 8;
        if(tmprec >> 8)
            tmprec = -1;
        else
            tmprec = 0;
    }
    for(i = 0; rec->number[i] == 0 && i < op1->length; i++)
    {}

    for(j = 0; j < op1->length - i; j++)
        rec->number[j] = rec->number[j + i];
    rec->length = op1->length - i;
    return 0;
}

void longz_mul(longz_ptr rec, longz op1, longz op2)
{
    longz buf1, buf2;
    int i, j;
    longz_init(buf1);
    longz_init(buf2);
    for (i = op2->length - 1; i >= 0; i--)
    {
        longz_mul_uc(buf2, op1, op2->number[i]);

        for(j = (op2->length - 1) - i; j > 0; j--)
            buf2->number[op2->length - 1 + j] = 0;
        buf2->length += (op2->length - 1) - i;

        longz_add(rec, buf1, buf2);
        longz_cpy(buf1, rec);
    }

    longz_clear(buf1);
    longz_clear(buf2);
}

void longz_mul_uc(longz_ptr rec, longz op1, unsigned char op2)
{
    unsigned int tmp = 0;
    for(int i = op1->length - 1; i >= 0; i--)
    {
        tmp += op1->number[i] * op2;
        rec->number[i] = tmp & 0xFF;
        tmp >>= 8;
    }
    if(tmp)
    {
        rec->length = op1->length + 1;
        for(int i = op1->length; i > 0; i--)
        {
            rec->number[i] = rec->number[i - 1];
        }
        rec->number[0] = tmp;
    }
    else
        rec->length = op1->length;

}

void longz_mul_ui(longz_ptr rec, longz op1, unsigned long op2)
{
    longz buf;
    longz_init(buf);
    longz_set_ui(buf, op2);

    longz_mul(rec, op1, buf);

    longz_clear(buf);
}


int longz_div(longz_ptr q, longz_ptr r, longz n, longz d)
{
    longz buf1, buf2;
    int i, j;
    int k;
    int qq;
    int nptr = 0;
    int nr = 0;
    q->length = 0;
    r->length = 0;

    if (d->length == 0)
    {
        return -1;
    }
    if (n->length == 0 || (n->length < d->length))
    {
        q->length = 0;
        longz_cpy(r, n);
        return 0;
    }
    else
    {
        longz_init(buf1);
        longz_init(buf2);

        nr = d->length;

        for(k = 0; k < nr; k++)
            buf1->number[k] = n->number[k];
        buf1->length = nr;
        nptr += nr;
        int fuck = 0;
        do
        {
            fuck ++;
            if (fuck == 43)
                fuck++;
            qq = 1;
            i = buf1->length;
            if (buf1->length == 1 && buf1->number[0] == 0)
            {
                buf1->length = 0;
                i--;


            }
            if ((n->length - nptr) > 0 && i < nr)
            {
                buf1->number[buf1->length++] = n->number[nptr];
                nptr++;
                i++;

                qq = 0;
                if (buf1->length == 1 && buf1->number[0] == 0)
                {
                    buf1->length = 0;
                    i--;
                    q->number[q->length++] = 0;
                    qq = 1;
                }
                while (i < nr && (n->length - nptr))
                {
                    buf1->number[buf1->length++] = n->number[nptr];
                    if ( !qq )
                        q->number[q->length++] = 0;
                    qq = 0;
                    nptr++;
                    i++;
                    if (buf1->number[0] == 0)
                    {
                        buf1->length = 0;
                        i--;
                    }
                }
                if (buf1->length < d->length && (n->length - nptr) == 0)
                {
                    if (!qq)
                        q->number[q->length++] = 0;
                    if (q->length == 0 )
                        q->number[q->length++] = 0;
                    if (buf1->length != 0)
                         longz_cpy(r, buf1);
                    else
                        r->number[r->length++] = 0;

                    longz_clear (buf1);
                    longz_clear (buf2);
                    return 1;
                }

            }

            for (i = 0; i < buf1->length; i++)
            {
                if (buf1->number[i] < d->number[i])
                {
                    if ((n->length - nptr))
                    {

                        buf1->number[buf1->length++] = n->number[nptr];
                        if (q->length && !qq )
                        {
                            q->number[q->length++] = 0;
                            qq = 0;
                        }
                        nptr++;
                        break;
                    }
                    else
                    {
                        if (q->length == 0)
                            q->number[q->length++] = 0;
                        if (!qq)
                            q->number[q->length++] = 0;
                        longz_cpy(r, buf1);

                        longz_clear (buf1);
                        longz_clear (buf2);
                        return 0;
                    }
                }
                else
                    if (buf1->number[i] > d->number[i])
                        break;

            }

            i = 0;
            while (longz_sub(buf2, buf1, d) >= 0)
            {
                longz_cpy(buf1, buf2);
                i++;
            }
            q->number[q->length++] = i;
        }
        while (n->length - nptr);
         longz_cpy(r, buf1);
        if (r->length == 0)
            r->number[r->length++] = 0;
    }

    longz_clear (buf1);
    longz_clear (buf2);

    return 1;
}




int longz_div_ui(longz_ptr q, longz_ptr r, longz n, unsigned long d)
{
    longz buf;
    longz_init(buf);
    longz_set_ui(buf, d);

    return longz_div(q, r, n, buf);

    longz_clear(buf);
}


int longz_mod(longz_ptr rec, longz op1, longz op2)
{
    if(longz_cmp_ui(op2, 0) == 0)
        return -1;
    else if(longz_cmp(op1, op2) < 0)
    {
        longz_cpy(rec, op1);
        return 0;
    }
    else if(longz_cmp(op1, op2) == 0)
    {
        longz_set_ui(rec, (unsigned long)0);
        return 0;
    }
    else
    {
        longz buf1, buf2, buf3;
        long i, j;
        unsigned int nptr2;

        longz_init(buf1);
        longz_init(buf2);
        longz_init(buf3);

        longz_cpy(rec, op1);
        while(longz_cmp(rec, op2) >= 0)
        {

            for(i = 0, nptr2 = op1->length; longz_cmp(rec, op2) >= 0; i++)
            {
                rec->length--;
                nptr2--;
            }
            if(longz_cmp(rec, op2) < 0)
            {
                rec->length++;
                nptr2++;
            }

            buf2->length = 0;

        for(j = 1; longz_cmp(buf2, rec) <= 0 ;j++)
        {
            longz_mul_ui(buf2, op2, (unsigned long)j);
        }

        if(longz_cmp(buf2, rec) > 0)
            longz_mul_uc(buf2, op2, (unsigned long)j - 2);

        else
            longz_mul_uc(buf2, op2, (unsigned long)j - 1);


        longz_sub(buf3, rec, buf2);
        longz_cpy(buf1, buf3);

        for(i = 0; i < op1->length - nptr2; i++)
        {
            buf1->number[buf1->length] = op1->number[nptr2 + i];
            buf1->length++;
        }
        longz_cpy(rec, buf1);

        }

        longz_clear(buf1);
        longz_clear(buf2);
        longz_clear(buf3);
        return 0;
    }
}

int longz_mod_ui(longz_ptr rec, longz op1, unsigned long op2)
{
    int intrec;
    longz buf;
    longz_init(buf);
    longz_set_ui(buf, op2);

    intrec = longz_mod(rec, op1, buf);

    longz_clear(buf);
    return intrec;
}

void longz_powm(longz_ptr rec, longz base, longz exp, longz mod)
{

    longz r, a, n, buf1, buf2, buf3;

    longz_init(r);
    longz_init(a);
    longz_init(n);
    longz_init(buf1);
    longz_init(buf2);
    longz_init(buf3);
    longz_set_ui(r, (unsigned long)1);
    longz_cpy(a, base);
    longz_cpy(n, exp);


    while(longz_cmp_ui(n, (unsigned long)0) > 0)
    {
        if((unsigned char)(n->length > 0 ? n->number[n->length - 1] : 0) % 2 == 0)
        {
            longz_set_ui(buf3, (unsigned long)2);
            longz_div(buf1, buf2, n, buf3);
            longz_cpy(n, buf1);

            longz_mul(buf1, a, a);
            longz_div(buf2, a, buf1, mod);
        }
        else
        {
            longz_sub_ui(buf1, n, (unsigned long)1);
            longz_cpy(n, buf1);

            longz_mul(buf1, r, a);
            longz_div(buf2, r, buf1, mod);
        }
    }

    longz_cpy(rec, r);
    longz_clear(r);
    longz_clear(a);
    longz_clear(n);
    longz_clear(buf1);
    longz_clear(buf2);
    longz_clear(buf3);
}

void longz_powm_ui(longz_ptr rec, longz base, unsigned long exp, longz mod)
{
    longz buf;
    longz_init(buf);
    longz_set_ui(buf, exp);
    longz_powm(rec, base, buf, mod);
    longz_clear(buf);
}

void longz_gcd(longz_ptr rec, longz op1, longz op2)
{
    longz a, b, q, r;

    longz_init(a);
    longz_init(b);
    longz_init(q);
    longz_init(r);

    if(longz_cmp(op1, op2) == 0)
    {
        longz_cpy(rec, op1);
        longz_clear(a);
        longz_clear(b);
        longz_clear(q);
        longz_clear(r);
        return;
    }
    else if(longz_cmp(op1, op2) < 0)
    {
        longz_cpy(a, op2);
        longz_cpy(b, op1);
    }
    else
    {
        longz_cpy(b, op2);
        longz_cpy(a, op1);
    }
    if(longz_cmp_ui(b,(unsigned long)0) == 0)
    {
        longz_cpy(rec, a);
        return;
    }
       while(longz_cmp_ui(b,(unsigned long)0) > 0)
    {
        longz_div(q, r, a, b);
        longz_cpy(a, b);
        longz_cpy(b, r);
    }

    longz_cpy(rec, a);
    longz_clear(a);
    longz_clear(b);
    longz_clear(q);
    longz_clear(r);
}

void longz_gcd_ui(longz_ptr rec, longz op1, unsigned long op2)
{
    longz buf;
    longz_init(buf);
    longz_set_ui(buf, op2);
    longz_gcd(rec, op1, buf);
    longz_clear(buf);
}

int longz_invert(longz_ptr rec, longz op1, longz op2)
{
    longz buf1, buf2, buf3;
    longz_init(buf1);
    longz_init(buf2);
    longz_init(buf3);

    longz_set_ui(rec, (unsigned long)1);

    while(longz_cmp(rec, op2) < 0)
    {
        longz_mul(buf3, rec, op1);

        longz_div(buf1, buf2, buf3, op2);
        if(longz_cmp_ui(buf2, (unsigned long)1) == 0)
        {
            longz_clear(buf1);
            longz_clear(buf2);
            longz_clear(buf3);
            return 1;
        }
        longz_cpy(buf1, rec);
        longz_add_ui(rec, buf1, (unsigned long)1);
    }
    return -1;
    longz_clear(buf1);
    longz_clear(buf2);
    longz_clear(buf3);
}
