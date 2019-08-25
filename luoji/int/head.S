@初始化文件:初始化中断向量表,设置中断栈,设置中断处理函数

.extern     main
.text 
.global _start 
_start:

@中断向量表
    b   Reset  @复位(按下复位键或上电),0地址

HandleUndef:
    b   HandleUndef  @0x04地址

HandleSWI:
    b   HandleSWI

HandlePrefetchAbort:
    b   HandlePrefetchAbort

HandleDataAbort:
    b   HandleDataAbort

HandleNotUsed:
    b   HandleNotUsed

    b   HandleIRQ   @快中断

HandleFIQ:
    b   HandleFIQ

Reset:
	ldr sp ,=4096  @设置栈指针(跳过前4k)
	bl  disable_watch_dog  @关闭看门狗

	msr cpsr_c, #0xd2  @设置当前程序状态寄存器,I:1(禁止IRQ),F:1(禁止FIQ),M4:1,M0:1(进入FIQ模式)
	ldr sp,=3072  @设置当前模式栈指针
	
	msr cpsr_c, #0xd3  @进入管理模式
	ldr sp, =4096  @(reset后cpu即处于管理模式)
	
	bl  init_led  @初始化led
	bl  init_irq  @初始化中断
	
	msr cpsr_c, #0x53  @开启IRQ,进入管理模式
	
	ldr lr, =halt_loop  @设置返回地址
	ldr pc, =main

halt_loop:
    b   halt_loop
	
HandleIRQ:
	sub lr, lr, #4  @计算返回地址(被中断出)
	stmdb   sp!,    { r0-r12,lr }  @保存现场(寄存器)
	
	ldr lr, =int_return  @设置调用ISR(中断处理服务)即EINT_Handle后的地址
	ldr pc, =EINT_Handle  @调用中断处理函数

int_return:
    ldmia   sp!,    { r0-r12,pc }^  @中断返回,^表示将spsr(程序状态保存寄存器)的值复制到cpsr
    
	
	
	
	
	
	
	
	
	
	
	
