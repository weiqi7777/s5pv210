#define GPA0CON (*(volatile unsigned long *)0xe0200000)

#define UART_0_BASE  0xe2900000

#define ULCON0  	(*(volatile unsigned long *)(UART_0_BASE + 0x0 ))
#define UCON0   	(*(volatile unsigned long *)(UART_0_BASE + 0X4 ))
#define UFCON0  	(*(volatile unsigned long *)(UART_0_BASE + 0X8 ))
#define UMCON0  	(*(volatile unsigned long *)(UART_0_BASE + 0XC ))
#define UTRSTAT0	(*(volatile unsigned long *)(UART_0_BASE + 0X10 ))
#define UTXH0   	(*(volatile unsigned char *)(UART_0_BASE + 0X20 ))
#define URXH0   	(*(volatile unsigned char *)(UART_0_BASE + 0X24 ))
#define UBRDIV0     (*(volatile unsigned long *)(UART_0_BASE + 0X28 ))
#define UDIVSLOT0  	(*(volatile unsigned long *)(UART_0_BASE + 0X2C ))




//uart init
void uart_init()
{
	//set GPIO
	GPA0CON &= 0xffffff00;
	GPA0CON |= 0x00000022;
	
//set uart0 normal mode ,no parity, one stop bit, 8-bit data
	ULCON0 = 0;
	ULCON0 = 0X3;

//set uart0 transmit and receive  mode  is  interrupt request and polling mode	
	UCON0 = 0;
	UCON0 = 0x5;

//disbale fifo
	UFCON0 = 0;

//disable modem and AFC
	UMCON0 = 0;

/*
	uart0 clk source pclk : 66M
	DIV_VAL = (66 * 10^6) / (115200 *16) - 1 = 34.8
	UBRDIV0 = 34
	0.8 * 16 = 12.8 ~~ 13   
	UDIVSLOT0 = 0xDFDD(1101_1111_1101_1101b)
*/
//	UBRDIV0 = 34;
//	UDIVSLOT0 = 0xdfdd;
	
	UBRDIV0 = 35;
	UDIVSLOT0 = 0x0888;
}

//uart send a char 
void putc(char a)
{
	while( (UTRSTAT0&0x2) == 0);
	UTXH0 = a;
}

//uart receive a char 
char getc(void)
{
	while( (UTRSTAT0&0x1) == 0);
	return URXH0;
}

void uart_puts(char *a)
{
	while(*a)
	{
		putc(*a);
		a++;
	}
}