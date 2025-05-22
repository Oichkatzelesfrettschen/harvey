#ifndef HARVEY_ARGS_H
#define HARVEY_ARGS_H

#include <stdbool.h>

typedef struct {
    bool verbose;
    const char *device;
} CmdArgs;

CmdArgs parse_args(int argc, char **argv);

#endif /* HARVEY_ARGS_H */
