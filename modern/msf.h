#ifndef HARVEY_MSF_H
#define HARVEY_MSF_H

#include <stdint.h>

/* Minute, second, frame structure from original acd code.
 * Fields use explicit 32-bit types for portability. */
typedef struct {
    int32_t m;
    int32_t s;
    int32_t f;
} Msf;


enum {
    MSF_SECS_PER_MIN = 60,
    MSF_FRAMES_PER_SEC = 75,
};

static inline Msf msf_from_frames(uint32_t frames) {
    Msf msf;
    msf.m = (int32_t)(frames / (MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC));
    frames %= MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC;
    msf.s = (int32_t)(frames / MSF_FRAMES_PER_SEC);
    msf.f = (int32_t)(frames % MSF_FRAMES_PER_SEC);
    return msf;
}

#endif /* HARVEY_MSF_H */
