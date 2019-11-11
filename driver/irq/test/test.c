#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	unsigned char key_vals;
	int cnt = 0;
	int fd = open("/dev/irq_leds_drv",O_RDWR);
	if(fd < 0) {
		printf("connot open device\n");
		return -1;
	}

	while (1)
	{
		read(fd, &key_vals, sizeof(key_vals));
		printf("key_vals = 0x%02x\n",key_vals);
	}

	return 0;
}
