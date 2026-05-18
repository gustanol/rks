#ifndef SUBSYS_REGISTRY_H
#define SUBSYS_REGISTRY_H

#include <rks/types.h>

/* Generic macro to register a new subsystem backend
 * ================================================================== */
#define REGISTER_SUBSYSTEM_BACKEND(SUBSYSTEM_TAG, NAME, INIT_FN, OPS) \
        __attribute__ ( ( section ( ".registry.subsystems" ), used ) ) \
        static const struct subsys_backend \
        __backend_##SUBSYSTEM_TAG##_##NAME = { \
                .name = ( const char * ) ( NAME ), \
                .tag = ( u32 ) ( SUBSYSTEM_TAG ), \
                .init = ( init_fn_t ) ( INIT_FN ), \
                .ops = ( const void * ) ( OPS ), \
        }

/* ================================================================== */

/* Subsystem backend descriptor */
struct subsys_backend
{
	const char* name;
	u32 tag;
	init_fn_t init;
	const void* ops;
};

/* Prototypes */
struct subsys_backend* subsys_find_backend ( u32 tag );
size_t subsys_find_multiple_backends ( u32 tag, struct subsys_backend** arr,
                                       size_t len );

#endif /* SUBSYS_REGISTRY_H */
