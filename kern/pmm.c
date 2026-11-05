#include <pmm.h>
#include <console.h>
#include <print.h>

#define PHYS_PAGES (0x100000)

#define PHYS_SET(page) phys_map[page/32] |= 1 << (page % 32)
#define PHYS_UNSET(page) phys_map[page/32] &= ~( 1 << (page % 32))
#define PHYS_IS(page) (phys_map[page/32] & ( 1 << (page % 32) ) )

#define ADDR2PAGE(addr) (addr/4096)

#define PHYS_ENTRIES (PHYS_PAGES/32)

#if PHYS_PAGES % 32 != 0
	#error PHYS_PAGES
#endif
extern void _kernel_start;
extern void _kernel_end;

static uint32_t phys_map[PHYS_ENTRIES];

void print_mmap(struct bootinfo *info)
{
	int i = info->mmap_entries;
	struct bootinfo_mmap *mmap = info->mmap;
	put_str("**Memory MAP\r\n");
	while(i-->0)
	{
		printf("Memory at %x size %x is %s\n", mmap->base, mmap->size, mmap->type != 1 ? "USED" : "FREE");
		mmap++;
	}
	put_str("**END\r\n");

}

uint32_t pmm_free(void)
{
	uint32_t pages = 0;
	uint32_t i;
	for(i=0;i<PHYS_PAGES;i++)
		if(!PHYS_IS(i))
			pages++;
	return pages * 4096;	
}

void pmm_init(struct bootinfo *info)
{
	uint32_t i;
	uint32_t page, count;
	struct bootinfo_mmap *mmap = info->mmap;
	for(i=0;i<PHYS_ENTRIES;i++)
	{
		phys_map[i] = ~0;
	}

	i=info->mmap_entries;
	while(i-->0)
	{
		if(mmap->type == 1)
		{
			page = ADDR2PAGE(mmap->base);
			count = mmap->size / 4096;
			while(count-- > 0)
			{
				PHYS_UNSET(page);
				page++;
			}
		}
		mmap++;
	}

	count = ( (uint32_t) &_kernel_end) - ( (uint32_t) &_kernel_start );
	count = count / 4096 + ( (count % 4096 == 0) ? 0 : 1 );
	page = ADDR2PAGE( (uint32_t) &_kernel_start);
	while(count-- > 0)
	{
		PHYS_SET(page);
		page++;
	}
}
