#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <stdlib.h>

static u8 heap[64 * 1024];

void print_adr(void *adr)
{
	int diff = (u8 *)adr - heap;

	printf("ADDRESS = 0x%06X = %8d\n", diff, diff);
}

int main(void)
{
	/* RESET */
	memalloc_init((ptr)heap, sizeof(heap));
	printf("\n--- RESET ---\n");
	print_chain();
	void *a = memalloc(32);
	void *b = memalloc(32);
	void *c = memalloc(32);
	void *d = memalloc(32);

	print_stats();

	/* CASE A */
	printf("\n--- CASE A ---\n");
	printf("Free A (expect no merge)\n");
	memfree(a);
	print_chain();

	/* RESET */
	memalloc_init((ptr)heap, sizeof(heap));
	printf("\n--- RESET ---\n");
	print_chain();
	a = memalloc(32);
	b = memalloc(32);
	c = memalloc(32);
	d = memalloc(32);

	/* CASE B */
	printf("\n--- CASE B ---\n");
	printf("Free A and then Free B (expect backwards merge)\n");
	memfree(a);
	memfree(b);
	print_chain();

	/* RESET */
	memalloc_init((ptr)heap, sizeof(heap));
	printf("\n--- RESET ---\n");
	print_chain();
	a = memalloc(32);
	b = memalloc(32);
	c = memalloc(32);
	d = memalloc(32);

	/* CASE C */
	printf("\n--- CASE C ---\n");
	printf("Free B and then Free A (expect forward merge)\n");
	memfree(b);
	memfree(a);
	print_chain();

	/* RESET */
	memalloc_init((ptr)heap, sizeof(heap));
	printf("\n--- RESET ---\n");
	print_chain();
	a = memalloc(32);
	b = memalloc(32);
	c = memalloc(32);
	d = memalloc(32);

	/* CASE D */
	printf("\n--- CASE D ---\n");
	printf("Free A and C and then Free B  (expect both-sided merge)\n");
	memfree(a);
	memfree(c);
	memfree(b);
	print_chain();

	/* RESET */
	memalloc_init((ptr)heap, sizeof(heap));
	printf("\n--- RESET ---\n");
	print_chain();

	/* STRESSTEST */

	u32 max_id = 0;
	void *ptrs[1024 * 64];
	memset(ptrs, 0, sizeof(ptrs));
	for (;;)
	{
		print_chain();

		u32 random_size = 8 + rand() % (64 - 8);
		void *p = memalloc(random_size);
		if (p == NULL)
		{
			print_stats();
			printf("YOU ARE FAILURE!!!\n");
			return 1;
		}

		for (int i = 0; i < sizeof(ptrs) / sizeof(*ptrs); ++i)
		{
			if (!ptrs[i])
			{
				ptrs[i] = p;
				++i;
				if (i > max_id)
					max_id = i;
				break;
			}
		}

		if (rand() % 100 > 80)
		{
			/* free random block */
			while (1)
			{
				int id = rand() % max_id;
				if (ptrs[id])
				{
					memfree(ptrs[id]);
					ptrs[id] = NULL;
					break;
				}
			}
		}
	}

	return 0;
}
