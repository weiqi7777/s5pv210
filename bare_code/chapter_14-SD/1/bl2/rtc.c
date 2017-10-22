
#include "rtc.h"




void rtc_init()
{
	int i;
	unsigned int  *p,*q;
	RTC_TIMER->RTCCON = 0xf<<4 | 1;
	RTC_Typedef ini_RTC_TIMER = {
		0,
	    {0,0,0},
		0xe<<4 | 1,
		1,
		{0,0},
		1<<6|1,
		0,0,0,0,0,0,
		0,
		DEC_T_BCD(50),  DEC_T_BCD(21),   DEC_T_BCD(13),
		DEC_T_BCD(30),  7,   DEC_T_BCD(11),   DEC_T_BCD(15),
		0,
		0
	};
	
	p = (unsigned int *)RTC_TIMER;
	q = (unsigned int *)&ini_RTC_TIMER;
	for(i=0; i<sizeof(RTC_Typedef)/4; i++)
	{
		*p++ = *q++;
	}
	RTC_TIMER->RTCCON |= 1<<8 ;
	// printf("BCDHOUR add=0x%x\r\n",&RTC_TIMER->BCDHOUR);
	// printf("BCDHOUR dat=0x%x\r\n",RTC_TIMER->BCDHOUR);	
	// printf("BCDMIN add=0x%x\r\n",&RTC_TIMER->BCDMIN);
	// printf("BCDMIN dat=0x%x\r\n",RTC_TIMER->BCDMIN);
	// printf("BCDSEC add=0x%x\r\n",&RTC_TIMER->BCDSEC);
	// printf("DCESEC dat=0x%x\r\n",RTC_TIMER->BCDSEC);
	
	// printf("RTCCON add=0x%x\r\n",&RTC_TIMER->RTCCON);
	intc_setvectaddr(TICK_INT, tick_interrupt);
	intc_enable(TICK_INT);
	
	intc_setvectaddr(ALARM_INT, alarm_interrupt);
	intc_enable(ALARM_INT);
	
}

void rtc_display()
{
	static unsigned int before_second = 0;
	if(before_second != RTC_TIMER->BCDSEC)
	{
		before_second = RTC_TIMER->BCDSEC;
		printf("%d%d%d%d-%d%d-%d%d week:%d   %d%d:%d%d:%d%d\r\n",
		2,RTC_TIMER->BCDYEAR>>8&0xf,RTC_TIMER->BCDYEAR>>4&0xf,RTC_TIMER->BCDYEAR&0xf,
		RTC_TIMER->BCDMON>>4,RTC_TIMER->BCDMON&0xf,
		RTC_TIMER->BCDDAY>>4,RTC_TIMER->BCDDAY&0xf,
		RTC_TIMER->BCDDAYWEEK,		
		RTC_TIMER->BCDHOUR>>4,RTC_TIMER->BCDHOUR&0xf,			
		RTC_TIMER->BCDMIN>>4,RTC_TIMER->BCDMIN&0xf,
		RTC_TIMER->BCDSEC>>4,RTC_TIMER->BCDSEC&0xf);
	}
}

void tick_interrupt()
{
	printf("%d%d%d%d-%d%d-%d%d week:%d   %d%d:%d%d:%d%d\r\n",
		2,RTC_TIMER->BCDYEAR>>8&0xf,RTC_TIMER->BCDYEAR>>4&0xf,RTC_TIMER->BCDYEAR&0xf,
		RTC_TIMER->BCDMON>>4,RTC_TIMER->BCDMON&0xf,
		RTC_TIMER->BCDDAY>>4,RTC_TIMER->BCDDAY&0xf,
		RTC_TIMER->BCDDAYWEEK,		
		RTC_TIMER->BCDHOUR>>4,RTC_TIMER->BCDHOUR&0xf,			
		RTC_TIMER->BCDMIN>>4,RTC_TIMER->BCDMIN&0xf,
		RTC_TIMER->BCDSEC>>4,RTC_TIMER->BCDSEC&0xf);
	RTC_TIMER->INIP = 1;
	 VIC0ADDR = 0;
}

void alarm_interrupt()
{
	printf("one minute\r\n");
	RTC_TIMER->INIP = 2;
	 VIC0ADDR = 0;
}