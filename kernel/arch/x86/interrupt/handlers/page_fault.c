#include <rks/kernel/klog/print.h>
#include <x86/interrupt/internal/handlers.h>

/**
 * Page Fault handler
 * Private handler
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct interrupt_frame* frame`: Stack frame pointer
 * Callers:
 *   - `_isr_generic_handler`
 **/
void
__page_fault_handler ( struct interrupt_frame* frame )
{
	/* Gets the address that caused the fault (stored at CR2)
         * ====================================================== */
	addr_t faulting_addr;
	__asm__ volatile ( "mov %%cr2, %0":"=r" ( faulting_addr ) );

	/* Present page */
	bool present = frame->err_code & 0x1;

	/* Writable page */
	bool write = frame->err_code & 0x2;

	/* Page user (0 for the kernel) */
	bool user = frame->err_code & 0x4;

	/* Reserved page */
	bool reserved = frame->err_code & 0x8;
	bool instruction_fetch = frame->err_code & 0x10;

	kprintf ( "  Page Fault (#PF)\n" );
	kprintf ( "    Address: 0x%lx\n", faulting_addr );
	kprintf ( "    RIP: 0x%lx\n", frame->rip );
	kprintf ( "    Error: 0x%x\n", frame->err_code );
	kprintf ( "    Present: %s\n", present ? "Yes" : "No" );
	kprintf ( "    Write: %s\n", write ? "Yes" : "No" );
	kprintf ( "    User: %s\n", user ? "Yes" : "No" );
	kprintf ( "    Reserved: %s\n", reserved ? "Yes" : "No" );
	kprintf ( "    Instruction Fetch: %s\n",
	          instruction_fetch ? "Yes" : "No" );
}
