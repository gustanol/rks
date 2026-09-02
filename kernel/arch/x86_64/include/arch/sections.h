#ifndef _X86_64_SECTIONS_H
#define _X86_64_SECTIONS_H

#define __BOOT_TEXT \
        __attribute__ ( ( section ( ".bootstrap.text" ), used ) )

#define __BOOT_DATA \
        __attribute__ ( ( section ( ".bootstrap.data" ), used ) )

#define __BOOT_BSS \
        __attribute__ ( ( section ( ".bootstrap.bss" ), used ) )

#endif /* _X86_64_SECTIONS_H */
