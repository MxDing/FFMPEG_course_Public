#include "base.h"
#include "ff_sdl.h"
#include "player.h"

char g_url[4096];

//先调用player_crreate,再调用player_init初始化

void start_proc(void* S_data);

//ffmpeg init
int ff_init()
{
	avformat_network_init();
	//ffmpeg init
	av_log_set_level(AV_LOG_INFO);//设置ffmpeg日志等级
	av_log(NULL, AV_LOG_INFO, "ff_init ok\n");
	return 0;
}

//sdl_init
int sdl_init()
{
	//
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		av_log(NULL, AV_LOG_ERROR, "Could not initialize SDL:%s \n", SDL_GetError());
		return -1;
	}
	av_log(NULL, AV_LOG_ERROR, "sdl_init ok\n");
	return 0;
}


player_t* player_create()
{
	printf("player_create...\n");
	if (ff_init() < 0)
	{
		av_log(NULL, AV_LOG_FATAL, "ff_init fail\n");
		return -1;
	}//ffmpeg的初始化
	if (sdl_init() < 0)//sdl的初始化
	{
		av_log(NULL, AV_LOG_FATAL, "sdl_init fail\n");
		return -1;
	}
	player_t* pobj = malloc(sizeof(player_t));
	if (pobj == NULL)
	{
		av_log(NULL,AV_LOG_ERROR,"player_create malloc fail\n");
		return NULL;
	}
	return pobj;
}


