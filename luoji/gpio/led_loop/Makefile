led_on_c.bin : crt0.S  led_key.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -g -c -o crt0.o crt0.S
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -g -c -o led_key.o led_key.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-ld -Ttext 0x0000000 -g  crt0.o led_key.o -o gpio_elf
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objcopy -O binary -S gpio_elf led_key.bin
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objdump -D -m arm  gpio_elf > led_key.dis
clean:
	rm -f led_key.dis led_key.bin gpio_elf *.o
