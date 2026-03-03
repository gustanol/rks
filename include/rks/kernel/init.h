#ifndef INIT_H
#define INIT_H

typedef void (*init_fn_t) (void);

#define DECLARE_DRIVER_INIT(func) \
        __attribute__((section(".drivers_init"), used)) \
        static init_fn_t __init_ptr_##func = func

void arch_early_init (void);

#endif /* INIT_H */
