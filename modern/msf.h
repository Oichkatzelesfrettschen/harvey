#ifndef HARVEY_MSF_H
#define HARVEY_MSF_H

#include <stdint.h>

/* Minute, second, frame structure from original acd code */
typedef struct {
    int m;
    int s;
    int f;
} Msf;

_Static_assert(sizeof(int) == 4, "int is not 32-bit");

enum {
    MSF_SECS_PER_MIN = 60,
    MSF_FRAMES_PER_SEC = 75,
};

static inline Msf msf_from_frames(uint32_t frames) {
    Msf msf;
    msf.m = (int)(frames / (MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC));
    frames %= MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC;
    msf.s = (int)(frames / MSF_FRAMES_PER_SEC);
    msf.f = (int)(frames % MSF_FRAMES_PER_SEC);
    return msf;
}

#endif /* HARVEY_MSF_H */
