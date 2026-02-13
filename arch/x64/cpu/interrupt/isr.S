/*
 * Interrupt Service Routines to be used in IDT entries
 */

/*
 * Divide error
 * #DE
 * Fault
 */
isr0:
	pushq 0 # Error code
	pushq 0 # Interrupt number
	# Call isr common stub

/*
 * Debug Exception
 * #DB
 * Trap
 */
isr1:
	pushq 0
	pushq 1

/*
 * Non-Maskable Interrupt (NMI)
 * NMI
 * Interrupt
 */
isr2:
	pushq 0
	pushq 2

/*
 * Breakpoint
 * #BP
 * Trap
 */
isr3:
	pushq 0
	pushq 3

/*
 * Overflow
 * #OF
 * Trap
 */
isr4:
	pushq 0
	pushq 4

/*
 * Bound Range Exceeded
 * #BR
 * Fault
 */
isr5:
	pushq 0
	pushq 5

/*
 * Invalid Opcode
 * #UD
 * Fault
 */
isr6:
	pushq 0
	pushq 6

/*
 * Device not available
 * #NM
 * Fault
 */
isr7:
	pushq 0
	pushq 7

/*
 * Double fault
 * #DF
 * Abort
 */
isr8:
	// Error code already pushed by CPU
	pushq 8
	
/*
 * Invalid TSS
 * #TS
 * Fault
 */
isr10:
	pushq 10

/*
 * Segment not present
 * #NP
 * Fault
 */
isr11:
	pushq 11

/*
 * Stack segment fault
 * #SS
 * Fault
 */
isr12:
	pushq 12

/*
 * General Protection
 * #GP
 * Fault
 */
isr13:
	pushq 13

/*
 * Page fault
 * #PF
 * Fault
 */
isr14:
	pushq 14
