#include "stdio.h"


#define GPH0CON  *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT  *((volatile unsigned long *)0xe0200c04)

#define GPH2CON  *((volatile unsigned long *)0xe0200c40)
#define GPH2DAT  *((volatile unsigned long *)0xe0200c44)

void key_init(void);
void led_config(void);
void pre_power(void);
void uart_init(void);

int main()
{
	char *p = "hello weiqi7777";
	//char receive;
	uart_init();
	pre_power();
	key_init();
	led_config();
	printf("%s\r\n",p);
	
	while(1)
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
	
	return 0;
}