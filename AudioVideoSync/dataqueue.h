#ifndef _DATAQUEUE_H
#define _DATAQUEUE_H

#include "ff_sdl.h"
#include "base.h"

typedef struct dataqueue dataqueue_t;
typedef struct mess_node mess_t;

//取最小值
#define _min(a,b) ((a)>(b))? (b):(a)
//#define MAX_DATA_SIZE (1024*1000)
#define PRE_DATA_SZIE (1024*1000)

typedef enum 
{
	DATA_OK = 0,
	DATA_NOENOUGH = -1,
	DATA_ERROR = -2
}d_err;

//函数
dataqueue_t* dataqueue_create(char* buf, int bufsize);
int dataqueue_get_buf_to_write(dataqueue_t* data_queue, char** write_buf, int* size);
int dataqueue_buf_writecomplete(dataqueue_t* data_queue, int size);
int dataqueue_get_buf_to_read(dataqueue_t* data_queue, char** read_buf, int* size);
int dataqueue_buf_to_readcomplete(dataqueue_t* data_queue, int size);
int dataqueue_free(dataqueue_t* data_queue);



#endif // !_DATAQUEUE_H


