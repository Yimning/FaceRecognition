#include "dac.h"


//DAC通道1输出初始化
void Dac1_Init(void)
{   	

	RCC->APB1ENR|=1<<29;   	//使能DAC时钟	   
	RCC->AHB1ENR|=1<<0;  	//使能PORTA时钟	  
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,模拟输入,下拉   

}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{

}



















































