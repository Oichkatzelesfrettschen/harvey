#!/usr/bin/env bash
set -euo pipefail
CONFIG_FILE="$(git rev-parse --show-toplevel)/clang-tidy.config"
for f in "$@"; do
  clang-tidy --config-file="$CONFIG_FILE" "$f" -- -std=c23
done
