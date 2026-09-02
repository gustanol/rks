#include <interrupt/handlers.h>

/**
 * Generic Interrupt Service Routine handler
 * Private handler
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct interrupt_frame* frame`: Stack frame pointer
 * Callers:
 *   - `isr_common_stub`
 **/
void
_isr_generic_handler ( struct interrupt_frame* frame )
{

	/* Treats each ISR by their interrupt number */
	switch ( frame->int_num )
	{
			case PAGE_FAULT:
			{
				__page_fault_handler ( frame );
				break;
			}
	}
}
