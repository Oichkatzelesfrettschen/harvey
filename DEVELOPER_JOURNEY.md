---
title: Plan-H Developer Journey Guide
category: navigation
audience: all_developers
status: active
last_updated: YYYY-MM-DD
---
# Plan-H Developer Journey Guide

Welcome to Plan-H! This guide is designed to help you navigate our documentation and resources based on what you'd like to achieve. Think of this as your central map to the Plan-H ecosystem.

## Where Do You Want to Start?

### "I want to understand what Plan-H is, its vision, and its core principles."
*   **Start Here:** **[ARCHITECTURAL_MANIFESTO.md](ARCHITECTURAL_MANIFESTO.md)**
    *   This is our foundational document explaining Plan-H's vision, target use cases, core design principles, multi-architecture strategy, security approach, and governance. Essential reading for anyone looking to understand the 'why' and 'what' of Plan-H.

### "I want to set up a full development environment to build and run Plan-H."
*   **Primary Guide:** **[Comprehensive Installation Guide](./docs/installation_guide/README.md)**
    *   This 10-step guide will walk you through setting up all necessary tools (Plan 9 and modern), cloning the source, building Plan-H for native and cross-compiled targets, and running it in QEMU. It uses the `setup-plan-h-dev-env.sh` script.
*   **The Setup Script (Direct Link):** If you're familiar with the process and just need the script, it's intended to be at `scripts/setup-plan-h-dev-env.sh`.
*   **Build Instructions Summary (if you prefer a direct reference after setup):** While the full guide is recommended, `BUILD_INSTRUCTIONS.md` (if it exists and is maintained) or specific sections of the installation guide cover build commands.

### "I want to contribute code (fix a bug, add a feature)."
1.  **Set up your Development Environment:** See the point above. A working environment is crucial.
2.  **Read our Contribution Guidelines:** **[Contribution Guide](./docs/community/contribution-guide.md)**
    *   This covers code style, `clang-format` usage, C17 standards, commit message conventions, the Pull Request process, and testing requirements.
3.  **Find an Issue to Work On:** Check the project's issue tracker on GitHub. Look for issues labeled 'good first issue', 'help wanted', or bugs you can reproduce.
4.  **Understand Code Modernization (if working on older code):** **[C17 Migration Guide](./docs/operations/c17-migration-guide.md)**
5.  **Discuss Your Approach:** For non-trivial changes, it's good to discuss your approach on the relevant issue or a developer mailing list before investing too much time. For significant new features, an RFC might be needed (see below).
6.  **(Conceptual) Architecture Compliance Check:** The idea of `scripts/architecture-check.sh` (design to be documented in `docs/scripts_and_automation/`) would eventually fit here to ensure your changes align with architectural rules.

### "I want to port Plan-H to a new hardware architecture."
1.  **Understand our Multi-Architecture Strategy:**
    *   **[ARCHITECTURAL_MANIFESTO.md](./ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)** (for the philosophy)
    *   **[Architecture Tier Criteria](./docs/architecture/tier-criteria.md)** (for how architectures are classified and supported).
2.  **Study Existing Ports:** Examine how current architectures (e.g., ARM64, x86_64) are implemented in the `sys/src/9/` directory.
3.  **Propose the New Architecture:** Submit an RFC (Request for Comments) as per our **[RFC Process](./docs/governance/rfc-process.md)**. This is a crucial step to discuss the feasibility, strategic value, and maintenance plan for the new port.

### "I want to help modernize legacy Plan 9 C code to C17."
1.  **Primary Guide:** **[C17 Migration Guide](./docs/operations/c17-migration-guide.md)**
    *   This document provides common patterns, examples, and validation strategies for converting older C code to C17.
2.  **Understand the Broader Modernization Effort:** **[Systematic Modernization Protocol](./docs/systematic-modernization.md)** (This document outlines component prioritization and tracking for modernization efforts, though detailed "how-to" is in the C17 Migration Guide).
3.  **(Conceptual) Migration Tracker:** The idea of `tools/migration-tracker.py` (design to be documented in `docs/scripts_and_automation/`) would help identify areas needing modernization.

### "I want to understand Plan-H's security approach."
*   **Primary Document:** **[Security Model](./docs/architecture/security-model.md)**
    *   This details our threat model, security principles, and validation procedures.

### "I want to know how Plan-H performance is managed."
*   **Primary Document:** **[Performance Baselines and Regression Strategy](./docs/architecture/performance-baselines.md)**
    *   This explains our reference workloads, baselines, and how we detect performance regressions.

### "I want to understand how decisions are made and how the project is governed."
*   **ARB Charter:** **[Plan-H ARB Charter](./docs/governance/arb-charter.md)** (describes the Architecture Review Board).
*   **RFC Process:** **[Plan-H RFC Process](./docs/governance/rfc-process.md)** (how to propose significant changes).
*   **Decision Log:** **[ARB Decision Log](./docs/governance/decision-log.md)** (record of ARB decisions).

### "Something is deprecated. What do I do?"
*   **Primary Document:** **[Deprecation Procedures](./docs/operations/deprecation-procedures.md)**
    *   This explains how Plan-H handles deprecation, including timelines and migration support.

---

This guide is a living document. If you find pathways that are unclear or missing, please let us know by opening an issue or suggesting improvements!
