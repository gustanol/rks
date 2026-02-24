# Includes
include config.mk
include Makefile.flags
include arch/x64/Makefile
include kernel/Makefile
include lib/Makefile
include drivers/Makefile

all_objs := $(obj-1)

all: $(root_build_dir)/$(target)
$(all_objs): $(autoconfig_header)

# Target build rule
$(root_build_dir)/$(target): $(all_objs)
	@$(LD) $(LDFLAGS) $^ -o $@
	@echo "LD  $@"
	@ln -s $@ .

clean:
	rm -rf $(root_build_dir)
	rm include/rks/autoconf.h
	rm ./$(target)

# Rule to generate a header with all flags
$(autoconfig_header): config.mk
	@echo "/* Generated header */" > $@
	@echo "#ifndef AUTOCONF_H" >> $@
	@echo "#define AUTOCONF_H" >> $@
	@$(foreach v, $(filter CFG_%, $(.VARIABLES)), \
		echo "#define $(v) $($(v))" >> $@;)
	@echo "#endif /* AUTOCONF_H */" >> $@

help:
	@echo "Research Kernel System"
	@echo ""
	@echo "Build rules:"
	@echo "  all: Compile and link build-specified files to create kernel binary"
	@echo "  clean: Clean built objects and the linked kernel binary"

.PHONY: all clean help
