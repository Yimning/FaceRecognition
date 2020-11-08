#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usmart.h"   
#include "sram.h" 
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	
#include "string.h"	
#include "math.h"	
#include "dcmi.h"	
#include "ov2640.h"	
#include "beep.h"	
#include "timer.h"

#include "atk_frec.h"
 
//ALIENTEK 探索者STM32F407开发板 实验x
//人脸识别 测试 实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司


int reg_time;
u8 ov2640_mode=0;						//工作模式:0,RGB565模式;1,JPEG模式
 
 
//处理JPEG数据
//当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
void jpeg_data_process(void)
{ 
	if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
	{ 
	}
} 
//切换为OV2640模式
void sw_ov2640_mode(void)
{
	OV2640_PWDN=0;//OV2640 Power Up
	//GPIOC8/9/11切换为 DCMI接口
 	GPIO_AF_Set(GPIOC,8,13);	//PC8,AF13  DCMI_D2
 	GPIO_AF_Set(GPIOC,9,13);	//PC9,AF13  DCMI_D3
 	GPIO_AF_Set(GPIOC,11,13);	//PC11,AF13 DCMI_D4  
} 








