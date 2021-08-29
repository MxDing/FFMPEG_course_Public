#include "dataqueue.h"

//����0�ɹ���-1ʧ��


struct dataqueue {
	int size;//���ݶ��е�������С
	int re_size;//Ŀǰ���ݶ��л��ж��ռ�ɶ�
	int wr_size;//Ŀǰ���ݶ��л��ж��ռ��д
	
	char* buf;//��ԭʼ��buf
	char* buf_read;//��һ���ɶ�������
	char* buf_write;//��һ����д������
	char* buf_end;//buf�Ľ����ط�
	
	bool rw_flag;//���ڶ���0������д��1����

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

//��ȡ�ɶ����ڴ�
 int dataqueue_get_buf_to_read(dataqueue_t* data_queue, char **read_buf, int *size)
 {
	 if (data_queue == NULL || *size == 0 || data_queue->re_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_read is NULL or size or re_size is 0\n");
		 return -1;
	 }

	 *read_buf = data_queue->buf_read;

	 if (data_queue->re_size < *size)
	 {
		 *size = data_queue->re_size;
		 av_log(NULL, AV_LOG_INFO, "No have enough data to read...adjusting..\n");
	 }
	 return 1;
	
 }


 int dataqueue_get_buf_to_readcomplete(dataqueue_t* data_queue, int* size)
 {
	 if (data_queue == NULL || *size == 0 || data_queue->re_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_read is NULL or size or re_size is 0\n");
		 return -1;
	 }

	 data_queue->buf_read = data_queue->buf_read + *size;

	 if (data_queue->re_size >= *size)
		 data_queue->re_size = data_queue->re_size - *size;
	 else
		 data_queue->re_size = 0;

 }



