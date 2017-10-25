#include "stdio.h"
#include "mytype.h"

void led_config();
void pre_power();
void uart_init(void);
void adc_init();
uint32_t adc_read();

int main()
{
	char *p = "hello weiqi7777\r\n";
	uint32_t receive;
	volatile int i;
	uart_init();
	pre_power();
	led_config();
	adc_init();
	while(1)
	{
			receive = adc_read();
			printf("AD value : 0x%x\r\n",receive);
			printf("AD value : %d\r\n",receive);
			//printf("voltage : %f\r\n",receive*3.3/4096);

			//delay
			for (i = 0; i < 5000000; i++);
	}

	return 0;
}
