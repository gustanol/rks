#ifndef _X86_LOADER_PARSER_H
#define _X86_LOADER_PARSER_H

#include <kernel/boot/info.h>
#include <stddef.h>
#include <stdint.h>

void parse_multiboot2 ( uint64_t info_ptr, struct boot_info* boot_info );

#endif /* _X86_LOADER_PARSER_H */
