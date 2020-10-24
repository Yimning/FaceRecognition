#include "sdio_sdcard.h"
#include "string.h"	 
#include "sys.h"	 
#include "usart.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//SDIO 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved		
//********************************************************************************
//修改说明
//V1.1	20140522
//1,加入超时判断,解决轮询接收死机的问题.
//V1.2 	20140715
//1,新增SD_GetState和SD_SendStatus函数.
//V1.3  20141108
//1,SD_Init增加卡类型判断，对V1.1和V1.2的SD卡降低SDIO速度，以防出错
////////////////////////////////////////////////////////////////////////////////// 	 


static u8 CardType=SDIO_STD_CAPACITY_SD_CARD_V1_1;		//SD卡类型（默认为1.x卡）
static u32 CSD_Tab[4],CID_Tab[4],RCA=0;					//SD卡CSD,CID以及相对地址(RCA)数据
static u8 DeviceMode=SD_DMA_MODE;		   				//工作模式,注意,工作模式必须通过SD_SetDeviceMode,后才算数.这里只是定义一个默认的模式(SD_DMA_MODE)
static u8 StopCondition=0; 								//是否发送停止传输标志位,DMA多块读写的时候用到  
volatile SD_Error TransferError=SD_OK;					//数据传输错误标志,DMA读写时使用	    
volatile u8 TransferEnd=0;								//传输结束标志,DMA读写时使用
SD_CardInfo SDCardInfo;									//SD卡信息

//SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
//需要用到该数组,确保数据缓存区地址是4字节对齐的.
__align(4) u8 SDIO_DATA_BUFFER[512];						  
 

//初始化SD卡
//返回值:错误代码;(0,无错误)
SD_Error SD_Init(void)
{
	SD_Error errorstatus=SD_OK;	  
	u8 clkdiv=0;
	//SDIO IO口初始化   	 
	RCC->AHB1ENR|=1<<2;    		//使能PORTC时钟
	RCC->AHB1ENR|=1<<3;    		//使能PORTD时钟 
	RCC->AHB1ENR|=1<<22;		//DMA2时钟使能
	RCC->APB2ENR|=1<<11;		//SDIO时钟使能
	RCC->APB2RSTR|=1<<11;		//SDIO复位
	GPIO_Set(GPIOC,0X1F<<8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	//PC8,9,10,11,12复用功能输出	
 	GPIO_Set(GPIOD,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	//PD2复用功能输出	
  	GPIO_AF_Set(GPIOC,8,12);	//PC8,AF12
 	GPIO_AF_Set(GPIOC,9,12);	//PC9,AF12
 	GPIO_AF_Set(GPIOC,10,12);	//PC10,AF12 
 	GPIO_AF_Set(GPIOC,11,12);	//PC11,AF12 
 	GPIO_AF_Set(GPIOC,12,12);	//PC12,AF12 
 	GPIO_AF_Set(GPIOD,2,12);	//PD2,AF12    
	RCC->APB2RSTR&=~(1<<11);	//SDIO结束复位
 	//SDIO外设寄存器设置为默认值 			   
	SDIO->POWER=0x00000000;
	SDIO->CLKCR=0x00000000;
	SDIO->ARG=0x00000000;
	SDIO->CMD=0x00000000;
	SDIO->DTIMER=0x00000000;
	SDIO->DLEN=0x00000000;
	SDIO->DCTRL=0x00000000;
	SDIO->ICR=0x00C007FF;
	SDIO->MASK=0x00000000;	  
 	MY_NVIC_Init(0,0,SDIO_IRQn,2);		//SDIO中断配置
   	errorstatus=SD_PowerON();			//SD卡上电
 	if(errorstatus==SD_OK)errorstatus=SD_InitializeCards();			//初始化SD卡														  
  	if(errorstatus==SD_OK)errorstatus=SD_GetCardInfo(&SDCardInfo);	//获取卡信息
 	if(errorstatus==SD_OK)errorstatus=SD_SelectDeselect((u32)(SDCardInfo.RCA<<16));//选中SD卡   
   	if(errorstatus==SD_OK)errorstatus=SD_EnableWideBusOperation(1);	//4位宽度,如果是MMC卡,则不能用4位模式 
  	if((errorstatus==SD_OK)||(SDIO_MULTIMEDIA_CARD==CardType))
	{  		    
		if(SDCardInfo.CardType==SDIO_STD_CAPACITY_SD_CARD_V1_1||SDCardInfo.CardType==SDIO_STD_CAPACITY_SD_CARD_V2_0)
		{
			clkdiv=SDIO_TRANSFER_CLK_DIV+2;	//V1.1/V2.0卡，设置最高48/4=12Mhz
		}else clkdiv=SDIO_TRANSFER_CLK_DIV;	//SDHC等其他卡，设置最高48/2=24Mhz
		SDIO_Clock_Set(clkdiv);	//设置时钟频率,SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK固定为48Mhz 
		//errorstatus=SD_SetDeviceMode(SD_DMA_MODE);	//设置为DMA模式
		errorstatus=SD_SetDeviceMode(SD_POLLING_MODE);//设置为查询模式
 	}
	return errorstatus;		 
}

