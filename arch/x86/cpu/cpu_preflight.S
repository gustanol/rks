#include <cpu/cpuid.h>

.section .bootstrap.text, "ax"

/* Global/extern symbols
 * ===================== */
.globl check_a20

/* Constants
 * ========= */
.equ EFLAGS_ID, 1 << 21

/**
 * 8086 had 20 address lines
 * It could address up to 1MB (2^20)
 * Addresses past 0xFFFFF wrapped around
 *
 * 80286 could address more than 1MB
 * The A20 gate is just a way to keep compability and allow us to address more than 1MB
 *
 * Most of modern bootloaders already enables it
 **/
.code32
check_a20:

	/* Sets two different addresses */
	movl $0x0000000, %esi
	movl $0x1000000, %edi

	/* Move them to other registers to restore later */
	movl (%esi), %eax
	movl (%esi), %ebx

	/* Sets them with different values */
	movl $0x10000000, (%esi)
        movl $0x50000000, (%edi)

	/* Gets their addresses */
	movl (%esi), %ecx
	movl (%edi), %edx

	/* Restores now */
	movl %eax, (%esi)
	movl %ebx, (%edi)

	/* Compares them and enable if needed */
	cmpl %ecx, %edx
	je _enable_a20

	/* Continues the operation */
	jmp .continue

_enable_a20:

	/* Disable interrupts */
	cli

	/* Enables it via port 0x92 */
	inb $0x92, %al

	/* If already set, do nothing */
	testb $0x02, %al
	jnz .continue

	/* Otherwise, enables it (bit 2) */
	orb $0x02, %al
	outb %al, $0x92

	/* Reenable interrupts */
	sti

/* We must check the CPUID instruction to guarantee the long mode is available */
check_cpuid:

	/* Pushes the 32-bit EFLAGS */
	pushfl
	popl %eax

	/* Saves them in ECX */
	movl %eax, %ecx
	xorl $EFLAGS_ID, %eax

	/* Saves EFLAGS by pushing it again to stack */
	pushl %eax
	popfl

	/* Reads it again */
	pushfl

	/* Gets the current EFLAGS */
	popl %eax

	/* Restores the original EFLAGS value by using %ecx register */
	pushl %ecx

	/* Removes it from stack */
	popfl

	/**
	 * If CPUID doesn't exists, the processor will ignore attempts to change that bit
	 *
	 * Thus, if the values are equal, there's no CPUID
	 **/
	xorl %ecx, %eax
	jz .halt

check_long_mode:

	/**
	 * EAX: range 0x00000000 ~ 0x8FFFFFFF -> Extended CPUID functions
	 *
	 * In call, CPUID will put in EAX this highest function number it supports
	 **/
	movl $CPUID_EXTENSIONS, %eax
	cpuid

	/**
	 * If the highest function number is less than the extended features,
	 * CPU doesn't support extended functions
	 **/
	cmpl $CPUID_EXTENDED_FEATURES, %eax
	jb .halt

	/* CPUID will return feature flags */
	movl $CPUID_EXTENDED_FEATURES, %eax
	cpuid

	/* If bit 29 is set, CPU supports long mode */
	testl $CPUID_EDX_LONG_MODE, %edx
	jz .halt

.continue:

	/* Jump to long mode setup */
	jmp enable_pae

/* Halts the CPU just on error */
.halt:
	cli
	hlt
	jmp .halt
