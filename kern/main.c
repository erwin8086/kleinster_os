#include <types.h>
#include <bootinfo.h>
#include <memory.h>
#include <console.h>
#include <print.h>
extern void heap_start;

void main(struct bootinfo *info)
{
	kmem_init();
	console_init();
	printf("Hello World!\nmain = %d = 0x%x = 0%o\n", (uint32_t) main, (uint32_t) main, (uint32_t) main);
	printf("HEAP = %x\n", (uint32_t) &heap_start);
	char *b = kmalloc(32);
	char *b2 = kmalloc(64);
	char *b3 = kmalloc(8);
	printf("b = %x, b2 = %x, b3 = %x\n", (uint32_t) b, (uint32_t) b2, (uint32_t) b3);
	kfree(b2);
	b2 = kmalloc(4);
	char *b4 = kmalloc(4);
	printf("b = %x, b2 = %x, b3 = %x, b4 = %x\n", (uint32_t) b, (uint32_t) b2, (uint32_t) b3, (uint32_t) b4);
	put_str("**HEAP Layout:\r\n");
	kmem_print();
	put_str("**END\r\n");
	kfree(b);
	kfree(b3);
	kfree(b2);
	kfree(b4);

	put_str("**HEAP Layout:\r\n");
	kmem_print();
	put_str("**END\r\n");

	b = kmalloc(128);
	printf("b = %x\n", (uint32_t) b);

}
