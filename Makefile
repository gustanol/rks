COMPILATION_DIR := compilation

include $(COMPILATION_DIR)/config.mk
include $(COMPILATION_DIR)/flags.mk
include $(COMPILATION_DIR)/rules/build.mk
include $(COMPILATION_DIR)/rules/msg.mk
include $(COMPILATION_DIR)/rules/clean.mk
include $(COMPILATION_DIR)/rules/reconfigure.mk
