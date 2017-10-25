



objs := Hsmmc.o led.o sdram_init.o clock.o uart.o  pre_power.o key.o interrupt.o pwm.o
objs += gsensor.o

dev.o : $(objs)
	${LD} -r -o dev.o   $^

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	
%.o : %.S
	$(CC)  $(CPPFLAGS) $(CFLAGS)  -c  -o $@ $< 

 

clean:
	rm -f   *.o 
	

	
	