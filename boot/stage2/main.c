#include "types.h"
#include "bios.h"
#include "elf.h"
#include "memory.h"

extern uint8_t drive;
extern uint8_t sector_buffer[512];

uint8_t fat[512*9];

struct {
	char filename[11];
	uint8_t attribute;
	uint8_t resv;
	uint8_t creat_sec;
	uint32_t tm_creat;
	uint16_t date_lastaccess;
	uint16_t cluster_high;
	uint32_t tm_mod;
	uint16_t cluster;
	uint32_t size;
} __attribute__((packed)) directory[64];

char hexbuf[9];
char *hex = "0123456789ABCDEF";
void read_blocks(uint8_t *buffer, uint32_t start, uint8_t len);

void initfat()
{
	read_blocks(fat, 1, 9);
	read_blocks((uint8_t*)directory, 9+9+1, 4);
}

void put_hex(uint32_t num)
{
	int i;
	for(i=7;i>=0;i--)
	{
		hexbuf[i] = hex[num & 0xF];
		num >>= 4;
	}
	hexbuf[8] = 0;
	put_str(hexbuf);
}

void read_block(uint32_t block)
{
	read_sector(DRV_CHS(drive, TRACK(block), HEAD(block), SECTOR(block)));
}

void memcopy(uint8_t *dest, uint8_t *src, uint32_t len)
{
	while(len-- > 0)
	{
		*dest++ = *src++;
	}
}

int memcompare(uint8_t *a, uint8_t *b, uint32_t len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(*a++ != *b++)
		{
			return 0;
		}
	}
	return 1;
}

void read_blocks(uint8_t *buffer, uint32_t start, uint8_t len)
{
	int i;
	for(i=0;i<len;i++)
	{
		read_block(start++);
		memcopy(buffer, sector_buffer, 512);
		buffer += 512;
	}
}

char labelbuffer[12];

#define VOLUME_ID 0x8
#define DIRECTORY 0x10

char* fatlabel()
{
	int i;
	for(i=0;i<64;i++)
	{
		if(directory[i].attribute & VOLUME_ID)
		{
			memcopy(labelbuffer, directory[i].filename, 11);
			labelbuffer[11] = 0;
			return labelbuffer;
		}
	}
	return "NO ID!";
}

int fatfind(char *filename)
{
	int i;
	for(i=0;i<64;i++)
	{
		if(! (directory[i].attribute & (VOLUME_ID | DIRECTORY)))
		{
			if(memcompare(directory[i].filename, filename, 11))
			{
				return i;
			}
		}
	}
	return -1;
}

uint16_t next_cluster(uint16_t cluster)
{
	uint16_t offset = cluster + (cluster/2);
	uint16_t value = *( (uint16_t*) &fat[offset] );
	return (cluster & 1) ? value >> 4 : value & 0xFFF;
}

int fatread(int file, uint8_t *buffer, uint32_t pos, uint32_t len)
{
	uint16_t cluster = directory[file].cluster;
	uint32_t cur = 0;
	uint16_t start_cluster = pos / 512;
	uint16_t offset = pos % 512;
	uint16_t cur_len;
	if(pos > directory[file].size)
		return -1;
	while(start_cluster > 0)
	{
		cluster = next_cluster(cluster);
		start_cluster--;
	}

	while(len > 0)
	{
		read_block(cluster + 21);
		cur_len = (512 - offset) >= len ? len : (512 - offset);
		memcopy(buffer, sector_buffer + offset, cur_len);
		buffer += cur_len;
		len -= cur_len;
		cluster = next_cluster(cluster);
		offset = 0;
	}
	return 0;
}
void read_textfile(char *filename)
{
	char buffer[32];
	int file = fatfind(filename);
	if(file < 0)
		return;
	uint32_t pos = 0;
	uint32_t len = directory[file].size;
	uint16_t cur_len;
	while(len > 0)
	{
		cur_len = len > 31 ? 31 : len;
		fatread(file, buffer, pos, cur_len);
		len -= cur_len;
		pos += cur_len;
		buffer[cur_len] = 0;
		put_str(buffer);
	}

}

Elf32_Ehdr elf_header;
Elf32_Phdr p_header[MAX_PHDR];
extern uint32_t kernel_entry;

void memset(uint8_t *dest, uint8_t src, uint32_t len)
{
	while(len-- > 0)
		*dest++ = src;
}


int loadelf(int file)
{
	int i;
	uint8_t *dest;
	fatread(file, (uint8_t*)&elf_header, 0, sizeof(elf_header));
	if(elf_header.e_ident[0] != 0x7f ||
	   elf_header.e_ident[1] != 'E'  ||
	   elf_header.e_ident[2] != 'L'  ||
	   elf_header.e_ident[3] != 'F')
	{
		put_str("Not ELF!\r\n");
		return -1;
	}

	if(elf_header.e_ident[4] != 1)
	{
		put_str("Not 32Bit!\r\n");
		return -1;
	}

	if(elf_header.e_ident[5] != 1)
	{
		put_str("Not LE!\r\n");
		return -1;
	}

	if(elf_header.e_ident[6] != 1)
	{
		put_str("Not Version 1\r\n");
		return -1;
	}

	kernel_entry = elf_header.e_entry;

	if(elf_header.e_phnum > MAX_PHDR)
	{
		put_str("Too many Programm headers!\r\n");
		return -1;
	}

	if(elf_header.e_phentsize != sizeof(Elf32_Phdr))
	{
		put_str("Wrong Program header size\r\n");
		return -1;
	}

	fatread(file, (uint8_t*) p_header, elf_header.e_phoff, elf_header.e_phentsize*elf_header.e_phnum);
	for(i=0;i<elf_header.e_phnum;i++)
	{
		if(p_header[i].p_type == 1)
		{
			dest=p_header[i].p_paddr-0x10000;
			if(p_header[i].p_filesz > 0)
			{
				fatread(file, dest, p_header[i].p_offset, p_header[i].p_filesz);
				dest += p_header[i].p_filesz;
			}
			if(p_header[i].p_filesz < p_header[i].p_memsz)
			{
				memset(dest, 0, p_header[i].p_memsz-p_header[i].p_filesz); 
			}
		}
	}
	return 0;

}

void exec_kernel(uint32_t);

struct bootinfo boot_info;

void main(void)
{
	uint16_t *bootsig = (uint16_t*) (sector_buffer+510);
	put_str("Hello World!\r\nBootdrive = ");
	put_hex(drive);
	print_map();
	set_bootinfo(&boot_info);
	boot_info.ident = 0xB0;
	boot_info.boot_drive = drive;
	initfat();
	put_str("\r\nKERNEL.ELF = ");
	int file = fatfind("KERNEL  ELF");
	if(file < 0)
	{
		put_str("Not Found!\r\n");
	} else {
		loadelf(file);
		put_str("OK\r\nEntry = ");
		put_hex(kernel_entry);
		exec_kernel(((uint32_t)&boot_info)-0x1000);
	}
	while(1) {
		asm("cli\nhlt");
	}
}
