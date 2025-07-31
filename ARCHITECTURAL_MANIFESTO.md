---
title: Plan-H Architectural Manifesto v2.0
category: core_architecture
audience: all
status: active
last_updated: YYYY-MM-DD
version: 2.0.0
# Version from docs/project/documentation_versioning.md
---
# Plan-H Architectural Manifesto (v2.0.0)

## 1. Preamble: Addressing the Past, Defining the Future

*   Acknowledge the findings of the "COMPREHENSIVE ARCHITECTURAL REVIEW."
*   State the purpose of this manifesto: to provide a clear, actionable architectural vision for Plan-H.
*   Briefly reiterate the commitment to excellence and proactive architectural management.

## 2. Core Identity: What is Plan-H in 2025 and Beyond?

*   **Declaration:** Plan-H is a lean, modular, and portable operating environment, inspired by the elegance and simplicity of Plan 9, but designed for contemporary computing landscapes and specialized applications. It is not a museum piece but a living evolution.
*   **Key Principles:** Minimalism, explicitness, portability, security through simplicity, and POSIX compatibility where pragmatic.

### 2.1 Target Use Cases & Success Criteria

Plan-H is engineered for environments where simplicity, transparency, control, and reliability are paramount. Our development and architectural decisions are guided by the following primary use cases. These use cases define the performance and feature targets that drive our engineering efforts.

**1. Use Case: Secure Embedded IoT Gateway**
*   **Name:** Secure Embedded IoT Gateway
*   **Description:** A robust device deployed in remote or industrial settings, responsible for aggregating data from local sensors/devices, performing initial processing/filtering, and securely transmitting data to upstream systems. Emphasizes reliability, low maintenance, and a minimal attack surface.
*   **Hardware Profile:**
    *   CPU: ARM Cortex-A53/A55 class (e.g., Raspberry Pi 3/4, NXP i.MX8M Mini) or equivalent RISC-V (e.g., SiFive U74 class), 1.0-1.5GHz, 2-4 cores.
    *   Memory: 512MB - 1GB RAM.
    *   Storage: 8GB - 16GB eMMC/SD card (high endurance).
    *   Networking: Gigabit Ethernet, optional Wi-Fi/Cellular.
*   **Performance Metrics & Success Criteria:**
    *   Boot Time (Power-on to operational data forwarding): < 7 seconds.
    *   Core System Memory Footprint (OS + essential services, idle): < 80MB.
    *   Latency p99 (sensor data receipt to upstream transmission): < 100ms (excluding network link latency).
    *   Secure Data Throughput (TLS to server): > 100 Mbps on wired Ethernet.
    *   Stability (MTBF for core OS): > 18 months.
*   **Validation Methodology:** Automated test suite simulating sensor inputs, network loads, and configuration changes. Long-duration stability tests on reference hardware.

**2. Use Case: Network Security Appliance**
*   **Name:** Dedicated Network Security Appliance (e.g., Firewall, IDS Sensor)
*   **Description:** A specialized device providing network traffic inspection, filtering, and routing capabilities. Focuses on high throughput, low-latency processing of network packets, and strong security isolation.
*   **Hardware Profile:**
    *   CPU: x86_64 server-class (e.g., Intel Xeon-D, AMD EPYC Embedded) or high-end ARM64 (e.g., Marvell Octeon), 2.0GHz+, 4-8 cores.
    *   Memory: 4GB - 16GB RAM (ECC recommended).
    *   Storage: 64GB+ SSD/NVMe for OS, logs, and rule sets.
    *   Networking: Multiple 1/10 Gbps Ethernet ports (Intel/Chelsio).
*   **Performance Metrics & Success Criteria:**
    *   Packet Forwarding Rate (64-byte packets, L3 forwarding): > 2 Mpps per core.
    *   Firewall Rule Capacity: > 50,000 active rules with < 5% performance degradation.
    *   Latency p99 (packet ingress to egress for allowed traffic): < 50 microseconds.
    *   Configuration Update Time (atomic application of large ruleset): < 0.5 seconds.
*   **Validation Methodology:** Standard network performance test tools (e.g., iperf, dpdk-pktgen), custom test suite for firewall rule validation and stateful inspection.

