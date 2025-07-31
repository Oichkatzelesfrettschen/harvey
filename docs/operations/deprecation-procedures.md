# Harvey OS Deprecation Procedures

This document outlines the formal procedures for deprecating and eventually removing components, APIs, features, or architectures from Harvey OS. The goal is to manage the evolution of the OS transparently, minimize disruption for users and developers, and keep the codebase lean and modern.

This process is overseen by the Architecture Review Board (ARB) and complements the deprecation strategy outlined in the `ARCHITECTURAL_MANIFESTO.md`.

## 1. Philosophy of Deprecation

Deprecation is a natural part of software evolution. Reasons for deprecation in Harvey OS can include:
*   Obsolescence (the feature is no longer relevant).
*   Redundancy (better alternatives exist within the OS).
*   High maintenance burden relative to value.
*   Security vulnerabilities that cannot be reasonably mitigated.
*   Conflict with core architectural principles or strategic direction.
*   An architecture failing to meet its Tier criteria for an extended period.

We aim for a clear, communicated, and phased deprecation process.

## 2. The Deprecation Lifecycle

A component or feature targeted for deprecation goes through the following stages:

1.  **Proposal & Initial Assessment (RFC)**
2.  **ARB Review & Decision**
3.  **Announcement & Deprecation Period**
4.  **Migration Support (if applicable)**
5.  **Removal/Archival**

### 2.1. Proposal & Initial Assessment (RFC)

*   Any contributor can propose a deprecation by submitting an RFC to the ARB.
*   The RFC must include:
    *   **Identifier:** The component, API, feature, or architecture to be deprecated.
    *   **Rationale:** Detailed reasons for deprecation, referencing the philosophy above.
    *   **Impact Analysis (Preliminary):**
        *   Known users or dependent components within Harvey OS.
        *   Potential impact on target use cases.
        *   Estimated effort for users/developers to migrate away.
    *   **Proposed Alternatives:** If applicable, recommend alternative solutions or migration paths.
    *   **Proposed Timeline:** Suggested duration for the deprecation period.

### 2.2. ARB Review & Decision

*   The ARB reviews the RFC, considering its merits, the impact analysis, and strategic alignment.
*   The ARB may request further information or broader community feedback.
*   **Decision:** Approve, Reject, or Defer the deprecation proposal. The decision and rationale are documented in the ARB's decision log.

### 2.3. Announcement & Deprecation Period

*   **Public Announcement:** If approved, the deprecation is announced through official Harvey OS channels (e.g., mailing list, website blog, release notes).
    *   The announcement includes the rationale, the specific version in which the item is marked deprecated, the duration of the deprecation period (e.g., 6 months, 1-2 major releases), and recommended alternatives/migration paths.
*   **Code Marking:**
    *   Source code APIs are marked with deprecation attributes (e.g., `[[deprecated("Use new_api() instead. Will be removed after YYYY-MM-DD.")]]` for C/C++).
    *   Build system warnings are enabled for usage of deprecated features.
    *   Documentation is updated to clearly indicate deprecated status and alternatives.
*   **Deprecation Period:** During this period, the feature remains available but its use is discouraged. Bug fixes are typically limited to critical security issues. No new development will use the deprecated feature.

### 2.4. Migration Support

*   **Migration Guide:** For significant deprecations, a migration guide will be created and published. This guide should include:
    *   Step-by-step instructions for transitioning from the deprecated feature to its replacement.
    *   Code examples.
    *   Tooling assistance, if any.
*   **Tooling (Conceptual):**
    *   **Dependency Analysis Scripts:** The project aims to develop or utilize tools that can help identify dependencies on deprecated components within the Harvey OS codebase or potentially in user projects (e.g., by scanning for use of deprecated API headers or function calls).
        *   *Goal:* `harvey-check-deprecated-usage <path_to_source>`
    *   **Automated Refactoring (Limited):** For common, simple deprecations, scripts or `clang-tidy` checks with fixes might be provided to assist in automated code updates.
    *   **Migration Guide Generator (Conceptual):** For complex deprecations, tools might assist in scaffolding sections of a migration guide based on API differences.

### 2.5. Removal/Archival

*   After the deprecation period ends, the component/feature is removed from the active codebase (`main` branch).
*   **Code Archival:** The code for major removed features or architectures may be moved to a separate archive repository (e.g., `harvey-os-archive/`) for historical reference, clearly marked as unmaintained.
*   **Final Notification:** The removal is noted in release announcements.

## 3. Stakeholder Notification System

Clear and timely communication is key to a smooth deprecation process.

*   **Initial RFC:** Publicly viewable, allowing for early community input.
*   **ARB Decision:** Published in decision logs.
*   **Official Deprecation Announcement:** Via mailing lists, website/blog, release notes of the version where deprecation marking occurs.
*   **Build System Warnings:** Direct feedback to developers using deprecated features.
*   **Documentation Banners:** Prominent notices in documentation for deprecated items.
*   **Pre-Removal Reminder:** A reminder announcement shortly before the end of the deprecation period and actual removal.
*   **Release Notes for Removal:** Clear mention in the release notes of the version where the item is finally removed.

## 4. Special Cases

*   **Architectures:** Deprecation of a supported architecture follows this lifecycle, typically triggered by failure to meet its Tier criteria over time, as detailed in `docs/architecture/tier-criteria.md`. The `386/` directory is an example of an ongoing deprecation as per the Architectural Manifesto.
*   **Urgent Security Deprecations:** In rare cases of severe, unpatchable security vulnerabilities in a feature with limited use, the ARB may approve an accelerated deprecation timeline. This will be clearly communicated with strong justification.

This structured approach to deprecation helps Harvey OS evolve responsibly, ensuring stability and predictability for its community.
