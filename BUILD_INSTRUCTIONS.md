# Build Instructions for Modern Utilities

**IMPORTANT NOTICE:** This document provides specific build steps for the utilities in `modern/`. For the overarching architectural vision, CI strategy, and modernization protocols for Harvey OS, please refer to:
*   `docs/architectural-manifesto.md`
*   `docs/ci-strategy.md`
*   `docs/systematic-modernization.md`

The build system for `modern/` utilities has been refactored. While the commands below (using `make ARCH=...` from the root directory) remain valid, the `modern/Makefile` itself now uses the `TARGET_ARCH` variable, and architecture-specific settings (like compiler flags and toolchains) are defined in files within the `mk/arch/` directory (e.g., `mk/arch/x86_64.mk`, `mk/arch/arm64.mk`).

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
    *(Note: Requires an ARM64 cross-compiler like `aarch64-linux-gnu-gcc`. The specific compiler is now defined in `mk/arch/arm64.mk` and should be in your PATH.)*

To build for all architectures defined in the `ARCHS` variable in the Makefile (currently i386, x86_64, arm64), you can use the `test` target, which cleans and builds for each:
```bash
make test
```
This is primarily used by the CI system.
