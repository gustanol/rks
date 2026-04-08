# Bootloader-specific code
OBJ-$(CFG_MULTIBOOT2_LOADER) += $(ARCH_BUILD_DIR)/boot/multiboot2/header.o
OBJ-$(CFG_MULTIBOOT2_LOADER) += $(ARCH_BUILD_DIR)/boot/multiboot2/parser.o

# 32-bit generic code
OBJ-1 += $(ARCH_BUILD_DIR)/boot/common/gdt.o
OBJ-1 += $(ARCH_BUILD_DIR)/boot/common/setup32.o

# 64-bit early boot long mode objects
OBJ-1 += $(ARCH_BUILD_DIR)/boot/common/transition_64.o
OBJ-1 += $(ARCH_BUILD_DIR)/boot/loader_parser.o
OBJ-1 += $(ARCH_BUILD_DIR)/boot/early_alloc.o
OBJ-1 += $(ARCH_BUILD_DIR)/boot/entry.o
OBJ-1 += $(ARCH_BUILD_DIR)/init.o

# Descriptors
OBJ-1 += $(ARCH_BUILD_DIR)/cpu/gdt.o
OBJ-1 += $(ARCH_BUILD_DIR)/cpu/tss.o
OBJ-1 += $(ARCH_BUILD_DIR)/cpu/idt.o

# Interrupts
OBJ-1 += $(ARCH_BUILD_DIR)/interrupt/isr.o
OBJ-1 += $(ARCH_BUILD_DIR)/interrupt/handlers/generic.o
OBJ-1 += $(ARCH_BUILD_DIR)/interrupt/handlers/page_fault.o

OBJ-1 += $(ARCH_BUILD_DIR)/mem/paging_vmm.o
OBJ-1 += $(ARCH_BUILD_DIR)/mem/paging.o
