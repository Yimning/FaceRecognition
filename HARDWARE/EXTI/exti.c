#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"

//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(WK_UP==1)	 
	{
		BEEP=!BEEP; 
	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}	



