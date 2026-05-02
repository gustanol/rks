
/*
 * ".bootstrap": Section for early setup (until higher half mapping) 
 *
 * Paranoid consistency (not too much)
 *
 * "ax" mark this section with flags `SHF_ALLOC` and `SHF_EXECINSTR`
 * Which means it's allocable and executable
 *
 * GAS uses to mark these standard flags for `.text` class sections
 * But not for all sections
 *
 * This defines it explicitly
 */

.section .bootstrap, "ax"

/* Global/extern symbols */
/* ============ */
.globl loader_info
.globl __rks_start

.extern _setup_gdt
/* ============ */

.align 16

/* Boot temporary stack memory */
/* ============ */
boot_stack_bottom:
	.skip 4096
boot_stack_top:
/* ============ */

/* Structure to keep information about the bootloader */
/* ========================================== */
loader_info:
	.long 0 /* Magic number */
	.long 0 /* Pointer to its info struct */
/* ========================================== */

.code32

/* x86-64 entry */
__rks_start:

	/* EAX = Bootloader magic number */
	/* EBX = Physical address of bootloader info struct */

	/* Bootloader information */
	/* ======================================== */

	/* Store the magic number */
	movl %eax, loader_info

	/* Store the Bootloader info struct pointer */
	movl %ebx, loader_info
	/* ======================================== */

	/* Set a boot stack for protected mode */
	movl $boot_stack_top, %esp

	/* Jump to next stage */
	jmp _setup_gdt

/* On error */
.halt:
	cli
	hlt
	jmp .halt
