X86_CPU_SUBSYS_SRC_DIR := $(ARCH_DIR)/cpu
X86_CPU_SUBSYS_BUILD_DIR := $(BUILD_DIR)/$(X86_CPU_SUBSYS_SRC_DIR)

# Core objects
OBJ-1 += \
	$(X86_CPU_SUBSYS_BUILD_DIR)/cpu_preflight.o \
	$(X86_CPU_SUBSYS_BUILD_DIR)/transition_long.o

# Descriptor objects
OBJ-1 += \
	$(X86_CPU_SUBSYS_BUILD_DIR)/descriptors/descriptors.o \
	$(X86_CPU_SUBSYS_BUILD_DIR)/descriptors/idt.o \
	$(X86_CPU_SUBSYS_BUILD_DIR)/descriptors/tss.o

# GDT-specific objects
OBJ-1 += \
	$(X86_CPU_SUBSYS_BUILD_DIR)/descriptors/gdt/boot_gdt.o \
	$(X86_CPU_SUBSYS_BUILD_DIR)/descriptors/gdt/long_gdt.o

# CPU CFLAGS
X86_CPU_SUBSYS_CFLAGS := \
	-I$(X86_CPU_SUBSYS_SRC_DIR)/include

# Scope flags
$(X86_CPU_SUBSYS_BUILD_DIR)/%.o: \
	CFLAGS += $(X86_CPU_SUBSYS_CFLAGS)
