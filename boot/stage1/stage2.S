.section .text
// Test File for Stage2
.code16
.global _start
_start:
	mov %cs, %ax
	mov %ax, %ds
	mov $0x0E, %ah
	mov $0x0008, %bx
	mov $msg, %si
.loop:
	mov (%si), %al
	or %al, %al
	jz .
	int $0x10
	inc %si
	jmp .loop
.space 4096
msg:
.asciz "Stage2 [OK]\r\n"
