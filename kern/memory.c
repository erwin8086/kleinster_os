#include <types.h>
extern void heap_start;
extern void heap_end;

#define TYPE_FREE 1
#define TYPE_USED 2

struct allocation {
	struct allocation *next;
	struct allocation *before;
	struct allocation *after;
	size_t size;
	uint32_t type;
} __attribute__((packed));

static struct allocation *first;

static void alloc_insert(struct allocation *new)
{
	struct allocation *cur = first;
	struct allocation *prev = NULL;
	if(first == NULL)
	{
		first = new;
		new->next = NULL;
		return;
	}
	while(cur->next && cur->size < new->size)
	{
		prev = cur;
		cur = cur->next;
	}
	new->next = cur;
	if(prev != NULL)
	{
		prev->next = new;
	} else {
		first = new;
	}
}

static void alloc_remove(struct allocation *old)
{
	struct allocation *cur = first;
	struct allocation *prev = NULL;
	while(cur != old)
	{
		prev = cur;
		cur = cur->next;
	}
	if(prev == NULL)
	{
		first = cur->next;
	} else {
		prev->next = cur->next;
	}
}


void kmem_init()
{
	first = (struct allocation*) &heap_start;
	first->type = TYPE_FREE;
	first->before = NULL;
	first->after = NULL;
	first->next = NULL;
	first->size = ((ptr_t) &heap_end - (ptr_t) &heap_start);
}

void kmem_print()
{
	struct allocation *cur = first;
	while(cur)
	{
		printf("[%x] size = %x, next = %x, before = %x, after = %x\n", (uint32_t) cur, (uint32_t) cur->size,
			(uint32_t) cur->next, (uint32_t) cur->before, (uint32_t) cur->after);
		cur=cur->next;
	}
}

void* kmalloc(size_t s)
{
	struct allocation *cur, *new;
	cur = first;

	s += sizeof(struct allocation);
	while(cur && ( cur->size < s || cur->type != TYPE_FREE ))
	{
		cur = cur->next;
	}
	if(!cur)
		return NULL;
	if(cur->size > (s + sizeof(struct allocation) + 1))
	{
		alloc_remove(cur);

		new = (struct allocation*) (((ptr_t) cur) + s);
		new->size = cur->size - s;
		new->type = TYPE_FREE;
		cur->size = s;

		new->before = cur;
		new->after = cur->after;
		if(new->after)
			new->after->before = new;

		cur->after = new;
		alloc_insert(cur);
		alloc_insert(new);
	}
	cur->type = TYPE_USED;

	return (void*) ( ( (ptr_t) cur ) + sizeof(struct allocation) );

}

void kfree(void *ptr)
{
	struct allocation *cur = (struct allocation*) ( ((ptr_t) ptr) - sizeof(struct allocation) );
	if(( cur->after != NULL && cur->after->type == TYPE_FREE) || ( cur->before != NULL && cur->before->type == TYPE_FREE))
	{
		if(cur->after != NULL && cur->after->type == TYPE_FREE)
		{
			cur->size += cur->after->size;
			alloc_remove(cur->after);
			cur->after = cur->after->after;

		}
		alloc_remove(cur);
		if(cur->before != NULL && cur->before->type == TYPE_FREE)
		{
			cur->before->size += cur->size;
			cur->before->after = cur->after;
			alloc_remove(cur->before);
			cur = cur->before;
		}
		if(cur->after)
			cur->after->before=cur;
		if(cur->before)
			cur->before->after=cur;
		alloc_insert(cur);
	}

	cur->type = TYPE_FREE;
}
