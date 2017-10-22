/*
 * 文件名：	start.S
 * 作者：	朱老师
 * 描述：	演示串口通信
 */

#define WTCON		0xE2700000

#define SVC_STACK	0xd0037d80
#define IRQ_STACK	0xd0037f80

.global _start	
.global IRQ_handle	

// 把_start链接属性改为外部，这样其他文件就可以看见_start了
_start:
	// 第1步：关看门狗（向WTCON的bit5写入0即可）
	ldr r0, =WTCON
	ldr r1, =0x0
	str r1, [r0]
	
	// 第2步：初始化时钟
	bl clock_init
	
	// 第3步：设置SVC栈
	ldr sp, =SVC_STACK
	
	// 第4步：开/关icache
	mrc p15,0,r0,c1,c0,0;			// 读出cp15的c1到r0中
	//bic r0, r0, #(1<<12)			// bit12 置0  关icache
	orr r0, r0, #(1<<12)			// bit12 置1  开icache
	mcr p15,0,r0,c1,c0,0;

	bl main
	
	// 从这里之后就可以开始调用C程序了
	//bl led_blink					// led_blink是C语言实现的一个函数
	
// 汇编最后的这个死循环不能丢
	b .
	
// 在这个汇编函数中，用来做中断模式下的现场保护和恢复，并且调用真正的中断处理程序

IRQ_handle:
	// 设置IRQ模式下的栈
	ldr sp, =IRQ_STACK
	// 保存LR
	// 因为ARM有流水线，所以PC的值会比真正执行的代码+8，
	sub lr, lr, #4
	// 保存r0-r12和lr到irq模式下的栈上面
	stmfd sp!, {r0-r12, lr}
	// 在此调用真正的isr来处理中断
	bl irq_handler
	// 处理完成开始恢复现场,其实就是做中断返回，关键是将r0-r12，pc，cpsr一起回复
	ldmfd sp!, {r0-r12, pc}^
	
	

	
	
	
	
	
	
	
	
	
