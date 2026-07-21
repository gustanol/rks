X86_INTERRUPT_BUILD_DIR := $(BUILD_DIR)/$(X86_INTERRUPT_SRC_DIR)

# Interrupt Service Routine listing object
OBJ-1 += \
	$(X86_INTERRUPT_BUILD_DIR)/isr.o

# Handlers objects
OBJ-1 += \
	$(X86_INTERRUPT_BUILD_DIR)/handlers/generic.o \
	$(X86_INTERRUPT_BUILD_DIR)/handlers/page_fault.o

# Extra flags
X86_INTERRUPT_CFLAGS := \
	-I$(X86_INTERRUPT_SRC_DIR)/include

# Scope flags
$(X86_BUILD_DIR)/%.o: \
	CFLAGS += $(X86_INTERRUPT_CFLAGS)
