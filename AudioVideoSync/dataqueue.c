#include "dataqueue.h"



//����0�ɹ���-1ʧ��
//�������ڴ湻������Ϊ��β���µ�return -1��Ϊreturn ������־

//���ݶ��з���������һ���ڴ棬�������ǿ��Խ�����ڴ��ֶ���Ϊ�����֣�һ��������Ϣͷ��һ��������ʵ����
#define MESS_SIZE sizeof((mess_t))

typedef enum _mess_data_type
{
	NORMAL_DATA = 0,
	FF_PACKET = 1,
}data_type_t;

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
struct mess_node
{
	int data_size;//���ݵ�ʵ�ʴ�С
	//int buf_off;//��������ݶ��з��صĵ�ַ��д���ݴ���ƫ����
	data_type_t d_type;
	
	//bool end_to_head;//��ȡ��

	char *data_buf;//ʵ��д���ݵĵط�
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
//��Ҫ����һ�����⣬���write�������һ��С�ռ䣬�����ӻ᲻��һֱ�����ڴ浼���޷�д�롣
int dataqueue_get_buf_to_write(dataqueue_t* data_queue, char** write_buf, int* size)
{
	 if (data_queue == NULL || *size == 0 || data_queue->wr_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_write is NULL or size or re_size is 0\n");
		 *write_buf = NULL;
		 return DATA_ERROR;
	 }
	 //�����ж�Ӧ������
	 int tail_size = 0;
	 if (data_queue->buf_write > data_queue->buf_read)
		 tail_size = data_queue->buf_end - data_queue->buf_write;
	 else
		 tail_size = data_queue->buf_read - data_queue->buf_write;
	 if(*size + MESS_SIZE <= tail_size)
	 {
		 //������Ϣ�ڵ��ĵ�ַ
		//mess_t * m_tmp = (mess_t *)data_queue->buf_write;
		 *write_buf = data_queue->buf_write + MESS_SIZE;
	 }else
	 {
		 *write_buf = NULL;
		 return DATA_NOENOUGH;
	 }
	 //*size = _min(*size + MESS_SIZE, tail_size);
	 //if (*size == 0)
	 //{
	 //    //�޿ռ��д
	 //    *write_buf = NULL;
	 //    return DATA_NOENOUGH;
	 //}
	 // *write_buf = data_queue->buf_write;
	  return DATA_OK;
	
}

//��д���ֽ����ύ
int dataqueue_buf_writecomplete(dataqueue_t* data_queue, int size , data_type_t type_id)
{
	 if (data_queue == NULL || size == 0 )
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_buf_writecomplete is NULL or size is 0\n");
		 return DATA_ERROR;
	 }
	 int total_size = size + MESS_SIZE;

	 mess_t * m_tmp = (mess_t *)data_queue->buf_write;
	 m_tmp -> data_size = size;
	 m_tmp -> d_type = type_id;
	 m_tmp -> data_buf = data_queue->buf_write + MESS_SIZE;

	 data_queue->buf_write = data_queue->buf_write + total_size;

	 if (data_queue->buf_write >= data_queue->buf_end)
		 data_queue->buf_write = data_queue->buf;

	 data_queue->re_size += total_size;
	 data_queue->wr_size = data_queue->size - data_queue->re_size;
	 return DATA_OK;
}

//��ȡ�ɶ����ڴ�
 int dataqueue_get_buf_to_read(dataqueue_t* data_queue, char **read_buf, int *size, data_type_t *type_id)
 {
	 if (data_queue == NULL || *size == 0 || data_queue->re_size <= 0)
	 {
		 av_log(NULL, AV_LOG_ERROR, "dataqueue_get_buf_to_read is NULL or size or re_size is 0\n");
		 return DATA_ERROR;
	 }

	 //��������ж�Ӧ������
	 int tail_size = 0;
	 if (data_queue->buf_write > data_queue->buf_read)
		 tail_size = data_queue->re_size;
	 else
		 tail_size = data_queue->buf_end - data_queue->buf_read;
	
	if(tail_size - MESS_SIZE < *size)
	{
	     *read_buf = NULL;
	     return DATA_NOENOUGH;
	}
	*size = 


	 *read_buf = data_queue->buf_read + MESS_SIZE;


	 //*size = _min(*size, tail_size);
	 //if (*size == 0)
	 //{
	 //    *read_buf = NULL;
	 //    return DATA_NOENOUGH;
	 //}
	 //*read_buf = data_queue->buf_read;

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


