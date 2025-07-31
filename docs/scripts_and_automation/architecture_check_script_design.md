# Design: architecture-check.sh

## 1. Purpose

The `architecture-check.sh` script is envisioned as an advanced tool to help ensure that code contributions align with Harvey OS's defined architectural principles, patterns, and constraints. This is a highly conceptual and potentially complex script, likely evolving over time. Its goal is to automate parts of architectural compliance review.

## 2. Conceptual Design & Features

*   **Input:** Path to source files or a specific changeset (e.g., a diff from a pull request).
*   **Core Checks (Examples - requires significant underlying logic/definitions):**
    *   **Module Dependency Rules:**
        *   Define allowed dependencies between high-level modules/layers (e.g., "kernel code should not directly call userland library X").
        *   Analyze `#include` directives and function calls to detect violations.
    *   **API Usage Conformance:**
        *   Check for usage of deprecated APIs (cross-reference with `docs/operations/deprecation-procedures.md`).
        *   Ensure usage of specific APIs adheres to documented contracts (e.g., resource acquisition/release patterns).
    *   **Coding Standard Adherence (Beyond `clang-format`):**
        *   Check for project-specific naming conventions not covered by formatters.
        *   Discourage use of problematic language features or library functions.
    *   **Tier-Specific Constraints:**
        *   If certain features or complexities are restricted to Tier 1 components, this script could check if code intended for Tier 2/3 components adheres to those limitations.
    *   **Security Principle Checks (Basic Heuristics):**
        *   Flag use of known insecure functions (e.g., `strcpy` instead of `strncpy_s` or `snprintf`).
        *   Check for missing error handling on critical function calls.
*   **Output:**
    *   Report of potential architectural violations, with file names, line numbers, and descriptions of the rules broken.
    *   Severity levels for violations (e.g., error, warning, info).
    *   Exit code non-zero if critical violations are found.
*   **Implementation Notes:**
    *   This would likely involve static analysis (e.g., custom `clang-tidy` checks, scripts parsing Abstract Syntax Trees using `libclang`, or tools like ArchUnit for other languages if Harvey OS incorporates them).
    *   Architectural rules would need to be formally defined in a machine-readable format or embedded within the script's logic.

## 3. Integration

*   **CI Pipeline:** Run on pull requests to provide early feedback on architectural compliance.
*   **Developer Tool:** Developers could run it locally to check their changes.

## 4. Example (Conceptual) Invocation
```bash
./scripts/architecture-check.sh --diff HEAD~1 # Check changes in the last commit
# Or
# ./scripts/architecture-check.sh ./src/kernel/ # Check a specific directory
```
**Note:** This is a highly aspirational tool. Initial versions might focus on a very small subset of easily checkable rules.
