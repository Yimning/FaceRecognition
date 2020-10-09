#include "dac.h"


//DAC通道1输出初始化
void Dac1_Init(void)
{   	

	RCC->APB1ENR|=1<<29;   	//使能DAC时钟	   
	RCC->AHB1ENR|=1<<0;  	//使能PORTA时钟	  
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,模拟输入,下拉   

	DAC->CR|=1<<0;	//使能DAC1
	DAC->CR|=1<<1;	//DAC1输出缓存不使能 BOFF1=1
	DAC->CR|=0<<2;	//不使用触发功能 TEN1=0
	DAC->CR|=0<<3;	//DAC TIM6 TRGO,不过要TEN1=1才行
	DAC->CR|=0<<6;	//不使用波形发生
	DAC->CR|=0<<8;	//屏蔽、幅值设置
	DAC->CR|=0<<12;	//DAC1 DMA不使能    
	DAC->DHR12R1=0;

}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
       double temp=vol;
       temp/=1000;
       temp=temp*4096/3.3;
       DAC->DHR12R1=temp;
}



















































