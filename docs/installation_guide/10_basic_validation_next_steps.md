# Step 10: Validation, Integration, and Your Plan-H Journey

Congratulations on successfully setting up your Plan-H development environment, building the OS for both native and cross-compiled targets, and running them in QEMU! This final step guides you through validating your setup, understanding how it aligns with the broader Plan-H architecture, and navigating your next steps in the ecosystem.

## 1. Comprehensive Validation Protocol

This section provides checks to ensure your Plan-H instances are functional.

### 1.1. Native Architecture Validation (in QEMU)

After booting your native Plan-H build in QEMU (as per Step 7):

1.  **Boot Confirmation:** Ensure the system boots to a login prompt or shell without critical errors.
2.  **Basic Command Execution:**
    *   Log in (e.g., user `none` or `glenda`).
    *   Execute fundamental commands:
        ```sh
        ls /
        ls /bin
        echo "Hello from Plan-H!"
        cat /dev/time
        cat /dev/cputime
        rc -c 'echo $path'
        ```
    *   Verify they produce expected output.
3.  **File System Operations (Basic):**
    *   Create a directory: `mkdir /tmp/testdir`
    *   Navigate into it: `cd /tmp/testdir`
    *   Create a file: `echo "test content" > testfile.txt`
    *   List contents: `ls`
    *   Display content: `cat testfile.txt`
    *   Remove file and directory: `rm testfile.txt && cd / && rm /tmp/testdir`
