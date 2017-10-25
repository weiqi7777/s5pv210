#include "stdio.h"
#include "interrupt.h"

#define GPH0CON  *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT  *((volatile unsigned long *)0xe0200c04)

#define GPH2CON  *((volatile unsigned long *)0xe0200c40)
#define GPH2DAT  *((volatile unsigned long *)0xe0200c44)

void key_init(void);
void led_config(void);
void pre_power(void);
void uart_init(void);

typedef void (*pBL2Type)(void);


int main()
{
	char *p = "bl1";
	int i;
	int res;
	unsigned int *ram = (unsigned int *)0x23e00000;
	unsigned int *q =ram;
	uart_init();
	pre_power();
	
	led_config();
	
	// 调用中断初始化来初步初始化中断控制器
	system_init_exception();
	key_init();
	

	printf("%s\r\n",p);
	for(i=0; i<32; i++)
	{
		
		if(i%8==0 && i !=0)
			printf("\r\n");
		printf("0x%x ",*q++);
	}
	printf("\r\n");
	printf("copy before\r\n");
	copy_bl2_to_ddr();
	q = ram;
	for(i=0; i<32; i++)
	{
		
		if(i%8==0 && i !=0)
			printf("\r\n");
		printf("0x%x ",*q++);
	}
	printf("\r\n");
	printf("copy finish\r\n");
	
	pBL2Type p2 = (pBL2Type)0x23e00000;
	p2();
	
}