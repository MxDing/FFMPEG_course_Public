#ifndef _PLAYER_H
#define _PLAYER_H


#include "ff_sdl.h"
#include "VideoAudio.h"

SDL_Thread* S_start_handle;
SDL_Thread* S_Video_handle;
SDL_Thread* S_Audio_handle;


struct _player_t
{
	VideoParams* v_src;
	AudioParams* a_src;

};

typedef struct _player_t player_t;

//FUNCTION
void player_init(player_t* pobj);
player_t* player_create();
int player_start(char* url);

#endif

