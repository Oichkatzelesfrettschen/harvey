# Design: weekly-architecture-sync.sh

## 1. Purpose

The `weekly-architecture-sync.sh` script is designed to be run periodically (e.g., weekly by an automated job) to generate a report that helps assess the alignment between the implemented state of Harvey OS and its documented architecture. It aims to detect architectural drift and track progress on key modernization efforts.

## 2. Conceptual Design & Features

*   **Input:** None (operates on the current state of the repository).
*   **Core Tasks & Report Sections:**
    *   **Code Migration Status:**
        *   Execute or incorporate logic from `tools/migration-tracker.py` (or its documented design) to report on C17 migration progress (e.g., percentage of files/lines migrated, list of remaining legacy components).
    *   **Architecture Compliance Summary:**
        *   Run `scripts/architecture-check.sh` (or its documented design) across the entire codebase.
        *   Summarize critical violations or trends.
    *   **Documentation Freshness:**
        *   Execute or incorporate logic from `scripts/check-doc-freshness.sh` (or its documented design).
        *   Report on potentially stale documentation sections.
    *   **Performance Baseline Drift (Future Integration):**
        *   Compare latest performance benchmark results (from CI) against official baselines from `docs/architecture/performance-baselines.md`.
        *   Flag significant deviations.
    *   **Deprecated API Usage Scan:**
        *   Scan codebase for usage of APIs marked for deprecation (from `docs/operations/deprecation-procedures.md`).
        *   Report findings, especially usage that will break in upcoming removals.
*   **Output:**
    *   A consolidated report (e.g., Markdown or HTML) that can be posted to a developer mailing list, wiki, or dashboard.
    *   The report should highlight areas needing attention (e.g., stalled migration, new architectural violations, stale docs).
*   **Implementation Notes:**
    *   A shell script that orchestrates calls to the other specialized scripts (`migration-tracker.py`, `architecture-check.sh`, `check-doc-freshness.sh`).
    *   Requires these underlying scripts to be functional and produce machine-readable or easily summarizable output.

## 3. Integration

*   **Scheduled Job (e.g., GitHub Action cron job):** Run automatically once a week.
*   **ARB Review:** The output report would be a key input for ARB meetings to track architectural health.

## 4. Example (Conceptual) Invocation
```bash
./scripts/weekly-architecture-sync.sh --output report-$(date +%Y-%m-%d).md
```