**3. Use Case: OS Research & Education Platform**
*   **Name:** OS Research and Education Platform
*   **Description:** An accessible and modifiable environment for teaching operating systems concepts and conducting research in areas like distributed systems, kernel architecture, and novel hardware interfaces. Prioritizes ease of understanding, modification, and debugging.
*   **Hardware Profile:**
    *   CPU: Common x86_64 desktop/server CPUs, ARM64 (e.g., Raspberry Pi 4/5, popular SBCs), and QEMU emulation for various architectures.
    *   Memory: 1GB+ RAM.
    *   Storage: Any standard disk/SSD.
*   **Performance Metrics & Success Criteria:**
    *   Kernel Compile Time (full debug build from clean, reference config): < 3 minutes on reference developer machine.
    *   Time to Interactive Debug Session (from boot to kernel debugger attachment): < 1 minute.
    *   Documentation Clarity: Undergraduate student with OS basics can build, boot, and modify a kernel module within 1 day using provided guides.
*   **Validation Methodology:** Community feedback, student projects, successful replication of research paper setups. Build and boot tests across supported hardware/emulators.

## 3. Multi-architecture Philosophy

*   **Rationale:** Multi-architecture support is strategically important for Plan-H, ensuring flexibility, enabling wider adoption, and providing resilience against single-architecture obsolescence.
*   **Tiered Support Model:** Architectures are categorized into tiers to reflect their level of support and strategic importance.
    *   **Tier 1 (Core Architectures):** Fully supported, rigorously tested, performance-optimized (e.g., x86_64, arm64). These are critical for Plan-H's primary use cases.
    *   **Tier 2 (Community Supported):** Build support provided, maintained by the community, may have performance variations (e.g., RISC-V, other relevant modern ISAs).
    *   **Tier 3 (Experimental/Legacy):** Maintained for specific purposes or transitioning out (e.g., i386).
The detailed criteria for tier assignment, the process for adding new architectures, and lifecycle management within these tiers are defined in the "[Plan-H Architecture Tier Criteria](docs/architecture/tier-criteria.md)" document.
*   **Binary Distribution Strategy:**
    *   Primarily per-architecture binary packages.
    *   Fat binaries considered only if a strong technical case exists for specific architectures and use cases, with performance impact analysis required.
*   **Runtime vs. Compile-Time Optimization:**
    *   Favor compile-time optimization for performance and simplicity where possible.
    *   Runtime detection for features that vary within an architecture (e.g., CPU features) but not as a primary means of architecture selection.

## 4. Modernization Principles

*   **C17 Adoption:** Systematic migration from older C dialects to C17 for core components. New C code adheres strictly to the C17 standard. The overall process for C17 adoption, including component prioritization, is managed according to the [Systematic Modernization Protocol](docs/systematic-modernization.md). Detailed code-level guidance, common migration patterns from Plan 9 C, and specific examples are provided in the **[Plan-H C17 Migration Guide](docs/operations/c17-migration-guide.md)**. The API compatibility matrix detailing the status of Plan 9 APIs (retained, wrapped, or deprecated) and their POSIX counterparts is maintained in `docs/systematic-modernization.md`.
*   **Plan 9 Ethos Meets Modern Practices:**
    *   Retain core Plan 9 concepts (e.g., everything as a file, namespaces) where they provide clear advantages.
    *   Integrate modern development practices (robust build systems, CI/CD, comprehensive testing, security best practices).
*   **POSIX Compatibility:** Strive for a practical level of POSIX compatibility to ease porting of software and improve interoperability, without sacrificing Plan-H's core principles. This is guided by the POSIX conformance test suite strategy.

## 5. Performance Engineering Framework

Plan-H is committed to achieving and maintaining high performance aligned with its target use cases, as defined by the Quantitative Success Criteria in Section 2.1. Our Performance Engineering Framework encompasses establishing clear performance targets, utilizing reference workloads, and implementing automated regression detection.

The detailed specifications for our reference workloads, the process for establishing performance baselines, the mechanics of our automated regression detection framework, and plans for a performance dashboard are documented in the **[Plan-H Performance Baselines and Regression Strategy](docs/architecture/performance-baselines.md)**. This document provides the operational details for achieving the performance metrics derived from our Target Use Cases.

