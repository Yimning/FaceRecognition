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
//切换为SD卡模式
void sw_sdcard_mode(void)
{
	OV2640_PWDN=1;//OV2640 Power Down 
	//GPIOC8/9/11切换为 SDIO接口
  	GPIO_AF_Set(GPIOC,8,12);	//PC8,AF12
 	GPIO_AF_Set(GPIOC,9,12);	//PC9,AF12 
 	GPIO_AF_Set(GPIOC,11,12);	//PC11,AF12  
} 

////////////////////////////////////////////////////////////////////////////////////////// 
//LCD显示区域限制参数
u16 face_offx,face_offy;
u16 face_xsize,face_ysize; 

u8 fontsize=12;		//字体大小

//设置图像到屏幕最中心.
void set_image_center(void)
{
	face_offx=0;
	face_offy=0;
	face_xsize=lcddev.width;
	face_ysize=lcddev.height;
	if(lcddev.id==0X1963||lcddev.id==0X5510)
	{ 
		face_offy=80;
		face_ysize=640;
		fontsize=24;
	}else if(lcddev.id==0X5310)
	{
		face_offx=10;
		face_offy=40;
		face_xsize=300;
		face_ysize=400;
		fontsize=16;
	}else fontsize=12;
	LCD_Set_Window(face_offx,face_offy,face_xsize,face_ysize);	//设置开窗口.  
}
//读取原始图片数据
//dbuf:数据缓存区
//xoff,yoff:要读取的图像区域起始坐标
//xsize:要读取的图像区域宽度
//width:要读取的宽度(宽高比恒为3:4)  
void frec_get_image_data(u16 *dbuf,u16 xoff,u16 yoff,u16 xsize,u16 width)
{
	int w, h; 
	u16 height=width*4/3;
	float scale=(float)xsize/width;
	for(h=0;h<height;h++)
	{
		for(w=0;w<width;w++)
		{
			dbuf[h*width+w]=LCD_ReadPoint(xoff+w*scale,yoff+h*scale); 
 		}
	}
}
//加载一个简单界面
//fsize:字体大小
void frec_load_ui(u8 fsize)
{
	if(fsize==16)
	{ 				    	 
		Show_Str(10,2,310,fsize,"KEY0:开始识别  KEY2:删除所有模板",fsize,1);	 			    	 
		Show_Str(10,4+16,310,fsize,"WK_UP:添加人脸模板",fsize,1);				    	 
	}else if(fsize==24)
	{
		Show_Str(10,10,470,fsize,"KEY0:开始识别  KEY2:删除所有模板",fsize,1);	 			    	 
		Show_Str(10,20+24,470,fsize,"WK_UP:添加人脸模板",fsize,1);				    	 		
	}
}
//显示提示信息
//str:要显示的字符串 
//line:第几行;0,第一行;1,第二行;其他,非法.
//fsize:字体大小
void frec_show_msg(u8* str,u8 line)
{
	if(line>1)return;
	if(lcddev.width==240)
	{ 
		Show_Str(10,lcddev.height-(2-line)*fontsize-(2-line)*5,lcddev.width,fontsize,str,fontsize,0);
	}else
	{
		Show_Str(10,lcddev.height-(2-line)*fontsize-(2-line)*(face_offy-fontsize*2)/3,lcddev.width,fontsize,str,fontsize,1);		
	} 
}	

u16 * pixdatabuf; 			//图像缓存
int main(void)
{        
	u8 res;							  
	u8 key;					//键值		   
	u8 i;						 
 	u8 msgbuf[30];			//消息缓存区 
	u8 person;
	
	
	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
	delay_init(168);			//延时初始化  
	uart_init(84,115200);		//初始化串口波特率为115200 
	LED_Init();					//初始化LED 
	usmart_dev.init(84);		//初始化USMART
	TIM3_Int_Init(100-1,8400-1);//10Khz计数,10ms中断一次
 	LCD_Init();					//LCD初始化  
	FSMC_SRAM_Init();			//初始化外部SRAM.
 	BEEP_Init();				//蜂鸣器初始化
 	KEY_Init();					//按键初始化   
	W25QXX_Init();				//初始化W25Q128 
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMEX);		//初始化内部内存池  
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存  
  	f_mount(fs[0],"0:",1); 		//挂载SD卡  
  	f_mount(fs[1],"1:",1); 		//挂载SPI FLASH
	POINT_COLOR=RED;      
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}  	 
 	Show_Str(30,50,200,16,"Explorer STM32F4开发板",16,0);	 			    	 
	Show_Str(30,70,200,16,"人脸识别实验",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:开始识别",16,0);			    	 
	Show_Str(30,110,200,16,"KEY2:删除所有模板",16,0);					    	 
	Show_Str(30,130,200,16,"WK_UP:添加人脸模板",16,0);				    	 
	Show_Str(30,150,200,16,"2015年10月2日",16,0);
	while(SD_Init())//检查SD卡
	{	     
		Show_Str(30,190,240,16,"SD Card Error!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);			  
	}   	  
	while(OV2640_Init())//初始化OV2640
	{
		Show_Str(30,190,240,16,"OV7670 错误!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}	  
	pixdatabuf=mymalloc(SRAMIN,ATK_GABOR_IMG_WID*ATK_GABOR_IMG_HEI*2);	//申请内存
 	Show_Str(30,190,200,16,"OV2640 正常",16,0);
	delay_ms(2000);
 	OV2640_RGB565_Mode();	//RGB565输出
	OV2640_ImageWin_Set((1600-900)/2,0,900,1200);//设置输出尺寸为:900*1200,3:4比例
 	DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,1,0);//DCMI DMA配置  

























