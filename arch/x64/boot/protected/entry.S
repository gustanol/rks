/*
 * First section `.multiboot` defines the Multiboot2 header
 *
 * It specify the kernel compatibility with it
 */
.section .multiboot, "ax"
.align 8

.equ MULTIBOOT2_HEADER_MAGIC, 0xE85250D6
.equ MULTIBOOT2_BOOTLOADER_MAGIC, 0x36D76289
.equ MULTIBOOT2_ARCH_I386, 0x00000000

multiboot_header_start:
	.long MULTIBOOT2_HEADER_MAGIC # Magic
	.long MULTIBOOT2_ARCH_I386 # Architecture i386
	.long multiboot_header_end - multiboot_header_start # Header length
	.long -(MULTIBOOT2_HEADER_MAGIC + 0x00 + (multiboot_header_end - multiboot_header_start))

	.short 1                # Type (u16)
    	.short 0                # Flags (u16)
    	.long 12                # Size (u32): 8 (header) + 4 (one request)
    	.long 6                 # Request: Tag Type 6 (Memory Map) (u32)

    	.balign 8

	.short 0 # Tag type = 0 (terminator)
	.short 0 # Flags = 0
	.long 8 # Size = 8 (terminator size)
multiboot_header_end:

.section .data
.globl multiboot_info_ptr

multiboot_info_ptr:
	.quad 0

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
	# %eax = Multiboot2 Bootloader Magic
	# %ebx = Physical address of Multiboot info struct

	/* Not jumped in by a multiboot bootloader */
	cmpl $MULTIBOOT2_BOOTLOADER_MAGIC, %eax
	jne _halt

	movl %ebx, multiboot_info_ptr # Stores the pointer to Multiboot2 header
	movl $boot_stack_top, %esp # Set a boot stack for protected mode

	/* Jump to next stage (GDT setup) */
	jmp _setup_gdt

/* On error */
_halt:
	cli
	hlt
	jmp _halt

.section .bss
.align 16

boot_stack_bottom:
	.skip 16384
boot_stack_top:
