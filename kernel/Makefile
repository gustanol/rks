kernel_dir := kernel
kernel_build_dir := $(root_build_dir)/$(kernel_dir)

# Kernel objects
obj-1 += $(kernel_build_dir)/init/main.o
obj-1 += $(kernel_build_dir)/console/console.o
obj-$(CFG_KLOG) += $(kernel_build_dir)/klog/log.o
obj-$(CFG_KLOG) += $(kernel_build_dir)/klog/print.o
obj-1 += $(kernel_build_dir)/mem/pmm.o
obj-1 += $(kernel_build_dir)/boot/multiboot2.o

$(kernel_build_dir)/%.o: $(kernel_dir)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
