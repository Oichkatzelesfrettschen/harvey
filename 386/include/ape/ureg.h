
#pragma once
#include <stdint.h>

struct Ureg {
    unsigned long di; /* general registers */
    unsigned long si; /* ... */
    unsigned long bp; /* ... */
    unsigned long nsp;
    unsigned long bx;    /* ... */
    unsigned long dx;    /* ... */
    unsigned long cx;    /* ... */
    unsigned long ax;    /* ... */
    unsigned long gs;    /* data segments */
    unsigned long fs;    /* ... */
    unsigned long es;    /* ... */
    unsigned long ds;    /* ... */
    unsigned long trap;  /* trap type */
    unsigned long ecode; /* error code (or zero) */
    unsigned long pc;    /* pc */
    unsigned long cs;    /* old context */
    unsigned long flags; /* old flags */
    union {
        unsigned long usp;
        unsigned long sp;
    };
    unsigned long ss; /* old stack segment */
};
