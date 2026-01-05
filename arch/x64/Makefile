# Tools (overridable)
AS ?= as
ASFLAGS ?= -g

SRC_DIR := .
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.S)
OBJECTS := $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(SOURCES))

# Default target: build all objects
all: $(OBJECTS)

# Assemble
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
