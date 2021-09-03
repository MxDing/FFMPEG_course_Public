#include "player.h"
#include "ff_sdl.h"
#include "base.h"

bool if_handle_true(void *handle,const char *m,int line)
{
	if (handle == NULL)
	{
		av_log(NULL,AV_LOG_ERROR,"%s: %d handle is NULL!!\n",m,line);
		return false;
	}
	return true;
}


