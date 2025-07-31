# Plan-H Documentation Suite v2.0 - Pre-Submission Checklist

This checklist is to be reviewed before committing and merging the "Plan-H Documentation Suite v2.0" (resulting from the "Plan-H Final Documentation Polish and Integration" plan). Its purpose is to ensure all key requirements, feedback points, and quality checks have been addressed.

## I. Identity Coherence

*   [ ] **Global Nomenclature Migration:**
    *   [ ] `scripts/migrate-to-plan-h.sh` content created.
    *   [ ] `docs/project/project_identity_migration.md` created and explains the change.
    *   [ ] All *newly created and modified documents in this final plan* consistently use "Plan-H", `PLANH_ROOT`, `plan-h-env.sh`, etc., where appropriate (manual check of generated content).
    *   [ ] A manual spot-check of a few older, key documents (like the Installation Guide intro, Developer Journey) confirms the naming appears correct.
    *   *(Note: Actual execution of `migrate-to-plan-h.sh` across the *entire* repository is a separate manual step after AI generation is complete and before the real commit).*

## II. Content & Structure (Key Documents)

*   [ ] **`PLANH_MANIFESTO.md` (v2.0):**
    *   [ ] Includes quantitative Use Cases.
    *   [ ] Links to `tier-criteria.md`, `security-model.md`, `performance-baselines.md`, `c17-migration-guide.md`, `deprecation-procedures.md`, governance docs, and community docs.
    *   [ ] Environmental Resilience and Security Architecture sections are robust.
    *   [ ] YAML front matter added.
*   [ ] **`DEVELOPER_JOURNEY.md`:**
    *   [ ] Created and acts as a central navigation hub.
    *   [ ] Links to all major documentation areas.
    *   [ ] YAML front matter added.
*   [ ] **Installation Guide (`docs/installation_guide/` 01-10 & README):**
    *   [ ] All 10 steps + main README for the guide created.
    *   [ ] Documents 01-10 include "Troubleshooting" and "Architectural Alignment" sections.
    *   [ ] Document 10 (`10_basic_validation_next_steps.md`) includes the enhanced `plan-h-validate-complete()` script with dynamic discovery.
    *   [ ] `docs/installation_guide/README.md` YAML front matter added.
*   [ ] **Core Architectural Docs (`docs/architecture/`):**
    *   [ ] `tier-criteria.md` created and linked from Manifesto. YAML front matter added.
    *   [ ] `security-model.md` created and linked from Manifesto.
    *   [ ] `performance-baselines.md` created and linked from Manifesto.
*   [ ] **Operational Guides (`docs/operations/`):**
    *   [ ] `c17-migration-guide.md` created.
    *   [ ] `deprecation-procedures.md` created.
    *   [ ] `day2-development-workflow.md` created.
*   [ ] **Governance Framework (`docs/governance/`):**
    *   [ ] `arb-charter.md` created.
    *   [ ] `rfc-process.md` created.
    *   [ ] `decision-log.md` (placeholder) created.
*   [ ] **Community Engagement (`docs/community/`):**
    *   [ ] `contribution-guide.md` created. YAML front matter added.
    *   [ ] `tier2-maintainer-handbook.md` created.
*   [ ] **Automation Script Designs (`docs/scripts_and_automation/`):**
    *   [ ] All 5 design documents created (`validate_docs_`, `architecture_check_`, `weekly_architecture_sync_`, `check_doc_freshness_`, `installation_telemetry_`).
    *   [ ] `implementation-status.md` created and accurately reflects their "Conceptual Design Only" or "Content Generated" status.
*   [ ] **Project Meta-Docs (`docs/project/`):**
    *   [ ] `documentation_versioning.md` created.
    *   [ ] `project_identity_migration.md` created.
    *   [ ] `documentation_review_summary.md` (from previous plan) updated.
    *   [ ] This `SUBMISSION_CHECKLIST.md` created.

## III. Link Integrity & Navigation

*   [ ] **Root `README.md`:** Updated with a prominent "Getting Started" section linking to the installation guide.
*   [ ] **Conceptual Link Validation:** A manual check of key navigation paths (e.g., from `DEVELOPER_JOURNEY.md` to major sections, from Manifesto to supporting docs, within Installation Guide) suggests links are structurally correct (e.g., `../../ARCHITECTURAL_MANIFESTO.md` vs `../ARCHITECTURE_MANIFESTO.md`).
*   *(Note: Actual automated link checking via `validate-docs.sh` depends on that script's implementation).*

## IV. Documentation Standards

*   [ ] **YAML Front Matter:** Added to specified key documents (`PLANH_MANIFESTO.md`, `DEVELOPER_JOURNEY.md`, `docs/installation_guide/README.md`, `tier-criteria.md`, `contribution-guide.md`).
*   [ ] **Consistency:** Plan-H naming used consistently in all documents generated/modified *within this final plan*.
*   [ ] **Clarity & Readability:** Documents have been structured for progressive disclosure where appropriate.

## V. Final Preparations (Conceptual for AI, Real for Human)

*   [ ] **Version Tags Prepared:** Conceptual: `docs-v2.0.0` tag ready for when Manifesto hits v2.0.0. Versioning strategy documented in `documentation_versioning.md`.
*   [ ] **Announcement Draft Reviewed:** `ANNOUNCEMENT_PLANH_DOCS_V2.md` (to be created in next step) content will be reviewed.

## VI. Sign-off (Conceptual)

*   **AI Agent (Jules):** Confirms all document generation and modification tasks in the "Plan-H Final Documentation Polish and Integration" plan have been executed to the best of its ability, based on user directives.
*   **User Review:** Pending final review of all generated artifacts by the user.

This checklist helps ensure that the documentation suite is coherent, addresses primary feedback, and is ready for formal commit and announcement as "Plan-H Documentation v2.0".
