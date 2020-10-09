#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RTC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140726
//����:RTC_Get_Week����,���ڸ�����������Ϣ,�õ�������Ϣ.
////////////////////////////////////////////////////////////////////////////////// 

	
u8 RTC_Init(void);						//RTC��ʼ��
u8 RTC_Wait_Synchro(void);				//�ȴ�ͬ��
u8 RTC_Init_Mode(void);					//�����ʼ��ģʽ
void RTC_Write_BKR(u32 BKRx,u32 data);	//д������SRAM
u32 RTC_Read_BKR(u32 BKRx);				//��������SRAM
u8 RTC_DEC2BCD(u8 val);					//ʮ����ת��ΪBCD��
u8 RTC_BCD2DEC(u8 val);					//BCD��ת��Ϊʮ��������
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);	//��ȡRTCʱ��
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);//��ȡRTC����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u8 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
u8 RTC_Get_Week(u16 year,u8 month,u8 day);				//���������������,���㵱���������ڼ�
#endif

















