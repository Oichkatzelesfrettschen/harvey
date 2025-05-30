#pragma once
#include <stdint.h>

/*
 * Machine register state used by the kernel when servicing traps or
 * interrupts. Matches the layout expected by trap handlers.
 */
struct Ureg {
    ulong di; /* general registers */
    ulong si; /* ... */
    ulong bp; /* ... */
    ulong nsp;
    ulong bx;    /* ... */
    ulong dx;    /* ... */
    ulong cx;    /* ... */
    ulong ax;    /* ... */
    ulong gs;    /* data segments */
    ulong fs;    /* ... */
    ulong es;    /* ... */
    ulong ds;    /* ... */
    ulong trap;  /* trap type */
    ulong ecode; /* error code (or zero) */
    ulong pc;    /* pc */
    ulong cs;    /* old context */
    ulong flags; /* old flags */
    union {
        ulong usp;
        ulong sp;
    };
    ulong ss; /* old stack segment */
};
