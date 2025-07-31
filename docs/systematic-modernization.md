# Systematic Modernization Protocol for Harvey OS

## 1. Introduction

*   This document details the protocol for the systematic modernization of Harvey OS components. It expands upon the principles laid out in the "Architectural Manifesto."
*   The primary goals are to enhance portability, maintainability, security, and POSIX compatibility where appropriate, while respecting the core Plan 9 heritage of Harvey OS.
*   This protocol covers C17 migration, Plan 9 API management, and the introduction of POSIX conformance testing.

## 2. C17 Migration Protocol

The strategic goals for C17 adoption are outlined in the `ARCHITECTURAL_MANIFESTO.md`. This section details the high-level process for achieving those goals. For specific code-level guidance, common migration patterns, detailed examples, and validation strategies for converting Plan 9 C to C17, please refer to the comprehensive **[Harvey OS C17 Migration Guide](docs/operations/c17-migration-guide.md)**.

*   **Scope:** The C17 migration applies to all C code within Harvey OS, including legacy code and new module development. Key focus areas include the `acme/` sources and `modern/` utilities.
*   **Objective:** The primary objective is to transition from K&R/Plan 9 C to modern C17, utilizing its features to develop safer, more portable, and maintainable code.
*   **Guiding Principles for Migration:**
    *   Clarity and correctness are paramount.
    *   Migration should occur incrementally with minimal disruption.
    *   All changes must be thoroughly covered by tests.
    *   Adherence to C17 standards is mandatory, compiling cleanly with strict compiler flags (e.g., `-std=c17 -Wall -Wextra -pedantic -Werror`).

*   **High-Level Migration Process (Superseding `c17-roadmap.md`):**
    1.  **Component Identification & Prioritization:** The Architecture Review Board (ARB) identifies and prioritizes components/modules for C17 migration. Factors include criticality, Plan 9 dependencies, potential benefits from C17 features, and developer availability. A schedule or backlog is maintained by the ARB.
    2.  **Analysis & Planning (per component):** Static analysis tools (e.g., `clang-tidy`, enhanced `scripts/analyze_repo.py`) are used to identify non-C17 constructs and Plan 9 idioms. Specific refactoring tasks are then defined for the component.
    3.  **Code Refactoring & Validation:** This is the core technical phase. Developers will follow the patterns and practices detailed in the **[Harvey OS C17 Migration Guide](docs/operations/c17-migration-guide.md)**. This includes updating function declarations, managing memory allocations safely, replacing Plan 9 specific types and routines with C17 equivalents, and applying standard formatting and linting tools. Crucially, this phase involves rigorous compilation with strict C17 flags and comprehensive testing (unit, behavioral, static/dynamic analysis) as specified in the migration guide.
    4.  **Review & Merge:** All migrated code undergoes peer review, focusing on correctness, adherence to C17 standards, project style, and the guidelines set forth in the C17 Migration Guide.

*   **Tracking Progress:**
    *   The ARB maintains a dashboard or document tracking the C17 migration status of components.
    *   The content of `docs/acd-plan9-audit.md` is considered retired or will be integrated into this broader tracking by the ARB.

## 3. Plan 9 API Management and Deprecation

*   **Objective:** Strategically manage the use of Plan 9 specific APIs to balance Harvey OS's heritage with the goals of portability and modernization.
*   **API Categories:**
    1.  **Core Retained APIs:** Essential Plan 9 APIs that define Harvey's character and are considered stable (e.g., file system interface, namespace manipulation). These will be maintained.
    2.  **Wrapped APIs:** Plan 9 APIs that provide useful functionality but may not be directly portable. These may be wrapped by a POSIX-compatible interface for external use, while their Plan 9 nature is preserved internally.
    3.  **Deprecated APIs:** Plan 9 APIs that are obsolete, problematic, or directly conflict with modernization goals (e.g., certain older IPC mechanisms if POSIX alternatives are adopted for wider use).
*   **Deprecation Process:** The strategy for managing Plan 9 specific APIs, including identifying candidates for retention, wrapping, or deprecation, is guided by the `ARCHITECTURAL_MANIFESTO.md`. The formal lifecycle, impact analysis, migration support, and stakeholder communication for all deprecations (including APIs, features, or architectures) are detailed in the **[Harvey OS Deprecation Procedures](docs/operations/deprecation-procedures.md)** document.
*   **Initial Deprecation Candidates for Plan 9 APIs:** (To be determined by ARB based on ongoing review, but could include specific functions identified in `scripts/analyze_repo.py` if they are problematic and fit the criteria outlined in the Deprecation Procedures).

## 4. POSIX Conformance Test Suite Strategy

*   **Objective:** To validate and improve the POSIX compatibility of Harvey OS components, where such compatibility is a stated goal.
*   **Selection of Test Suite:**
    *   Investigate standard POSIX test suites (e.g., parts of Open POSIX Test Suite, Austin Group's test suites).
    *   Focus on tests relevant to Harvey OS's implemented features and architectural goals. It's not expected to achieve 100% POSIX compliance across the entire system, but rather for specific interfaces or utilities intended for broader compatibility.
*   **Integration into CI:**
    *   Gradually integrate relevant POSIX tests into the CI pipeline (as per `docs/ci-strategy.md`).
    *   Initially, failures may be informational, then progress to being mandatory pass criteria for specific components.
*   **Reporting:**
    *   Track POSIX conformance levels for key components.

This protocol provides a framework for evolving Harvey OS in a deliberate and structured manner, ensuring that modernization efforts are coherent and aligned with the overall architectural vision.
