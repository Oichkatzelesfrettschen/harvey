#pragma once

#include <stdbool.h>
#include <stdint.h>

/*
 * Parsed command line arguments. 'verbose' is set when -v is present and
 * 'device' holds the target CD device name.
 */
typedef struct {
    bool verbose;
    const char *device;
} CmdArgs;

/*
 * Parse command line arguments. Uses explicit 32-bit types for portability.
 */
CmdArgs parse_args(int32_t argc, char **argv);
