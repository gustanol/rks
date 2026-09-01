#include <arch/const.h>

.section .bootstrap.data, "aw"
.align 0x1000

/* Global/extern symbols */
.globl enable_pae
.extern early_kernel_higher_half

/* Constants */
.equ CR4_PAE, 1 << 5
.equ PAGE_PRW, 0x3
.equ PAGE_HUGE, 1 << 7
.equ EFER_MSR, 0xC0000080
.equ CR0_PM, 1 << 0
.equ CR0_PG, 1 << 31

/* Page Mapping Level 4 */
pml4:
	.fill PAGE_SIZE, 1, 0

/* Page Directory Pointer Table */
pdpt:
	.fill PAGE_SIZE, 1, 0

/* ".text" content */
.section .bootstrap.text, "ax"
.code32

/* Physical memory extension */
enable_pae:

	/* Loads the value of CR4 */
	movl %cr4, %eax

	/* Sets the PAE bit */
	orl $CR4_PAE, %eax

	/* Writes it back */
	movl %eax, %cr4

set_cr3:

	/* CR3 = PML4 physical address */
	movl $pml4, %edi

	/* Sets CR3 as the PML4 base address */
	movl %edi, %cr3

map_identity:

	/* PDPT base address (destination) */
	movl $pdpt, %eax

	/*
	 * Present
	 * Readable/Writable
	 * Ring 0 (kernel mode)
	 */
	orl $PAGE_PRW, %eax
	movl %eax, (%edi) /* 32 lower bits */
	movl $0, 4(%edi) /* The rest are zeros */

	movl $pdpt, %edi /* PDPT address */
	testl %eax, %eax /* The PDPT entry is going to be zeroed */
	orl $PAGE_PRW, %eax /* Permissions */

	/**
	 * NOTE
	 *
	 * At early boot, all bootstrap code must be contemplate by identity mapping
	 * It includes the Bootstrap Arena where early allocations work on
	 * This means a considerable portion must be mapped to identity to include everything
	 *
	 * Considering one huge page at PD layer only maps 2 MiB,
	 * We map one huge page at PDPT layer, which includes 1 GiB
	 * This is far beyond any size limit the kernel image could achieve
	 *
	 * Notice the mapping size doesn't care once it's unmapped later
	 **/

	/* Set the huge page bit */
	orl $PAGE_HUGE, %eax

	/* Writing */
	movl %eax, (%edi)
	movl $0, 4(%edi)

enable_long_mode:

	/**
 	 * Extended Feature Enable Register
 	 * Model-Specific Register (MSR) that controls specific features in long mode
 	 **/

	/* Reads EFER */
	movl $EFER_MSR, %ecx
	rdmsr

	/* Writes EFER */
	orl $(1 << 8), %eax /* Sets the 8th EFER bit */
	wrmsr

enable_paging:

	/* Enable paging */
	movl %cr0, %eax /* Load CR0 */

	/* Ensure protected mode bit is set */
	orl $CR0_PM, %eax

	/* Set the paging mode bit */
	orl $CR0_PG, %eax

	/* Write it again */
	movl %eax, %cr0 

	/* Long-jump to the next stage (higher half mapping) */
	ljmp $0x18, $early_kernel_higher_half
