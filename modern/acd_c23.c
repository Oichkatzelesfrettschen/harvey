#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Minute, second, frame structure used in the original acd code. */
typedef struct {
    int m;
    int s;
    int f;
} Msf;

static void usage(void) {
    fputs("usage: acd <device>\n", stderr);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    bool verbose = false;
    const char *device = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (!device) {
            device = argv[i];
        } else {
            usage();
        }
    }

    if (!device)
        usage();

    printf("C23 acd skeleton running on device %s. Verbose=%d\n", device, verbose);
    /* TODO: implement SCSI access and event loop using pthreads. */

    return 0;
}
