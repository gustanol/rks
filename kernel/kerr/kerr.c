#include <kernel/kerr/kerr.h>

/**
 * Generated array of pointers to error strings
 *
 * It uses X-Macros to generate at the correct order
 **/
static const char const* kerr_strings[__KERR_COUNT] = {

/* CORRECT THE INCLUDE PATH IF IT CHANGES LATER */
#define X(name, str) [name] = str,
#include <kerr.def>
#undef X
};

/**
 * Convertor from error value to its string message
 * Public wrapper
 *
 * Return -> `const char *`: The message
 * Arguments:
 *   1. `kerr_t err`: Error value to be converted
 * Callers:
 *   - Many
 **/
const char*
kerr_str ( kerr_t err )
{

	/**
         * Simply gets the associated message
         * Complexity: O(1)
         **/
	return kerr_strings[err];
}
