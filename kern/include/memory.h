#ifndef __MEMORY_H
#define __MEMORY_H

#include <types.h>

void kmem_init(void);
void kmem_print(void);
void *kmalloc(size_t);
void kfree(void*);

#endif
