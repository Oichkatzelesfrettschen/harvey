# Plan-H Automation Scripts: Implementation Status

This document tracks the implementation status of various automation scripts conceptualized for the Plan-H project. These scripts are designed to aid in documentation validation, architectural compliance, periodic synchronization, and improving the developer/user experience.

For detailed design specifications of each script, please refer to their respective `*_design.md` files in this directory (`docs/scripts_and_automation/`).

## Current Status of Automation Scripts

As of YYYY-MM-DD, the status of the conceptualized scripts is as follows:

| Script Name                               | Design Document                                       | Current Status          | Notes                                                                 |
|-------------------------------------------|-------------------------------------------------------|-------------------------|-----------------------------------------------------------------------|
| `validate-docs.sh`                        | [validate_docs_script_design.md](./validate_docs_script_design.md) | Conceptual Design Only  | Core features: link checking, YAML validation.                        |
| `architecture-check.sh`                   | [architecture_check_script_design.md](./architecture_check_script_design.md) | Conceptual Design Only  | Advanced; for checking code against architectural rules.            |
| `weekly-architecture-sync.sh`             | [weekly_architecture_sync_script_design.md](./weekly_architecture_sync_script_design.md) | Conceptual Design Only  | Orchestrates other scripts for a periodic report.                     |
| `check-doc-freshness.sh`                  | [check_doc_freshness_script_design.md](./check_doc_freshness_script_design.md) | Conceptual Design Only  | Heuristic checks for potentially stale documentation.                 |
| `installation-telemetry.sh` (or mechanism)| [installation_telemetry_script_design.md](./installation_telemetry_script_design.md) | Conceptual Design Only  | For opt-in collection of installation metrics; privacy is paramount. |
| `migrate-to-plan-h.sh`                    | [N/A - Script content exists in `scripts/`](../../../scripts/migrate-to-plan-h.sh) | Implemented (Content Generated) | Script content created; requires manual execution and review.         |
| `plan-h-validate-complete.sh` (example)   | [N/A - Example in Install Guide](../../installation_guide/10_basic_validation_next_steps.md) | Implemented (Example in Docs) | Example script provided in installation guide for users to create.    |


**Status Definitions:**
*   **Conceptual Design Only:** The script's purpose and features are documented in its design file, but no runnable code has been implemented yet.
*   **Partially Implemented:** Some core features of the script are implemented. Details would be provided in the "Notes" column.
*   **Implemented (Content Generated):** The content of the script has been generated (e.g. `migrate-to-plan-h.sh`). It needs to be placed in the `scripts/` directory and tested.
*   **Implemented (Example in Docs):** A working example of the script is provided within documentation for users to adapt.
*   **Fully Implemented:** The script is implemented, tested, and available in the `scripts/` directory of the Plan-H repository.

## Implementation Roadmap & Priorities (Proposed)

Actually implementing these automation scripts will significantly enhance the quality, maintainability, and developer experience of Plan-H. The following is a suggested prioritization, subject to ARB review and contributor availability:

1.  **Priority 1 (Critical for Maintainability):**
    *   `validate-docs.sh`: Essential for ensuring documentation links remain valid and structured data (YAML) is correct. Should be integrated into CI as a pre-requisite for merging documentation changes.
    *   `migrate-to-plan-h.sh`: While its content is generated, it needs to be thoroughly tested and run once the documentation suite is otherwise stable to ensure all "Harvey OS" references are correctly updated to "Plan-H". (This is a one-time critical run for the bulk of existing docs, then for new contributions).

2.  **Priority 2 (Enhancing Developer Workflow & Quality):**
    *   `plan-h-validate-complete.sh` (Official version in `scripts/`): Formalize the example script from the installation guide into a robust, maintained script in `scripts/`.
    *   `check-doc-freshness.sh`: Helps identify documentation that needs attention as the codebase evolves.

3.  **Priority 3 (Advanced Architectural & Project Health):**
    *   `weekly-architecture-sync.sh`: Provides valuable periodic insights for the ARB and community. Depends on other scripts being at least partially implemented.
    *   `architecture-check.sh`: A more complex, long-term effort. Initial versions could focus on simple, high-value architectural rules.
    *   `installation-telemetry.sh` (or mechanism): Requires careful consideration of privacy and infrastructure for data collection; valuable for long-term improvement of the setup process.

This status document and roadmap will be updated as scripts are implemented and their designs evolve. Community contributions towards implementing these scripts are highly encouraged and should follow the standard [Plan-H Contribution Guide](../../community/contribution-guide.md).
