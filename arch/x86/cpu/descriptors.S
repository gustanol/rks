.section .text
.align 16

/* Global/external symbols */
/* =================== */
/* The main function */
.globl set_64_descriptors

/* External functions */
.extern _setup_long_gdt
.extern _setup_tss
.extern _setup_idt
/* =================== */

/*
 * Entry point to set the 64-bit GDT
 *
 * Proceed by jumping to the next stage
 */
set_64_descriptors:

	/* Set the 64-bit GDT */
	call _setup_long_gdt

	/* Absolute 64-bit address of the next stage label */
	movabsq $.final_descriptors, %rax

	/* Far return to reload the code segment */
	/* ================ */

	/* Kernel 64-bit CS */
	pushq $0x18
	pushq %rax
	lretq
	/* ================ */

.final_descriptors:

	/* Update the registers segments with the new kernel 64-bit DS */
	/* ========= */
	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	/* ========= */

	/* Set the Task State Segment */
	call _setup_tss

	/* Set the Interrupt Descriptor Table */
	call _setup_idt

	ret
