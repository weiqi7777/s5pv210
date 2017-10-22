#ifndef __MAIN_H__
#define __MAIN_H__

// main.h就是用来存放各个外设的操作函数的声明的

// key.c
void key_init_interrupt(void);
void isr_eint2(void);
void isr_eint3(void);
void isr_eint16171819(void);
void key_init(void);
void key_polling(void);

// led.c
void led_blink(void);
void led1(void);
void led2(void);
void led3(void);
void led_off(void);


// uart.c
void uart_init(void);


















#endif



















