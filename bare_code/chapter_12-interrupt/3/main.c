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



int main()
{
	char *p = "hello weiqi7777";
	uart_init();
	pre_power();
	
	led_config();
	
	// 调用中断初始化来初步初始化中断控制器
	system_init_exception();
	key_init();
	

	printf("%s\r\n",p);
	
	while(1)
	{

	}
	
	return 0;
}