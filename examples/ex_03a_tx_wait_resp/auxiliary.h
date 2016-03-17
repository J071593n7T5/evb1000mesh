#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_

#include "deca_device_api.h"

// REQ: (original) from start to destination (hopping)
// RSP: (response) from destination back to start (hopping)
// SPAM: send an alive-message (no hopping)
#define	CMD_NONE	0x0
#define CMD_REQ		'X'
#define CMD_RSP		'Z'
#define CMD_SPAM	'S'


// indexes on LCD (macro contains the length of the item as well)
// visible positions: 0..15 and 40..55
#define LCD_BUILDDATE_IDX8		0
#define LCD_SND_CNT_IDX2		1
#define LCD_END_DEST_IDX1		3
#define LCD_RCV_OK_CNT_IDX2		4
#define LCD_NODE_IDX1			6
#define LCD_RCV_ERR_CNT_IDX2	7
//#define LCD_RCV_LEN_IDX2		10
#define LCD_RCV_ANON_IDX1		13
#define LCD_OUT_SEQ_IDX1		15
#define LCD_MSG_IDX14			40
#define LCD_MSG_TTL_IDX1		54
#define LCD_MSG_SEQ_IDX1		55
#define LCD_SYSTIME_IDX8		40
#define LCD_EUI_IDX16			40
#define LCD_BUILDTIME_IDX8		40
#define LCD_SWITCH_IDX2			50


#define	FRAME_HOP_LEN	11

typedef struct
{
	uint8 cmd;
	uint8 dst;
	uint8 src;
	uint8 hop[FRAME_HOP_LEN];
	uint8 ttl;
	uint8 seq;
	uint8 crc[2]; // used by decawave
} FRAME;

extern uint32 getRandom(uint8 bits, uint32 nodeId);
extern void systime(void);
extern uint8 read_sw(void);
extern void send(FRAME * f);

#endif
