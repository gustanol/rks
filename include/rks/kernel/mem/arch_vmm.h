#ifndef ARCH_VMM_H
#define ARCH_VMM_H

#include "vmm.h"

void arch_vmm_init (  );
bool arch_vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags );
void arch_vmm_unmap ( vaddr_t vaddr );

#endif /* ARCH_VMM_H */
