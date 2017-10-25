/*
 * 文件名：	led.s	
 * 作者：	朱老师
 * 描述：	演示重定位（在SRAM内部重定位）
 */

#define WTCON		0xE2700000

#define SVC_STACK	0x35000000

.global _start					// 把_start链接属性改为外部，这样其他文件就可以看见_start了
_start:
//run_on_dram:	
	// 长跳转到led_blink开始第二阶段
	ldr sp, =SVC_STACK
	ldr pc, =main				// ldr指令实现长跳转
	
	
// 汇编最后的这个死循环不能丢
	b .
	

	
	
	

	
	
	
	
	
	
	
	
	
