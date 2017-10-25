



objs := Hsmmc.o

$(objs) : Hsmmc.c
	
	$(CC)  $(CPPFLAGS) $(CFLAGS)  -c  -o $@ $< 
	
%.o : %.S
	$(CC)  $(CPPFLAGS) $(CFLAGS)  -c  -o $@ $< 

 

clean:
	rm *.o -f
	

	
	