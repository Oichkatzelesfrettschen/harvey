#include "args.h"
#include "msf.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char **argv) {
    CmdArgs args = parse_args(argc, argv);
    printf("C23 acd skeleton running on device %s. Verbose=%d\n", args.device, args.verbose);

    /* TODO: implement SCSI access and event loop using pthreads. */
    Msf start = msf_from_frames(0);
    printf("Start at %02d:%02d.%02d\n", start.m, start.s, start.f);

    return 0;
}
