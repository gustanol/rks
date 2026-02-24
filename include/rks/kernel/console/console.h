#ifndef SCREEN_H
#define SCREEN_H

#include <rks/types.h>

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#define SCREEN_BUFFER_SIZE (2 * SCREEN_HEIGHT * SCREEN_WIDTH)
#define CONSOLE_DEFAULT_COLOR 0x0F

struct screen_cell
{
	__u8 character;
	__u8 color;
};

void console_init (void);
void console_putchar (char** c);
void console_write (char* c);
void console_flush (void);

#endif /* SCREEN_H */
