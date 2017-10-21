SECTIONS
{
	. = 0xd0024010;
	
	.text :
	{		
		start.o
		led.o
		*(.text)
	}
	
	.rodata : 
	{
		*(.rodata)
	}
	
	.align = 4;
	.data  : 
	{
		*(.data)
	}
	.align = 4;
	bss_start = .;
	.bss :
	{
		*(.bss)
	}
	bss_end = .;
}