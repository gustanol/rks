.section .bss
.align 16

/* Kernel stack memory */
/* ============== */

.globl kernel_stack_top

kernel_stack_bottom:
	.skip 16384
kernel_stack_top:
/* ============== */

.section .text.long

.globl identity_long

identity_long:
.code64

.extern KERNEL_VIRT_BASE
.extern kernel_start
.extern kernel_end

	movw $0x20, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	/* Update the stack pointer */
	movq $kernel_stack_top, %rsp

	/* Prepare to map to higher-half */
	movq KERNEL_VIRT_BASE, %rsi

map_higher_half:

.extern pml4

/* Right-shifts */
/* =========== */
.equ PML4_SHR, 39
.equ PDPT_SHR, 30
.equ PD_SHR, 21
.equ PT_SHR, 12
/* =========== */

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

	/* PT index */
	/* ================= */
	movq %rsi, %rax

	/* Get the PT index */
	shrq $PT_SHR, %rax
	andq $0x1FF, %rax

	/* Store it into R12 */
	movq %rax, %r12
	/* ================= */

	/* Is PDPT present? */
	/* ========================================= */

	/* Multiply by 8 (entry = 8 bytes) */
	movq %r15, %rax
	shlq $3, %rax

	/*
	 * If the PDPT entry is present:
	 *   - It was already mapped
	 *   - entry OR 1st bit
	 *     - Should be != 0
	 *
	 * If not present:
	 *   - The comparison will be zero
	 *   - It will continue the flow and map a PDPT
	 */
	orq %rax(pml4), $0x1
	jnz .continue_pdpt
	/* ========================================= */

/*.alloc_pdpt:*/

.continue_pdpt:

	/* Is PD present? */
	/* ============== */
	movq %r14, %rax
	shlq $3, %rax
	/* ============== */

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
