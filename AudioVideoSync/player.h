#ifndef _PLAYER_H
#define _PLAYER_H


#include "ff_sdl.h"
#include "VideoAudio.h"
#include "dataqueue.h"

#define MAX_AUDIO_FRAME_SIZE 192000

extern SDL_Thread* S_start_handle;
extern SDL_Thread* S_Video_handle;
extern SDL_Thread* S_Audio_handle;


struct _player_t
{
	VideoParams* v_src;
	AudioParams* a_src;
	dataqueue_t* video_queue;
	dataqueue_t* audio_queue;

};

typedef struct _player_t player_t;

//FUNCTION
void player_init(player_t* pobj);
player_t* player_create();
int player_start(char* url);

#endif

