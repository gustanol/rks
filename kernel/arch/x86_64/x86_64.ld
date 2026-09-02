/* Entry point */
ENTRY(__rks_start)

/* Global symbols */
KERNEL_PHYS_BASE = ALIGN(0x100000, 0x1000);
KERNEL_VIRT_OFF = ALIGN(0xFFFFFFFF80000000, 0x1000);
KERNEL_VIRT_BASE = KERNEL_VIRT_OFF + KERNEL_PHYS_BASE;
HHDM_BASE = ALIGN(0xFFFFFFFFC0000000, 0x1000);
BOOSTRAP_ARENA_SIZE = 0x100000;

SECTIONS
{
	. = KERNEL_PHYS_BASE;
	KERNEL_START = KERNEL_VIRT_BASE;

	/* Identity-mapped code running on physical addresses */
	.bootstrap.text : ALIGN(0x1000) {
		*(.bootstrap.text)
		*(.bootstrap.text.*)
	}

	.bootstrap.rodata : {
		*(.bootstrap.rodata)
		*(.bootstrap.rodata.*)
	}

	.bootstrap.data : {
		*(.bootstrap.data)
		*(.bootstrap.data.*)
	}

	.bootstrap.bss (NOLOAD) : {
		*(.bootstrap.bss)
		*(.bootstrap.bss.*)
	}

	.bootstrap.arena (NOLOAD) : ALIGN(0x1000) {
		BOOTSTRAP_ARENA_START = .;
		. += BOOTSTRAP_ARENA_SIZE;
		BOOTSTRAP_ARENA_END = .;
	}

	/* Align the end of bootstrap code */
	. = ALIGN(0x1000);

	/* Shift VMA to higher half */
	. += KERNEL_VIRT_OFF;

	/**
	 * VMA is already at higher half
	 *
	 * We can subtract it from KERNEL_VIRT_OFF to get the physical address (for LMA)
	 **/
	.text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VIRT_OFF) {
		KEEP(*(.text))
		KEEP(*(.text.*))
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VIRT_OFF) {
		*(.rodata .rodata.*)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRT_OFF)	{
		PROVIDE(gdt_start = .);
		*(.data .data*)
	}

	.bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VIRT_OFF)
	{
		*(COMMON)
		*(.bss .bss.*)
	}

	.stack ALIGN(0x1000) : AT(ADDR(.stack) - KERNEL_VIRT_OFF) {
		kernel_stack_bottom = .;
			. += 0x4000;
		kernel_stack_top = .;
	}

	.subsys ALIGN(0x1000) : AT(ADDR(.subsys) - KERNEL_VIRT_OFF)
	{
	    __subsys_backend_registry_start = .;
		    KEEP(*(.subsys.registry .subsys.registry*))
	    __subsys_backend_registry_end = .;
	}

	/* Align up the end 2MiB */
	. = ALIGN(0x200000);

	/* VMA kernel end symbol */
	KERNEL_END = .;
}
