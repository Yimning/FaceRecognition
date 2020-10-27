#include "stmflash.h"
#include "delay.h"
#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//STM32内部FLASH读写 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com				  
////////////////////////////////////////////////////////////////////////////////// 

//解锁STM32的FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR=FLASH_KEY1;	//写入解锁序列.
	FLASH->KEYR=FLASH_KEY2; 
}
//flash上锁
void STMFLASH_Lock(void)
{
	FLASH->CR|=(u32)1<<31;//上锁
}
//得到FLASH状态
//返回值:
//0,操作完成
//1,忙 
//2,操作异常 
u8 STMFLASH_GetStatus(void)
{	
	 u32 res=0;		
	res=FLASH->SR;  
	if(res&(1<<16))return 1;   		//忙
	else if(res&(1<<4))return 2;	//操作异常 
	else if(res&(1<<5))return 2;	//操作异常 
	else if(res&(1<<6))return 2;	//操作异常 
	else if(res&(1<<7))return 2;	//操作异常 
	return 0;						//没有任何状态/操作完成.
} 
//等待操作完成
//time:要延时的长短(单位:10us)
//返回值:
//0,完成
//2,操作异常
//0XFF,超时       
u8 STMFLASH_WaitDone(u32 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		delay_us(10);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除扇区
//sectoraddr:扇区地址,范围是:0~11.
//0~3,16K扇区;4,64K扇区;5~11,128K扇区.
//返回值:执行情况
u8 STMFLASH_EraseSector(u32 sectoraddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(200000);//等待上次操作结束,最大2s    
	if(res==0)
	{ 
		FLASH->CR&=~(3<<8);	//清除PSIZE原来的设置
		FLASH->CR|=2<<8;	//设置为32bit宽,确保VCC=2.7~3.6V之间!!
		FLASH->CR&=~(0X1F<<3);//清除原来的设置
		FLASH->CR|=sectoraddr<<3;//设置要擦除的扇区 
		FLASH->CR|=1<<1;	//扇区擦除 
		FLASH->CR|=1<<16;	//开始擦除		  
		res=STMFLASH_WaitDone(200000);//等待操作结束,最大2s  
		if(res!=1)			//非忙
		{
			FLASH->CR&=~(1<<1);//清除扇区擦除标志.
		}
	}
	return res;
}
//在FLASH指定地址写一个字
//faddr:指定地址(此地址必须为4的倍数!!)
//dat:要写入的数据
//返回值:0,写入成功
//    其他,写入失败
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR&=~(3<<8);	//清除PSIZE原来的设置
		FLASH->CR|=2<<8;	//设置为32bit宽,确保VCC=2.7~3.6V之间!!
 		FLASH->CR|=1<<0;	//编程使能
		*(vu32*)faddr=dat;	//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成,一个字编程,最多100us.
		if(res!=1)//操作成功
		{
			FLASH->CR&=~(1<<0);//清除PG位.
		}
	} 
	return res;
} 
//读取指定地址的一个字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}  

























