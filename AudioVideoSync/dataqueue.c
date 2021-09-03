#include "dataqueue.h"



//返回0成功，-1失败
//考虑在内存够但是因为临尾导致的return -1改为return 其他标志


struct dataqueue {
	int size;//数据队列的整个大小
	int re_size;//目前数据队列还有多大空间可读
	int wr_size;//目前数据队列还有多大空间可写（总体来说）
	
	char* buf;//最原始的buf
	char* buf_read;//下一个可读的区域
	char* buf_write;//下一个可写的区域
	char* buf_end;//buf的结束地方
	
	bool rw_flag;//正在读（0）还是写（1）？

};

//为了满足刚好读完和写完的需求，需要申请2的n次方大小
dataqueue_t *dataqueue_create(char* buf, int bufsize)
{
	 av_log(NULL, AV_LOG_INFO, "dataqueue create\n");
	 dataqueue_t* data_queue = malloc(sizeof(dataqueue_t));
	 if (data_queue == NULL)
	 {
		 av_log(NULL, AV_LOG_FATAL, "data_queue malloc fail\n");
		 return NULL;
	 }
	 memset(data_queue, 0, sizeof(dataqueue_t));
	 data_queue->size = bufsize;
	 data_queue->buf = buf;
	 data_queue->buf_end = buf + bufsize;
	 //data_queue->buf_write = NULL;
	 data_queue->buf_write = buf;
	 data_queue->buf_read = NULL;
	 data_queue->re_size = 0;
	 data_queue->wr_size = bufsize;
	 data_queue->rw_flag = 0;

	 return data_queue;
}

//对可写内存进行写,需要注意控制tail_size
int dataqueue_get_buf_to_write(dataqueue_t* data_queue, char** write_buf, int* size)
{
	 if (data_queue == NULL || *size == 0 || data_queue->wr_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_write is NULL or size or re_size is 0\n");
		 *write_buf = NULL;
		 return DATA_ERROR;
	 }
	 int tail_size = 0;
	 if (data_queue->buf_write > data_queue->buf_read)
		 tail_size = data_queue->buf_end - data_queue->buf_write;
	 else
		 tail_size = data_queue->buf_read - data_queue->buf_write;
	 *size = _min(*size, tail_size);
	 if (*size == 0)
	 {
		 //无空间可写
		 *write_buf = NULL;
		 return DATA_NOENOUGH;
	 }
	  *write_buf = data_queue->buf_write;
	  return DATA_OK;
	
}

//对写部分进行提交
int dataqueue_buf_writecomplete(dataqueue_t* data_queue, int size)
{
	 if (data_queue == NULL || size == 0 )
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_buf_writecomplete is NULL or size is 0\n");
		 return DATA_ERROR;
	 }
	 data_queue->buf_write = data_queue->buf_write + size;

	 if (data_queue->buf_write >= data_queue->buf_end)
		 data_queue->buf_write = data_queue->buf;

	 data_queue->re_size += size;
	 data_queue->wr_size = data_queue->size - data_queue->re_size;
	 return DATA_OK;
}

//获取可读的内存
 int dataqueue_get_buf_to_read(dataqueue_t* data_queue, char **read_buf, int *size)
 {
	 if (data_queue == NULL || *size == 0 || data_queue->re_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_read is NULL or size or re_size is 0\n");
		 return DATA_ERROR;
	 }
	 int tail_size = 0;
	 if (data_queue->buf_write > data_queue->buf_read)
		 tail_size = data_queue->re_size;
	 else
		 tail_size = data_queue->buf_end - data_queue->buf_read;
	 *size = _min(*size, tail_size);
	 if (*size == 0)
	 {
		 *read_buf = NULL;
		 return DATA_NOENOUGH;
	 }
	 *read_buf = data_queue->buf_read;

	 return DATA_OK;
	
 }

//提交读部分
 int dataqueue_buf_to_readcomplete(dataqueue_t* data_queue, int size)
 {
	 //size不能乱写！！
	 if (data_queue == NULL || size == 0 || data_queue->re_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_buf_to_readcomplete is NULL or size or re_size is 0\n");
		 return DATA_ERROR;
	 }

	 data_queue->buf_read = data_queue->buf_read + size;

	 if (data_queue->buf_read >= data_queue->buf_end)
		 data_queue->buf_read=data_queue->buf;

	 data_queue->re_size -= size;
	 data_queue->wr_size = data_queue->size - data_queue->re_size;
	 return DATA_OK;
 }

//释放dataqueue
 int dataqueue_free(dataqueue_t* data_queue)
 {
	 if (data_queue == NULL)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_free is NULL\n");
		 return -1;
	 }
	 free(data_queue->buf);
	 free(data_queue);
	 return 0;
 }


