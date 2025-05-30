# acd Source Analysis

This document summarizes the current state of the original `acd` sources and
highlights remaining Plan9 dependencies detected by `scripts/analyze_repo.py`.

The script scans each file in `acme/bin/source/acd` and reports its type and
any Plan9 specific constructs.

```bash
$ python3 scripts/analyze_repo.py
```

Typical output resembles:

```
README       1034 bytes text
access       9785 bytes text
acd.h        4077 bytes C source  plan9: \bBiobuf, \bChannel
cddb.c       4638 bytes C source  plan9: \bBiobuf, Brdline, dial(
main.c       2986 bytes C source  plan9: \bChannel
mmc.c        6100 bytes C source  plan9: fmtprint
```

The presence of constructs like `Channel`, `Biobuf`, `dial()` and Plan9 style
formatting functions (`fmtprint`, `Brdline`) indicates incomplete portability.
To finish the translation to C17/C++17 we still need to:

- Provide real replacements or rewrites for Plan9 I/O (`Biobuf`, `Binit`,
  `Brdline`), network access via `dial`, and the thread API.
- Replace channel-based concurrency with POSIX threads or C++17 concurrency
  features.
- Implement a modern formatting layer so that old `Fmt` usage can map to
  `printf` or `<format>` utilities.

Running the script regularly helps track progress as more Plan9 constructs are
eliminated.
