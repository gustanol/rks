#ifndef _KERNEL_ALLOC_BACKEND_H
#define _KERNEL_ALLOC_BACKEND_H

#include <rks/kernel/container_of.h>
#include <rks/kernel/err/kerr.h>
#include <rks/kernel/subsys/registry.h>

#define SUBSYS_TAG_ALLOC 0x414C4C4F43

/* ALLOC backend registration macro */
#define REGISTER_ALLOC_BACKEND(BACKEND) \
        ({ \
                const struct alloc_backend* __tmp = &(BACKEND); \
                REGISTER_SUBSYSTEM_BACKEND( &__tmp->base ); \
        })

/**
 * As more than just one backend is gonna be used at the same time in this subsystem,
 * At registration time, they have to specify themselves
 **/
typedef enum
{

/* Generate all backend types */
#define X(name, id) name,
#include "backends.def"
#undef X

	ALLOC_BACKEND_COUNT,
} alloc_backend_type;

/* Backend operations */
struct alloc_ops
{
	kerr_t ( *malloc ) ( size_t );
	kerr_t ( *free ) ( void*  );
};

/* Specific ALLOC backend context structure */
struct alloc_backend
{
	/* Generic subsystem base context */
	const struct subsys_backend base;

	/* Backend type */
	alloc_backend_type type;

	/* Initializer function */
	  kerr_t ( *init ) ( void );

	/* Operations */
	const struct alloc_ops* ops;

};

/**
 * Base subsystem backend generic struct to `struct alloc_backend`
 * Private helper
 *
 * Return -> `struct alloc_backend *`: Pointer to `alloc_backend`
 * Arguments:
 *   1. `struct subsys_backend* base`: Base pointer member of `alloc_backend`
 **/
static inline struct alloc_backend*
alloc_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct alloc_backend, base );
}

#endif /* _KERNEL_ALLOC_BACKEND_H */
