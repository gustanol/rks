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
	xorl $EFLAGS_ID, %eax

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
	movl $CPUID_EXTENSIONS, %eax
	cpuid

	/* If the highest function number is less than the extended features, CPU doesn't support extended functions */
	cmpl $CPUID_EXTENDED_FEATURES, %eax
	jb .halt

	/* CPUID will return feature flags */
	movl $CPUID_EXTENDED_FEATURES, %eax
	cpuid

	/* If bit 29 is set, CPU supports long mode */
	testl $CPUID_EDX_LONG_MODE, %edx
	jz .halt

/* From now, CPU will interpret CR3 as long-mode paging */
enable_pae:
.equ CR4_PAE, 1 << 5

	/* Sets the Physical Address Extension */
	movl %cr4, %eax
	orl $CR4_PAE, %eax
	movl %eax, %cr4

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
	 * Tables must be aligned to 4 KiB (page size) to not overwrite flags
	 * E.G.: A 4-KiB aligned address 0x1000 (0001 0000 0000 0000) has "free" bits from 0 to 11
	 * For each entry, this space will be used to set flags
	 * For a non-aligned address, we can't guarantee it will be loaded at a 4-KiB aligned address
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

	movl $pt, %edi
	movl $0, %ecx

# Fill all 512 entries of the first PT
.fill_pt_loop:
	movl %ecx, %eax # Page index
	shll $12, %eax # Multiply by 4Kb to get the physical address of the current page

	orl $0x3, %eax # Present + Writable + Ring 0
	movl %eax, (%edi) # Low 32 bits
	movl $0, 4(%edi) # High 32 bits

	addl $8, %edi # Next PT entry
	incl %ecx # Next page index

	cmpl $512, %ecx # Repeat until fill all 512 entries of this PT
	jl .fill_pt_loop

enable_long_mode:
/*
 * EFER: Extended Feature Enable Register
 *
 * It's a Model-Specific Register (MSR) that controls specific features in long mode
 */
.equ EFER_MSR, 0xC0000080 # EFER MSR number

	/* Read this specific internal register (EFER) */
	movl $EFER_MSR, %ecx
	rdmsr

	/* Set bit 8 of EFER (LME - Long Mode Enable) */
	orl $(1 << 8), %eax
	wrmsr /* Write EFER back */

enable_paging:
	/* Enable paging by setting the bit 31 of CR0 */
	movl %cr0, %eax
	orl $(1 << 31), %eax
	movl %eax, %cr0

	ljmp $0x18, $finish64

finish64:
.code64

.extern rks_main
.extern multiboot_info_ptr

	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	/* Update the stack pointer */
	movq $kernel_stack_top, %rsp

	/* Call the main kernel function */
	movq multiboot_info_ptr, %rdi
	call rks_main

/* It should not be reached */
.halt:
	cli
	hlt
	jmp .halt

.section .bss
.align 16

.globl kernel_stack_top

kernel_stack_bottom:
	.skip 16384
kernel_stack_top:
