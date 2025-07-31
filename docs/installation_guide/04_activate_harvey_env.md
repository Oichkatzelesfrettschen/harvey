# Step 4: Activate Harvey OS Development Environment

With the development tools installed and verified, the next step is to activate the Harvey OS-specific environment settings for your current terminal session. The `setup-harvey-dev-env.sh` script should have created a helper script named `harvey-env.sh` in your `$HARVEY_ROOT` directory for this purpose.

## 4.1. Understanding `harvey-env.sh`

This script, when sourced, typically performs the following actions:

*   **Sets/Exports Key Environment Variables:**
    *   `HARVEY_ROOT`: Confirms or sets the root of your Harvey OS workspace.
    *   `PLAN9`: Confirms or sets the path to your `plan9port` installation.
    *   `PATH`: Adds Harvey-specific tool paths (like `$PLAN9/bin`, potentially others) to your system `PATH` for the current session.
    *   `CC`, `CXX`, `LD`, `AR`, `NM`, `RANLIB`: Sets default compilers and binary utilities, often pointing to Clang/LLVM tools (e.g., `clang-17`, `lld-17`) as per the setup script's design.
*   **Defines Helper Functions:**
    *   `harvey_cross <arch>`: A function to quickly set up environment variables for cross-compiling to a specified architecture (e.g., `arm64`, `i386`).
    *   `hmk [...]`: A wrapper around the `mk` (Plan 9 make) command, possibly pre-setting `HARVEY_ROOT`.
    *   `harvey_build <component|all>`: A higher-level function to build specific parts of Harvey OS (kernel, libs, cmds) or all of it.
    *   `harvey_fuzz <target>`: A helper to set up and run fuzzing sessions.
    *   `harvey_analyze <dir>`: A helper to run static analysis tools.
    *   Other custom functions defined in the script.

Sourcing this script ensures that your terminal session is correctly configured to build and work with Harvey OS.

## 4.2. Activating the Environment

1.  **Open a Terminal:** If you don't have one open, or if you want to ensure a clean start, open a new terminal.
2.  **Navigate to your Harvey Workspace (Optional but good practice):**
    ```bash
    cd $HARVEY_ROOT
    # If HARVEY_ROOT is not yet set globally, use the actual path, e.g., cd $HOME/harvey-dev
    ```
