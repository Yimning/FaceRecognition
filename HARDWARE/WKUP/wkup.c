#include "wkup.h"
#include "led.h"
#include "delay.h"	
#include "rtc.h"
#include "usart.h"																	   

//ALIENTEK STM32F407开发板
//待机唤醒 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	  
//////////////////////////////////////////////////////////////////////////////////
	 



//系统进入待机模式
void Sys_Enter_Standby(void)
{		
	u32 tempreg;			//零时存储寄存器值用
	//关闭所有外设(根据实际情况写) 
 	RCC->AHB1RSTR|=0X01FE;	//复位除GPIOA以外的所有IO口	
  	while(WKUP_KD);			//等待WK_UP按键松开(在有RTC中断时,必须等WK_UP松开再进入待机)
 	RCC->AHB1RSTR|=1<<0;	//复位GPIOA
	
	//STM32F4,当开启了RTC相关中断后,必须先关闭RTC中断,再清中断标志位,然后重新设置
	//RTC中断,再进入待机模式才可以正常唤醒,否则会有问题.
	RCC->APB1ENR|=1<<28;	//使能电源时钟 
	PWR->CR|=1<<8;			//后备区域访问使能(RTC+SRAM) 
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	tempreg=RTC->CR&(0X0F<<12);//记录原来的RTC中断设置
	RTC->CR&=~(0XF<<12);	//关闭RTC所有中断
	RTC->ISR&=~(0X3F<<8);	//清除所有RTC中断标志.  
	PWR->CR|=1<<2;      	//清除Wake-up 标志
	RTC->CR|=tempreg;		//重新设置RTC中断 
	RTC->WPR=0xFF;			//使能RTC寄存器写保护  
	Sys_Standby();	//进入待机模式






