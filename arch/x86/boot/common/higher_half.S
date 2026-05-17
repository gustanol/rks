#include <mem/paging.h>

.section .bootstrap, "ax"
.align 16

/* Global/extern symbols */
/* ============================= */
.globl identity_long

/* Kernel stack */
.extern kernel_stack_top

/* kernel_pages -> R9 */
.extern KERNEL_PHYS_BASE /* R10 */
.extern KERNEL_VIRT_BASE /* R11 */
.extern KERNEL_VIRT_OFF /* R12 */

/* Generic symbols */
.extern kernel_start
.extern kernel_end
.extern pml4
.extern loader_info

/* Functions */
.extern early_alloc_phys
.extern early_alloc
.extern rks_main
.extern set_64_descriptors
/* ============================ */

/* Constants */
/* =========== */
.equ PML4_SHR, 39
.equ PDPT_SHR, 30
.equ PD_SHR, 21

.equ PG_PERMISSIONS, 0b111
.equ PG_PERMISSIONS_2MB, 0b10000000
.equ PG_PRESENT, 0x1
/* =========== */

/* Early kernel stack memory */
/* ============== */
early_stack_bottom:
	.skip 16384
early_stack_top:
/* ============== */

identity_long:
.code64

	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	/* Update the stack pointer */
	leaq early_stack_top(%rip), %rsp

	/* 2-MiB-sized pages for kernel code */
	/* ======================================= */

	/* Get the kernel code size */
	movq $kernel_end, %rax
	subq $kernel_start, %rax

	/* 2MiB - 1 (Guarantee it will no be zero) */
	addq $0x1FFFFF, %rax

	/* Clears RDX to avoid garbage division */
	xorq %rdx, %rdx

	/* Divide it by 2MiB */
	movq $0x200000, %rsi
	divq %rsi

	/* Move to R9 (pages needed) */
	movq %rax, %r9
	/* ======================================= */

	/*
	 * Get the kernel physical base and align it
	 *
	 * For 2MiB pages, the mapping must start at an aligned region
	 * To include the kernel, we ALWAYS must align it down
	 */
	movabsq $KERNEL_PHYS_BASE, %r10
	andq $~0x1FFFFF, %r10

	/* Kernel virtual base */
	movabsq $KERNEL_VIRT_BASE, %r11

	/* Kernel virtual offset */
	movabsq $KERNEL_VIRT_OFF, %r12

map_higher_half:

	/* PML4 index */
	/* ============================================ */
	movq %r11, %rax

	/*
	 * Right shift 39 bits
	 *
	 * Place the 9 bits of PML4 index at start
	 * That's because a virtual address is 48-bit size
	 */
	shrq $PML4_SHR, %rax

	/*
	 * Once the bits are at the start,
	 * We can get the index by seeing the 9 bits
	 */
	andq $0x1FF, %rax

	/* Store the PML4 index into R15 */
	movq %rax, %r15
	/* ============================================= */

	/* PDPT index */
	/* ================== */
	movq %r11, %rax

	/* Get the PDPT index */
	shrq $PDPT_SHR, %rax
	andq $0x1FF, %rax

	/* Store it into R14 */
	movq %rax, %r14
	/* ================== */

	/* PD index */
	/* ================== */
	movq %r11, %rax

	/* Get the PD index */
	shrq $PD_SHR, %rax
	andq $0x1FF, %rax

	/* Store it into R13 */
	movq %rax, %r13
	/* ================== */

.pdpt_present:

	/* Load the PML4 base address */
	leaq pml4(%rip), %rbx

	/* Is PDPT present? */
	/* ========================================= */

	/*
	 * PML4 (RBX) present bit checking
	 *   The index is located in R15
	 */
	testb $PG_PRESENT, (%rbx, %r15, 8)

	/*
	 * == 0 means the bit is not set
	 * We must map a page before proceding
	 */
	jz .pdpt_alloc

	/*
	 * != 0 means the bit is set
	 * We can safely get its address
	 */
	jnz .pdpt_address
	/* ========================================= */

