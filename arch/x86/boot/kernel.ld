/* Entry point */
ENTRY(__rks_start)

/* Global symbols */
/* ================================================== */
KERNEL_PHYS_BASE = ALIGN(0x100000, 0x1000);
KERNEL_VIRT_BASE = ALIGN(0xFFFFFFFF80000000, 0x1000);
KERNEL_PHYSMAP_BASE = ALIGN(0xFFFFFFFFC0000000, 0x1000);

/*
 * Kernel virtual offset
 *
 * We use to convert physical addresses to virtual ones or the opposite
 * Note: For 2MiB pages, the mapping base is KERNEL_VIRT_BASE
 * The kernel starts at KERNEL_VIRT_BASE + KERNEL_PHYS_BASE
 *
 * After higher half, any physical address can be get by calculating:
 *   VMA - KERNEL_VIRT_OFF
 */
KERNEL_VIRT_OFF = KERNEL_VIRT_BASE;
early_alloc_phys = early_alloc - KERNEL_VIRT_OFF;
/* ================================================== */

SECTIONS
{
	. = KERNEL_PHYS_BASE;
	KERNEL_START = KERNEL_VIRT_BASE;

	/*
	 * Early code must run at physical addresses
	 *
	 * Until kernel is higher half mapped, it will be only identity mapped
	 * Virtual address must be equal to physical ones
	 *
	 * That's the role of this section
	 */
	.bootstrap : {
		*(.bootstrap .bootstrap.*)
	}

	/* Shift VMA to higher half */
	. += KERNEL_VIRT_OFF;

	/*
	 * VMA is already at higher half
	 *
	 * We can subtract it from KERNEL_VIRT_OFF to get the physical address (for LMA)
	 */
	.text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VIRT_OFF) {
		KEEP(*(.text))
		KEEP(*(.text.*))
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VIRT_OFF) {
		*(.rodata .rodata.*)
	}

	.bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VIRT_OFF)
	{
		*(COMMON)
		*(.bss .bss.*)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRT_OFF)	{
		PROVIDE(gdt_start = .);
		*(.data .data*)
	}

	.stack ALIGN(0x1000) : AT(ADDR(.stack) - KERNEL_VIRT_OFF) {
		kernel_stack_bottom = .;
		. += 0x4000;
		kernel_stack_top = .;
	}

	/* Section for keeping all drivers */
	.registry ALIGN(0x1000) : AT(ADDR(.registry) - KERNEL_VIRT_OFF)
	{
	    __registry_drivers_start = .;
	    KEEP(*(.registry.drivers))
	    __registry_drivers_end = .;

	    __registry_subsystems_start = .;
	    KEEP(*(.registry.subsystems .registry.subsystems.*))
	    __registry_subsystems_end = .;
	}

	KERNEL_END = .;
}
