# mk/arch/arm64.mk
# Architecture-specific settings for arm64
CC_FOR_TARGET = aarch64-linux-gnu-gcc
CXX_FOR_TARGET = aarch64-linux-gnu-g++
TARGET_ARCH_CFLAGS ?= -march=armv8-a -mtune=cortex-a72
TARGET_ARCH_LDFLAGS ?=
