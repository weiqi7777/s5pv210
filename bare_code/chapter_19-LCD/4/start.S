/*
   file name: start.s
   author:   lujun
   description: implement uart
*/
.global _start
.global IRQ_handle
_start:
	# disable watch-dog
	#define WTCON 0xE2700000
	ldr r0, =WTCON
	ldr r1, =0
	str r1, [r0]

	# set stack-top position
	#define SVC_STACK_TOP  0xd0037d80
	ldr sp, =SVC_STACK_TOP

	# enable/disable icache
	mrc p15,0,r0,c1,c0,0  // read cp15 c1 to r0
	# bic r0, r0, #(1<<12)  # bit-12 set 0, disable icache
	orr r0, r0, #(1<<12)  // bit-12 set 1, enable icache
	mcr p15,0,r0,c1,c0,0  // write r0 to cp15 c1

	# initial clock
	bl clock_init

	

enable_interrupt:
	mrs r0, cpsr
	bic r0, r0, #0x00000080
	msr cpsr, r0

	# recolate
	# adrָ������_start��ǰ���е�ַ
	adr r0, _start
	# ldrָ������_start�����ӵ�ַ
	ldr r1, =_start
	#  bss�ε���ʼ��ַ
	ldr r2, =bss_start
	//add r2, r2 ,#3
	cmp r0, r1      // �Ƚ�_start������ʱ��ַ�����ӵ�ַ�Ƿ�����
	beq clean_bss   // ����˵��������dram�����У�����Ҫ��������
	
	# initial ddr2
	bl sdram_asm_init

copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4  // ����4���ֽ����ݵĿ���
	cmp r1, r2
	bne copy_loop


	#  ��bss�Σ���ʵ���������ӵ�ַ����bss��ȫ������
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1				//  ����r0����r1��˵��bss��Ϊ�գ�ֱ����ȥ
	beq run_on_dram			//  ����bss��֮���ĵ�ַ
	mov r2, #0
clear_loop:
	str r2, [r0], #4		//  �Ƚ�r2�е�ֵ����r0��ָ�����ڴ���ַ��r0�е�ֵ��Ϊ�ڴ���ַ����
	cmp r0, r1				//  Ȼ��r0 = r0 + 4
	bne clear_loop



run_on_dram:
	ldr pc, =main				//  ldrָ��ʵ�ֳ���ת
