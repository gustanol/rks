#ifndef ARCH_VMM_H
#define ARCH_VMM_H

#include <rks/kernel/mem/vmm.h>

void _arch_vmm_init ( void );
bool _arch_vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags );
bool _arch_vmm_unmap ( vaddr_t vaddr );
void _arch_vmm_dump_tables ( vaddr_t vaddr );

#endif /* ARCH_VMM_H */
