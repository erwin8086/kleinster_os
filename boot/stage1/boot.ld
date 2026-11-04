SECTIONS
{
	. = 0x7C00;
	.text : {
		_text_start = .;
		*(.text);
		_text_end = .;
		_text_len = _text_end - _text_start;
		. += 510 - _text_len;
		_data_start = .;
		*(.bootsig);
		_fat1_start = .;
		*(.fat1);
		_fat1_end = .;
		_fat1_len = _fat1_end - _fat1_start;
		. += 0x1200 - _fat1_len;
		_fat2_start = .;
		*(.fat2);
		_fat2_end = .;
		_fat2_len = _fat2_end - _fat2_start;
		. += 0x1200 - _fat2_len;
		*(.fat_directory);
	}
	. = _data_start;
	.data (OVERLAY) : {
		*(.data)
	}
}
