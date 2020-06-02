/*********************************************************
*  @file    cm_tts_demo.c
*  @brief   ML302 OpenCPU ADC APIs header
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/21
********************************************************/
#include "main.h"

#define MAX_SIZE 512
int tts_init = 0;
void cm_test_tts(unsigned char **cmd,int len)
{
	int textlen = strlen(cmd[2]);
	unsigned  coding;
	if(tts_init == 0)
	{
		cm_tts_init();
		tts_init= 1;
	}
	if(char_to_int(cmd[3]) == 0)
	{
		coding = ML_UTF8;
	}
	else
	{
		coding = ML_CP936;
	}
	if(cm_tts_playing())
	{
		cm_printf("[TTS]:busy\n");
		return;
	}
	if(textlen > MAX_SIZE)
	{
		cm_printf("[TTS]:size too long\n");
		return;
	}
	cm_printf("[TTS]:\n",ttsPlayText(cmd[2], textlen, coding));

}
