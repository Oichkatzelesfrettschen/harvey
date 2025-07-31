# Harvey OS C17 Migration Guide

This document provides practical guidance, code examples, and validation strategies for migrating Harvey OS components from legacy Plan 9 C (often K&R-like) to modern C17. Adherence to this guide is crucial for improving code portability, safety, and maintainability across the OS.

Refer to the `ARCHITECTURAL_MANIFESTO.md` for the overall C17 adoption strategy and `docs/systematic-modernization.md` for the broader modernization protocol (unless its C17 content is fully superseded by this guide).

## 1. Core Principles of C17 Migration

*   **Clarity and Correctness:** The primary goal is to produce C17 code that is easier to understand, less prone to errors, and behaves correctly according to both C17 standards and Harvey OS requirements.
*   **Incremental Changes:** Migrate code in manageable, testable chunks.
*   **Standard Compliance:** Code must compile cleanly with `-std=c17 -Wall -Wextra -pedantic -Werror` (or equivalent strict flags for the project's chosen compiler).
*   **Leverage C17 Features:** Utilize C17 features where they improve safety or clarity (e.g., `[[nodiscard]]`, `[[fallthrough]]`, `_Generic` for type-safe macros if appropriate, designated initializers).

## 2. Common Migration Patterns & Examples

This section provides a library of common patterns encountered when migrating from Plan 9 C to C17, along with recommended transformations and validation approaches.

### 2.1. Function Declarations and Definitions (K&R to Prototypes)

*   **Old (K&R Style):**
    ```c
    // In a header (e.g., u.h or local .h)
    // int func();

    // In a .c file
    // int
    // func(a, b)
    //   char *a;
    //   long b;
    // { /* ... */ }
    ```
*   **New (C17 Prototypes):**
    ```c
    // In the .h file (ensure header guards)
    #ifndef MY_HEADER_H
    #define MY_HEADER_H

    #include <stddef.h> // For size_t etc.
    #include <stdint.h> // For int32_t etc.

    int my_func(const char *a, int32_t b); // Use specific types

    #endif // MY_HEADER_H

    // In the .c file
    #include "my_header.h" // Or relevant system header

    int my_func(const char *a, int32_t b) {
        // ... implementation ...
        return 0; // Example
    }
    ```
*   **Validation:**
    *   Compiler warnings/errors for mismatched declarations/definitions.
    *   Ensure all call sites are updated if argument types implicitly changed (e.g., `int` to `long` was common). Static analysis can help.

### 2.2. Memory Allocation (e.g., `smalloc`, `mallocz`)

*   **Old (Plan 9 `smalloc`):**
    ```c
    // void* p = smalloc(1024); // sysfatal on error
    ```
*   **New (C17 with Harvey Compatibility/Error Handling):**
    ```c
    #include <stdlib.h> // For malloc, free
    #include <stdio.h>  // For perror or custom error logging
    // #include "harvey_error.h" // Hypothetical header for harvey_sysfatal

    [[nodiscard]] void* safe_malloc(size_t n) {
        void *p = malloc(n);
        if (!p && n > 0) { // Check n > 0 because malloc(0) can return NULL or a unique pointer
            // Option 1: Use a Harvey-specific fatal error handler if one exists and is appropriate here
            // #ifdef HARVEY_COMPAT_SYSFATAL
            //   harvey_sysfatal("out of memory");
            // #else
            // Option 2: Standard C error handling (preferred for portable modules)
            perror("safe_malloc: malloc failed");
            // Consider exit(EXIT_FAILURE) only if truly unrecoverable at this level
            // For libraries, usually better to return NULL and let caller decide.
            // #endif
        }
        return p;
    }

    // Usage:
    // void* p = safe_malloc(1024);
    // if (!p) { /* handle error appropriately */ }
    ```
    *   **Note on `mallocz` (zeroed memory):** Replace with `calloc` or `malloc` followed by `memset(p, 0, n)`.
        ```c
        // Old: void* p = mallocz(1024, 1);
        // New:
        void* p = calloc(1, 1024); // If elements are 1 byte and count is 1024
        if (!p) { /* handle error */ }
        ```
*   **Validation:**
    *   Ensure `NULL` checks are present after allocation.
    *   Memory leak analysis (e.g., Valgrind, ASan).
    *   Test error paths by simulating allocation failures (if possible via test harnesses).

### 2.3. String Manipulation (e.g., `strecpy`, Plan 9 `fmt` routines)

*   **`strecpy(dst, end, src)`:** A safe copy function.
    *   **New (C17):** Use `strncpy` with care (it might not null-terminate), or preferably `snprintf`.
        ```c
        // char dest[100];
        // char *end_ptr = strecpy(dest, dest + sizeof(dest), "hello");

        // New using snprintf (safer):
        // snprintf(dest, sizeof(dest), "%s", "hello");
        // To replicate 'end_ptr' behavior (pointer to null terminator):
        // size_t len = strlen(src);
        // if (len < size_of_dest_buffer) {
        //    memcpy(dest, src, len + 1); // +1 for null terminator
        //    end_ptr = dest + len;
        // } else { /* handle truncation */ }
        ```
        A more direct replacement for `strecpy`'s specific behavior (copy up to `end-1` and null terminate) might be a custom helper or careful `strncpy` usage:
        ```c
        char* safe_strecpy(char *dst, char *end, const char *src) {
            size_t N = end - dst;
            if (N == 0) return dst;
            strncpy(dst, src, N - 1);
            dst[N - 1] = '\0'; // Ensure null termination
            return dst + strlen(dst); // Points to the new null terminator
        }
        ```
*   **Plan 9 `fmt` routines (`fmtprint`, `sprint`, etc.):**
    *   **New (C17):** Replace with `snprintf`, `fprintf`, `printf` family. Pay attention to format string differences.
    *   Custom Plan 9 format verbs (e.g., `%H` for Ethernet addresses, `%lld` in Plan 9 C might be standard `%lld` or `%I64d` depending on compiler/vintage) need careful translation to standard C format specifiers or custom formatting functions.
*   **Validation:**
    *   Test for buffer overflows (ASan, Valgrind).
    *   Verify correct output formatting, especially for edge cases.
    *   Ensure null termination.

### 2.4. Plan 9 Specific Types (e.g., `uchar`, `ushort`, `ulong`, `vlong`)
*   **New (C17):** Replace with standard types from `<stdint.h>` for fixed-width integers (`uint8_t`, `uint16_t`, `uint32_t`, `int64_t`, `uint64_t`) or standard C types (`unsigned char`, `unsigned short`, `unsigned long`, `long long`) where exact width is not critical but general size is.
*   **Validation:** Compiler checks. Ensure no accidental sign changes or truncation if sizes differ subtly across platforms for generic types like `long`.

### 2.5. Error Handling (e.g., `errstr`, `sysfatal`)
*   **`errstr` for setting global error string, `rerrstr` for retrieving:**
    *   **New (C17):** For library code, prefer returning error codes and letting callers use `strerror(errno)` or custom error retrieval functions. Avoid global error variables in new code where possible. If mimicking, thread-local storage for error strings might be an option.
*   **`sysfatal(format, ...)`:**
    *   **New (C17):** As shown in `safe_malloc`, this depends on context. For critical unrecoverable errors in applications, `fprintf(stderr, ...); exit(EXIT_FAILURE);` is standard. Libraries should not typically call `exit()`.
*   **Validation:** Test error paths and ensure appropriate error reporting or propagation.

### 2.6. Concurrency (e.g., Plan 9 `rendezvous`, `proc`, `threadmain`)
*   **New (C17):** This is a major refactoring area.
    *   Plan 9 `proc` for new processes: Use `fork`/`exec` (if POSIX compatibility is a goal) or platform-specific process creation.
    *   Plan 9 threads/`rendezvous`: Migrate to `pthreads` (if POSIX compatibility is a goal). This involves careful redesign of synchronization logic.
*   **Validation:** Extensive testing for race conditions (TSan), deadlocks, and correct synchronization. This often requires new test harnesses. (This is a complex topic; further details might be in a dedicated threading guide).

## 3. Validation Test Harness for Migration Patterns

For each significant migration pattern or refactored module:
1.  **Unit Tests:** Create fine-grained unit tests that specifically exercise the migrated code.
    *   Focus on inputs that might have caused issues in the old code (e.g., boundary conditions, null inputs, large inputs).
    *   Verify outputs against expected values.
    *   Use a standard C unit testing framework (e.g., CUnit, CMocka, or even simple assert-based tests).
2.  **Behavioral Equivalence (where applicable):** If the refactored code is intended to be a drop-in replacement, design tests that verify its behavior is identical to the old code for a range of inputs. This might involve running both old and new versions side-by-side in a test harness if feasible.
3.  **Code Coverage:** Aim for high code coverage for migrated components to ensure all paths are tested.
4.  **Static and Dynamic Analysis:**
    *   Run `clang-tidy` and other static analyzers on the new code.
    *   Use Valgrind, ASan, UBSan during testing to catch memory errors, undefined behavior.
    *   Use TSan for threaded components.

## 4. Integration with Broader Modernization Efforts

*   Refer to `docs/systematic-modernization.md` for the overall process of component identification, prioritization, and tracking of the C17 migration.
*   This guide provides the "how-to" for the code-level changes. The ARB will determine which components are modernized and when.

This guide is a living document and will be updated as new patterns are identified and best practices evolve.
