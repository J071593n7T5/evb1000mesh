#include <stdbool.h>

#include "deca_device_api.h"

#include "hop.h"
#include "auxiliary.h"

extern void
clearHop(FRAME * f)
{
	int i;

	for (i = 0; i < FRAME_HOP_LEN; i++)
	{
		f->hop[i] = '.';
	}
}

extern void
addHop(FRAME * f, uint8 hop)
{
	int i;

	for (i = 0; i < FRAME_HOP_LEN; i++)
	{
		if (f->hop[i] == hop)
		{
			return;
		}
		if (f->hop[i] < 'a' || f->hop[i] > 'z')
		{
			f->hop[i] = hop;
			return;
		}
	}
}

extern bool
isExistingHop(FRAME * f, uint8 hop)
{
	int i;

	for (i = 0; i < FRAME_HOP_LEN; i++)
	{
		if (f->hop[i] == hop)
		{
			return (true);
		}
	}
	// not found
	return (false);
}
