SECTIONS
{
	. = 0x0;
	
	.text :
	{		
		start.o
		sdram_init.o
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