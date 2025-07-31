#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Parsed command line arguments.
 *
 * The @c verbose flag is set when the @c -v option is provided and
 * @c device holds the target CD device name.
 */
typedef struct {
    bool verbose;       /**< Whether verbose output was requested. */
    const char *device; /**< Name of the CD device to operate on. */
} CmdArgs;

/**
 * @brief Parse command line arguments.
 *
 * @param argc Number of command line arguments.
 * @param argv Argument vector.
 * @return Parsed command line options.
 */
CmdArgs parse_args(int32_t argc, char **argv);
