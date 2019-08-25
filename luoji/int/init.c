#include "s3c24xx.h"

#define GPF4OUT  (1<<8)
#define GPF5OUT  (1<<10)
#define GPF6OUT  (1<<12)

#define GPF4MASK  (3<<8) //用作清除当前寄存器状态(置为Reserved状态)
#define GPF5MASK  (3<<10)
#define GPF6MASK  (3<<12)

#define GPF0EIN  (0x2)  //GPF0管脚配置为中断模式
#define GPF2EIN  (0x2<<4)  //GPF2管脚配置为中断模式
#define GPG3EIN  (0x2<<6)  //GPG3管脚配置为中断模式

#define GPF0MASK  (0x3<<0)
#define GPF2MASK  (0x3<<4)
#define GPG3MASK  (0x3<<6)

void disable_watch_dog(void)
{
	WTCON = 0x0;
}

void init_led(void)
{
	GPFCON &= ~(GPF4MASK | GPF5MASK | GPF6MASK); //将寄存器对应位清0
	GPFCON |= (GPF4OUT | GPF5OUT | GPF6OUT); //将寄存器对应位设为想要的值
}

void init_irq(void)
{
	GPFCON &= ~(GPF0MASK | GPF2MASK);
	GPFCON |= GPF0EIN | GPF2EIN;
	
	GPGCON &= ~GPG3MASK;
	GPGCON |= GPG3EIN;

	EINTMASK &= ~(1<<11); //使能EINT11,EINT0,EINT2为Reserved状态,故不做处理
	
	PRIORITY = (PRIORITY & ((~0x01) | (0x3<<7))) | (0x0 << 7);
	INTMSK   &= (~(1<<0)) & (~(1<<2)) & (~(1<<5)); //使能EINT0,EINT2,EINT11(第5位为8~23共用使能位)
}
