CROSS=i686-linux-musl
OBJS=start.o main.o console.o print.o memory.o
CC=$(CROSS)-gcc
LD=$(CROSS)-ld
OBJCOPY=$(CROSS)-objcopy
FLAGS=-nostdinc -I../boot/include -Iinclude
CFLAGS=$(FLAGS)
ASFLAGS=$(FLAGS)
LDFLAGS=-nostdlib
HEADERS=include/types.h include/console.h include/print.h include/memory.h

all: clean floppy.img

floppy.img: kernel.elf ../boot/stage1/boot.bin ../boot/stage2/stage2.sys
	dd if=/dev/zero bs=512 count=$(shell echo 2880 - $(shell wc -c <../boot/stage1/boot.bin) / 512 | bc) | cat ../boot/stage1/boot.bin - > floppy.img
	mcopy -i floppy.img ../boot/stage2/stage2.sys ::
	mcopy -i floppy.img kernel.elf ::


kernel.elf: $(OBJS) kernel.ld
	$(LD) $(LDFLAGS) -Tkernel.ld $(OBJS)

%.o: %.S $(HEADERS)
	$(CC) $(ASFLAGS) -c -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.elf *.o floppy.img
