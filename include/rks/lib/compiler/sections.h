#ifndef _LIB_COMPILER_SECTIONS_H
#define _LIB_COMPILER_SECTIONS_H

/* Bootstrap sections */
#define __BOOT_TEXT \
        __attribute__ ( ( section ( ".bootstrap.text" ), used ) )

#define __BOOT_DATA \
        __attribute__ ( ( section ( ".bootstrap.data" ), used ) )

#define __BOOT_BSS \
        __attribute__ ( ( section ( ".bootstrap.bss" ), used ) )

#endif /* _LIB_COMPILER_SECTIONS_H */
