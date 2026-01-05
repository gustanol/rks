include arch/x64/Makefile

ALL_OBJECTS += $(OBJECTS)

LD ?= ld
LINKER_SCRIPT := kernel.ld
LDFLAGS := -T $(LINKER_SCRIPT)

TARGET := rks.elf

all: $(TARGET)

$(TARGET): $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf build

.PHONY: all clean
