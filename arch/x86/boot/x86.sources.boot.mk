X86_BOOT_SRC_DIR := $(ARCH_DIR)/boot
X86_BOOT_BUILD_DIR := $(BUILD_DIR)/$(X86_BOOT_SRC_DIR)

# Global linker script
LINKER_SCRIPT := $(X86_BOOT_SRC_DIR)/kernel.ld

# x86 entry object
OBJ-1 += \
	$(X86_BOOT_BUILD_DIR)/entry.o

# Generic bootloader selector
OBJ-1 += \
	$(X86_BOOT_BUILD_DIR)/protocols/loader_parser.o

# Multiboot2 protocol objects
OBJ-$(MULTIBOOT2_SUPPORT) += \
	$(X86_BOOT_BUILD_DIR)/protocols/multiboot2/parsers/framebuffer.o \
	$(X86_BOOT_BUILD_DIR)/protocols/multiboot2/parsers/generic.o \
	$(X86_BOOT_BUILD_DIR)/protocols/multiboot2/parsers/mmap.o
