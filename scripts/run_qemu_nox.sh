#!/usr/bin/env bash
# Run a Harvey kernel in QEMU using a text console.
set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 <harvey_kernel> [extra qemu args...]" >&2
    exit 1
fi

KERNEL="$1"
shift

exec qemu-system-x86_64 -m 512M -nographic -kernel "$KERNEL" "$@"
