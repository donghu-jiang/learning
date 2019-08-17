led_on_c.bin : crt0.S  gpio.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -g -c -o crt0.o crt0.S
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -g -c -o gpio.o gpio.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-ld -Ttext 0x0000000 -g  crt0.o gpio.o -o gpio_elf
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objcopy -O binary -S gpio_elf gpio.bin
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objdump -D -m arm  gpio_elf > gpio.dis
clean:
	rm -f gpio.dis gpio.bin gpio_elf *.o
