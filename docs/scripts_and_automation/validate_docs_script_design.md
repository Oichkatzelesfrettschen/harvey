# Design: validate-docs.sh

## 1. Purpose

The `validate-docs.sh` script is intended to be a crucial tool for maintaining the integrity and quality of the Harvey OS documentation suite. It will perform automated checks to identify common issues like broken links and invalid syntax within documentation files. This script would typically be run as a pre-commit hook and in CI pipelines.

## 2. Conceptual Design & Features

*   **Input:** Path to documentation directory (e.g., `docs/`) or specific markdown files. Defaults to checking all `.md` files in the repository if no path is given.
*   **Core Checks:**
    *   **Internal Link Validation:**
        *   Recursively find all markdown files.
        *   Extract all internal markdown links (e.g., `[text](./path/to/file.md)`, `[text](../another.md#section)`, `[text](#section-in-this-file)`).
        *   Verify that the linked files exist at the specified relative paths.
        *   Verify that section links (`#section`) point to valid section headers within the target file (this is an advanced feature, might require parsing markdown headers).
        *   Handle different base paths correctly if checking individual files vs. a whole directory.
    *   **YAML Block Validation:**
        *   Identify markdown code blocks marked as `yaml`.
        *   Extract the YAML content.
        *   Use a YAML parser (e.g., via Python's `PyYAML` library, or a command-line YAML linter like `yamllint`) to validate syntax.
        *   Report errors with file name, line number (approximate for block), and parser error message.
    *   **(Optional) Code Block Compilation/Syntax Check (Basic):**
        *   Identify markdown code blocks for common languages (e.g., `bash`, `c`, `python`).
        *   For `bash`, potentially run `bash -n` (no-execute syntax check).
        *   For `c`, attempt a basic compile with default flags (e.g., `gcc -fsyntax-only`). This is complex due to needing context/includes. Might be limited to very simple snippets or require special annotations in the markdown.
*   **Output:**
    *   Human-readable summary of checks performed.
    *   Detailed error messages for any failures, including filename, line number (if possible), and description of the error.
    *   Exit code 0 for success, non-zero for failures (to allow CI integration).
*   **Implementation Notes:**
    *   Likely a shell script orchestrating calls to `grep`, `sed`, `awk`, `find`, and potentially Python/Perl for more complex parsing (like the robust link extraction or YAML validation examples from user feedback).
    *   Should be configurable (e.g., to exclude certain directories or link patterns).

## 3. Integration

*   **Pre-commit Hook:** Automatically run before commits to catch issues early.
*   **CI Pipeline:** Run as part of every pull request and merge to `main` to ensure documentation quality.

## 4. Example (Conceptual) Invocation
```bash
./scripts/validate-docs.sh ./docs/
# Or to check a specific file
# ./scripts/validate-docs.sh ./ARCHITECTURAL_MANIFESTO.md
```
