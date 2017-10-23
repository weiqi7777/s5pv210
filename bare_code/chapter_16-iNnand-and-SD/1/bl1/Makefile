start.bin: start.o  sdram_init.o sd_relocate.o led.o
	arm-linux-ld  -Tlink.lds -o start.elf $^
	arm-linux-objcopy -O binary start.elf start.bin
	arm-linux-objdump -D start.elf > start_elf.dis
	gcc mkv210_image.c -o mkx210
	./mkx210 start.bin bl1.bin
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-linux-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f

	
	