#include "stdio.h"

void led_config();
void pre_power();
void uart_init(void);

int main()
{
	char *p = "hello weiqi7777\r\n";
	char receive;
	uart_init();
	pre_power();
	led_config();
	printf("%s\r\n",p);
	
	while(1)
	{
		receive = getc();
		if(receive == '\r' || receive == '\n')
		{
			printf("\r\n");
		}
		else
			putc(receive);
	}
	
	return 0;
}