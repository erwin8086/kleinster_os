.section .text
.global put_str
put_str:
	push %ebp
	mov %esp, %ebp
	push %ebx
	push %esi
	push %edi

	mov 8(%ebp), %si
	call .puts_go16

	pop %edi
	pop %esi
	pop %ebx
	mov %ebp, %esp
	pop %ebp
	ret

.puts_go16:
	mov %esp, .esp_save
	jmp $0x30, $.put_clr_pe
.code16
.put_clr_pe:
	mov $0x28, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %cr0, %eax
	xor $1, %eax
	mov %eax, %cr0
	ljmp $0x1000, $.puts16
.puts16:
	mov $.tmp_stack, %sp
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov $0x0E, %ah
	mov $0x0008, %bx
.loop:
	mov (%si), %al
	or %al, %al
	jz .finish16
	int $0x10
	inc %si
	jmp .loop

.finish16:
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0
	ljmp $8, $.finish
.code32
.finish:
	mov $16, %ax
	mov %ax, %ds
	mov %ax, %ss

	mov .esp_save, %esp
	ret
.global read_sector
.global sector_buffer
read_sector:
	push %ebp
	mov %esp, %ebp
	push %ebx
	push %esi
	push %edi

	mov 8(%ebp), %eax
	mov %eax, .drv
	call .read_sector_go16

	pop %edi
	pop %esi
	pop %ebx
	mov %ebp, %esp
	pop %ebp
	ret

.read_sector_go16:
	mov %esp, .esp_save
	jmp $0x30, $.read_sector_pexit
.code16
.read_sector_pexit:
	mov $0x28, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %cr0, %eax
	xor $1, %eax
	mov %eax, %cr0
	ljmp $0x1000, $.read_sector16

.read_sector16:
	mov $.tmp_stack, %sp
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es

	mov $0x0201, %ax
	mov .sec, %cx
	mov .drv, %dx
	mov $sector_buffer, %bx
	int $0x13

	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0
	ljmp $8, $.read_sector_finish
.code32
.read_sector_finish:
	mov $16, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es

	mov .esp_save, %esp
	ret

.drv:
.byte 0
.head:
.byte 0

.sec:
.byte 0
.cyl:
.byte 0

	
	
.space 256
.tmp_stack:

sector_buffer:
.space 512

.esp_save:
.4byte 0
