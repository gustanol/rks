#ifndef _KERNEL_ERR_H
#define _KERNEL_ERR_H

#include <rks/kernel/mem/vmm/types.h>

#define KERR_PTR_BASE 0xFFFFEE0000000000ULL
#define KERR_PTR_MASK 0xFFFFFF0000000000ULL

/* kerr_t -> tagged error convertor */
#define ENCODE_KERR(err) \
        ( ( void * ) ( KERR_PTR_BASE | ( ( vaddr_t ) ( err ) & 0xFFFFULL ) ) )

/* Checks a valid encoded kerr_t */
#define IS_KERR(ptr) \
        ( ( ( vaddr_t ) ( ptr ) & KERR_PTR_MASK ) == KERR_PTR_BASE )

/* Enconded kerr_t extractor */
#define GET_KERR(ptr) \
        ( ( kerr_t ) ( ( vaddr_t ) ( ptr ) & 0xFFFFULL ) )

#define IS_RET_OK(err) \
        ( ( err ) == KERR_OK )

/* Return a kerr_t on condition pass */
#define KERR_RET_IF(cond, err, fmt, ...) \
        ({ \
                if ( cond ) { \
                        klog_err ( fmt, ##__VA_ARGS__ ); \
                        return ( err ); \
                } \
        })

/**
 * Instead of returning the error code directly, encode it first
 * For functions that does not return a `kerr_t`
 **/
#define KERR_RET_ENCODE_IF(cond, err, fmt, ...) \
        ({ \
                if ( cond ) { \
                        klog_err ( fmt, ##__VA_ARGS__ ); \
                        return ENCODE_KERR ( err ); \
                } \
        })

/* Forward if `obj` is already an encoded error */
#define KERR_PROPAGATE(obj) \
        ({ \
                if ( IS_KERR ( obj ) ) \
                        return GET_KERR ( obj ); \
        })

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
