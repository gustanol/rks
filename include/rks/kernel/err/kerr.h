#ifndef _KERNEL_ERR_H
#define _KERNEL_ERR_H

/* General error types */
typedef enum
{

/* Generate types using X-Macros */
#define X(name, str) name
#include "kerr.def"
#undef X

	/**
         * Errors count
         * Shouldn't be used globally (useless)
         **/
	__KERR_COUNT
} kerr_t;

/* Error -> String convertor */
const char* kerr_str ( kerr_t err );

#endif /* _KERNEL_ERR_H */
