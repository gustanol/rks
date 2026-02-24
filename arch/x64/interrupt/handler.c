#include <rks/types.h>
#include <rks/kernel/klog/print.h>

struct interrupt_frame_t
{
	__u64 r15,
	  r14,
	  r13,
	  r12,
	  r11,
	  r10,
	  r9;
	__u64 rbp,
	  rdi,
	  rsi,
	  rdx,
	  rcx,
	  rbx,
	  rax;
	__u64 int_num,
	  err_code;
	__u64 rip,
	  cs,
	  rflags,
	  rsp,
	  ss;
} __attribute__ ((packed));

/*
 * Interrupt Service Routine handler
 *
 * -> void
 * @frame: Stack frame pointer
 */
void
_isr_handler (struct interrupt_frame_t* frame)
{
	/*
	 * Notice it'd only work after console is been initialized
	 *
	 * It means if any interruption is been called before main function, there'll not be visible
	 *
	 * To make kernel initialize console specifications before it usually does, enable "CFG_EARLY_CONSOLE"
	 */
	kprintf ("Exception %d\n", frame->err_code);
	while (1)
		__asm__ volatile ("hlt");
}
