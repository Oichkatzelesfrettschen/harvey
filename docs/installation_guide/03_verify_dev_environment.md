# Step 3: Verify Development Environment Installation

After the `setup-harvey-dev-env.sh` script has completed, it's crucial to verify that all components were installed correctly and your development environment is ready.

## 3.1. Check the Installation Report

The setup script is designed to generate a summary report of its actions and a validation status.

1.  **Locate the Report:**
    The report is typically found in your Harvey OS workspace root, which defaults to `$HOME/harvey-dev`. The full path would be:
    ```
    $HARVEY_ROOT/installation-report.txt
    ```
    (If you set a custom `HARVEY_ROOT` before running the script, use that path.)

2.  **Review the Report:**
    Open the report with a text editor (e.g., `cat`, `less`, `nano`, `vim`, `gedit`):
    ```bash
    cat $HARVEY_ROOT/installation-report.txt
    # or
    less $HARVEY_ROOT/installation-report.txt
    ```
    Look for:
    *   **Overall Validation Status:** The script should indicate a "PASSED" or "FAILED" status near the end of the report.
    *   **Tool Validation Section:** This section (as designed in the `setup-harvey-dev-env.sh` script) lists key tools and whether they were found (`✓`) or not found (`✗`). Ensure all critical tools show a checkmark.
    *   **Compiler Versions:** Verify that expected versions of GCC and Clang are reported.
    *   **Environment Variables:** Note the reported values for `HARVEY_ROOT` and `PLAN9` (path to plan9port).

If the report indicates "FAILED" or shows `✗` marks next to essential tools like `gcc`, `clang`, `rc`, or `mk`, you may need to revisit the output of the setup script (Step 2) to identify specific error messages and troubleshoot them. Common issues could be missing system dependencies that the script couldn't auto-install, network problems during downloads, or insufficient permissions.

## 3.2. Manual Verification of Key Tools

Even if the report says "PASSED", it's good practice to manually check a few key commands in your terminal.

1.  **Open a New Terminal Session:** This is important to ensure that any environment changes (like `PATH` modifications added to `.bashrc` by `plan9port`) are loaded. Alternatively, you can re-source your `.bashrc`:
    ```bash
    source $HOME/.bashrc
    ```

2.  **Check C Compilers:**
    ```bash
    gcc --version
    clang --version
    # For Clang 17 specifically, if that was the target:
    # clang-17 --version
    ```
    These should output version information for GCC and Clang.

3.  **Check Plan 9 Utilities (from 9base and plan9port):**
    ```bash
    rc -version   # Should output 'rc version...'
    mk -version   # Should output 'mk version...'
    9c -V         # (If 9c is the Plan 9 C compiler from plan9port) - or similar version flag
    sam -v        # (If sam text editor is installed)
    acme -v       # (If acme text editor is installed)
    ```
    The exact version flags might vary slightly, but the goal is to see if these commands are found in your `PATH` and execute.

4.  **Check Cross-Compilers (Example for ARM64):**
    ```bash
    aarch64-linux-gnu-gcc --version
    ```
    This should output version information for the AArch64 GCC cross-compiler.

5.  **Check QEMU:**
    ```bash
    qemu-system-x86_64 --version
    qemu-system-aarch64 --version
    ```
    These should output QEMU version information.

## 3.3. Troubleshooting and Recovery Procedures

This section expands on common issues encountered during verification.

*   **`installation-report.txt` Shows Failures or Missing Tools (`✗`):**
    *   **Symptoms:** The report explicitly states "FAILED" or critical tools are marked with `✗`.
    *   **Root Cause Analysis:** An error occurred during Step 2 (`setup-harvey-dev-env.sh`). This could be due to network issues, package conflicts, or script errors.
    *   **Resolution Protocol:** Carefully review the full output log of `setup-harvey-dev-env.sh` (if you saved it) or its terminal output for specific error messages related to the failed tools. Address the root cause (e.g., fix network, resolve package conflicts) and re-run `setup-harvey-dev-env.sh`. If the script is idempotent, it should pick up where it left off or re-attempt failed steps.

*   **Manual `command --version` Fails ("Command not found"):**
    *   **Symptoms:** Typing a command like `gcc --version` or `rc -version` results in a "command not found" error, even if the installation report seemed okay.
    *   **Root Cause Analysis:**
        *   The `PATH` environment variable is not correctly set up for your current terminal session. This is common if you haven't opened a new terminal or re-sourced `~/.bashrc` after `plan9port` installation (which modifies `.bashrc`).
        *   The tool was genuinely not installed correctly, despite the report (less likely if the report validated it).
    *   **Resolution Protocol:**
        1.  Open a brand new terminal session to ensure all `.bashrc` changes are loaded.
        2.  Alternatively, manually re-source: `source ~/.bashrc`.
        3.  If the command is still not found, it points to an installation failure in Step 2. Check the `setup-harvey-dev-env.sh` logs.

*   **Incorrect Tool Versions Displayed:**
    *   **Symptoms:** `gcc --version` shows a much older version than expected, or `clang --version` points to a system Clang instead of the one installed by the script (e.g., `clang-17`).
    *   **Root Cause Analysis:**
        *   Your system's `PATH` prioritizes system-installed versions over those intended by the setup script (e.g., if `/usr/bin` comes before `$HARVEY_ROOT/tools/bin` or where the script places its preferred tools).
        *   The `harvey-env.sh` script (which you'll activate in Step 4) is designed to correctly prioritize paths and set specific compiler variables like `CC`. These manual checks are preliminary; the environment script is key.
    *   **Resolution Protocol:**
        1.  For now, note the discrepancy.
        2.  Proceed to Step 4 to activate `harvey-env.sh`. After sourcing it, re-check `echo $CC` and `$CC --version`. This should point to the Harvey OS preferred compiler. If it's still incorrect after sourcing `harvey-env.sh`, there might be an issue with how `harvey-env.sh` sets variables or a conflict in your shell configuration.

## 3.4. Architectural Alignment

Verifying the development environment is a direct confirmation of several architectural goals:

*   **Environmental Resilience ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)):** This step ensures that the tools and dependencies installed by `setup-harvey-dev-env.sh` are present and functional, validating the script's success in creating a consistent environment.
*   **Toolchain Consistency (relevant to [Performance Baselines](../../docs/architecture/performance-baselines.md) and [C17 Migration Guide](../../docs/operations/c17-migration-guide.md)):** Confirming the versions of key compilers (GCC, Clang) is important because performance can be sensitive to compiler versions, and C17 features/behavior must be consistent with the toolchain used for development and testing.
*   **Multi-architecture Preparedness ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)):** Checking for cross-compilers (e.g., `aarch64-linux-gnu-gcc`) confirms that the environment is ready for building Harvey OS for different hardware targets.

## 3.5. Next Steps

Once you are satisfied that the development environment is correctly installed and key tools are accessible, you can proceed to **Step 4: Activate Harvey OS Development Environment**. This step will configure your current terminal session with Harvey-specific settings and helper functions.
