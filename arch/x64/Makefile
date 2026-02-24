x64_dir := arch/x64
x64_build_dir := $(root_build_dir)/$(x64_dir)

# 32-bit init code
obj-1 += $(x64_build_dir)/boot/protected/entry.o
obj-1 += $(x64_build_dir)/boot/protected/gdt.o
obj-1 += $(x64_build_dir)/boot/protected/setup32.o

# 64-bit long mode objects
obj-1 += $(x64_build_dir)/boot/long/transition.o

obj-1 += $(x64_build_dir)/cpu/descriptor.o
obj-1 += $(x64_build_dir)/cpu/idt.o

obj-1 += $(x64_build_dir)/interrupt/isr.o
obj-1 += $(x64_build_dir)/interrupt/handler.o

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
