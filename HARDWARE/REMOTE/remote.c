#include "remote.h"
#include "delay.h"
#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved								  
//////////////////////////////////////////////////////////////////////////////////
 
//红外遥控初始化
//设置IO以及TIM1_CH1的输入捕获
void Remote_Init(void)    			  
{									   
	RCC->APB2ENR|=1<<0;   	//TIM1 时钟使能 
	RCC->AHB1ENR|=1<<0;   	//使能PORTA时钟	
	
	GPIO_Set(GPIOA,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉
	GPIO_AF_Set(GPIOA,8,1);	//PA8,AF1 
  	  
 	TIM1->ARR=10000;  		//设定计数器自动重装值 最大10ms溢出  
	TIM1->PSC=167;  		//预分频器,1M的计数频率,1us加1.	 

	TIM1->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM1->CCMR1|=3<<4;  	//IC1F=0011 配置输入滤波器 8个定时器时钟周期滤波
 	TIM1->CCMR1|=0<<2;  	//IC1PS=00 	配置输入分频,不分频 

	TIM1->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM1->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中
	TIM1->DIER|=1<<1;   	//允许CC1IE捕获中断				
	TIM1->DIER|=1<<0;   	//允许更新中断				
	TIM1->CR1|=0x01;    	//使能定时器1
  	MY_NVIC_Init(1,3,TIM1_CC_IRQn,2);//抢占1，子优先级3，组2									 
  	MY_NVIC_Init(1,2,TIM1_UP_TIM10_IRQn,2);//抢占1，子优先级2，组2									 
}
//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta=0;	  	  
u16 Dval;		//下降沿时计数器的值
u32 RmtRec=0;	//红外接收到的数据	   		    
u8  RmtCnt=0;	//按键按下的次数	 
//定时器1溢出中断
void TIM1_UP_TIM10_IRQHandler(void)
{
	u16 tsr; 
	tsr=TIM1->SR;
	if(tsr&0X01)//处理溢出
	{
		if(RmtSta&0x80)//上次有数据被接收到了
		{	
			RmtSta&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//清空引导标识
				RmtSta&=0XF0;	//清空计数器	
			}						 	   	
		}							    
	}
	TIM1->SR=0;	//清除中断标志位 	 
} 




 





