.section .bss
.align 16

/* Boot stack reserved memory */
boot_stack_bottom:
	.skip 16384
boot_stack_top:

loader_info:
	.long 0
	.long 0

/* Start of code */
.section .data
.globl loader_info

/*
 * Defines a new section and declares the starter point in there
 *
 * Paranoid consistency:
 *
 * "ax" mark this section with flags `SHF_ALLOC` and `SHF_EXECINSTR`
 * Which means it's allocable and executable
 *
 * GAS uses to mark these standard flags for `.text` class sections
 * But this snippet defines it explicit
 */
.section .text.init, "ax"

.globl __rks_start

.extern _setup_gdt

.code32
__rks_start:
	/* %eax = Bootloader magic number */
	/* %ebx = Physical address of bootloader info struct */

	movl %eax, loader_info /* Store the magic number */
	movl %ebx, loader_info + 4 /* Store the Multiboot2 info struct pointer */

	movl $boot_stack_top, %esp /* Set a boot stack for protected mode */

	/* Jump to next stage (GDT setup) */
	jmp _setup_gdt

/* On error */
.halt:
	cli
	hlt
	jmp .halt
