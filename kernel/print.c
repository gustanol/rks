#include <rks/args.h>
#include <rks/print.h>
#include <rks/screen/__terminal.h>
#include <rks/string.h>

/*
 * Prints a formatted text to screen
 *
 * -> `void`
 * @fmt: The string
 * @...: Extra arguments
 */
void kprintf(const char *fmt, ...)
{
	buf_t buf[FMT_BUF_SZ];

	/* Extra arguments */
	va_list args;

	va_start(args, fmt);

	/* Format the string */
	_fmt(buf, FMT_BUF_SZ, fmt, args);
	va_end(args);

	/* Write to screen */
	_terminal_write(buf);
	__terminal_flush();
}
