#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//OLED 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
 


//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 	1 
		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS 	PBout(7)
#define OLED_RST  	PGout(15)	
#define OLED_RS 	PDout(6)
#define OLED_WR 	PAout(4)		  
#define OLED_RD 	PDout(7)
 
//使用4线串行接口时使用 
#define OLED_SCLK 	PCout(6)
#define OLED_SDIN 	PCout(7)
		     
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

	 
