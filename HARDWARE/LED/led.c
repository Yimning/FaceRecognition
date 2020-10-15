#include "led.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;//使能PORTF时钟 
	GPIO_Set(GPIOF,PIN9|PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10设置
	LED0=1;//LED0关闭
	LED1=1;//LED1关闭
}














