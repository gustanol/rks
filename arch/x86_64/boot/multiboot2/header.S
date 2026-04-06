/*
 * The Multiboot2 header must be at start
 * So, define it at the first section
 *
 * It'll specify the request of what data we're going to need from Multiboot2
 */
.section .boot, "ax"
.align 8

.globl MULTIBOOT_BOOTLOADER_MAGIC

/* Constants */
.equ MULTIBOOT2_BOOTLOADER_MAGIC, 0x36D76289
.equ MULTIBOOT2_HEADER_MAGIC, 0xE85250D6
.equ MULTIBOOT2_ARCH_I386, 0x00000000

multiboot_header_start:
	.long MULTIBOOT2_HEADER_MAGIC /* Magic */
	.long MULTIBOOT2_ARCH_I386 /* i386 rchitecture */
	.long multiboot_header_end - multiboot_header_start /* Header length */

	/* Checksum */
	.long -(MULTIBOOT2_HEADER_MAGIC + 0x00 + (multiboot_header_end - multiboot_header_start))

	/* Information tag request */
	.balign 8
	.short 1 /* Type 1: Information request */
    	.short 0 /* Flags */
    	.long 24 /* Size: 8 (header) + 4 (one request) */

	.long 1 /* Boot command line */
	.long 6 /* Memory map */
	.long 8 /* Framebuffer info */
	.long 15 /* ACPI new */

    	.balign 8
	.short 0 /* Tag type = 0 (terminator) */
	.short 0 /* Flags = 0 */
	.long 8 /* Size = 8 (terminator size) */
multiboot_header_end:
