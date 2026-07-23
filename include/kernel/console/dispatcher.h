#ifndef __DISPATCHER_H
#define __DISPATCHER_H

#include "console.h"

#define MAX_CONSOLES 2

typedef struct
{
	char* name;
	void ( *flush ) ( const screen_cell_t * buffer );
} console_driver_t;

void console_register ( console_driver_t * drv );

#endif /* __DISPATCHER_H */
