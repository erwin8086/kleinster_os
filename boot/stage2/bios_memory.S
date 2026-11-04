#define __ASM
#include "memory.h"

.section .text
.code16
.extern smap
.extern smap_entries
.extern lowmem_size

#define ENTR_BASE(entr) entr*20
#define ENTR_SIZE(entr) entr*20+8
#define ENTR_TYPE(entr) entr*20+16

.global _get_memory_map
_get_memory_map:
	mov $0xE820, %eax
	mov $0x534D4150, %edx
	xor %ebx, %ebx
	mov $SMAP_SIZE, %ecx
	mov $smap, %di
	int $0x15
	jc .no_smap

	cmp $0x534D4150, %eax
	jnz .no_smap
	add %cx, %di
.loop:
	push %cx
	mov $SMAP_SIZE, %ax
	sub %cx, %ax
	mov %ax, %cx
	mov $0xE820, %eax
	mov $0x534D4150, %edx
	int $0x15
	jc .loop_end
	or %ebx, %ebx
	jz .loop_end
	add %cx, %di

	pop %ax
	add %ax, %cx
	jmp .loop

.loop_end:
	pop %ax
	add %ax, %cx

	mov %cx, %ax
	mov $20, %cl
	div %cl
	
	and $0xFF, %eax
	mov %eax, smap_entries
	jmp .pcmap
	
.no_smap:
	mov $0xE801, %ax
	int $0x15
	jc .map_88

	or %ax, %ax
	jnz .got_map
	or %bx, %bx
	jnz .got_map

	mov %cx, %ax
	mov %dx, %bx
.got_map:
	mov $smap, %bp
	mov $0x100000, %ecx
	mov %ecx, ENTR_BASE(0)(%bp)
	mov $1, %ecx
	mov %ecx, ENTR_TYPE(0)(%bp)
	and $0xFFFF, %eax
	shl $10, %eax
	mov %eax, ENTR_SIZE(0)(%bp)

	or %bx, %bx
	jz .got_map1

	mov $0x1000000, %ecx
	mov %ecx, ENTR_BASE(1)(%bp)
	mov $1, %ecx
	mov %ecx, ENTR_TYPE(1)(%bp)
	and $0xFFFF, %ebx
	shl $16, %ebx
	mov %ebx, ENTR_SIZE(1)(%bp)

	mov $2, %eax
	mov %eax, smap_entries
	jmp .pcmap

.got_map1:
	mov $1, %eax
	mov %eax, smap_entries
	jmp .pcmap

.map_88:
	mov $smap, %bp

	mov $0x88, %ah
	int $0x15

	and $0xFFFF, %eax
	shl $10, %eax
	mov %eax, ENTR_SIZE(0)(%bp)
	mov $0x100000, %eax
	mov %eax, ENTR_BASE(0)(%bp)
	mov $1, %eax
	mov %eax, ENTR_TYPE(0)(%bp)
	mov %eax, smap_entries

.pcmap:
	int $0x12
	mov %ax, lowmem_size
	ret

	
