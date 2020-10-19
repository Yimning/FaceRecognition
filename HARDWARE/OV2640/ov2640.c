#include "sys.h"
#include "ov2640.h"
#include "ov2640cfg.h"
#include "timer.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"	
#include "exti.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//OV2640 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
 
//初始化OV2640 
//配置完以后,默认输出是1600*1200尺寸的图片!! 
//返回值:0,成功
//    其他,错误代码
u8 OV2640_Init(void)
{ 
	u16 i=0;
	u16 reg;
	//设置IO     	
  	RCC->AHB1ENR|=1<<6;		//使能外设PORTG时钟    
 	GPIO_Set(GPIOG,PIN9|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	//PG9,15推挽输出
 	OV2640_PWDN=0;	//POWER ON
	delay_ms(10);
	OV2640_RST=0;	//复位OV2640
	delay_ms(10);
	OV2640_RST=1;	//结束复位 
  	SCCB_Init();        		//初始化SCCB 的IO口	 
	SCCB_WR_Reg(OV2640_DSP_RA_DLMT, 0x01);	//操作sensor寄存器
 	SCCB_WR_Reg(OV2640_SENSOR_COM7, 0x80);	//软复位OV2640
	delay_ms(50); 
	reg=SCCB_RD_Reg(OV2640_SENSOR_MIDH);	//读取厂家ID 高八位
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_MIDL);	//读取厂家ID 低八位
	if(reg!=OV2640_MID)
	{
		printf("MID:%d\r\n",reg);
		return 1;
	}
	reg=SCCB_RD_Reg(OV2640_SENSOR_PIDH);	//读取厂家ID 高八位
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_PIDL);	//读取厂家ID 低八位
	if(reg!=OV2640_PID)
	{
		printf("HID:%d\r\n",reg);
		//return 2;
	}   
 	//初始化 OV2640,采用SXGA分辨率(1600*1200)  
	for(i=0;i<sizeof(ov2640_uxga_init_reg_tbl)/2;i++)
	{
	   	SCCB_WR_Reg(ov2640_uxga_init_reg_tbl[i][0],ov2640_uxga_init_reg_tbl[i][1]);
 	} 
  	return 0x00; 	//ok
} 
//OV2640切换为JPEG模式
void OV2640_JPEG_Mode(void) 
{
	u16 i=0;
	//设置:YUV422格式
	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]); 
	} 
	//设置:输出JPEG数据
	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);  
	}  
}
//OV2640切换为RGB565模式
void OV2640_RGB565_Mode(void) 
{
	u16 i=0;
	//设置:RGB565输出
	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]); 
	} 
} 
//自动曝光设置参数表,支持5个等级
const static u8 OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
	{
		0xFF,0x01,
		0x24,0x20,
		0x25,0x18,
		0x26,0x60,
	},
	{
		0xFF,0x01,
		0x24,0x34,
		0x25,0x1c,
		0x26,0x00,
	},
	{
		0xFF,0x01,	
		0x24,0x3e,	
		0x25,0x38,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x48,
		0x25,0x40,
		0x26,0x81,
	},
	{
		0xFF,0x01,	
		0x24,0x58,	
		0x25,0x50,	
		0x26,0x92,	
	},
}; 




