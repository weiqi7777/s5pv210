#include "stdio.h"
#include "interrupt.h"
#include "Hsmmc.h"
#include "rtc.h"
#include "gsensor.h"

#define GPH0CON  *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT  *((volatile unsigned long *)0xe0200c04)

#define GPH2CON  *((volatile unsigned long *)0xe0200c40)
#define GPH2DAT  *((volatile unsigned long *)0xe0200c44)

void key_init(void);

RTC_Typedef *RTC_TIMER;

uint8_t buffer_array[2048];

int main()
{
	char *p = "bl2";	
	int res;
	// 调用中断初始化来初步初始化中断控制器
	system_init_exception();
	key_init();
	printf("%s IIC test\r\n",p);

	gsensor_init();
	
	while(1)
	{
		res = gsensor_write_byte(0x01,0);
		if(res == 1)
		{
			generate_stop_bit();
			printf("send chip address timeout\r\n");
		}
		else if(res == 2)
		{
			generate_stop_bit();
			printf("send data address timeout\r\n");
		}
		else if(res == 3)
		{
			generate_stop_bit();
			printf("send data timeout\r\n");
		}
		Delay_ms(500);
		/*
		gsensor_read_byte(0x3b);
		gsensor_read_byte(0x3c);
		gsensor_read_byte(0x3d);
		gsensor_read_byte(0x3e);
		gsensor_read_byte(0x3f);
		*/
	}
	
	
}