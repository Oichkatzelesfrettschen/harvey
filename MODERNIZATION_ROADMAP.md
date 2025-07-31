# Modernization Roadmap

This document outlines planned and potential modernization tasks.

## `modern/` Directory

### `modern/plan9_compat.h`
1.  **Address `bugprone-easily-swappable-parameters`**:
    *   **Task**: Review function signatures (e.g., `send`, `recv`, `mkalt`). Consider if wrapper structs or more distinct types can be used for parameters to improve type safety, without sacrificing the genericity required by the channel implementation.
    *   **Priority**: Medium.

2.  **Address `readability-identifier-length`**:
    *   **Task**: Discuss with maintainers whether to lengthen short variable names (e.g., `ch` to `channel`, `v` to `value`) for improved readability. This is a style consideration; Plan 9 style favors brevity.
    *   **Priority**: Low.

3.  **Address `readability-braces-around-statements`**:
    *   **Task**: Add curly braces to all single-statement blocks within `if`, `else`, `while`, `for` statements.
    *   **Priority**: Low (Easy Fix).

4.  **Address `clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling` (for `memcpy`)**:
    *   **Task**: Double-check all `memcpy` calls for correctness regarding buffer sizes and potential overflows. While current uses seem to use `ch->size`, confirm no edge cases. Consider adding explicit assertions or runtime checks if feasible, or document why current usage is safe.
    *   **Priority**: Medium.

5.  **Resolve `<thread.h>` dependency definitively for compilation:**
    *   **Task**: The current build works because `-DUSE_POSIX_THREADS` is implicitly or explicitly active (e.g. via `-pthread` in Makefiles). Ensure this is robust. If there are scenarios where `USE_POSIX_THREADS` might *not* be defined during a standard build, the `<thread.h>` dependency needs a proper solution (e.g., providing a minimal portable `<thread.h>` if Plan 9 threads are ever intended to be a compile target outside of Plan 9, or removing that `#else` branch if only POSIX threads are supported going forward).
    *   **Priority**: High (for build robustness).

### General `modern/` Code
*   **Task**: Ensure all C code (`acd.c`, `args.c`, `spinlock.c`) fully adheres to C17. (Clang-tidy reported no issues here, but a manual C17 compliance check is good practice).
*   **Priority**: Medium.

## `acme/bin/source/acd/` (CD Player Utility)

**Overall Strategy**: This component is heavily Plan 9 dependent. Modernization requires a significant decision.

1.  **Option A: Maintain as Plan 9 Specific (Default/Current)**
    *   **Task**: No code changes. Build only within a Plan 9 / Harvey OS environment using its native tools. Document this limitation.
    *   **Pros**: No effort.
    *   **Cons**: Utility not available outside Plan 9.
    *   **Priority**: N/A (status quo).

2.  **Option B: Full Modernization to C17 & POSIX**
    *   **Task**: Major refactoring effort:
        *   Replace all Plan 9 system calls with POSIX equivalents.
        *   Replace Plan 9 library calls (e.g., for graphics, file I/O) with POSIX/standard C or cross-platform library alternatives.
        *   Rewrite build system using Makefiles compatible with clang/gcc.
        *   Address all portability and C17 compliance issues.
    *   **Pros**: Makes the utility portable.
    *   **Cons**: Very high effort, essentially a rewrite of large parts.
    *   **Priority**: Very Low (unless deemed critical).

3.  **Detailed Dependency Analysis (If Modernization is Considered):**
    *   **Task**: Perform a manual code review to list all Plan 9 headers (e.g., `<libg.h>`, `<bio.h>`) and library functions used. Estimate effort for finding POSIX/C17 replacements.
    *   **Priority**: Low (contingent on pursuing Option B).

## `386/include/` Headers

*   **Task**: Keep as historical reference. Confirm they are not included by any code in `modern/` or any other code we intend to build with standard C17 toolchains. If any `modern/` code *did* use them, that dependency would need to be removed or refactored.
*   **Priority**: Low (Monitoring).
