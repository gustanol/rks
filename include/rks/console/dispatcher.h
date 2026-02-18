#ifndef __DISPATCHER_H
#define __DISPATCHER_H

#include <rks/console/console.h>

#define MAX_CONSOLES 2

struct console_driver
{
	char* name;
	void (*flush) (const struct screen_cell * buffer);
};

void console_register (struct console_driver* drv);

#endif /* __DISPATCHER_H */
