#!/usr/bin/env bash
# Root environment setup script used to install all dependencies.
# It installs packages via apt and falls back to pip, npm or go where needed.
# Any failures are logged but the script attempts to proceed.
set -euo pipefail
set -xv

export DEBIAN_FRONTEND=noninteractive

LOG_FILE=/tmp/setup.log
FAIL_LOG=/tmp/setup_failures.log
touch "$LOG_FILE" "$FAIL_LOG"

exec > >(tee -a "$LOG_FILE") 2>&1

## \brief Configure default build flags if not already set.
set_build_flags() {
  export CFLAGS="${CFLAGS:--O3 -march=native -mtune=native -pipe -fPIC}"
  export CXXFLAGS="${CXXFLAGS:--O3 -march=native -mtune=native -pipe -fPIC}"
  export LDFLAGS="${LDFLAGS:--fuse-ld=lld}"
}

## \brief Update apt repositories and upgrade packages.
update_system() {
  apt-get update -y || echo "apt-get update failed" >> "$FAIL_LOG"
  apt-get dist-upgrade -y || echo "apt-get dist-upgrade failed" >> "$FAIL_LOG"
  if ! command -v apt-cache >/dev/null 2>&1; then
    apt-get install -y apt || echo "apt install failed" >> "$FAIL_LOG"
  fi
}

## \brief Install a package via apt, pip3 or npm.
## \param pkg Package name
install_pkg() {
  local pkg="$1"
  apt-get install -y "$pkg" || \
    pip3 install --no-cache-dir "$pkg" || \
    npm install -g "$pkg" || \
    echo "failed to install $pkg" >> "$FAIL_LOG"
}

## \brief Install all packages listed on the command line.
install_list() {
  for pkg in "$@"; do
    install_pkg "$pkg"
  done
}

## \brief Install any packages related to tmux by searching apt.
install_tmux_packages() {
  mapfile -t pkgs < <(apt-cache search '^tmux' | awk '{print $1}')
  install_list "${pkgs[@]:-}"
}

## \brief Install required Python packages via pip.
install_pip_packages() {
  local pkgs=(black flake8 pyyaml)
  for pkg in "${pkgs[@]}"; do
    pip3 install --no-cache-dir "$pkg" || echo "pip install $pkg failed" >> "$FAIL_LOG"
  done
}

## \brief Install global npm packages.
install_npm_packages() {
  local pkgs=(eslint)
  for pkg in "${pkgs[@]}"; do
    npm install -g "$pkg" || echo "npm install $pkg failed" >> "$FAIL_LOG"
  done
}

## \brief Install Go tools using go install.
install_go_packages() {
  local pkgs=(
    harvey-os.org/cmd/ufs@latest
    github.com/golang/dep/cmd/dep@latest
  )
  for pkg in "${pkgs[@]}"; do
    GOPATH=${GOPATH:-$HOME/go} go install "$pkg" || echo "go install $pkg failed" >> "$FAIL_LOG"
  done
}

## \brief Clean up package lists and print any failures.
finalize() {
  apt-get clean
  rm -rf /var/lib/apt/lists/*
  if [ -s "$FAIL_LOG" ]; then
    echo "The following packages failed to install:" >&2
    cat "$FAIL_LOG" >&2
  fi
}

update_system
set_build_flags

REQUIRED_PACKAGES=(
  apt-utils apt-transport-https
  build-essential clang clang-tidy clang-format make git
  python3 python3-pip pre-commit python3-yaml python3-sphinx
  python3-breathe python3-sphinx-rtd-theme shellcheck cloc graphviz doxygen
  compiledb llvm lld coq isabelle capnproto gcc-multilib g++-multilib
  rc expect go-dep golang-go gdb gdbserver valgrind strace ltrace \
  linux-tools-common linux-tools-generic lsof htop
  dstat afl++ zzuf radamsa ccache lcov binutils binutils-aarch64-linux-gnu
  binutils-x86-64-linux-gnu gcc-aarch64-linux-gnu g++-aarch64-linux-gnu nodejs npm
  qemu qemu-system-x86 qemu-system-arm qemu-utils qemu-nox tmux bochs
)

install_list "${REQUIRED_PACKAGES[@]}"
install_tmux_packages
install_pip_packages
install_npm_packages
install_go_packages
finalize
exit 0
