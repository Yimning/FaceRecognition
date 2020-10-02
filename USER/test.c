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
 
//ALIENTEK ̽����STM32F407������ ʵ��x
//����ʶ�� ���� ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


int reg_time;
u8 ov2640_mode=0;						//����ģʽ:0,RGB565ģʽ;1,JPEGģʽ
 
 
//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{ 
	if(ov2640_mode)//ֻ����JPEG��ʽ��,����Ҫ������.
	{ 
	}
} 
//�л�ΪOV2640ģʽ
void sw_ov2640_mode(void)
{
	OV2640_PWDN=0;//OV2640 Power Up
	//GPIOC8/9/11�л�Ϊ DCMI�ӿ�
 	GPIO_AF_Set(GPIOC,8,13);	//PC8,AF13  DCMI_D2
 	GPIO_AF_Set(GPIOC,9,13);	//PC9,AF13  DCMI_D3
 	GPIO_AF_Set(GPIOC,11,13);	//PC11,AF13 DCMI_D4  
} 
//�л�ΪSD��ģʽ
void sw_sdcard_mode(void)
{
	OV2640_PWDN=1;//OV2640 Power Down 
	//GPIOC8/9/11�л�Ϊ SDIO�ӿ�
  	GPIO_AF_Set(GPIOC,8,12);	//PC8,AF12
 	GPIO_AF_Set(GPIOC,9,12);	//PC9,AF12 
 	GPIO_AF_Set(GPIOC,11,12);	//PC11,AF12  
} 

////////////////////////////////////////////////////////////////////////////////////////// 
//LCD��ʾ�������Ʋ���
u16 face_offx,face_offy;
u16 face_xsize,face_ysize; 

u8 fontsize=12;		//�����С

//����ͼ����Ļ������.
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
	LCD_Set_Window(face_offx,face_offy,face_xsize,face_ysize);	//���ÿ�����.  
}

//��ȡԭʼͼƬ����
//dbuf:���ݻ�����
//xoff,yoff:Ҫ��ȡ��ͼ��������ʼ����
//xsize:Ҫ��ȡ��ͼ��������
//width:Ҫ��ȡ�Ŀ��(��߱Ⱥ�Ϊ3:4)  
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
//����һ���򵥽���
//fsize:�����С
void frec_load_ui(u8 fsize)
{
	if(fsize==16)
	{ 				    	 
		Show_Str(10,2,310,fsize,"KEY0:��ʼʶ��  KEY2:ɾ������ģ��",fsize,1);	 			    	 
		Show_Str(10,4+16,310,fsize,"WK_UP:�������ģ��",fsize,1);				    	 
	}else if(fsize==24)
	{
		Show_Str(10,10,470,fsize,"KEY0:��ʼʶ��  KEY2:ɾ������ģ��",fsize,1);	 			    	 
		Show_Str(10,20+24,470,fsize,"WK_UP:�������ģ��",fsize,1);				    	 		
	}
}
//��ʾ��ʾ��Ϣ
//str:Ҫ��ʾ���ַ��� 
//line:�ڼ���;0,��һ��;1,�ڶ���;����,�Ƿ�.
//fsize:�����С
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

u16 * pixdatabuf; 			//ͼ�񻺴�

