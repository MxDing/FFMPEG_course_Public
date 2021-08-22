#ifndef _VIDEOAUDIO_H
#define _VIDEOAUDIO_H

#include "ff_sdl.h"
#include "base.h"


//STRUCTURE
struct _VideoParams_t
{
	//ffmpeg str
	AVFormatContext* pFormatCtx;
	int videoindex;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec ;
	AVFrame* pFrame , * pFrameYUV ;
	AVPacket* packet ;

	struct SwsContext* img_convert_ctx ;
	unsigned char* out_buffer;
	int  got_picture ;

	//SDL
	int screen_w , screen_h ;
	SDL_Window* screen;
	SDL_Renderer* sdlRenderer ;
	SDL_Texture* sdlTexture ;
	SDL_Rect sdlRect;
	SDL_Thread* video_tid;
	SDL_Event event;

	char filepath[4096] ;//资源路径

	int cur_channel;

};
typedef struct _VideoParams_t VideoParams;

struct _AudioParams_t
{
	//ffmpeg str
	AVFormatContext* pFormatCtx;
	int audioindex;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVFrame* pFrame;
	AVPacket* packet;

	unsigned char* out_buffer;
	int  got_mp3;
	//音频通道格式类型
	uint64_t out_channel_layout;
	int out_nb_samples;
	enum AVSampleFormat out_sample_fmt;
	int out_sample_rate;
	int out_channels;
	int out_buffer_size;
	int64_t in_channel_layout;

	//SDL
	SDL_AudioSpec wanted_spec;
	SDL_Thread* audio_tid;
	SDL_Event event;
	SwrContext* au_convert_ctx;

	char filepath[4096];//资源路径
	int cur_channel;

};
typedef struct _AudioParams_t AudioParams;


//FUNCTION

VideoParams* VideoParams_create(void);
AudioParams* AudioParams_create(void);

#endif // !_VIDEOAUDIO_H


