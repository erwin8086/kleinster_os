.section .text
.code16
.extern gdtr
.extern main
.extern gdtr_kern
.global _stack
.global _start
_start:
	mov $0xFFFF, %ax
	mov %ax, %ds
	mov 0x7E0E, %bx
	cmp $0xAA55, %bx
	jz .a20_enable

	not %bx
	mov %bx, 0x7E0E
	xor %ax, %ax
	mov %ax, %ds
	mov 0x7DFE, %ax
	cmp %ax, %bx 
	jnz .a20_ok

.a20_enable:
// Enable A20
	mov $0xAD, %al
	outb %al, $0x64
	mov $0xA7, %al
	outb %al, $0x64
.read_loop:
	inb $0x64, %al
	and $1, %al
	jz .read_ok

	inb $0x60, %al
	jmp .read_loop

.read_ok:
	call .wait_kbc
	mov $0x60, %al
	outb %al, $0x64
	xor %al, %al
	outb %al, $0x60

.set_a20:
	mov $0xD0, %al
	outb %al, $0x64
.wait_data:
	inb $0x64, %al
	and $1, %al
	jz .wait_data
	inb $0x60, %al
	or $2, %al
	push %ax
	call .wait_kbc
	mov $0xD1, %al
	outb %al, $0x64

	pop %ax
	outb %al, $0x60
.a20_ok:
	mov %cs, %ax
	mov %ax, %ds

	lgdt gdtr
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0
	
	ljmp $8, $.entry32

.wait_kbc:
	inb $0x64, %al
	and $1, %al
	jnz .wait_kbc
	ret
.code32
.entry32:
	mov $16, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	xor %eax, %eax
	mov %eax, %ebp
	mov $_stack, %esp
	call main
	jmp .

.global exec_kernel
exec_kernel:
	lgdt gdtr_kern
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
.byte 0xEA
.global kernel_entry
kernel_entry:
.4byte 0
.2byte 8


.section .bss
.space 1024
_stack:
