#ifndef _KERNEL_TERMINAL_BACKEND_H
#define _KERNEL_TERMINAL_BACKEND_H

#include <kernel/boot/fb.h>
#include <kernel/container_of.h>
#include <kernel/kerr/kerr.h>
#include <kernel/susys/registry.h>

#define SUBSYS_TAG_TERMINAL 0x434F4E534F4C45

/* Terminal backend registration macro */
#define REGISTER_TERMINAL_BACKEND(BACKEND) \
	({ \
        const struct terminal_backend* __tmp = &( BACKEND ); \
        REGISTER_SUBSYSTEM_BACKEND ( &__tmp->base ); \
    })

/**
 * Terminal backend initialization entry
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `const struct framebuffer_info* fb`: Framebuffer global state
 * Prototype
 **/
#define TERMINAL_BACKEND_INIT(name) \
    kerr_t name ( const struct framebuffer_info * fb )

/**
 * Terminal backend character array insert function
 *
 * Return -> `void`
 * Arguments:
 *   1. `uint32_t at`: Where to put within backend buffer
 *   2. `char *c`: Character array base pointer 
 *   3. `uint32_t limit`: Array limit
 * Prototype
 **/
#define TERMINAL_BACKEND_WRITE_AT(name) \
    void name ( uint32_t at, char* s, uint32_t limit )

/**
 * Terminal backend character put function
 *
 * Return -> `void`
 * Arguments:
 *   1. `uint32_t at`: Where to put within backend buffer
 *   2. `char c`: Character to put
 * Prototype
 **/
#define TERMINAL_BACKEND_PUTC_AT(name) \
    void name ( uint32_t at, char c )

/**
 * Terminal backend style setter
 *
 * Return -> `void`
 * Arguments:
 *   1. `STYLE style`: Style to be set
 * Prototype
 **/
#define TERMINAL_BACKEND_SET_STYLE(name) \
	void name ( STYLE style )

/**
 * Terminal backend data flusher (memory writer)
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Prototype
 **/
#define TERMINAL_BACKEND_FLUSH(name) \
    void name ( void )

/* Shared operations */
struct terminal_ops
{

	/**
	 * Writes character at a specific location
	 *
	 * Note the backend is responsible to determine
	 * Out of bounds locations
	 **/
	TERMINAL_BACKEND_WRITE_AT ( ( *write_at ) );

	/* Puts a single char at a specific location */
	TERMINAL_BACKEND_PUTC_AT ( ( *putc_at ) );

	/* Sets current backend style */
	TERMINAL_BACKEND_SET_STYLE ( ( *set_style ) );

	/* Writes screen content to memory */
	TERMINAL_BACKEND_FLUSH ( ( *flush ) );
};

/* Specific terminal backend context */
struct terminal_backend
{

	/* Base backend context */
	struct subsys_backend base;

	/* Backend init function */
	  TERMINAL_BACKEND_INIT ( ( *init ) );

	/* Terminal operations */
	struct terminal_ops* ops;
};

/**
 * Base subsystem backend generic struct to `struct terminal_backend`
 * Private helper
 *
 * Return -> `struct terminal_backend *`: Pointer to `terminal_backend`
 * Arguments:
 *   1. `struct subsys_backend* base`: Backend base context
 **/
static inline struct terminal_backend*
console_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct terminal_backend, base );
}

#endif /* _KERNEL_TERMINAL_BACKEND_H */
