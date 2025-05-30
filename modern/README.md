# Modern C17 port

This directory contains experimental work to rewrite parts of the original
Harvey OS utilities into standard C17. The goal is to eventually build
portable versions that target both 32‑bit and 64‑bit x86 systems without
relying on the original Plan 9 libraries.

The first utility under modernization is `acd`. The code has been split into
small modules to ease future development:

* `msf.h` – helper for dealing with CD minute/second/frame values.
* `args.c`/`args.h` – minimal command line parsing.
* `acd.c` – main entry point demonstrating the modernized skeleton.

Run `make` in this directory to build for the host architecture. Pass
`ARCH=<target>` to select another architecture or run `make test` to
compile all targets listed in `ARCHS`.
