.section .bootstrap.data, "aw"

/* Global/extern symbols
 * ===================== */
.globl _setup_gdt
.extern check_a20

.align 16

/* Basic temporary 32-bit GDT */
gdt_start:

	/* Null descriptor (0x0) */
	.quad 0x0000000000000000

	/* 32-bit code segment descriptor (0x8)
	 * ==================================== */

	/* Limit (lower 16 bits) */
	.word 0xFFFF

	/* Base address (lower 16) */
	.word 0x0000

	/* Base address (middle byte) */
	.byte 0x00
	.byte 0b10011010

	/*
	 * Granularity
	 *   Limit in 4-KiB pages
	 * 32 bits
	 * Upper limit nibble
	 */
	.byte 0b11001111

	/* Upper byte of base address */
	.byte 0x00

	/* 32-bit data segment descriptor (0x10)
	 * ===================================== */
	.word 0xFFFF
	.word 0x0000
	.byte 0x00

	/* Data segment */
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x00

	/* 64-bit code segment (0x18)
	 * ========================== */
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0b10011010

	/* Long mode */
	.byte 0b00100000
	.byte 0x00

	/* 64-bit data segment descriptor (0x20)
	 * ===================================== */
	.word 0xFFFF
	.word 0x0000
	.byte 0x00

	/* Data segment */
	.byte 0b10010010
	.byte 0b00000000
	.byte 0x00
gdt_end:

.align 4
gdt_descriptor:

	/* GDT limit address */
	.word gdt_end - gdt_start - 1

	/* GDT start */
	.long gdt_start

.section .bootstrap.text, "ax"

.code32
_setup_gdt:

	/* Loads the GDT descriptor */
	lgdt (gdt_descriptor)

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	/* Far jump to reload CS */
	ljmp $0x08, $1f

1:
	/* Jump to the next stage */
	jmp check_a20

.halt:
	cli
	hlt
	jmp .halt
