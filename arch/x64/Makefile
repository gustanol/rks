ARCH_DIR := arch/x64
ARCH_BUILD_DIR := $(ROOT_BUILD_DIR)/$(ARCH_DIR)

ARCH_ASM_SOURCES := $(shell find $(ARCH_DIR) -name '*.S')
ARCH_ASM_OBJECTS := $(patsubst $(ARCH_DIR)/%.S,$(ARCH_BUILD_DIR)/%.o,$(ARCH_ASM_SOURCES))

ARCH_C_SOURCES := $(shell find $(ARCH_DIR) -name '*.c')
ARCH_C_OBJECTS := $(patsubst $(ARCH_DIR)/%.c,$(ARCH_BUILD_DIR)/%.o,$(ARCH_C_SOURCES))

ARCH_OBJECTS := $(ARCH_ASM_OBJECTS) $(ARCH_C_OBJECTS)
ALL_OBJECTS += $(ARCH_OBJECTS)

# Build rules for Assembly
$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.S
	@mkdir -p $(dir $@)
	@echo "AS  $<"
	@$(AS) $(ASFLAGS) $< -o $@

# Build rules for C
$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
