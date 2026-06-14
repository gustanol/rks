# Toolchain
# =========
CC ?= gcc
AS ?= as
LD ?= ld
OBJCOPY ?= objcopy
# =========

# Architectures
# =============
ARCHS := $(notdir $(wildcard arch/*))

# Cached files
CACHE_DIR := cache
BUILD_CONFIG := $(COMPILATION_DIR)/.build.config
BUILD_SOURCES := $(CACHE_DIR)/.build.sources
ARCH_CACHE := $(CACHE_DIR)/.arch

ifneq ($(wildcard $(ARCH_CACHE)),)
	include $(ARCH_CACHE)

	ARCH_DIR := arch/$(ARCH)
	ARCH_BUILD_FILE := $(ARCH_DIR)/arch.mk
	ARCH_INCLUDES_DIR := $(ARCH_DIR)/include/$(ARCH)

	-include $(ARCH_BUILD_FILE)
endif

# Project directories
# ===================
DRIVERS_DIR := drivers
KERNEL_DIR := kernel
LIB_DIR := lib

# Build directories/files
BUILD_DIR := build
DEBUG_DIR := debug

# Include directories
INCLUDES_DIR := include
ARCH_GENERIC_INCLUDES := $(INCLUDES_DIR)/arch
# ===================

# Defines
# =======
OBJ-1 :=
OBJ-0 :=

ifneq ($(wildcard $(BUILD_SOURCES)),)
	include $(BUILD_SOURCES)
endif

# Debug directive
DEBUG ?= 0

# Binaries
BASE_NAME := rks
ELF_FILE := $(BUILD_DIR)/$(BASE_NAME).elf
BIN_NAME := $(BASE_NAME).bin
FINAL_BIN := $(BUILD_DIR)/$(BIN_NAME)

# Symbol map that will be gotten from the ELF file
DEBUG_MAP := $(DEBUG_DIR)/$(BASE_NAME).debug

# External
CONFIG_HEADER := $(INCLUDES_DIR)/rks/autoconf.h
# =======
