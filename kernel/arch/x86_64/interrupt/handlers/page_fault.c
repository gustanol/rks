#include <interrupt/handlers.h>
#include <stdbool.h>

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
	/* Gets the address that caused the fault (stored at CR2) */
	uint64_t faulting_addr;
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

	/*
	   k_print ( "  Page Fault (#PF)\n" );
	   k_print ( "    Address: 0x%lx\n", faulting_addr );
	   k_print ( "    RIP: 0x%lx\n", frame->rip );
	   k_print ( "    Error: 0x%x\n", frame->err_code );
	   k_print ( "    Present: %s\n", present ? "Yes" : "No" );
	   k_print ( "    Write: %s\n", write ? "Yes" : "No" );
	   k_print ( "    User: %s\n", user ? "Yes" : "No" );
	   k_print ( "    Reserved: %s\n", reserved ? "Yes" : "No" );
	   k_print ( "    Instruction Fetch: %s\n", instruction_fetch ? "Yes" : "No" );
	 */
}