void player_init(player_t *pobj)
{
	if (pobj == NULL)
	{
		av_log(NULL, AV_LOG_INFO, "pobj is NULL,be careful to init\n");
	}
	else
	{
		pobj->v_src = VideoParams_create();
		pobj->a_src = AudioParams_create();

		av_log(NULL, AV_LOG_INFO, "pobj is init ok\n");
	}
	av_log(NULL, AV_LOG_INFO, "player_init ok\n");
}
//打开音视频线程等
//SDL等
int player_start(player_t *pobj)
{
	//创建运行的总线程
	S_start_handle = SDL_CreateThread(start_proc, "start_proc", (void*)pobj);
	if (S_start_handle == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "S_start_handle fail\n");
		return -1;
	}
	return 0;
	
}
//启动线程
void start_proc(void* S_data)
{
	player_t* pobj = S_data;
	if (pobj == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "pobj is NULL\n");
	}

	//视频打开资源,SDL等的打开放到解码线程里，便于拓展
	memcpy(pobj->v_src->filepath, g_url, sizeof(g_url));
	if (avformat_open_input(pobj->v_src->pFormatCtx, pobj->v_src->filepath, NULL, NULL) != 0)
	{
		av_log(NULL, AV_LOG_INFO, "Couldn't open input stream.\n");
		return -1;
	}
	if (avformat_find_stream_info(pobj->v_src->pFormatCtx, NULL) < 0)
	{
		av_log(NULL, AV_LOG_INFO, "Couldn't find stream info.\n");
		return -1;
	}
	for (int i = 0; i < pobj->v_src->pFormatCtx->nb_streams; i++)
	{
		if (pobj->v_src->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			av_log(NULL, AV_LOG_INFO, "find video stream index succed:%d.\n", i);
			pobj->v_src->videoindex = i;
		}
	}
	if (pobj->v_src->videoindex == -1)
	{
		av_log(NULL, AV_LOG_ERROR, "Video index find fail\n");
		return -1;//需要改
	}
	int video_index = pobj->v_src->videoindex;

	pobj->v_src->pCodecCtx = pobj->v_src->pFormatCtx->streams[video_index]->codec;
	pobj->v_src->pCodec = avcodec_find_decoder(pobj->v_src->pCodecCtx->codec_id);
	if (pobj->v_src->pCodec == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "pobj->v_src->videoindex not found\n");
		return -1;
	}

	if (avcodec_open2(pobj->v_src->pCodecCtx, pobj->v_src->pCodec, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Could not open codec\n");
		return -1;
	}
	
	pobj->v_src->out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,
		pobj->v_src->pCodecCtx->width, pobj->v_src->pCodecCtx->height, 1));

	av_image_fill_arrays(pobj->v_src->pFrameYUV->data, pobj->v_src->pFrameYUV->linesize,
		pobj->v_src->out_buffer, AV_PIX_FMT_YUV420P, pobj->v_src->pCodecCtx->width,
		pobj->v_src->pCodecCtx->height, 1);

	av_log(NULL, AV_LOG_INFO, "----Video FIle Information----\n");
	av_dump_format(pobj->v_src, 0, pobj->v_src->filepath, 0);
	av_log(NULL, AV_LOG_INFO, "------------------------------\n");

	pobj->v_src->img_convert_ctx = sws_getContext(pobj->v_src->pCodecCtx->width, pobj->v_src->pCodecCtx->height,
		pobj->v_src->pCodecCtx->pix_fmt, pobj->v_src->pCodecCtx->width, pobj->v_src->pCodecCtx->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	//音频
	memcpy(pobj->a_src->filepath, g_url, sizeof(g_url));
	if (avformat_open_input(pobj->a_src->pFormatCtx, pobj->a_src->filepath, NULL, NULL) != 0)
	{
		av_log(NULL, AV_LOG_INFO, "Couldn't open input stream.\n");
		return -1;
	}
	if (avformat_find_stream_info(pobj->a_src->pFormatCtx, NULL) < 0)
	{
		av_log(NULL, AV_LOG_INFO, "Couldn't find stream info.\n");
		return -1;
	}

	for (int i = 0; i < pobj->a_src->pFormatCtx->nb_streams; i++)
	{
		if (pobj->a_src->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			av_log(NULL, AV_LOG_INFO, "find audio stream index succed:%d.\n", i);
			pobj->a_src->audioindex = i;
		}
	}
	if (pobj->a_src->audioindex == -1)
	{
		av_log(NULL, AV_LOG_ERROR, "Audio index find fail\n");
		return -1;//需要改
	}
	int audio_index = pobj->a_src->audioindex;

	pobj->a_src->pCodecCtx = pobj->a_src->pFormatCtx->streams[audio_index]->codec;
	pobj->a_src->pCodec = avcodec_find_decoder(pobj->a_src->pCodecCtx->codec_id);
	if (pobj->a_src->pCodec == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "pobj->a_src->aduioindex not found\n");
		return -1;
	}

	if (avcodec_open2(pobj->a_src->pCodecCtx, pobj->a_src->pCodec, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Could not open Audio codec\n");
		return -1;
	}
	pobj->a_src->out_channel_layout = AV_CH_LAYOUT_STEREO;//音频通道格式类型：双声道
	pobj->a_src->out_nb_samples = pobj->a_src->pCodecCtx->frame_size;//音频帧中每个packet的采样数量，打开流时就已确定
	pobj->a_src->out_sample_fmt = AV_SAMPLE_FMT_S16;//音频的量化精度,S16是有符号
	pobj->a_src->out_sample_rate = 44100;//音频采样率
	pobj->a_src->out_channels = av_get_channel_layout_nb_channels(pobj->a_src->out_channel_layout);//根据音频通道格式返回音频通道数
	//Out Buffer size根据上面的音频设置信息返回一块建议大小的内存size
	pobj->a_src->out_buffer_size = av_samples_get_buffer_size(NULL, pobj->a_src->out_channels, pobj->a_src->out_nb_samples,
		pobj->a_src->out_sample_fmt, 1);
	//分配内存
	pobj->a_src->out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	//SDL_AUDIO
	pobj->a_src->wanted_spec.freq = pobj->a_src->out_sample_rate;
	pobj->a_src->wanted_spec.format = AUDIO_S16SYS;//采样位数，量化精度
	pobj->a_src->wanted_spec.channels = pobj->a_src->out_channels;
	pobj->a_src->wanted_spec.callback = fill_audio;//这个是回调函数，后面会被反复调用的
	pobj->a_src->wanted_spec.userdata = pobj->a_src;//传入回调函数的参数
	//打开音频设备,将音频硬件信息给到第二个参数，若第二个参数为空，则将通过会回调函数生成指定的音频信息
	if (SDL_OpenAudio(&(pobj->a_src->wanted_spec), NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Could not open Audio Device\n");
		return -1;
	}
	pobj->a_src->in_channel_layout = av_get_default_channel_layout(pobj->a_src->pCodecCtx->channels);
	//Swr
	pobj->a_src->au_convert_ctx = swr_alloc_set_opts(pobj->a_src->au_convert_ctx,
		pobj->a_src->out_channel_layout, pobj->a_src->out_sample_fmt, pobj->a_src->out_sample_rate,
		pobj->a_src->in_channel_layout, pobj->a_src->pCodecCtx->sample_fmt,
		pobj->a_src->pCodecCtx->sample_rate, 0, NULL);
	//真正初始化au_convert_ctx句柄
	swr_init(pobj->a_src->au_convert_ctx);

	//停止播放:参数为0就播放，否则停止
	SDL_PauseAudio(1);

	//初始化成功，需要打开音频和视频的解码线程
	av_log(NULL, AV_LOG_INFO, "Audio/Video start_init ok\n");

	S_Video_handle = SDL_CreateThread(Video_decode_proc, "Video_decode_proc", (void*)pobj);
	if (S_Video_handle == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "S_Video_handle fail\n");
		return -1;
	}
	S_Audio_handle = SDL_CreateThread(Audio_decode_proc, "Audio_decode_proc", (void*)pobj);
	if (S_Video_handle == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "S_Audio_handle fail\n");
		return -1;
	}



	return 0;
	

}

int setDataSource(char* url)
{
	int url_len = sizeof(url);
	if (url_len > 4096)
	{
		av_log(NULL, AV_LOG_ERROR, "url to long\n");
		return -1;
	}
	memcpy(g_url, url, url_len);
	
	av_log(NULL, AV_LOG_INFO, "setdatasource ok\n");
	return 0;

}