.pdpt_alloc:

	/* Push important registers */
	/* ==== */
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %rbx
	/* ==== */

	/* Allocate another page */
	/* =============== */
	movq $PAGE_SIZE, %rdi

	/*
	 * Get the physical address of 'early_alloc' function,
	 * Which is located at '.section .text' (higher half)
	 */
	movabsq $early_alloc_phys, %rax
	call *%rax

	/* RAX = Returned virtual address */

	/* Subtract the virtual kernel offset (R12) to get the physical address */
	subq %r12, %rax
	/* =============== */

	/* Pop the registers */
	/* === */
	popq %rbx
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	/* === */

	/* Write page base address + flags to PML4 index */
	/* ============================= */

	/*
	 * Returned page base address
	 * Pass it to PML4 index
	 */
	movq %rax, (%rbx, %r15, 8)

	/* By an OR, add its permissions */
	orq $PG_PERMISSIONS, (%rbx, %r15, 8)
	/* ============================= */

.pdpt_address:

	/* PDPT address to RBX */
	movq (%rbx, %r15, 8), %rbx

	/* Alignment */
	/* ========================================== */

	/*
	 * Now, we need to align the address to 4096
	 * This happens because we shall not consider the flags
	 *
	 * Basically, the least significant 12 bits are zeroed (up to 2 ^ 11)
	 * Doing this, we got only the physical page address
	 */
	andq $~0xFFF, %rbx
	/* =========================================== */

.pd_present:

	/* Is PD present? */
	/* ========================================= */

	/* PDPT (RBX) present bit checking */
	testb $PG_PRESENT, (%rbx, %r14, 8)
	jz .pd_alloc
	jnz .pd_address
	/* ========================================= */

.pd_alloc:

	/* Push registers */
	/* ==== */
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %rbx
	/* ==== */

	/* Allocate another page */
	/* =============== */
	movq $PAGE_SIZE, %rdi

	/* Call 'early_alloc' */
	movabsq $early_alloc_phys, %rax
	call *%rax

	/* Get the physical address */
	subq %r12, %rax
	/* =============== */

	/* Pop registers */
	/* === */
	popq %rbx
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	/* === */

	/*
	 * Returned page base address
	 * Pass it to PDPT index
	 */
	movq %rax, (%rbx, %r14, 8)

	/* Permissions */
	orq $PG_PERMISSIONS, (%rbx, %r14, 8)

.pd_address:

	/* PD index */
	movq (%rbx, %r14, 8), %rbx

	/* Alignment */
	andq $~0xFFF, %rbx

.map_pd_phys_addr:

	/* Verify if the PD's index is present */
	testb $PG_PRESENT, (%rbx, %r13, 8)
	jnz .continue_mapping

	/* Write the physical address and its flags to PD's index */
	/* ================================== */

	/* Updated kernel physical base */
	movq %r10, (%rbx, %r13, 8)

	/* Permissions */
	orq $PG_PERMISSIONS, (%rbx, %r13, 8)
	orq $PG_PERMISSIONS_2MB, (%rbx, %r13, 8)
	/* ================================== */

.continue_mapping:

	/* Kernel code needed pages update */
	/* ======================= */
	
	/* Remaining pages to be mapped */
	movq %r9, %rax

	/* Update it */
	decq %rax
	movq %rax, %r9
	/* ======================= */

	/* Verify if all kernel code was mapped */
	/* ============================================= */

	/*
	 * Comparing the same value will always result in 1
	 *   Unless the value is zero
	 *
	 * Being zero, we don't need to map any other pages
	 */

	testq %rax, %rax
	jz .higher_half_jmp
	jnz .map_another_page
	/* ============================================= */

.map_another_page:

	/* Update bases */
	/* =========================== */

	/* Physical base */
	movq %r10, %rax
	addq $0x200000, %rax
	movq %rax, %r10

	/* Virtual base */
	movq %r11, %rax
	addq $0x200000, %rax
	movq %rax, %r11
	/* =========================== */

	/* Go back to continue the mapping */
	jmp map_higher_half

.higher_half_jmp:

	/* Load the next label and jump to it */
	movabsq $.higher_half_entry, %rax
    	jmp *%rax

/* Only on error of jumping while still on identity mapping */
.identity_halt:

	cli
	hlt
	jmp .identity_halt

/* From now on, RIP is truly on higher half */
.section .text
.code64

.higher_half_entry:

	/* Reload the stack pointer */
	movabsq $kernel_stack_top, %rsp

	/* Set all needed 64-bit CPU descriptors */
	call set_64_descriptors

	/* Load the bootloader information structure */
	movabsq $loader_info, %rdi

	/* Get its virtual address */
	addq %r12, %rdi

	/* Call the main C function */
	call rks_main

/* It should not be reached */
.halt:

	cli
	hlt
	jmp .halt
