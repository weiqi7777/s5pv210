#define SD_START_BLOCK	45
#define SD_BLOCK_CNT	(2048*2)
#define DDR_START_ADDR	0x23E00000



typedef unsigned int bool;

// 通道号：0，或者2
// 开始扇区号：45
// 读取扇区个数：32
// 读取后放入内存地址：0x23E00000
// with_init：0
typedef bool(*pCopySDMMC2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);

typedef void (*pBL2Type)(void);


// 从SD卡第45扇区开始，复制32个扇区内容到DDR的0x23E00000，然后跳转到23E00000去执行
void copy_bl2_2_ddr(void)
{
	// 第一步，读取SD卡扇区到DDR中
	pCopySDMMC2Mem p1 = (pCopySDMMC2Mem)(*(unsigned int *)0xD0037F98);
	//pCopySDMMC2Mem p1 = (pCopySDMMC2Mem)0xD0037F98);
	
	led2();
	delay();
	p1(2, SD_START_BLOCK, SD_BLOCK_CNT, (unsigned int *)DDR_START_ADDR, 0);		// 读取SD卡到DDR中
	led1();
	delay();
	// 第二步，跳转到DDR中的BL2去执行
	pBL2Type p2 = (pBL2Type)DDR_START_ADDR;
	p2();
	
	led3();
	delay();
}






















