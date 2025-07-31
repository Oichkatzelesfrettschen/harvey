# Harvey OS Tier 2 Architecture Maintainer Handbook

This handbook is for individuals who volunteer to maintain a Tier 2 architecture for Harvey OS. Tier 2 architectures are valuable to the Harvey OS ecosystem, often supporting broader adoption, research, or specific community interests. They are primarily maintained by the community with infrastructure support from the core team.

Refer to `docs/architecture/tier-criteria.md` for the definition and requirements of Tier 2 architectures.

## 1. Role and Responsibilities

As a Tier 2 Architecture Maintainer, your primary responsibilities include:

*   **Build Health:**
    *   Monitoring the build status for your architecture in the CI system.
    *   Addressing build breakages promptly.
    *   Keeping the architecture's build scripts and toolchain configurations up-to-date with core Harvey OS changes.
*   **Testing:**
    *   Ensuring that core functional tests (as defined for Tier 2 in `tier-criteria.md`) pass on your architecture.
    *   Running additional architecture-specific tests if applicable.
    *   Reporting and helping to debug test failures.
*   **Issue Management:**
    *   Triaging and responding to bugs reported specifically for your architecture.
    *   Porting fixes for general Harvey OS bugs to your architecture if architecture-specific code is involved.
*   **Documentation:**
    *   Maintaining documentation specific to building, installing, and using Harvey OS on your architecture.
    *   Ensuring any known quirks or limitations are documented.
*   **Communication:**
    *   Acting as the primary point of contact for your architecture.
    *   Communicating significant issues or changes related to your architecture to the core Harvey OS team and the wider community.
    *   Participating in discussions related to your architecture.
*   **Advocacy (Optional but Encouraged):**
    *   Promoting Harvey OS on your architecture.
    *   Helping users get started with Harvey OS on that platform.

## 2. Support from Core Harvey OS Team

The core Harvey OS team will provide the following support for Tier 2 maintainers:

*   **CI Infrastructure:** Access to CI resources for building and testing your architecture (within reasonable limits defined by ARB).
*   **Technical Guidance:** Advice and support from core developers on Harvey OS internals and porting challenges.
*   **Code Review:** Core developers will assist with reviewing PRs related to your architecture, especially those affecting core components.
*   **Communication Channels:** Inclusion in relevant developer communication channels.

## 3. Best Practices for Tier 2 Maintenance

*   **Stay Updated:** Regularly sync your architecture's port with the Harvey OS `main` branch to avoid large, complex merges.
*   **Test Thoroughly:** Before submitting PRs, test changes extensively on your target hardware.
*   **Automate:** Script build processes, tests, and common maintenance tasks where possible.
*   **Document:** Keep your architecture-specific documentation current. This helps users and potential future co-maintainers.
*   **Collaborate:** Engage with other Harvey OS developers and users of your architecture. If possible, find co-maintainers to share the workload.
*   **Communicate Proactively:** If you foresee issues (e.g., toolchain becoming obsolete, lack of time to maintain), inform the ARB and community in advance.

## 4. Stepping Down or Transitioning Maintenance

If you are no longer able to maintain a Tier 2 architecture:
1.  Notify the ARB and the Harvey OS community as early as possible.
2.  Assist in finding and mentoring a new maintainer if possible.
3.  Ensure any architecture-specific knowledge or resources are documented and handed over.

If no new maintainer is found and the architecture fails to meet Tier 2 criteria for an extended period, the ARB may re-classify it to Tier 3 or, in rare cases, deprecate it entirely, following the procedures in `docs/operations/deprecation-procedures.md`.

Thank you for your dedication to supporting Harvey OS on diverse architectures! Your work is vital to the project's reach and resilience.
