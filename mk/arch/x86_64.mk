# mk/arch/x86_64.mk
# Architecture-specific settings for x86_64
TARGET_ARCH_CFLAGS ?= -m64
TARGET_ARCH_LDFLAGS ?= -m64
CC_FOR_TARGET = $(CC)
CXX_FOR_TARGET = $(CXX)
