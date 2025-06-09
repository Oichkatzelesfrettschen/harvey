/* Simple argument parser for the acd utility. */
#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Print usage information and terminate.
 */
static void usage(void) {
    fputs("usage: acd [-v] <device>\n", stderr);
    exit(EXIT_FAILURE);
}

/**
 * @brief Parse command line arguments.
 *
 * The @c -v flag enables verbose output and the device name is taken from the
 * first positional argument.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Parsed arguments structure.
 */
CmdArgs parse_args(int32_t argc, char **argv) {
    CmdArgs args = {.verbose = false, .device = NULL};
    for (int32_t i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0) {
            args.verbose = true;
        } else if (!args.device) {
            args.device = argv[i];
        } else {
            usage();
        }
    }
    if (!args.device) {
        usage();
    }
    return args;
}
