start.bin: start.o led.o sdram_init.o clock.o uart.o main.o pre_power.o
	arm-linux-ld  -Tlink.lds -o main.elf $^
	arm-linux-objcopy -O binary main.elf main.bin
	arm-linux-objdump -D main.elf > main_elf.dis
	gcc mkv210_image.c -o mkx210
	./mkx210 main.bin 210.bin
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-linux-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f

	
	