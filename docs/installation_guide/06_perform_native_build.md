# Step 6: Perform Initial Build (Native Architecture)

Now that you have the Harvey OS source code and your development environment is activated, you can compile the operating system for your computer's native architecture. This typically means building for x86_64 if you are on a standard 64-bit PC, or arm64 if you are on an arm64-based machine (like a Raspberry Pi 4/5 running a 64-bit OS).

The `harvey-env.sh` script, which you sourced in Step 4, provides helper functions to simplify this process.

## 6.1. Prerequisites

*   **Harvey Environment Activated:** Ensure you have sourced `$HARVEY_ROOT/harvey-env.sh` in your current terminal session. You should see your chosen default compiler (e.g., `clang-17`) when you run `echo $CC`.
*   **Current Directory:** You should be in the root directory of the cloned Harvey OS source code. If you followed Step 5, this would be `$HARVEY_ROOT/src/harvey-os/`.
    ```bash
    # If not already there:
    # cd $HARVEY_ROOT/src/harvey-os
    ```

## 6.2. Understanding the Build Process

Harvey OS typically uses `mk` (the Plan 9 make utility) for its build process. The `harvey-env.sh` script likely defined:
*   `hmk [...]`: A wrapper for `mk` that might pass necessary environment variables like `HARVEY_ROOT`.
*   `harvey_build <component|all>`: A higher-level function that uses `hmk` to build specific parts of the OS (like `kernel`, `libs`, `cmds`) or everything (`all`).

When you build for your "native" architecture, the build system uses your system's default compilers (which `harvey-env.sh` might have set to Clang or GCC) without needing specific cross-compiler flags.

## 6.3. Building Harvey OS

1.  **Perform a Full Build:**
    To build all components of Harvey OS (kernel, libraries, userland commands), use the `harvey_build all` command (if available and defined as such in `harvey-env.sh`) or the `hmk all` command:
    ```bash
    # Using the higher-level helper, if available:
    harvey_build all
    ```
    Or, if `harvey_build` is not defined or you prefer to use `hmk` directly:
    ```bash
    # Using the mk wrapper:
    hmk all
    ```
    This command will invoke the build process. You will see a lot of compiler output scrolling through your terminal as each part of the OS is compiled. This can take some time, especially for the first build.

2.  **Monitoring the Build:**
    *   Watch for any error messages. Errors are typically prefixed with `error:` or stop the build process.
    *   If the build fails, carefully read the last few lines of output to understand which component failed and why. Common issues can include missing dependencies (though `setup-harvey-dev-env.sh` aims to prevent this), compiler errors due to code issues, or incorrect environment setup.

## 6.4. Build Artifacts

Once the build completes successfully, the compiled binaries and other artifacts will be placed in specific directories within the Harvey OS source tree. The exact locations can vary, but common patterns include:
*   **Kernel:** Often in a path like `sys/src/9/pc/9pc64` (for an x86_64 PC kernel) or `sys/src/9/<arch>/9<kernelfile>`.
*   **Userland Binaries:** May be populated into a directory structure mimicking the final OS layout, perhaps under a `/<arch>/bin` or `$HARVEY_ROOT/sysimg/<arch>/bin` style path within the build tree.
*   **Disk Image:** Some build processes might also generate a bootable disk image (e.g., `disk.img`) in the root of the source tree or a build output directory.

The specific build system of Harvey OS will determine these locations. Consult Harvey OS's own build documentation if you need to find specific files.

## 6.5. Cleaning Build Artifacts (Optional)

If you need to rebuild from scratch or clean up build artifacts, there's usually a `clean` target:
```bash
# Using the higher-level helper:
# harvey_build clean
# (Behavior depends on how harvey_build clean is implemented)

# Or using the mk wrapper:
hmk clean
```
This will remove compiled object files and binaries, allowing for a fresh build.

## 6.6. Next Steps

With Harvey OS successfully compiled for your native architecture, you are ready to run it in an emulator. Proceed to **Step 7: Run Harvey OS in QEMU (Native Architecture Image)**.

## 6.7 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: Build command not found (`hmk`, `harvey_build`)
*   **Symptoms:** Terminal outputs `hmk: command not found` or `harvey_build: command not found`.
*   **Root Cause Analysis:** The `harvey-env.sh` script was not sourced in the current terminal session, or it has errors preventing function definitions.
*   **Resolution Protocol:**
    1.  Ensure you have sourced the environment script: `source $HARVEY_ROOT/harvey-env.sh`.
    2.  Verify the functions are defined: `declare -F | grep harvey_` or `declare -F | grep hmk`.
    3.  If they are missing, check `harvey-env.sh` for errors or re-run Step 2 if the script itself is missing/corrupted.

#### Failure Mode 2: Compiler errors (e.g., "unknown type name", "implicit declaration of function")
*   **Symptoms:** The build process stops with C compiler errors.
*   **Root Cause Analysis:**
    *   Missing headers or libraries (less likely if `setup-harvey-dev-env.sh` was thorough, but possible for obscure dependencies or if system headers are unusual).
    *   Source code errors (bugs in the code).
    *   Incorrect compiler flags or environment for the specific component being built.
    *   Toolchain issues (e.g., a faulty compiler version, though `setup-harvey-dev-env.sh` aims to install specific working versions).
*   **Resolution Protocol:**
    1.  Carefully read the compiler error. It usually points to the file and line number.
    2.  If it's a missing header, ensure all dependencies from Step 2 were installed.
    3.  If it seems like a source code bug, check the Harvey OS issue tracker for similar reports. This might be a genuine bug you've encountered.
    4.  Ensure your environment (especially `CC`, `CFLAGS`, `LD`, `LDFLAGS` from `harvey-env.sh`) is correctly set and not being overridden by local shell settings.
    5.  Try a `hmk clean` followed by `hmk all` to ensure a fresh build.

#### Failure Mode 3: Insufficient system resources
*   **Symptoms:** Build process is extremely slow, system becomes unresponsive, or compiler is killed (e.g., "Killed" message).
*   **Root Cause Analysis:** Not enough RAM or disk space. Compilation, especially with many parallel jobs (`mk` might do this), can be memory intensive.
*   **Resolution Protocol:**
    1.  Check available RAM (`free -h`) and disk space (`df -h`).
    2.  Close other resource-heavy applications.
    3.  If using a VM for development, consider increasing its allocated RAM.
    4.  Some `mk` systems allow limiting parallel jobs (e.g., `hmk -j1 all` to run single-threaded, though this will be slower). Check Harvey's `mk` documentation.

## 6.8 Architectural Alignment

Performing the native build is a key validation and operational step:

*   **Tier 1 Architecture Validation ([docs/architecture/tier-criteria.md](../../docs/architecture/tier-criteria.md)):** Your native architecture (e.g., x86_64, arm64) is likely a Tier 1 ("Core") architecture for Harvey OS. A successful native build is the first practical step in validating support for this tier.
*   **Build System Integrity (Modernization Principles - [ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#4-modernization-principles)):** This step directly tests the functionality and robustness of Harvey OS's build system (`mk` and associated `mkfiles`) using modern toolchains for a primary architecture.
*   **C17 Compliance ([docs/operations/c17-migration-guide.md](../../docs/operations/c17-migration-guide.md)):** The build process will enforce C17 standards if compilers and flags are set as per the C17 migration strategy, providing immediate feedback on code compliance.
*   **Foundation for Performance Baselines ([docs/architecture/performance-baselines.md](../../docs/architecture/performance-baselines.md)):** A working native build is a prerequisite for establishing meaningful performance baselines on reference hardware for that architecture.
