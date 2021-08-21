#pragma once
#ifndef FF_SDL_H
#define FF_SDL_H
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include "libavutil/samplefmt.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavfilter/avfilter.h"
#include "libavutil/log.h"
#include "libavutil/time.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"//用于音频格式转换的s
#include "SDL.h"

#ifdef  __cplusplus
}
#endif //  __cplusplus

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")


#endif



