# Harvey OS Request for Comments (RFC) Process

This document describes the Request for Comments (RFC) process used by Harvey OS for proposing and discussing significant changes to the system's architecture, core features, governance, or operational procedures. The RFC process aims to ensure transparency, gather community and expert feedback, and maintain a clear record of decision-making.

## 1. When to Submit an RFC

An RFC is required for (but not limited to):
*   Significant changes to the `ARCHITECTURAL_MANIFESTO.md`.
*   Proposing new architectures or changing an architecture's tier.
*   Deprecating or removing existing features, APIs, or architectures.
*   Introducing new core libraries or subsystems.
*   Fundamental changes to the build system, CI/CD infrastructure, or release engineering.
*   Changes to governance documents (including this RFC process or the ARB Charter).
*   Adoption of new core technologies or programming languages.

For minor changes, bug fixes, or routine feature enhancements within an existing architectural framework, an RFC is typically not required, but using the issue tracker for discussion is still encouraged. If in doubt, consult with an ARB member or propose an RFC.

## 2. RFC Lifecycle

1.  **Idea & Draft (Pre-RFC Discussion - Optional but Recommended)**
    *   Before writing a full RFC, authors are encouraged to discuss their idea on relevant Harvey OS communication channels (e.g., mailing list, developer forum) to gauge initial interest and gather early feedback. This can help refine the proposal.
2.  **Submission (Pull Request)**
    *   The RFC author drafts the RFC using the template below.
    *   RFCs are submitted as a pull request (PR) to the Harvey OS repository, adding a new file typically in a `docs/rfc/YYYY-MM-DD-short-title.md` format or a numbered sequence like `docs/rfc/NNNN-short-title.md`.
    *   The PR description should briefly summarize the RFC.
3.  **Review Period (Community & ARB)**
    *   Once submitted, the RFC enters a review period, typically lasting 14-30 calendar days (the specific duration may be suggested by the author and confirmed by the ARB based on complexity).
    *   Community members and ARB members review the RFC and provide feedback via comments on the PR.
    *   The RFC author is expected to engage with feedback, answer questions, and make revisions to the RFC PR as appropriate.
4.  **ARB Deliberation & Decision**
    *   After the review period, the ARB will formally discuss the RFC (and its revisions) during its meetings.
    *   The ARB will vote on the RFC according to the procedures in the ARB Charter (`docs/governance/arb-charter.md`).
    *   Possible decisions:
        *   **Accept:** The RFC is approved.
        *   **Reject:** The RFC is not approved.
        *   **Defer:** The RFC requires further work or information before a decision can be made. The ARB will provide specific feedback.
        *   **Accept with Conditions:** The RFC is approved pending certain modifications.
5.  **Finalization & Logging**
    *   The outcome of the ARB vote is recorded in the PR comments and in the ARB Decision Log (`docs/governance/decision-log.md`).
    *   If Accepted, the RFC PR is merged. The RFC document itself is now considered active or informational (depending on its nature).
    *   If Rejected or Deferred with substantial feedback, the PR is typically closed. Authors may choose to submit a revised RFC in the future.

## 3. RFC Template

RFCs should generally follow this structure:

```markdown
# RFC: [NNNN | YYYY-MM-DD] - [Concise Title of RFC]

*   **Author(s):** [Your Name(s)/Handle(s)]
*   **Status:** [Draft | Proposed | Accepted | Rejected | Deferred | Superseded] (ARB updates this)
*   **Date:** [YYYY-MM-DD of submission]
*   **Supersedes:** [RFC-XXXX if applicable]
*   **Superseded by:** [RFC-YYYY if applicable]
*   **Related Issues/PRs:** [Links to relevant discussions]

## 1. Summary

A brief (1-2 paragraph) explanation of the proposed change and the problem it solves or improvement it brings.

## 2. Motivation

Describe the reasons for the proposed change. What are the pain points with the current situation? What are the benefits of this proposal? Include specific use cases or scenarios if applicable.

## 3. Detailed Design / Proposal

This is the core of the RFC. Describe the proposed changes in detail. This may include:
*   API changes (with code examples if applicable).
*   Architectural diagrams.
*   Changes to data structures or file formats.
*   Impact on existing components or users.
*   Algorithm descriptions.

## 4. Rationale and Alternatives Considered

*   Explain why this particular design was chosen.
*   Discuss alternative designs that were considered and why they were not chosen. What are the trade-offs?

## 5. Impact Assessment

*   **Security Implications:** What are the security considerations of this change? Does it introduce new risks or mitigate existing ones?
*   **Performance Implications:** How might this change affect system performance? Are benchmarks needed?
*   **Backwards Compatibility:** Is this change backward compatible? If not, what is the migration path?
*   **Documentation Impact:** What documentation will need to be updated or created?
*   **Testing Impact:** What new tests will be needed to validate this change?
*   **Community/User Impact:** How will this affect users and contributors?

## 6. Implementation Plan (Optional but Recommended)

*   If known, outline the proposed steps to implement this RFC.
*   Are there any dependencies?
*   Who might be involved in the implementation?

## 7. Unresolved Questions / Open Issues

List any aspects of the proposal that are still under discussion or require further research.

## 8. Acknowledgements (Optional)

Thank anyone who contributed significantly to the drafting or discussion of this RFC.
```

## 4. RFC Statuses

*   **Draft:** The RFC is actively being worked on by the author, often before formal PR submission.
*   **Proposed:** The RFC has been submitted as a PR and is under active review.
*   **Accepted:** The RFC has been approved by the ARB. Implementation can proceed.
*   **Rejected:** The RFC has been rejected by the ARB.
*   **Deferred:** The ARB has postponed a decision, usually pending more information or revisions.
*   **Withdrawn:** The author has withdrawn the RFC.
*   **Superseded:** A subsequent RFC has replaced this one.
*   **Implemented:** The changes described in an Accepted RFC have been implemented.

The ARB or designated editors will update the status field in the RFC document header.
