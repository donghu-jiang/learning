#include <linux/types.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/utsname.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/initrd.h>
#include <linux/hdreg.h>
#include <linux/bootmem.h>
#include <linux/tty.h>
#include <linux/gfp.h>
#include <linux/percpu.h>
#include <linux/kmod.h>
#include <linux/kernel_stat.h>
#include <linux/start_kernel.h>
#include <linux/security.h>
#include <linux/workqueue.h>
#include <linux/profile.h>
#include <linux/rcupdate.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>
#include <linux/writeback.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/efi.h>
#include <linux/tick.h>
#include <linux/interrupt.h>
#include <linux/taskstats_kern.h>
#include <linux/delayacct.h>
#include <linux/unistd.h>
#include <linux/rmap.h>
#include <linux/mempolicy.h>
#include <linux/key.h>
#include <linux/unwind.h>
#include <linux/buffer_head.h>
#include <linux/debug_locks.h>
#include <linux/lockdep.h>
#include <linux/pid_namespace.h>
#include <linux/device.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/bugs.h>
#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/cacheflush.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

int major;
static struct class *first_drv_class;
static struct class_device *first_drv_class_dev;
volatile unsigned long *gpfcon;
volatile unsigned long *gpfdata;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdata;

static unsigned char keyval;

static DECLARE_WAIT_QUEUE_HEAD(irq_wait);
static volatile int ev_press = 0;

struct desc_pin {
	unsigned int pin;
	unsigned int kay_val;
};

struct desc_pin desc_pins[4] = {
	{S3C2410_GPF0,0x01},
	{S3C2410_GPF2,0x02},
	{S3C2410_GPG3,0x03},
	{S3C2410_GPG11,0x04},
};

static irqreturn_t irq_handle(int irq,void *dev_id)
{
	struct desc_pin *descpin = (struct desc_pin *)dev_id;
	unsigned int pinval;
	
	pinval = s3c2410_gpio_getpin(descpin->pin);
	if(pinval) { //松开为高电平
		keyval = 0x40 | descpin->kay_val; //确定键值(随意定义，应用程序按定义的值判断即可)

	} else { //按下为低电平
		keyval = descpin->kay_val;

	}

	ev_press = 1;
	wake_up_interruptible(&irq_wait);  //中断处理函数将irq_wait队列上的休眠进程唤醒

	return IRQ_HANDLED;
}

static int drv_open(struct inode *inode,struct file *file)
{
	request_irq(IRQ_EINT0, irq_handle,IRQT_RISING,"S1",&desc_pins[0]);
	request_irq(IRQ_EINT2, irq_handle,IRQT_RISING,"S2",&desc_pins[1]);
	request_irq(IRQ_EINT11,irq_handle,IRQT_RISING,"S3",&desc_pins[2]);
	request_irq(IRQ_EINT19,irq_handle,IRQT_RISING,"S4",&desc_pins[3]);
	
	return 0;
}

static int drv_read(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	if(count != sizeof(keyval)) {
		return -EINVAL;
	}

	//无按键动作则休眠
	wait_event_interruptible(irq_wait,ev_press);  //该函数阻塞在此，调用唤醒函数将其唤醒后判断ev_press为真才会往下执行,应用程序会阻塞在read()处
	
	//有按键动作则返回
	copy_to_user(buf,&keyval,sizeof(keyval));
	ev_press = 0;
	
	return sizeof(keyval);
}

static int drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	unsigned char keyVal[4];
	int regVal;

	if(count != sizeof(regVal)) {
		return -EINVAL;
	}

	copy_from_user(keyVal,buf,count);
	printk("%d %d %d %d\n",keyVal[0],keyVal[1],keyVal[2],keyVal[3]);
	if(!keyVal[0]) {
		//gpf0 is down
		*gpfdata &= ~(1<<4);
	} else if(!keyVal[1]) {
		//gpf2 is down
		*gpfdata &= ~(1<<5);
	} else if(!keyVal[2]) {
		//gpg3 is down
		*gpfdata &= ~(1<<6);
	} else if(!keyVal[3]) {
		//gpg11 is down
		*gpfdata &= ~((1<<4) | (1<<5) | (1<<6));
	}

	//*gpfdata |= (1<<4) | (1<<5) | (1<<6);

	return sizeof(keyVal);
}

static int drv_release(struct inode *inode,struct file *file)
{
	free_irq(IRQ_EINT0,&desc_pins[0]);
	free_irq(IRQ_EINT2,&desc_pins[1]);
	free_irq(IRQ_EINT11,&desc_pins[2]);
	free_irq(IRQ_EINT19,&desc_pins[3]);
	
	return 0;
}

static struct file_operations drv_leds_fops = {
	.owner = THIS_MODULE,
	.open = drv_open,
	.read = drv_read,
	.write = drv_write,
	.release = drv_release,
};

int drv_init(viod)
{
	major = register_chrdev(0,"irq_leds_drv",&drv_leds_fops); //向内核注册drv_leds_fops结构中的函数

	//在/sys/下创建first_class类，在该类下创建first_class_dev设备，此后mdev会自动创建/dev/first_class_dev设备节点
	first_drv_class = class_create(THIS_MODULE,"leds_class");
	first_drv_class_dev = class_device_create(first_drv_class,NULL,MKDEV(major,0),NULL,"irq_leds_drv");

	//将物理地址映射为虚拟地址
	gpfcon = (volatile unsigned long *)ioremap(0x56000050,16);
	gpfdata = gpfcon + 1;
	gpgcon = (volatile unsigned long *)ioremap(0x56000060,16);
	gpgdata = gpgcon + 1;

	return 0;
}

void drv_exit(viod)
{
	unregister_chrdev(major,"irq_leds_drv"); //从内核卸载drv_leds_fops结构中的函数
	//删除设备
	class_device_unregister(first_drv_class_dev);
	class_destroy(first_drv_class);
	iounmap(gpfcon);
	iounmap(gpgcon);
}

module_init(drv_init);
module_exit(drv_exit);

MODULE_LICENSE("GPL");
