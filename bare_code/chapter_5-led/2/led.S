/*
   file name: led.s
   author:   lujun 
   description: implement light led
*/
.global _start
_start:
	//set GPJ0-3 GPJ0-4 GPJ0-5 is output
	#define GPJ0CON 0XE0200240
	ldr r0, =GPJ0CON
	ldr r1, [r0]
	ldr r4, =0xfff000
	ldr r5, =0x111000
	bic r1, r1, r4
	orr r1, r1, r5
	str r1, [r0]	

	//set GPD0-1 is output
	#define GPD0CON 0xE02000A0
	ldr r0, =GPD0CON
	ldr r1, [r0]
	bic r1, r1, #(0xf<<4)
	orr r1, r1, #(0x1<<4)
	str r1,[r0]
	mov r0, #0x0
	
_loop:	
	mov r0, #0x7
	bl _set_GPJ_value
	mov r0, #0x1
	bl _set_GPD_value
	bl _delay
	mov r0, #0x0
	bl _set_GPJ_value
	mov r0, #0x0
	bl _set_GPD_value
	bl _delay
	b _loop


_set_GPJ_value:
	//set GPJ-3,4,5 output value , r0 is output value  (5,4,3)
	#define GPJ0DAT 0xE0200244
	ldr r1, =GPJ0DAT
	ldr r2, [r1]
	bic r2, r2, #0x7<<3
	lsl r0, r0, #3
	orr r2, r2, r0
	str r2, [r1]
	mov pc, lr

_set_GPD_value:
	//set GPD0-1 output value
	#define GPD0DAT 0xE02000A4
	ldr r1, =GPD0DAT
	ldr r2, [r1]
	bic r2, r2, #0x1<<1
	lsl r0, r0, #1
	orr r2, r2, r0
	str r2, [r1]
	mov pc,lr

_delay:
	ldr r4,=0xffffff
	mov r5,#0
_delay_loop:
	sub r4, r4, #1
	cmp r4, r5
	bne _delay_loop
	mov pc, lr