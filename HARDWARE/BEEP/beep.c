#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
								  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF8为输出口		    
//BEEP IO初始化
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟 
	GPIO_Set(GPIOF,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD); //PF8设置,下拉
	BEEP=0;						//关闭蜂鸣器	
}






