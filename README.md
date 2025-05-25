# Harvey Build Instructions

This repository contains a small subset of the Harvey OS utilities.
The `.codex/setup.sh` script installs toolchains, QEMU and CMake
packages required for cross development. After the dependencies are
installed run the following commands to build the utilities using
CMake:

```bash
cmake -S . -B build
cmake --build build
```

Before committing changes, run `pre-commit` to execute formatters and
`clang-tidy` checks. The `.codex/setup.sh` script installs the required
tools and configures the git hook via `pre-commit install`.

To boot a custom Harvey kernel without a graphical console you can use
`qemu-nox` via the helper script in `scripts/run_qemu_nox.sh`:

```bash
./scripts/run_qemu_nox.sh path/to/harvey-kernel
```

The script invokes `qemu-system-x86_64` with the `-nographic` flag so
that the virtual machine is accessible through the terminal. Append any
additional QEMU arguments after the kernel path if needed.
