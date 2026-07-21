/*
 * Interrupt Service Routines to be used in IDT entries
 */

/*
 * Divide error
 * #DE
 * Fault
 */
.globl isr0
isr0:
	pushq $0 # Error code
	pushq $0 # Interrupt number
	call isr_common_stub

/*
 * Debug Exception
 * #DB
 * Trap
 */
.globl isr1
isr1:
	pushq $0
	pushq $1
	call isr_common_stub

/*
 * Non-Maskable Interrupt (NMI)
 * NMI
 * Interrupt
 */
.globl isr2
isr2:
	pushq $0
	pushq $2
	call isr_common_stub

/*
 * Breakpoint
 * #BP
 * Trap
 */
.globl isr3
isr3:
	pushq $0
	pushq $3
	call isr_common_stub

/*
 * Overflow
 * #OF
 * Trap
 */
.globl isr4
isr4:
	pushq $0
	pushq $4
	call isr_common_stub

/*
 * Bound Range Exceeded
 * #BR
 * Fault
 */
.globl isr5
isr5:
	pushq $0
	pushq $5
	call isr_common_stub

/*
 * Invalid Opcode
 * #UD
 * Fault
 */
.globl isr6
isr6:
	pushq $0
	pushq $6
	call isr_common_stub

/*
 * Device not available
 * #NM
 * Fault
 */
.globl isr7
isr7:
	pushq $0
	pushq $7
	call isr_common_stub

/*
 * Double fault
 * #DF
 * Abort
 */
.globl isr8
isr8:
	// Error code already pushed by CPU
	pushq $8
	call isr_common_stub
	
/*
 * Invalid TSS
 * #TS
 * Fault
 */
.globl isr10
isr10:
	pushq $10
	call isr_common_stub

/*
 * Segment not present
 * #NP
 * Fault
 */
.globl isr11
isr11:
	pushq $11
	call isr_common_stub

/*
 * Stack segment fault
 * #SS
 * Fault
 */
.globl isr12
isr12:
	pushq $12
	call isr_common_stub

/*
 * General Protection
 * #GP
 * Fault
 */
.globl isr13
isr13:
	pushq $13
	call isr_common_stub

/*
 * Page fault
 * #PF
 * Fault
 */
.globl isr14
isr14:
	pushq $14
	call isr_common_stub

isr_common_stub:
.extern _isr_generic_handler

	/*
	 * Save all registers
	 *
	 * This order will reflect in fields organization of interrupt stack frame struct
	 */
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	/* Pass the stack frame as argument */
	movq %rsp, %rdi
	call _isr_generic_handler

	/* Restore all registers */
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax

	/*
	 * Notice that, after we pop all pushed registers, there's still the error code and the interruption number
	 *
	 * For them, we set the stack pointer 16 bytes up to overwrite them later
	 */
	addq $16, %rsp
	iretq
