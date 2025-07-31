---
title: Plan-H Contribution Guide
category: community_guidelines
audience: contributors, developers
status: active
last_updated: YYYY-MM-DD
---
# Plan-H Contribution Guide

Welcome to the Plan-H project! We appreciate your interest in contributing. This guide outlines how you can contribute to Plan-H, our coding standards, testing requirements, and how community feedback is handled.

For project governance, RFC process, and architectural principles, please refer to documents in `docs/governance/` and the `ARCHITECTURAL_MANIFESTO.md`.

## 1. Ways to Contribute

There are many ways to contribute to Plan-H, including:

*   **Code Contributions:** Developing new features, fixing bugs, porting Plan-H to new architectures, writing device drivers, or improving existing code.
*   **Documentation:** Writing new documentation, improving existing guides, creating tutorials, or translating documentation.
*   **Testing:** Helping to test new releases, verifying bug fixes, writing new test cases (unit, integration, performance).
*   **Bug Reporting:** Submitting well-documented bug reports.
*   **Feature Requests:** Proposing new features (ideally via discussion leading to an RFC for significant features).
*   **Community Support:** Helping other users on mailing lists, forums, or chat channels.
*   **Maintaining a Tier 2 Architecture:** See the `docs/community/tier2-maintainer-handbook.md`.

## 2. Getting Started

1.  **Familiarize Yourself:** Read the `ARCHITECTURAL_MANIFESTO.md` and relevant documents in `docs/` to understand the project's vision and structure.
2.  **Set up Development Environment:** Follow instructions in `BUILD_INSTRUCTIONS.md` (or equivalent, ideally the new [Installation Guide](../installation_guide/README.md)).
3.  **Find Something to Work On:**
    *   Look for issues tagged "good first issue" or "help wanted" in the project's issue tracker.
    *   Pick a component that interests you and look for open bugs or areas for improvement.
    *   Propose a new feature or improvement (start with a discussion, then potentially an RFC).
4.  **Communicate:** Join the Plan-H mailing list or developer forum to discuss your plans and ask questions.

## 3. Code Contribution Guidelines

### 3.1. Code Style and Formatting
*   Plan-H C code generally follows the Plan 9 C style. Key aspects include:
    *   Tabs for indentation (set to 4 spaces).
    *   Specific bracing style (K&R variant).
    *   Function and variable naming conventions.
*   Refer to the `.clang-format` file in the root of the repository for automated formatting rules. All C/C++ code **must** be formatted with `clang-format` using this file before submission.
*   For new code, adhere to C17 standards as per `docs/operations/c17-migration-guide.md`.

### 3.2. Commit Messages
*   Follow standard Git commit message guidelines:
    *   Short (<=50 char) summary line.
    *   Longer explanatory text after a blank line, if needed.
    *   Explain *what* and *why*, not just *how*.
    *   Reference relevant issue numbers (e.g., "Fixes #123").

### 3.3. Pull Requests (PRs)
*   Submit PRs against the `main` branch (or designated development branch).
*   Ensure your PR is focused on a single issue or feature.
*   Provide a clear description of the changes in the PR.
*   Ensure your code builds cleanly and passes all relevant CI checks (linters, tests).
*   PRs will be reviewed by maintainers. Be prepared to discuss your changes and make revisions.

### 3.4. Testing Requirements
*   **Unit Tests:** New code that implements testable logic (e.g., library functions, complex algorithms) should be accompanied by unit tests.
*   **Integration Tests:** Changes that affect interactions between components may require new or updated integration tests.
*   **Manual Testing:** Clearly document any manual testing steps performed if automated tests are not feasible for a particular change.
*   All tests must pass before a PR can be merged. Refer to `docs/testing-philosophy.md` (if it exists) or current best practices within the project for more details.

### 3.5. Licensing
*   All contributions must be compatible with the Plan-H project license (typically MIT or a similar permissive license - verify this in the main `LICENSE` file).
*   For substantial contributions, you may be asked to affirm a Developer Certificate of Origin (DCO) or sign a Contributor License Agreement (CLA) if the project adopts one.

## 4. Documentation Contributions

*   Documentation should be clear, concise, and accurate.
*   Follow existing documentation style and structure.
*   For significant new documents or changes to existing ones, a PR is also required.

## 5. Community Feedback and Issue Resolution

*   **Bug Reports:** Use the project's issue tracker. Provide detailed steps to reproduce, expected behavior, actual behavior, and system information.
*   **Feature Requests:** Discuss on community channels first. For significant features, an RFC will be required (see `docs/governance/rfc-process.md`).
*   **Code of Conduct:** All contributors are expected to adhere to the project's Code of Conduct (link to it if one exists, or state that respectful interaction is expected). Disrespectful or harassing behavior will not be tolerated.

## 6. Recognition and Incentive Structures (Conceptual)

Plan-H values its contributors. While direct monetary rewards are not standard for open-source contributions, we aim to recognize effort through:
*   **Public Acknowledgement:** Regular shout-outs in release notes, project blog, or community meetings for significant contributions.
*   **Contributor Roles:** Potential for active and trusted contributors to gain more responsibilities (e.g., becoming a maintainer for a component or Tier 2 architecture).
*   **Authorship:** Clear authorship maintained in commit history and documentation.
*   **Swag/Merchandise:** (Future possibility) Occasional distribution of project-related merchandise to key contributors.
*   **Letters of Recommendation/Reference:** For student contributors or those seeking professional advancement, core maintainers may provide references based on quality and consistency of contributions.

The ARB may develop more formal recognition programs as the community grows.

Thank you for contributing to Plan-H!
