SECTIONS
{
	. = 0x23e00000;
	.text :
	{		
		start.o
		peripher/dev.o
		*(.text)
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
