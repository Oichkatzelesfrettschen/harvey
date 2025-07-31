# Plan-H: Day 2 Development Workflow Guide

Welcome to ongoing development with Plan-H! You've successfully navigated the installation and have a working environment. This guide outlines common workflows, practices, and resources for your daily development activities, whether you're fixing bugs, adding features, or exploring the system.

For initial setup, refer to the **[Comprehensive Installation Guide](../installation_guide/README.md)**.
For contribution rules and code style, see the **[Plan-H Contribution Guide](../community/contribution-guide.md)**.

## 1. Daily Development Cycle

A typical development session might involve these steps:

1.  **Activate Plan-H Environment:**
    *   Open a new terminal.
    *   Source the environment script:
        ```bash
        source $PLANH_ROOT/plan-h-env.sh
        ```
    *   This ensures `mk`, `rc`, compilers (`$CC`, `$CXX`), and helper functions are correctly configured.

2.  **Synchronize Source Code:**
    *   Navigate to your Plan-H source directory: `cd $PLANH_ROOT/src/plan-h`
    *   Fetch the latest changes from the main repository:
        ```bash
        git fetch origin
        # (or the name of your upstream remote if different)
        ```
    *   Update your local `main` (or primary development) branch:
        ```bash
        git checkout main
        git pull origin main
        ```
    *   If working on a feature branch, rebase it onto the latest `main`:
        ```bash
        git checkout your-feature-branch
        git rebase main
        ```
        Resolve any merge conflicts.

3.  **Build Verification (Quick Check):**
    *   Perform a quick build of a key component or the entire system to ensure your updated source tree is healthy before making changes.
        ```bash
        # Example: Build all (can take time)
        # hmk all
        # Or build a specific part, e.g., a command you're working on
        # cd cmd/mycommand && hmk
        ```
    *   The goal is to catch integration issues early.

4.  **Development & Testing:**
    *   Create or switch to your feature/bugfix branch: `git checkout -b my-new-feature` or `git checkout existing-bugfix-branch`.
    *   Make your code changes.
    *   Compile frequently.
    *   Write unit tests for new functionality.
    *   Run relevant tests (unit, integration).
    *   Test your changes in QEMU (native or cross-compiled target as appropriate). Refer to `docs/installation_guide/07_run_native_qemu.md` and `docs/installation_guide/09_run_cross_compiled_qemu.md` for QEMU basics.

5.  **Code Formatting & Static Analysis:**
    *   Before committing, format your C/C++ code: `clang-format-17 -i path/to/your/file.c` (ensure it uses the project's `.clang-format` settings).
    *   Run static analyzers (conceptual helpers from `plan-h-env.sh`):
        ```bash
        # plan_h_analyze path/to/your/changed_files_or_directory
        ```
        This would ideally run `clang-tidy` and `cppcheck` as configured for Plan-H.

6.  **Commit Changes:**
    *   Follow commit message guidelines from the Contribution Guide.
    *   `git add <files>`
    *   `git commit -s` (the `-s` adds your Signed-off-by line)

7.  **Contribution Preparation:**
    *   Push your branch to your fork on GitHub.
    *   Create a Pull Request (PR) against the main Plan-H repository.
    *   Ensure CI checks pass on your PR. Engage with reviewer feedback.

## 2. Common Development Tasks & Workflows

### 2.1. Kernel Modification Workflow

1.  **Locate Kernel Source:** Typically under `sys/src/9/` in subdirectories for different architectures (e.g., `pc/` for x86, `arm64/`).
2.  **Make Changes:** Edit relevant kernel files.
3.  **Rebuild Kernel:**
    ```bash
    # cd $PLANH_ROOT/src/plan-h
    # Ensure your environment is set for the correct target architecture
    # (e.g., native, or use `plan_h_cross arm64` for ARM64)
    hmk clean   # Clean previous kernel objects for that arch
    hmk kernel  # Or specific target like '9pc64' or '9pi'
    ```
    (The exact `mk` targets might vary; check kernel Makefiles/mkfiles).
4.  **Test in QEMU:** Update your QEMU command (from installation guide steps 7 or 9) to use the newly built kernel image.
5.  **Debugging:** Use GDB with QEMU (QEMU can expose a GDB stub via `-s -S` flags). Kernel print statements (`print()`, `iprint()`) are also common.

### 2.2. Userland Utility / Command Development

1.  **Locate Source:** Typically under `cmd/` or `port/cmd/`.
2.  **Make Changes:** Edit source files for the command.
3.  **Rebuild Command:**
    ```bash
    # cd $PLANH_ROOT/src/plan-h/cmd/yourcommand/
    hmk clean
    hmk install # 'install' often builds and places binary in the correct place in the build tree
    ```
4.  **Test in QEMU:**
    *   If you have a disk image that's part of your build tree, the `hmk install` might place the command there.
    *   Alternatively, you might need to copy the new binary into your QEMU disk image or use Plan 9's `upas/fs` or `9pfuse` to mount your host's build directory into the running QEMU instance. This allows rapid testing without rebuilding the whole disk image.

### 2.3. Library Development

1.  **Locate Source:** Typically under `lib/` or `port/lib/`.
2.  **Make Changes:** Edit library source files.
3.  **Rebuild Library:**
    ```bash
    # cd $PLANH_ROOT/src/plan-h/lib/yourlibrary/
    hmk clean
    hmk install
    ```
4.  **Rebuild Dependent Components:** Any commands or kernel parts that link against this library will need to be rebuilt to pick up the changes. `hmk all` after a library change often ensures this, or you can rebuild specific dependencies.

### 2.4. Driver Implementation Patterns (Conceptual - requires more specific Plan-H driver docs)

*   **Identify Device:** Determine hardware specifics (PCI IDs, MMIO regions, interrupt lines).
*   **Study Similar Drivers:** Look at existing drivers in Plan-H (e.g., in `sys/src/9/pc/dev*.c`) for patterns.
*   **Memory-Mapped I/O:** Use appropriate functions for reading/writing device registers.
*   **Interrupt Handling:** Implement interrupt service routines.
*   **Device File Interface:** Create entries in the `/dev` namespace for userland interaction (e.g., `#l/ether0` for an Ethernet driver).
*   **Resource Management:** Locking, memory allocation.
*   **Testing:** Incremental testing in QEMU (QEMU can emulate some common devices like e1000 NICs, VirtIO devices). For real hardware, direct testing is needed.

## 3. Using Helper Functions from `plan-h-env.sh`

Remember the helpers configured in your environment:
*   `plan_h_cross <arch>`: Quickly switch toolchains for cross-compilation.
*   `hmk [...]`: Standard way to invoke builds.
*   `plan_h_build <component|all>`: High-level build command.
*   `plan_h_fuzz <target>`: For fuzz testing components.
*   `plan_h_analyze <dir>`: For running static analysis.

## 4. Further Reading

*   **[Plan-H Architectural Manifesto](../../ARCHITECTURAL_MANIFESTO.md)**
*   **[C17 Migration Guide](./c17-migration-guide.md)**
*   **[Performance Baselines](../architecture/performance-baselines.md)** (if optimizing)
*   Plan 9 papers and documentation (for general Plan 9 concepts).

Happy developing on Plan-H!
