.section .data
.align 4096

pml4:
	.fill 4096, 1, 0

pdpt:
	.fill 4096, 1, 0

pd:
	.fill 4096, 1, 0

pt:
	.fill 4096, 1, 0

.section .text.long
.globl checkCPUID
.code32

.equ EFLAGS_ID, 1 << 21
.equ CPUID_EXTENSIONS, 0x80000000
.equ CPUID_EXTENDED_FEATURES, 0x80000001
.equ CPUID_EDX_LONG_MODE, 1 << 29

/*
 * We must check the CPUID instruction to guarantee the long mode is available
 */
checkCPUID:
	pushfl # Push the 32-bit EFLAGS
	popl %eax

	/* Save current EFLAGS in %ecx */
	movl %eax, %ecx
	xorl EFLAGS_ID, %eax

	/* Save EFLAGS by pushing it again to stack */
	pushl %eax
	popfl

	pushfl # Read it again
	popl %eax # Gets the current EFLAGS

	/* Restore the original EFLAGS value by using %ecx register */
	pushl %ecx
	popfl # Remove it from stack

	/* If CPUID doesn't exists, the processor will only ignores the attempt to change that bit */
	xorl %ecx, %eax # So, if the values are equal, there's no CPUID
	jz .halt

check_long_mode:
	/*
	 * EAX: range 0x*0000000 ~ 0x8FFFFFFF -> Extended CPUID functions
	 *
	 * In call, CPUID will put in %eax this highest function number it supports
	 */
	movl CPUID_EXTENSIONS, %eax
	cpuid

	/* If the highest function number is less than the extended features, CPU doesn't support extended functions */
	cmpl CPUID_EXTENDED_FEATURES, %eax
	jb .halt

	/* CPUID will return feature flags */
	movl CPUID_EXTENDED_FEATURES, %eax
	cpuid

	/* If bit 29 is set, CPU supports long mode */
	testl CPUID_EDX_LONG_MODE, %edx
	jz .halt

set_cr3:
	movl $pml4, %edi
	movl %edi, %cr3 # CPU uses CR3 to determine where the page tables are

.extern kernel_end # End of kernel code (from linker script)
set_entries:
	movl $pdpt, %eax # Get the base address of PDPT
	orl $0x3, %eax # Set it as Present, Readable/Writable and Ring 0 (kernel mode)
	movl %eax, (%edi) # Write the low 32 bits
	movl $0, 4(%edi) # Zero the rest

	/*
	 * Note:
	 *
	 * In protected mode, table entries has 32 bits (1024 entries)
	 * We are aimed in long mode, so we must write 64 bits for each of them (512 entries)
	 *
	 * Tables must be align to 4Kb (page size) to not overwrite flags
	 * E.G.: A 4-Kb aligned address 0x1000 (0001 0000 0000 0000) has "free" bits from 0 to 11
	 * For each entry, these space will be used to set flags
	 * For a non-aligned address, we can't guarantee it will be loaded at a 4-Kb aligned address
	 */

	/* Set PD pointer as the first entry of PDPT */
	movl $pdpt, %edi
	movl $pd, %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $0, 4(%edi)

	/* Add PT pointer at the first entry of PD */
	movl $pd, %edi
	movl $pt, %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $0, 4(%edi)

	/* Add the first free physical frame (page) to PT */
	movl $pt, %edi

	/* Align it to 4Kb (the linker should already do this) */
	movl $kernel_end, %eax
	addl $0xFFF, %eax # Make equal (only if 1) or bigger than 4Kb
	andl $0xFFFFF000, %eax # For the cases not aligned to 4Kb, we zero the first 12 bits and get the nearest align

	/* Now we can set flags at the first 12 bits */
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $0, 4(%edi)

enable_pae:
.equ CR4_PAE, 1 << 5

	/* Sets the Physical Address Extension */
	movl %cr4, %eax
	orl CR4_PAE, %eax
	movl %eax, %cr4

enable_long_mode:
/*
 * EFER: Extended Feature Enable Register
 *
 * It's a Model-Specific Register (MSR) that controls specific features in long mode
 */
.equ EFER_MSR, 0xC0000000 # EFER MSR number
.equ EFER_LM_ENABLE, 1 << 8

	/* Read this specific internal register (EFER) */
	movl EFER_MSR, %ecx
	rdmsr

	/* Set bit 8 of EFER (LME - Long Mode Enable) */
	orl EFER_LM_ENABLE, %eax
	wrmsr # Write EFER back

enable_paging:
	/* Enable paging by setting the bit 31 of CR0 */
	movl %cr0, %eax
	orl 1 << 31, %eax
	movl %eax, %cr0

	jmp _gdt64_start

.code64
_gdt64_start:
	# Null descriptor
	.quad 0x0000000000000000

	# Code segment descriptor
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0b10011010
	.byte 0b10101111 # Long mode
	.byte 0x00

	# Data segment descriptor
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0b10010010 # Data segment
	.byte 0b11001111
	.byte 0x00
_gdt64_end:

gdt_descriptor:
	/* GDT limit address */
	.word _gdt64_end - _gdt64_start - 1
	.long _gdt64_start

finish64:
	lgdt (gdt_descriptor)

	movw $0x10, %ax # Data segment descriptor within GDT

	/* Set data descriptor as default */
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	#jmp rks_main

.halt:
	cli
	hlt
	jmp .halt
