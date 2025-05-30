#!/usr/bin/env bash
# Generate a compile_commands.json using compiledb.
# If the database changes, exit with status 1 so pre-commit can fail.

set -euo pipefail

# Run compiledb to capture build commands.
compiledb make >/dev/null

# Check for modifications.
if git diff --quiet -- compile_commands.json; then
    exit 0
else
    echo "compile_commands.json updated"
    exit 1
fi
