.section .text
.align 16

/* Global/external symbols */
/* ================== */
.globl arch_early_init

.extern _setup_long_gdt
.extern _setup_tss
.extern _setup_idt
/* ================= */

/* Generic architecture-dependent layer to initialize specific late configurations */
arch_early_init:

	/* Set the 64-bit GDT */
	call _setup_long_gdt

	movabsq $final_setup, %rax

	/* Far return to reload CS */
	pushq $0x18
	pushq %rax
	lretq

final_setup:

	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	call _setup_tss /* Set Task State Segment */
	call _setup_idt /* Set Interrupt Descriptor Table */
	ret
