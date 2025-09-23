/*utils.h */
#pragma once
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic ignored "-Wformat-truncation="
#pragma GCC diagnostic push

#include <stdio.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/* C */
void copy(int8*,int8*,int16);

/* N */
int16 nstoh(int16);

/* P */
void printhex(int8*,int16,int8);


/* Z */
void zero(int8*,int16);


void copy(int8 *dst, int8 *src, int16 size) {
    int8 *d, *s;
    int16 n;

    for (n=size, d=dst, s=src; n; n--, d++, s++)
        *d = *s;

    return;
}

int16 nstoh(int16 srcport) {
    int16 dstport;  
    int8 a, b;

    a = ((srcport & 0xff00) >> 8);
    b = (srcport & 0xff);
    dstport = 0;
    dstport = (b << 8) + a;

    return dstport;
}

void zero(int8 *str, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=str; n<size; n++, p++)
        *p = 0;

    return;
}

void printhex(int8 *str, int16 size, int8 delim) {
    int8 *p;
    int16 n;

    for (p=str, n=size; n; n--, p++) {
        printf("%.02x", *p);
        if (delim)
            printf("%c", delim);
        fflush(stdout);
    }
    printf("\n");

    return;
}