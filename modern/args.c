#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(void) {
    fputs("usage: acd [-v] <device>\n", stderr);
    exit(EXIT_FAILURE);
}

CmdArgs parse_args(int argc, char **argv) {
    CmdArgs args = {.verbose = false, .device = NULL};
    for (int i = 1; i < argc; ++i) {
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
