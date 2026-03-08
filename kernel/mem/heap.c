#include <rks/kernel/mem/heap.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/klog/print.h>

struct heap_ctx
{
	ul64 heap_start;
	struct heap_header* start;
} heap;

/*
 * Heap context initializer
 *
 * -> void
 */
void
heap_init (void)
{
	/*
	 * Calculate the amount of frames heap will need and allocate them
	 */
	u64 frames_needed = HEAP_INITIAL_SIZE / FRAME_SIZE;
	ul64 heap_start = pmm_alloc_frames (frames_needed);

	heap.heap_start = heap_start;
	heap.start = (struct heap_header *) heap_start;

	/* It will be used as start reference */
	heap.start->size = HEAP_INITIAL_SIZE - sizeof (struct heap_header);
	heap.start->is_free = true;
	heap.start->next = NULL;
}

void*
kmalloc (size_t size)
{
	struct heap_header* current = heap.start;
	do
	{
		/* Find a free block equal or bigger the given size */
		if (current->is_free && current->size >= size)
		{
			if (current->size >=
			    size + sizeof (struct heap_header) + 64)
			{
				struct heap_header* new_block =
					(struct heap_header *) ((u8 *)
								current +
								sizeof (struct
									heap_header)
								+ size);

				new_block->size =
					current->size - size -
					sizeof (struct heap_header);
				new_block->is_free = true;
				new_block->next = NULL;

				current->size = size;
				current->next = new_block;
			}

			current->is_free = false;
			return (void *) ((u8 *) current +
					 sizeof (struct heap_header));
		}

		current = current->next;
	} while (current);
	return NULL;
}

void
kfree (void* ptr)
{
	if (!ptr)
		return;

	struct heap_header* block =
		(struct heap_header *) ((u8 *) ptr -
					sizeof (struct heap_header));
	block->is_free = true;

	struct heap_header* current = heap.start;
	do
	{
		if (current->is_free && current->next->is_free)
		{
			current->size +=
				sizeof (struct heap_header) +
				current->next->size;
			current->next = current->next->next;
		}

		else
			current = current->next;
	} while (current && current->next);
}

void
test_heap (void)
{
	kprintf ("\nTesting Heap\n");

	void* ptr1 = kmalloc (128);
	void* ptr2 = kmalloc (256);

	kprintf ("ptr1=%p; ptr2=%p; ptr1=ptr2=%d\n", ptr1, ptr2,
		 ptr1 == ptr2);

	kfree (ptr1);

	void* ptr3 = kmalloc (64);
	kprintf ("ptr1=%p; ptr3=%p; ptr1=ptr3=%d\n", ptr1, ptr3,
		 ptr1 == ptr3);
}
