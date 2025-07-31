#===============================================================================
# Makefile for modern/ utilities — builds the `acd` example for multiple arches
#
# Usage:
#   make                # builds for default ARCH (x86_64)
#   make ARCH=i386      # builds for 32-bit x86
#   make ARCH=arm64     # builds for 64-bit ARM
#   make test           # clean & build for all ARCHS
#   make clean          # remove all built binaries
#===============================================================================

#–– User‐configurable variables ––#
# C compiler (overridden by arch-specific include, if provided)
CC    ?= clang
CXX   ?= clang++
# Target architecture (one of $(ARCHS))
ARCH  ?= x86_64
# List of all supported architectures
ARCHS := i386 x86_64 arm64

#–– Determine project root and include arch settings ––#
# assume this Makefile is in $(HARVEY_ROOT)/modern/
HARVEY_ROOT := $(CURDIR)/..

# Select final TARGET_ARCH (alias for ARCH)
TARGET_ARCH ?= $(ARCH)
# Pull in architecture‐specific settings:
#   mk/arch/$(TARGET_ARCH).mk should define:
#     CC_FOR_TARGET, CXX_FOR_TARGET,
#     TARGET_ARCH_CFLAGS, TARGET_ARCH_LDFLAGS, etc.
-include $(HARVEY_ROOT)/mk/arch/$(TARGET_ARCH).mk

# Override compilers if arch makefile provides them
ifndef CC_FOR_TARGET
  CC_FOR_TARGET := $(CC)
endif
ifndef CXX_FOR_TARGET
  CXX_FOR_TARGET := $(CXX)
endif
CC  := $(CC_FOR_TARGET)
CXX := $(CXX_FOR_TARGET)

#–– Common flags ––#
CFLAGS   ?= -Wall -Wextra -Wpedantic -Werror -std=c17 -pthread
CXXFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c++17 -pthread
LDFLAGS  ?=

# Append any arch-specific flags from the included mk file
CFLAGS   += $(TARGET_ARCH_CFLAGS)
CXXFLAGS += $(TARGET_ARCH_CXXFLAGS)
LDFLAGS  += $(TARGET_ARCH_LDFLAGS)

#–– Sources & output ––#
SRC := acd.c args.c spinlock.c
OUT := acd-$(TARGET_ARCH)

#–– Phony targets ––#
.PHONY: all test clean

# Default: build for the selected target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build & then clean/build each supported ARCH
test:
	@for a in $(ARCHS); do \
	  echo "=== Building for $$a ==="; \
	  $(MAKE) ARCH=$$a clean all; \
	done

# Remove all produced binaries
clean:
	@rm -f $(addprefix acd-,$(ARCHS))

# EOF