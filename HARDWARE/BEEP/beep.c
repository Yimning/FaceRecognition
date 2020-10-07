#include "beep.h" 


//初始化PF8为输出口		    
//BEEP IO初始化
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟 

}












