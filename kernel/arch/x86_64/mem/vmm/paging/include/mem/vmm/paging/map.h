#ifndef _X86_PAGING_MAP_H
#define _X86_PAGING_MAP_H

#include <kernel/kerr/kerr.h>
#include <kernel/mem/pmm/types.h>
#include <kernel/mem/vmm/types.h>
#include <vmm/backend.h>

/* Mapping paging operations */
VMM_BACKEND_MAP ( _paging_map );
VMM_BACKEND_UNMAP ( _paging_unmap );
VMM_BACKEND_REMAP ( _paging_remap );
VMM_BACKEND_UNMAP_IDENTITY ( _paging_unmap_identity );
VMM_BACKEND_VIRT_TO_PHYS ( _paging_virt_to_phys );
VMM_BACKEND_PHYS_TO_VIRT ( _paging_phys_to_virt );

#endif /* _X86_PAGING_MAP_H */
