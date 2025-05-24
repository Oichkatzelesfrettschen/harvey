#!/usr/bin/env bash
set -euo pipefail
export DEBIAN_FRONTEND=noninteractive

FAIL_LOG=/tmp/setup_failures.log
touch "$FAIL_LOG"

if ! apt-get update -y; then
  echo "apt-get update failed" >> "$FAIL_LOG"
fi

# Minimal packages required to build this repository
REQUIRED_PACKAGES=(
  build-essential
  clang
  clang-tidy
  clang-format
  make
  git
  python3
  python3-pip
)

if ! apt-get install -y "${REQUIRED_PACKAGES[@]}"; then
  echo "apt-get install failed" >> "$FAIL_LOG"
fi

# Install pre-commit via pip
if ! pip3 install --no-cache-dir pre-commit; then
  echo "pip3 install pre-commit failed" >> "$FAIL_LOG"
fi

# Install git pre-commit hooks
if command -v pre-commit >/dev/null 2>&1; then
  if ! pre-commit install --install-hooks; then
    echo "pre-commit install failed" >> "$FAIL_LOG"
  fi
fi

apt-get clean
rm -rf /var/lib/apt/lists/*

if [ -s "$FAIL_LOG" ]; then
  echo "The following packages failed to install:" >&2
  cat "$FAIL_LOG" >&2
fi

exit 0
