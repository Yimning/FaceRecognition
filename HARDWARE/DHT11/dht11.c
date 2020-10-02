#include "dht11.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//DHT11 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////
    
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT=0; 	//拉低DQ
    delay_ms(20);    	//拉低至少18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}
























