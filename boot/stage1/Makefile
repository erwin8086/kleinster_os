CROSS=i686-linux-musl
OBJCOPY=$(CROSS)-objcopy
CC=$(CROSS)-gcc
LD=$(CROSS)-ld
CFLAGS=-nostdinc -g
LDFLAGS=-nostdlib -Tboot.ld -g
OBJS=boot.o fat.o
QEMU=qemu-system-i386

floppy.img: boot.bin stage2.sys
	dd if=/dev/zero bs=512 count=$(shell echo 2880 - $(shell wc -c <boot.bin) / 512 | bc) | cat boot.bin - > floppy.img
	mcopy -i floppy.img stage2.sys ::

qemu-gdb: floppy.img
	$(QEMU) -chardev socket,path=/tmp/gdbsocket0,server=on,wait=off,id=gdb0 -gdb chardev:gdb0 -S -fda $<

stage2.sys: stage2.o
	$(LD) -nostdlib -Tstage2.ld -o stage2.elf stage2.o
	$(OBJCOPY) -j .text -O binary stage2.elf stage2.sys


boot.bin: boot.elf
	$(OBJCOPY) -j .text -O binary boot.elf boot.bin

boot.elf: $(OBJS) boot.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o:%.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm boot.o boot.elf boot.bin floppy.img fat.o
