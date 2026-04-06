ENTRY(__rks_start) /* x64 entry point */

SECTIONS
{
    . = 0x100000;  /* 1MB */
    kernel_start = .;
    
    .text : ALIGN(4096)
    {
        KEEP(*(.boot))
        *(.text.init)
        *(.text.gdt)
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

    . = ALIGN(4096);
    kernel_end = .;
}
