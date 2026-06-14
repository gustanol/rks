# C compiler flags
# ================
CFLAGS := \
	-nostdinc \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-pic \
	-fno-pie \
	-Wall \
	-Wextra \
	-Werror \
	-Wfatal-errors \
	-Wpedantic \
	-Wstrict-prototypes \
	-Wold-style-definition \
	-mno-red-zone \
	-mcmodel=kernel \
	-std=gnu11 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-m64 \
	-D__KERNEL__ \
	-DARCH_$(ARCH) \
	-I./$(INCLUDES_DIR)
# ================

# Debug flags
# ===========
ifeq ($(DEBUG),1)
	CFLAGS += \
		-g3 \
		-O0
endif

# GAS flags
# =========
GAS_CFLAGS := -D__ASSEMBLY__

# Assembler flags
# ===============
ASFLAGS := \
	--fatal-warnings \
	-g \
	--64
# ===============

# Linker flags
# ============
LDFLAGS := \
	--fatal-warnings \
	--warn-common \
	--warn-once \
	--script=./$(LINKER_SCRIPT) \
	-nostdlib \
	-z max-page-size=0x1000 \
	-no-pie
# ============
