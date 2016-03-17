#include <stdbool.h>
#include "deca_device_api.h"

#include "auxiliary.h"
#include "pending.h"

#define	PENDING_LEN		30

static FRAME pending[PENDING_LEN];

extern void
pendingInit(void)
{
	int i;
	for (i = 0; i < PENDING_LEN; i++)
	{
		pending[i].cmd = CMD_NONE;
	}
}

extern void
pendingAdd(FRAME * f)
{
	int i;
	for (i = 0; i < PENDING_LEN; i++)
	{
		if (pending[i].cmd == CMD_NONE)
		{
			pending[i] = *f;
			return;
		}
	}
}

extern bool
pendingHasFrames()
{
	int i;
	for (i = 0; i < PENDING_LEN; i++)
	{
		if (pending[i].cmd != CMD_NONE)
		{
			return (true);
		}
	}
	return (false);
}

extern void
pendingSend(void)
{
	int i;
	for (i = 0; i < PENDING_LEN; i++)
	{
		if (pending[i].cmd != CMD_NONE)
		{
			send(&pending[i]);
			pending[i].cmd = CMD_NONE;
			return;
		}
	}
}
