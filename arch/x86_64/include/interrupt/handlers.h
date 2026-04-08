#ifndef HANDLERS_H
#define HANDLERS_H

#include <rks/types.h>

#define PAGE_FAULT 14

/* Frame with all pushed registers */
typedef struct
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
} __attribute__ ( ( packed ) ) interrupt_frame_t;

/* Handlers declaration */
void page_fault_handler ( interrupt_frame_t * frame );

#endif /* HANDLERS_H */
