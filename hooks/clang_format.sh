#!/usr/bin/env bash
# Format C source and header files using clang-format.
# The script exits with status 1 if any file was modified by formatting.

set -euo pipefail

# Track whether clang-format changed any file.
changed=0

for file in "$@"; do
    # Skip paths that are not regular files.
    if [ ! -f "$file" ]; then
        continue
    fi

    # Apply in-place formatting.
    clang-format -i "$file"
done

# Check if formatting altered files by inspecting git diff output.
if ! git diff --quiet -- "$@"; then
    changed=1
fi

exit $changed