4.  **Process Creation Verification:**
    *   Run a simple command in the background (if supported by the shell): `ls /bin &`
    *   Check process status (Plan-H may have a command like `ps` or a way to inspect `/proc`). Example: `cat /proc/*/status | grep ls` (adapt to Plan-H's process inspection method).

### 1.2. Cross-Architecture Validation (e.g., ARM64 in QEMU)

After booting your cross-compiled Plan-H build (e.g., ARM64) in QEMU (as per Step 9):

1.  **Boot Confirmation:** Ensure boot to a login/shell.
2.  **Basic Command Execution:** Repeat the same basic commands as in section 1.1.2.
3.  **Architectural Behavior Parity (Conceptual):**
    *   For a few key commands or simple test programs, the output or behavior should ideally be identical to the native build (unless the command is inherently architecture-dependent). This is more of an advanced check.
4.  **Binary Compatibility Verification (Conceptual):**
    *   If you have simple, portable C test programs, try compiling them with the native toolchain from Step 6 and then with the cross-compiler from Step 8. Then, try running the cross-compiled test binary in the cross-compiled QEMU environment. (e.g., a "hello world" program).
5.  **Performance Baseline Comparison (Advanced/Future):**
    *   While full performance testing is complex, note if the cross-compiled environment feels significantly slower or less responsive than expected.
    *   Formal performance baseline comparisons would use the scripts and definitions from `docs/architecture/performance-baselines.md` and are typically part of CI or dedicated performance analysis, but it's good to be mindful.

## 2. Development Environment Health Check

The `setup-plan-h-dev-env.sh` (formerly `setup-harvey-dev-env.sh`) script aimed to create a consistent environment. You can use the following script to perform a more detailed health check.

**Conceptual `plan-h-validate-complete.sh` script:**
(This is a template; you can create this script in `$PLANH_ROOT/scripts/` for your use)
```bash
#!/bin/bash
# plan-h-validate-complete.sh
# Validates the completeness of the Plan-H development setup

echo "=== Plan-H Complete Installation Validation Suite ==="
FAILURES=0

# Source environment to get $PLANH_ROOT, etc.
if [ -z "$PLANH_ROOT" ] && [ -f "$HOME/plan-h-dev/plan-h-env.sh" ]; then
    echo "INFO: PLANH_ROOT not set, sourcing from $HOME/plan-h-dev/plan-h-env.sh"
    source "$HOME/plan-h-dev/plan-h-env.sh"
elif [ -n "$PLANH_ROOT" ] && [ -f "$PLANH_ROOT/plan-h-env.sh" ]; then
    echo "INFO: Sourcing plan-h-env.sh from PLANH_ROOT: $PLANH_ROOT"
    source "$PLANH_ROOT/plan-h-env.sh"
else
    echo "ERROR: plan-h-env.sh not found. Cannot run all checks. PLANH_ROOT is '$PLANH_ROOT'."
    # Attempt to proceed with some checks that don't strictly need PLANH_ROOT sourced for PATH
fi

# Function to find kernel artifacts (dynamic discovery)
find_kernel_artifact() {
    local search_path="${1:-$PLANH_ROOT/src/plan-h}" # Default search path
    local arch_pattern="${2:-$(uname -m)}" # Default to host architecture

    # Common kernel names/patterns for Plan 9 derivatives
    # This list can be expanded based on Plan-H's actual build outputs
    local kernel_names=("9pc64" "9pc" "9pi" "kernel")

    echo "INFO: Searching for kernel for architecture pattern '$arch_pattern' in '$search_path'..."

    # Prioritize specific arch directories if they exist (e.g., sys/src/9/pc, sys/src/9/arm64)
    local arch_specific_paths=("$search_path/sys/src/9/pc" "$search_path/sys/src/9/arm64" "$search_path/sys/src/9/$arch_pattern")

    for path_prefix in "${arch_specific_paths[@]}"; do
        if [ -d "$path_prefix" ]; then
            for kname in "${kernel_names[@]}"; do
                # Look for files that might be kernels, potentially with arch in name
                # Using find's -executable flag can be helpful if kernels are marked as such
                local found_kernel=$(find "$path_prefix" -name "*$kname*" -type f -executable 2>/dev/null | head -1)
                if [ -n "$found_kernel" ]; then
                    echo "$found_kernel"
                    return 0
                fi
                # Fallback for non-executable kernel files or simpler naming
                found_kernel=$(find "$path_prefix" -name "*$kname*" -type f 2>/dev/null | head -1)
                if [ -n "$found_kernel" ]; then
                    echo "$found_kernel"
                    return 0
                fi
            done
        fi
    done

    # Broader search if not found in typical arch-specific paths
    # This is a more general search and might pick up other executables if kernel_names is too broad
    for kname in "${kernel_names[@]}"; do
        local found_kernel=$(find "$search_path" -name "*$kname*" -ipath "*${arch_pattern}*" -type f -executable 2>/dev/null | head -1)
         if [ -n "$found_kernel" ]; then
            echo "$found_kernel"
            return 0
        fi
        found_kernel=$(find "$search_path" -name "*$kname*" -ipath "*${arch_pattern}*" -type f 2>/dev/null | head -1)
        if [ -n "$found_kernel" ]; then
            echo "$found_kernel"
            return 0
        fi
    done

    echo "INFO: Kernel artifact not found with current patterns for '$arch_pattern'."
    return 1
}

# 1. Tool Availability (ensure Plan-H naming)
echo "[Checking core tools...]"
CORE_TOOLS=(rc mk acme 9c sam gcc clang go)
PLAN9_TOOLS=(rc mk acme 9c sam)
MODERN_TOOLS=(gcc clang go cmake ninja)
CROSS_COMPILERS=(aarch64-linux-gnu-gcc arm-linux-gnueabihf-gcc i686-linux-gnu-gcc)
PLANH_HELPERS=(plan_h_cross plan_h_build hmk) # Assuming helpers are renamed

for tool in "${CORE_TOOLS[@]}"; do
    if command -v $tool >/dev/null 2>&1; then
        echo "  ✓ $tool found"
    else
        echo "  ✗ MISSING: $tool"
        ((FAILURES++))
    fi
done

for tool in "${CROSS_COMPILERS[@]}"; do
    if command -v $tool >/dev/null 2>&1; then
        echo "  ✓ $tool cross-compiler found"
    else
        echo "  INFO: $tool cross-compiler not found (may be okay if not targeting this arch)"
    fi
done

echo "[Checking if Plan-H helper functions are defined (requires sourcing plan-h-env.sh)...]"
for helper_func in "${PLANH_HELPERS[@]}"; do
   if command -v "$helper_func" >/dev/null 2>&1 && declare -F "$helper_func" > /dev/null ; then
       echo "  ✓ Plan-H helper function '$helper_func' is defined."
   else
       echo "  ✗ Plan-H helper function '$helper_func' is NOT defined. (Have you sourced plan-h-env.sh?)"
       ((FAILURES++))
   fi
done

# 2. Build Artifacts (Using dynamic discovery)
echo "[Checking for example build artifacts...]"
if [ -n "$PLANH_ROOT" ] && [ -d "$PLANH_ROOT/src/plan-h" ]; then # Assuming source dir is plan-h now
    NATIVE_KERNEL_FOUND=$(find_kernel_artifact "$PLANH_ROOT/src/plan-h")
    if [ -n "$NATIVE_KERNEL_FOUND" ]; then
        echo "  ✓ Example native kernel artifact found: $NATIVE_KERNEL_FOUND"
    else
        echo "  INFO: Example native kernel artifact NOT found (Build for native arch may not have been run or paths differ)."
        # Not incrementing FAILURES for this informational check in basic validation
    fi

    NATIVE_BIN_DIR_EXAMPLE="$PLANH_ROOT/src/plan-h/pc/bin" # Example path, adjust if needed
    if [ -d "$NATIVE_BIN_DIR_EXAMPLE" ]; then
        echo "  ✓ Example native /bin directory found: $NATIVE_BIN_DIR_EXAMPLE"
    else
        echo "  INFO: Example native /bin directory NOT found at $NATIVE_BIN_DIR_EXAMPLE (Path might differ or build not run)"
    fi

    # Check general build directory as before
    if [ -d "$PLANH_ROOT/build" ]; then
        echo "  ✓ General build directory exists: $PLANH_ROOT/build"
        IMG_FILES_COUNT=$(find "$PLANH_ROOT/build" -name "*.img" 2>/dev/null | wc -l)
        if [ "$IMG_FILES_COUNT" -gt 0 ]; then
            echo "  ✓ Found $IMG_FILES_COUNT .img file(s) in $PLANH_ROOT/build"
            find "$PLANH_ROOT/build" -name "*.img" -exec ls -lh {} \;
        else
            echo "  INFO: No .img files found in $PLANH_ROOT/build (This may be normal)"
        fi
    else
        echo "  INFO: General build directory $PLANH_ROOT/build missing"
    fi
else
    echo "  INFO: Cannot check build artifacts, PLANH_ROOT or Plan-H source directory not found as expected."
fi

echo "------------------------------------------"
if [ $FAILURES -eq 0 ]; then
    echo "Development Environment Health Check: PASSED (basic checks)"
    echo "Note: Some checks are informational. Deeper validation requires building and running Plan-H."
else
    echo "Development Environment Health Check: FAILED ($FAILURES critical issues found)"
fi
exit $FAILURES
```

## 3. Troubleshooting Validation Issues

If the `plan-h-validate-complete.sh` script reports failures, or your manual checks in Section 1 don't yield expected results:

*   **`plan-h-env.sh` not found / Helper functions not defined:**
    *   Ensure `setup-plan-h-dev-env.sh` completed successfully and reported the creation of `$PLANH_ROOT/plan-h-env.sh`.
    *   Make sure you have sourced it correctly in your current terminal: `source $PLANH_ROOT/plan-h-env.sh`.
*   **Core tools missing (rc, mk, gcc, clang, etc.):**
    *   This indicates a problem during Step 2 (Execute Setup Script). Review the output of `setup-plan-h-dev-env.sh` for errors from `apt-get install` or other package manager commands.
    *   You might need to manually install the missing package(s) or resolve system conflicts.
*   **Cross-compilers missing:** If you intend to target a specific architecture (e.g., ARM64) and its cross-compiler (e.g., `aarch64-linux-gnu-gcc`) is missing, it's likely an issue from Step 2.
*   **Build artifacts missing:**
    *   Ensure you have successfully run the build commands in Step 6 (native build) and Step 8 (cross-compile).
    *   The paths in the validation script are examples; you may need to adjust them to match the actual output structure of Plan-H's build system. Consult Plan-H documentation or explore your `$PLANH_ROOT/src/plan-h` directory after a build.
*   **General Script Errors:** If the validation script itself has errors, ensure you copied it correctly and it has execute permissions.

If persistent issues arise, seeking help from the Plan-H community (forums, mailing lists) with specific error messages and details from your `installation-report.txt` is recommended.

## 4. Architectural Alignment Verification (Self-Assessment)

Consider how your setup and builds align with Plan-H's goals:

*   **Architectures Built vs. Tier Designations:**
    *   You've likely built for your native architecture (often Tier 1, like x86_64 or ARM64) and cross-compiled for another (e.g., ARM64 if native was x86_64, or i386 if exploring legacy).
    *   Refer to **[Plan-H Architecture Tier Criteria](../../docs/architecture/tier-criteria.md)** to understand the support levels and expectations for these architectures.
*   **Performance Baselines (Awareness):**
    *   While you haven't run formal benchmarks yet, keep in mind that **[Plan-H Performance Baselines and Regression Strategy](../../docs/architecture/performance-baselines.md)** defines performance targets for Tier 1 architectures. Future performance testing would use these.
*   **Security Model Compliance (Awareness):**
    *   As you develop or contribute, remember the principles in the **[Plan-H Security Model](../../docs/architecture/security-model.md)**. The environment setup includes tools (like static analyzers and fuzzers, if installed by the setup script) that can aid in secure development.

## 5. Architectural Context: Installation as Manifestation of Principles

Successfully completing these installation steps means you have a development environment that embodies several core principles from the **[PLANH_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md)** (referred to as PLANH_MANIFESTO.md as per project identity migration):

*   **Multi-architecture Support:** Your setup includes tools for both native and cross-compilation, reflecting Plan-H's multi-architecture philosophy.
*   **Environmental Resilience (via Setup Script):** The `setup-plan-h-dev-env.sh` script aimed to create a consistent, reproducible environment by managing dependencies and toolchains, reducing the "it works on my machine" problem.
*   **Modern Tooling:** Installation of modern compilers (Clang, GCC), build tools, and analysis tools supports the modernization goals of Plan-H.

This setup is your launchpad for contributing to or simply exploring Plan-H.

## 6. Your Next Steps in the Plan-H Ecosystem

You have a functional Plan-H development setup! Here’s how to continue your journey:

### 6.1. Immediate Actions (Recommended This Week)

1.  **Join Community Channels:** (e.g., Mailing Lists, IRC, Forums - check Plan-H official website for links). Introduce yourself!
2.  **Read Key Architectural Documents:**
    *   **[PLANH_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md):** Understand the vision, core principles, and overall direction of Plan-H.
    *   **[Contribution Guide](../../docs/community/contribution-guide.md):** Essential reading before making any contributions.
3.  **Explore the Codebase:** Navigate `$PLANH_ROOT/src/plan-h/` and start exploring components that interest you.
4.  **Find Your First Contribution Area (if desired):** Look at the issue tracker for tasks labeled "good first issue," "help wanted," or documentation improvements.

### 6.2. Learning Pathways

Depending on your interests:

*   **For Kernel Developers:**
    *   Start by studying the code for your native architecture's kernel (e.g., `sys/src/9/pc/` for x86, `sys/src/9/arm64/` for ARM64).
    *   Refer to general Plan 9 kernel documentation and any Plan-H-specific notes or diagrams that may be available in the `docs/architecture/` directory (e.g., `kernel_internals.md` if it exists).
*   **For Toolchain and Userland Development:**
    *   Review the **[C17 Migration Guide](../../docs/operations/c17-migration-guide.md)** if you plan to modernize existing C code.
    *   Explore `cmd/` and `lib/` directories within the Plan-H source.
*   **Interested in Porting to New Architectures?**
    *   Thoroughly read **[Plan-H Architecture Tier Criteria](../../docs/architecture/tier-criteria.md)**.
    *   Study existing ports (e.g., ARM64 if you're familiar with x86).
    *   Prepare an RFC as per the **[Plan-H RFC Process](../../docs/governance/rfc-process.md)**.
*   **Want to Improve Documentation?**
    *   Identify areas in the current docs that could be clearer or more detailed.
    *   Propose changes via Pull Requests, following guidelines in the Contribution Guide.

### 6.3. Contribution Readiness Checklist

Before submitting your first code or significant documentation Pull Request:

- [ ] Development environment validated (using checks in this document).
- [ ] Core principles of `PLANH_MANIFESTO.md` understood.
- [ ] `docs/community/contribution-guide.md` reviewed and understood.
- [ ] Relevant coding style and C17 migration guidelines (`docs/operations/c17-migration-guide.md`) reviewed if contributing code.
- [ ] You have identified an issue to work on or have discussed your proposed change on a community channel.
- [ ] You are familiar with the project's Git workflow (forking, branching, PRs).

Welcome to the Plan-H project! We look forward to your contributions and participation.
(Consider linking to a `DEVELOPER_JOURNEY.md` or a "Where to Go From Here" summary document if one gets created.)
