#pragma once

#include <stdint.h>

/*
 * Minute-second-frame structure from the original acd code. Fields use
 * explicit 32-bit types for portability across 32/64-bit systems.
 */
typedef struct {
    int32_t m;
    int32_t s;
    int32_t f;
} Msf;

enum {
    MSF_SECS_PER_MIN = 60,
    MSF_FRAMES_PER_SEC = 75,
};

/*
 * Convert a frame count to an Msf structure where one minute equals 75*60
 * frames. Useful when translating from raw CD frame positions.
 */
static inline Msf msf_from_frames(uint32_t frames) {
    Msf msf;
    msf.m = (int32_t)(frames / (MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC));
    frames %= MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC;
    msf.s = (int32_t)(frames / MSF_FRAMES_PER_SEC);
    msf.f = (int32_t)(frames % MSF_FRAMES_PER_SEC);
    return msf;
}

/*
 * Convert an Msf structure back to an absolute frame count. Useful when
 * interacting with low level CD commands which operate on frame numbers.
 */
static inline uint32_t msf_to_frames(Msf msf) {
    /* Convert minutes to frames. */
    uint32_t from_minutes = (uint32_t)(msf.m * MSF_SECS_PER_MIN * MSF_FRAMES_PER_SEC);
    /* Convert seconds to frames. */
    uint32_t from_seconds = (uint32_t)(msf.s * MSF_FRAMES_PER_SEC);
    /* Frames are already stored in the structure. */
    uint32_t from_frames = (uint32_t)msf.f;

    /* Sum all contributions for the total frame count. */
    return from_minutes + from_seconds + from_frames;
}
