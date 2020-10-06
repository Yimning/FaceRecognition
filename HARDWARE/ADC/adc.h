#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 

#define ADC_CH5  		5 		 	//通道5	   	      	    
#define ADC_CH_TEMP  	16 		 	//通道16,内部温度传感器专用通道	   	    
	   									   
void Adc_Init(void); 				//ADC初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
short Get_Temprate(void);			//读取内部温度传感器值 
void Adc3_Init(void); 				//ADC3初始化
u16  Get_Adc3(u8 ch); 				//获得ADC3某个通道值  
#endif 















