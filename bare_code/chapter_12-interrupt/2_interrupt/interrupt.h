
#ifndef __INT_H__
#define __INT_H__

#define  exception_vector_base    0xd0037400
#define  exception_reset		  (exception_vector_base + 0x0 )
#define  exception_undef		  (exception_vector_base + 0x4 )
#define  exception_soft_int		  (exception_vector_base + 0x8 )
#define  exception_prefetch		  (exception_vector_base + 0xC )
#define  exception_dataabort	  (exception_vector_base + 0x10)
#define  exception_irq			  (exception_vector_base + 0x18)
#define  exception_fiq			  (exception_vector_base + 0x1C)

#define  r_exception_reset		  (*(volatile unsigned int *)exception_reset)
#define  r_exception_undef		  (*(volatile unsigned int *)exception_undef)
#define  r_exception_soft_int	  (*(volatile unsigned int *)exception_soft_int)
#define  r_exception_prefetch	  (*(volatile unsigned int *)exception_prefetch)
#define  r_exception_dataabort	  (*(volatile unsigned int *)exception_dataabort)
#define  r_exception_irq	 	  (*(volatile unsigned int *)exception_irq)
#define  r_exception_fiq		  (*(volatile unsigned int *)exception_fiq)


void  reset_exception(void)     ;
void  undef_exception(void)     ;
void  soft_int_exception(void)  ;
void  prefetch_exception(void)  ;
void  dataabort_exception(void) ;
void  irq_handler(void)       ;
void  IRQ_handle(void);
void  fiq_exception(void)       ;
void  system_init_exception(void) ;
int printf(const char *fmt, ...);



#endif
