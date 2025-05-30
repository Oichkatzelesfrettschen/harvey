#!/usr/bin/env bash
# Create a configuration database using configuredb.
# Exit status 1 indicates the file changed.

set -euo pipefail

# Generate configuration database.
configuredb >/dev/null

# Check for updates.
if git diff --quiet -- config.db; then
    exit 0
else
    echo "config.db updated"
    exit 1
fi
