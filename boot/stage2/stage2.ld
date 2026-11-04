OUTPUT_FORMAT("elf32-i386")
OUTPUT("stage2.elf")
ENTRY(_entry)
SECTIONS {
	. = 0x0;
	.text : {
		*(.text_start);
		*(.text);
	}
	.data : {
		*(.data);
	}
	.bss : {
		*(.bss);
	}
	.rodata : {
		*(.rodata);
	}
}

MEMORY {
	ram (wx) : ORIGIN = 0, LENGTH = 64K
}
