# Modernization Plan

This document outlines the high level tasks required to refactor the Harvey utilities to modern C17, C++17 and portable assembly.

## C17 Migration

- Audit existing C sources for deprecated constructs and Plan 9 specific extensions.
- Use `scripts/analyze_repo.py` to track remaining Plan9 constructs in the `acd` sources.
- Introduce a portable build system using clang with `-std=c17`.
- Enable compiler warnings for portability issues and adopt clang-tidy modernize checks.
- See `c17-roadmap.md` for a detailed staged approach.

## C++17 Components

- Identify subsystems that would benefit from C++ abstractions, e.g. file handling or command line parsing.
- Establish a minimal runtime environment compatible with both C and C++ utilities.
- Compile C++ sources using `-std=c++17` and integrate with the existing makefiles.

## Portable Assembly

- Replace Plan 9 specific assembly files with standard GNU syntax or intrinsics.
- Provide both 32‑bit and 64‑bit implementations when required.
- Document calling conventions and ensure the assembly integrates with modern toolchains.

## Continuous Integration

- Use `pre-commit` to run clang-format, clang-tidy and build checks.
- Update the build scripts in `modern/` to handle both C and C++ sources.

## Final thread and I/O approach

The modern code base no longer relies on the Plan 9 threading model.  All
concurrency uses POSIX `pthread` calls and the temporary
`modern/plan9_compat.h` wrapper has been removed.  File operations and
buffer management have been rewritten around standard C and POSIX
functions.  The optional APEX layer may still be used for additional
POSIX tooling but is not required for building `acd`.

### Building

Compile the utilities with a normal host compiler:

```bash
make -C modern
```

Only clang or gcc is needed; no Plan 9 libraries or cross toolchains are
required.
