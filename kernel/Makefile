KERNEL_DIR := kernel
KERNEL_BUILD_DIR := $(ROOT_BUILD_DIR)/$(KERNEL_DIR)

KERNEL_C_SOURCES := $(shell find $(KERNEL_DIR) -name '*.c')
KERNEL_C_OBJECTS := $(patsubst $(KERNEL_DIR)/%.c,$(KERNEL_BUILD_DIR)/%.o,$(KERNEL_C_SOURCES))
KERNEL_OBJECTS := $(KERNEL_C_OBJECTS)
ALL_OBJECTS += $(KERNEL_OBJECTS)

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
