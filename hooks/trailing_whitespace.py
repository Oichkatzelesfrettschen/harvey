#!/usr/bin/env python3
import sys, pathlib
changed=False
for path in sys.argv[1:]:
    p = pathlib.Path(path)
    if not p.is_file():
        continue
    try:
        text = p.read_text()
    except Exception:
        continue
    lines = text.splitlines()
    new_lines = [line.rstrip(' \t') for line in lines]
    fixed='\n'.join(new_lines)
    if text.endswith('\n'):
        fixed+='\n'
    if fixed != text:
        p.write_text(fixed)
        print(f"Fixed trailing whitespace: {path}")
        changed=True
sys.exit(1 if changed else 0)
