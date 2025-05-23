#!/usr/bin/env python3
import sys, yaml, pathlib
ret=0
for path in sys.argv[1:]:
    p=pathlib.Path(path)
    if not p.is_file():
        continue
    try:
        with open(p, 'r') as f:
            yaml.safe_load(f)
    except Exception as e:
        print(f"{path}: YAML parse error: {e}")
        ret=1
sys.exit(ret)
