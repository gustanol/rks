# Sub-directories
X86_BOOT_SRC_DIR := $(ARCH_DIR)/boot
X86_CPU_SUBSYS_SRC_DIR := $(ARCH_DIR)/cpu
X86_INTERRUPT_SRC_DIR := $(ARCH_DIR)/interrupt

# x86 linker script
LINKER_SCRIPT := $(X86_BOOT_SRC_DIR)/kernel.ld

# x86 build directory
X86_BUILD_DIR := $(BUILD_DIR)/$(ARCH_DIR)

# Main includes
-include $(X86_BOOT_SRC_DIR)/boot.mk
-include $(X86_CPU_SUBSYS_SRC_DIR)/cpu.mk
-include $(X86_INTERRUPT_SRC_DIR)/interrupt.mk

# Backends
VMM_BACKEND := $(ARCH_DIR)/mem/vmm/paging/paging.mk

# Extra flags
X86_CFLAGS := \
	-I$(ARCH_DIR)/include

# Scope flags
$(X86_BUILD_DIR)/%.o: \
	CFLAGS += $(X86_CFLAGS)
