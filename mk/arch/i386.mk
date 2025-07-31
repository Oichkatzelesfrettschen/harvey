# mk/arch/i386.mk
# Architecture-specific settings for i386
TARGET_ARCH_CFLAGS ?= -m32
TARGET_ARCH_LDFLAGS ?= -m32
CC_FOR_TARGET = $(CC)
CXX_FOR_TARGET = $(CXX)
