#define GPJ0CON		0xE0200240
#define GPJ0DAT		0xE0200244

#define rGPJ0CON	*((volatile unsigned int *)GPJ0CON)
#define rGPJ0DAT	*((volatile unsigned int *)GPJ0DAT)

void delay(void);

void led1(void)
{
	rGPJ0CON = 0x11111111;
	rGPJ0DAT = ((0<<3) | (1<<4) | (1<<5));
}

void led2(void)
{
	rGPJ0CON = 0x11111111;
	rGPJ0DAT = ((0<<3) | (0<<4) | (1<<5));
}

void led3(void)
{
	rGPJ0CON = 0x11111111;
	rGPJ0DAT = ((0<<3) | (0<<4) | (0<<5));
}

// 该函数要实现led闪烁效果
void led_blink(void)
{
	// led初始化，也就是把GPJ0CON中设置为输出模式
	//volatile unsigned int *p = (unsigned int *)GPJ0CON;
	//volatile unsigned int *p1 = (unsigned int *)GPJ0DAT;
	rGPJ0CON = 0x11111111;
	
	while (1)
	{
		// led亮
		rGPJ0DAT = ((0<<3) | (0<<4) | (0<<5));
		// 延时
		delay();
		// led灭
		rGPJ0DAT = ((1<<3) | (1<<4) | (1<<5));
		// 延时
		delay();
	}
}


void delay(void)
{
	volatile unsigned int i = 900000;		// volatile 让编译器不要优化，这样才能真正的减
	while (i--);							// 才能消耗时间，实现delay
}

	
