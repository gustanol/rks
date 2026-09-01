.section .bootstrap.data, "aw"

/* Global/extern symbols */
.globl loader_info
.globl __rks_start

.extern _setup_gdt

.align 16

/* Boot temporary stack memory */
boot_stack_bottom:
	.skip 4096
boot_stack_top:

/* Structure to keep information about the bootloader */
loader_info:

	/* Magic number */
	.long 0

	/* Pointer to its info struct */
	.quad 0

/**
 * "ax" marks this section with flags `SHF_ALLOC` and `SHF_EXECINSTR`
 * Which means it's allocable and executable
 **/
.section .bootstrap.text, "ax"

.code32
__rks_start:

	/**
	 * EAX = Bootloader magic number
	 * EBX = Physical address of bootloader info struct
	 *
	 * By default, every supported bootloader protocol should follow the
	 * Registers above
	 *
	 * Loading these information at these registers allow us to parse them later,
	 * Keeping it neat and easy to recover and populate the generic boot info structure
	 **/

	/* Bootloader information */
	movl $loader_info, %edx

	/* Stores the magic number */
	movl %eax, (%edx)

	/* Stores the Bootloader info struct pointer */
	movl %ebx, 4(%edx)

	/* Sets the boot stack for protected mode */
	movl $boot_stack_top, %esp

	/* Jumps to the next stage */
	jmp _setup_gdt

/* On error */
.halt:
	cli
	hlt
	jmp .halt
