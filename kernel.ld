ENTRY(__rks_start) /* Entry point */

SECTIONS
{
    . = 0x100000;  /* 1MB */
    
    /* Multiboot header must be early */
    .multiboot : ALIGN(4096)
    {
        *(.multiboot)
    }
    
    .text : ALIGN(4096)
    {
        *(.text.init)
        *(.text.gdt)
        *(.text.boot)
        *(.text)
    }
    
    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }
    
    .data : ALIGN(4096)
    {
        *(.data)
    }
    
    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    /* Where the kernel ends in memory */
    kernel_end = .;
}
