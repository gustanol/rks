#ifndef SCREEN_H
#define SCREEN_H

#include <rks/types.h>

void _terminal_init (void);
void _terminal_putchar (char **c);
void _terminal_write (char *c);
void _terminal_scroll (void);
void __terminal_flush (void);

#endif /* SCREEN_H */
