ARCH_DIR := arch/x64
ARCH_BUILD_DIR := $(ROOT_BUILD_DIR)/$(ARCH_DIR)

ARCH_ASM_SOURCES := $(shell find $(ARCH_DIR) -name '*.S')
ARCH_ASM_OBJECTS := $(patsubst $(ARCH_DIR)/%.S,$(ARCH_BUILD_DIR)/%.o,$(ARCH_ASM_SOURCES))
ARCH_OBJECTS := $(ARCH_ASM_OBJECTS) $(ARCH_C_OBJECTS)
ALL_OBJECTS += $(ARCH_OBJECTS)

# Build rules for assembly (with subdirectory support)
$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.S
	@mkdir -p $(dir $@)
	@echo "AS  $<"
	@$(AS) $(ASFLAGS) $< -o $@
