#include "malloc.h"	   
//V1.1 20140706
//1,修改部分函数名字,防止和其他代码的名字冲突(比如LWIP).
//2,修改内存池为32字节对齐
//V1.2 20140716
//1,修改my_mem_malloc函数,对于大内存分配错误的bug.
////////////////////////////////////////////////////////////////////////////////// 	 


//内存池(32字节对齐)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//内部SRAM内存池
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//内部CCM内存池
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//内部CCM内存池MAP
//内存管理参数	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小


//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	my_mem_init,						//内存初始化
	my_mem_perused,						//内存使用率
	mem1base,mem2base,mem3base,			//内存池
	mem1mapbase,mem2mapbase,mem3mapbase,//内存管理状态表
	0,0,0,  		 					//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//内存管理初始化  
//memx:所属内存块
void my_mem_init(u8 memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//内存池所有数据清零  
	mallco_dev.memrdy[memx]=1;								//内存管理初始化OK  
}  












