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
 * Terminal backend initialization entry.
 *
 * @name:
 *      Name of the backend initialization function.
 *
 * Returns:
 *      KERR_NONE on success.
 *      Appropriate error code otherwise.
 *
 * Parameters:
 *      fb  Framebuffer information supplied by the terminal subsystem.
 *
 * This is the required prototype for every terminal backend.
 **/
#define TERMINAL_BACKEND_INIT(name) \
        kerr_t name ( const struct framebuffer_info * fb )

#define TERMINAL_BACKEND_PUTCHAR(name) \
        void name ( char c )

#define TERMINAL_BACKEND_WRITE(name) \
        void name ( char* s )

#define TERMINAL_BACKEND_FLUSH(name) \
        void name ( char* buf, uint32_t width, uint32_t height )

#define TERMINAL_BACKEND_CLEAR(name) \
        void name ( void )

/* Shared operations */
struct terminal_ops
{

	/* Puts a single char at the end of the screen */
	TERMINAL_BACKEND_PUTCHAR ( ( *putchar ) );

	/* Writes a null-terminated string */
	TERMINAL_BACKEND_WRITE ( ( *write ) );

	/* Writes screen content to memory */
	TERMINAL_BACKEND_FLUSH ( ( *flush ) );

	/* Cleans the screen */
	TERMINAL_BACKEND_CLEAR ( ( *clear ) );
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
