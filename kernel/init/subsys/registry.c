#include <rks/kernel/init/subsystem.h>
#include <rks/kernel/mem/pmm.h>

/* Subsystems section limits */
extern struct subsys_backend* __registry_subsystem_start;
extern struct subsys_backend* __registry_subsystem_end;


/**
 * Subsystem backend locator
 * Public helper
 *
 * Return -> `struct subsys_backend *`: Backend state pointer
 * Arguments:
 *   1. `u32 tag`: Subsystem tag
 * Callers:
 *   - Subsystems implemenations
 **/
struct subsys_backend*
subsys_find_backend ( u32 tag )
{
	/* Stores the found backend (or NULL) */
	struct subsys_backend* found = NULL;

	/* Iterate for each backend stored in the section */
	for ( struct subsys_backend * backend = __registry_subsystems_start;
	      backend < __registry_subsystems_end; backend++ )
	{

		/* Verifies if the current tag is equal to the one we're aiming */
		if ( backend->tag == tag )
		{
			/* Fills out the `found` pointer */
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
 * Return -> `size_t`: Amount of the backends found
 * Arguments:
 *   1. `u32 tag`: Subsystem tag
 *   2. `struct subsys_backend** arr`: Array of backend pointers
 *   3. `size_t len`: Amount of backends to be found
 * Callers:
 *   - Subsystems internal implementations
 **/
size_t
subsys_find_multiple_backends ( u32 tag, struct subsys_backend** arr,
                                size_t len )
{
	/* Backends counter */
	size_t count = 0;

	/* Iteration throughout the section */
	for ( struct subsys_backend * backend = __registry_subsystems_start;
	      backend < __registry_subsystems_end; backend++ )
	{
		/* Compare tags */
		if ( backend->tag == tag )
		{
			/* For each case, fill the pointer array with the current backend */
			arr[count++] = backend;

			/* Only break after all fillings */
			if ( count >= len )
				break;
		}
	}

	/* Return the count */
	return count;
}
