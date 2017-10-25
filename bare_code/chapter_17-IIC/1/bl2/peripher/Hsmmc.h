#ifndef __HSMMC_H__
#define __HSMMC_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stdio.h"

#define HSMMC_NUM		3

#if (HSMMC_NUM == 0)
#define HSMMC_BASE	(0xEB000000)
#elif (HSMMC_NUM == 1)
#define HSMMC_BASE	(0xEB100000)
#elif (HSMMC_NUM == 2)
#define HSMMC_BASE	(0xEB200000)
#elif (HSMMC_NUM == 3)
#define HSMMC_BASE	(0xEB300000)
#else
#error "Configure HSMMC: HSMMC0 ~ HSMM3(0 ~ 3)"
#endif

#define MAX_BLOCK  65535



#define SDMASYSAD_OFFSET    	(0x00)
#define BLKSIZE_OFFSET			(0x04)
#define BLKCNT_OFFSET			(0x06)
#define ARGUMENT_OFFSET			(0x08)
#define TRNMOD_OFFSET			(0x0c)
#define CMDREG_OFFSET			(0x0e)
#define RSPREG0_OFFSET			(0x10)
#define RSPREG1_OFFSET			(0x14)
#define RSPREG2_OFFSET			(0x18)
#define RSPREG3_OFFSET			(0x1c)
#define BDATA_OFFSET			(0x20)
#define PRNSTS_OFFSET			(0x24)
#define HOSTCTL_OFFSET			(0x28)
#define PWRCON_OFFSET			(0x29)
#define BLKGAP_OFFSET			(0x2a)
#define WAKCON_OFFSET			(0x2b)
#define CLKCON_OFFSET			(0x2c)
#define TIMEOUTCON_OFFSET		(0x2e)
#define SWRST_OFFSET			(0x2f)
#define NORINTSTS_OFFSET		(0x30)
#define ERRINTSTS_OFFSET		(0x32)
#define NORINTSTSEN_OFFSET		(0x34)
#define ERRINTSTSEN_OFFSET		(0x36)
#define NORINTSIGEN_OFFSET		(0x38)
#define ERRINTSIGEN_OFFSET		(0x3a)
#define ACMD12ERRSTS_OFFSET		(0x3c)
#define CAPAREG_OFFSET			(0x40)
#define MAXCURR_OFFSET			(0x48)
#define CONTROL2_OFFSET			(0x80)
#define CONTROL3_OFFSET			(0x84)
#define CONTROL4_OFFSET			(0x8c)
#define HCVER_OFFSET			(0xfe)

#if (HSMMC_NUM == 0)
	#define GPG0CON_REG 	(*(volatile unsigned int *)0xe02001A0) 
	#define GPG0PUD_REG 	(*(volatile unsigned int *)0xe02001A8) 
	#define GPG0DRV_REG 	(*(volatile unsigned int *)0xe02001Ac) 
#elif (HSMMC_NUM == 1)
	#define GPG1CON_REG 	(*(volatile unsigned int *)0xe02001C0) 
	#define GPG1PUD_REG 	(*(volatile unsigned int *)0xe02001C8) 
	#define GPG1DRV_REG 	(*(volatile unsigned int *)0xe02001Cc) 
#elif (HSMMC_NUM == 2)
	#define GPG2CON_REG 	(*(volatile unsigned int *)0xe02001e0) 
	#define GPG2PUD_REG 	(*(volatile unsigned int *)0xe02001e8) 
	#define GPG2DRV_REG 	(*(volatile unsigned int *)0xe02001ec) 
#elif (HSMMC_NUM == 3)
	#define GPG3CON_REG 	(*(volatile unsigned int *)0xe0200200) 
	#define GPG3PUD_REG 	(*(volatile unsigned int *)0xe0200208) 
	#define GPG3DRV_REG 	(*(volatile unsigned int *)0xe020020C) 
#else
#error "Configure HSMMC: HSMMC0 ~ HSMM3(0 ~ 3)"
#endif


#define CLK_SRC4_REG 	(*(volatile unsigned int *)0xe0100210) 
#define CLK_DIV4_REG 	(*(volatile unsigned int *)0xe0100310) 




#define int8_t		char
#define int16_t		short
#define int32_t		long

