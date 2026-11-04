.macro .fat_entry a, b
.byte \a & 0xFF
.byte (\a & 0xF00) >> 8 | (\b & 0xF) << 4
.byte (\b & 0xFF0) >> 4
.endm

.macro .fat fat
.section \fat
.fat_entry 0xFF0, 0xFFF
.fill 4317, 1, 0x00
.endm

.fat .fat1
.fat .fat2

#define FAT_TIME(h, m, s) ((h)<<11) | ((m)<<5) | (s)
#define FAT_DATE(y, m, d) ((y)<<9) | ((m)<<5) | (d)

.macro .dir_entry filename, attr, start, len
.set .start,.
.ascii "\filename"
.fill 11-(.-.start), 1, 0x20
.byte \attr
.byte 0
.byte 0 // sec/100
.2byte FAT_TIME(12, 0, 0)
.2byte FAT_DATE(26, 10, 31)
.2byte FAT_DATE(26, 10, 31)
.2byte 0
.2byte FAT_TIME(12, 0, 0)
.2byte FAT_DATE(26, 10, 32)
.2byte \start
.4byte \len
.endm

.section .fat_directory
.dir_entry KLEINSTEROS, 0x08, 0, 0
.rept 62
.dir_entry \xE5, 0, 0, 0
.endr
.dir_entry \x00, 0, 0, 0
