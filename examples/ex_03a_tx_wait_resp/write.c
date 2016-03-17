
#include "deca_device_api.h"
#include "lcd.h"

// initial contents are not important
static char line[80] = ".....     .....     .....     .....     .....     .....   ";

extern void write_s(int offset, int len, uint8 * str);

extern void
_clear(void)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		line[ 0 + i] = ' ';
		line[40 + i] = ' ';
	}
}


extern void
write_d(int offset, int val)
{
	line[offset + 1] = val % 10 + '0';
	val /= 10;
	line[offset] = val % 10 + '0';
	lcd_display_str(line);
}

extern void
_write_a(int offset, int val)
{
	line[offset] = (val % 64) + '0';
}

extern void
write_a(int offset, int val)
{
	_write_a(offset, val);
	lcd_display_str(line);
}

extern void
write_c(int offset, uint8 val)
{
	line[offset] = val;
	lcd_display_str(line);
}

extern void
_write_s(int offset, int len, uint8 * str)
{
	int i;

	for (i = 0; i < len; i++)
	{
		write_c(i + offset, str[i]);
	}
}

extern void
write_s(int offset, int len, uint8 * str)
{
	_write_s(offset, len, str);
	lcd_display_str(line);
}

extern void
_write_h1(int offset, uint8 val)
{
	uint8 v = val % 16;
	if (v < 10)
	{
		line[offset] = v + '0';
	}
	else
	{
		line[offset] = v - 10 + 'A';
	}
}

extern void
_write_h2(int offset, uint8 val)
{
	_write_h1(offset, val >> 4);
	_write_h1(offset + 1, val);
}

extern void
write_h2(int offset, uint8 v)
{
	_write_h2(offset, v);
	lcd_display_str(line);
}

extern void
write_hn(int offset, int len, uint8 v[])
{
	int i;
	for (i = 0; i < len; i++)
	{
		_write_h2(offset + 2 * i, v[i]);
	}
	lcd_display_str(line);
}

