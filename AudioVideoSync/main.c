#define __STDC_CONSTANT_MACROS
#define _CRT_SECURE_NO_WARNINGS
#include "ff_sdl.h"
#include "player.h"

#include <stdio.h>

int main()
{
	player_t* pobj = player_create();
	if (pobj == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "player_create fail\n");
		return -1;
	}
	player_init(pobj);
	
	printf("hello world\n");
	return 1;
}
