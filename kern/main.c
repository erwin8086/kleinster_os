#include <types.h>
#include <bootinfo.h>
#include <memory.h>
#include <console.h>
#include <print.h>
#include <pmm.h>
extern void heap_start;

void main(struct bootinfo *info)
{
	kmem_init();
	console_init();
	printf("Hello World!\nmain = %d = 0x%x = 0%o\n", (uint32_t) main, (uint32_t) main, (uint32_t) main);
	printf("HEAP = %x\n", (uint32_t) &heap_start);
	printf("BootInfo -> %x, version = %x\n", (uint32_t) info, info->ident);
	print_mmap(info);
	pmm_init(info);
	printf("PHYS Memory Free: %f\n", pmm_free());

}
