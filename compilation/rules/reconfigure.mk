# Sources build files
# =======================
$(BUILD_SOURCES):
	@mkdir -p $(dir $@)
	@echo "# Generated build file. Do not edit it by yourself" > $@
	@echo "" >> $@
	@echo "# Subsystems" >> $@
	@find . -name '*.sources.subsys.mk' | \
		sort | sed 's/^/include /' >> $@
	@echo "Regenerating build sources -> $@"
# =======================

# Architecture cached-name
# ========================
$(ARCH_CACHE):
	@mkdir -p $(dir $@)
	@echo "# Generated file. Do not edit it by yourself" > $@
	@echo ""
	@printf "Choose one architecture [INDEX]:\n"
	@i=1; \
	for arch in $(ARCHS); do \
		printf "%d) %s\n" $$i $$arch; \
		i=$$((i+1)); \
	done; \
	printf "> "; \
	read choice; \
	arch=$$(echo "$(ARCHS)" | cut -d' ' -f$$choice); \
	echo "ARCH := $$arch" >> $@
	@echo "Regenerating cached architecture -> $@"
	@echo ""
# ========================

reconfigure:
	@rm -f $(BUILD_SOURCES)
	@echo "RM -> $(BUILD_SOURCES)"

	@rm -f $(ARCH_CACHE)
	@echo "RM -> $(ARCH_CACHE)"

	@$(MAKE) $(BUILD_SOURCES) $(ARCH_CACHE)

.PHONY: \
	sources_reconfigure \
	arch_reconfigure \
	reconfigure
