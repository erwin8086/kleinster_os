#include "memory.h"
#include "bios.h"
uint32_t smap_entries;
uint16_t lowmem_size;
struct BIOS_E820_SMAP smap[SMAP_ENTR_MAX];
void put_hex(uint32_t);

void print_map()
{
	int i;
	put_str("\r\n**Map START\r\nMemory at 0x00000000 size 0x");
	put_hex(lowmem_size*1024);
	for(i=0;i<smap_entries;i++)
	{
		put_str("\r\nMemory at 0x");
		put_hex(smap[i].base);
		put_str(" size 0x");
		put_hex(smap[i].length);
	}
	put_str("\r\n**Map END\r\n");
}

void set_bootinfo(struct bootinfo *info)
{
	int i;
	struct bootinfo_mmap *map = info->mmap;
	map->base = 0;
	map->size = lowmem_size*1024;
	map->type = 1;
	map++;
	for(i=0;i<smap_entries;i++)
	{
		map->base = smap[i].base;
		map->size = smap[i].length;
		map->type = smap[i].type;
		map++;
	}
	info->mmap_entries = smap_entries + 1;
}
