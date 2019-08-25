#include "s3c24xx.h"

void timer0_Handle(void)
{
	//timer0中断INTOFFSET偏移10
	if(INTOFFSET == 10)
    {
        GPFDAT = ~(GPFDAT & (0x7 << 4)); //反转led状态
    }
    //清中断
    SRCPND = 1 << INTOFFSET;
    INTPND = INTPND;   
}
