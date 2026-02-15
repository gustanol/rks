#include <rks/types.h>

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
	frame->r15 = 0;
	while (1)
		__asm__ volatile ("hlt");
}
