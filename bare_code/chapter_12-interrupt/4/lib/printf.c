#include "vsprintf.h"
#include "string.h"
#include "printf.h"

extern void putc(unsigned char c);
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024

// 自己定义了2个全局变量数组，分别作为发送/接收缓冲区。
// 将来发送时先将要发送的信息格式化送入发送缓冲区，然后putc函数直接从发送缓冲区取
// 数据发送出去。
static char g_pcOutBuf[OUTBUFSIZE];
static char g_pcInBuf[INBUFSIZE];


// putc函数真正和输出设备绑定，这个函数需要我们自己去实现，这就是移植的关键
// printf("a = %d, b = %s.\n", a, p);
int printf(const char *fmt, ...)
{
	int i;
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsprintf(g_pcOutBuf,fmt,args);
	va_end(args);
	for (i = 0; i < strlen(g_pcOutBuf); i++)
	{
		putc(g_pcOutBuf[i]);
	}
	return len;
}



int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
	while(1)
	{
		c = getc();
		putc(c);
		if((c == 0x0d) || (c == 0x0a))
		{
			g_pcInBuf[i] = '\0';
			break;
		}
		else
		{
			g_pcInBuf[i++] = c;
		}
	}
	
	va_start(args,fmt);
	i = vsscanf(g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}

