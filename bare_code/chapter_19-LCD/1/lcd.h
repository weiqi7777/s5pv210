#ifndef __LCD_H_
#define __LCD_H_

#include "mytype.h"
#define GPF0CON			(*(volatile unsigned long *)0xE0200120)
#define GPF1CON			(*(volatile unsigned long *)0xE0200140)
#define GPF2CON			(*(volatile unsigned long *)0xE0200160)
#define GPF3CON			(*(volatile unsigned long *)0xE0200180)

#define GPD0CON			(*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT			(*(volatile unsigned long *)0xE02000A4)

#define CLK_SRC1		(*(volatile unsigned long *)0xe0100204)
#define CLK_DIV1		(*(volatile unsigned long *)0xe0100304)
#define DISPLAY_CONTROL	(*(volatile unsigned long *)0xe0107008)

#define VIDCON0			(*(volatile unsigned long *)0xF8000000)
#define VIDCON1			(*(volatile unsigned long *)0xF8000004)
#define VIDTCON2		(*(volatile unsigned long *)0xF8000018)
#define WINCON0 		(*(volatile unsigned long *)0xF8000020)
#define WINCON2 		(*(volatile unsigned long *)0xF8000028)
#define SHADOWCON 		(*(volatile unsigned long *)0xF8000034)
#define VIDOSD0A 		(*(volatile unsigned long *)0xF8000040)
#define VIDOSD0B 		(*(volatile unsigned long *)0xF8000044)
#define VIDOSD0C 		(*(volatile unsigned long *)0xF8000048)

#define VIDW00ADD0B0 	(*(volatile unsigned long *)0xF80000A0)
#define VIDW00ADD1B0 	(*(volatile unsigned long *)0xF80000D0)

#define VIDTCON0 		(*(volatile unsigned long *)0xF8000010)
#define VIDTCON1 		(*(volatile unsigned long *)0xF8000014)

#define HSPW 			(40)				// 1~40 DCLK
#define HBPD			(10 - 1)			// 46
#define HFPD 			(240 - 1)			// 16 210 354
#define VSPW			(20)				// 1~20 DCLK
#define VBPD 			(10 - 1)			// 23
#define VFPD 			(30 - 1)			// 7 22 147

// FB地址
#define FB_ADDR			(0x23000000)
#define ROW				(480)
#define COL				(800)
#define HOZVAL			(COL-1)
#define LINEVAL			(ROW-1)

#define XSIZE			COL
#define YSIZE			ROW

//define color
#define BLUE   0X0000FF
#define RED    0XFF0000
#define GREEN  0X00FF00
#define WHITE  0xffffff
#define BLACK  0x000000

static void delay(void);
void lcd_init();
static inline void lcd_draw_pixel(u32 x, u32 y, u32 color);
void lcd_draw_backgroud(u32 color);
void lcd_draw_hline(u32 x1, u32 x2, u32 y, u32 color);
void lcd_draw_vline(u32 x, u32 y1, u32 y2, u32 color);
void lcd_draw_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);
void lcd_draw_line_with_dalay(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);
void lcd_draw_circular(u32 centerX, u32 centerY, u32 radius, u32 color);
void lcd_draw_circular_with_delay(u32 centerX, u32 centerY, u32 radius, u32 color);

void lcd_test();
#endif
