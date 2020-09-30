#ifndef __ATK_FREC_H
#define __ATK_FREC_H
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//������ʶ�������ALIENTEK�ṩ,�����ṩ1��LIB(��:ATKFREC.lib),�����ʹ�� 

//����:��������ͷ,ʵ������ʶ��.
//˵��:��ʶ���,��Ҫ�õ��ڴ����,�ڴ���ռ������560KB����(20������).ÿ����һ������,�ڴ�ռ����10KB����.
//����:���ڱ�ʶ�����M3/M4ΪĿ�괦����,�ڴ�����,�㷨�Ͻ����˴����˸�,����,�ܶ๦�ܲ�̫����,Ч��Ҳ����
//     �ܺ�.��û����ʶ����Ч���(������������,Ҳ���н�����).����,�δ���,������Ҳο���.
//
//��������:
//1,����ͷģ��һ��.
//2,SD��һ��
//
//ʹ�÷���:					   
//��һ��:����atk_frec_initialization����,��ʼ������ʶ���
//�ڶ���:����atk_frec_add_a_face����,�������ģ��(����Ѿ�����,���Ժ��Դβ�)
//������:����atk_frec_load_data_model����,��������ģ�嵽�ڴ�����(���������ģ�����Ҫ,��û�������ģ��,��ɺ��Դ˲�)
//���Ĳ�:����atk_frec_recognition_face����,��ȡʶ����.
//���岽:����atk_frec_delete_data����,����ɾ��һ������ģ��
//������:�����������ʶ���,�����atk_frec_destroy����,�ͷ������ڴ�,��������ʶ��.
//
//�汾:V1.0
//����ԭ��@ALIENTEK
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved													    								  
//��Ȩ���У�����ؾ���
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����������,�û������Լ��޸�.
#define MAX_LEBEL_NUM 				20								//����������֧�ֶ�������ʶ��,ÿ����1,�ڴ�ռ������10KB
#define ATK_FREC_DATA_PDIR 			"0:ATK_FREC"					//����ģ�ʹ洢�ļ���
#define ATK_FREC_DATA_PNAME 		"0:ATK_FREC/MODEL_%03d.fea"		//����ģ�ʹ洢�ļ���+�ļ���


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���º궨��,�û������޸�
//�������
#define ATK_FREC_OK					0								//ִ������
#define ATK_FREC_MEMORY_ERR			1								//�ڴ����
#define ATK_FREC_FACE_FULL_ERR		2  								//����ģ����������
#define ATK_FREC_READ_WRITE_ERR		3 								//���ݴ洢/��ȡʧ��
#define ATK_FREC_MODEL_DATA_ERR		4								//û�п��õ�ģ��,��Ҫ���������ģ�� 
#define ATK_FREC_UNREC_FACE_ERR		5								//�޷�ʶ�������
//��������ʶ������ʶ���ͼ��ߴ�(��ֵ�벻Ҫ�޸�!!)
#define ATK_GABOR_IMG_WID			30								//����ʶ���ͼ����
#define ATK_GABOR_IMG_HEI			40								//����ʶ���ͼ��߶�

//����ʶ��ģ�ͽṹ��
typedef struct RecogModelStruct
{
	unsigned char count;
	unsigned char *validSample;
	float *feature;
	float **models;
}RecogModel;
extern RecogModel model;					//ʶ��ģ��
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����5������,�����.lib����,�û�ֻ��ֱ�ӵ��ü���.

//�ú������ڳ�ʼ������ʶ��,����0��ʾ����,�����������.
unsigned char atk_frec_initialization(void);										//��ʼ������ʶ��

//�ú������ڽ�������ʶ��,�ͷ������ڴ�.
void atk_frec_destroy(void);														//��������ʶ��,�ͷ������ڴ�

//�ú������һ������ģ��
//data:ͼ��ԭʼ����,RGB565��ʽ,�ߴ������:ATK_GABOR_IMG_WID*ATK_GABOR_IMG_HEI
//     ������ɫֵ���˳��Ϊ:������(ATK_GABOR_IMG_WID),���ϵ���(ATK_GABOR_IMG_HEI)
//index:�洢��ͷ������,0~MAX_LEBEL_NUM-1,����;0XFF,�洢ʧ��
//����ֵ:0,����
//    ����,������� 
unsigned char atk_frec_add_a_face(unsigned short *data,unsigned char *index);		//���һ������

//�ú�������ʶ��һ������
//data:ͼ��ԭʼ����,RGB565��ʽ,�ߴ������:ATK_GABOR_IMG_WID*ATK_GABOR_IMG_HEI
//     ������ɫֵ���˳��Ϊ:������(ATK_GABOR_IMG_WID),���ϵ���(ATK_GABOR_IMG_HEI)
//findex:ʶ����������������(��Χ:0~MAX_LEBEL_NUM-1,0XFF,��ʾʧ��)
//����ֵ:0,����
//    ����,�������
unsigned char atk_frec_recognition_face(unsigned short *data,unsigned char *findex);//ʶ��һ������

//�ú������ڼ�����������ģ��
//��������������Ժ�,ͨ���ú������Լ��ص��ڴ�
void atk_frec_load_data_model(void);												//��������ʶ������ģ��

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����6������,��Ҫ�û��Լ�ʵ��
void atk_frec_memset(char *p,char c,unsigned long len);	//�ڴ�����
void *atk_frec_malloc(unsigned int size);				//�ڴ�����
void atk_frec_free(void *ptr);							//�ڴ�����
unsigned char atk_frec_save_data(unsigned char index,unsigned char* buf,unsigned int size);	//��������
unsigned char atk_frec_read_data(unsigned char index,unsigned char* buf,unsigned int size);	//��ȡ����
unsigned char atk_frec_delete_data(unsigned char index);									//ɾ��һ����������

#endif


























