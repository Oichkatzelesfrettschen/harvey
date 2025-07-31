---
title: Plan-H Installation Guide - Overview
category: installation
audience: new_users, developers
status: active
last_updated: YYYY-MM-DD
---
# Plan-H Installation Guide

Welcome to the Plan-H Installation Guide! This multi-step guide provides detailed instructions to set up a complete Plan-H development environment, build the operating system for various architectures, and run it in an emulator.

This process is designed to be robust and educational, aligning with Plan-H's principles of transparency and environmental resilience, as detailed in our **[Architectural Manifesto](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)**.

## Overview

This 10-step guide is structured to progressively build your Plan-H environment:
*   **Steps 1-4:** Focus on preparing your host system, installing all necessary tools and dependencies using our canonical setup script, verifying the setup, and activating the Plan-H-specific shell environment.
*   **Steps 5-6:** Cover obtaining the Plan-H source code and performing your first native build.
*   **Steps 7-9:** Guide you through running your native build in QEMU, then cross-compiling Plan-H for a different architecture (e.g., ARM64), and running that cross-compiled version in QEMU.
*   **Step 10:** Provides comprehensive validation checks for your setup and directs you on your next steps within the Plan-H ecosystem, including how to start contributing.

## Prerequisites

Before starting, please ensure you meet the general prerequisites outlined in the first step of this guide:
*   **[Step 1: Prepare Host System & Obtain Setup Script](01_prepare_host_and_setup_script.md)**
    *   Primarily: Ubuntu 24.04 (or compatible Linux), `sudo` access, internet, ~30-50GB free disk space, and ~4-8GB RAM.

## Guide Structure & Navigation

Below is a list of all steps in this guide. We recommend following them in order, especially if this is your first time setting up Plan-H.

| Step | Document                                                                 | Estimated Time | Complexity | Focus                                           |
|------|--------------------------------------------------------------------------|----------------|------------|-------------------------------------------------|
| 1    | [Prepare Host & Get Script](01_prepare_host_and_setup_script.md)         | 10-20 mins     | Low        | System checks, download setup script            |
| 2    | [Execute Setup Script](02_execute_setup_script.md)                       | 30-90 mins+    | Medium     | Automated toolchain & dependency installation   |
| 3    | [Verify Environment](03_verify_dev_environment.md)                       | 10-15 mins     | Low        | Check script output, verify key tools           |
| 4    | [Activate Plan-H Env](04_activate_harvey_env.md)                         | 5 mins         | Low        | Source `plan-h-env.sh`, understand helpers      |
| 5    | [Clone Source Code](05_clone_harvey_source.md)                           | 5-15 mins      | Low        | `git clone` Plan-H repository                   |
| 6    | [Perform Native Build](06_perform_native_build.md)                       | 20-60 mins+    | Medium     | Compile OS for your machine's architecture      |
| 7    | [Run Native Build in QEMU](07_run_native_qemu.md)                        | 10-20 mins     | Medium     | Boot and interact with native Plan-H in QEMU   |
| 8    | [Cross-Compile Target Arch](08_cross_compile_target.md)                  | 20-60 mins+    | High       | Build OS for a different architecture (e.g., ARM64) |
| 9    | [Run Cross-Compiled in QEMU](09_run_cross_compiled_qemu.md)              | 10-20 mins     | High       | Boot and interact with cross-compiled Plan-H |
| 10   | [Validation & Next Steps](10_basic_validation_next_steps.md)             | 20-30 mins     | Medium     | Final checks, ecosystem navigation, contribution  |

*(Estimated times are approximate and can vary based on system performance and internet speed.)*

## Quick Navigation & Common Tasks

*   **Just starting?** Begin with **[Step 1: Prepare Host System & Obtain Setup Script](01_prepare_host_and_setup_script.md)**.
*   **Setup script failed?** See troubleshooting in **[Step 2: Execute Setup Script](02_execute_setup_script.md)** and general troubleshooting tips in each relevant document.
*   **Ready to contribute after setup?** Jump to **[Step 10: Validation & Next Steps](10_basic_validation_next_steps.md#6-your-next-steps-in-the-plan-h-ecosystem)** for guidance (Note: section number updated due to Plan-H context).
*   **Want to understand Plan-H's vision first?** Read the **[Architectural Manifesto](../../ARCHITECTURAL_MANIFESTO.md)**.

We hope this guide provides a smooth and informative experience for setting up your Plan-H development environment!
