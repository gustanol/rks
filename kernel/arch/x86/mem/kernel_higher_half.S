.section .bootstrap, "ax"
.align 16

/* Global/extern symbols */
.globl early_kernel_higher_half

/* Kernel stack */
.extern kernel_stack_top

/* Main function */
.extern main

/* Descriptors setter */
.extern set_64_descriptors

.extern _map_kernel_hh

/* Early kernel stack memory */
early_stack_bottom:
	.skip 0x2000
early_stack_top:

.code64
early_kernel_higher_half:

	/* Set the 64-bit data segment descriptor */
	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	/* Update the stack pointer */
	leaq early_stack_top(%rip), %rsp

	/* Load the PML4 physical address base */
	movq %cr3, %rdi

	/* Call the kernel higher half mapper */
	movabsq $_map_kernel_hh, %rax
	call *%rax

.higher_half_jmp:

	/* Loads the next label and jump to it */
	movabsq $.higher_half_entry, %rax
    	jmp *%rax

/* Only on error of jumping while still on identity mapping */
.identity_halt:

	cli
	hlt
	jmp .identity_halt

/* From now on, RIP is on higher half */
.section .text

.code64
.higher_half_entry:

	/* Reload the stack pointer */
	movabsq $kernel_stack_top, %rsp

	/* Set all needed 64-bit CPU descriptors */
	call set_64_descriptors

	/* Load the bootloader information structure */
	movabsq $loader_info, %rdi

	/* Get its virtual address */
	addq %r12, %rdi

	/* Call the main C function */
	call main

/* It should not be reached unless it fails to continue */
.halt:

	cli
	hlt
	jmp .halt
