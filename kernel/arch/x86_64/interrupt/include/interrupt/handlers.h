#ifndef _X86_INTERRUPT_HANDLERS_H
#define _X86_INTERRUPT_HANDLERS_H

#include <stddef.h>
#include <stdint.h>

#define PAGE_FAULT 14

/* Frame with all pushed registers */
struct _interrupt_frame
{
	uint64_t r15,
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
	uint64_t int_num,
	  err_code;
	uint64_t rip,
	  cs,
	  rflags,
	  rsp,
	  ss;
} _attribute__ ( ( packed ) );

/* Handlers declaration */
void __page_fault_handler ( struct interrupt_frame* frame );

#endif /* _X86_INTERRUPT_HANDLERS_H */
