/*
 * First section `.multiboot` defines the Multiboot header
 *
 * It specify the kernel compatibility with it
 */
.section .multiboot
.align 2

#define MUTIBOOT1_MAGIC 0x1BADB002

multiboot_header_start:
	.long MULTIBOOT1_MAGIC # Magic number (Multiboot1)
	.long 0x00000000 # Flags
	.long -(MULTIBOOT1_MAGIC + 0x00)
multiboot_header_end:

/*
 * Defines a new section and declares the starter point in there
 *
 * Paranoid consistency:
 * "ax" mark this section with flags `SHF_ALLOC` and `SHF_EXECINSTR`
 * Which it means it's allocable and exacutable
 *
 * GAS uses to mark these standard flags for `.text` class sections
 * But this snippet defines it explicit
 */
.section .text.boot, "ax"
.globl __rks_start
.code 32

__rks_start:
	# %eax = MULTIBOOT1_MAGIC
	# %ebx = Physical address of Multiboot info struct

	/* Not jumped in by a multiboot bootloader */
	cmpl MULTIBOOT1_MAGIC, %eax
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
