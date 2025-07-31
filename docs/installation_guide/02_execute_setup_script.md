# Step 2: Execute Development Environment Setup Script

With your host system prepared and the `setup-harvey-dev-env.sh` script downloaded and made executable, you are now ready to set up the comprehensive Harvey OS development environment.

## 2.1. Before You Run the Script

*   **Location:** Ensure you are in the directory where you downloaded `setup-harvey-dev-env.sh`.
*   **Internet Connection:** The script will download numerous system packages and potentially source code (like plan9port). A stable internet connection is required.
*   **Sudo Privileges:** The script will use `sudo` to install system-wide packages. It will prompt you for your password when `sudo` commands are executed.
*   **Time:** The script can take a significant amount of time to complete (anywhere from 15 minutes to over an hour, depending on your internet speed and system performance), as it installs many tools and compilers. Please be patient.
*   **Workspace Directory (`HARVEY_ROOT`):**
    *   By default, the script will create and use `$HOME/harvey-dev` as the root directory for all Harvey OS related tools, source code, and build artifacts.
    *   If you wish to use a different location, you can set the `HARVEY_ROOT` environment variable *before* running the script:
        ```bash
        export HARVEY_ROOT="/path/to/your/chosen/workspace"
        # Example: export HARVEY_ROOT="$HOME/my_harvey_os_project"
        # Then run the script:
        # ./setup-harvey-dev-env.sh
        ```
        Ensure this chosen directory is one where you have write permissions.

## 2.2. Running the Setup Script

Execute the script from your terminal:

```bash
./setup-harvey-dev-env.sh
```
For more granular control, see the 'Advanced Usage of `setup-harvey-dev-env.sh` (Intended Features)' section below for features like phased execution and explaining script actions.

The script will:
1.  Display informational messages, often color-coded (INFO, WARN, ERROR).
2.  Prompt for your `sudo` password to install packages.
3.  Update your system's package lists.
4.  Install essential build tools, modern compilers (GCC, Clang), cross-compilers for various architectures (ARM64, ARMhf, i386), static analysis tools, fuzzing tools, and other development utilities.
5.  Install Plan 9 core utilities (e.g., 9base, drawterm).
6.  Clone and build `plan9port` from source.
7.  Optionally build supplementary Plan 9 tools like `9pfuse`.
8.  Install Harvey OS specific dependencies like QEMU.
9.  Create a Harvey OS environment configuration script (`$HARVEY_ROOT/harvey-env.sh`).
10. Perform Docker-specific optimizations if it detects it's running inside a Docker container.
11. Run a validation phase to check if key tools were installed correctly.
12. Generate an installation report (`$HARVEY_ROOT/installation-report.txt`).

**Phased Execution (Optional):**
The `setup-harvey-dev-env.sh` script (as designed in our internal discussions) may support phase flags (e.g., `--phase-1`, `--phase-2`, etc.) allowing you to run only specific parts of the installation. This is useful for debugging or if you have some components already installed. Refer to the script's help output (if available via `./setup-harvey-dev-env.sh --help`) or its internal comments for details on phased execution if you need this advanced feature. For a first-time setup, running the script without phase flags is recommended. Details on intended advanced features are in section 2.3.

### 2.3 Advanced Usage of `setup-harvey-dev-env.sh` (Intended Features)

While running `./setup-harvey-dev-env.sh` directly is recommended for a full first-time setup, the script is designed with several advanced features to offer more control and insight:

1.  **Explaining Script Actions (`--explain` flag):**
    *   **Purpose:** To understand what the script does in each of its major phases without actually executing the installations. This is useful for learning about the setup process or for debugging.
    *   **Intended Usage:**
        ```bash
        ./setup-harvey-dev-env.sh --explain
        ```
    *   **Expected Output:** The script would print descriptions of each phase (e.g., "Phase 1: System Foundation - Updates package lists and installs core system dependencies like build-essential...", "Phase 2: Modern Development Toolchain - Installs GCC, Clang, cross-compilers...", etc.) rather than running the commands.

2.  **Phased (Incremental) Execution (`--phase-X` flags):**
    *   **Purpose:** To allow users to run specific parts of the setup. This is helpful if a phase fails and needs to be rerun, if some tools are already manually installed, or if only a portion of the environment is needed.
    *   **Intended Usage (Examples):**
        ```bash
        # Run only Phase 1 (System Foundation)
        ./setup-harvey-dev-env.sh --phase-1

        # Run Phase 2 and Phase 4
        ./setup-harvey-dev-env.sh --phase-2 --phase-4

        # Run all phases up to Phase 3
        ./setup-harvey-dev-env.sh --upto-phase-3
        # (Alternative: ./setup-harvey-dev-env.sh --phase-1 --phase-2 --phase-3)
        ```
    *   **Note:** The exact phase definitions (what each phase includes) would be detailed by the `--explain` flag or internal script comments. The script should handle dependencies between phases correctly (e.g., Phase 2 might require Phase 1 to have run).

3.  **Environment Status File (`harvey-status.txt` generation):**
    *   **Purpose:** After execution (or potentially as a separate flag like `--status`), the script could generate a `harvey-status.txt` file in the `$HARVEY_ROOT` directory.
    *   **Intended Content of `harvey-status.txt`:**
        *   Detected Host OS and version.
        *   Key tool versions found (e.g., GCC, Clang, Python, Go, `rc`, `mk`).
        *   `HARVEY_ROOT` and `PLAN9` paths.
        *   Installed cross-compilers.
        *   (Conceptual) Assessment of which Harvey OS architecture tiers (from `docs/architecture/tier-criteria.md`) the current environment is capable of building, based on installed tools.
        *   Timestamp of the last setup run.
    *   **Benefit:** This file provides a quick snapshot of the configured environment for debugging, sharing, or local reference.

These advanced features aim to make `setup-harvey-dev-env.sh` a more flexible and informative tool for managing the Harvey OS development environment. Please refer to the script's internal help (`./setup-harvey-dev-env.sh --help`, if implemented) or its source code for the most up-to-date information on these capabilities as they are fully realized.

## 2.4. Monitoring the Script's Progress

*   Pay attention to the output in your terminal. The script should provide `[INFO]` messages about what it's currently doing.
*   If you see `[WARN]` messages, note them down, but they may not always indicate a critical failure.
*   If you see `[ERROR]` messages, the script might stop, or a particular component might have failed to install. Read the error message carefully. You might need to resolve an underlying system issue (e.g., broken package dependencies, lack of internet) and then try re-running the script.

## 2.5. After the Script Completes

*   The script should end with a message like "Installation complete!" and provide final instructions.
*   It will also indicate the location of the installation report (`$HARVEY_ROOT/installation-report.txt`).
*   Upon completion, the script also generates an `installation-report.txt` and may create a `harvey-status.txt` detailing the configured environment (see 'Advanced Usage of `setup-harvey-dev-env.sh` (Intended Features)').

## 2.6. Next Steps

Once the script has finished successfully, proceed to **Step 3: Verify Development Environment Installation**, where you will examine the installation report and perform some manual checks.

## 2.7 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: Script fails due to `sudo` password issues
*   **Symptoms:** Script exits early asking for a password repeatedly, or errors related to `sudo` permissions.
*   **Root Cause Analysis:** Incorrect password entered, or `sudo` access is misconfigured for the user.
*   **Resolution Protocol:**
    *   Ensure you are typing your user password correctly (characters may not be displayed).
    *   Verify your user has `sudo` privileges (e.g., try `sudo ls /root`). If not, consult your system administrator or Linux distribution's documentation on granting sudo access.
    *   Re-run the script: `./setup-harvey-dev-env.sh`.

#### Failure Mode 2: Package installation failures (e.g., `apt-get install ... failed`)
*   **Symptoms:** Error messages during package installation, often mentioning "Could not resolve host," "404 Not Found" for package repositories, or unmet dependencies.
*   **Root Cause Analysis:**
    *   Internet connectivity issues.
    *   Misconfigured package repositories (e.g., in `/etc/apt/sources.list`).
    *   Temporary unavailability of a package mirror.
    *   Existing broken packages on the system.
*   **Resolution Protocol:**
    1.  Check internet connection: `ping google.com`.
    2.  Try updating package lists manually: `sudo apt-get update`. If this fails, address repository or network issues.
    3.  If specific packages fail, try installing one manually to see more detailed errors: `sudo apt-get install <package_name>`.
    4.  Sometimes, running `sudo apt-get --fix-broken install` can resolve dependency issues.
    5.  Once system issues are resolved, re-run `./setup-harvey-dev-env.sh`. The script is designed to be somewhat idempotent, skipping already completed steps.

#### Failure Mode 3: Script execution interrupted or takes excessively long
*   **Symptoms:** Script stops without a clear error, or seems to hang on a particular step for hours.
*   **Root Cause Analysis:**
    *   Accidental `Ctrl-C` by the user.
    *   Very slow internet connection, especially during large downloads like `plan9port` cloning or toolchain installations.
    *   Insufficient system resources (CPU, RAM) causing extreme slowdowns.
*   **Resolution Protocol:**
    *   If accidentally stopped, you can generally re-run `./setup-harvey-dev-env.sh`.
    *   For slow downloads, ensure your network is stable. Consider running the script during off-peak hours if bandwidth is an issue.
    *   Monitor system resources (`top`, `htop`) during script execution. If your system is heavily swapping or CPU is at 100% for prolonged periods on non-compilation tasks, you might need a more powerful host or to free up resources.
    *   If the script supports phased execution (check with `./setup-harvey-dev-env.sh --help`), you could try running phases individually after an interruption.

## 2.8 Architectural Alignment

Executing the `setup-harvey-dev-env.sh` script is a critical step that embodies several architectural goals:

*   **Environmental Resilience & Reproducible Builds ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)):** The script is the primary mechanism for establishing a consistent and reproducible development environment by automating the installation of specific toolchains and dependencies. This directly supports the goal of minimizing environment-related build issues.
*   **Multi-architecture Philosophy ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)):** The script's installation of various cross-compilers (e.g., for ARM64, i386) lays the practical groundwork for Harvey OS's multi-architecture build capabilities.
*   **Modernization Principles ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#4-modernization-principles)):** By installing modern compilers (Clang, GCC) and tools, the script equips developers for tasks like C17 migration and integration with contemporary development practices (CI/CD, static analysis) mentioned in the manifesto and detailed in documents like `docs/operations/c17-migration-guide.md`.
*   **Toolchain Management (as mentioned in `docs/ci-strategy.md`):** While the setup script primarily uses public repositories, it establishes the set of toolchains that a future offline-capable mirror (mentioned as a strategic goal) would need to host.
