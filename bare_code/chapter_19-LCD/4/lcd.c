#include "lcd.h"

u32 *pfb = (u32 *)FB_ADDR;

static void delay(void)
{
   volatile u32 i,j;
   for (i = 0; i < 500; i++) {
     for (j = 0; j < 100; j++) {
     }
   }
}

// 初始化LCD
void lcd_init(void)
{
	// 配置引脚用于LCD功能
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON = 0x22222222;

	// 打开背光	GPD0_0（PWMTOUT0）
	GPD0CON &= ~(0xf<<0);
	GPD0CON |= (1<<0);			// output mode
	GPD0DAT &= ~(1<<0);			// output 0 to enable backlight

	// 10: RGB=FIMD I80=FIMD ITU=FIMD
	DISPLAY_CONTROL = 2<<0;

	// bit[26~28]:使用RGB接口
	// bit[18]:RGB 并行
	// bit[2]:选择时钟源为HCLK_DSYS=166MHz
	VIDCON0 &= ~( (3<<26)|(1<<18)|(1<<2) );

	// bit[1]:使能lcd控制器
	// bit[0]:当前帧结束后使能lcd控制器
	VIDCON0 |= ( (1<<0)|(1<<1) );

	// bit[6]:选择需要分频
	// bit[6~13]:分频系数为5，即VCLK = 166M/(4+1) = 33M
	VIDCON0 |= 4<<6 | 1<<4;


	// H43-HSD043I9W1.pdf(p13) 时序图：VSYNC和HSYNC都是低脉冲
	// s5pv210芯片手册(p1207) 时序图：VSYNC和HSYNC都是高脉冲有效，所以需要反转
	VIDCON1 |= 1<<5 | 1<<6;

	// 设置时序
	VIDTCON0 = VBPD<<16 | VFPD<<8 | VSPW<<0;
	VIDTCON1 = HBPD<<16 | HFPD<<8 | HSPW<<0;

	// 设置长宽(物理屏幕)
	VIDTCON2 = (LINEVAL << 11) | (HOZVAL << 0);

	// 设置window0
	// bit[0]:使能
	// bit[2~5]:24bpp（RGB888）
	WINCON0 |= 1<<0;
	WINCON0 &= ~(0xf << 2);
	WINCON0 |= (0xB<<2) | (1<<15);

#define LeftTopX     0
#define LeftTopY     0
#define RightBotX   799
#define RightBotY   479

	// 设置window0的上下左右
	// 设置的是显存空间的大小
	VIDOSD0A = (LeftTopX<<11) | (LeftTopY << 0);
	VIDOSD0B = (RightBotX<<11) | (RightBotY << 0);
	VIDOSD0C = (LINEVAL + 1) * (HOZVAL + 1);


	// 设置fb的地址
	VIDW00ADD0B0 = FB_ADDR;
	VIDW00ADD1B0 = (((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff);

	// 使能channel 0传输数据
	SHADOWCON = 0x1;
}

static inline void lcd_draw_pixel(u32 x, u32 y, u32 color)
{
		*(pfb + (COL * y + x)) = color;
}

void lcd_draw_backgroud(u32 color)
{
	 int width = COL;
	 int hight = ROW;
	 for (hight = 0; hight < ROW; hight++) {
		 for (width = 0; width < COL; width++) {
			 	lcd_draw_pixel(width,hight,color);
		 }
	 }
}

void lcd_test(void)
{
	 lcd_init();
   lcd_draw_backgroud(WHITE);
   lcd_draw_picture(gImage_800600);

   while(1)
	 {

	 }
}

void lcd_draw_hline(u32 x1, u32 x2, u32 y, u32 color)
{
	 u32 x;
	 for (x = x1; x < x2; x++) {
	 	  lcd_draw_pixel(x1,y,color);
	 }
}


void lcd_draw_vline(u32 x, u32 y1, u32 y2, u32 color)
{
	 u32 y;
	 for (y = y1; y < y2; y++) {
      lcd_draw_pixel(x,y,color);
	 }
}

void lcd_draw_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color)
{
	 int dx, dy, e;
	 dx = x2 - x1;
	 dy = y2 - y1;
	 if(dx >=0)  //dx > 0
	 {
		  if (dy >= 0)  //dy > 0
			{
		  	if (dx > dy) {
					e = dy - dx/2;
					while(x1 <= x2)
					{
						 lcd_draw_pixel(x1,y1,color);
						 if(e > 0){
							  y1 += 1;
								e -= dx;
						 }
						 x1 += 1;
						 e += dy;
					}
		  	}
				else
				{
					 e = dx - dy/2;
					 while(y1 <= y2)
					 {
						  lcd_draw_pixel(x1,y1,color);
							if (e > 0 ) {
								x1 += 1;
								e -= dy;
							}
							y1 += 1;
							e += dx;
					 }
				}
		  }
			else  //dy < 0
			{
				 dy = -dy;
				 if( dx >= dy)
				 {
					  e = dy - dx/2;
						while(x1 <= x2)
						{
							 lcd_draw_pixel(x1,y1,color);
							 if(e>0)
							 {
								  y1 -= 1;
									e -= dx;
							 }
							 x1 += 1;
							 e += dy;
						}
				 }
				 else
				 {
					  e = dx - dy/2;
						while(y1 >= y2)
						{
							 lcd_draw_pixel(x1,y1,color);
							 if(e>0)
							 {
								 x1 += 1;
								 e -= dy;
							 }
							 y1 -=1;
							 e += dx;
						}
				 }
			}
	 }
	 else   //dx <0
	 {
		   dx  = -dx;
			 if(dy >= 0) //dy >= 0
			 {
				  if (dx >= dy) {
						e = dy - dx/2;
						while (x1 > x2) {
							lcd_draw_pixel(x1,y1,color);
							if (e > 0) {
								y1 += 1;
								e -= dx;
							}
							x1 -= 1;
							e += dy;
						}
				  } else {
						  e = dx - dy /2;
							while (y1 < y2) {
								lcd_draw_pixel(x1,y1,color);
								if (e>0) {
									x1 -= 1;
									e -= dy;
								}
								y1 += 1;
								e += dx;
							}
				  }
			 }
			 else   //dy <0
			 {
				  dy = -dy;
					if (dx >= dy) {
						e = dy -dx/2;
						while(x1 >= x2)
						{
							lcd_draw_pixel(x1,y1,color);
							if (e > 0) {
								y1 -= 1;
								e -= dx;
							}
							x1 -= 1;
							e += dy;
						}
					} else {
						 e = dx - dy/2;
						 while(y1 >= y2)
						 {
							  lcd_draw_pixel(x1,y1,color);
								if (e > 0) {
									x1 -= 1;
									e -= dy;
								}
								y1 -= 1;
								e += dx;
						 }
					}
			 }
	 }
}

void lcd_draw_line_with_dalay(u32 x1, u32 y1, u32 x2, u32 y2, u32 color)
{
	 int dx, dy, e;
	 dx = x2 - x1;
	 dy = y2 - y1;
	 if(dx >=0)  //dx > 0
	 {
		  if (dy >= 0)  //dy > 0
			{
		  	if (dx > dy) {
					e = dy - dx/2;
					while(x1 <= x2)
					{
						 lcd_draw_pixel(x1,y1,color);
						 delay();
						 if(e > 0){
							  y1 += 1;
								e -= dx;
						 }
						 x1 += 1;
						 e += dy;
					}
		  	}
				else
				{
					 e = dx - dy/2;
					 while(y1 <= y2)
					 {
						  lcd_draw_pixel(x1,y1,color);
							delay();
							if (e > 0 ) {
								x1 += 1;
								e -= dy;
							}
							y1 += 1;
							e += dx;
					 }
				}
		  }
			else  //dy < 0
			{
				 dy = -dy;
				 if( dx >= dy)
				 {
					  e = dy - dx/2;
						while(x1 <= x2)
						{
							 lcd_draw_pixel(x1,y1,color);
							 delay();
							 if(e>0)
							 {
								  y1 -= 1;
									e -= dx;
							 }
							 x1 += 1;
							 e += dy;
						}
				 }
				 else
				 {
					  e = dx - dy/2;
						while(y1 >= y2)
						{
							 lcd_draw_pixel(x1,y1,color);
							 delay();
							 if(e>0)
							 {
								 x1 += 1;
								 e -= dy;
							 }
							 y1 -=1;
							 e += dx;
						}
				 }
			}
	 }
	 else   //dx <0
	 {
		   dx  = -dx;
			 if(dy >= 0) //dy >= 0
			 {
				  if (dx >= dy) {
						e = dy - dx/2;
						while (x1 > x2) {
							lcd_draw_pixel(x1,y1,color);
							 delay();
							if (e > 0) {
								y1 += 1;
								e -= dx;
							}
							x1 -= 1;
							e += dy;
						}
				  } else {
						  e = dx - dy /2;
							while (y1 < y2) {
								lcd_draw_pixel(x1,y1,color);
							 delay();
								if (e>0) {
									x1 -= 1;
									e -= dy;
								}
								y1 += 1;
								e += dx;
							}
				  }
			 }
			 else   //dy <0
			 {
				  dy = -dy;
					if (dx >= dy) {
						e = dy -dx/2;
						while(x1 >= x2)
						{
							lcd_draw_pixel(x1,y1,color);
							 delay();
							if (e > 0) {
								y1 -= 1;
								e -= dx;
							}
							x1 -= 1;
							e += dy;
						}
					} else {
						 e = dx - dy/2;
						 while(y1 >= y2)
						 {
							  lcd_draw_pixel(x1,y1,color);
							 delay();
								if (e > 0) {
									x1 -= 1;
									e -= dy;
								}
								y1 -= 1;
								e += dx;
						 }
					}
			 }
	 }
}

void lcd_draw_circular(u32 centerX, u32 centerY, u32 radius, u32 color)
{
	int x,y ;
	int tempX,tempY;;
    int SquareOfR = radius*radius;

	for(y=0; y<XSIZE; y++)
	{
		for(x=0; x<YSIZE; x++)
		{
			if(y<=centerY && x<=centerX)
			{
				tempY=centerY-y;
				tempX=centerX-x;
			}
			else if(y<=centerY&& x>=centerX)
			{
				tempY=centerY-y;
				tempX=x-centerX;
			}
			else if(y>=centerY&& x<=centerX)
			{
				tempY=y-centerY;
				tempX=centerX-x;
			}
			else
			{
				tempY = y-centerY;
				tempX = x-centerX;
			}
			if ((tempY*tempY+tempX*tempX)<=SquareOfR)
				lcd_draw_pixel(x, y, color);
		}
	}
}


void lcd_draw_circular_with_delay(u32 centerX, u32 centerY, u32 radius, u32 color)
{
	int x,y ;
	int tempX,tempY;;
    int SquareOfR = radius*radius;

	for(y=0; y<XSIZE; y++)
	{
		for(x=0; x<YSIZE; x++)
		{
			if(y<=centerY && x<=centerX)
			{
				tempY=centerY-y;
				tempX=centerX-x;
			}
			else if(y<=centerY&& x>=centerX)
			{
				tempY=centerY-y;
				tempX=x-centerX;
			}
			else if(y>=centerY&& x<=centerX)
			{
				tempY=y-centerY;
				tempX=centerX-x;
			}
			else
			{
				tempY = y-centerY;
				tempX = x-centerX;
			}
			if ((tempY*tempY+tempX*tempX)<=SquareOfR)
			{
				lcd_draw_pixel(x, y, color);
				delay();
			}
		}
	}
}

void lcd_draw_picture(const u8 *pic)
{
   u32 x,y;
   const u8 *data = pic;
   u32 color;
   for(y=0; y<ROW; y++)
   {
     for (x = 0; x < COL; x++) {
        color = data[0] | (data[1] <<8) | (data[2]<<16);
        data += 3;
        lcd_draw_pixel(x,y,color);
     }
   }
}
