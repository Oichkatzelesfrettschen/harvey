# Modern C23 port

This directory contains experimental work to rewrite parts of the original
Harvey OS utilities into standard C23. The goal is to eventually build
portable versions that target both 32‑bit and 64‑bit x86 systems without
relying on the original Plan 9 libraries.

The first utility under modernization is `acd`. The code has been split into
small modules to ease future development:

* `msf.h` – helper for dealing with CD minute/second/frame values.
* `args.c`/`args.h` – minimal command line parsing.
* `acd_c23.c` – main entry point demonstrating the modernized skeleton.

Run the root CMake build to compile the `acd` utility:

```bash
cmake -S .. -B ../build
cmake --build ../build --target acd
```
