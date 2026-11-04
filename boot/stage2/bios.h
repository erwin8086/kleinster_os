void put_str(char *);
#define SPT 18
#define HPT 2
#define TRK 80

#define DRV_CHS(drv, cyl, head, sector) \
	((drv) | (head) << 8 | (sector) << 16 | (cyl) << 24)

#define SECTOR(sector) ((sector) % SPT)+1
#define HEAD(sector) ((sector) / SPT) % 2
#define TRACK(sector) ((sector) / (SPT*HPT))

void read_sector(uint32_t sector);
