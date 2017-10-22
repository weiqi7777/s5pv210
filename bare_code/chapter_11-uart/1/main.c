
void main()
{
	char *p = "hello weiqi7777\r\n";
	char receive;
	uart_init();
	pre_power();
	led_config();
	uart_puts(p);
	
	while(1)
	{
		receive = uart_getc();
		uart_putc(receive);
	}
	
	
}