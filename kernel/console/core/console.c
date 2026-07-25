#include <kernel/console/init.h>
#include <kernel/mem/alloc.h>

struct console i_console;

kerr_t
console_init ( struct framebuffer_info* fb )
{

	g_console = &i_console;
	kerr err;

	/* Discover all registered backends */
	struct backends_list* backends =
	        subsys_find_multiple_backends ( SUBSYS_TAG_CONSOLE );

	KERR_RET_IF ( backends == NULL, KERR_INTERNAL,
	              "Error getting console backends" );

	KERR_RET_IF ( backends->count == 0, KERR_INTERNAL,
	              "No registered console backend" );

	g_console->backends.count = backends->count;

	g_console->backends.ops_list =
	        k_malloc ( sizeof ( struct console_ops * ) * backends->count );

	KERR_PROPAGATE ( g_console->ops_list );

	for ( uint32_t i = 0; i < backends->count; i++ )
	{
		struct console_backend* current_console_backend =
		        console_backend_from_base ( backends->arr[i] );

		err = current_console_backend->init ( fb );

		/*
		   if (!IS_RET_OK(err))
		   k_log(...);
		 */

		g_console->backends.list[i] = current_console_backend->ops;
	}

	err = k_mfree ( backends->arr );
	/*
	   if (!IS_RET_OK(err))
	   k_log(...);
	 */

	err = k_mfree ( backends );
	/*
	   if (!IS_RET_OK(err))
	   k_log(...);
	 */

	g_console->ready = true;
}
