# Modern C23 port

This directory contains experimental work to rewrite parts of the original
Harvey OS utilities into standard C23. The goal is to eventually build
portable versions that target both 32‑bit and 64‑bit x86 systems without
relying on the original Plan 9 libraries.

Currently the implementation only provides a small skeleton for the
`acd` CD‑player tool. The code is not feature complete and serves merely as a
starting point for future modernization efforts.
