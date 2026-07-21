#ifndef HANDLERS_H
#define HANDLERS_H

#include <rks/kernel/types.h>

#define PAGE_FAULT 14

/* Frame with all pushed registers */
struct __attribute__ ( ( packed ) )
{
	reg_t r15,
	  r14,
	  r13,
	  r12,
	  r11,
	  r10,
	  r9,
	  rbp,
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
} interrupt_frame;

/* Handlers declaration */
void __page_fault_handler ( struct interrupt_frame* frame );

#endif /* HANDLERS_H */
