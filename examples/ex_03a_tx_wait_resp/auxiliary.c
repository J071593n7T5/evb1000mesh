#include "deca_device_api.h"
#include "port.h"

#include "auxiliary.h"
#include "write.h"

extern uint32 getRandom(uint8 bits, uint32 nodeId)
{
	uint32 mask = (1 << bits) - 1;
	uint32 v = dwt_readsystimestamphi32();
	v /= nodeId;
	v &= mask;
	return (v);
}

extern void systime(void)
{
	uint32 b;

	b = dwt_readsystimestamphi32();
	write_hn(LCD_SYSTIME_IDX8, 4, (uint8*) &b);
}

extern uint8
read_sw(void)
{
	uint8 rtnval = 0;
	if (is_switch_on(TA_SW1_8) == S1_SWITCH_ON)
	{
		rtnval += 1;
	}
	if (is_switch_on(TA_SW1_7) == S1_SWITCH_ON)
	{
		rtnval += 2;
	}
	if (is_switch_on(TA_SW1_6) == S1_SWITCH_ON)
	{
		rtnval += 4;
	}
	if (is_switch_on(TA_SW1_5) == S1_SWITCH_ON)
	{
		rtnval += 8;
	}
	if (is_switch_on(TA_SW1_4) == S1_SWITCH_ON)
	{
		rtnval += 16;
	}
	if (is_switch_on(TA_SW1_3) == S1_SWITCH_ON)
	{
		rtnval += 32;
	}
	return (rtnval);
}


