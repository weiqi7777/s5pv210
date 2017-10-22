
#include "interrupt.h"
#include "stdio.h"


void system_init_exception(void)
{
	r_exception_reset		  = (unsigned int )reset_exception;
	r_exception_undef		  = (unsigned int )undef_exception;
	r_exception_soft_int	  = (unsigned int )soft_int_exception;
	r_exception_prefetch      = (unsigned int )prefetch_exception;
	r_exception_dataabort	  = (unsigned int )dataabort_exception;
	r_exception_irq	 		  = (unsigned int )IRQ_handle;
	r_exception_fiq	          = (unsigned int )fiq_exception;
}   	

void  reset_exception(void)
{
	printf("reset_exception.\r\n");
}

void  undef_exception(void)
{
	printf("undef_exception.\r\n");
}
void  soft_int_exception(void)
{
	printf("soft_int_exception.\r\n");
}
void  prefetch_exception(void)
{
	printf("prefetch_exception.\r\n");
}
void  dataabort_exception(void)
{
	printf("dataabort_exception.\r\n");
}

/*
	中断处理程序，不考虑保护现场以及恢复现场，只是中断处理
*/
void  irq_handler(void)
{
	printf("irq_exception.\r\n");
}


void  fiq_exception(void)
{
	printf("fiq_exception.\r\n");
}