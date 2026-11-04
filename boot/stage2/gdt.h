struct GDTR {
	uint16_t gdt_limit;
	uint32_t gdt_ptr;
} __attribute__((packed)) ;

typedef uint64_t gdt_t;

#define GDT_BIT_G 0x80000000000000LL
#define GDT_BIT_B 0x40000000000000LL
#define GDT_BIT_P 0x800000000000LL

#define GDT_ENTR(base, limit) \
	(limit & 0xFFFFLL) | \
	( (base & 0xFFFFFFLL) << 16) | \
	( (limit & 0xF0000LL) << 32) | \
	( (base & 0xFF000000LL) << 32 )

#define GDT_CODE(base, limit, priv) \
	GDT_ENTR(base, limit) | \
	GDT_BIT_G | GDT_BIT_P | GDT_BIT_B | (0x1ALL << 40) | ( (priv) << 45 )

#define GDT_DATA(base, limit, priv) \
	GDT_ENTR(base, limit) | \
	GDT_BIT_G | GDT_BIT_P | GDT_BIT_B | (0x12LL << 40) | ( (priv) << 45 )

#define GDT16_CODE(base, limit, priv) \
	GDT_ENTR(base, limit) | \
	GDT_BIT_P | (0x1ALL << 40) | ( (priv) << 45 )

#define GDT16_DATA(base, limit, priv) \
	GDT_ENTR(base, limit) | \
	GDT_BIT_P | (0x12LL << 40) | ( (priv) << 45)
