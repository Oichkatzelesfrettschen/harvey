# Step 1: Prepare Host System & Obtain Setup Script

This first step ensures your host system is ready and you have the Harvey OS development environment setup script.

## 1.1. Host System Prerequisites

Before you begin, please ensure your system meets the following requirements:

*   **Operating System:** A modern Linux distribution. This guide and the setup script are optimized for **Ubuntu 24.04 (Noble Numbat)** or a compatible derivative (e.g., Debian Bookworm or newer). Other distributions may work but might require manual adjustments to the setup script or package names.
*   **Permissions:** You will need `sudo` (administrator) privileges to install system-wide packages.
*   **Internet Access:** Required to download the setup script and system packages.
*   **Basic Utilities:**
    *   `wget` or `curl`: To download the setup script. Most systems have one of these pre-installed.
    *   `git`: To clone the Harvey OS source code later. The setup script will install this if missing, but it's good to have.
*   **Disk Space:** At least 20-30 GB of free disk space is recommended for the OS source code, toolchains, and build artifacts.
*   **Memory (RAM):** A minimum of 4GB RAM is recommended, with 8GB or more being preferable for a smoother experience, especially when running QEMU instances.

You can check your Ubuntu version with `lsb_release -a`.

To install `wget` (if you don't have it):
```bash
sudo apt-get update
sudo apt-get install wget
```
To install `git` (if you don't have it):
```bash
sudo apt-get update
sudo apt-get install git
```

## 1.2. Obtain the Harvey OS Development Environment Setup Script

The `setup-harvey-dev-env.sh` script automates the installation of nearly all dependencies and tools required for Harvey OS development.

1.  **Choose a Workspace Directory:**
    Decide where you want to set up your Harvey OS development area. The script will create a root directory (defaulting to `$HOME/harvey-dev`) and install tools and source code within it. For this guide, we'll assume `$HOME/harvey-dev`.

2.  **Download the Script:**
    Open your terminal and use `wget` to download the script. Assuming the script is hosted in the `scripts/` directory of the main Harvey OS repository on GitHub:
    ```bash
    # Create the workspace if you want it in a specific place, e.g., $HOME/harvey-workspace
    # mkdir -p $HOME/harvey-workspace
    # cd $HOME/harvey-workspace

    # Or let the script create its default $HOME/harvey-dev
    cd $HOME

    wget https://raw.githubusercontent.com/Harvey-OS/harvey/main/scripts/setup-harvey-dev-env.sh
    ```
    (Note: If the URL changes, please use the official URL provided by the Harvey OS project.)

3.  **Make the Script Executable:**
    Once downloaded, you need to give the script execute permissions:
    ```bash
    chmod +x setup-harvey-dev-env.sh
    ```

## 1.3. Next Steps

You are now ready to execute the setup script, which is covered in the next step of this guide. Make sure you are in the directory where you downloaded `setup-harvey-dev-env.sh` before proceeding.

If you encountered any issues downloading the script or ensuring prerequisites, please resolve them before moving to Step 2.

## 1.4 Troubleshooting and Recovery Procedures

### Common Failure Modes

#### Failure Mode 1: `wget` command not found
*   **Symptoms:** Terminal outputs `wget: command not found` or similar.
*   **Root Cause Analysis:** `wget` is not installed on your system.
*   **Resolution Protocol:**
    ```bash
    sudo apt-get update
    sudo apt-get install wget
    ```
    Then re-try downloading the script. If `apt-get` also fails, check your system's package manager or internet connectivity.

#### Failure Mode 2: Permission denied when running `chmod +x`
*   **Symptoms:** Terminal outputs `chmod: changing permissions of 'setup-harvey-dev-env.sh': Operation not permitted` or similar.
*   **Root Cause Analysis:** You do not own the file, or you are in a directory where you don't have write/execute permissions on the file. This is uncommon if you just downloaded it to your home directory.
*   **Resolution Protocol:**
    *   Ensure you are the owner of the script: `ls -l setup-harvey-dev-env.sh`.
    *   If not, and you downloaded it as root, you might need `sudo chmod +x setup-harvey-dev-env.sh`, but it's better to ensure you download and own the script as your regular user.
    *   Check directory permissions if you are in an unusual location.

#### Failure Mode 3: Network connectivity issues preventing script download
*   **Symptoms:** `wget` hangs indefinitely or reports errors like "Unable to resolve host address" or "Connection timed out."
*   **Root Cause Analysis:** Your system has no internet access, DNS is not working, or a firewall is blocking the connection.
*   **Resolution Protocol:**
    *   Check your system's network connection (e.g., try `ping google.com`).
    *   Verify DNS settings (e.g., `cat /etc/resolv.conf`).
    *   Check local or network firewalls.
    *   Try downloading from a different network if possible, or ensure the URL is correct and accessible.

## 1.5 Architectural Alignment

This initial preparation step aligns with several Harvey OS architectural principles:

*   **Environmental Resilience ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#7-environmental-resilience-and-reproducible-builds)):** By guiding users to obtain a canonical setup script, we begin the process of ensuring a reproducible and consistent development environment, which is a core tenet of our resilience strategy.
*   **Developer Experience (as implicitly supported by [Contribution Guide](../../docs/community/contribution-guide.md)):** Providing clear, prerequisite checks and a standardized setup script is the first step towards a positive developer onboarding experience.
*   **Accessibility (General Principle):** Ensuring users can easily obtain the necessary tools to start working with Harvey OS, regardless of their prior specific Plan 9 experience, by leveraging standard Linux tools like `wget` and `bash`.
