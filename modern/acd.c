/* Example CD player skeleton using modern threading primitives. */
#include "args.h" /* command line parser */
#include "msf.h"
#include "spinlock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Entry point demonstrating argument parsing and spinlock usage. The program
 * simply prints the parsed arguments and exits.
 */
int main(int argc, char **argv) {
    CmdArgs args = parse_args((int32_t)argc, argv);
    printf("C23 acd skeleton running on device %s. Verbose=%d\n", args.device, (int)args.verbose);

    /* Example use of the recursive spinlock. */
    Spinlock lock;
    spinlock_init(&lock);
    spinlock_acquire(&lock);
    /* Re-acquire recursively. */
    spinlock_acquire(&lock);

    /* TODO: implement SCSI access and event loop using pthreads. */
    Msf start = msf_from_frames(0);
    printf("Start at %02d:%02d.%02d\n", start.m, start.s, start.f);

    spinlock_release(&lock);
    spinlock_release(&lock);

    return 0;
}
