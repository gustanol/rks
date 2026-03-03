#ifndef STRING_H
#define STRING_H

#include "args.h"
#include <rks/types.h>

#define FMT_BUF_SZ 512

typedef char buf_t;

size_t itoa (ul64 num, size_t base, buf_t * buf, size_t buf_s);
ul64 atoi (buf_t * buf, size_t buf_s);
void _fmt (buf_t * buf, size_t size, const char* fmt, va_list args);

#endif /* STRING_H */
