#ifndef _LIB_COMPILER_BUILTINS_H
#define _LIB_COMPILER_BUILTINS_H

/* Offset between the start of a type to one of its members, including paddings */
#define offsetof(type, member) \
        __builtin_offsetof ( type, member )

#endif /* _LIB_COMPILER_BUILTINS_H */
