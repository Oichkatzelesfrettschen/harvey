# Step 5: Clone Harvey OS Source Code

With your development environment set up and activated, you are now ready to download the Harvey OS source code. Harvey OS uses Git for version control, and the source code is typically hosted on a platform like GitHub.

## 5.1. Prerequisites

*   **Git Installed:** The `setup-harvey-dev-env.sh` script should have installed Git. You can verify this with `git --version`.
*   **Harvey Environment Activated:** Ensure you have sourced `$HARVEY_ROOT/harvey-env.sh` in your current terminal session as per Step 4. While not strictly necessary for cloning, it's good practice to work within the configured environment.
*   **Designated Source Directory:** The `setup-harvey-dev-env.sh` script creates a workspace structure, including a `src/` directory within `$HARVEY_ROOT`. This is the recommended location for cloning the Harvey OS source code.

## 5.2. Cloning the Repository

1.  **Navigate to the Source Directory:**
    Open your terminal and change to the `src` directory within your Harvey OS workspace:
    ```bash
    cd $HARVEY_ROOT/src
    # If HARVEY_ROOT is not an active environment variable, use the full path:
    # cd $HOME/harvey-dev/src
    ```

2.  **Clone the Harvey OS Repository:**
    Use the `git clone` command to download the source code. You will need the official repository URL. The example below uses a common URL structure for GitHub projects. **Please replace `<official_harvey_repo_url>` with the actual URL provided by the Harvey OS project.**
    ```bash
    git clone <official_harvey_repo_url> harvey-os
    ```
    For example, if the URL is `https://github.com/Harvey-OS/harvey.git`, the command would be:
    ```bash
    git clone https://github.com/Harvey-OS/harvey.git harvey-os
    ```
    This command:
    *   Downloads the entire history of the Harvey OS project.
    *   Creates a new directory named `harvey-os` (or whatever you specify as the last argument) inside `$HARVEY_ROOT/src/`.
    *   Populates this directory with the Harvey OS source files.

    This process might take a few minutes depending on the size of the repository and your internet connection.

## 5.3. After Cloning

1.  **Verify Cloned Directory:**
    Once the clone is complete, check that the `harvey-os` directory has been created:
    ```bash
    ls
    # You should see 'harvey-os' listed.
    ```

2.  **Navigate into the Cloned Repository:**
    ```bash
    cd harvey-os
    ```
    You are now in the root directory of the Harvey OS source code. You can explore the file and directory structure (e.g., using `ls -la`). You should see familiar top-level directories like `sys` (for kernel code), `cmd` (for userland commands), `lib` (for libraries), Makefiles (`mkfile` or `Makefile`), etc.

## 5.4. Staying Updated (Optional - For Future Reference)

To update your local copy of the Harvey OS source code with the latest changes from the official repository in the future, you can navigate into `$HARVEY_ROOT/src/harvey-os` and run:
```bash
git pull origin main # Or the appropriate branch, e.g., master or develop
```

## 5.5. Next Steps

With the Harvey OS source code successfully cloned, you are now ready to compile it. Proceed to **Step 6: Perform Initial Build (Native Architecture)**.

## 5.6 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: `git` command not found
*   **Symptoms:** Terminal outputs `git: command not found`.
*   **Root Cause Analysis:** Git was not installed by `setup-harvey-dev-env.sh` or was uninstalled.
*   **Resolution Protocol:**
    1.  Run `sudo apt-get update && sudo apt-get install git` (or your system's equivalent).
    2.  Re-try the `git clone` command.

#### Failure Mode 2: Repository URL incorrect or inaccessible
*   **Symptoms:** `git clone` fails with errors like "Repository not found," "Authentication failed," or "Could not resolve hostname."
*   **Root Cause Analysis:**
    *   The URL `<official_harvey_repo_url>` was not replaced with the actual Harvey OS repository URL.
    *   A typo in the URL.
    *   Network connectivity issues (DNS, firewall, no internet).
    *   The repository is private and requires authentication you haven't provided (less common for initial public clone).
*   **Resolution Protocol:**
    1.  Double-check the official Harvey OS repository URL.
    2.  Verify network connectivity (`ping github.com` or the relevant host).
    3.  Ensure you are using `https` for public cloning unless SSH is specifically instructed and set up.
    4.  If authentication is required (e.g., for a private fork you're using), ensure your Git client is configured with the correct credentials or SSH keys.

#### Failure Mode 3: Permission denied in target directory
*   **Symptoms:** `git clone` fails with "Permission denied" when trying to create the `harvey-os` directory.
*   **Root Cause Analysis:** You do not have write permissions in the current directory (e.g., `$HARVEY_ROOT/src`). This is unlikely if `setup-harvey-dev-env.sh` created `$HARVEY_ROOT` correctly under your home directory.
*   **Resolution Protocol:**
    1.  Verify permissions of the parent directory: `ls -ld $HARVEY_ROOT/src`.
    2.  Ensure you own the directory or have write permissions. If not, use `sudo chown -R $(whoami):$(whoami) $HARVEY_ROOT` (use with caution if `HARVEY_ROOT` is a system path, but it shouldn't be).
    3.  Alternatively, choose a different directory where you have write permissions to clone into and adjust `$HARVEY_ROOT` or your working paths accordingly.

## 5.7 Architectural Alignment

Cloning the source code is fundamental and aligns with:

*   **Single Source of Truth (Implied by VCS usage):** Using Git to obtain the source code ensures you are working with the canonical version controlled codebase, which is essential for collaborative development and reproducible builds ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)).
*   **Systematic Modernization ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#4-modernization-principles)):** Access to the full source tree is the prerequisite for any modernization efforts, including C17 migration (`docs/operations/c17-migration-guide.md`) and code analysis.
*   **Community Contribution (`docs/community/contribution-guide.md`):** The standard Git workflow (clone, branch, commit, PR) is central to how community members contribute to Harvey OS. This step initiates that workflow for the user.
