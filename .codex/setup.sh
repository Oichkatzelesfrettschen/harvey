#!/usr/bin/env bash
set -euo pipefail
export DEBIAN_FRONTEND=noninteractive

FAIL_LOG=/tmp/setup_failures.log
touch "$FAIL_LOG"

#— helper to pin to the repo’s exact version if it exists
apt_pin_install(){
  pkg="$1"
  ver=$(apt-cache show "$pkg" 2>/dev/null \
        | awk '/^Version:/{print $2; exit}')
  if [ -n "$ver" ]; then
    install_cmd=(apt-get install -y "${pkg}=${ver}")
  else
    install_cmd=(apt-get install -y "$pkg")
  fi
  if ! "${install_cmd[@]}"; then
    echo "$pkg" >> "$FAIL_LOG"
    # Attempt python fallback if applicable
    if [[ "$pkg" == python3-* ]]; then
      pip_pkg="${pkg#python3-}"
      pip3 install --no-cache-dir "$pip_pkg" \
        || echo "pip3 install $pip_pkg failed" >> "$FAIL_LOG"
    elif [[ "$pkg" == pre-commit ]]; then
      pip3 install --no-cache-dir pre-commit \
        || echo "pip3 install pre-commit failed" >> "$FAIL_LOG"
    fi
  fi
}

#— enable foreign architectures for cross-compilation
for arch in i386 armel armhf arm64 riscv64 powerpc ppc64el ia64; do
  dpkg --add-architecture "$arch"
done

if ! apt-get update -y; then
  echo "apt-get update failed" >> "$FAIL_LOG"
fi

#— core build tools, formatters, analysis, science libs
for pkg in \
  build-essential gcc g++ clang lld llvm \
  clang-tidy clang-format uncrustify astyle editorconfig pre-commit \
  make bmake ninja-build cmake meson \
  autoconf automake libtool m4 gawk flex bison byacc \
  pkg-config file ca-certificates curl git unzip \
  libopenblas-dev liblapack-dev libeigen3-dev \
  strace ltrace linux-perf systemtap systemtap-sdt-dev crash \
  valgrind kcachegrind trace-cmd kernelshark \
  libasan6 libubsan1 likwid hwloc; do
  apt_pin_install "$pkg"
done

# ensure pre-commit is available
if ! command -v pre-commit >/dev/null 2>&1; then
  if ! pip3 install --no-cache-dir pre-commit; then
    echo "pip3 install pre-commit failed" >> "$FAIL_LOG"
  fi
fi

# ensure yacc command exists
if ! command -v yacc >/dev/null 2>&1; then
  if command -v byacc >/dev/null 2>&1; then
    ln -s "$(command -v byacc)" /usr/local/bin/yacc
  elif command -v bison >/dev/null 2>&1; then
    ln -s "$(command -v bison)" /usr/local/bin/yacc
  fi
fi

#— Python & deep-learning / MLOps
for pkg in \
  python3 python3-pip python3-dev python3-venv python3-wheel \
  python3-numpy python3-scipy python3-pandas \
  python3-matplotlib python3-scikit-learn \
  python3-torch python3-torchvision python3-torchaudio \
  python3-yaml \
  python3-onnx python3-onnxruntime; do
  apt_pin_install "$pkg"
done

if ! pip3 install --no-cache-dir \
  tensorflow-cpu jax jaxlib \
  tensorflow-model-optimization mlflow onnxruntime-tools; then
  echo "pip3 install ML packages failed" >> "$FAIL_LOG"
fi

#— QEMU emulation for foreign binaries
for pkg in \
  qemu-user-static \
  qemu-system-x86 qemu-system-x86-64 qemu-system-i386 \
  qemu-system-arm qemu-system-aarch64 \
  qemu-system-riscv64 qemu-system-mips qemu-system-mipsel \
  qemu-system-ppc qemu-system-ppc64 qemu-system-s390x \
  qemu-system-sparc qemu-utils; do
  apt_pin_install "$pkg"
done

