#include "deca_device_api.h"

#include "auxiliary.h"
#include "spam.h"
#include "write.h"

#define SPAM_LIST_LEN	14

static uint8 spamList[SPAM_LIST_LEN];
static uint8 prevSpamSeqNbr = 0xff;

static void
clearSpamList(void)
{
	int i;

	for (i = 0; i < SPAM_LIST_LEN; i++)
	{
		spamList[i] = '.';
	}
}

extern void
addSpamList(uint8 seqNbr, uint8 hop)
{
	int i;

	if (seqNbr != prevSpamSeqNbr)
	{
		prevSpamSeqNbr = seqNbr;
		clearSpamList();
	}

	for (i = 0; i < SPAM_LIST_LEN; i++)
	{
		if (spamList[i] == hop)
		{
			break;
		}
		if (spamList[i] < 'a' || spamList[i] > 'z')
		{
			spamList[i] = hop;
			break;
		}
	}
	write_s(LCD_MSG_IDX14, SPAM_LIST_LEN, spamList);
}
