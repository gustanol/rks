/**
 * Normally, the bootloaders protocol headers must be at the start of the kernel binary
 * This preprocessed file is where all the request headers are located
 *
 * It uses preprocessed macros to detemine whether to compile a header or not
 **/

/* Bootstrap section */
.section .bootstrap, "ax"

#ifdef MULTIBOOT_SUPPORT /* If supported */
#endif MULTIBOOT_SUPPORT

#ifdef MULTIBOOT2_SUPPORT /* Defined at build time */

.align MULTIBOOT2_HEADER_ALIGN

#include <boot/vendor/protocols/multiboot2.h>

/* Constants */
.equ MULTIBOOT2_HEADER_LENGTH ( multiboot2_header_end - multiboot2_header_start )

multiboot2_header_start:

	.long MULTIBOOT2_HEADER_MAGIC /* Magic number */
	.long MULTIBOOT2_ARCHITECTURE_I386
	.long MULTIBOOT2_HEADER_LENGTH /* Header length */

	/* Checksum */
	.long -( MULTIBOOT2_HEADER_MAGIC + 0x00 + MULTIBOOT2_HEADER_LENGTH )

	/* Alignment */
	.balign MULTIBOOT2_TAG_ALIGN

multiboot2_information_request_header_start:

	.equ MULTIBOOT2_INFORMATION_REQUEST_HEADER_LENGTH ( multiboot2_information_request_header_end - multiboot2_information_request_header_start )
	.equ MULTIBOOT2_INFORMATION_REQUEST_BODY_LENGTH ( multiboot2_information_request_body_end - multiboot2_information_request_body_start )

	/* Information tag request */
	.short MULTIBOOT2_TAG_INFORMATION_REQUEST
	.short 0 /* Additional flags */
	.long MULTIBOOT2_INFORMATION_REQUEST_HEADER_LENGTH + MULTIBOOT2_INFORMATION_REQUEST_BODY_LENGTH

multiboot2_information_request_header_end:

/* Data request via tags */
multiboot2_information_request_body_start:

	.long MULTIBOOT2_TAG_TYPE_CMDLINE
	.long MULTIBOOT2_TAG_TYPE_MMAP
	.long MULTIBOOT2_TAG_TYPE_FRAMEBUFFER
	.long MULTIBOOT2_TAG_TYPE_ACPI_OLD
	.long MULTIBOOT2_TAG_TYPE_ACPI_NEW

multiboot2_information_request_body_end:

	.balign MULTIBOOT2_TAG_ALIGN
	.short MULTIBOOT2_TAG_TYPE_END
	.short 0 /* Extra flags = 0 */
	.long 8 /* Size = 8 (terminator size) */

multiboot2_header_end:

#endif /* MULTIBOOT2_SUPPORT */
