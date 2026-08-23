#ifndef _X86_PAGING_FLAGS
#define _X86_PAGING_FLAGS

#define PAGE_ENTRIES 512

/* Page Mapping Level 4 index */
#define PML4_IDX(VADDR) \
        ( ( ( VADDR ) >> 39 ) & 0x1FF)

/* Page Directory Pointer Table index */
#define PDPT_IDX(VADDR) \
        ( ( ( VADDR ) >> 30 ) & 0x1FF)

/* Page Directory index */
#define PD_IDX(VADDR) \
        ( ( ( VADDR ) >> 21 ) & 0x1FF)

/* Page Tabel index */
#define PT_IDX(VADDR) \
        ( ( ( VADDR ) >> 12 ) & 0x1FF)

/* Address offset within the virtual address */
#define ADDR_OFFSET(VADDR) \
        ( ( VADDR ) & 0xFFF )

/* Page entry flags masker */
#define PAGE_ADDR_MASK ( ~0xFFF )
#define ADDR_MASK(ADDR) \
        ( ( ADDR ) & ( PAGE_ADDR_MASK ) )

/* Core flags */
#define PAGE_PRESENT ( 1ULL << 0 )
#define PAGE_RW ( 1ULL << 1 )
#define PAGE_US ( 1ULL << 2 )
#define PAGE_CD ( 1ULL << 3 )
#define PAGE_ACCESSED ( 1ULL << 5 )
#define PAGE_DIRTY ( 1ULL << 6 )
#define PAGE_HUGE ( 1ULL << 7 )
#define PAGE_XD ( 1ULL << 63 )

/* Helper macro to identify already present entries */
#define IS_PRESENT(ENT) \
        ( ( ENT ) & PAGE_PRESENT )

#endif /* _X86_PAGING_FLAGS */
