# Harvey Build Instructions

This repository contains a small subset of the Harvey OS utilities.
The `.codex/setup.sh` script automatically installs the toolchains,
QEMU and the Bochs emulator along with other packages required for
cross development. After the dependencies are installed run `make`
to build the utilities under the `modern` directory.

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

## Cross Compilation

The Makefiles can produce binaries for multiple architectures. Select a
target by passing the `ARCH` variable to `make`:

```bash
make ARCH=i386     # build 32-bit binaries
make ARCH=x86_64   # build 64-bit binaries
make ARCH=i8086    # experimental 16-bit build
```

Use `make test` to compile all entries defined in the `ARCHS` variable
(defaults to `i386 x86_64`).
