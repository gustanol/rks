# Includes
include Makefile.flags
include arch/x64/Makefile

TARGET := rks.elf

all: $(ROOT_BUILD_DIR)/$(TARGET)

$(ROOT_BUILD_DIR)/$(TARGET): $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(ROOT_BUILD_DIR)

.PHONY: all clean
