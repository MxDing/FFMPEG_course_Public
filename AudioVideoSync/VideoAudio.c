#include "base.h"
#include "player.h"


//创建Video句柄及初始化相关信息
VideoParams* VideoParams_create(void)
{
	VideoParams* vobj = malloc(sizeof(VideoParams));
	if (vobj == NULL)
	{
		av_log(NULL, AV_LOG_FATAL, "VideoParams malloc fail\n");
		return NULL;
	}
	memset(vobj, 0, sizeof(VideoParams));
	
	vobj->pFormatCtx = NULL;
	vobj->pFormatCtx = avformat_alloc_context();
	vobj->videoindex = -1;
	vobj->pCodecCtx = NULL;
	vobj->pCodec = NULL;
	vobj->pFrame = av_frame_alloc();
	vobj->pFrameYUV = av_frame_alloc();

	vobj->packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	vobj->img_convert_ctx = NULL;
	vobj->out_buffer = NULL;
	vobj->got_picture = -1;
	//vobj->cur_channel

	if (vobj->packet == NULL || vobj->pFrame == NULL || vobj->pFrameYUV == NULL ||
		vobj->pFormatCtx == NULL)
	{
		av_log(NULL, AV_LOG_FATAL, "VideoParam create some is NULL!!\n");
		goto FAIL;
	}

	//SDL
	vobj->screen_w = 0;
	vobj->screen_h = 0;
	vobj->screen = NULL;
	vobj->sdlRenderer = NULL;
	vobj->sdlTexture = NULL;
	//vobj->sdlRect = NULL;
	vobj->video_tid = NULL;
	//vobj->event

	av_log(NULL, AV_LOG_INFO, "VideoParam create OK!!\n");
	return vobj;

FAIL:
	if (vobj->pFormatCtx != NULL)
	{
		avformat_free_context(vobj->pFormatCtx);
	}
	if (vobj->pFrame != NULL)
	{
		av_frame_free(&vobj->pFrame);
	}
	if (vobj->pFrameYUV != NULL)
	{
		av_frame_free(&vobj->pFrameYUV);
	}
	if(vobj->packet != NULL)
	{
		av_free(vobj->packet);
	}
	if (vobj != NULL)
	{
		free(vobj);
	}

	return NULL;
}
//创建Audio句柄及初始化相关信息
AudioParams* AudioParams_create(void)
{
	AudioParams* aobj = malloc(sizeof(AudioParams));
	if (aobj == NULL)
	{
		av_log(NULL, AV_LOG_FATAL, "AudioParams malloc fail\n");
		return NULL;
	}
	memset(aobj, 0, sizeof(AudioParams));
	
	aobj->pFormatCtx = NULL;
	aobj->pFormatCtx = avformat_alloc_context();
	aobj->audioindex = -1;
	aobj->pCodecCtx = NULL;
	aobj->pCodec = NULL;
	aobj->pFrame = av_frame_alloc();

	aobj->packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	aobj->out_buffer = NULL;
	aobj->got_mp3 = -1;
	//vobj->cur_channel

	if (aobj->packet == NULL || aobj->pFrame == NULL || aobj->pFormatCtx == NULL)
	{
		av_log(NULL, AV_LOG_FATAL, "AudioParam create some is NULL!!\n");
		goto FAIL;
	}

	//SDL
	//aobj->wanted_spec
	aobj->audio_tid = NULL;


	av_log(NULL, AV_LOG_INFO, "AudioParam create OK!!\n");
	return aobj;

FAIL:
	if (aobj->pFormatCtx != NULL)
	{
		avformat_free_context(aobj->pFormatCtx);
	}
	if (aobj->pFrame != NULL)
	{
		av_frame_free(&aobj->pFrame);
	}
	
	if(aobj->packet != NULL)
	{
		av_free(aobj->packet);
	}
	if (aobj != NULL)
	{
		free(aobj);
	}

	return NULL;
}
