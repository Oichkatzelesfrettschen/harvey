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
    if not text.endswith('\n'):
        p.write_text(text + '\n')
        print(f"Added newline at end of file: {path}")
        changed=True
sys.exit(1 if changed else 0)
