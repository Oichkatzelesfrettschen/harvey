# Deprecation Notice: `386/` Directory

This directory contains legacy components related to a Plan 9 environment targeting the 386 (i386) architecture.

**Contents:**
*   `include/`: Plan 9 style C header files. Some have been updated to be compatible with modern C standards, while others (e.g., `ureg.h`, `ape/ureg.h`) are inherently x86-specific. These headers are not actively used by the utilities built from the `modern/` directory.
*   `mkfile`: A legacy Plan 9 makefile stub, not used in the primary build process of this repository.
*   `bbin/`: Contains scripts for building Go-based utilities (e.g., from u-root) for Plan 9 on the 386 architecture.
*   `bin/`: This subdirectory is presumed to contain pre-compiled binaries or utility scripts for a 386 Plan 9 environment. The C source code for these binaries is not present in this repository.

**Status:**
The components within this `386/` directory are considered legacy and are **not part of the primary build process** for the utilities found in the `modern/` directory. They are retained for historical reference or for developers working with a specific 386 Plan 9 environment.

The main development and build efforts in this repository focus on the C utilities in the `modern/` directory, targeting more current architectures like i386 (as a modern target), x86_64, and ARM64 using modern compilers.
