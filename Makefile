# Root Makefile to drive builds in the modern directory
ARCH ?= x86_64
ARCHS ?= i386 x86_64
CC ?= clang
CXX ?= clang++
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror

# Build the modern utilities for the selected ARCH
.PHONY: all test clean
all:
	$(MAKE) -C modern ARCH=$(ARCH) CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" all

test:
	$(MAKE) -C modern ARCHS="$(ARCHS)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" test

clean:
	$(MAKE) -C modern clean
