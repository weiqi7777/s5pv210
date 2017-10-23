#include "stdio.h"
#include "interrupt.h"
#include "Hsmmc.h"
#include "rtc.h"

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
	int i=50;
	int res;	
	unsigned char *temp=buffer_array;
	// 调用中断初始化来初步初始化中断控制器
	system_init_exception();
	key_init();
	printf("%s\r\n",p);
	printf("SD test\r\n");
	while(i)
	{
		res = Hsmmc_Init();
		if(res != 0)
		{
			printf("detect SD error\r\n");
		}
		else
		{
			printf("detect SD \r\n");
			break;
		}
		i--;
		Delay_ms(100);
	}
	if(i==0)
	{
		printf("SD error, maybe program is not right\r\n");
		return 1;
	}
	
	Hsmmc_ReadBlock(buffer_array,63136,4);
	printf("SD read finish\r\n");
	while(*temp)
	{
		printf("%c",*temp++);
	}
	return 0;
}