typedef unsigned char		uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;
typedef	 unsigned char	 uint8_t;
typedef	 unsigned short  uint16_t;
typedef	 unsigned int	 uint32_t;


#define __REG(x)	(*(vu_long *)(x))
#define __REGl(x)	(*(vu_long *)(x))
#define __REGw(x)	(*(vu_short *)(x))
#define __REGb(x)	(*(vu_char *)(x))

#define Debug printf

// SD协议规定的命令码	
#define	CMD0	0
#define	CMD1	1
#define	CMD2	2	
#define	CMD3	3	
#define	CMD6	6
#define	CMD7	7
#define	CMD8	8
#define	CMD9	9
#define	CMD13	13
#define	CMD16	16
#define	CMD17	17
#define	CMD18	18
#define	CMD23	23	
#define	CMD24	24
#define	CMD25	25	
#define	CMD32	32	
#define	CMD33	33	
#define	CMD38	38	
#define	CMD41	41	
#define CMD51	51
#define	CMD55	55	

// 卡类型
#define UNUSABLE		0     //不能识别
#define SD_V1			1	  //SD卡1.0标准的卡
#define	SD_V2			2	  //SD卡2.0标准的卡
#define	SD_HC			3     //SD高存储量的卡
#define	MMC				4     //MMC卡
	
// 卡状态
#define CARD_IDLE		0			// 空闲态
#define CARD_READY		1			// 准备好
#define CARD_IDENT		2			// 卡认证状态
#define CARD_STBY		3			// 卡识别状态
#define CARD_TRAN		4           // 卡传输状态
#define CARD_DATA		5
#define CARD_RCV		6           // 卡数据读取状态
#define CARD_PRG		7			// 卡编程状态
#define CARD_DIS		8			// 断开连接

// 卡回复类型	
#define CMD_RESP_NONE	0			// 无回复
#define CMD_RESP_R1		1			// 标准R1响应，48-bits
#define CMD_RESP_R2		2           // CID,CSD寄存器，136-bits
#define CMD_RESP_R3		3           // OCR寄存器， 48-bits
#define CMD_RESP_R4		4			// 目前不支持
#define CMD_RESP_R5		5			// 目前不支持
#define CMD_RESP_R6		6			// RCA响应， 48-bits
#define CMD_RESP_R7		7
#define CMD_RESP_R1B	8			// 标准R1响应，但是要使用数据线
	
typedef struct {
	uint32_t RESERVED1;
	uint32_t RESERVED2 : 16;	
	uint32_t SD_BUS_WIDTHS : 4;
	uint32_t SD_SECURITY : 3;	
	uint32_t DATA_STAT_AFTER_ERASE : 1;
	uint32_t SD_SPEC : 4;	
	uint32_t SCR_STRUCTURE : 4;
} SD_SCR;	

int32_t Hsmmc_Init(void);
int32_t Hsmmc_GetCardState(void);
int32_t Hsmmc_GetSdState(uint8_t *pStatus);
int32_t Hsmmc_Get_SCR(SD_SCR *pSCR);
int32_t Hsmmc_Get_CSD(uint8_t *pCSD);
int32_t Hsmmc_EraseBlock(uint32_t StartBlock, uint32_t EndBlock);
int32_t Hsmmc_WriteBlock(uint8_t *pBuffer, uint32_t BlockAddr, uint32_t BlockNumber);
int32_t Hsmmc_ReadBlock(uint8_t *pBuffer, uint32_t BlockAddr, uint32_t BlockNumber);

int32_t Hsmmc_Switch(uint8_t Mode, int32_t Group, int32_t Function, uint8_t *pStatus);
int32_t Hsmmc_IssueCommand(uint8_t Cmd, uint32_t Arg, uint8_t Data, uint8_t Response);
int32_t Hsmmc_WaitForTransferDone(void);
int32_t Hsmmc_WaitForCommandDone(void);
int32_t Hsmmc_WaitForBufferWriteReady(void);
int32_t Hsmmc_WaitForBufferReadReady(void);
void Hsmmc_SetClock(uint32_t Clock);
void Hsmmc_ClockOn(uint8_t On);
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
int32_t Hsmmc_SetBusWidth(uint8_t Width);

#ifdef __cplusplus
}
#endif

#endif /*__HSMMC_H__*/
