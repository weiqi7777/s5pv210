bootloader1.bin: start.o  sdram_init.o sd_relocate.o led.o
	arm-linux-ld -Tlink.lds -o bootloader1.elf $^
	arm-linux-objcopy -O binary bootloader1.elf bootloader1.bin
	arm-linux-objdump -D bootloader1.elf > bootloader1_elf.dis
	gcc mkv210_image.c -o mkx210
	./mkx210 bootloader1.bin BL1.bin
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-linux-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f

	
	