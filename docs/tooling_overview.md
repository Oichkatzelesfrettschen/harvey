# Tooling Overview

## Package Installation

The project relies on a set of development utilities installed via the system package manager or Python's package installer:

- `pre-commit`, `clang-format`, `clang-tidy`, `shellcheck` and `doxygen` are available through `apt`.
- `compiledb` is distributed as a Python package and is installed with `pip`.
- `graphviz` is required for Doxygen's diagram generation and is also obtained from `apt`.

## Pre-commit

The repository defines custom hooks in the pre-commit configuration file `.pre-commit-config.yaml` to enforce formatting and static analysis during commits【F:.pre-commit-config.yaml†L1-L33】.

After installing the package, enable the hooks with `pre-commit install`. Executing the installed hooks yields no issues for the files exercised in this task:

```
pre-commit install
```

## Clang-Format

Source formatting adheres to the style options specified in `.clang-format`【F:.clang-format†L1-L4】. Verifying the formatting of `hello.c` produces no output, indicating the file is already compliant:

```
clang-format --dry-run --Werror hello.c
```

## Clang-Tidy

Static analysis is configured through `clang-tidy.config`, enabling modernisation, readability and performance checks with warnings treated as errors【F:clang-tidy.config†L1-L10】. Running the analyser on `hello.c` reports no diagnostics:

```
clang-tidy hello.c --
```

## ShellCheck

Shell scripts such as `setup.sh` are validated with ShellCheck. The script passes without warnings:

```
shellcheck setup.sh
```

## Compiledb

`compiledb` generates a `compile_commands.json` database for C sources, facilitating indexing and further analysis. Invoking it for the x86-64 Hello World target shows the existing build is up to date:

```
compiledb make hello_x86_64
```

## Doxygen

API documentation is produced with Doxygen using the configuration in `docs/Doxyfile`. The tool requires Graphviz's `dot` for diagrams. After installing the dependency, documentation generation completes successfully:

```
doxygen Doxyfile
```

