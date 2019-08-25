objs := head.o init.o int.o main.o

int.bin : $(objs)
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-ld -Ttext 0x00000000 -o int_elf $^
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objcopy -O binary -S int_elf $@

%.o:%.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -Wall -O2 -c -o $@ $<

%.o:%.S
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f *.bin *.o
