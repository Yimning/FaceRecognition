#include "timer.h"
#include "led.h"
#include "usart.h"

//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
////////////////////////////////////////////////////////////////////////////////// 	




extern int reg_time;
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		//printf("frame:%d\r\n",ov_frame);//打印帧率
		reg_time++; 
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
