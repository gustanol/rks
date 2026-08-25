#ifndef _KERNEL_CONTAINER_OF_H
#define _KERNEL_CONTAINER_OF_H

/**
 * Calculate the original object pointer from one of its members
 *
 * Arguments:
 *   1. `ptr`: Pointer to be used to recover the object
 *   2. `type`: Type of the object to be recovered
 *   3. `member`: Member name inside the object
 *
 * Explanation:
 *   - `( ( type * ) 0 )`: Pointer to type whose value is NULL
 *     - It actually doesn't care because this value will never be deferenced
 *   - That's why we can access the type of the member using `typeof`
 *     - `const typeof ( ( ( type * ) 123 )->member )`
 *
 *   - The second line effectively does:
 *     - A convertion of the temporary pointer into a `char *`
 *       - This happens because `ptr + 1` depends on the type
 *       - It's evaluated like this: `ptr + 1 * sizeof (ptr)`
 *       - If `int* ptr`, then `ptr + 1 = ptr + 4 bytes`
 *       - `char*` guarantees only one byte will be moved
 *     - A subtraction between the temporary pointer and the offset between the object and its member
 *       - The pointer is the objects base pointer + inner offset 'til the member
 *       - Removing it, we've got only the base address
 *     - A cast to the type
 **/
#define container_of(ptr, type, member) \
        ({ \
                const typeof ( ( ( type * ) 0 )->member )* __mptr = ( ptr ); \
                ( type * ) ( ( char * ) __mptr - __builtin_offsetof ( type, member ) ); \
        })

#endif /* _KERNEL_CONTAINER_OF_H */
