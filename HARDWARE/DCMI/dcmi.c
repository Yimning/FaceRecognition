#include "sys.h"
#include "dcmi.h" 
#include "lcd.h" 
#include "led.h" 
#include "ov2640.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//DCMI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved	
//********************************************************************************
//V1.1 20140516
//修改DCMI_DMA_Init函数，支持双缓冲模式
//////////////////////////////////////////////////////////////////////////////////	 


u8 ov_frame=0;  						//帧率
extern void jpeg_data_process(void);	//JPEG数据处理函数
//DCMI中断服务函数
void DCMI_IRQHandler(void)
{  
	if(DCMI->MISR&0X01)//捕获到一帧图像
	{
		jpeg_data_process();//jpeg数据处理
		DCMI->ICR|=1<<0;	//清除帧中断
		LED1=!LED1;
		ov_frame++;  
	}										 
} 







