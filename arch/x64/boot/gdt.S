# Section to define and load GDT
.section .data

/* We will must a set GDT to switch to long mode */
gdt_start:
	# Null descriptor
	.quad 0x0000000000000000

	/*
	 * Although we are still in protected mode, each GDT entry has 8 bytes
	 *
	 * For the null descriptor, only the lowest 32 bits are gonna be write
	 */

	# Code segment descriptor
	.word 0xFFFF # Limit (lower 16 bits)
	.word 0x0000 # Base address (lower 16)
	.byte 0x00 # Base address (middle byte)

	/*
	 * Present segment (1)
	 * Ring 0 (00)
	 * Code segment (1)
	 * Executable (1)
	 * Direction
	 * Readable
	 * Accessed
	 */
	.byte 0b10011010
	.byte 0b11001111 # Granularity (limit is in 4 KiB pages) | 32-bit | upper limit nibble
	.byte 0x00 # Upper byte of base address

	# Data segment descriptor
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0b10010010 # Data segment
	.byte 0b11001111
	.byte 0x00
gdt_end:

gdt_descriptor:
	/* GDT limit address */
	.word gdt_end - gdt_start - 1
	.long gdt_start

.section .text.gdt
.globl _setup_gdt
.extern check_a20
.code32

/* Global label */
_setup_gdt:
	/* Loads GDT */
	lgdt (gdt_descriptor)

	movw $0x10, %ax # Data segment descriptor within GDT

	/* Set data descriptor as default */
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	jmp check_a20 # Jump to next stage to ckeck the A20 line

_halt:
	cli
	hlt
	jmp _halt
