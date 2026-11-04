#define __ASM
#include "page.h"
.section .text
.global _start
_start:
	mov $_init, %ecx
	mov $_PHYS_page_directory, %eax
	mov %eax, %cr3
	mov %cr0, %eax
	or $CR0_PG, %eax
	mov %eax, %cr0
	jmp *%ecx
_init:
	mov $.stack, %esp
	xor %eax, %eax
	mov %eax, %ebp
	push %ebx
	call main
.hlt:
	cli
	hlt
	jmp .hlt

.section .bss
.space 4096
.stack:

.section .page_directory
.global page_directory
.global page_table
page_directory:
.4byte _PHYS_page_table + P_PRESENT | P_RW
.rept 991
	.4byte 0
.endr
.4byte _PHYS_page_table + P_PRESENT | P_RW
.rept 31
	.4byte 0
.endr

page_table:
.rept 1024
	.4byte \+*4096 + P_PRESENT | P_RW
.endr
