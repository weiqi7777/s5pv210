/*
   file name: led.s
   author:   lujun 
   description: implement light led
*/
.global _start
_start:
	//disable watch-dog
	#define WTCON 0xE2700000
	ldr r0, =WTCON
	ldr r1, =0
	str r1, [r0]
	
	//set stack-top position
	#define SVC_STACK_TOP  0xd0037d80
	ldr sp, =SVC_STACK_TOP

	//enable/disable icache
	mcr p15,0,r0,c1,c0,0  //read cp15 c1 to r0
	//bic r0, r0, #(1<<12)  //bit-12 set 0, disable icache
	orr r0, r0, #(1<<12)  //bit-12 set 1, enable icache
	mrc p15,0,r0,c1,c0,0  //write r0 to cp15 c1
	
	
	//recolate
	//adr指令加载_start当前运行地址
	adr r0, _start
	//ldr指令加载_start的链接地址
	ldr r1, =_start
	// bss段的起始地址
	ldr r2, =bss_start
	add r2, r2 ,#3
	cmp r0, r1      //比较_start的运行时地址和链接地址是否相等
	beq clean_bss   //相等说明，是在dram中运行，不需要拷贝代码
	
copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4  //完成4个字节内容的拷贝
	cmp r1, r2
	bne copy_loop
	
	// 清bss段，其实就是在链接地址处把bss段全部清零
clean_bss:
	ldr r0, =bss_start					
	ldr r1, =bss_end
	cmp r0, r1				// 如果r0等于r1，说明bss段为空，直接下去
	beq run_on_dram			// 清除bss完之后的地址
	mov r2, #0
clear_loop:
	str r2, [r0], #4		// 先将r2中的值放入r0所指向的内存地址（r0中的值作为内存地址），
	cmp r0, r1				// 然后r0 = r0 + 4
	bne clear_loop

run_on_dram:	
	// 长跳转到led_blink开始第二阶段
	ldr pc, =led				// ldr指令实现长跳转
	
	// 从这里之后就可以开始调用C程序了
	bl led					// bl指令实现短跳转
	
	// 汇编最后的这个死循环不能丢
	b .	
	b led
