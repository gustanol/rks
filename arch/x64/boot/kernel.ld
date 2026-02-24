ENTRY(__rks_start) /* x64 entry point */

SECTIONS
{
    . = 0x100000;  /* 1MB */
    
    /* Multiboot header */
    .multiboot : ALIGN(4096)
    {
        *(.multiboot)
    }
    
    .text : ALIGN(4096)
    {
        *(.text.init)
        *(.text.gdt)
        *(.text.boot)
        *(.text.long)
        *(.text)
    }
    
    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        PROVIDE(gdt_start = .);
        *(.data*)
    }
    
    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    .drivers_init : ALIGN(4096)
    {
	__drivers_init_start = .;
	*(.drivers_init)
	__drivers_init_end = .;
    }

    kernel_end = .;
}
