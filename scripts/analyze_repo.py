import os
import re
from collections import defaultdict

ROOT = os.path.join(os.path.dirname(__file__), '..')
SRC_DIR = os.path.join(ROOT, 'acme', 'bin', 'source', 'acd')

# Patterns indicating Plan9 constructs
PATTERNS = [
    r'\bthread_',
    r'\bBiobuf',
    r'\bChannel',
    r'\bqlock',
    r'fmtprint',
    r'Brdline',
    r'dial\(',
]

def classify_file(path):
    """Return a tuple of (size, file_type)"""
    with open(path, 'rb') as f:
        data = f.read()
    if b'\0' in data:
        return len(data), 'binary'
    text = data.decode('utf-8', errors='replace')
    if text.startswith('#!/bin/rc'):
        return len(data), 'rc script'
    if re.search(r'#include', text):
        return len(data), 'C source'
    return len(data), 'text'


def scan_plan9_usage(path):
    """Return list of Plan9 related patterns found in file."""
    with open(path, 'r', errors='replace') as f:
        text = f.read()
    found = [p for p in PATTERNS if re.search(p, text)]
    return found


def main():
    summary = []
    for fname in sorted(os.listdir(SRC_DIR)):
        full = os.path.join(SRC_DIR, fname)
        if os.path.isfile(full):
            size, ftype = classify_file(full)
            plan9 = scan_plan9_usage(full) if ftype == 'C source' else []
            summary.append((fname, size, ftype, plan9))
    for name, size, ftype, plan9 in summary:
        line = f"{name:10} {size:6d} bytes {ftype:9}"
        if plan9:
            line += " plan9: " + ', '.join(plan9)
        print(line)

if __name__ == '__main__':
    main()
