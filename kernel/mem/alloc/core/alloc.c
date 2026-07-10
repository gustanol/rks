#include <alloc/internal.h>
#include <rks/kernel/mem/alloc/init.h>

/* Statically allocate the global state Allocation context struct */
static struct alloc_ctx i_alloc;

void
alloc_init ( void )
{
	/* Sets the pointer */
	g_alloc = &i_alloc;

	/**
         * As this subsystem can have more than one registered backend,
         * We will find them as an array of pointers
         **/
	struct subsys_backend* arr[ALLOC_BACKEND_AMOUNT];

	/* Find them */
	size_t backends_found =
	        subsys_find_multiple_backends ( SUBSYS_TAG_ALLOC, arr,
	                                        ALLOC_BACKEND_AMOUNT );

	/* As a critical operation, if failed, raises a panic */
	if ( IS_KERR ( backends_found ) )
		k_panic ( kerr_str ( GET_KERR ( backends_found ) ) );

	/* For all defined backends */
	for ( size_t i = 0; i < ALLOC_BACKEND_AMOUNT; i++ )
	{
		/* Gets the current backend from the generic base struct pointer */
		struct alloc_backend* backend =
		        alloc_backend_from_base ( arr[i] );

		/* Initializes it */
		backend->init (  );

		/* Now, according to the current backend, sets its operations */
		g_alloc->backends[backend->type].ops = backend->ops;
	}

	/* Ready to be used */
	g_alloc->ready = 1;
}
