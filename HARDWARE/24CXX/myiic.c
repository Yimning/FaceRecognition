#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IIC
void IIC_Init(void)
{					     
	RCC->AHB1ENR|=1<<1;    //使能PORTB时钟	   	  
	GPIO_Set(GPIOB,PIN8|PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB8/PB9设置 
	IIC_SCL=1;
	IIC_SDA=1;
}



























