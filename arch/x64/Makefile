x64_dir := arch/x64
x64_build_dir := $(root_build_dir)/$(x64_dir)

# Bootloader-specific code
obj-1 += $(x64_build_dir)/boot/multiboot2/header.o
obj-1 += $(x64_build_dir)/boot/multiboot2/parser.o

# 32-bit generic code
obj-1 += $(x64_build_dir)/boot/common/gdt.o
obj-1 += $(x64_build_dir)/boot/common/setup32.o

# 64-bit early boot long mode objects
obj-1 += $(x64_build_dir)/boot/common/transition_64.o
obj-1 += $(x64_build_dir)/boot/loader_parser.o
obj-1 += $(x64_build_dir)/boot/early_alloc.o
obj-1 += $(x64_build_dir)/boot/entry.o
obj-1 += $(x64_build_dir)/init.o

# Descriptors
obj-1 += $(x64_build_dir)/cpu/gdt.o
obj-1 += $(x64_build_dir)/cpu/tss.o
obj-1 += $(x64_build_dir)/cpu/idt.o

# Interrupts
obj-1 += $(x64_build_dir)/interrupt/isr.o
obj-1 += $(x64_build_dir)/interrupt/handler.o

obj-1 += $(x64_build_dir)/mem/paging_vmm.o
obj-1 += $(x64_build_dir)/mem/paging.o

# Build rules for Assembly
$(x64_build_dir)/%.o: $(x64_dir)/%.S
	@mkdir -p $(dir $@)
	@echo "AS  $<"
	@$(AS) $(ASFLAGS) $< -o $@

# Build rules for C
$(x64_build_dir)/%.o: $(x64_dir)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
