KERNEL_LINKER_SCRIPT = $(ARCH_DIR)/boot/kernel.ld

# Boot objects ==================================================
# Protocols --------------------------------------------------

# Multiboot2
OBJ-$(CFG_MULTIBOOT2_LOADER) += \
	$(ARCH_BUILD_DIR)/boot/protocols/multiboot2/header.o \
	$(ARCH_BUILD_DIR)/boot/protocols/multiboot2/parser.o

# Generic parser
OBJ-1 += $(ARCH_BUILD_DIR)/boot/protocols/loader_parser.o
# ------------------------------------------------------------

# Generic early objects ------------------
OBJ-1 += \
	$(ARCH_BUILD_DIR)/boot/entry.o \
	$(ARCH_BUILD_DIR)/boot/arch_init.o
# ----------------------------------------

# Common bootloader-independant objects ---------------
OBJ-1 += \
	$(ARCH_BUILD_DIR)/boot/common/boot_gdt.o \
	$(ARCH_BUILD_DIR)/boot/common/cpu_preflight.o \
	$(ARCH_BUILD_DIR)/boot/common/transition_long.o \
	$(ARCH_BUILD_DIR)/boot/common/higher_half.o
# -----------------------------------------------------
# ==============================================================

# CPU-specific objects ==============
OBJ-1 += \
	$(ARCH_BUILD_DIR)/cpu/gdt.o \
	$(ARCH_BUILD_DIR)/cpu/tss.o \
	$(ARCH_BUILD_DIR)/cpu/idt.o
# ===================================

# Interrupts ============================================
OBJ-1 += \
	$(ARCH_BUILD_DIR)/interrupt/isr.o \
	$(ARCH_BUILD_DIR)/interrupt/handlers/generic.o \
	$(ARCH_BUILD_DIR)/interrupt/handlers/page_fault.o
# =======================================================

# Memory ===================================
OBJ-1 += \
	$(ARCH_BUILD_DIR)/mem/early_alloc.o \
	$(ARCH_BUILD_DIR)/mem/paging_vmm.o \
	$(ARCH_BUILD_DIR)/mem/paging.o
# ==========================================
