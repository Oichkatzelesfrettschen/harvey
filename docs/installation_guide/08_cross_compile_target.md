# Step 8: Cross-Compile for a Target Architecture (e.g., ARM64)

After successfully building and running Harvey OS for your native architecture, you can now try cross-compiling it for a different target architecture. This means using compilers on your current machine (e.g., x86_64) to generate code that runs on another type of processor (e.g., ARM64, i386, etc.).

The `setup-harvey-dev-env.sh` script installed various cross-compilers, and the `harvey-env.sh` script (sourced in Step 4) provides a helper function `harvey_cross <arch>` to simplify switching your build environment for cross-compilation.

## 8.1. Prerequisites

*   **Harvey Environment Activated:** Ensure you have sourced `$HARVEY_ROOT/harvey-env.sh` in your current terminal session.
*   **Source Code Cloned:** You should have the Harvey OS source code cloned (Step 5).
*   **Native Build Completed (Recommended):** While not strictly necessary for cross-compilation, having done a successful native build (Step 6) ensures your source tree and basic build setup are sound.
*   **Current Directory:** Be in the root directory of the cloned Harvey OS source code (`$HARVEY_ROOT/src/harvey-os/`).

## 8.2. Understanding Cross-Compilation Setup

The `harvey_cross <arch>` helper function is designed to modify environment variables like `CC`, `CXX`, `LD`, `AR`, `NM`, etc., to point to the appropriate cross-compiler toolchain for the specified architecture `arch`. For example:
*   `harvey_cross arm64` would set `CC` to `aarch64-linux-gnu-gcc` (or a similar Clang equivalent if preferred by Harvey's setup).
*   `harvey_cross i386` might set `CC` to `gcc -m32`.

The Harvey OS build system (`mkfiles`), when these variables are set, should then use these cross-tools to produce binaries for the target architecture.

## 8.3. Performing a Cross-Compilation (Example: ARM64)

Let's assume you want to cross-compile for ARM64.

1.  **Set up the Cross-Compilation Environment:**
    In your terminal (within `$HARVEY_ROOT/src/harvey-os/`), run:
    ```bash
    harvey_cross arm64
    ```
    You should see a confirmation message like: "Cross-compilation environment set for arm64".
    You can verify by checking the `CC` variable:
    ```bash
    echo $CC
    # Expected output might be: aarch64-linux-gnu-gcc (or similar)
    ```

2.  **Clean Previous Build Artifacts (Recommended):**
    Since you are switching architectures, it's highly recommended to clean any artifacts from previous builds (especially if they were for a different architecture like your native one) to avoid conflicts or unexpected behavior.
    ```bash
    hmk clean
    # Or, if harvey_build handles cleaning appropriately for cross-setups:
    # harvey_build clean
    ```

3.  **Build for the Target Architecture:**
    Now, perform the full build using the `harvey_build all` or `hmk all` command. The build system will use the cross-compilers defined by `harvey_cross`.
    ```bash
    # Using the higher-level helper:
    harvey_build all
    ```
    Or:
    ```bash
    # Using the mk wrapper:
    hmk all
    ```
    This process will again involve compiling the kernel, libraries, and commands, but this time producing ARM64 binaries. It might take a similar amount of time as the native build.

4.  **Resetting to Native Environment (Optional):**
    If you want to switch back to building for your native architecture after cross-compiling, you can often re-source the environment script or use a specific command if `harvey_cross` provides one (e.g., `harvey_cross native` or `harvey_cross x86_64` if your native is x86_64).
    ```bash
    # Example:
    # harvey_cross x86_64
    # Or re-source:
    # source $HARVEY_ROOT/harvey-env.sh
    ```

## 8.4. Cross-Compiled Artifacts

The build artifacts for the target architecture will be placed in locations specific to that architecture within the build tree. For example, an ARM64 kernel might be at `sys/src/9/arm64/9pi` or similar. Userland binaries might be under a path like `arm64/bin/` or `$HARVEY_ROOT/sysimg/arm64/bin/`.

Consult Harvey OS's build system documentation or examine the output directories to locate the specific artifacts for your cross-compiled target.

## 8.5. Other Architectures

You can repeat the process for other supported target architectures by using the appropriate `arch` argument with `harvey_cross` (e.g., `i386`, `armhf`, etc.), provided the necessary cross-compilers were installed by `setup-harvey-dev-env.sh` and are supported by Harvey OS's build system.

## 8.6. Next Steps

With Harvey OS successfully cross-compiled for your target architecture (e.g., ARM64), you are now ready to run this version in QEMU. Proceed to **Step 9: Run Harvey OS in QEMU (Cross-Compiled Image)**.

## 8.7 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: `harvey_cross <arch>` command not found or fails
*   **Symptoms:** Terminal outputs `harvey_cross: command not found`, or `harvey_cross arm64` results in an error message like "Unknown architecture" or "Cross-compiler for arm64 not found."
*   **Root Cause Analysis:**
    *   `harvey-env.sh` not sourced (see Step 4 troubleshooting).
    *   The `harvey_cross` function in `harvey-env.sh` might have an issue or doesn't recognize the specified `arch`.
    *   The required cross-compiler toolchain for `<arch>` was not installed by `setup-harvey-dev-env.sh` (Step 2).
*   **Resolution Protocol:**
    1.  Ensure `harvey-env.sh` is sourced: `source $HARVEY_ROOT/harvey-env.sh`.
    2.  Verify the architecture name is correct and supported by `harvey_cross` (check `harvey-env.sh` for supported targets).
    3.  Check if the cross-compiler was installed (e.g., `aarch64-linux-gnu-gcc --version`). If missing, revisit Step 2 logs or try to install it manually if you know the package name. Re-running `setup-harvey-dev-env.sh` might be necessary.

#### Failure Mode 2: `hmk clean` or `hmk all` fails after `harvey_cross`
*   **Symptoms:** Build fails with errors related to incorrect compiler options, linker errors about incompatible architectures, or tools from the wrong toolchain being invoked.
*   **Root Cause Analysis:**
    *   The environment variables (`CC`, `LD`, etc.) were not correctly set by `harvey_cross <arch>`.
    *   Previous build artifacts from a different architecture were not fully cleaned, leading to conflicts.
    *   The Harvey OS `mkfiles` are not correctly interpreting the cross-compilation environment variables.
*   **Resolution Protocol:**
    1.  After running `harvey_cross <arch>`, verify key variables: `echo $CC`, `echo $LD`. They should point to the cross-tools.
    2.  Always run `hmk clean` (or equivalent) after switching architectures and before building.
    3.  If errors persist, this might indicate a deeper issue in the build system's cross-compilation support for that specific architecture. Consult Harvey OS documentation or community channels.

#### Failure Mode 3: Cross-compilation is extremely slow
*   **Symptoms:** The build process for the target architecture is significantly slower than the native build.
*   **Root Cause Analysis:**
    *   Cross-compiling can sometimes be slower due to the overhead of the tools or if the build system isn't optimized for it.
    *   If emulated tools are involved (less common for typical cross-compilers but possible in some setups), performance will be much worse.
    *   General system resource limitations (CPU, RAM) as with native builds.
*   **Resolution Protocol:**
    *   This is often expected to some degree. Ensure no other resource-intensive tasks are running.
    *   Monitor system resources. If issues are severe, you might need a more powerful host machine.

## 8.8 Architectural Alignment

Cross-compilation is a direct manifestation of Harvey OS's architectural commitments:

*   **Multi-architecture Philosophy ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)):** This step is the practical application of this core principle, enabling the creation of binaries for architectures other than the build host. It's essential for supporting diverse hardware targets and the Tiered Support Model detailed in `docs/architecture/tier-criteria.md`.
*   **Portability ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#2-core-identity-what-is-harvey-os-in-2025-and-beyond)):** Successful cross-compilation relies on the portability of the Harvey OS codebase. Issues encountered here can highlight areas where code is not sufficiently portable.
*   **Build System Modernization & Robustness (Modernization Principles - [ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#4-modernization-principles)):** The build system must be robust enough to handle different toolchains and target flags correctly. The `harvey_cross` script and underlying `mkfiles` demonstrate this aspect of the build system.
*   **Environmental Resilience ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)):** The `harvey_cross` function helps ensure that a consistent set of environment variables is used for a given cross-compilation target, contributing to reproducible cross-builds.
