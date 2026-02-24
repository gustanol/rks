video_drivers_dir := drivers/video
video_drivers_build_dir:= $(root_build_dir)/$(video_drivers_dir)

obj-$(CFG_CONSOLE_VGA) += $(video_drivers_build_dir)/vga/vga.o

$(video_drivers_build_dir)/%.o: $(video_drivers_dir)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
