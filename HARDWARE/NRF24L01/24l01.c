#include "24l01.h"
#include "lcd.h"
#include "delay.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//NRF24L01驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
    
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
 
//初始化24L01的IO口
void NRF24L01_Init(void)
{
	RCC->AHB1ENR|=1<<1;     //使能PORTB时钟 
	RCC->AHB1ENR|=1<<6;     //使能PORTG时钟 
	GPIO_Set(GPIOB,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);		//PB14推挽输出
	GPIO_Set(GPIOG,PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PG6,7推挽输出
	GPIO_Set(GPIOG,PIN8,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);	//PG8,上拉输入
	GPIOB->ODR|=1<<14;		//PB14输出1,防止SPI FLASH干扰NRF的通信  
 	SPI1_Init();    		//初始化SPI1  
	//针对NRF的特点修改SPI的设置
 	SPI1->CR1&=~(1<<6); 	//SPI设备失能
	SPI1->CR1&=~(1<<1); 	//空闲模式下SCK为0 CPOL=0
	SPI1->CR1&=~(1<<0); 	//数据采样从第1个时间边沿开始,CPHA=0  
	SPI1->CR1|=1<<6; 		//SPI设备使能 
	NRF24L01_CE=0; 			//使能24L01
	NRF24L01_CSN=1;			//SPI片选取消	 		 	 
}













