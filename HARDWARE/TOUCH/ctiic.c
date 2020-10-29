#include "ctiic.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//电容触摸屏-IIC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com

//控制I2C速度的延时
void CT_Delay(void)
{
	delay_us(2);
} 
//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{					     
	RCC->AHB1ENR|=1<<1;    		//使能PORTB时钟    
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟  
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 	//PB0设置为推挽输出
	GPIO_Set(GPIOF,PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF11设置为推挽输出
}


