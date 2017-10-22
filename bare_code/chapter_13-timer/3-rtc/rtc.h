
#ifndef __RTC_H_
#define __RTC_H_

#include "stdio.h"
#include "interrupt.h"
#define __IO    volatile 

#define DEC_T_BCD(n) ( n/10<<4 | n%10 )
#define TICK_INT  NUM_RTC_TICK
#define ALARM_INT NUM_RTC_ALARM

typedef struct {
  __IO unsigned long INIP;
  __IO unsigned long reserved0[3];
  __IO unsigned long RTCCON;
  __IO unsigned long TICCNT;
  __IO unsigned long reserved1[2];
  __IO unsigned long RTCALM;
  __IO unsigned long ALMSEC;
  __IO unsigned long ALMMIN;
  __IO unsigned long ALMHOUR;
  __IO unsigned long ALMDAY;
  __IO unsigned long ALMMON;
  __IO unsigned long ALMYEAR;
  __IO unsigned long reserved2;
  __IO unsigned long BCDSEC;
  __IO unsigned long BCDMIN;
  __IO unsigned long BCDHOUR;
  __IO unsigned long BCDDAY;
  __IO unsigned long BCDDAYWEEK;
  __IO unsigned long BCDMON;
  __IO unsigned long BCDYEAR;
  __IO unsigned long reserved3;
  __IO unsigned long CURTICNT;
}RTC_Typedef;

extern RTC_Typedef *RTC_TIMER ;

void rtc_init();
void rtc_display();

void tick_interrupt();
void alarm_interrupt();
#endif