ARCH ?= x86_64
ARCHS ?= i386 x86_64
CFLAGS ?=
CFLAGS += -std=c23 -Wall -Wextra -Wpedantic -Werror

all:
	$(MAKE) -C modern ARCH=$(ARCH) CFLAGS="$(CFLAGS)" all

test:
	$(MAKE) -C modern ARCHS="$(ARCHS)" CFLAGS="$(CFLAGS)" test

clean:
	$(MAKE) -C modern clean
