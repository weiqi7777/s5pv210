
#include "interrupt.h"
#include "stdio.h"

#define GPD0CON        (*(volatile unsigned long *)0xe02000a0)
                       
#define TCFG0          (*(volatile unsigned long *)0xe2500000)
#define TCFG1	       (*(volatile unsigned long *)0xe2500004)
#define TIMER_CON      (*(volatile unsigned long *)0xe2500008)
#define TCNT1	       (*(volatile unsigned long *)0xe2500018)
#define TCMPB1         (*(volatile unsigned long *)0xe250001c)
#define TCNT2	       (*(volatile unsigned long *)0xe2500024)
#define TCMPB2         (*(volatile unsigned long *)0xe2500028)
#define TINT_CSTAT     (*(volatile unsigned long *)0xe2500044)


#define TIMER1_INT	     NUM_TIMER1
#define TIMER2_INT       NUM_TIMER2

void timer1_interrupt();
void timer2_interrtpt();


void pwm_init()
{
	//set GPD0-2 is TOUT_2
	GPD0CON &= ~(0xff<<4);
	GPD0CON |= (0x22<<4);
	
	//set pwm2 input clk is 1M , pwm1 input clk is 500k
	TCFG0 = (130<<8) | 130 ;
	//set mux1 1/16
	TCFG1 =  (4<<4 | 4<<8);
	
	TCNT2 = 100;
	TCMPB2 = 50;
	
	TCNT1 = 120;
	TCMPB1 = 50;
	
	TINT_CSTAT = (1<<1 | 1<<2);
	//auto-reload,  inverter-on, update TCNTB,TCMPB, 
	TIMER_CON = (1<<15 | 1<<14 | 1<<13 | 1<<11 | 1<<10 | 1<<9 ) ;
	TIMER_CON &= ~(1<<13 | 1<<9) ;
	
	intc_setvectaddr(TIMER1_INT,timer1_interrupt);
	intc_enable(TIMER1_INT);
	
	intc_setvectaddr(TIMER2_INT,timer2_interrtpt);
	intc_enable(TIMER2_INT);
	
	
	//start timer1-2
	TIMER_CON |= (1<<12 | 1<<8);
	printf("TIMER_CON: 0x%x\r\n",TIMER_CON);
}


void timer1_interrupt()
{
	static int change_dir=0;
	static int count=0;
	if(count > 20)
	{
		printf("timer1\r\n");
		count = 0;
		if(change_dir == 0)
		{
			if(TCNT1 < 95)
				TCNT1++;
			else
				change_dir = 1;
		}
		else
		{
			if(TCNT1 > 5)
				TCNT1--;
			else
				change_dir = 0;
		}
	}
	else
		count++;
	
	TINT_CSTAT |= (1<<6);
	VIC0ADDR = 0;
}


void timer2_interrtpt()
{
	static int change_dir=0;
	static int count=0;
	if(count > 20)
	{
		count = 0;
		printf("timer2\r\n");
		if(change_dir == 0)
		{
			if(TCNT2 < 95)
				TCNT2++;
			else
				change_dir = 1;
		}
		else
		{
			if(TCNT2 > 5)
				TCNT2--;
			else
				change_dir = 0;
		}
	}
	else
		count++;
	
	TINT_CSTAT |= (1<<7);
	VIC0ADDR = 0;
}