#include <interrupt/handlers.h>
#include <rks/kernel/klog/print.h>

/*
 * Generic Interrupt Service Routine handler
 * It calls specifics handlers
 *
 * -> void
 * @frame: Stack frame pointer
 *   Passed by the ISR stubs declarator
 */
void
_isr_generic_handler ( interrupt_frame_t* frame )
{
	kprintf ( "Exception %d\n", frame->int_num );

	/* Treat each ISR by their interrupt number */
	switch ( frame->int_num )
	{
	  case PAGE_FAULT:
		  {
			  page_fault_handler ( frame );
			  break;
		  }
	}
}
