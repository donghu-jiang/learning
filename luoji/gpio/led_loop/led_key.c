
#define GPFCON  *((volatile unsigned long *)0x56000050)
#define GPFDAT      (*(volatile unsigned long *)0x56000054)

#define GPF4OUT  (1<<8)
#define GPF5OUT  (1<<10)
#define GPF6OUT  (1<<12)

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main()
{
#if 1
	int i = 4;
	GPFCON = GPF4OUT | GPF5OUT | GPF6OUT; //设置三个寄存器均为输出

	while(1) {
		GPFDAT = ~(1<<i);
		wait(30000);
		if(i == 6)
			i = 3;
		i++;
	}
#endif
#if 0
	unsigned long i = 0;

	GPFCON = GPF4OUT | GPF5OUT | GPF6OUT;

	while(1){
		wait(30000);
		GPFDAT = (~(i<<4));
		if(++i == 8)
			i = 0;
	}
#endif
	return 0;
}
