#include <interrupt/handlers.h>
#include <rks/kernel/klog/print.h>

/*
 * Function to handle page faults
 *
 * -> void
 * @frame: Stack frame pointer
 */
void
page_fault_handler ( interrupt_frame_t* frame )
{
	/* Get the address that caused the fault (stored at CR2) */
	addr_t faulting_addr;
	__asm__ volatile ( "mov %%cr2, %0":"=r" ( faulting_addr ) );

	/* Present page */
	bool present = frame->err_code & 0x1;
	bool write = frame->err_code & 0x2;
	bool user = frame->err_code & 0x4;
	bool reserved = frame->err_code & 0x8;
	bool instruction_fetch = frame->err_code & 0x10;

	kprintf ( "Page Fault\n\n" );
	kprintf ( "Address: 0x%lx\n", faulting_addr );
	kprintf ( "RIP: 0x%lx\n", frame->rip );
	kprintf ( "Error: 0x%x\n", frame->err_code );
	kprintf ( "Present: %s\n", present ? "Yes" : "No" );
	kprintf ( "Write: %s\n", write ? "Yes" : "No" );
	kprintf ( "User: %s\n", user ? "Yes" : "No" );
	kprintf ( "Reserved: %s\n", reserved ? "Yes" : "No" );
	kprintf ( "Instruction Fetch: %s\n", instruction_fetch ? "Yes" : "No" );
}
