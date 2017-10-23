

#define SD_CHANNEL      2
#define SD_START_BLOCK  45
#define SD_BLOCK_NUM    (2048*2)
#define DDR_START_ADD   0X23e00000


typedef unsigned int bool;
typedef bool(*pCopySDMMC2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);

typedef void (*pBL2Type)(void);

void led_config();

void copy_bl2_to_ddr()
{
	// read SD sector code to ddr
	pCopySDMMC2Mem p1 = (pCopySDMMC2Mem)(*(unsigned int *)0xD0037F98);
	p1(SD_CHANNEL,SD_START_BLOCK,SD_BLOCK_NUM,(unsigned int *)DDR_START_ADD,0);
	//pBL2Type p2 = (pBL2Type)DDR_START_ADD;
	//p2();
}