#include <kernel/kerr/kerr.h>
#include <kernel/mem/alloc.h>
#include <kernel/subsys/registry.h>

/* Subsystems section limits */
extern struct subsys_backend* __subsys_backend_registry_start;
extern struct subsys_backend* __subsys_backend_registry_end;

/**
 * Subsystem backend locator
 * Public helper
 *
 * Return -> `struct subsys_backend *`: Backend state pointer
 * Arguments:
 *   1. `uint32_t tag`: Subsystem tag
 * Callers:
 *   - Subsystems implemenations
 **/
struct subsys_backend*
subsys_find_backend ( uint32_t tag )
{

	/* Stores the found backend (or NULL) */
	struct subsys_backend* found = NULL;

	/* Iterate for each backend stored in the section */
	for ( struct subsys_backend * backend = __subsys_backend_registry_start;
	    backend < __subsys_backend_registry_end; backend++ )
	{

		/* Verifies if the current tag is equal to the one we're aiming */
		if ( backend->tag == tag )
		{
			found = backend;
			break;
		}
	}

	/* Return the result */
	return found;
}

/**
 * Multiple subsystem backends locator
 * Public helper
 *
 * Return -> `struct backends_list *`: List of found backends
 * Arguments:
 *   1. `uint32_t tag`: Subsystem tag
 * Callers:
 *   - Subsystems internal implementations
 *
 * - This function shouldn't be called before MM setup
 **/
struct backends_list*
subsys_find_multiple_backends ( uint32_t tag )
{

	/* Allocate the return type */
	struct backends_list* list =
	    ( struct backends_list * ) k_malloc ( sizeof ( struct backends_list ) );

	/* Stop on error */
	if ( IS_KERR ( list ) )
		return NULL;

	/* Start the count at zero */
	list->count = 0;

	/* First iteration to get the backend count */
	for ( struct subsys_backend * backend = __subsys_backend_registry_start;
	    backend < __subsys_backend_registry_end; backend++ )

	    /* Compare tags */
		if ( backend->tag == tag )
			list->count++;

	/* Allocate the array of pointers */
	list->arr = k_malloc ( sizeof ( struct subsys_backend * ) * list->count );

	/* Again, stop on error */
	if ( IS_KERR ( list ) )
		return NULL;

	/* Second iteration to set all backends */
	uint32_t i = 0;
	for ( struct subsys_backend * backend = __subsys_backend_registry_start;
	    backend < __subsys_backend_registry_end; backend++ )
	{
		if ( backend->tag == tag )
			list->arr[i++] = backend;

		/* Only break after all fillings */
		if ( i >= list->count )
			break;
	}

	/* Return the dynamic list */
	return list;
}
