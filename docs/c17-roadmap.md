# C17 Migration Roadmap

This document outlines a staged approach for converting the Harvey `acd` sources from their original K&R/Plan9 style C into strict C17. The repository currently contains eight C source files in `acme/bin/source/acd`.

## 1. Initial Survey

Run `scripts/analyze_repo.py` to detect Plan9 constructs. The current output identifies uses of `Channel`, `dial()` and custom formatting functions such as `fmtprint`.

## 2. Batch Refactoring

Refactor the code in batches of up to 30 files. For each batch:

1. Replace Plan9 headers with standard equivalents.
2. Translate Plan9 concurrency primitives to POSIX threads.
3. Remove K&R declarations and adopt modern prototypes.
4. Apply `clang-format` to enforce the style defined in `.clang-format`.
5. Compile with `-std=c17` and resolve warnings.

## 3. Validation

Use `pre-commit` hooks to run `clang-format`, `clang-tidy` and compile the updated sources. Continuous integration should ensure no regressions as the migration progresses.

## 4. Tracking Progress

Record remaining Plan9 constructs in `docs/acd-plan9-audit.md` and update it after each batch is refactored. The modernization is considered complete when the scan reports zero Plan9 patterns and the code builds cleanly with C17.
