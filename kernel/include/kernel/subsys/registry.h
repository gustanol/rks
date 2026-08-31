#ifndef _KERNEL_SUBSYS_REGISTRY_H
#define _KERNEL_SUBSYS_REGISTRY_H

#include <stddef.h>
#include <stdint.h>

/* Generic macro to register a new subsystem backend */
#define REGISTER_SUBSYSTEM_BACKEND(BASE) \
        __attribute__ ( ( section ( ".subsys.registry" ), used ) ) \
        const struct subsys_backend* __REG_BACKEND_##__COUNTER__ = &( BASE );

/* Subsystem backend descriptor */
struct subsys_backend
{
	const char* name;
	uint32_t tag;
};

/* Return type for multiple backends lookup */
struct backends_list
{
	struct subsys_backend** arr;
	size_t count;
};

/* Prototypes */
struct subsys_backend* subsys_find_backend ( uint32_t tag );
struct backends_list* subsys_find_multiple_backends ( uint32_t tag );

#endif /* _KERNEL_SUBSYS_REGISTRY_H */
