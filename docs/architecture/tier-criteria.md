---
title: Plan-H Architecture Tier Criteria
category: architecture_specification
audience: developers, contributors, arb_members
status: active
last_updated: YYYY-MM-DD
---
# Plan-H Architecture Tier Criteria

This document defines the criteria for assigning Plan-H supported architectures to specific tiers, the process for evaluating and adding new architectures, and the lifecycle management for architectures within these tiers. This process is overseen by the Architecture Review Board (ARB).

## 1. Tier Definitions and Philosophy

Plan-H utilizes a tiered system to manage resources effectively, set clear expectations for support and stability, and guide strategic focus.

*   **Tier 1 (Core Architectures):** These are architectures critical to Plan-H's primary target use cases and strategic goals. They receive the highest level of support, testing, and performance optimization.
*   **Tier 2 (Community Supported Architectures):** These architectures are valuable to the Plan-H ecosystem, often supporting broader adoption, research, or specific community interests. They are primarily maintained by the community with infrastructure support from the core team.
*   **Tier 3 (Experimental/Legacy Architectures):** These architectures are maintained for specific research purposes, historical reference, or are in the process of being phased out. Support is minimal and primarily for build validation.

## 2. Architecture Tier Criteria Matrix

The ARB uses the following matrix to guide tier assignments. Meeting these criteria is indicative, and the ARB makes final decisions based on overall strategic alignment.

| Criteria                     | Tier 1 (Core)                                         | Tier 2 (Community)                                    | Tier 3 (Experimental/Legacy)        |
|------------------------------|-------------------------------------------------------|-------------------------------------------------------|-------------------------------------|
| **Strategic Alignment**      | Essential for ≥1 defined Target Use Cases             | Supports broader ecosystem, research, or niche uses   | Historical, experimental, declining |
| **Stability & Maturity**     | Production-ready; documented high stability (e.g., MTBF targets met) | Generally stable; may have non-critical known issues  | Experimental; known issues expected |
| **CI Test Coverage**         | 100% core functional tests; All performance baselines pass | ≥75% core functional tests; Build validation essential | Build validation only               |
| **Build Time (CI, Full)**    | < 20 minutes (target for core system)                 | < 40 minutes (target for core system)                 | Best effort                         |
| **Dedicated Maintainer(s)**  | ≥1 Core Team Maintainer(s) with <48hr critical issue SLA | ≥1 Community Maintainer(s) with best-effort SLA       | Optional/Historical                 |
| **Performance**              | Meets/Exceeds all quantitative criteria for relevant Target Use Cases | Acceptable for general use; may not meet all Tier 1 performance marks | Not a primary concern               |
| **Adoption/Demand Indicator**| Documented use in strategic projects or by >N active community users/developers (N to be set by ARB, e.g., 50) | Clear evidence of active community use or strong interest for specific applications | Low or primarily historical interest|
| **Documentation**            | Comprehensive: install, config, usage, known issues   | Essential: install, config; Community-supported docs  | Minimal or archival                 |
| **Toolchain Support**        | Modern, actively maintained toolchain readily available | Reasonably available toolchain                        | May require legacy toolchains       |

## 3. Tier Promotion and Demotion Process

*   **Promotion (e.g., Tier 3 → Tier 2, Tier 2 → Tier 1):**
    1.  **Nomination:** A maintainer or community member submits an RFC to the ARB, providing evidence that the architecture meets the criteria for the higher tier for a sustained period (e.g., 6-12 months).
    2.  **ARB Review:** The ARB evaluates the RFC, considering current strategic goals and resource availability.
    3.  **Decision:** Documented in the ARB's decision log. Promotion may require commitment of additional resources (e.g., CI enhancements, core maintainer time for Tier 1).
*   **Demotion (e.g., Tier 1 → Tier 2, Tier 2 → Tier 3):**
    1.  **Review Trigger:** An architecture consistently fails to meet its tier criteria (e.g., lack of maintainer, failing tests, obsolescence).
    2.  **ARB Evaluation:** The ARB assesses the situation and may issue a warning or grace period.
    3.  **Decision:** If issues are not rectified, the ARB may decide to demote the architecture. This decision is communicated publicly with rationale.

## 4. Process for Adding a New Architecture

1.  **Proposal (RFC):** An individual or group submits an RFC to the ARB. The RFC must include:
    *   The specific architecture and its variants.
    *   Rationale for inclusion: alignment with Plan-H vision, potential use cases, community interest.
    *   Proposed initial tier (usually Tier 3).
    *   Availability of hardware for development and testing.
    *   Identification of at least one prospective maintainer.
    *   Initial porting efforts, challenges, and estimated work.
2.  **ARB Review (Decision Tree - Conceptual):**
    *   Is there a clear benefit to Plan-H?
    *   Are resources (hardware, maintainers) available or likely?
    *   Is the toolchain mature and accessible?
    *   What is the estimated impact on CI and core team resources?
    *   Decision: Approve (assign to Tier 3), Defer (request more info/maturity), or Reject (with rationale).
3.  **Initial Integration (Tier 3):**
    *   Basic build system integration.
    *   Minimal CI loop (build validation).
    *   Core Plan-H components ported sufficiently to boot and run basic tests.
4.  **Ongoing Development:** Primarily community-led, aiming for promotion criteria.

## 5. Automated Tier Assessment Tooling (Conceptual)

To aid the ARB and provide transparency, the project aims to develop tools that can:
*   Track CI test coverage and build times per architecture.
*   Monitor maintainer activity (e.g., from VCS/issue tracker).
*   Report on architectures failing to meet their tier's technical criteria (e.g., test pass rates).
This data will inform ARB reviews but not replace human judgment.
