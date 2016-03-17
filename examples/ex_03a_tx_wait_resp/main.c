/*! ----------------------------------------------------------------------------
 *  @file    main.c
 *  @brief   TX then wait for response example code
 *
 *           This is a simple code example that sends a frame and then turns on the DW1000 receiver to receive a response. The response could be
 *           anything as no check is performed on it and it is only displayed in a local buffer but the sent frame is the one expected by the
 *           companion simple example "RX then send a response example code". After the response is received or if the reception timeouts, this code
 *           just go back to the sending of a new frame.
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_sleep.h"
#include "port.h"

#include "auxiliary.h"
#include "hop.h"
#include "led.h"
#include "pending.h"
#include "spam.h"
#include "write.h"

/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config =
{
	2, /* Channel number. */
	DWT_PRF_64M, /* Pulse repetition frequency. */
	DWT_PLEN_1024, /* Preamble length. */
	DWT_PAC32, /* Preamble acquisition chunk size. Used in RX only. */
	9, /* TX preamble code. Used in TX only. */
	9, /* RX preamble code. Used in RX only. */
	1, /* Use non-standard SFD (Boolean) */
	DWT_BR_110K, /* Data rate. */
	DWT_PHRMODE_STD, /* PHY header mode. */
	(1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

#define	TTL_MAX			6

uint8 nodeId = '?';
bool isDestinationNode = false;
uint8 seqNbr = 0;
uint8 rcvCntOk = 0;
uint8 rcvCntErr = 0;
uint8 sndCntOk = 0;

void mainReceiving(void);

static void setNodeId(uint8 eui[8])
{
	// (simplified) NodeId is based on the eui
	// when using a new device, add one entry to this switch!
	switch (eui[0])
	{
	case 0xbc:
		nodeId = 'a';
		break;
	case 0xbf:
		nodeId = 'b';
		break;
	case 0x42:
		nodeId = 'c';
		break;
	case 0x2c:
		nodeId = 'd';
		break;
	case 0x22:
		nodeId = 'e';
		break;
	case 0x23:
		nodeId = 'f';
		break;
	case 0x4c:
		nodeId = 'g';
		break;
	default:
		nodeId = '?';
		break;
	}
	write_c(LCD_NODE_IDX1, nodeId);
}

static void setIsDestinationNode()
{
	if (is_switch_on(TA_SW1_3) == S1_SWITCH_ON)
	{
		isDestinationNode = true;
		write_c(LCD_END_DEST_IDX1, 'x');
	}
}

/**
 * Application entry point.
 */
int main(void)
{
	uint8 eui[8] =
	{ 0xbc, 0x45, 0x67, 0x89, 0xde, 0xad, 0xbe, 0xef };

	/* Start with board specific hardware init. */
	peripherals_init();

	_write_h2(LCD_SWITCH_IDX2, read_sw());	// validate that we can read all switches
	_write_s(LCD_BUILDTIME_IDX8, strlen(__TIME__), (uint8 *) __TIME__);	// validate that downloading SW was correct
	write_s(LCD_BUILDDATE_IDX8, strlen(__DATE__), (uint8 *) __DATE__);	// validate that downloading SW was correct
	deca_sleep(2000);
	led_on(LED_ALL);	// validate that the LEDs are working
	//dwt_seteui(eui);	// only to reprogram if the eui is empty (or ffff etc.)
	dwt_geteui(eui);
	_clear();
	write_hn(LCD_EUI_IDX16, 8, eui);
	deca_sleep(2000);
	led_off(LED_ALL);
	_clear();
	setNodeId(eui);

	setIsDestinationNode();
	pendingInit();

	/* Reset and initialise DW1000.
	 * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
	 * performance. */
	reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
	spi_set_rate_low();
	dwt_initialise(DWT_LOADNONE);
	spi_set_rate_high();
	/* Configure DW1000. See NOTE 2 below. */
	dwt_configure(&config);

	mainReceiving();
	return (0);
}

bool poll_toggleMsg(void)
{
	static bool toggle = false;
	bool new = (is_switch_on(TA_SW1_6) == S1_SWITCH_ON);

	if (new != toggle)
	{
		// high-low, or low-high transition on this dip-switch
		toggle = new;
		return (true);
	}
	return (false);
}

bool poll_toggleSpam(void)
{
	static bool toggle = false;
	bool new = (is_switch_on(TA_SW1_7) == S1_SWITCH_ON);

	if (new != toggle)
	{
		// high-low, or low-high transition on this dip-switch
		toggle = new;
		return (true);
	}
	return (false);
}

void send(FRAME * f)
{
	led_watchdog2();

	/* Write response frame data to DW1000 and prepare transmission. See NOTE 5 below.*/
	dwt_writetxdata(sizeof(FRAME), (uint8 *) f, 0);
	dwt_writetxfctrl(sizeof(FRAME), 0);

	/* Send the response. */
	dwt_starttx(DWT_START_TX_IMMEDIATE);

	/* Poll DW1000 until TX frame sent event set. */
	while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
	{
	}

	/* Clear TX frame sent event. */
	dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

	sndCntOk++;
	_write_a(LCD_OUT_SEQ_IDX1, f->seq);
	write_h2(LCD_SND_CNT_IDX2, sndCntOk);
}

void prepareInitialMsg(uint8 cmd)
{
	FRAME frame;

	systime();

	rcvCntErr = 0;
	rcvCntOk = 0;
	sndCntOk = 0;
	led_set(LED_PC8, false);

	clearHop(&frame);
	addHop(&frame, nodeId);
	seqNbr++;
	frame.cmd = cmd;
	frame.dst = 'z';
	frame.src = nodeId;
	frame.ttl = TTL_MAX;
	frame.seq = seqNbr;

	pendingAdd(&frame);
}

bool prepareRsp(FRAME * f)
{
	switch (f->cmd)
	{
	case CMD_REQ:
		if (isDestinationNode)
		{
			// REQ message arrives at destination: turn around (send a RSP back to src)
			led_watchdog3();

			f->cmd = CMD_RSP;
			f->ttl = TTL_MAX;
			clearHop(f);
		}
		else
		{
			if (isExistingHop(f, nodeId) == true)
			{
				// quit the trail
				return (false);
			}
		}
		break;
	case CMD_SPAM:
		addSpamList(f->seq, f->src);
		f->src = nodeId;
		clearHop(f);
		break;
	case CMD_RSP:
		if (f->src == nodeId)
		{
			led_set(LED_PC8, true);

			// ready
			return (false);
		}
		else
		{
			if (isExistingHop(f, nodeId) == true)
			{
				// quit the trail
				return (false);
			}
		}
		break;
	}

	if (f->ttl > 0)
	{
		f->ttl--;
		addHop(f, nodeId);
		pendingAdd(f);
		return (true);
	}
	else
	{
		// TLL expired: quit the trail
		return (false);
	}
}

void pollInitialMessages(void)
{
	static uint32 counter = 0xffff;

	if (is_switch_on(TA_SW1_8) == S1_SWITCH_ON)
	{
		counter--;
		if (counter == 0)
		{
			counter = 1 << 18;
			led_watchdog1();
			if (is_switch_on(TA_SW1_6) == S1_SWITCH_ON)
			{
				prepareInitialMsg(CMD_REQ);
			}
			if (is_switch_on(TA_SW1_7) == S1_SWITCH_ON)
			{
				prepareInitialMsg(CMD_SPAM);
			}
		}
	}
	else
	{
		if (poll_toggleMsg())
		{
			prepareInitialMsg(CMD_REQ);
		}
		if (poll_toggleSpam())
		{
			prepareInitialMsg(CMD_SPAM);
		}
	}
}

/* Buffer to store received frame. See NOTE 4 below. */
#define FRAME_LEN_MAX 		127

void mainReceiving(void)
{
	uint8 rx_buffer[FRAME_LEN_MAX];
	FRAME frame;

	bool toBreak;

	uint32 status_reg = 0;
	uint16 frame_len = 0;
	uint8 frameLenAnomalyCnt = 0;
	uint32 randomCounter;

	poll_toggleMsg(); // first time, to read actual status
	poll_toggleSpam(); // first time, to read actual status

	while (1)
	{
		toBreak = false;
		randomCounter = getRandom(12, nodeId);

		/* Activate reception immediately. See NOTE 3 below. */
		dwt_rxenable(0);

		/* Poll until a frame is properly received or an error occurs. See NOTE 4 below.
		 * STATUS register is 5 bytes long but, as the events we are looking at are in the lower bytes of the register, we can use this simplest API
		 * function to access it. */
		while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID))
				& (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)))
		{
			randomCounter--;
			if (randomCounter == 0)
			{
				//led_watchdog1();
				randomCounter = getRandom(12, nodeId);
				if (pendingHasFrames())
				{
					toBreak = true;
					break; // out of while
				}
			}
			led_watchdog0(12);
			pollInitialMessages();
		}

		if (toBreak == true)
		{
			dwt_forcetrxoff();
			pendingSend();
		}
		else
		{
			if (status_reg & SYS_STATUS_RXFCG)
			{
				/* A frame has been received, read it into the local buffer. */
				frame_len = dwt_read32bitreg(RX_FINFO_ID)
						& RX_FINFO_RXFL_MASK_1023;
				if (frame_len <= FRAME_LEN_MAX)
				{
					dwt_readrxdata(rx_buffer, frame_len, 0);
				}
				memcpy(&frame, rx_buffer, sizeof(frame));
				if (frame_len != sizeof(frame))
				{
					frameLenAnomalyCnt++;
				}

				/* Clear good RX frame event in the DW1000 status register. */
				dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

				rcvCntOk++;
				//_write_h1(LCD_RCV_LEN_IDX2, frame_len);
				_write_h1(LCD_RCV_ANON_IDX1, frameLenAnomalyCnt);
				_write_h2(LCD_RCV_OK_CNT_IDX2, rcvCntOk);
				_write_h1(LCD_MSG_TTL_IDX1, frame.ttl);
				_write_a(LCD_MSG_SEQ_IDX1, frame.seq);
				write_s(LCD_MSG_IDX14, 14, (uint8 *) &frame);

				prepareRsp(&frame);
			}
			else
			{
				/* Clear RX error events in the DW1000 status register. */
				dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);

				rcvCntErr++;
				write_h2(LCD_RCV_ERR_CNT_IDX2, rcvCntErr);
			}
		}
	}
}