3.  **Source the Script:**
    Use the `source` (or `.`) command to execute `harvey-env.sh` in the context of your current shell:
    ```bash
    source $HARVEY_ROOT/harvey-env.sh
    # Or, if HARVEY_ROOT is not yet an active environment variable:
    # source $HOME/harvey-dev/harvey-env.sh
    ```

    Upon successful sourcing, you should see an informational message like:
    ```
    Harvey OS development environment loaded
    Commands available:
      harvey_cross <arch>  - Set up cross-compilation
      harvey_build <part>  - Build Harvey components
      harvey_fuzz <target> - Fuzz test a component
      harvey_analyze <dir> - Run static analysis
    ```
    (The exact output depends on the `harvey-env.sh` script's content.)

## 4.3. Verifying Activation

You can perform a few quick checks:

*   **Check `PATH`:**
    ```bash
    echo $PATH
    ```
    You should see paths like `$HOME/harvey-dev/tools/plan9port/bin` (or your custom `$PLAN9/bin`) included.
*   **Check Compiler Variables:**
    ```bash
    echo $CC
    echo $CXX
    ```
    These should reflect the defaults set by the script (e.g., `clang-17`).
*   **Test a Helper Function (Availability):**
    Type `harvey_cross` and press Enter. It should print usage information or an error about missing arguments, indicating the function is defined.
    ```bash
    harvey_cross
    # Expected output might be: Unknown architecture:
    ```

## 4.4. Important Notes

*   **Session Specific:** Sourcing `harvey-env.sh` only configures your *current* terminal session. If you open a new terminal, you will need to source it again.
*   **`.bashrc` / `.zshrc` Integration (Optional Advanced Step):**
    For convenience, you *could* add `source $HARVEY_ROOT/harvey-env.sh` to your shell's startup file (like `$HOME/.bashrc` or `$HOME/.zshrc`). However, be mindful that this loads the Harvey environment for *every* new terminal, which might not always be desired if you work on many different projects. A common alternative is to define a simple alias in your `.bashrc`:
    ```bash
    # Example alias in .bashrc
    # alias load_harvey_env="source $HOME/harvey-dev/harvey-env.sh"
    # Then you can just type 'load_harvey_env' in a new terminal.
    ```

## 4.5. Next Steps

With the Harvey OS development environment activated, your session is now prepared for compiling Harvey OS source code. Proceed to **Step 5: Clone Harvey OS Source Code**.

## 4.6 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: `harvey-env.sh: No such file or directory`
*   **Symptoms:** When trying to `source $HARVEY_ROOT/harvey-env.sh`, the shell reports that the file is not found.
*   **Root Cause Analysis:**
    *   The `setup-harvey-dev-env.sh` script (Step 2) did not complete successfully or failed to create this environment script.
    *   `HARVEY_ROOT` environment variable is not set, or points to the wrong directory.
    *   You are not in the correct directory if using a relative path, or the path to `harvey-env.sh` is mistyped.
*   **Resolution Protocol:**
    1.  Verify that `$HARVEY_ROOT` is set: `echo $HARVEY_ROOT`. If it's empty, ensure `setup-harvey-dev-env.sh` ran and you've potentially set it manually if you chose a custom location. The default is `$HOME/harvey-dev`.
    2.  Check if the script exists: `ls -l $HARVEY_ROOT/harvey-env.sh` (e.g., `ls -l $HOME/harvey-dev/harvey-env.sh`).
    3.  If the script is missing, re-run `setup-harvey-dev-env.sh` (Step 2) and carefully check its output for errors, especially near the end where it would create `harvey-env.sh`.
    4.  Ensure you are using the `source` command (or `.`) not trying to execute it directly: `source ./harvey-env.sh` not `./harvey-env.sh`.

#### Failure Mode 2: Helper functions (e.g., `harvey_cross`) not defined after sourcing
*   **Symptoms:** After sourcing `harvey-env.sh`, typing `harvey_cross arm64` results in "harvey_cross: command not found".
*   **Root Cause Analysis:**
    *   The `harvey-env.sh` script might have errors within it that prevent it from fully executing when sourced.
    *   The script was not sourced correctly (e.g., it was executed in a subshell).
*   **Resolution Protocol:**
    1.  Source it again, ensuring no error messages appear: `source $HARVEY_ROOT/harvey-env.sh`.
    2.  Open `harvey-env.sh` in a text editor and look for obvious syntax errors if the problem persists.
    3.  Check if the functions are defined using `declare -F | grep harvey_`.

#### Failure Mode 3: `echo $CC` shows system compiler, not Harvey's default (e.g., Clang)
*   **Symptoms:** After sourcing `harvey-env.sh`, `echo $CC` shows `gcc` instead of the expected `clang-17` (or other version set by the script).
*   **Root Cause Analysis:**
    *   The `harvey-env.sh` script might not be overriding `CC`/`CXX` correctly, or it's being overridden by another shell configuration loaded later.
    *   Your shell configuration (`.bashrc`, `.zshrc`, etc.) might be explicitly setting `CC` after the point where `harvey-env.sh` would be sourced (if you added it to your shell rc file).
*   **Resolution Protocol:**
    1.  Examine `harvey-env.sh` to see how `CC` and `CXX` are set.
    2.  Temporarily disable custom settings in your `.bashrc` or other shell startup files that might interfere, then open a new terminal and re-source `harvey-env.sh`.
    3.  Ensure that the specific Clang version (e.g., `clang-17`) was correctly installed by the setup script in Step 2.

## 4.7 Architectural Alignment

Activating the `harvey-env.sh` script is a key step in aligning your development session with Harvey OS's architectural goals:

*   **Modernization Principles & C17 Adoption ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#4-modernization-principles)):** The script sets default compilers (e.g., `clang-17`) and environment variables that ensure builds use modern C17 standards and toolchains, as specified in the `docs/operations/c17-migration-guide.md`.
*   **Multi-architecture Philosophy ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)):** The `harvey_cross` helper function directly enables the "Tiered Support Model" by allowing developers to easily switch toolchains for different architectures (Tier 1, Tier 2, etc.) as defined in `docs/architecture/tier-criteria.md`.
*   **Developer Experience & Automation:** Providing helper functions like `harvey_build` and `hmk` simplifies common development tasks, making the environment more user-friendly and reducing manual configuration errors, which supports the goals of the `docs/community/contribution-guide.md`.
*   **Environmental Resilience ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)):** By encapsulating environment settings (paths, compiler choices) in a script, `harvey-env.sh` contributes to a more reproducible build environment across different developer machines, complementing the work of the main setup script.
