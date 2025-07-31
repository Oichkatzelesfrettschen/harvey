# Harvey OS Architecture Review Board (ARB) Charter

This document defines the charter, composition, responsibilities, and operational procedures of the Harvey OS Architecture Review Board (ARB). The ARB is the primary governing body for the technical and architectural direction of Harvey OS.

Refer to `ARCHITECTURAL_MANIFESTO.md` for the high-level commitment to architectural governance and `docs/governance/rfc-process.md` for details on the Request for Comments procedure.

## 1. Mission and Scope

*   **Mission:** To ensure Harvey OS evolves in a coherent, robust, secure, and sustainable manner, aligned with its core principles and strategic goals as defined in the `ARCHITECTURAL_MANIFESTO.md`.
*   **Scope:** The ARB's authority covers:
    *   Approval of significant architectural changes.
    *   Oversight of the multi-architecture tiering strategy and new architecture additions (`docs/architecture/tier-criteria.md`).
    *   Approval of deprecations (`docs/operations/deprecation-procedures.md`).
    *   Evolution of core security model (`docs/architecture/security-model.md`) and performance baselines (`docs/architecture/performance-baselines.md`).
    *   Resolution of technical disputes that cannot be resolved at the component/module level.
    *   Maintenance and evolution of this charter and related governance documents.

## 2. Composition and Roles

### 2.1. Standard Composition
The ARB shall typically consist of 3 to 5 members, aiming for a balance of expertise and representation.
*   **Lead Architect (Chair):**
    *   **Selection:** Appointed by project sponsorship or elected by core contributors if no formal sponsorship exists.
    *   **Responsibilities:** Chairs ARB meetings, facilitates discussions, ensures adherence to process, holds tie-breaking vote or veto power (see below), champions the overall architectural vision.
*   **Core System Architect(s) (1-2 members):**
    *   **Selection:** Nominated by core contributors, confirmed by existing ARB. Must have deep expertise in Harvey OS internals (kernel, core libraries, build system).
    *   **Responsibilities:** Provide expert technical assessment of proposals, ensure designs are feasible and maintainable.
*   **Security Architect (0-1 dedicated member, or role fulfilled by a Core System Architect):**
    *   **Selection:** Nominated/confirmed by ARB. Must have expertise in system security and threat modeling.
    *   **Responsibilities:** Review all proposals for security implications, guide the evolution of the security model.
*   **Community Representative (1 member):**
    *   **Selection:** Elected by active Harvey OS contributors (defined as those with merged contributions in the last 12 months) for a 12-month term. Max 2 consecutive terms.
    *   **Responsibilities:** Represent community interests and perspectives in ARB discussions, facilitate communication between ARB and the broader community.

### 2.2. Term Limits and Rotation
*   Lead Architect: Serves until resignation or replacement by project sponsorship/core contributor consensus.
*   Core System/Security Architects: Serve for 2-year terms, renewable. Staggered terms are encouraged to ensure continuity.
*   Community Representative: 12-month term, max 2 consecutive terms.

### 2.3. Vacancies
Vacancies will be filled as soon as practical using the selection process for that role.

## 3. Operational Procedures

### 3.1. Meetings
*   **Cadence:** The ARB meets regularly (e.g., bi-weekly or monthly, determined by current workload) and ad-hoc for urgent RFCs or issues.
*   **Quorum:** A quorum of >50% of seated ARB members is required for binding decisions (e.g., 2 out of 3, 3 out of 4 or 5).
*   **Agenda:** Published at least 48 hours before regular meetings.
*   **Minutes:** Taken for all meetings and published publicly (e.g., in `docs/governance/decision-log.md` or a dedicated meeting minutes archive) within 72 hours. Minutes will record topics discussed, decisions made, and vote counts (if applicable).

### 3.2. Decision-Making Process
1.  **Proposal:** Architectural decisions are typically triggered by an RFC (see `docs/governance/rfc-process.md`).
2.  **Discussion:** ARB members discuss the proposal during meetings and/or designated communication channels (e.g., a private ARB mailing list or forum section for deliberation, with summaries made public).
3.  **Voting:**
    *   Decisions are made by simple majority vote of members present at a meeting with quorum.
    *   A 2/3 majority of *all seated members* (not just those present) is required for:
        *   Changes to the `ARCHITECTURAL_MANIFESTO.md`.
        *   Addition or removal of a Tier 1 architecture.
        *   Fundamental changes to the ARB charter itself.
    *   Each member has one vote. The Chair votes as a regular member.
4.  **Tie-Breaking:** If a simple majority vote results in a tie, the Lead Architect (Chair) may cast an additional tie-breaking vote OR table the motion for further discussion and a future vote.
5.  **Veto Power:** The Lead Architect holds a veto power for decisions that, in their documented assessment, would critically compromise Harvey OS's strategic goals, security, or stability. A veto must be accompanied by a detailed written justification. A veto can be overridden by a unanimous vote of all other seated ARB members within 30 days.
6.  **Decision Log:** All significant decisions, their rationale, and vote outcomes (where applicable) are recorded in a publicly accessible decision log (e.g., `docs/governance/decision-log.md`).

### 3.3. Transparency
While ARB deliberations may occur privately to allow frank discussion, the ARB is committed to transparency. All RFCs, meeting agendas, minutes (excluding highly sensitive security details), and decision logs will be publicly accessible.

## 4. Conflict of Interest
ARB members must declare any potential conflicts of interest related to proposals under review and may need to recuse themselves from voting on such proposals.

## 5. Amendments to this Charter
Amendments to this charter require a 2/3 majority vote of all seated ARB members, following an RFC proposing the changes and a community review period of at least 30 days.
