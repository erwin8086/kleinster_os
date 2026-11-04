ENTRY(_entry);
OUTPUT_FORMAT(elf32-i386);
OUTPUT(kernel.elf);

SECTIONS
{
	PAGE_SIZE = 4096;
	. =  0xF8100000;
	PHYS = 0x100000;
	_PHYS_OFFSET = . - PHYS;
	_kernel_start = PHYS;
	_kernel_text_start = .;
	.text : AT(PHYS) {
		*(.text);
	}
	. = NEXT(PAGE_SIZE);
	_kernel_text_end = .;
	_kernel_text_size = _kernel_text_end - _kernel_text_start;
	PHYS += _kernel_text_size;
	_kernel_data_start = .;
	.data : AT(PHYS) {
	 	*(.data);
		*(.rodata);
		. = NEXT(PAGE_SIZE);
		*(.page_directory);
	}
	_kernel_data_end = .;
	_kernel_data_size = _kernel_data_end - _kernel_data_start;
	PHYS += _kernel_data_size;
	_kernel_bss_start = .;
	.bss : AT(PHYS) {
		*(.bss);
		heap_start = .;
		. += 8192;
		heap_end = .;
	}
	. = NEXT(PAGE_SIZE);
	_kernel_bss_end = .;
	_kernel_bss_size = _kernel_bss_end - _kernel_bss_start;
	PHYS += _kernel_bss_size;
	_kernel_end = PHYS;
	_entry = _start - _PHYS_OFFSET;
	_PHYS_page_table = page_table - _PHYS_OFFSET;
	_PHYS_page_directory = page_directory - _PHYS_OFFSET;
		
}
