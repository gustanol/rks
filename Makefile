COMPILATION_DIR := compilation

-include ./$(COMPILATION_DIR)/CONFIG
include ./$(COMPILATION_DIR)/Makefile.vars
-include $(CONFIG_SOURCES_LIST)
include ./$(COMPILATION_DIR)/Makefile.rules
include $(COMPILATION_DIR)/Makefile.help
