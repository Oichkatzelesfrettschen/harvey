#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* Basic type aliases using modern C17 fixed-width types. */
#define nil ((void *)0)
typedef uint16_t ushort;     /* 16-bit unsigned integer */
typedef uint8_t uchar;       /* 8-bit unsigned integer */
typedef unsigned long ulong; /* native unsigned long */
typedef uint32_t uint;       /* 32-bit unsigned integer */
typedef int8_t schar;        /* 8-bit signed integer */
typedef int64_t vlong;       /* 64-bit signed integer */
typedef uint64_t uvlong;     /* 64-bit unsigned integer */
typedef uintptr_t uintptr;   /* unsigned integer the size of a pointer */
typedef size_t usize;        /* alias for size_t */
typedef uint32_t Rune;       /* Unicode code point */

typedef union FPdbleword FPdbleword;
typedef long jmp_buf[2];
#define JMPBUFSP 0
#define JMPBUFPC 1
#define JMPBUFDPC 0
typedef unsigned int mpdigit; /* for /sys/include/mp.h */
typedef uint8_t u8int;        /* 8-bit unsigned integer */
typedef uint16_t u16int;      /* 16-bit unsigned integer */
typedef uint32_t u32int;      /* 32-bit unsigned integer */
typedef uint64_t u64int;      /* 64-bit unsigned integer */

/* FCR */
#define FPINEX (1 << 5)
#define FPUNFL ((1 << 4) | (1 << 1))
#define FPOVFL (1 << 3)
#define FPZDIV (1 << 2)
#define FPINVAL (1 << 0)
#define FPRNR (0 << 10)
#define FPRZ (3 << 10)
#define FPRPINF (2 << 10)
#define FPRNINF (1 << 10)
#define FPRMASK (3 << 10)
#define FPPEXT (3 << 8)
#define FPPSGL (0 << 8)
#define FPPDBL (2 << 8)
#define FPPMASK (3 << 8)
/* FSR */
#define FPAINEX FPINEX
#define FPAOVFL FPOVFL
#define FPAUNFL FPUNFL
#define FPAZDIV FPZDIV
#define FPAINVAL FPINVAL
union FPdbleword {
    double x;
    struct {
        uint32_t lo; /* little-endian low word */
        uint32_t hi; /* little-endian high word */
    };
};

/* Use the standard <stdarg.h> macros instead of custom versions. */
