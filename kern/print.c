#include <types.h>
#include <console.h>
#include <stdarg.h>
static const char *hex = "0123456789ABCDEF";

void put_hex(uint32_t x)
{
	char buffer[9];
	int i;
	for(i=0;i<8;i++)
	{
		buffer[7-i] = hex[x & 0xF];
		x >>= 4;
	}
	buffer[8] = 0;
	put_str(buffer);
}

void put_dec(uint32_t d)
{
	char buffer[11];
	char *c = &buffer[10];
	if(d==0)
	{
		put_char('0');
		return;
	}
	*c-- = 0;
	while(d > 0)
	{
		*c-- = '0' + (d%10);
		d /= 10;
	}
	put_str(c+1);
}

void put_oct(uint32_t o)
{
	char buffer[12];
	char *c = &buffer[11];

	if(o==0)
	{
		put_char('0');
		return;
	}
	*c-- = 0;
	while(o > 0)
	{
		*c-- = '0' + (o&7);
		o >>= 3;
	}
	put_str(c+1);
}

void printf(char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	int state = 0;

	while(*format)
	{
		if(state)
		{
			switch(*format)
			{
				case '%':
					put_char('%');
					break;
				case 'd':
				case 'i':
					put_dec(va_arg(ap, uint32_t));
					break;
				case 'x':
					put_hex(va_arg(ap, uint32_t));
					break;
				case 'o':
					put_oct(va_arg(ap, uint32_t));
					break;
				case 's':
					put_str(va_arg(ap, char*));
					break;
				case 'c':
					put_char(va_arg(ap, uint32_t));
					break;
				default:
					put_char('%');
					put_char(*format);
					break;
			}
			state = 0;
		} else if(*format == '%') {
			state = 1;
		} else if(*format == '\n') {
			put_char('\r');
			put_char('\n');
		} else {
			put_char(*format);
		}
		format++;
	}
	va_end(ap);
}
