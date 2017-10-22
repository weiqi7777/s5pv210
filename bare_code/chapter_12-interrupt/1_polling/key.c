
#define GPH0CON  *((volatile unsigned long *)0xe0200c00)
#define GPH0DAT  *((volatile unsigned long *)0xe0200c04)

#define GPH2CON  *((volatile unsigned long *)0xe0200c40)
#define GPH2DATA *((volatile unsigned long *)0xe0200c44)


void key_init()
{
//set SW5(GPH0_2)  SE6(GPH0_3) is input 
	GPH0CON &= ~(0xff<<8);

//set SW7(GPH2_0) SW8(GPH2_1) SW9(GPH2_2) SW10(GPH2_3) is input 
	GPH2CON &= ~(0XFFFF);
}




