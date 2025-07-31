#!/bin/bash
set -euxo pipefail

export DEBIAN_FRONTEND=noninteractive

echo "Attempting to clean and update apt repositories..."
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*
# Add a small delay in case of rapid operations
sleep 2
if ! sudo apt-get update -y; then
    echo "apt-get update failed. There might be an issue with the system's clock or repository configuration."
    echo "Please investigate this manually. Some packages may not install."
    # Exiting here might be too strict if some tools can still be installed.
    # For now, we'll let it continue and try to install what it can.
fi

echo "Installing core development tools..."
sudo apt-get install -y \
    build-essential \
    clang-17 lld-17 llvm-17-tools \
    gcc-13 gdb \
    valgrind

# Check if specific versions were installed (apt might pick defaults)
echo "Verifying tool versions (approximate check)..."
clang --version || echo "clang not found or version check failed"
gcc --version || echo "gcc not found or version check failed"
# gdb --version || echo "gdb not found or version check failed"
valgrind --version || echo "valgrind not found or version check failed"

echo "Installing QEMU for user and system emulation..."
sudo apt-get install -y qemu-user-static qemu-system-arm qemu-system-x86

echo "Installing cross compilers..."
# x86 (multilib for 32-bit from 64-bit host)
sudo apt-get install -y gcc-multilib g++-multilib

# ARM32 (ARMv7 with hard-float)
sudo apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf libc6-dev-armhf-cross

# ARM64
sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu libc6-dev-arm64-cross

echo "Advanced development environment setup script finished."
echo "NOTE: This script provides a baseline. Specific QEMU system images for all 13 architectures"
echo "and highly specific cross-compilers (e.g., for i386, i486, ARMv6, ARMv8.x variants)"
echo "may require manual searching and installation if not covered by these general packages."
