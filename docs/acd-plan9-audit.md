# Porting `acd` away from Plan 9 Threads

The original sources under `acme/bin/source/acd` rely heavily on Plan 9
libraries. The header `acd.h` includes a number of Plan 9 specific
interfaces:

```
#include <9p.h>
#include <auth.h>
#include <bio.h>
#include <disk.h>
#include <fcall.h>
#include <libc.h>
#include <thread.h>
#include <u.h>
```

The implementation uses the Plan 9 thread API with entry points like
`threadmain`, and functions such as `threadexitsall`, `threadexits` and
`threadsetname`.  Communication between goroutines is performed using the
`Channel` type, along with helpers like `chancreate`, `send`, `recv` and
`alt`.

To build these sources on a standard POSIX system a small compatibility
layer can map the Plan 9 APIs to `pthread` primitives.  The header
`modern/plan9_compat.h` provides minimal stubs for channels and thread
management when compiled with `-DUSE_POSIX_THREADS`.  It defines
`threadmain` as `main`, implements `proccreate` using `pthread_create`
and emulates `Channel` operations with mutexes and condition variables.

This layer is only intended for compilation and further modernization
work; it does not perfectly reproduce Plan 9 semantics.  A full port
would replace the channel based concurrency with standard POSIX queues or
other C23 facilities.

## Final thread and I/O model

After completing the modernization, the temporary Plan 9 compatibility
wrapper was removed.  All concurrency now uses POSIX `pthread` calls
directly and `modern/plan9_compat.h` no longer exists.  I/O routines were
rewritten to rely solely on standard C and POSIX interfaces such as
`open`, `read`, `write`, `fopen` and `fread`.  No Plan 9 headers remain in
the build.

Build the updated sources with any modern C toolchain:

```bash
cd modern
make
```

The build system requires only clang or gcc and does not depend on any
Plan 9 libraries or shims.

