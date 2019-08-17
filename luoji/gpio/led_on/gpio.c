

#define GPFCON (*(volatile unsigned long *)0x56000050)
#define GPFDAT (*(volatile unsigned long *)0x56000054)


int main()
{
	GPFCON = 0x010000000; //GPF4:01 output
	GPFDAT = 0x0; //GPF[4]:0 output low vol

	return 0;
}
