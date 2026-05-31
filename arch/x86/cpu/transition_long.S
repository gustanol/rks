#include <x86/const.h>

.section .bootstrap.data, "aw"
.align 0x1000

/* Global/extern symbols
 * ===================== */
.globl enable_pae
.extern kernel_end
.extern early_kernel_higher_half

/* Constants
 * =================== */
.equ CR4_PAE, 1 << 5
.equ KERNEL_PRW, 0x3
.equ EFER_MSR, 0xC0000080
.equ CR0_PM, 1 << 0
.equ CR0_PG, 1 << 31

/* Page tables
 * =========== */

/* Page Mapping Level 4 */
pml4:
	.fill PAGE_SIZE, 1, 0

/* Page Directory Pointer Table */
pdpt:
	.fill PAGE_SIZE, 1, 0

/* Page Directory */
pd:
	.fill PAGE_SIZE, 1, 0

/* Page Table */
pt:
	.fill PAGE_SIZE, 1, 0

/* ".text" content */
.section .bootstrap.text, "ax"

.code32
enable_pae:

	/* Physical memory extension */

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

	/* Page directory pointer table entry
	 * ================================== */

	/* PDPT base address (destination) */
	movl $pdpt, %eax

	/*
	 * Present
	 * Readable/Writable
	 * Ring 0 (kernel mode)
	 */
	orl $KERNEL_PRW, %eax

	/* 32 lower bits */
	movl %eax, (%edi)

	/* The rest are zeros */
	movl $0, 4(%edi)

	/**
	 * Note:
	 *
	 * In protected mode, table entries has 32 bits (1024 entries)
	 * For long mode, they must be written as 64-bit address (512 entries)
	 *
	 * Tables must be aligned by 4 KiB (page size) to not overwrite flags
	 * E.G.: A 4-KiB aligned address 0x1000 (0001 0000 0000 0000) has "free" bits from 0 to 11
	 * For each entry, this space will be used to set flags
	 * For a non-aligned address, we can't guarantee it will be loaded at a 4-KiB aligned address
	 **/

	/* Page directory entry
	 * ==================== */

	/* PDPT address */
	movl $pdpt, %edi

	/* PD base address */
	movl $pd, %eax

	/* Permissions */
	orl $KERNEL_PRW, %eax

	/* Writing */
	movl %eax, (%edi)
	movl $0, 4(%edi)

	/* Page table entry
	 * ================ */
	movl $pd, %edi
	movl $pt, %eax
	orl $KERNEL_PRW, %eax

	/* Writing */
	movl %eax, (%edi)
	movl $0, 4(%edi)

	/* Prepares to fill the PT */
	movl $pt, %edi
	movl $0, %ecx

/* Fills all 512 entries of the first PT */
.fill_pt_loop:

	/* Page index */
	/* ====================================================== */
	movl %ecx, %eax

	/*
	 * n << 12 = n * 0x1000 = n * 4096
	 * Used to get the physical base address of the current page
	 */
	shll $12, %eax
	/* ======================================================= */

	/* Current entry */
	/* ================================= */
	orl $KERNEL_PRW, %eax /* Permissions */

	movl %eax, (%edi) /* 32 lower bits */
	movl $0, 4(%edi)
	/* ================================= */

	/* Next entry */
	/* ======================= */

	/* PT + 8b = Next PT entry */
	addl $8, %edi

	/* Next page index */
	incl %ecx
	/* ======================= */

	/* Repeats until all 512 entries are filled */
	cmpl $512, %ecx
	jl .fill_pt_loop

enable_long_mode:

	/*
 	 * EFER: Extended Feature Enable Register
 	 *
 	 * It's a Model-Specific Register (MSR) that controls specific features in long mode
 	 */

	/* Reads EFER */
	/* ============== */
	movl $EFER_MSR, %ecx
	rdmsr
	/* ============== */

	/* Writes EFER */
	/* ==================== */

	/* Sets the 8th EFER bit */
	orl $(1 << 8), %eax
	wrmsr
	/* ==================== */

enable_paging:

	/* Enables paging */
	/* ================================ */
	movl %cr0, %eax /* Load CR0 */

	/* Ensures protected mode bit is set */
	orl $CR0_PM, %eax

	/* Sets the paging mode bit */
	orl $CR0_PG, %eax

	movl %eax, %cr0 /* Write it again */
	/* ================================ */

	/* Long-jump to the next stage (higher half mapping) */
	ljmp $0x18, $early_kernel_higher_half
