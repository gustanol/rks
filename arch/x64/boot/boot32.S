.section .text.boot
.globl check_a20
.extern checkCPUID
.code32

/*
 * 8086 had 20 address lines
 * It could address up to 1MB (2^20)
 * Addresses past 0xFFFFF wrapped around
 *
 * 80286 could address more than 1MB
 * The A20 gate is just a way to keep compability and allow us to address more than 1MB
 *
 * Most of modern bootloaders already enables it
 * We just check and enable if it's needed
 */
check_a20:
	/* Sets two different addresses */
	movl $0x0000000, %esi
	movl $0x1000000, %edi

	/* Move them to other registers to restore later */
	movl (%esi), %eax
	movl (%esi), %ebx

	/* Set them different values */
	movl $0x10000000, (%esi)
        movl $0x50000000, (%edi)

	/* Gets they're addresses */
	movl (%esi), %ecx
	movl (%edi), %edx

	/* Restore now */
	movl %eax, (%esi)
	movl %ebx, (%edi)

	cmpl %ecx, %edx
	je _enable_a20

	jmp .continue

_enable_a20:
	cli # Disable interrupts

	/* Enable it via port 0x92 */
	inb $0x92, %al
	testb $0x02, %al # If already set, does nothing
	jnz .continue

	orb $0x02, %al
	outb %al, $0x92

	sti

.continue:
	/* Jumps to long mode setup */
	jmp checkCPUID
