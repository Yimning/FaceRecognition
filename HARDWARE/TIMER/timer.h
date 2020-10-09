#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//V1.1 20140504
//����TIM14_PWM_Init����,����PWM���
//V1.2 20140504
//����TIM5_CH1_Cap_Init����,�������벶��
//V1.3 20140506
//����TIM9_CH2_PWM_Init����,����PWM DACʵ��
////////////////////////////////////////////////////////////////////////////////// 	

//ͨ���ı�TIM14->CCR1��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
#define LED0_PWM_VAL TIM14->CCR1    
//TIM9 CH2��ΪPWM DAC�����ͨ�� 
#define PWM_DAC_VAL  TIM9->CCR2  

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM9_CH2_PWM_Init(u16 arr,u16 psc);	
#endif























