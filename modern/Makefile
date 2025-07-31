# Makefile for modern utilities. Builds the acd example.
CC ?= clang
CXX ?= clang++
ARCH ?= x86_64
ARCHS ?= i386 x86_64 arm64 # Added arm64

# Root directory of the Harvey OS source tree (assume Makefile is in modern/)
HARVEY_ROOT := $(CURDIR)/..

# Include architecture-specific settings
# Check if TARGET_ARCH is set, if not, default to ARCH
TARGET_ARCH ?= $(ARCH)
include $(HARVEY_ROOT)/mk/arch/$(TARGET_ARCH).mk

# Override CC and CXX if specified by the arch makefile
CC := $(CC_FOR_TARGET)
CXX := $(CXX_FOR_TARGET)

# Common compiler flags for C sources
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c17 -pthread
# Common compiler flags for C++ sources
CXXFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c++17 -pthread
LDFLAGS ?=

SRC = acd.c args.c spinlock.c
OUT = acd-$(TARGET_ARCH)

# Default target builds the binary for the selected ARCH
.PHONY: all test clean
all: $(OUT)

$(OUT): $(SRC)
	        $(CC) $(CFLAGS) $(TARGET_ARCH_CFLAGS) $(SRC) -o $@ $(LDFLAGS) $(TARGET_ARCH_LDFLAGS)

test:
	        @for a in $(ARCHS); do 	                $(MAKE) TARGET_ARCH=$$a clean all ;         done

clean:
		rm -f acd-*
