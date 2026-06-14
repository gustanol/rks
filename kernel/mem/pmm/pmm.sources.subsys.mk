PMM_SUBSYS_SRC_DIR := $(KERNEL_DIR)/mem/pmm
PMM_SUBSYS_BUILD_DIR := $(BUILD_DIR)/$(PMM_SUBSYS_SRC_DIR)

# Core objects
OBJ-1 += \
    $(PMM_SUBSYS_BUILD_DIR)/core/dispatch.o \
    $(PMM_SUBSYS_BUILD_DIR)/core/pmm.o \
    $(PMM_SUBSYS_BUILD_DIR)/core/wrappers.o

# Backend-specific objects
OBJ-$(COMPILE_PMM_BITMAP_BACKEND) += \
    $(PMM_SUBSYS_BUILD_DIR)/backend/bitmap/bitmap.o

# PMM CFLAGS
PMM_SUBSYS_CFLAGS := \
    -I$(PMM_SUBSYS_SRC_DIR)/include

# Scope flags
$(PMM_SUBSYS_BUILD_DIR)/%.o: \
	CFLAGS += $(PMM_SUBSYS_CFLAGS)
