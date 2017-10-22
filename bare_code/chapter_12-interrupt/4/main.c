#include "stdio.h"
#include "int.h"
#include "main.h"

void uart_init(void);

#define KEY_EINT2		NUM_EINT2		// left
#define KEY_EINT3		NUM_EINT3		// down
#define KEY_EINT16_19	NUM_EINT16_31	// 其余4个共用的

void delay(int i)
{
	volatile int j = 10000;
	while (i--)
		while(j--);
}


int main(void)
{
	uart_init();
	//key_init();
	key_init_interrupt();
	
	// 如果程序中要使用中断，就要调用中断初始化来初步初始化中断控制器
	system_init_exception();
	
	printf("-------------key interrypt test--------------");
	
	// 绑定isr到中断控制器硬件
	intc_setvectaddr(KEY_EINT2, isr_eint2);
	intc_setvectaddr(KEY_EINT3, isr_eint3);
	intc_setvectaddr(KEY_EINT16_19, isr_eint16171819);
	
	// 使能中断
	intc_enable(KEY_EINT2);
	intc_enable(KEY_EINT3);
	intc_enable(KEY_EINT16_19);
	

	
	printf("r_exception_irq: %x\r\n",r_exception_irq);
	printf("VIC0INTENABLE:   %x\r\n",VIC0INTENABLE);
	
	printf("inter2 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(2-0)) ));
	printf("inter3 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(3-0)) ));
	printf("inter6-11 add:   %x\r\n", *( (volatile unsigned long *)(VIC0VECTADDR + 4*(16-0)) ));

	
	// 在这里加个心跳
	while (1)
	{
		//printf("A ");
		//delay(10000);
	}

	return 0;
}