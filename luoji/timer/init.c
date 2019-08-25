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

#define S3C2410_MPLL_200MHZ     ((0x5c<<12)|(0x04<<4)|(0x00))
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02)) //MDIV=0x5c,PDIV=0x01,SDIV=0x02,以满足

void disable_watch_dog(void)
{
	WTCON = 0x0;
}

void init_led(void)
{
	GPFCON &= ~(GPF4MASK | GPF5MASK | GPF6MASK); //将寄存器对应位清0
	GPFCON |= (GPF4OUT | GPF5OUT | GPF6OUT); //将寄存器对应位设为想要的值
}

void copy_steppingstone_to_sdram(void)
{
    unsigned int *pdwSrc  = (unsigned int *)0;
    unsigned int *pdwDest = (unsigned int *)0x30000000;
    
    while (pdwSrc < (unsigned int *)4096)
    {
        *pdwDest = *pdwSrc;
        pdwDest++;
        pdwSrc++;
    }
}

void clock_init(void)
{
    // LOCKTIME = 0x00ffffff;   //lock time,使用默认值即可
    CLKDIVN  = 0x03;            //使FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1


__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n" 
    "orr    r1, r1, #0xc0000000\n" 
    "mcr    p15, 0, r1, c1, c0, 0\n"
    );

    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002)) //选择板子
    {
        MPLLCON = S3C2410_MPLL_200MHZ;
    }
    else
    {
        MPLLCON = S3C2440_MPLL_200MHZ;
    }       
}

void memsetup(void)
{
    volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

    p[0] = 0x22011110;     //BWSCON
    p[1] = 0x00000700;     //BANKCON0
    p[2] = 0x00000700;     //BANKCON1
    p[3] = 0x00000700;     //BANKCON2
    p[4] = 0x00000700;     //BANKCON3  
    p[5] = 0x00000700;     //BANKCON4
    p[6] = 0x00000700;     //BANKCON5
    p[7] = 0x00018005;     //BANKCON6
    p[8] = 0x00018005;     //BANKCON7
    
    /* REFRESH,
     * HCLK=12MHz:  0x008C07A3,
     * HCLK=100MHz: 0x008C04F4
     */ 
    p[9]  = 0x008C04F4;
    p[10] = 0x000000B1;     //BANKSIZE
    p[11] = 0x00000030;     //MRSRB6
    p[12] = 0x00000030;     //MRSRB7
}

//F=PCLK/{prescaler value+1}/{divider value} = 100MHZ/100/16=62500HZ
void timer0_init(void)
{
    TCFG0  = 99;        //预分频器prescalers0=99
    TCFG1  = 0x03;      //分频系数divider=16
    TCNTB0 = 31250;     //定时时间0.5s
    TCON   |= (1<<1);   //获取'手动更新'位状态
    TCON   = 0x09;      //设置模式为时间间隔定时器模式(自动加载),清除'手动更新'位，开始定时器
}

void init_irq(void)
{        
    //使能timer0中断
    INTMSK   &= (~(1<<10));
}










