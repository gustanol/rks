#ifndef _KERNEL_CONSOLE_BACKEND_H
#define _KERNEL_CONSOLE_BACKEND_H

#include <kernel/boot/fb.h>
#include <kernel/container_of.h>
#include <kernel/kerr/kerr.h>
#include <kernel/susys/registry.h>

#define SUBSYS_TAG_CONSOLE 0x434F4E534F4C45

/* Console backend registration macro */
#define REGISTER_CONSOLE_BACKEND(BACKEND) \
        ({ \
                const struct console_backend* __tmp = &( BACKEND ); \
                REGISTER_SUBSYSTEM_BACKEND ( &__tmp->base ); \
        })

/* Signature macros */
#define CONSOLE_BACKEND_INIT(name) \
        kerr_t name ( const struct framebuffer_info * fb )

#define CONSOLE_BACKEND_PUTCHAR(name) \
        void name ( char c )

#define CONSOLE_BACKEND_WRITE(name) \
        void name ( char* s )

#define CONSOLE_BACKEND_FLUSH(name) \
        void name ( void )

#define CONSOLE_BACKEND_CLEAR(name) \
        void name ( void )

/* Shared operations */
struct console_ops
{

	/* Puts a single char at the end of the screen */
	CONSOLE_BACKEND_PUTCHAR ( ( *putchar ) );

	/* Writes a null-terminated string */
	CONSOLE_BACKEND_WRITE ( ( *write ) );

	/* Writes screen content to memory */
	CONSOLE_BACKEND_FLUSH ( ( *flush ) );

	/* Cleans the screen */
	CONSOLE_BACKEND_CLEAR ( ( *clear ) );
};

/* Specific Console backend context */
struct console_backend
{

	/* Base backend context */
	struct subsys_backend base;

	/* Backend init function */
	  CONSOLE_BACKEND_INIT ( ( *init ) );

	/* Console operations */
	struct console_ops* ops;
};

/**
 * Base subsystem backend generic struct to `struct console_backend`
 * Private helper
 *
 * Return -> `struct console_backend *`: Pointer to `console_backend`
 * Arguments:
 *   1. `struct subsys_backend* base`: Backend base context
 **/
static inline struct console_backend*
console_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct console_backend, base );
}

#endif /* _KERNEL_CONSOLE_BACKEND_H */
