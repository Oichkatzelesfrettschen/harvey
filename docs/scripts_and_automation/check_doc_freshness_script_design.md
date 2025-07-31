# Design: check-doc-freshness.sh

## 1. Purpose

The `check-doc-freshness.sh` script aims to identify documentation files or sections that may be stale or outdated based on recent changes to related source code. This is a heuristic tool to prompt review, not a definitive indicator of staleness.

## 2. Conceptual Design & Features

*   **Input:**
    *   Time window for analysis (e.g., "last 7 days", "since tag v1.2.0").
    *   (Optional) Mapping file defining relationships between code modules/directories and documentation files.
*   **Core Heuristics:**
    *   **Code Changes vs. Doc Changes:**
        *   Identify source code files modified within the time window (using `git log`).
        *   Identify documentation files modified within the same window.
        *   If significant code modules have changed but their primary associated documentation (based on naming convention or mapping file) has not, flag the documentation as potentially stale.
    *   **API Signature Changes:**
        *   (Advanced) If tools can extract API signatures (e.g., from C headers using `ctags` or `libclang`), compare signatures from current code with those potentially embedded or described in documentation. Flag discrepancies.
    *   **"TODO", "FIXME", "XXX" in Docs:** Scan documentation for these markers that might indicate unfinished or outdated sections.
    *   **Broken Section Links:** Leverage parts of `validate-docs.sh` to find broken intra-document links, which can indicate refactoring without doc updates.
*   **Output:**
    *   A report listing documentation files/sections flagged as potentially stale, along with the reason (e.g., "Related code in `src/kernel/sched.c` changed recently, but `docs/architecture/scheduler.md` has not.").
*   **Implementation Notes:**
    *   Heavy reliance on `git` commands for history analysis.
    *   Text processing (`grep`, `awk`, `sed`) for marker scanning.
    *   A mapping file (e.g., YAML or JSON) could define explicit links:
        ```yaml
        - code_path: "src/kernel/memory/"
          doc_paths: ["docs/architecture/memory_management.md"]
        - api_header: "include/lib/graphics.h"
          doc_paths: ["docs/api/libgraphics.md"]
        ```

## 3. Integration

*   Part of the `weekly-architecture-sync.sh` report.
*   Can be run manually by developers or documentation maintainers.

## 4. Example (Conceptual) Invocation
```bash
./scripts/check-doc-freshness.sh --since "7 days ago"
# Or using a mapping file
# ./scripts/check-doc-freshness.sh --mapfile .doc_code_map.yaml --since "tag v2.1.0"
```
