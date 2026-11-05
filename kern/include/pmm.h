#ifndef __PMM_H
#define __PMM_H

#include <types.h>
#include <bootinfo.h>
void print_mmap(struct bootinfo *info);
uint32_t pmm_free(void);
void pmm_init(struct bootinfo *info);

#endif
