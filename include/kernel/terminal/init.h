#ifndef _KERNEL_TERMINAL_INIT_H
#define _KERNEL_TERMINAL_INIT_H

#include <kernel/boot/fb.h>
#include <kernel/kerr/kerr.h>

kerr_t terminal_init ( struct framebuffer_info* fb );

#endif /* _KERNEL_CONSOLE_INIT_H */
