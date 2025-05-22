#ifndef HARVEY_MSF_H
#define HARVEY_MSF_H

#include <stdint.h>

/* Minute, second, frame structure from original acd code */
typedef struct {
    int m;
    int s;
    int f;
} Msf;

static inline Msf msf_from_frames(uint32_t frames) {
    Msf msf;
    msf.m = frames / (60 * 75);
    frames %= 60 * 75;
    msf.s = frames / 75;
    msf.f = frames % 75;
    return msf;
}

#endif /* HARVEY_MSF_H */
