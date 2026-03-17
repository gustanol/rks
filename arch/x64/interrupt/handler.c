#include <rks/types.h>
#include <rks/kernel/klog/print.h>

typedef struct
{
	u64 r15,
	  r14,
	  r13,
	  r12,
	  r11,
	  r10,
	  r9;
	u64 rbp,
	  rdi,
	  rsi,
	  rdx,
	  rcx,
	  rbx,
	  rax;
	u64 int_num,
	  err_code;
	u64 rip,
	  cs,
	  rflags,
	  rsp,
	  ss;
} __attribute__ ( ( packed ) ) interrupt_frame_t;

/*
 * Interrupt Service Routine handler
 *
 * -> void
 * @frame: Stack frame pointer
 */
void
_isr_handler ( interrupt_frame_t* frame )
{
	/*
	 * Notice it'd only work after console is been initialized
	 *
	 * It means if any interruption is been called before main function, there'll not be visible
	 *
	 * To make kernel initialize console specifications before it usually does, enable "CFG_EARLY_CONSOLE"
	 */
	kprintf ( "Exception %d\n", frame->err_code );
	while ( 1 )
		__asm__ volatile ( "hlt" );
}