Key aspects of this framework include:
*   **Baseline Establishment:** Performance baselines for benchmark applications and metrics are established for each Tier 1 architecture, using the reference workloads and hardware specified in `docs/architecture/performance-baselines.md`.
*   **Regression Criteria:** Clear thresholds for acceptable performance regressions are defined within the performance baselines document. Regressions exceeding these thresholds must be justified or rectified.
*   **Performance Monitoring & Automation:** Performance testing is integrated into the CI pipeline for Tier 1 architectures. The CI system automatically executes reference workloads, compares results against baselines, and reports regressions, as detailed in both `docs/ci-strategy.md` and `docs/architecture/performance-baselines.md`.

### 5.1. Reference Workload Definition (Summary)

Performance is measured against standardized reference workloads designed to simulate key aspects of our target use cases. The full specification, definition format, and acceptance criteria for these workloads are maintained in **[Plan-H Performance Baselines and Regression Strategy](docs/architecture/performance-baselines.md)**. The `docs/ci-strategy.md` details how these workloads are executed and results are processed within the CI environment.

*Conceptual Example Snippet (details in `docs/architecture/performance-baselines.md`):*
```yaml
# Example from docs/architecture/performance-baselines.md
harvey_reference_workloads:
  - name: "iot_gateway_data_forwarding"
    # ... operations ...
    acceptance_criteria:
      arm_cortex_a53: # Matches target hardware profile
        throughput_mbps: 500
        latency_ms_avg: < 50
  - name: "firewall_rule_processing"
    # ... operations ...
    acceptance_criteria:
      x86_64_2core:
        packets_per_sec: 2000000 # 1Mpps per core
```

## 6. Deprecation Strategy

Plan-H employs a formal, transparent process for deprecating and removing components, APIs, features, or architectures that are obsolete, redundant, insecure, or no longer aligned with our strategic goals. This ensures the OS remains lean, secure, and maintainable.

Our deprecation philosophy emphasizes clear communication, phased roll-out, and support for migration where feasible. The complete lifecycle, including proposal, review, announcement, migration support (with conceptual tooling for dependency analysis and guide generation), and removal/archival, is detailed in the **[Plan-H Deprecation Procedures](docs/operations/deprecation-procedures.md)**.

A specific, ongoing example is the deprecation of the legacy `386/` directory and its associated components, which has a defined timeline for archival as noted in its deprecation notice and tracked by the ARB. This specific deprecation follows the general procedures outlined in the linked document.

## 7. Environmental Resilience and Reproducible Builds

To ensure build consistency, guard against environmental variances (such as the "temporal anomaly" previously experienced), and promote long-term maintainability, Plan-H mandates a strong environmental resilience strategy. This strategy is crucial for developers, CI systems, and users who need to build or reproduce Plan-H components reliably.

Key pillars of this strategy include:

*   **Containerized Build Environments:**
    *   All official builds and Continuous Integration (CI) processes **must** utilize containerized environments (e.g., Docker). This isolates builds from the host system's configuration and ensures a consistent environment.
    *   Detailed specifications for these build containers, including base operating system versions, explicitly versioned toolchains (e.g., GCC 13.x, Clang 17.x), and all necessary build dependencies, are defined and maintained. The primary reference for these environments will be Dockerfiles stored within the Plan-H project (e.g., in a `build/docker/` directory) and further detailed in the `docs/ci-strategy.md`.

*   **Temporal Independence via Fixed Package Versions:**
    *   Build containers **must** use fixed, explicitly defined versions for all system packages, libraries, and toolchains. This mitigates issues arising from upstream package updates altering the build environment unexpectedly.
    *   This is achieved through:
        *   Explicit version pinning in container definitions (e.g., `FROM ubuntu:24.04`, `RUN apt-get install -y gcc-13=13.x.x ...`).
        *   Use of package manager lockfiles or equivalent mechanisms where the base container OS supports them (e.g., generating a `plan-h-packages.lock` from a known-good state and using it to install packages). This ensures that `apt-get update && apt-get install some-package` always yields the same version of `some-package`.

