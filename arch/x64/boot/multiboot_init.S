/*
 * First section `.multiboot` defines the Multiboot2 header
 *
 * It specify the kernel compatibility with it
 */
.section .multiboot
.align 2

#define MUTIBOOT2_MAGIC 0xE85250D6

multiboot_header_start:
    .long MULTIBOOT2_MAGIC # Magic
    .long 0x00000000 # Aechitecture i386
    .long multiboot_header_end - multiboot_header_start  # Header length
    .long -(MULTIBOOT2_MAGIC + 0x00 + (multiboot_header_end - multiboot_header_start))
    .short 0 # Tag type = 0 (terminator)
    .short 0 # Flags = 0
    .long 8 # Size = 8 (terminator size)
multiboot_header_end:

/*
 * Defines a new section and declares the starter point in there
 *
 * Paranoid consistency:
 * "ax" mark this section with flags `SHF_ALLOC` and `SHF_EXECINSTR`
 * Which means it's allocable and exacutable
 *
 * GAS uses to mark these standard flags for `.text` class sections
 * But this snippet defines it explicit
 */
.section .text.boot, "ax"
.globl __rks_start
.code 32

__rks_start:
	# %eax = MULTIBOOT2_MAGIC
	# %ebx = Physical address of Multiboot info struct

	/* Not jumped in by a multiboot bootloader */
	cmpl MULTIBOOT2_MAGIC, %eax
	jne _halt

	movl $stack_top, %esp

	/* Call main function */
	call _rks_main

/* On error */
_halt:
	cli
	hlt
	jmp _halt

.section .bss
.align 16

stack_bottom:
	.skip 16384
stack_top:
