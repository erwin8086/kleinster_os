.section .text_start
.code16
.global _entry
.extern _start
.global drive
_entry:
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $_stack, %sp
	xor %eax, %eax
	mov %eax, %ebp
	mov %dl, drive
	jmp _start

drive:
.byte 0
