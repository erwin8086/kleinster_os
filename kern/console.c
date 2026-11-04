#include <types.h>
#define VIDEO_MEM 0xF80B8000
#define SCREEN_H 25
#define SCREEN_W 80
static uint16_t *video = (uint16_t *) VIDEO_MEM;
static uint16_t *video_end = (uint16_t *) (VIDEO_MEM + SCREEN_H*SCREEN_W*2);
static uint32_t x, y;

static uint8_t color;


void console_init()
{
	int i;
	x=0;
	y=0;
	color = 0x0A;
	for(i=0;i<SCREEN_H*SCREEN_W;i++)
		video[i] = 0;
}

void console_scroll()
{
	uint16_t *src = video + SCREEN_W;
	uint16_t *dest = video;
	while(src < video_end)
	{
		*dest++=*src++;
	}
	while(dest < video_end)
	{
		*dest++=0;
	}
}

void put_char(char c)
{
	int i;
	switch(c)
	{
		case '\n':
			y++;
			break;
		case '\r':
			x=0;
			break;
		case '\t':
			for(i=0;i<8;i++)
				put_char(' ');
			break;
		case ' ':
			video[y*SCREEN_W+x] = 0;
			x++;
			break;
		default:
			video[y*SCREEN_W+x] = (color<<8) | c;
			x++;
			break;
	}
	if(x>=SCREEN_W)
	{
		y++;
	}
	if(y>=SCREEN_H)
	{
		console_scroll();
		y--;
	}
}


void put_str(char *str)
{
	while(*str)
		put_char(*str++);
}
