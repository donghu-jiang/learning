#define GPFCON      (*(volatile unsigned long *)0x56000050)
#define GPFDAT      (*(volatile unsigned long *)0x56000054)

#define GPGCON      (*(volatile unsigned long *)0x56000060)
#define GPGDAT      (*(volatile unsigned long *)0x56000064)

#define GPF4OUT  (1<<8)
#define GPF5OUT  (1<<10)
#define GPF6OUT  (1<<12)

#define GPF4MASK  (3<<8) //用作清除当前寄存器状态
#define GPF5MASK  (3<<10)
#define GPF6MASK  (3<<12)

#define GPF0IN  (0<<1)
#define GPF2IN  (0<<4)
#define GPG3IN  (0<<6)

#define GPF0MASK  (3<<1)
#define GPF2MASK  (3<<4)
#define GPG3MASK  (3<<6)


int main()
{
	unsigned long dwDat;
	GPFCON &= ~(GPF4MASK | GPF5MASK | GPF6MASK); //将寄存器对应位清0
	GPFCON |= (GPF4OUT | GPF5OUT | GPF6OUT); //将寄存器对应位设为想要的值
	
	GPFCON &= ~(GPF0MASK | GPF2MASK); //S2,S3按键输入脚
	GPFCON |= (GPF0IN | GPF2IN);
	
	GPGCON &= ~(GPG3MASK); //gpg寄存器设置,S4按键输入脚
	GPGCON |= GPG3IN;
	
	while(1)
	{
		dwDat = GPFDAT; //获取gpf寄存器管脚状态
		if(dwDat & (1<<0)) //GPF0脚为高电平,即按键未按下
			GPFDAT |= 1<<4; //熄灯
		else
			GPFDAT &= ~(1<<4); //开灯
		if(dwDat & (1<<2))
			GPFDAT |= 1<<5;
		else
			GPFDAT &= ~(1<<5);

		dwDat = GPGDAT;
		if (dwDat & (1<<3))
			GPFDAT |= (1<<6);
        else    
            GPFDAT &= ~(1<<6);      // LED3µãÁÁ
	}
	
	
	return 0;
}
