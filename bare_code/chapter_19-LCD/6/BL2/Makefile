BL2.bin: start.o lcd.o main.o
	arm-linux-ld -Tlink.lds -o BL2.elf $^
	arm-linux-objcopy -O binary BL2.elf BL2.bin
	arm-linux-objdump -D BL2.elf > BL2_elf.dis
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-linux-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f

	
	