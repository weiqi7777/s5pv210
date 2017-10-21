
#define GPJBASE  0XE0200240
#define GPJ0CON  ((volatile unsigned long *)GPJBASE)
#define GPJ0DAT  ((volatile unsigned long *)(GPJBASE + 0x4))
#define GPDBASE  0xE02000A0
#define GPD0CON  ((volatile unsigned long *)GPDBASE)
#define GPD0DAT  ((volatile unsigned long *)(GPDBASE + 0x4))


int global_var1 = 15;
int global_var2 = 12;
char *p = "hello weiqi77777";

void led_config()
{
	unsigned int config = 0;
	config = config | 0x111000;
	*GPJ0CON = config;
	
	config = 0;
	config = config | (0x1 << 4);
	*GPD0CON = config;
}

void delay(int k)
{
	volatile int i = k;
	while(i--);
}

void led()
{
	unsigned int led_state = 0x1;
	int i;
	int delay_value = 0x5ffff;
	led_config();
	while(1)
	{
		led_state = 0x1;
		for(i=0; i<5; i++)
		{
			*GPJ0DAT = ~(led_state<<3);
			*GPD0DAT = ~((led_state&0x8>>2)&0x2);
			led_state = led_state << 1;
			delay(delay_value);
		}
		// led_state = 0x0;
		// *GPJ0DAT = (led_state<<3);
		// *GPD0DAT = (led_state<<1);
		// delay();
		// led_state = ~0x0;
		// *GPJ0DAT = (led_state<<3);
		// *GPD0DAT = (led_state<<1);
		// delay();
		
	}
	
	
}