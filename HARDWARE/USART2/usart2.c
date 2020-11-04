#include "sys.h"
#include "usart2.h"	  

//ALIENTEK STM32F407开发板
//串口2初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com

//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	


//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV,OVER8设置为0
	mantissa=temp;				 			//得到整数部分
	fraction=(temp-mantissa)*16; 			//得到小数部分,OVER8设置为0	 

}







 


