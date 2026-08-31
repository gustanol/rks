#ifndef _KERNEL_KERR_H
#define _KERNEL_KERR_H

#include <stddef.h>
#include <stdint.h>

#define KERR_PTR_BASE 0xFFFFEE0000000000ULL
#define KERR_PTR_MASK 0xFFFFFF0000000000ULL

/* kerr_t -> tagged error convertor */
#define ENCODE_KERR(err) \
	( ( KERR_PTR_BASE ) | ( ( uint64_t ) ( err ) & 0xFFFFULL ) )

/* Encode a `kerr_t` for functions that return a pointer */
#define ENCODE_KERR_PTR(err) \
	( void * ) ENCODE_KERR ( err );

/* Checks a valid encoded kerr_t */
#define IS_KERR(ptr) \
	( ( ( uint64_t ) ( ptr ) & KERR_PTR_MASK ) == KERR_PTR_BASE )

/* Enconded kerr_t extractor */
#define GET_KERR(ptr) \
	( ( kerr_t ) ( ( uint64_t ) ( ptr ) & 0xFFFFULL ) )

#define IS_RET_OK(err) \
	( ( err ) == KERR_OK )

/* Return a kerr_t on condition pass */
#define KERR_RET_IF(cond, err, fmt, ...) \
	({ \
			if ( cond ) { \
					/* klog_err ( fmt, ##__VA_ARGS__ ); */ \
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
				   /* klog_err ( fmt, ##__VA_ARGS__ ); */ \
					return ENCODE_KERR ( err ); \
			} \
	})

/* Uses `ENCODE_KERR_PTR` */
#define KERR_RET_ENCODE_PTR_IF(cond, err, fmt, ...) \
	({ \
		if ( cond ) { \
			/* klog_err ( fmt, ##__VA_ARGS__ ); */ \
			return ENCODE_KERR_PTR ( err ); \
		} \
	})

/* Forward if `obj` is already an encoded error */
#define KERR_PROPAGATE(obj) \
	({ \
			if ( IS_KERR ( obj ) ) \
					return GET_KERR ( obj ); \
	})

/* If `obj` is a KERR, just return it (it's already encoded) */
#define KERR_PROPAGATE_ENCODE(obj) \
	({ \
			if ( IS_KERR ( obj ) ) \
					return ( obj ); \
	})

/* General error types */
typedef enum
{

/* Generate types using X-Macros */
#define X(name, str) name,
#include "kerr.def"
#undef X

	/**
	 * Errors count
	 * Shouldn't be used globally (useless)
	 **/
	__KERR_COUNT,
} kerr_t;

/* Error -> String convertor */
const char* kerr_str ( kerr_t err );

#endif /* _KERNEL_KERR_H */
