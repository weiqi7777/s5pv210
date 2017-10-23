/*
   file name: led.s
   author:   lujun 
   description: implement light led
*/
.global _start
_start:
	# disable watch-dog
	#define WTCON 0xE2700000
	ldr r0, =WTCON
	ldr r1, =0
	str r1, [r0]
	
	# set stack-top position
	#define SVC_STACK_TOP  0x28000000
	ldr sp, =SVC_STACK_TOP

	# enable/disable icache
	mrc p15,0,r0,c1,c0,0  // read cp15 c1 to r0
	# bic r0, r0, #(1<<12)  # bit-12 set 0, disable icache
	orr r0, r0, #(1<<12)  // bit-12 set 1, enable icache
	mcr p15,0,r0,c1,c0,0  // write r0 to cp15 c1
	
	# initial ddr2
	bl sdram_asm_init
	
	
	//copy SD 45-sector code to ddr , use 0x23e00000 to save the code
	bl copy_bl2_to_ddr
	ldr pc,=0x23e00000
		
#  汇编最后的这个死循环不能丢
	b .	
	
	
	
