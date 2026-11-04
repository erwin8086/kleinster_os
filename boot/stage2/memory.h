#ifndef __MEMORY_H
#define __MEMORY_H

#ifndef __ASM
#include "types.h"
#include <bootinfo.h>

struct BIOS_E820_SMAP {
	uint64_t base;
	uint64_t length;
	uint32_t type;

} __attribute__((packed));

void print_map(void);
void set_bootinfo(struct bootinfo *info);

#endif

#define SMAP_ENTR_MAX 32
#define SMAP_SIZE SMAP_ENTR_MAX*20
#endif
