# Cleaning
# ========
clean: clean_arch_includes
	@rm -rf ./$(BUILD_DIR)
	@echo "RM -> ./$(BUILD_DIR)"

	@rm -f $(CONFIG_HEADER)
	@echo "RM -> $(CONFIG_HEADER)"

	@rm -f ./$(BIN_NAME)
	@echo "RM -> ./$(BIN_NAME)"

	@rm -rf ./$(DEBUG_DIR)
	@echo "RM -> ./$(DEBUG_DIR)"

clean_arch:
	@rm -rf $(ARCH_BUILD_DIR)
	@echo "RM -> $(ARCH_BUILD_DIR)"

clean_drivers:
	@rm -rf ./$(DRIVERS_BUILD_DIR)
	@echo "RM -> $(DRIVERS_BUILD_DIR)"

clean_kernel:
	@rm -rf $(KERNEL_BUILD_DIR)
	@echo "RM -> $(KERNEL_BUILD_DIR)"

clean_lib:
	@rm -rf $(LIB_BUILD_DIR)
	@echo "RM -> $(LIB_BUILD_DIR)"

clean_arch_includes:
	@rm -rf ./$(INCLUDES_DIR)/arch
	@echo "RM -> ./$(INCLUDES_DIR)/arch"

.PHONY: \
	clean \
	clean_arch \
	clean_drivers \
	clean_kernel \
	clean_lib \
	clean_arch_includes
