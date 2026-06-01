#ifndef LOADER_PARSER_H
#define LOADER_PARSER_H

#include <rks/kernel/boot/info.h>

void _parse_mbi2 ( addr_t info_ptr, struct boot_info* boot_info );

#endif /* LOADER_PARSER_H */
