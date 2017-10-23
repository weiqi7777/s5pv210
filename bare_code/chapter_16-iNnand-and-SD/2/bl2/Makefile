CC		  = arm-linux-gcc
LD 		  = arm-linux-ld
OBJCOPY   = arm-linux-objcopy
OBJDUMP   = arm-linux-objdump
AR		  = arm-linux-ar

INCDIR := $(shell pwd)

# C预处理器的flag，flag就是编译器可选的选项
CPPFLAGS 	:= -nostdlib -nostdinc -I$(INCDIR)/include
# C编译器的flag
CFLAGS		:= -Wall -O2 -fno-builtin

# 导出这些变量到全局，给子文件夹下面的makefile使用
export CC LD OBJCOPY OBJDUMP AR CPPFLAGS CFLAGS

objs := start.o led.o sdram_init.o clock.o uart.o main.o pre_power.o key.o interrupt.o pwm.o
objs += watchdog.o rtc.o
objs += lib/libc.a sd/Hsmmc.o

main.bin: $(objs)
	$(LD)  -Tlink.lds -o main.elf $^
	$(OBJCOPY) -O binary main.elf main.bin
	$(OBJDUMP) -D main.elf > main_elf.dis
	gcc mkv210_image.c -o mkx210


lib/libc.a: 
	make -C lib 

sd/Hsmmc.o:
	make -C sd
%.o : %.S
	$(CC)  $(CPPFLAGS) $(CFLAGS)  -c  -o $@ $< 

%.o : %.c
	$(CC)  $(CPPFLAGS) $(CFLAGS)  -c  -o $@ $<  

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f
	make -C lib clean
	make -C sd clean

	
	