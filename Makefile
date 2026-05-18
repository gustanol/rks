COMPILATION_DIR := compilation

-include ./$(COMPILATION_DIR)/CONFIG
include ./$(COMPILATION_DIR)/Makefile.vars
-include $(shell find . -name 'Makefile.subsystem')
include ./$(COMPILATION_DIR)/Makefile.rules
include $(COMPILATION_DIR)/Makefile.help