int main(void)
{        
	u8 res;							  
	u8 key;					//��ֵ		   
	u8 i;						 
 	u8 msgbuf[30];			//��Ϣ������ 
	u8 person;
	
	
	Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz 
	delay_init(168);			//��ʱ��ʼ��  
	uart_init(84,115200);		//��ʼ�����ڲ�����Ϊ115200 
	LED_Init();					//��ʼ��LED 
	usmart_dev.init(84);		//��ʼ��USMART
	TIM3_Int_Init(100-1,8400-1);//10Khz����,10ms�ж�һ��
 	LCD_Init();					//LCD��ʼ��  
	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM.
 	BEEP_Init();				//��������ʼ��
 	KEY_Init();					//������ʼ��   
	W25QXX_Init();				//��ʼ��W25Q128 
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMEX);		//��ʼ���ڲ��ڴ��  
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  	f_mount(fs[0],"0:",1); 		//����SD��  
  	f_mount(fs[1],"1:",1); 		//����SPI FLASH
	POINT_COLOR=RED;      
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	 
 	Show_Str(30,50,200,16,"Explorer STM32F4������",16,0);	 			    	 
	Show_Str(30,70,200,16,"����ʶ��ʵ��",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:��ʼʶ��",16,0);			    	 
	Show_Str(30,110,200,16,"KEY2:ɾ������ģ��",16,0);					    	 
	Show_Str(30,130,200,16,"WK_UP:�������ģ��",16,0);				    	 
	Show_Str(30,150,200,16,"2015��10��2��",16,0);
	while(SD_Init())//���SD��
	{	     
		Show_Str(30,190,240,16,"SD Card Error!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);			  
	}   	  
	while(OV2640_Init())//��ʼ��OV2640
	{
		Show_Str(30,190,240,16,"OV7670 ����!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}	  
	pixdatabuf=mymalloc(SRAMIN,ATK_GABOR_IMG_WID*ATK_GABOR_IMG_HEI*2);	//�����ڴ�
 	Show_Str(30,190,200,16,"OV2640 ����",16,0);
	delay_ms(2000);
 	OV2640_RGB565_Mode();	//RGB565���
	OV2640_ImageWin_Set((1600-900)/2,0,900,1200);//��������ߴ�Ϊ:900*1200,3:4����
 	DCMI_Init();			//DCMI����
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,1,0);//DCMI DMA����  

	LCD_Clear(BLACK);
	set_image_center();		//���õ���Ļ������
	frec_load_ui(fontsize);	//��ʾGUI 
 	OV2640_OutSize_Set(face_xsize,face_ysize); 
	sw_sdcard_mode();		//SD��ģʽ 
	res=atk_frec_initialization();	//��ʼ������ʶ��
	if(res)
	{
		printf("atk_frec_initialization error:%d\r\n",res);//��ӡ�������
	} 
	sw_ov2640_mode();	//2640ģʽ
	DCMI_Start();		//�������� 
	while(1)
	{	 
		delay_ms(10);
 		key=KEY_Scan(0);//��֧������
		if(key)
		{
			DCMI_Stop();		//ֹͣ���� 
			sw_sdcard_mode();	//SD��ģʽ
			switch(key)
			{
				case KEY2_PRES: //ɾ������ģ��
 					sprintf((char*)msgbuf,"����ɾ��...");
					frec_show_msg(msgbuf,0);	 			    	 
					for(i=0;i<MAX_LEBEL_NUM;i++)
					{
						res=atk_frec_delete_data(i);//ɾ��ģ��
						if(res==FR_OK)printf("delete face:%d ok\r\n",i);
						else printf("delete face:%d failed\r\n",i);
					} 
					atk_frec_load_data_model();	//���¼�������ʶ��ģ��(��ɾ����,���޷����ؽ���.)
 					if(lcddev.width!=240)LCD_Fill(10,lcddev.height-2*fontsize-2*(face_offy-fontsize*2)/3,lcddev.width,lcddev.height,BLACK);	//���֮ǰ����ʾ
 					sprintf((char*)msgbuf,"ɾ�����");
					frec_show_msg(msgbuf,0);	 			    	 
					delay_ms(1000);
 					if(lcddev.width!=240)LCD_Fill(10,lcddev.height-2*fontsize-2*(face_offy-fontsize*2)/3,lcddev.width,lcddev.height,BLACK);		//�����ʾ
					break;
				case KEY0_PRES: //ʶ������
					frec_get_image_data(pixdatabuf,face_offx,face_offy,face_xsize,30);//��ȡͼ������ 
 					sprintf((char*)msgbuf,"����ʶ��...");
					frec_show_msg(msgbuf,0);	 			    	 
					reg_time=0; 
 					res=atk_frec_recognition_face(pixdatabuf,&person);//����ʶ��
					if(res==ATK_FREC_MODEL_DATA_ERR)
					{	 
						sprintf((char*)msgbuf,"û�п���ģ��,��KEY_UP���ģ��!");
					}else if(res==ATK_FREC_UNREC_FACE_ERR)
					{	
						sprintf((char*)msgbuf,"�޷�ʶ�������,������!"); 
					}else 
					{
						sprintf((char*)msgbuf,"ʶ����:%02d��  ��ʱ:%dms",person,reg_time*10);  
					}
 					if(lcddev.width!=240)LCD_Fill(10,lcddev.height-2*fontsize-2*(face_offy-fontsize*2)/3,lcddev.width,lcddev.height,BLACK);			//���֮ǰ����ʾ
					frec_show_msg(msgbuf,0);	 			    	 
					sprintf((char*)msgbuf,"�����ⰴ������!");  
					frec_show_msg(msgbuf,1);	 			    	 
					while(!KEY_Scan(0))	//�ȴ���������
					{
						delay_ms(10); 
					}
 					if(lcddev.width!=240)LCD_Fill(10,lcddev.height-2*fontsize-2*(face_offy-fontsize*2)/3,lcddev.width,lcddev.height,BLACK);
					break;
				case WKUP_PRES://���һ������������ݿ�
 					frec_get_image_data(pixdatabuf,face_offx,face_offy,face_xsize,30);//��ȡͼ������
					sprintf((char*)msgbuf,"�����������ģ��...");
					frec_show_msg(msgbuf,0);	 			    	 
					res=atk_frec_add_a_face(pixdatabuf,&person);	//���һ������
					if(res==0)
					{
						sprintf((char*)msgbuf,"��ӳɹ�,���:%02d   ",person);
						atk_frec_load_data_model();	//���¼�������ʶ��ģ��(����ӵ�����,���ؽ���)
					}else 
					{
						sprintf((char*)msgbuf,"���ʧ��,�������:%02d",res);
					}
					frec_show_msg(msgbuf,1);	 			    	 
					delay_ms(1000);
					if(lcddev.width!=240)LCD_Fill(10,lcddev.height-2*fontsize-2*(face_offy-fontsize*2)/3,lcddev.width,lcddev.height,BLACK);
					break;  
				default :
					break;
			} 
			sw_ov2640_mode();	//2640ģʽ
			DCMI_Start(); 				//�������� 
		}
		delay_ms(10);
		i++;
		if(i==20)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
 		}
	}
}
















