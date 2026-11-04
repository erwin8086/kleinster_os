.altmacro
.macro .printPlusPlus a
	.print "\a"
.endm

.section .text
.code16
.equ STAGE2_SEG, 0x1000
.equ STAGE2_OFFSET, 0

.global _start
_start:
// BPB
jmp _entry // Bootloader start
nop
.if .==0x7c03 // Make sure its the correct offset
	.error "Offset incorrect"
.endif
.ascii "KLEINOS "
.2byte 512
.byte 1
.2byte 1
.byte 2
.2byte 64
.2byte 2880
.byte 0xF0
.2byte 9
.2byte 18
.2byte 2
.4byte 0
.4byte 0
.byte 0
.byte 0
.byte 0x29
.4byte 0xDEADBEEF
.ascii "KLEINSTEROS"
.ascii "FAT12   "
_entry:
	cli
	cld

	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov $_stack, %sp

// LOAD FAT1
	mov $0x0209, %ax
	mov $0x0002, %cx
	xor %dh, %dh
	mov $_fat, %bx
	int $0x13
	jc .error
// LOAD ROOTDIR
	mov $0x0204, %ax
	mov $0x0002, %cx
	mov $0x01, %dh
	mov $_root_dir, %bx
	int $0x13
	jc .error

	mov $_root_dir, %si
.find_loop:
	mov (%si), %al
	or %al, %al
	jz .error

	mov $11, %cx
	mov $filename, %di
	rep cmpsb
	jz .found
	add %cx, %si
	add $21, %si
	jmp .find_loop
	

.found:
	mov $STAGE2_SEG, %ax
	mov %ax, %es
	
	add $15, %si
	mov (%si), %ax
	mov 2(%si), %bx
	shr $1, %bx
	jnc .check_low
	inc %bh
	jmp .stcnt
.check_low:
	or %bl, %bl
	jz .stcnt
	inc %bh
.stcnt:
	mov %bh, _ldcnt
	xor %bx, %bx
.load:
	push %ax
	add $21, %ax
	call .translate
	int $0x13
	jc .error
	pop %ax

	mov $_fat, %si
	add %ax, %si
	shr $1, %ax
	jc .need_shift
	add %ax, %si
	mov (%si), %ax
	and $0xFFF, %ax
	jmp .next_load
.need_shift:
	add %ax, %si
	mov (%si), %ax
	shr $4, %ax

.next_load:
	add $512, %bx
	mov _ldcnt, %cl
	dec %cl
	mov %cl, _ldcnt
	jnz .load
	
	ljmp $STAGE2_SEG, $STAGE2_OFFSET

.translate:
	mov $18, %cl
	div %cl
	mov %ah, %cl
	inc %cl
	xor %dh, %dh
	shr $1, %al
	rcl $1, %dh
	mov %al, %ch
	mov $0x0201, %ax
	ret

.error:
	mov $msg, %si
	call _print_str
	jmp .

_print_str:
	mov $0x0E, %ah
	mov 0x08, %bx
.loop:
	mov (%si), %al
	or %al, %al
	jz .end
	int $0x10
	inc %si
	jmp .loop
.end:
	ret
msg:
.asciz "Boot Failed!\r\n"
ok:
.asciz "[OK]\r\n"

filename:
.ascii "STAGE2  SYS"
	
	

.section .bootsig
_boot_signature:
.2byte 0xAA55

.section .data
.space 64
_stack:
_fat:
.space 512*9

_root_dir:
.space 64*32
_ldcnt:
.byte 0
