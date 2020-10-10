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
//DCMI DMA配置
//mem0addr:存储器地址0  将要存储摄像头数据的内存地址(也可以是外设地址)
//mem1addr:存储器地址1  当只使用mem0addr的时候,该值必须为0
//memsize:存储器长度    0~65535
//memblen:存储器位宽    0,8位,1,16位,2,32位
//meminc:存储器增长方式,0,不增长;1,增长
void DCMI_DMA_Init(u32 mem0addr,u32 mem1addr,u16 memsize,u8 memblen,u8 meminc)
{ 
	RCC->AHB1ENR|=1<<22;		//DMA2时钟使能 
 	while(DMA2_Stream1->CR&0X01);//等待DMA2_Stream1可配置 
	DMA2->LIFCR|=0X3D<<6*1;		//清空通道1上所有中断标志
	DMA2_Stream1->FCR=0X0000021;//设置为默认值	
	
	DMA2_Stream1->PAR=(u32)&DCMI->DR;//外设地址为:DCMI->DR
	DMA2_Stream1->M0AR=mem0addr;	//mem0addr作为目标地址0
	DMA2_Stream1->M1AR=mem1addr;	//mem1addr作为目标地址1
	DMA2_Stream1->NDTR=memsize;		//传输长度为memsize
	DMA2_Stream1->CR=0;				//先全部复位CR寄存器值  
	DMA2_Stream1->CR|=0<<6;			//外设到存储器模式 
	DMA2_Stream1->CR|=1<<8;			//循环模式
	DMA2_Stream1->CR|=0<<9;			//外设非增量模式
	DMA2_Stream1->CR|=meminc<<10;	//存储器增量模式
	DMA2_Stream1->CR|=2<<11;		//外设数据长度:32位
	DMA2_Stream1->CR|=memblen<<13;	//存储器位宽,8/16/32bit
	DMA2_Stream1->CR|=2<<16;	//高优先级 
	DMA2_Stream1->CR|=0<<21;	//外设突发单次传输
	DMA2_Stream1->CR|=0<<23;	//存储器突发单次传输
	DMA2_Stream1->CR|=1<<25;	//通道1 DCMI通道  
	if(mem1addr)//双缓冲的时候,才需要开启
	{
		DMA2_Stream1->CR|=1<<18;				//双缓冲模式
		DMA2_Stream1->CR|=1<<4;					//开启传输完成中断
		MY_NVIC_Init(0,0,DMA2_Stream1_IRQn,2);	//抢占1，子优先级3，组2  
	}
}   

void (*dcmi_rx_callback)(void);//DCMI DMA接收回调函数
//DMA2_Stream1中断服务函数(仅双缓冲模式会用到)
void DMA2_Stream1_IRQHandler(void)
{        
	if(DMA2->LISR&(1<<11))	//DMA2_Steam1,传输完成标志
	{ 
		DMA2->LIFCR|=1<<11;	//清除传输完成中断
      	dcmi_rx_callback();	//执行摄像头接收回调函数,读取数据等操作在这里面处理  
	}    											 
}  
