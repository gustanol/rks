#include <rks/kernel/boot/multiboot2.h>
#include <rks/kernel/klog/print.h>

void*
find_multiboot2_tag (void* mbi, __u32 type)
{
	/* Skip the Multiboot2 header, which has 8 bytes */
	struct multiboot2_tag* tag =
		(struct multiboot2_tag *) ((__u8 *) mbi + 8);

	do
	{
		/* If it's the target type, return it as a pointer to void */
		if (tag->type == type)
			return (void *) tag;

		/* Get the following tag by summing the size and the base of the current one */
		__u64 next_addr = (__u64) tag + tag->size;

		/*
		 * In here, besides using the next address (next flag), we guarantee a 8-byte align
		 *
		 * This isn't stricly required, but often offers better processor performance
		 *
		 * So, basically, the least significant three bits are set to zero, because they're not divisible by 8
		 *
		 * ~7 = 11...1000: All bits are flipped
		 *
		 * E.G.:
		 * - 15 (0000 1111):
		 *   - 0000 1111 & 1111 1000 = 0000 1000 = 8
		 *
		 * - 16 (0001 0000):
		 *   - 0001 0000 & 1111 1000 = 0001 0000 = 16
		 *
		 * Every number besides the three first bits are multiple of 8
		 */
		tag = (struct multiboot2_tag *) ((next_addr + 7) & ~7);
	} while (tag->type != MULTIBOOT2_TAG_TYPE_END);	/* End tag */

	/* No tag found */
	return NULL;
}
