#include "interrupt.h"
#define WTCON    (*(volatile unsigned long *)0xe2700000)
#define WTDAT	 (*(volatile unsigned long *)0xe2700004)
#define WTCNT    (*(volatile unsigned long *)0xe2700008)
#define WTCLRINT (*(volatile unsigned long *)0xe270000c)

#define WDT_INT  NUM_WDT

void wdt_isr();

void wdt_init()
{
	WTCON |= (1<<5 | 1<<2 | 1<<0);
	intc_setvectaddr(WDT_INT,wdt_isr);
	intc_enable(WDT_INT);
}


void wdt_isr()
{
	printf("wdt interrupt\r\n");
	VIC0ADDR = 0;
	WTCLRINT = 1;
}