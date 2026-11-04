CC=i686-linux-musl-gcc
LD=i686-linux-musl-ld
FLAGS=-nostdinc -g -fno-PIC
CFLAGS=$(FLAGS) -m32 -march=i386
ASFLAGS=$(FLAGS)
LDFLAGS=-nostdlib -no-pie
HEADERS=$(wildcard *.h)
OBJCOPY=i686-linux-musl-objcopy
QEMU=qemu-system-i386
all: clean floppy.img

qemu-gdb: floppy.img
	$(QEMU) -chardev socket,path=/tmp/gdbsocket0,server=on,wait=off,id=gdb0 -gdb chardev:gdb0 -S -bios ./bios.bin -fda $< -monitor stdio

floppy.img: stage2.sys ../stage1/boot.bin kernel.elf
	dd if=/dev/zero bs=512 count=$(shell echo "2880-$(shell wc -c <../stage1/boot.bin)/512" | bc) | cat ../stage1/boot.bin - > floppy.img
	mcopy -i floppy.img stage2.sys ::
	mcopy -i floppy.img kernel.elf ::

kernel.elf: kernel.o
	$(LD) $(LDFLAGS) -Tkernel.ld $^

stage2.sys: stage2.elf
	$(OBJCOPY) -O binary $< $@

stage2.elf: init.o gdt.o start.o main.o bios.o
	$(LD) $(LDFLAGS) -Tstage2.ld $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf floppy.img
