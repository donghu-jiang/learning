#include "s3c24xx.h"

void EINT_Handle(void)
{
	unsigned long oft = INTOFFSET;

	switch(INTOFFSET)
	{
		case 0:
			GPFDAT |= (0x7<<4);
            GPFDAT &= ~(1<<4);
            break;
		case 2:
			GPFDAT |= (0x7<<4);
            GPFDAT &= ~(1<<5);
            break;
		case 5:
			GPFDAT |= (0x7<<4);
            GPFDAT &= ~(1<<6);             
            break;

	}
	//清除中断(否则cpu会认为中断又一次发生了)
	if(oft == 5)
		EINTPEND = 0x1<<11;
	SRCPND = 1<<oft;
    INTPND = 1<<oft;
}
