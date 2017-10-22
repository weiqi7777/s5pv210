/*
   file name: start.s
   author:   lujun 
   description: implement uart
*/

.global IRQ_handle
.global _start
_start:

	
enable_interrupt:
	mrs r0, cpsr
	bic r0, r0, #0x00000080
	msr cpsr, r0
	
	# recolate
	# adr指令加载_start当前运行地址
	adr r0, _start
	# ldr指令加载_start的链接地址
	ldr r1, =_start
	#  bss段的起始地址
	ldr r2, =bss_start
	//add r2, r2 ,#3
	cmp r0, r1      // 比较_start的运行时地址和链接地址是否相等
	beq clean_bss   // 相等说明，是在dram中运行，不需要拷贝代码
	
copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4  // 完成4个字节内容的拷贝
	cmp r1, r2
	bne copy_loop

	
	#  清bss段，其实就是在链接地址处把bss段全部清零
clean_bss:
	ldr r0, =bss_start					
	ldr r1, =bss_end
	cmp r0, r1				//  如果r0等于r1，说明bss段为空，直接下去
	beq run_on_dram			//  清除bss完之后的地址
	mov r2, #0
clear_loop:
	str r2, [r0], #4		//  先将r2中的值放入r0所指向的内存地址（r0中的值作为内存地址），
	cmp r0, r1				//  然后r0 = r0 + 4
	bne clear_loop



run_on_dram:	
	#  长跳转到led_blink开始第二阶段
	ldr pc, =main				//  ldr指令实现长跳转
	bl main
	#  从这里之后就可以开始调用C程序了
	bl led					//  bl指令实现短跳转
	
#  汇编最后的这个死循环不能丢
	b .	
	b led
/*
  中断处理函数, 首先保存环境，调用真正的中断处理函数， 最后恢复环境
  1. 设置中断模式的栈顶
  2. 保存
  3.   调用C语言写的中断处理函数
*/
#define IRQ_STACK  0xd0037f80


IRQ_handle:
	# 设置栈顶
	ldr  sp, =IRQ_STACK
	# 保存lr, 因为流水线，PC的值是执行代码地址+8，而中断保存的是PC的值，
	# 所以要减4，才是正在执行的下一条地址
	sub lr, lr, #4
	# 保存寄存器
	stmfd sp!, {r0-r12,lr}
	
	# 调用中断处理函数
	bl irq_handler
	
	# 恢复现场，其实就是做中断返回，关键是将r0-r12,pc,cpsr一起恢复
	ldmfd sp!, {r0-r12,pc}^

	
	
	
