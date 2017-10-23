
#define GPH0CON (*(volatile unsigned long *)0xe0200C00)
#define GPH0DAT (*(volatile unsigned long *)0xe0200C04)


void pre_power()
{
	GPH0CON &= ~0xf;
	GPH0CON |= 0x1;	
	GPH0DAT |= 0x1;
}