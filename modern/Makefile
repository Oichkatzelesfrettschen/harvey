CC = clang
ARCH ?= x86_64
ARCHS ?= i386 x86_64
CFLAGS += -std=c23 -Wall -Wextra -Wpedantic -Werror -pthread

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

SRC = acd_c23.c args.c spinlock.c
OUT = acd-$(ARCH)

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

test:
	@for a in $(ARCHS); do \
		$(MAKE) ARCH=$$a clean all ; \
	done

clean:
	rm -f acd-*
