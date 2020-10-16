#include "lsens.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//LSENS(光敏传感器)驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////

  
//初始化光敏传感器
void Lsens_Init(void)
{
	RCC->AHB1ENR|=1<<5;    	//使能PORTF时钟	  
	GPIO_Set(GPIOF,PIN7,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PF7,模拟输入,下拉   
	Adc3_Init();
}





