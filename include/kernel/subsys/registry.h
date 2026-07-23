#ifndef _KERNEL_SUBSYS_REGISTRY_H
#define _KERNEL_SUBSYS_REGISTRY_H

#include <rks/kernel/types.h>
#include <rks/lib/compiler/builtins.h>

/* Generic macro to register a new subsystem backend
 * ================================================= */
#define REGISTER_SUBSYSTEM_BACKEND(BASE) \
        __attribute__ ( ( section ( ".subsys.registry" ), used ) ) \
        const struct subsys_backend *const CONCAT ( __REG_BACKEND, BASE ) = &( BASE );

/* Subsystem backend descriptor */
struct subsys_backend
{
	const char* name;
	u32 tag;
};

/* Prototypes
 * ========== */
struct subsys_backend* subsys_find_backend ( u32 tag );
size_t subsys_find_multiple_backends ( u32 tag, struct subsys_backend** arr,
                                       size_t len );

#endif /* _KERNEL_SUBSYS_REGISTRY_H */
