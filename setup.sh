#!/usr/bin/env bash
# Root environment setup script used to install all dependencies.
# It attempts to
# install packages via apt, then falls back to pip and npm if needed. Any
# failures are logged but the script continues.
set -euo pipefail
set -x

export DEBIAN_FRONTEND=noninteractive

LOG_FILE=/tmp/setup.log
FAIL_LOG=/tmp/setup_failures.log
touch "$LOG_FILE" "$FAIL_LOG"

exec > >(tee -a "$LOG_FILE") 2>&1

if ! apt-get update -y; then
  echo "apt-get update failed" >> "$FAIL_LOG"
fi

if ! apt-get dist-upgrade -y; then
  echo "apt-get dist-upgrade failed" >> "$FAIL_LOG"
fi

install_pkg() {
  local pkg="$1"
  if ! apt-get install -y "$pkg"; then
    echo "apt-get install $pkg failed" >> "$FAIL_LOG"
    if ! pip3 install --no-cache-dir "$pkg"; then
      echo "pip3 install $pkg failed" >> "$FAIL_LOG"
      if ! npm install -g "$pkg"; then
        echo "npm install $pkg failed" >> "$FAIL_LOG"
        case "$pkg" in
          capnproto)
            if command -v wget >/dev/null 2>&1; then
              wget -q https://capnproto.org/capnproto-c++-0.9.1.tar.gz -O /tmp/capnproto.tar.gz && \
                tar -xzf /tmp/capnproto.tar.gz -C /tmp && \
                cd /tmp/capnproto-c++-0.9.1 && \
                ./configure && make -j"$(nproc)" && make install
            else
              echo "wget not found for capnproto" >> "$FAIL_LOG"
            fi
            ;;
        esac
      fi
    fi
  fi
}

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
  python3-yaml
  shellcheck
  graphviz
  doxygen
  compiledb
  configuredb
  llvm
  lld
  coq
  isabelle
  capnproto
  gcc-multilib
  g++-multilib
  rc
  expect
  go-dep
  golang-go
)

# Emulator packages for Harvey
REQUIRED_PACKAGES+=(
  qemu
  qemu-system-x86
  qemu-utils
  bochs
)

for pkg in "${REQUIRED_PACKAGES[@]}"; do
  install_pkg "$pkg"
done

# Install PyYAML for Python hooks
if ! pip3 install --no-cache-dir pyyaml; then
  echo "pip3 install pyyaml failed" >> "$FAIL_LOG"
fi

apt-get clean
rm -rf /var/lib/apt/lists/*

if [ -s "$FAIL_LOG" ]; then
  echo "The following packages failed to install:" >&2
  cat "$FAIL_LOG" >&2
fi

exit 0
