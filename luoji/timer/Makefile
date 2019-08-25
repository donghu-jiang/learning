objs := head.o init.o int.o main.o

timer.bin : $(objs)
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-ld -Ttimer.lds -o timer_linux $^
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-objcopy -O binary -S timer_linux $@

%.o:%.c
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -Wall -O2 -c -o $@ $<

%.o:%.S
	/home/jiang/system/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f *.bin *.o
