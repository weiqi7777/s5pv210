#include "interrupt.h"
#include "stdio.h"


#define GPH0CON         *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT         *((volatile unsigned long *)0xe0200c04)
                        
#define GPH2CON         *((volatile unsigned long *)0xe0200c40)
#define GPH2DAT         *((volatile unsigned long *)0xe0200c44)

#define EXT_INT_0_CON   *((volatile unsigned long *)0xe0200e00)
#define EXT_INT_2_CON   *((volatile unsigned long *)0xe0200e08)
#define EXT_INT_0_MASK  *((volatile unsigned long *)0xe0200f00)
#define EXT_INT_2_MASK  *((volatile unsigned long *)0xe0200f08)
#define EXT_INT_0_PEND  *((volatile unsigned long *)0xe0200f40)
#define EXT_INT_2_PEND  *((volatile unsigned long *)0xe0200f48)

#define SW5_INT	       NUM_EINT2
#define SW6_INT        NUM_EINT3
#define SW7_11_INT     NUM_EINT16_31

void key_sw5_isr();
void key_sw6_isr();
void key_sw7_11_isr();

void key_init()
{
//set SW5(GPH0_2)  SE6(GPH0_3) is EXT_INT 
	GPH0CON |= (0xff<<8);

//set SW7(GPH2_0) SW8(GPH2_1) SW9(GPH2_2) SW10(GPH2_3) is EXT_INT
	GPH2CON |= 0XFFFF;

//set EXT falling edge tiggered	
	EXT_INT_0_CON &= ~(0xff<<8);
	EXT_INT_0_CON |= (0x22<<8);
	
	EXT_INT_2_CON &= ~(0xffff);
	EXT_INT_2_CON |= 0x2222;

//	
	EXT_INT_0_MASK &= ~0xc;
	EXT_INT_2_MASK &= ~0xf;
	
//
	EXT_INT_0_PEND = 0xffffffff;
	EXT_INT_2_PEND = 0xffffffff;
	
	
//set EXT enable, and set isr address	
	intc_setvectaddr(SW5_INT,key_sw5_isr);
	intc_enable(SW5_INT);
	intc_setvectaddr(SW6_INT,key_sw6_isr);
	intc_enable(SW6_INT);
	intc_setvectaddr(SW7_11_INT,key_sw7_11_isr);
	intc_enable(SW7_11_INT);
	
	// printf("GPH0CON : %x\r\n",GPH0CON);
	// printf("GPH2CON : %x\r\n",GPH2CON);
	// printf("EXT_INT_0_CON: %x\r\n",EXT_INT_0_CON);
	// printf("EXT_INT_2_CON: %x\r\n",EXT_INT_2_CON);
	// printf("EXT_INT_0_MASK: %x\r\n",EXT_INT_0_MASK);
	// printf("EXT_INT_2_MASK: %x\r\n",EXT_INT_2_MASK);
	// printf("EXT_INT_0_PEND: %x\r\n",EXT_INT_0_PEND);
	// printf("EXT_INT_2_PEND: %x\r\n",EXT_INT_2_PEND);
	
	// printf("r_exception_irq: %x\r\n",r_exception_irq);
	// printf("VIC0INTENABLE:   %x\r\n",VIC0INTENABLE);
	
	// printf("inter2 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(2-0)) ));
	// printf("inter3 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(3-0)) ));
	// printf("inter6-11 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(16-0)) ));
}


void key_sw5_isr()
{
	printf("sw5 is pressed\r\n");
	VIC0ADDR = 0;
	EXT_INT_0_PEND |= 0x4; 
}

void key_sw6_isr()
{
	printf("sw6 is pressed\r\n");
	VIC0ADDR = 0;
	EXT_INT_0_PEND |= 0x8;
}

void key_sw7_11_isr()
{
	if(EXT_INT_2_PEND & 0x01)
	{
		printf("sw7 is pressed\r\n");
		EXT_INT_2_PEND |= 0x1;
	}
	else if(EXT_INT_2_PEND & 0X02)
	{
		printf("sw8 is pressed\r\n");
		EXT_INT_2_PEND |= 0x2;
	}
	else if(EXT_INT_2_PEND & 0X04)
	{
		printf("sw9 is pressed\r\n");
		EXT_INT_2_PEND |= 0x4;
	}		
	else
	{
		printf("sw10 is pressed\r\n");
		EXT_INT_2_PEND |= 0x8;
	}
	VIC0ADDR = 0;
}

void key_detect()
{
	if((GPH0DAT&0xc) != 0xc)
	{
		if((GPH0DAT&0x8) == 0)
		{
			printf("SW5 is pressed.\r\n");
			while((GPH0DAT&0x8) == 0);
		}
		if((GPH0DAT&0x4) == 0)
		{
			printf("SW6 is pressed.\r\n");
			while((GPH0DAT&0x4) == 0);
		}
	}
	if((GPH2DAT&0xf) != 0xf)
	{
		if((GPH2DAT & 0X1) == 0)
		{
			printf("SW7 is pressed.\r\n");
			while((GPH2DAT & 0X1) == 0);
		}
		if((GPH2DAT & 0X2) == 0)
		{
			printf("SW8 is pressed.\r\n");
			while((GPH2DAT & 0X2) == 0);
		}
		if((GPH2DAT & 0X4) == 0)
		{
			printf("SW9 is pressed.\r\n");
			while((GPH2DAT & 0X4) == 0);
		}
		if((GPH2DAT & 0X8) ==0)
		{
			printf("SW10 is pressed.\r\n");
			while((GPH2DAT & 0X8) ==0);
		}
	}
}




