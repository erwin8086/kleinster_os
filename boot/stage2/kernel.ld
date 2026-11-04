OUTPUT_FORMAT("elf32-i386")
OUTPUT("kernel.elf")
ENTRY(_entry)
SECTIONS {
	. = 0x100000;
	.text : {
		*(.text);
	}
}
