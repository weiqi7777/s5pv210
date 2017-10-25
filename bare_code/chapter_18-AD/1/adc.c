
#include "mytype.h"

#define TSADCCON  (*(volatile unsigned long *)0xe1700000)
#define TSCON     (*(volatile unsigned long *)0xe1700004)
#define TSDATX    (*(volatile unsigned long *)0xe170000c)
#define ADCMUX    (*(volatile unsigned long *)0xe170001c)


void adc_init()
{
   TSADCCON = 0;
   TSADCCON = (1<<16 | 1<<14 | 35<<6 | 0 << 2 | 0 <<1 | 0<< 0);
   ADCMUX = 0;
}

uint32_t adc_read()
{
   uint32_t data=0;
   int i;
   //enable ADC converter
   TSADCCON |= 0x2;
   TSDATX;
   //wait ADC converter finish
   for (i = 0; i < 32; i++) {
     while(!(TSADCCON & (1<<15)));
     //read ADC value
     data += (TSDATX & 0xfff);
   }
   TSADCCON &= ~(1<<1);
   //return ADC value
   return (data>>5);
}
