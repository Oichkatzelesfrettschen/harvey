# Makefile for modern utilities. Builds the acd example.
CC ?= clang
CXX ?= clang++
ARCH ?= x86_64
ARCHS ?= i386 x86_64
# Common compiler flags for C sources
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c17 -pthread
# Common compiler flags for C++ sources
CXXFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c++17 -pthread

ifeq ($(ARCH),i386)
ARCH_FLAGS = -m32
else ifeq ($(ARCH),x86_64)
ARCH_FLAGS = -m64
else ifeq ($(ARCH),i8086)
ARCH_FLAGS = -m16
else
ARCH_FLAGS =
endif

CFLAGS += $(ARCH_FLAGS)
CXXFLAGS += $(ARCH_FLAGS)

SRC = acd.c args.c spinlock.c
OUT = acd-$(ARCH)

# Default target builds the binary for the selected ARCH
.PHONY: all test clean
all: $(OUT)

$(OUT): $(SRC)
	        $(CC) $(CFLAGS) $(SRC) -o $@

test:
	        @for a in $(ARCHS); do \
	                $(MAKE) ARCH=$$a clean all ; \
        done

clean:
		rm -f acd-*
