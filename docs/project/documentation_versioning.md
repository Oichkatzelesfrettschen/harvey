# Harvey OS Documentation Versioning Strategy

This document outlines the strategy for versioning the Harvey OS documentation suite. A clear versioning approach helps users and contributors understand the currency of documents, track changes over time, and align documentation with specific software releases or architectural states.

## 1. Core Principles

*   **Clarity:** Versioning should be easy to understand and follow.
*   **Consistency:** Apply versioning rules consistently across different types of documentation.
*   **Alignment:** Where appropriate, documentation versions should align with Harvey OS software releases or major architectural milestones.
*   **Accessibility:** Users should be able to easily find and access documentation relevant to their version of Harvey OS or their area of interest.

## 2. Versioning Schemes by Document Type

Different types of documentation may benefit from slightly different versioning schemes:

### 2.1. `ARCHITECTURAL_MANIFESTO.md`
*   **Scheme:** Semantic Versioning (MAJOR.MINOR.PATCH), e.g., `v2.0.0`, `v2.1.0`.
*   **MAJOR (e.g., v2.x.x -> v3.x.x):** Indicates significant, potentially breaking changes in core architectural vision, principles, or governance structure. Requires ARB approval and community consultation.
*   **MINOR (e.g., v2.0.x -> v2.1.x):** Represents substantial additions, refinements, or new sections that add value but do not fundamentally alter the core tenets (e.g., adding a new major architectural strategy document link, significantly expanding a section like Environmental Resilience). Requires ARB approval.
*   **PATCH (e.g., v2.0.0 -> v2.0.1):** For clarifications, typo fixes, minor corrections, or updates to links that do not change the meaning or intent of the Manifesto. Can be approved by documentation maintainers or lead architect.
*   **Location of Version:** The version number will be clearly stated at the top of the `ARCHITECTURAL_MANIFESTO.md` file itself (e.g., `# Harvey OS Architectural Manifesto (v2.0.0)`).

### 2.2. Core Architectural & Operational Documents
Includes documents in `docs/architecture/`, `docs/operations/`, `docs/governance/`, `docs/community/`, `docs/installation_guide/`, `docs/scripts_and_automation/`.
*   **Scheme:** Date-based versioning (YYYY-MM-DD) reflecting the last significant update. This is simpler for documents that evolve more organically.
*   **Alternative Scheme (Considered):** Aligning with Manifesto MINOR versions if changes are tightly coupled. For example, if Manifesto v2.1 introduces a new process, the document detailing that process could also be marked v2.1 or reflect the date of its alignment with that Manifesto version.
*   **Current Decision:** Primarily date-based for simplicity. A "Last Updated: YYYY-MM-DD" line at the top of each document.
*   **Change Management:** Significant changes (new sections, process overhauls) should be noted in a brief changelog at the end of the document or in commit messages.

### 2.3. API Reference Documentation (If generated)
*   **Scheme:** Directly tied to Harvey OS software release versions (e.g., API docs for Harvey OS v0.5.0).
*   **Generation:** Ideally auto-generated from source code comments (e.g., using Doxygen) as part of the release process.

## 3. Git Tagging for Documentation Releases

*   To provide stable snapshots of the entire documentation suite, Git tags will be used.
*   Tags will correspond to major Harvey OS software releases or significant architectural documentation milestones (e.g., `docs-v2.0.0` corresponding to `ARCHITECTURAL_MANIFESTO.md v2.0.0` and the state of supporting docs at that time).
*   This allows users to check out a version of the documentation that matches a specific state or release of the Harvey OS project.

## 4. Deprecation Marking for Documents

When a document is superseded or becomes obsolete:
*   A clear deprecation notice will be added at the top of the document.
    ```markdown
    <!-- DEPRECATED: This document has been superseded by [New Document Name](path/to/new/document.md) as of YYYY-MM-DD. -->
    <!-- Reason: [Brief explanation] -->
    <!-- Please refer to the new document for up-to-date information. -->
    ```
*   The deprecated document may be moved to an `archive/` subdirectory within `docs/` after a suitable period.
*   Links pointing to the deprecated document should be updated throughout the documentation suite.

## 5. Review and Update Process

*   Documentation will be reviewed periodically by the ARB and community for accuracy and relevance.
*   The `scripts/check-doc-freshness.sh` (conceptual design) aims to help identify potentially stale documents.
*   Updates follow standard contribution guidelines (PRs, reviews).

This versioning strategy aims to keep the Harvey OS documentation clear, current, and navigable as the project evolves.
