#include <rks/args.h>
#include <rks/print.h>
#include <rks/screen/__terminal.h>

int kprintf(const char *fmt, ...)
{
	/* Extra arguments */
	va_list args;
	va_start(args, fmt);

	do
	{
		if (*fmt == '%')
		{
			fmt++;
			switch (*fmt)
			{
			case 'c':
				_terminal_putchar((char)va_arg(args, int));
				break;
			case 'd':
				_terminal_putnumber(va_arg(args, int), 10);
				break;
			case 'x':
				_terminal_putnumber(va_arg(args, int), 16);
				break;
			case 's':
				_terminal_write((const char *)va_arg(args, long));
				break;
			default:
				_terminal_putchar(*fmt);
				break;
			}
		}
		else
			_terminal_putchar(*fmt);
		fmt++;
	} while (*fmt);

	va_end(args);
	__terminal_flush();
	return 0;
}
