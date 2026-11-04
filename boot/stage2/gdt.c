#include "types.h"
#include "gdt.h"

gdt_t gdt[7] = {
	0LL,
	GDT_CODE(0x10000LL, 0xFFFFFFLL, 0LL), // 0x8
	GDT_DATA(0x10000LL, 0xFFFFFFLL, 0LL), // 0x10
	GDT_CODE(0x10000LL, 0xFFFFFFLL, 3LL), // 0x18
	GDT_DATA(0x10000LL, 0xFFFFFFLL, 3LL), // 0x20
	GDT16_DATA(0x10000LL, 0xFFFFLL, 0LL), // 0x28
	GDT16_CODE(0x10000LL, 0xFFFFLL, 0LL)  // 0x30
};

gdt_t gdt_kern[3] = {
	0LL,
	GDT_CODE(0LL, 0xFFFFFFFFLL, 0LL),
	GDT_DATA(0LL, 0xFFFFFFFFLL, 0LL)
};
struct GDTR gdtr = { sizeof(gdt), ((uint32_t) gdt) + 0x10000 };
struct GDTR gdtr_kern = { sizeof(gdt_kern), ((uint32_t) gdt_kern) + 0x10000 };
