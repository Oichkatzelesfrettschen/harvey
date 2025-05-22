#ifndef HARVEY_ARGS_H
#define HARVEY_ARGS_H

#include <stdbool.h>

typedef struct {
    bool verbose;
    const char *device;
} CmdArgs;

_Static_assert(sizeof(int) == 4, "int is not 32-bit");

CmdArgs parse_args(int argc, char **argv);

#endif /* HARVEY_ARGS_H */
