# Harvey Build Instructions

This repository contains a small subset of the Harvey OS utilities.
The `./setup.sh` script automatically installs the toolchains,
QEMU and the Bochs emulator along with other packages required for
cross development. After the dependencies are installed run `make`
to build the utilities under the `modern` directory.

The setup script also installs additional packages for 32-bit
cross compilation, Go tooling, and various utilities used by the
CI workflows. These include `gcc-multilib`, `g++-multilib`, `rc`,
`expect`, `go-dep`, `golang-go` and the `PyYAML` Python module.

The setup script installs `pre-commit` so you can run the hooks locally.
After running the setup script, execute `pre-commit install` to add the git
hooks. Run `pre-commit` in the repository root before pushing changes to
ensure all configured checks pass. The hooks invoke `shellcheck` for shell
scripts and automatically generate `compile_commands.json` using
`compiledb`.

Before committing changes, run `pre-commit` or rely on the CI workflow
which executes all hooks using the `pre-commit` GitHub action. The
pipeline installs additional utilities such as `compiledb`,
`configuredb` and `shellcheck`.

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

## Documentation

Doxygen extracts API references for all C sources. Run `doxygen docs/Doxyfile` to
generate XML output consumed by Sphinx. To build the HTML manual, execute:

```bash
sphinx-build -b html docs docs/_build
```

The resulting pages in `docs/_build` can be viewed with any browser.
