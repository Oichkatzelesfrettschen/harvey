# Project Identity Migration: Harvey OS to Plan-H

## Date: YYYY-MM-DD

## 1. Announcement of Name Change

This document records the official decision and process for migrating the project identity from "Harvey OS" (and its variants like "harvey-os") to **"Plan-H"**. This change reflects the project's evolution into a distinct distribution and modernization effort rooted in the Harvey OS heritage but with its own forward-looking architectural vision and community goals.

All new documentation, code comments, and official communications should use "Plan-H" moving forward. Existing assets are being updated.

## 2. Rationale

The name change to "Plan-H" aims to:
*   **Establish a Clear Identity:** Differentiate this project as a specific, modern evolution and distribution, distinct from the original Harvey OS or other Plan 9 derivatives.
*   **Signal Evolution:** Mark a significant step in the project's lifecycle, emphasizing the new architectural framework, modernized practices, and refined goals.
*   **Unify Branding:** Provide a consistent name for use across all project assets, from documentation and code to community channels.

While "Plan-H" honors its Harvey OS lineage, the new name signifies its unique trajectory.

## 3. Migration Scope

The nomenclature migration impacts:
*   All documentation files (`.md`).
*   Variable names in scripts and code (e.g., `HARVEY_ROOT` to `PLANH_ROOT`).
*   Public-facing text, announcements, and community platforms.
*   File and directory names where "harvey" is part of the identity (requires manual review).

**Note on Historical References:** References to the original Harvey OS project (e.g., specific URLs like `github.com/Harvey-OS/...`, academic papers, or historical discussions) will be preserved to maintain accurate attribution and historical context. The migration script attempts to be careful about this.

## 4. Migration Process & Tooling

A shell script, `scripts/migrate-to-plan-h.sh`, has been created to automate a significant portion of the text replacement in markdown files.

*   **Script Location:** `[scripts/migrate-to-plan-h.sh](../../scripts/migrate-to-plan-h.sh)`
*   **Functionality:** The script uses `find` and `sed` to perform global search and replace operations for various forms of "Harvey OS" with "Plan-H" and related terms (e.g., `harvey-env` to `plan-h-env`). It creates backup files (`.planHbak`) before modifying files.
*   **Manual Review Required:** After running the script, a thorough manual review of changes is essential to catch any unintended replacements or missed instances, especially in complex contexts or where "Harvey" might be used in a non-project-specific way.
*   **File/Directory Renaming:** Manual renaming of specific files or directories (e.g., `harvey-env.sh` itself to `plan-h-env.sh`) will be necessary.

## 5. Impact on Contributors

*   **Use New Naming:** All new contributions should use "Plan-H" and related terms (`PLANH_ROOT`, `plan-h-env.sh`, etc.).
*   **Update Local Clones:** After the migration is merged into the main repository, contributors will need to update their local clones. Some local configuration (like build scripts or environment variables) might need manual adjustment if they hardcoded old names.
*   **Be Aware of Context:** When working with older branches or historical code, the old naming will still be present.

This migration is a critical step in solidifying Plan-H's identity. Community cooperation in adopting the new nomenclature is greatly appreciated.
