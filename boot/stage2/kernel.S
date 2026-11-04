.section .text
.global _entry
_entry:
	mov $'A', %al
	mov $2, %ah
	mov %ax, 0xB8000
.hlt:
	cli
	hlt
	jmp .hlt

