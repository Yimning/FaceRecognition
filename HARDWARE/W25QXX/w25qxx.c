#include "w25qxx.h" 
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	

//ALIENTEK STM32F407开发板
//W25QXX 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com

 
u16 W25QXX_TYPE=W25Q128;	//默认是W25Q128

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 
													 
//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{ 
	RCC->AHB1ENR|=1<<1;     //使能PORTB时钟 
	RCC->AHB1ENR|=1<<6;     //使能PORTG时钟 
	GPIO_Set(GPIOB,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB14推挽输出
	GPIO_Set(GPIOG,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PG7推挽输出
	GPIOG->ODR|=1<<7;		//PG7输出1,防止NRF干扰SPI FLASH的通信  
	W25QXX_CS=1;			//SPI FLASH不选中
	SPI1_Init();		   			//初始化SPI
	SPI1_SetSpeed(SPI_SPEED_2);		//设置为42M时钟,高速模式 
	W25QXX_TYPE=W25QXX_ReadID();	//读取FLASH ID.
}  

