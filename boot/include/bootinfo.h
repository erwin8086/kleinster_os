#ifndef __BOOTINFO_H
#define __BOOTINFO_H

struct bootinfo_mmap {
	uint32_t base;
	uint32_t size;
	uint32_t type;
};

#define MMAP_MAX_ENTR 32

struct bootinfo {
	uint8_t ident; // 0xB0 = Version 0
	uint8_t boot_drive;
	uint8_t mmap_entries;
	struct bootinfo_mmap mmap[MMAP_MAX_ENTR];
};

#endif

