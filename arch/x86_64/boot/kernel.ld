ENTRY(__rks_start)

KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;
KERNEL_PHYS_BASE = 0x100000;

SECTIONS
{
    . = KERNEL_PHYS_BASE;
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
