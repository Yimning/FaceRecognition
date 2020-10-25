
#include "spi.h"

//ALIENTEK STM32F407开发板
//SPI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
	u16 tempreg=0;
	RCC->AHB1ENR|=1<<0;    	//使能PORTA时钟	   
	RCC->APB2ENR|=1<<12;   	//SPI1时钟使能 
	GPIO_Set(GPIOB,7<<3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB3~5复用功能输出	
  	GPIO_AF_Set(GPIOB,3,5);	//PB3,AF5
 	GPIO_AF_Set(GPIOB,4,5);	//PB4,AF5
 	GPIO_AF_Set(GPIOB,5,5);	//PB5,AF5 

	//这里只针对SPI口初始化
	RCC->APB2RSTR|=1<<12;	//复位SPI1
	RCC->APB2RSTR&=~(1<<12);//停止复位SPI1
	tempreg|=0<<10;			//全双工模式	
	tempreg|=1<<9;			//软件nss管理
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI主机  
	tempreg|=0<<11;			//8位数据格式	
	tempreg|=1<<1;			//空闲模式下SCK为1 CPOL=1 
	tempreg|=1<<0;			//数据采样从第2个时间边沿开始,CPHA=1  
 	//对SPI1属于APB2的外设.时钟频率最大为84Mhz频率.
	tempreg|=7<<3;			//Fsck=Fpclk1/256
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI启动 
	SPI1->CR1=tempreg; 		//设置CR1  
	SPI1->I2SCFGR&=~(1<<11);//选择SPI模式
	SPI1_ReadWriteByte(0xff);//启动传输		 
}   




















