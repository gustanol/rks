#include <mem/paging.h>

/* ------------------- */
.section .bss
.align 16

/* Kernel stack memory */
/* ============== */
.globl kernel_stack_top

kernel_stack_bottom:
	.skip 16384
kernel_stack_top:
/* ============== */

kernel_pages: .quad 0
/* ------------------- */

/* --------------------------- */
.section .data

.globl KERNEL_VIRT_BASE

KERNEL_VIRT_BASE:
	.quad 0xFFFFFFFF80000000;
/* --------------------------- */

.section .text.long

.globl identity_long

identity_long:
.code64

.extern kernel_start
.extern kernel_end

	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	/* Update the stack pointer */
	movq $kernel_stack_top, %rsp

	/* Prepare to map to higher-half */
	movq $KERNEL_VIRT_BASE, %rsi

	/* 2-MiB-sized pages for kernel code */
	/* ======================================= */

	/* Get the kernel code size */
	movq $kernel_end, %rax
	subq $kernel_start, %rax

	/*
	 * Just calculates the size of 2 MiB in bytes
	 *
	 * 1024 * 1024 * 2 (<< 1)
	 */
	movq $0x200000, %rsi
	
	/* Amount of pages */
	divq %rsi

	/* Move to a symbol */
	movq %rax, kernel_pages(%rip)
	/* ======================================= */

map_higher_half:

/* Constants */
/* =========== */
.equ PML4_SHR, 39
.equ PDPT_SHR, 30
.equ PD_SHR, 21

.equ PG_PERMISSIONS, 0b111
.equ PG_PERMISSIONS_2MB, 0b10000000
.equ PG_PRESENT, 0x1
/* =========== */

/* Extern symbols */
/* ============== */
.extern pml4
.extern early_alloc
/* ============== */

	/* PML4 index */
	/* ============================================ */
	movq %rsi, %rax

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
	movq %rsi, %rax

	/* Get the PDPT index */
	shrq $PDPT_SHR, %rax
	andq $0x1FF, %rax

	/* Store it into R14 */
	movq %rax, %r14
	/* ================== */

	/* PD index */
	/* ================= */
	movq %rsi, %rax

	/* Get the PD index */
	shrq $PD_SHR, %rax
	andq $0x1FF, %rax

	/* Store it into R13 */
	movq %rax, %r13
	/* ================= */

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
	pushq %rbx
	/* ==== */

	/* Allocate another page */
	/* =============== */
	movq $PAGE_SIZE, %rdi
	call early_alloc
	/* =============== */

	/* Pop the registers */
	/* === */
	popq %rbx
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
	leaq (%rbx, %r15, 8), %rbx

	/* Alignment */
	/* ========================================== */

	/*
	 * We need to get only the address
	 * For this, we don't consider the flags
	 *
	 * Aligning them means:
	 *   - Right shift 12 bits (4096) to remove them
	 *   - Left shift 12 bits to get them zero
	 */
	shrq $12, %rbx
	shlq $12, %rbx
	/* =========================================== */

.pd_present:

	/* Is PD present? */
	/* ========================================= */

	/* PDPT (RBX) present bit checking */
	testb $PG_PRESENT, (%rbx, %r14, 8)
	jz .pd_alloc
	jnz .continue_mapping
	/* ========================================= */

.pd_alloc:

	/* Push registers */
	/* ==== */
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %rbx
	/* ==== */

	/* Allocate another page */
	/* =============== */
	movq $PAGE_SIZE, %rdi
	call early_alloc
	/* =============== */

	/* Pop registers */
	/* === */
	popq %rbx
	popq %r13
	popq %r14
	popq %r15
	/* === */

	/* Write page base address  */
	/* ============================= */

	/*
	 * Returned page base address
	 * Pass it to PDPT index
	 */
	movq %rax, (%rbx, %r14, 8)

	/* Add its permissions */
	orq $PG_PERMISSIONS, (%rbx, %r15, 8)
	orq $PG_PERMISSIONS_2MB, (%rbx, %r15, 8)
	/* ============================= */

.continue_mapping:

	/* Kernel code needed pages update */
	/* ======================= */
	movq kernel_pages(%rip), %rax
	decq %rax
	movq %rax, kernel_pages(%rip)
	/* ======================= */

	/* Verify if all kernel was mapped */
	/* ============================================= */

	/*
	 * Comparing the same value will always result in 1
	 *   Unless the value is zero
	 *
	 * Being zero, we don't need to map any other pages
	 */

	testq %rax, %rax
	jz .finish
	jnz .map_another_page
	/* ============================================= */

.map_another_page:

	/* Update the virtual base */
	/* =========================== */

	/* The current base is located at RSI */
	movq %rsi, %rax
	addq $200000, %rax
	/* =========================== */

	/* Alignment */
	/* ============================================= */

	/*
	 * Notice the base address is already 2-MiB-aligned
	 */

	/* 2 >> 21 = 2MiB */
	shrq $21, %rax
	shlq $21, %rax

	/* Update it */
	movq %rax, %rsi
	/* ============================================== */

.finish:

.extern rks_main
.extern loader_info

	/* Call the main kernel function */
	movq $loader_info, %rdi
	call rks_main

/* It should not be reached */
.halt:
	cli
	hlt
	jmp .halt
