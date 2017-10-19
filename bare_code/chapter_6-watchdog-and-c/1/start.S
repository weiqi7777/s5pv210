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

	b led
	
	
	
