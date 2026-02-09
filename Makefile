# Includes
include config.mk
include Makefile.flags
include arch/x64/Makefile
include kernel/Makefile
include drivers/Makefile

all: $(ROOT_BUILD_DIR)/$(TARGET)

$(ROOT_BUILD_DIR)/$(TARGET): $(ALL_OBJECTS)
	@$(LD) $(LDFLAGS) $^ -o $@
	@echo "LD  $@"

clean:
	rm -rf $(ROOT_BUILD_DIR)

make help:
	@echo "Research Kernel System"
	@echo ""
	@echo "Build rules:"
	@echo "  all: Compile and link build-specified files to create kernel binary"
	@echo "  clean: Clean built objects and the linked kernel binary"

.PHONY: all clean help
