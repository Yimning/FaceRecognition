#include "rtc.h"
#include "led.h"
#include "delay.h"
#include "usart.h" 

//ALIENTEK STM32F407开发板
//RTC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com		  
//********************************************************************************
//修改说明
//V1.1 20140726
//新增:RTC_Get_Week函数,用于根据年月日信息,得到星期信息.
////////////////////////////////////////////////////////////////////////////////// 

//等待RSF同步
//返回值:0,成功;1,失败;
u8 RTC_Wait_Synchro(void)
{ 
	u32 retry=0XFFFFF; 
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	RTC->ISR&=~(1<<5);		//清除RSF位 
	while(retry&&((RTC->ISR&(1<<5))==0x00))//等待影子寄存器同步
	{
		retry--;
	}
    if(retry==0)return 1;	//同步失败 
	RTC->WPR=0xFF;			//使能RTC寄存器写保护  
	return 0;
}

//RTC进入初始化模式
//返回值:0,成功;1,失败;
u8 RTC_Init_Mode(void)
{ 
	u32 retry=0X10000; 
	if(RTC->ISR&(1<<6))return 0; 
	RTC->ISR|=1<<7;	//进入RTC初始化模式
	while(retry&&((RTC->ISR&(1<<6))==0x00))//等待进入RTC初始化模式成功
	{
		retry--;
	}
    if(retry==0)return 1;	//同步失败
	else return 0; 			//同步成功 
}
//RTC写入后备区域SRAM
//BKPx:后备区寄存器编号,范围:0~19
//data:要写入的数据,32位长度
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(u32*)temp)=data; 
}
//RTC读取后备区域SRAM
//BKPx:后备区寄存器编号,范围:0~19
//返回值:读取到的数据
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(u32*)temp);		//返回读取到的值
}

//十进制转换为BCD码
//val:要转换的十进制数
//返回值:BCD码
u8 RTC_DEC2BCD(u8 val)
{
	u8 bcdhigh = 0; 
	while(val>=10)
	{
		bcdhigh++;
		val-=10;
	} 
	return ((u8)(bcdhigh<<4)|val);
}
//BCD码转换为十进制数据
//val:要转换的BCD码
//返回值:十进制数据
u8 RTC_BCD2DEC(u8 val)
{
	u8 temp=0;
	temp=(val>>4)*10;
	return (temp+(val&0X0F));
}

//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:AM/PM,0=AM/24H,1=PM.
//返回值:0,成功
//       1,进入初始化模式失败 
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	u32 temp=0;
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//进入RTC初始化模式失败
	temp=(((u32)ampm&0X01)<<22)|((u32)RTC_DEC2BCD(hour)<<16)|((u32)RTC_DEC2BCD(min)<<8)|(RTC_DEC2BCD(sec));
	RTC->TR=temp;
	RTC->ISR&=~(1<<7);			//退出RTC初始化模式 
	return 0; 
}
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:0,成功
//       1,进入初始化模式失败 
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	u32 temp=0;
 	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//进入RTC初始化模式失败
	temp=(((u32)week&0X07)<<13)|((u32)RTC_DEC2BCD(year)<<16)|((u32)RTC_DEC2BCD(month)<<8)|(RTC_DEC2BCD(date)); 
	RTC->DR=temp;
	RTC->ISR&=~(1<<7);			//退出RTC初始化模式 
	return 0; 
}
//获取RTC时间
//*hour,*min,*sec:小时,分钟,秒钟 
//*ampm:AM/PM,0=AM/24H,1=PM.
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//等待同步  	 
	temp=RTC->TR;
	*hour=RTC_BCD2DEC((temp>>16)&0X3F);
	*min=RTC_BCD2DEC((temp>>8)&0X7F);
	*sec=RTC_BCD2DEC(temp&0X7F);
	*ampm=temp>>22; 
}
//获取RTC日期
//*year,*mon,*date:年,月,日
//*week:星期
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//等待同步  	 
	temp=RTC->DR;
	*year=RTC_BCD2DEC((temp>>16)&0XFF);
	*month=RTC_BCD2DEC((temp>>8)&0X1F);
	*date=RTC_BCD2DEC(temp&0X3F);
	*week=(temp>>13)&0X07; 
}


