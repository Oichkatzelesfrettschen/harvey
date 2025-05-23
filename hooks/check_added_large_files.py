#!/usr/bin/env python3
import os, sys
MAX_SIZE = 5 * 1024 * 1024
ret = 0
for path in sys.argv[1:]:
    if not os.path.isfile(path):
        continue
    if os.path.getsize(path) > MAX_SIZE:
        print(f"{path}: file exceeds {MAX_SIZE} bytes")
        ret = 1
sys.exit(ret)
