#include "dataqueue.h"

//返回0成功，-1失败


struct dataqueue {
	int size;//数据队列的整个大小
	int re_size;//目前数据队列还有多大空间可读
	int wr_size;//目前数据队列还有多大空间可写
	
	char* buf;//最原始的buf
	char* buf_read;//下一个可读的区域
	char* buf_write;//下一个可写的区域
	char* buf_end;//buf的结束地方
	
	bool rw_flag;//正在读（0）还是写（1）？

};

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
	 data_queue->buf_write = buf;
	 data_queue->buf_read = NULL;
	 data_queue->re_size = 0;
	 data_queue->wr_size = bufsize;
	 data_queue->rw_flag = 0;

	 return data_queue;
}

 int dataqueue_get_buf_to_read(dataqueue_t* data_queue, char *read_buf, int *size)
 {
	 if (data_queue == NULL || *size == 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_read is NULL or size is 0\n");
		 return -1;
	 }
	 if (data_queue->re_size >= *size)
	 {
		
	 }
	
 }





