# Build Instructions for Modern Utilities

> **IMPORTANT:** This guide covers the **modern/** directory utilities only.  
> For the overall architectural vision, CI strategy, and modernization protocol of Harvey OS, see:
> - `docs/architectural-manifesto.md`  
> - `docs/ci-strategy.md`  
> - `docs/systematic-modernization.md`  

---

## Supported Architectures

The `modern/` utilities may be built for:

- **x86_64** (64-bit Intel/AMD)  
- **i386**   (32-bit Intel/AMD)  
- **arm64**  (64-bit ARM; requires cross-compilation)

---

## Prerequisites

1. **Core Toolchain**  
   - `make`  
   - C compiler (`clang` or `gcc`)

2. **i386 Builds**  
   - 32-bit development libraries (Debian/Ubuntu: `gcc-multilib`)

3. **ARM64 Cross-Compilation**  
   - ARM-64 cross-compiler (Debian/Ubuntu: `gcc-aarch64-linux-gnu`)

4. **Dependencies Installer**  
   - A helper script `./setup.sh` will attempt to install all needed packages.  
     ```bash
     ./setup.sh
     ```  
   - If `setup.sh` fails (e.g. `apt-get update` issues), you may need to install the above packages manually.

---

## Build Commands

From the **repository root**, invoke the top-level `Makefile` which delegates into `modern/`:

### 1. Build for a Single Architecture

- **x86_64** (default):
  ```bash
  make ARCH=x86_64
  # or simply:
  make

Produces modern/acd-x86_64.
	•	i386:

make ARCH=i386

Produces modern/acd-i386.
Requires 32-bit libs (e.g. gcc-multilib).

	•	arm64:

make ARCH=arm64

Produces modern/acd-arm64.
Requires an aarch64 cross-compiler (e.g. aarch64-linux-gnu-gcc).

2. Build & Test All

The test target will clean and rebuild modern/ for every architecture listed in its ARCHS variable (by default: i386 x86_64 arm64):

make test

This is intended for CI use.

⸻

Customizing Compiler Flags

By default, setup.sh exports optimized flags for local builds:

export CFLAGS="-O3 -march=native -mtune=native -pipe -fPIC"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="-fuse-ld=lld"

To override:

make ARCH=x86_64 CFLAGS="-O2 -g" CXXFLAGS="-O2 -g"

CI workflows rely on the same defaults to guarantee repeatable, cross-architecture builds.

⸻

Cleaning Up

To remove all built artifacts for the modern utilities:

make ARCH=x86_64 clean    # cleans only x86_64 targets
# or to clean all:
make test-clean

(Check modern/Makefile for exact clean targets.)

⸻

Further Reading
	•	CI & Architecture: docs/ci-strategy.md
	•	Modernization Plan: docs/systematic-modernization.md
	•	Architectural Manifesto: docs/architectural-manifesto.md
	•	Top-Level Build: see build.sh for multi-profile orchestrations

⸻

Happy hacking!

