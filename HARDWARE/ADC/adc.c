#include "adc.h"
#include "delay.h"		 

//1,修改Adc_Init函数,支持内部温度传感器数据采集.
//2,新增Get_Temprate函数,用于获取内部温度传感器采集到的温度值
//20140714  V1.2
//1,新增Adc3_Init函数,用于初始化ADC3
//2,新增Get_Adc3函数,用于读取ADC3的某个通道
////////////////////////////////////////////////////////////////////////////////// 
 
//初始化ADC
//这里我们仅以规则通道为例
//我们开启了ADC1_CH5和内部温度传感器通道(CH16)																   
void  Adc_Init(void)
{    
	//先初始化IO口
 	RCC->APB2ENR|=1<<8;    	//使能ADC1时钟 
	RCC->AHB1ENR|=1<<0;    	//使能PORTA时钟	  
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);	//PA5,模拟输入,下拉   

	RCC->APB2RSTR|=1<<8;   	//ADCs复位
	RCC->APB2RSTR&=~(1<<8);	//复位结束	 
	ADC->CCR=1<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
 	ADC->CCR|=1<<23;		//使能内部温度传感器
	
	ADC1->CR1=0;   			//CR1设置清零
	ADC1->CR2=0;   			//CR2设置清零
	ADC1->CR1|=0<<24;      	//12位模式
	ADC1->CR1|=0<<8;    	//非扫描模式	
	
	ADC1->CR2&=~(1<<1);    	//单次转换模式
 	ADC1->CR2&=~(1<<11);   	//右对齐	
	ADC1->CR2|=0<<28;    	//软件触发
	
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1|=0<<20;     	//1个转换在规则序列中 也就是只转换规则序列1 			   
	//设置通道5的采样时间
	ADC1->SMPR2&=~(7<<(3*5));		//通道5采样时间清空	  
 	ADC1->SMPR2|=7<<(3*5); 			//通道5  480个周期,提高采样时间可以提高精确度	

	ADC1->SMPR1&=~(7<<(3*(16-10))); //清除通道16原来的设置	 
	ADC1->SMPR1|=7<<(3*(16-10));   	//通道16 480周期,提高采样时间可以提高精确度
	
 	ADC1->CR2|=1<<0;	   	//开启AD转换器	  
}					  

//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<30;       //启动规则转换通道 
	while(!(ADC1->SR&1<<1));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  








