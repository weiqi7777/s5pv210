#include "mytype.h"
#include "lcd.h"

#include "800600.h"

void pre_power(void);
void led_config();


int main()
{
	pre_power();
led_config();
  lcd_test();
	return 0;
}
