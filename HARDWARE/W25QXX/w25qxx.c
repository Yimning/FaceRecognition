#include "w25qxx.h" 
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	

//ALIENTEK STM32F407开发板
//W25QXX 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com

 
u16 W25QXX_TYPE=W25Q128;	//默认是W25Q128