#— multi-arch cross-compilers
for pkg in \
  bcc bin86 elks-libc \
  gcc-ia64-linux-gnu g++-ia64-linux-gnu \
  gcc-i686-linux-gnu g++-i686-linux-gnu \
  gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
  gcc-arm-linux-gnueabi g++-arm-linux-gnueabi \
  gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf \
  gcc-riscv64-linux-gnu g++-riscv64-linux-gnu \
  gcc-powerpc-linux-gnu g++-powerpc-linux-gnu \
  gcc-powerpc64-linux-gnu g++-powerpc64-linux-gnu \
  gcc-powerpc64le-linux-gnu g++-powerpc64le-linux-gnu \
  gcc-m68k-linux-gnu g++-m68k-linux-gnu \
  gcc-hppa-linux-gnu g++-hppa-linux-gnu \
  gcc-loongarch64-linux-gnu g++-loongarch64-linux-gnu \
  gcc-mips-linux-gnu g++-mips-linux-gnu \
  gcc-mipsel-linux-gnu g++-mipsel-linux-gnu \
  gcc-mips64-linux-gnuabi64 g++-mips64-linux-gnuabi64 \
  gcc-mips64el-linux-gnuabi64 g++-mips64el-linux-gnuabi64; do
  apt_pin_install "$pkg"
done

#— high-level language runtimes and tools
for pkg in \
  golang-go nodejs npm typescript \
  rustc cargo clippy rustfmt \
  lua5.4 liblua5.4-dev luarocks \
  ghc cabal-install hlint stylish-haskell \
  sbcl ecl clisp cl-quicklisp slime cl-asdf \
  ldc gdc dmd-compiler dub libphobos-dev \
  chicken-bin libchicken-dev chicken-doc \
  openjdk-17-jdk maven gradle dotnet-sdk-8 mono-complete \
  swift swift-lldb swiftpm kotlin gradle-plugin-kotlin \
  ruby ruby-dev gem bundler php-cli php-dev composer phpunit \
  r-base r-base-dev dart flutter gnat gprbuild gfortran gnucobol \
  fpc lazarus zig nim nimble crystal shards gforth; do
  apt_pin_install "$pkg"
done

#— GUI & desktop-dev frameworks
for pkg in \
  libqt5-dev qtcreator libqt6-dev \
  libgtk1.2-dev libgtk2.0-dev libgtk-3-dev libgtk-4-dev \
  libfltk1.3-dev xorg-dev libx11-dev libxext-dev \
  libmotif-dev openmotif cde \
  xfce4-dev-tools libxfce4ui-2-dev lxde-core lxqt-dev-tools \
  libefl-dev libeina-dev \
  libwxgtk3.0-dev libwxgtk3.0-gtk3-dev \
  libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev \
  libglfw3-dev libglew-dev; do
  apt_pin_install "$pkg"
done

#— containers, virtualization, HPC, debug
for pkg in \
  docker.io podman buildah virt-manager libvirt-daemon-system qemu-kvm \
  gdb lldb perf gcovr lcov bcc-tools bpftrace \
  openmpi-bin libopenmpi-dev mpich; do
  apt_pin_install "$pkg"
done

#— IA-16 (8086/286) cross-compiler
if IA16_VER=$(curl -fsSL https://api.github.com/repos/tkchia/gcc-ia16/releases/latest \
               | awk -F\" '/tag_name/{print $4; exit}'); then
  if curl -fsSL "https://github.com/tkchia/gcc-ia16/releases/download/${IA16_VER}/ia16-elf-gcc-linux64.tar.xz" \
       | tar -Jx -C /opt; then
    echo 'export PATH=/opt/ia16-elf-gcc/bin:$PATH' > /etc/profile.d/ia16.sh
    export PATH=/opt/ia16-elf-gcc/bin:$PATH
  else
    echo "ia16-elf-gcc install failed" >> "$FAIL_LOG"
  fi
else
  echo "ia16-elf-gcc version fetch failed" >> "$FAIL_LOG"
fi

#— protoc installer (pinned)
PROTO_VERSION=25.1
if curl -fsSL "https://raw.githubusercontent.com/protocolbuffers/protobuf/v${PROTO_VERSION}/protoc-${PROTO_VERSION}-linux-x86_64.zip" \
  -o /tmp/protoc.zip && unzip -d /usr/local /tmp/protoc.zip; then
  rm /tmp/protoc.zip
else
  echo "protoc install failed" >> "$FAIL_LOG"
fi

#— gmake alias
command -v gmake >/dev/null 2>&1 || ln -s "$(command -v make)" /usr/local/bin/gmake

# install git pre-commit hooks if pre-commit is available
if command -v pre-commit >/dev/null 2>&1; then
  if ! pre-commit install --install-hooks; then
    echo "pre-commit install failed" >> "$FAIL_LOG"
  fi
fi

#— clean up
apt-get clean
rm -rf /var/lib/apt/lists/*

if [ -s "$FAIL_LOG" ]; then
  echo "The following packages failed to install:" >&2
  cat "$FAIL_LOG" >&2
fi

exit 0
