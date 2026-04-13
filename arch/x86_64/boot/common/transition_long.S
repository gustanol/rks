#include <mem/paging.h>

.section .data
.align 4096

.globl pml4

/* Page tables */
/* ======================= */

/* Initialize them filled by 0s */

pml4:
	.fill PAGE_SIZE, 1, 0

pdpt:
	.fill PAGE_SIZE, 1, 0

pd:
	.fill PAGE_SIZE, 1, 0

pt:
	.fill PAGE_SIZE, 1, 0
/* ======================= */

.section .text.long
.globl enable_pae

.code32

/* From now, CPU will interpret CR3 as long-mode paging */
enable_pae:
.equ CR4_PAE, 1 << 5

	/* Set the Physical Address Extension */
	/* ============ */
	movl %cr4, %eax
	orl $CR4_PAE, %eax
	movl %eax, %cr4
	/* ============ */

set_cr3:

	/*
	 * The CR3 register is used to get the physical address of PML4
	 *
	 * In here, the base address of the PML4 is loaded
	 */
	movl $pml4, %edi

	/* Set CR3 as the PML4 base address */
	movl %edi, %cr3

/* End of kernel code (from linker script) */
.extern kernel_end

map_identity:
.equ KERNEL_PRW, 0x3

	/* Page directory pointer table entry */
	/* ================================================ */
	movl $pdpt, %eax /* PDPT base address (destination) */

	/*
	 * Present
	 * Readable/Writable
	 * Ring 0 (kernel mode)
	 */
	orl $KERNEL_PRW, %eax

	movl %eax, (%edi) /* 32 lower bits */
	movl $0, 4(%edi) /* Zero the rest */
	/* ================================================= */

	/*
	 * Note:
	 *
	 * In protected mode, table entries has 32 bits (1024 entries)
	 * For long mode, they must be written as 64-bit address (512 entries)
	 *
	 * Tables must be aligned by 4 KiB (page size) to not overwrite flags
	 * E.G.: A 4-KiB aligned address 0x1000 (0001 0000 0000 0000) has "free" bits from 0 to 11
	 * For each entry, this space will be used to set flags
	 * For a non-aligned address, we can't guarantee it will be loaded at a 4-KiB aligned address
	 */

	/* Page directory entry */
	/* ================================= */
	movl $pdpt, %edi /* PDPT address */
	movl $pd, %eax /* PD base address */
	orl $KERNEL_PRW, %eax /* Permissions */

	/* Lower 32 bits */
	movl %eax, (%edi)
	movl $0, 4(%edi)
	/* ================================= */

	/* Page table entry */
	/* ================================= */
	movl $pd, %edi /* PD address */
	movl $pt, %eax /* PT base address */
	orl $KERNEL_PRW, %eax /* Permissions */

	/* Write the entry */
	movl %eax, (%edi)
	movl $0, 4(%edi)
	/* ================================= */

	/* Prepare to fill the PT */
	movl $pt, %edi
	movl $0, %ecx

/* Fill all 512 entries of the first PT */
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

	/* Repeat until all 512 entries are filled */
	cmpl $512, %ecx
	jl .fill_pt_loop

enable_long_mode:

/* EFER MSR number */
.equ EFER_MSR, 0xC0000080
	
	/*
 	 * EFER: Extended Feature Enable Register
 	 *
 	 * It's a Model-Specific Register (MSR) that controls specific features in long mode
 	 */

	/* Read EFER */
	/* ============== */
	movl $EFER_MSR, %ecx
	rdmsr
	/* ============== */

	/* Write EFER */
	/* ==================== */

	/* Set the 8th EFER bit */
	orl $(1 << 8), %eax
	wrmsr
	/* ==================== */

enable_paging:

.extern identity_long

.equ CR0_PM, 1 << 0
.equ CR0_PG, 1 << 31

	/* Enable paging */
	/* ================================ */
	movl %cr0, %eax /* Load CR0 */

	/* Ensure protected mode bit is set */
	orl $CR0_PM, %eax

	/* Set the paging mode bit */
	orl $CR0_PG, %eax

	movl %eax, %cr0 /* Write it again */
	/* ================================ */

	ljmp $0x18, $identity_long
