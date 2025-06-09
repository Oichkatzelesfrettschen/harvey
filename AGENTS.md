# Guidelines for Contributors

All code modifications must embrace modern programming paradigms and idiomatic expressions.  When modifying existing files:

- Refactor functions into clear, decomposed units following mathematical reasoning.
- Flatten complex control flow and favor composition over nesting.
- Unroll loops only when it simplifies reasoning and performance without sacrificing clarity.
- Factor out shared logic so each function has a single responsibility.
- Ensure all functions carry Doxygen comments documenting parameters, return values and side effects.
- Adopt the most modern language features available for the file's language standard.

Additionally, keep code formatting clean and consistent. Use `clang-format` or
equivalent tooling where applicable. Install the git hooks with
`pre-commit install` after running `setup.sh` and execute `pre-commit` before
pushing any commits so the automated checks can format code and validate
documentation. Shell scripts must pass `shellcheck` and C sources should be
indexable via `compiledb` so static analysis tools can function correctly.
