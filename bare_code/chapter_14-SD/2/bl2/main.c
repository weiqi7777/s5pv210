#include "stdio.h"
#include "interrupt.h"
#include "rtc.h"

#define GPH0CON  *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT  *((volatile unsigned long *)0xe0200c04)

#define GPH2CON  *((volatile unsigned long *)0xe0200c40)
#define GPH2DAT  *((volatile unsigned long *)0xe0200c44)

void key_init(void);
void led_config(void);
void pre_power(void);
void uart_init(void);

RTC_Typedef *RTC_TIMER = ((RTC_Typedef *)0xe2800030);


int main()
{
	char *p = "bl2";
	
	uart_init();
	pre_power();
	
	//led_config();
	
	// 调用中断初始化来初步初始化中断控制器
	system_init_exception();
	key_init();
	//pwm_init();
	rtc_init();
	printf("%s\r\n",p);
	//wdt_init();
	while(1)
	{
		//rtc_display();
	}
	
	return 0;
}