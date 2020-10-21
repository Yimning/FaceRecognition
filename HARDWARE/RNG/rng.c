#include "rng.h"
#include "delay.h"
//ALIENTEK STM32F407开发板
//RNG(随机数发生器)驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//初始化RNG
//返回值:0,成功;1,失败
u8 RNG_Init(void)
{
	u16 retry=0; 
	RCC->AHB2ENR=1<<6;	//开启RNG时钟,来自PLL48CLK
	RNG->CR|=1<<2;		//使能RNG
	while((RNG->SR&0X01)==0&&retry<10000)	//等待随机数就绪
	{
		retry++;
		delay_us(100);
	}
	if(retry>=10000)return 1;//随机数产生器工作不正常
	return 0;
}