*   **Offline-Capable Toolchain and Dependency Repository (Strategic Goal):**
    *   To further reduce reliance on external repositories and protect against their potential outages or changes, Plan-H will work towards establishing an internal mirror for critical toolchains and dependencies.
    *   **Design Concept:**
        *   This mirror would host specific, vetted versions of compilers, build tools, and source libraries.
        *   Build containers would be configured to prioritize this internal mirror.
        *   A process for updating the mirror with new, vetted toolchain/dependency versions will be established by the ARB.
    *   The detailed plan and implementation for this repository will be part of the ongoing infrastructure development, guided by `docs/ci-strategy.md`.

*   **Time-Independent Build Processes:**
    *   Build scripts, Makefiles, and other automation **must avoid** dependencies on the specific time of day or date on the build host, where feasible.
    *   Generated artifacts should, where possible, not embed build timestamps in ways that cause non-deterministic outputs for identical sources. If timestamps are necessary (e.g., for release versioning), they should be sourced from a controlled, deterministic input (like a VCS tag).

This multi-faceted approach ensures that Plan-H builds are reproducible today and in the future, regardless of individual developer setups or changes in the wider software ecosystem.

## 8. Security Architecture

Security is a fundamental design consideration for Plan-H, not an afterthought. Our approach is built upon the principles of simplicity, minimalism, and explicit control, augmented by modern security best practices to meet the demands of contemporary computing environments and our defined target use cases.

We aim to provide a secure platform by:
*   **Minimizing the attack surface** through a lean codebase and carefully curated features.
*   Adhering to the **principle of least privilege** for all system processes.
*   Implementing **secure defaults** to protect users from common misconfigurations.
*   Employing **defense-in-depth** strategies across various system layers.

A comprehensive discussion of our security philosophy, threat model (aligned with our target use cases), core security principles, and detailed security validation procedures is available in the dedicated **[Plan-H Security Model](docs/architecture/security-model.md)** document. This document guides our development practices and architectural decisions to ensure Plan-H is robust against known and emerging threats. The Architecture Review Board (ARB) oversees the evolution of this security model.

## 9. Architectural Governance

Plan-H is committed to transparent, structured, and community-informed architectural governance. This is primarily achieved through the Architecture Review Board (ARB) and a formal Request for Comments (RFC) process.

*   **Architecture Review Board (ARB):** The ARB is the principal group responsible for guiding Plan-H's technical direction, ensuring architectural coherence, and making decisions on significant changes. Its mission, composition, responsibilities, and operational procedures are detailed in the **[Plan-H ARB Charter](docs/governance/arb-charter.md)**.
*   **Request for Comments (RFC) Process:** Significant changes to Plan-H architecture, features, or governance are proposed and discussed via RFCs. This process ensures that potential changes receive thorough review from both the ARB and the wider community before decisions are made. The complete RFC lifecycle, template, and submission guidelines are specified in the **[Plan-H RFC Process](docs/governance/rfc-process.md)**.
*   **Decision Log:** Key decisions made by the ARB are recorded in a publicly accessible **[ARB Decision Log](docs/governance/decision-log.md)**.

This governance framework is designed to be robust yet adaptable, fostering both careful consideration of changes and the ability for Plan-H to evolve effectively.

## 10. Community Engagement

Plan-H thrives on community involvement and collaboration. We encourage participation from developers, users, testers, and documenters at all levels of experience. Our goal is to foster an open, welcoming, and productive environment.

Key aspects of our community engagement include:
*   **Open Communication:** Utilizing public mailing lists, forums, and issue trackers for discussions and development.
*   **Contribution Pathways:** Providing clear guidance for various types of contributions, from code and documentation to testing and support. Detailed information for contributors can be found in the **[Plan-H Contribution Guide](docs/community/contribution-guide.md)**.
*   **Support for Tiered Architectures:** Community maintainers play a crucial role in supporting Tier 2 architectures. Resources and responsibilities for these maintainers are outlined in the **[Plan-H Tier 2 Architecture Maintainer Handbook](docs/community/tier2-maintainer-handbook.md)**.
*   **Feedback Integration:** Processes for submitting, discussing, and integrating community feedback and feature requests, often leveraging the RFC process for significant items.
*   **Recognition:** Valuing and acknowledging the efforts of our contributors.

We believe a strong community is essential for the long-term health and innovation of Plan-H.

## 11. Conclusion: A Living Document

*   This manifesto is a living document and will be reviewed and updated periodically by the ARB (at least annually) to reflect the evolving landscape and goals of Plan-H.
