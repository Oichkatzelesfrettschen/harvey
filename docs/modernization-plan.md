# Modernization Plan

This document outlines the high level tasks required to refactor the Harvey utilities to modern C17, C++17 and portable assembly.

## C17 Migration

- Audit existing C sources for deprecated constructs and Plan 9 specific extensions.
- Use `scripts/analyze_repo.py` to track remaining Plan9 constructs in the `acd` sources.
- Introduce a portable build system using clang with `-std=c17`.
- Enable compiler warnings for portability issues and adopt clang-tidy modernize checks.

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

