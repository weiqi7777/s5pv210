SECTIONS
{
	. = 0x20000002;
	.text :
	{
		start.o
		clock.o
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
