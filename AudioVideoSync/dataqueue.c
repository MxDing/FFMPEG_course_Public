#include "dataqueue.h"



//����0�ɹ���-1ʧ��
//�������ڴ湻������Ϊ��β���µ�return -1��Ϊreturn ������־


struct dataqueue {
	int size;//���ݶ��е�������С
	int re_size;//Ŀǰ���ݶ��л��ж��ռ�ɶ�
	int wr_size;//Ŀǰ���ݶ��л��ж��ռ��д��������˵��
	
	char* buf;//��ԭʼ��buf
	char* buf_read;//��һ���ɶ�������
	char* buf_write;//��һ����д������
	char* buf_end;//buf�Ľ����ط�
	
	bool rw_flag;//���ڶ���0������д��1����

};

//Ϊ������պö����д���������Ҫ����2��n�η���С
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

//�Կ�д�ڴ����д,��Ҫע�����tail_size
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
		 //�޿ռ��д
		 *write_buf = NULL;
		 return DATA_NOENOUGH;
	 }
	  *write_buf = data_queue->buf_write;
	  return DATA_OK;
	
}

//��д���ֽ����ύ
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

//��ȡ�ɶ����ڴ�
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

//�ύ������
 int dataqueue_buf_to_readcomplete(dataqueue_t* data_queue, int size)
 {
	 //size������д����
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

//�ͷ�dataqueue
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


