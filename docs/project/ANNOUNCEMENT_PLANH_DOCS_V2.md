# Announcement: Plan-H Architectural Documentation v2.0 Released!

**Date:** YYYY-MM-DD (To be filled in upon actual release)

We are thrilled to announce a landmark achievement for the Plan-H project: the release of our **Architectural Documentation Suite v2.0**! This comprehensive suite represents a significant overhaul and expansion of our project documentation, establishing a clear technical direction, operational frameworks, and robust community engagement guidelines.

A key part of this release is the formal adoption of **"Plan-H"** as our project identity, moving forward from "Harvey OS" to reflect our distinct vision and modernized approach.

## What's New in Documentation v2.0?

This release is the culmination of extensive review and refinement, incorporating feedback to create a truly valuable resource for all users, developers, and contributors. Highlights include:

1.  **[Plan-H Architectural Manifesto (v2.0)](../../ARCHITECTURAL_MANIFESTO.md):** Our cornerstone document, now updated with:
    *   Quantitative Use Cases and success criteria.
    *   Detailed strategies for Multi-Architecture Support (Tiering), Security, Performance Engineering, Environmental Resilience, and Deprecation.
    *   Clear links to all new supporting detailed specifications.

2.  **Comprehensive Installation Guide (`docs/installation_guide/`):**
    *   A 10-step guide to setting up a full Plan-H development environment from scratch, building, and running Plan-H in QEMU for multiple architectures.
    *   Each step now includes detailed "Troubleshooting" and "Architectural Alignment" sections.
    *   Includes an enhanced validation script design in Step 10.

3.  **Developer Journey Map ([DEVELOPER_JOURNEY.md](../../DEVELOPER_JOURNEY.md)):** A new central navigation hub to guide you to the right information based on your goals.

4.  **Detailed Architectural Specifications (`docs/architecture/`):**
    *   `tier-criteria.md`: Measurable criteria for architecture support tiers.
    *   `security-model.md`: In-depth look at our threat model, security principles, and validation.
    *   `performance-baselines.md`: Strategy for reference workloads and regression detection.

5.  **Practical Operational Guides (`docs/operations/`):**
    *   `c17-migration-guide.md`: Actionable patterns for modernizing C code.
    *   `deprecation-procedures.md`: Formal lifecycle for deprecating features/APIs.
    *   `day2-development-workflow.md`: Guidance for ongoing development tasks.

6.  **Clear Governance Framework (`docs/governance/`):**
    *   `arb-charter.md`: Defines the Architecture Review Board.
    *   `rfc-process.md`: How to propose significant changes.

7.  **Enhanced Community Engagement Docs (`docs/community/`):**
    *   `contribution-guide.md`: Your guide to contributing to Plan-H.
    *   `tier2-maintainer-handbook.md`: For those maintaining specific architectures.

8.  **Automation Script Designs (`docs/scripts_and_automation/`):**
    *   Conceptual designs for scripts to validate docs, check architectural compliance, sync architecture status, and more. Includes an `implementation-status.md` for clarity.

9.  **Project Meta-Documentation (`docs/project/`):**
    *   `project_identity_migration.md`: Explaining the "Plan-H" name change.
    *   `documentation_versioning.md`: How we version our docs.
    *   `SUBMISSION_CHECKLIST.md`: Ensuring quality for these releases.

## Call to Action & Feedback

*   **Explore the New Docs:** Start with the [DEVELOPER_JOURNEY.md](../../DEVELOPER_JOURNEY.md) or dive directly into the [Plan-H Architectural Manifesto (v2.0)](../../ARCHITECTURAL_MANIFESTO.md). New contributors should begin with the [Installation Guide](../../docs/installation_guide/README.md).
*   **Adopt "Plan-H":** Please begin using "Plan-H" in all new communications and contributions. A migration script (`scripts/migrate-to-plan-h.sh`) is available for updating existing local documentation if needed (use with care on your own forks/branches).
*   **Provide Feedback:** This is a living documentation suite. We welcome your feedback! Please open an issue on our GitHub repository with the label `arch-docs-v2-feedback` for any suggestions, corrections, or clarifications. There will be a formal 14-day review period from this announcement.

This documentation suite is a testament to our commitment to transparency, quality, and community-driven development. We believe it will serve as a robust foundation for Plan-H's growth and success.

Thank you for your continued support and involvement!

The Plan-H Team (via the ARB, once formed)
