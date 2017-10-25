#ifndef __GENSENSOR_H_
#define __GENSENSOR_H_
#include "stdio.h"

typedef unsigned char uint8_t;
#define IIC_USE_CHANNEL 2

#if (IIC_USE_CHANNEL == 0) 
	#define  I2CCON    (*(volatile unsigned long *)0xe1800000)
	#define  I2CSTAT   (*(volatile unsigned long *)0xe1800004)
	#define  I2CADD	   (*(volatile unsigned long *)0xe1800008)
	#define  I2CDS	   (*(volatile unsigned long *)0xe180000c)	
	#define  I2CCLC    (*(volatile unsigned long *)0xe1800010)
#elif (IIC_USE_CHANNEL == 1)
	#define  I2CCON    (*(volatile unsigned long *)0xfab00000)
	#define  I2CSTAT   (*(volatile unsigned long *)0xfab00004)
	#define  I2CADD	   (*(volatile unsigned long *)0xfab00008)
	#define  I2CDS	   (*(volatile unsigned long *)0xfab0000c)	
	#define  I2CCLC    (*(volatile unsigned long *)0xfab00010)
#elif (IIC_USE_CHANNEL == 2)
	#define  I2CCON    (*(volatile unsigned long *)0xe1a00000)
	#define  I2CSTAT   (*(volatile unsigned long *)0xe1a00004)
	#define  I2CADD	   (*(volatile unsigned long *)0xe1a00008)
	#define  I2CDS	   (*(volatile unsigned long *)0xe1a0000c)
	#define  I2CCLC    (*(volatile unsigned long *)0xe1a00010)
#else
	#error "Configure IIC: IIC0 ~ IIC2(0 ~ 2)"
#endif	

#define TIMEOUT_VALUE 5000


#define  GPD1CON   (*(volatile unsigned long *)0xe02000c0)
#define  GPD1PUD   (*(volatile unsigned long *)0xe02000c8)

#define SLAVE_ADDRESS_W   (0x68 << 1)
#define SLAVE_ADDRESS_R   ((0x68 << 1) | 1) 

#define IIC_CON_VALUE  (1 << 7 | 1<<6 | 1<<5 | 0x1)

#define DETECT_TIMEOUT

void gsensor_init(void);	
int gsensor_read_byte(uint8_t add , uint8_t *data);
int gsensor_write_byte(uint8_t add, uint8_t data);
int generate_stop_bit();
void delay_us(int n);
#endif