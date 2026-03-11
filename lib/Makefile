lib_dir := lib
lib_build_dir := $(root_build_dir)/$(lib_dir)

# Objects
obj-1 += $(lib_build_dir)/math.o
obj-1 += $(lib_build_dir)/string.o
obj-1 += $(lib_build_dir)/bitwise.o
obj-1 += $(lib_build_dir)/boot/multiboot2.o

$(lib_build_dir)/%.o: $(lib_dir)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
