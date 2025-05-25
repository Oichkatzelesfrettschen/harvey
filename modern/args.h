#ifndef HARVEY_ARGS_H
#define HARVEY_ARGS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool verbose;
    const char *device;
} CmdArgs;

/* Use explicit 32-bit type rather than assuming sizeof(int) == 4. */


CmdArgs parse_args(int32_t argc, char **argv);

#endif /* HARVEY_ARGS_H */
