# Harvey OS

This repository contains Harvey OS, a modern, open-source operating system derived from Plan 9.

## Getting Started with Harvey OS

Welcome to Harvey OS! Whether you're looking to try Harvey, develop for it, or contribute to the project, this section will guide you.

### Full Development Environment Setup (Recommended)

For the most complete experience, including building the OS, running it in QEMU, and setting up for cross-compilation, we provide a comprehensive 10-step installation guide:

*   **Start Here: [Harvey OS Installation Guide](./docs/installation_guide/README.md)**

This guide will walk you through:
1.  Preparing your host system (Ubuntu 24.04 LTS recommended).
2.  Using our automated script (`setup-harvey-dev-env.sh`) to install all necessary toolchains (Plan 9 and modern) and dependencies.
3.  Cloning, building, and running Harvey OS for native and cross-compiled targets (e.g., x86_64, ARM64) in QEMU.
4.  Validating your setup and finding your next steps in the Harvey ecosystem.

**(Conceptual Installation Status: [![Build Status](<link_to_CI_badge_for_setup_script_if_exists>)](<link_to_CI_pipeline>))**
*(This is a placeholder for a real CI badge indicating the health of the main setup script or core build.)*

### Understanding Harvey OS

Before diving into code, or if you want to understand the project's vision and architecture:

*   **[ARCHITECTURAL_MANIFESTO.md](./ARCHITECTURAL_MANIFESTO.md):** Our core document outlining Harvey OS's vision, principles, use cases, and technical direction.
*   **[DEVELOPER_JOURNEY.md](./DEVELOPER_JOURNEY.md):** (To be created) A guide to help you find resources based on your goals.

### Developer Prerequisites

If you plan to contribute, please familiarize yourself with:
*   The [Installation Guide](./docs/installation_guide/README.md) to set up your environment correctly.
*   Our [Contribution Guide](./docs/community/contribution-guide.md).
*   The [Architectural Manifesto](./ARCHITECTURAL_MANIFESTO.md) to understand our design goals.

## Architectural Vision and Modernization Strategy

The Harvey OS project is undergoing a comprehensive modernization effort. Our architectural vision, build strategy, CI/CD approach, and modernization protocols are detailed in the following documents:

*   **[ARCHITECTURAL_MANIFESTO.md](./ARCHITECTURAL_MANIFESTO.md):** Defines the core identity, multi-architecture philosophy, and guiding principles for Harvey OS's future.
*   **Build System:** The build system has been refactored for clarity and to support multi-architecture builds more systematically. Key configurations can be found in `mk/arch/` and the main Makefiles. Refer to the "Multi-architecture Philosophy" in the Architectural Manifesto for guiding principles.
*   **[CI Strategy](./docs/ci-strategy.md):** Outlines our approach to continuous integration for robust multi-architecture validation.
*   **[Systematic Modernization Protocol](./docs/systematic-modernization.md):** Details our methods for C17 migration, Plan 9 API management, and POSIX conformance.

We encourage all contributors to familiarize themselves with these documents.

## Legacy Build Information & Quick Start (Modern Utilities)

The information below pertains to older methods or specific utilities within this repository. For a full OS setup, please use the **[Harvey OS Installation Guide](./docs/installation_guide/README.md)**.

The `./setup.sh` script (distinct from `setup-harvey-dev-env.sh`) automatically installs some toolchains, QEMU and the Bochs emulator along with other packages required for cross development. After the dependencies are installed run `make` to build the utilities under the `modern` directory.

The setup script also installs additional packages for 32-bit
cross compilation, debugging utilities, and Go tooling used by
the CI workflows. These include `gcc-multilib`, `g++-multilib`, `rc`,
`expect`, the `linux-tools` package (providing `perf`), `golang-go`
and the `PyYAML` Python module. The Go dependency manager `dep`
is installed separately via `go install`.

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

## Build Flags

Running `setup.sh` exports a set of optimized compiler flags.  The variables
`CFLAGS` and `CXXFLAGS` default to `-O3 -march=native -mtune=native -pipe -fPIC`
while `LDFLAGS` defaults to `-fuse-ld=lld`.  Override them when invoking
`make` if different flags are required:

```bash
CFLAGS='-O2 -g' make
```

These defaults are also used by the CI workflows so local builds match the
automated tests.

## Documentation

Run `doxygen docs/Doxyfile` to extract API references from all C sources.  The
XML output is written to `build/docs/xml` and consumed by Sphinx.  Build the
HTML manual with:

```bash
sphinx-build -b html docs build/sphinx
```

The resulting pages in `docs/_build` can be viewed with any browser.
