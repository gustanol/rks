#ifndef STRING_H
#define STRING_H

#include "args.h"
#include <rks/types.h>

#define FMT_BUF_SZ 512

typedef char buf_t;

/* Number converting functions */
size_t itoa ( ul64 num, size_t base, buf_t * buf, size_t buf_s );
ul64 atoi ( buf_t * buf, size_t buf_s );

/* Format function */
void _fmt ( buf_t * buf, size_t size, const char* fmt, va_list args );

/* MEM* functions */
void* memset ( void* ptr, ubyte_t c, size_t size );

#endif /* STRING_H */
