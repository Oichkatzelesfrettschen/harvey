# Build Instructions for Modern Utilities

This document describes how to build the modern C utilities located in the `modern/` directory.

## Supported Architectures

The utilities can be compiled for the following target architectures:
- `x86_64` (64-bit Intel/AMD)
- `i386` (32-bit Intel/AMD)
- `arm64` (64-bit ARM, requires cross-compilation)

## Prerequisites

1.  **Basic Build Tools:**
    *   `make`
    *   A C compiler like `clang` (default) or `gcc`.

2.  **For i386 builds:**
    *   32-bit development libraries are required. On Debian/Ubuntu, this is typically provided by the `gcc-multilib` package.

3.  **For ARM64 cross-compilation:**
    *   An ARM64 cross-compiler is needed. On Debian/Ubuntu, you can install `gcc-aarch64-linux-gnu`.

4.  **Automated Dependency Installation:**
    *   The `./setup.sh` script attempts to install all necessary dependencies, including compilers and libraries for different architectures. It's recommended to run this script first:
        ```bash
        ./setup.sh
        ```
    *   Note: If `setup.sh` encounters issues with `apt-get update` (e.g., due to system clock or repository misconfiguration), manual installation of the above packages might be necessary if the script fails to install them.

## Build Commands

Navigate to the root of the repository. The main `Makefile` will delegate the build to `modern/Makefile`.

To build for a specific architecture, use the `ARCH` variable:

*   **For x86_64 (default):**
    ```bash
    make ARCH=x86_64
    ```
    Or simply:
    ```bash
    make
    ```
    This will produce `modern/acd-x86_64`.

*   **For i386:**
    ```bash
    make ARCH=i386
    ```
    This will produce `modern/acd-i386`.
    *(Note: Requires `gcc-multilib` or equivalent 32-bit development libraries.)*

*   **For arm64 (cross-compilation):**
    ```bash
    make ARCH=arm64
    ```
    This will produce `modern/acd-arm64`.
    *(Note: Requires `aarch64-linux-gnu-gcc` or an equivalent ARM64 cross-compiler to be in your PATH and recognized by the build system via the `CC_ARM64` variable in `modern/Makefile`.)*

To build for all architectures defined in the `ARCHS` variable in the Makefile (currently i386, x86_64, arm64), you can use the `test` target, which cleans and builds for each:
```bash
make test
```
This is primarily used by the CI system.

## Build Flags

The `setup.sh` script configures optimized compiler flags for local builds by
exporting the variables `CFLAGS`, `CXXFLAGS` and `LDFLAGS`.  The defaults are:

```bash
CFLAGS="-O3 -march=native -mtune=native -pipe -fPIC"
CXXFLAGS="-O3 -march=native -mtune=native -pipe -fPIC"
LDFLAGS="-fuse-ld=lld"
```

Override them when invoking `make` if alternate flags are desired.  The CI
workflows rely on these same defaults to ensure consistent builds across
architectures.
