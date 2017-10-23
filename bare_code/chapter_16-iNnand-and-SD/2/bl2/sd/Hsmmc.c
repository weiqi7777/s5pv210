
#include "Hsmmc.h"


static uint8_t CardType; // 卡类型
static uint32_t RCA; // 卡相对地址

void Delay_us(uint16_t us)
{
	int i;
	while(us--)
		for(i=0; i<1000;i++);
}

void Delay_ms(uint16_t ms)
{
	while(ms--)
		Delay_us(1000);
}

 void Hsmmc_ClockOn(uint8_t On)
{
	uint32_t Timeout;
	if (On) {
		__REGw(HSMMC_BASE+CLKCON_OFFSET) |= (1<<2); // sd时钟使能
		Timeout = 1000; // Wait max 10 ms
		while (!(__REGw(HSMMC_BASE+CLKCON_OFFSET) & (1<<3))) {
			// 等待SD输出时钟稳定
			if (Timeout == 0) {
				return;
			}
			Timeout--;
			Delay_us(10);
		}
	} else {
		__REGw(HSMMC_BASE+CLKCON_OFFSET) &= ~(1<<2); // sd时钟禁止
	}
}

 void Hsmmc_SetClock(uint32_t Clock)
{
	uint32_t Temp;
	uint32_t Timeout;
	uint32_t i;
	Hsmmc_ClockOn(0); // 关闭时钟
	Temp = __REG(HSMMC_BASE+CONTROL2_OFFSET);
	// Set SCLK_MMC(48M) from SYSCON as a clock source
	Temp = (Temp & (~(3<<4))) | (2<<4);
	Temp |= (1u<<31) | (1u<<30) | (1<<8);
	if (Clock <= 500000) {
		Temp &= ~((1<<14) | (1<<15));
		__REG(HSMMC_BASE+CONTROL3_OFFSET) = 0;
	} else {
		Temp |= ((1<<14) | (1<<15));
		__REG(HSMMC_BASE+CONTROL3_OFFSET) = (1u<<31) | (1<<23);
	}
	__REG(HSMMC_BASE+CONTROL2_OFFSET) = Temp;

	for (i=0; i<=8; i++) {
		if (Clock >= (48000000/(1<<i))) {
			break;
		}
	}
	Temp = ((1<<i) / 2) << 8; // clock div
	Temp |= (1<<0); // Internal Clock Enable
	__REGw(HSMMC_BASE+CLKCON_OFFSET) = Temp;
	Timeout = 1000; // Wait max 10 ms
	while (!(__REGw(HSMMC_BASE+CLKCON_OFFSET) & (1<<1))) {
		// 等待内部时钟振荡稳定
		if (Timeout == 0) {
			return;
		}
		Timeout--;
		Delay_us(10);
	}

	Hsmmc_ClockOn(1); // 使能时钟
}

 int32_t Hsmmc_WaitForBufferReadReady(void)
{
	int32_t ErrorState;
	while (1) {
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<15)) { // 出现错误
			break;
		}
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<5)) { // 读缓存准备好
			__REGw(HSMMC_BASE+NORINTSTS_OFFSET) |= (1<<5); // 清除准备好标志
			return 0;
		}
	}

	ErrorState = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET) & 0x1ff; // 可能通信错误,CRC检验错误,超时等
	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志
	Debug("Read buffer error, NORINTSTS: %04x\r\n", ErrorState);
	return ErrorState;
}

 int32_t Hsmmc_WaitForBufferWriteReady(void)
{
	int32_t ErrorState;
	while (1) {
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<15)) { // 出现错误
			break;
		}
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<4)) { // 写缓存准备好
			__REGw(HSMMC_BASE+NORINTSTS_OFFSET) |= (1<<4); // 清除准备好标志
			return 0;
		}
	}

	ErrorState = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET) & 0x1ff; // 可能通信错误,CRC检验错误,超时等
	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志
	Debug("Write buffer error, NORINTSTS: %04x\r\n", ErrorState);
	return ErrorState;
}

 int32_t Hsmmc_WaitForCommandDone(void)
{
	uint32_t i;
	int32_t ErrorState;
	// 等待命令发送完成
	for (i=0; i<0x20000000; i++) {
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<15)) { // 出现错误
			break;
		}
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<0)) {
			do {
				__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = (1<<0); // 清除命令完成位
			} while (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<0));
			return 0; // 命令发送成功
		}
	}

	ErrorState = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET) & 0x1ff; // 可能通信错误,CRC检验错误,超时等
	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志
	do {
		__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = (1<<0); // 清除命令完成位
	} while (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<0));

	Debug("Command error, ERRINTSTS = 0x%x ", ErrorState);
	return ErrorState; // 命令发送出错
}

 int32_t Hsmmc_WaitForTransferDone(void)
{
	int32_t ErrorState;
	uint32_t i;
	// 等待数据传输完成
	for (i=0; i<0x20000000; i++) {
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<15)) { // 出现错误
			break;
		}
		if (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<1)) { // 数据传输完
			do {
				__REGw(HSMMC_BASE+NORINTSTS_OFFSET) |= (1<<1); // 清除传输完成位
			} while (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<1));
			return 0;
		}
	}

	ErrorState = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET) & 0x1ff; // 可能通信错误,CRC检验错误,超时等
	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志
	Debug("Transfer error, rHM1_ERRINTSTS = 0x%04x\r\n", ErrorState);
	do {
		__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = (1<<1); // 出错后清除数据完成位
	} while (__REGw(HSMMC_BASE+NORINTSTS_OFFSET) & (1<<1));

	return ErrorState; // 数据传输出错
}

 int32_t Hsmmc_IssueCommand(uint8_t Cmd, uint32_t Arg, uint8_t Data, uint8_t Response)
{
	uint32_t i;
	uint32_t Value;
	uint32_t ErrorState;
	// 检查CMD线是否准备好发送命令
	for (i=0; i<0x1000000; i++) {
		if (!(__REG(HSMMC_BASE+PRNSTS_OFFSET) & (1<<0))) {
			break;
		}
	}
	if (i == 0x1000000) {
		Debug("CMD line time out, PRNSTS: %04x\r\n", __REG(HSMMC_BASE+PRNSTS_OFFSET));
		return -1; // 命令超时
	}

	// 检查DAT线是否准备好
	if (Response == CMD_RESP_R1B) { // R1b回复通过DAT0反馈忙信号
		for (i=0; i<0x10000000; i++) {
			if (!(__REG(HSMMC_BASE+PRNSTS_OFFSET) & (1<<1))) {
				break;
			}
		}
		if (i == 0x10000000) {
			Debug("Data line time out, PRNSTS: %04x\r\n", __REG(HSMMC_BASE+PRNSTS_OFFSET));
			return -2;
		}
	}

	__REG(HSMMC_BASE+ARGUMENT_OFFSET) = Arg; // 写入命令参数

	Value = (Cmd << 8); // command index
	// CMD12可终止传输
	if (Cmd == 12) {
		Value |= (0x3 << 6); // command type
	}
	if (Data) {
		Value |= (1 << 5); // 需使用DAT线作为传输等
	}

	switch (Response) {
	case CMD_RESP_NONE:
		Value |= (0<<4) | (0<<3) | 0x0; // 没有回复,不检查命令及CRC
		break;
	case CMD_RESP_R1:
	case CMD_RESP_R5:
	case CMD_RESP_R6:
	case CMD_RESP_R7:
		Value |= (1<<4) | (1<<3) | 0x2; // 检查回复中的命令,CRC
		break;
	case CMD_RESP_R2:
		Value |= (0<<4) | (1<<3) | 0x1; // 回复长度为136位,包含CRC
		break;
	case CMD_RESP_R3:
	case CMD_RESP_R4:
		Value |= (0<<4) | (0<<3) | 0x2; // 回复长度48位,不包含命令及CRC
		break;
	case CMD_RESP_R1B:
		Value |= (1<<4) | (1<<3) | 0x3; // 回复带忙信号,会占用Data[0]线
		break;
	default:
		break;
	}

	__REGw(HSMMC_BASE+CMDREG_OFFSET) = Value;

	ErrorState = Hsmmc_WaitForCommandDone();
	if (ErrorState) {
		Debug("Command = %d\r\n", Cmd);
	}
	return ErrorState; // 命令发送出错
}

int32_t Hsmmc_Switch(uint8_t Mode, int32_t Group, int32_t Function, uint8_t *pStatus)
{
	int32_t ErrorState;
	int32_t Temp;
	uint32_t i;

	__REGw(HSMMC_BASE+BLKSIZE_OFFSET) = (7<<12) | (64<<0); // 最大DMA缓存大小,block为512位64字节
	__REGw(HSMMC_BASE+BLKCNT_OFFSET) = 1; // 写入这次读1 block的sd状态数据
	Temp = (Mode << 31U) | 0xffffff;
	Temp &= ~(0xf<<(Group * 4));
	Temp |= Function << (Group * 4);
	__REG(HSMMC_BASE+ARGUMENT_OFFSET) = Temp; // 写入命令参数

	// DMA禁能,读单块
	__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (0<<5) | (1<<4) | (0<<2) | (1<<1) | (0<<0);
	// 设置命令寄存器,SWITCH_FUNC CMD6,R1回复
	__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD6<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
	ErrorState = Hsmmc_WaitForCommandDone();
	if (ErrorState) {
		Debug("CMD6 error\r\n");
		return ErrorState;
	}

	ErrorState = Hsmmc_WaitForBufferReadReady();
	if (ErrorState) {
		return ErrorState;
	}

	pStatus += 64 - 1;
	for (i=0; i<64/4; i++) {
		Temp = __REG(HSMMC_BASE+BDATA_OFFSET);
		*pStatus-- = (uint8_t)Temp;
		*pStatus-- = (uint8_t)(Temp>>8);
		*pStatus-- = (uint8_t)(Temp>>16);
		*pStatus-- = (uint8_t)(Temp>>24);
	}

	ErrorState = Hsmmc_WaitForTransferDone();
	if (ErrorState) {
		Debug("Get sd status error\r\n");
		return ErrorState;
	}
	return 0;
}

// 512位的sd卡扩展状态位
int32_t Hsmmc_GetSdState(uint8_t *pStatus)
{
	int32_t ErrorState;
	uint32_t Temp;
	uint32_t i;
	if (CardType == SD_HC || CardType == SD_V2 || CardType == SD_V1) {
		if (Hsmmc_GetCardState() != CARD_TRAN) { // 必需在transfer status
			return -1; // 卡状态错误
		}
		Hsmmc_IssueCommand(CMD55, RCA<<16, 0, CMD_RESP_R1);

		__REGw(HSMMC_BASE+BLKSIZE_OFFSET) = (7<<12) | (64<<0); // 最大DMA缓存大小,block为512位64字节
		__REGw(HSMMC_BASE+BLKCNT_OFFSET) = 1; // 写入这次读1 block的sd状态数据
		__REG(HSMMC_BASE+ARGUMENT_OFFSET) = 0; // 写入命令参数

		// DMA禁能,读单块
		__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (0<<5) | (1<<4) | (0<<2) | (1<<1) | (0<<0);
		// 设置命令寄存器,读状态命令CMD13,R1回复
		__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD13<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
		ErrorState = Hsmmc_WaitForCommandDone();
		if (ErrorState) {
			Debug("CMD13 error\r\n");
			return ErrorState;
		}

		ErrorState = Hsmmc_WaitForBufferReadReady();
		if (ErrorState) {
			return ErrorState;
		}

		pStatus += 64 - 1;
		for (i=0; i<64/4; i++) {
			Temp = __REG(HSMMC_BASE+BDATA_OFFSET);
			*pStatus-- = (uint8_t)Temp;
			*pStatus-- = (uint8_t)(Temp>>8);
			*pStatus-- = (uint8_t)(Temp>>16);
			*pStatus-- = (uint8_t)(Temp>>24);
		}

		ErrorState = Hsmmc_WaitForTransferDone();
		if (ErrorState) {
			Debug("Get sd status error\r\n");
			return ErrorState;
		}

		return 0;
	}
	return -1; // 非sd卡
}

// Reads the SD Configuration Register (SCR).
int32_t Hsmmc_Get_SCR(SD_SCR *pSCR)
{
	uint8_t *pBuffer;
	int32_t ErrorState;
	uint32_t Temp;
	uint32_t i;
	Hsmmc_IssueCommand(CMD55, RCA<<16, 0, CMD_RESP_R1);
	__REGw(HSMMC_BASE+BLKSIZE_OFFSET) = (7<<12) | (8<<0); // 最大DMA缓存大小,block为64位8字节
	__REGw(HSMMC_BASE+BLKCNT_OFFSET) = 1; // 写入这次读1 block的sd状态数据
	__REG(HSMMC_BASE+ARGUMENT_OFFSET) = 0; // 写入命令参数

	// DMA禁能,读单块
	__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (0<<5) | (1<<4) | (0<<2) | (1<<1) | (0<<0);
	// 设置命令寄存器,read SD Configuration CMD51,R1回复
	__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD51<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
	ErrorState = Hsmmc_WaitForCommandDone();
	if (ErrorState) {
		Debug("CMD51 error\r\n");
		return ErrorState;
	}

	ErrorState = Hsmmc_WaitForBufferReadReady();
	if (ErrorState) {
		return ErrorState;
	}

	// Wide width data (SD Memory Register)
	pBuffer = (uint8_t *)pSCR + sizeof(SD_SCR) - 1;
	for (i=0; i<8/4; i++) {
		Temp = __REG(HSMMC_BASE+BDATA_OFFSET);
		*pBuffer-- = (uint8_t)Temp;
		*pBuffer-- = (uint8_t)(Temp>>8);
		*pBuffer-- = (uint8_t)(Temp>>16);
		*pBuffer-- = (uint8_t)(Temp>>24);
	}

	ErrorState = Hsmmc_WaitForTransferDone();
	if (ErrorState) {
		Debug("Get SCR register error\r\n");
		return ErrorState;
	}

	return 0;
}

// Asks the selected card to send its cardspecific data
int32_t Hsmmc_Get_CSD(uint8_t *pCSD)
{
	uint32_t i;
	uint32_t Response[4];
	int32_t State = -1;

	if (CardType != SD_HC && CardType != SD_V1 && CardType != SD_V2) {
		return State; // 未识别的卡
	}
	// 取消卡选择,任何卡均不回复,已选择的卡通过RCA=0取消选择,
	// 卡回到stand-by状态
	Hsmmc_IssueCommand(CMD7, 0, 0, CMD_RESP_NONE);
	for (i=0; i<1000; i++) {
		if (Hsmmc_GetCardState() == CARD_STBY) { // CMD9命令需在standy-by status
			break; // 状态正确
		}
		Delay_us(100);
	}
	if (i == 1000) {
		return State; // 状态错误
	}

	// 请求已标记卡发送卡特定数据(CSD),获得卡信息
	if (!Hsmmc_IssueCommand(CMD9, RCA<<16, 0, CMD_RESP_R2)) {
		pCSD++; // 跳过第一字节,CSD中[127:8]位对位寄存器中的[119:0]
		Response[0] = __REG(HSMMC_BASE+RSPREG0_OFFSET);
		Response[1] = __REG(HSMMC_BASE+RSPREG1_OFFSET);
		Response[2] = __REG(HSMMC_BASE+RSPREG2_OFFSET);
		Response[3] = __REG(HSMMC_BASE+RSPREG3_OFFSET);
		for (i=0; i<15; i++) { // 拷贝回复寄存器中的[119:0]到pCSD中
			*pCSD++ = ((uint8_t *)Response)[i];
		}
		State = 0; // CSD获取成功
	}

	Hsmmc_IssueCommand(CMD7, RCA<<16, 0, CMD_RESP_R1); // 选择卡,卡回到transfer状态
	return State;
}

// R1回复中包含了32位的card state,卡识别后,可在任一状态通过CMD13获得卡状态
int32_t Hsmmc_GetCardState(void)
{
	if (Hsmmc_IssueCommand(CMD13, RCA<<16, 0, CMD_RESP_R1)) {
		return -1; // 卡出错
	} else {
		return ((__REG(HSMMC_BASE+RSPREG0_OFFSET)>>9) & 0xf); // 返回R1回复中的[12:9]卡状态
	}
}

 int32_t Hsmmc_SetBusWidth(uint8_t Width)
{
	int32_t State;
	if ((Width != 1) || (Width != 4)) {
		return -1;
	}
	State = -1; // 设置初始为未成功
	__REGw(HSMMC_BASE+NORINTSTSEN_OFFSET) &= ~(1<<8); // 关闭卡中断
	Hsmmc_IssueCommand(CMD55, RCA<<16, 0, CMD_RESP_R1);
	if (Width == 1) {
		if (!Hsmmc_IssueCommand(CMD6, 0, 0, CMD_RESP_R1)) { // 1位宽
			__REGb(HSMMC_BASE+HOSTCTL_OFFSET) &= ~(1<<1);
			State = 0; // 命令成功
		}
	} else {
		if (!Hsmmc_IssueCommand(CMD6, 2, 0, CMD_RESP_R1)) { // 4位宽
			__REGb(HSMMC_BASE+HOSTCTL_OFFSET) |= (1<<1);
			State = 0; // 命令成功
		}
	}
	__REGw(HSMMC_BASE+NORINTSTSEN_OFFSET) |= (1<<8); // 打开卡中断
	return State; // 返回0为成功
}

int32_t Hsmmc_EraseBlock(uint32_t StartBlock, uint32_t EndBlock)
{
	uint32_t i;

	if (CardType == SD_V1 || CardType == SD_V2) {
		StartBlock <<= 9; // 标准卡为字节地址
		EndBlock <<= 9;
	} else if (CardType != SD_HC) {
		return -1; // 未识别的卡
	}
	Hsmmc_IssueCommand(CMD32, StartBlock, 0, CMD_RESP_R1);
	Hsmmc_IssueCommand(CMD33, EndBlock, 0, CMD_RESP_R1);
	if (!Hsmmc_IssueCommand(CMD38, 0, 0, CMD_RESP_R1B)) {
		for (i=0; i<0x10000; i++) {
			if (Hsmmc_GetCardState() == CARD_TRAN) { // 擦除完成后返回到transfer状态
				Debug("Erasing complete!\r\n");
				return 0; // 擦除成功
			}
			Delay_us(1000);
		}
	}

	Debug("Erase block failed\r\n");
	return -1; // 擦除失败
}

int32_t Hsmmc_ReadBlock(uint8_t *pBuffer, uint32_t BlockAddr, uint32_t BlockNumber)
{
	uint32_t Address = 0;
	uint32_t ReadBlock;
	uint32_t i;
	uint32_t j;
	int32_t ErrorState;
	uint32_t Temp;

	if (pBuffer == 0 || BlockNumber == 0) {
		return -1;
	}

	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志

	while (BlockNumber > 0) {
		if (BlockNumber <= MAX_BLOCK) {
			ReadBlock = BlockNumber; // 读取的块数小于65536 Block
			BlockNumber = 0; // 剩余读取块数为0
		} else {
			ReadBlock = MAX_BLOCK; // 读取的块数大于65536 Block,分多次读
			BlockNumber -= ReadBlock;
		}
		// 根据sd主机控制器标准,按顺序写入主机控制器相应的寄存器
		__REGw(HSMMC_BASE+BLKSIZE_OFFSET) = (7<<12) | (512<<0); // 最大DMA缓存大小,block为512字节
		__REGw(HSMMC_BASE+BLKCNT_OFFSET) = ReadBlock; // 写入这次读block数目

		if (CardType == SD_HC) {
			Address = BlockAddr; // SDHC卡写入地址为block地址
		} else if (CardType == SD_V1 || CardType == SD_V2) {
			Address = BlockAddr << 9; // 标准卡写入地址为字节地址
		}
		BlockAddr += ReadBlock; // 下一次读块的地址
		__REG(HSMMC_BASE+ARGUMENT_OFFSET) = Address; // 写入命令参数

		if (ReadBlock == 1) {
			// 设置传输模式,DMA禁能,读单块
			__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (0<<5) | (1<<4) | (0<<2) | (1<<1) | (0<<0);
			// 设置命令寄存器,单块读CMD17,R1回复
			__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD17<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
		} else {
			// 设置传输模式,DMA禁能,读多块
			__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (1<<5) | (1<<4) | (1<<2) | (1<<1) | (0<<0);
			// 设置命令寄存器,多块读CMD18,R1回复
			__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD18<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
		}
		ErrorState = Hsmmc_WaitForCommandDone();
		if (ErrorState) {
			Debug("Read Command error\r\n");
			return ErrorState;
		}
		for (i=0; i<ReadBlock; i++) {
			ErrorState = Hsmmc_WaitForBufferReadReady();
			if (ErrorState) {
				return ErrorState;
			}
			if (((uint32_t)pBuffer & 0x3) == 0) {
				for (j=0; j<512/4; j++) {
					*(uint32_t *)pBuffer = __REG(HSMMC_BASE+BDATA_OFFSET);
					pBuffer += 4;
				}
			} else {
				for (j=0; j<512/4; j++) {
					Temp = __REG(HSMMC_BASE+BDATA_OFFSET);
					*pBuffer++ = (uint8_t)Temp;
					*pBuffer++ = (uint8_t)(Temp>>8);
					*pBuffer++ = (uint8_t)(Temp>>16);
					*pBuffer++ = (uint8_t)(Temp>>24);
				}
			}
		}

		ErrorState = Hsmmc_WaitForTransferDone();
		if (ErrorState) {
			Debug("Read block error\r\n");
			return ErrorState;
		}
	}
	return 0; // 所有块读完
}

int32_t Hsmmc_WriteBlock(uint8_t *pBuffer, uint32_t BlockAddr, uint32_t BlockNumber)
{
	uint32_t Address = 0;
	uint32_t WriteBlock;
	uint32_t i;
	uint32_t j;
	int32_t ErrorState;

	if (pBuffer == 0 || BlockNumber == 0) {
		return -1; // 参数错误
	}

	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET); // 清除中断标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET); // 清除错误中断标志

	while (BlockNumber > 0) {
		if (BlockNumber <= MAX_BLOCK) {
			WriteBlock = BlockNumber;// 写入的块数小于65536 Block
			BlockNumber = 0; // 剩余写入块数为0
		} else {
			WriteBlock = MAX_BLOCK; // 写入的块数大于65536 Block,分多次写
			BlockNumber -= WriteBlock;
		}
		if (WriteBlock > 1) { // 多块写,发送ACMD23先设置预擦除块数
			Hsmmc_IssueCommand(CMD55, RCA<<16, 0, CMD_RESP_R1);
			Hsmmc_IssueCommand(CMD23, WriteBlock, 0, CMD_RESP_R1);
		}

		// 根据sd主机控制器标准,按顺序写入主机控制器相应的寄存器
		__REGw(HSMMC_BASE+BLKSIZE_OFFSET) = (7<<12) | (512<<0); // 最大DMA缓存大小,block为512字节
		__REGw(HSMMC_BASE+BLKCNT_OFFSET) = WriteBlock; // 写入block数目

		if (CardType == SD_HC) {
			Address = BlockAddr; // SDHC卡写入地址为block地址
		} else if (CardType == SD_V1 || CardType == SD_V2) {
			Address = BlockAddr << 9; // 标准卡写入地址为字节地址
		}
		BlockAddr += WriteBlock; // 下一次写地址
		__REG(HSMMC_BASE+ARGUMENT_OFFSET) = Address; // 写入命令参数

		if (WriteBlock == 1) {
			// 设置传输模式,DMA禁能写单块
			__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (0<<5) | (0<<4) | (0<<2) | (1<<1) | (0<<0);
			// 设置命令寄存器,单块写CMD24,R1回复
			__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD24<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
		} else {
			// 设置传输模式,DMA禁能写多块
			__REGw(HSMMC_BASE+TRNMOD_OFFSET) = (1<<5) | (0<<4) | (1<<2) | (1<<1) | (0<<0);
			// 设置命令寄存器,多块写CMD25,R1回复
			__REGw(HSMMC_BASE+CMDREG_OFFSET) = (CMD25<<8)|(1<<5)|(1<<4)|(1<<3)|0x2;
		}
		ErrorState = Hsmmc_WaitForCommandDone();
		if (ErrorState) {
			Debug("Write Command error\r\n");
			return ErrorState;
		}

		for (i=0; i<WriteBlock; i++) {
			ErrorState = Hsmmc_WaitForBufferWriteReady();
			if (ErrorState) {
				return ErrorState;
			}
			if (((uint32_t)pBuffer & 0x3) == 0) {
				for (j=0; j<512/4; j++) {
					__REG(HSMMC_BASE+BDATA_OFFSET) = *(uint32_t *)pBuffer;
					pBuffer += 4;
				}
			} else {
				for (j=0; j<512/4; j++) {
					__REG(HSMMC_BASE+BDATA_OFFSET) = pBuffer[0] + ((uint32_t)pBuffer[1]<<8) +
					((uint32_t)pBuffer[2]<<16) + ((uint32_t)pBuffer[3]<<24);
					pBuffer += 4;
				}
			}
		}

		ErrorState = Hsmmc_WaitForTransferDone();
		if (ErrorState) {
			Debug("Write block error\r\n");
			return ErrorState;
		}
		for (i=0; i<0x10000000; i++) {
			if (Hsmmc_GetCardState() == CARD_TRAN) { // 需在transfer status
				break; // 状态正确
			}
		}
		if (i == 0x10000000) {
			return -3; // 状态错误或Programming超时
		}
	}
	return 0; // 写完所有数据
}

int32_t Hsmmc_Init(void)
{
	int32_t Timeout;
	uint32_t Capacity;
	uint32_t i;
	uint32_t OCR;
	uint32_t Temp;
	uint8_t SwitchStatus[64];
	SD_SCR SCR;
	uint8_t CSD[16];
	uint32_t c_size, c_size_multi, read_bl_len;

	// 设置HSMMC的接口引脚配置
#if (HSMMC_NUM == 0)
	// channel 0,GPG0[0:6] = CLK, CMD, CDn, DAT[0:3]
	GPG0CON_REG = 0x2222222;
	// pull up enable
	GPG0PUD_REG = 0x2aaa;
	GPG0DRV_REG = 0x3fff;
	// channel 0 clock src = SCLKEPLL = 96M
	CLK_SRC4_REG = (CLK_SRC4_REG & (~(0xf<<0))) | (0x7<<0);
	// channel 0 clock = SCLKEPLL/2 = 48M
	CLK_DIV4_REG = (CLK_DIV4_REG & (~(0xf<<0))) | (0x1<<0);

#elif (HSMMC_NUM == 1)
	// channel 1,GPG1[0:6] = CLK, CMD, CDn, DAT[0:3]
	GPG1CON_REG = 0x2222222;
	// pull up enable
	GPG1PUD_REG = 0x2aaa;
	GPG1DRV_REG = 0x3fff;
	// channel 1 clock src = SCLKEPLL = 96M
	CLK_SRC4_REG = (CLK_SRC4_REG & (~(0xf<<4))) | (0x7<<4);
	// channel 1 clock = SCLKEPLL/2 = 48M
	CLK_DIV4_REG = (CLK_DIV4_REG & (~(0xf<<4))) | (0x1<<4);

#elif (HSMMC_NUM == 2)
	// channel 2,GPG2[0:6] = CLK, CMD, CDn, DAT[0:3]
	GPG2CON_REG = 0x2222222;
	// pull up enable
	GPG2PUD_REG = 0x2aaa;
	GPG2DRV_REG = 0x3fff;
	// channel 2 clock src = SCLKEPLL = 96M
	CLK_SRC4_REG = (CLK_SRC4_REG & (~(0xf<<8))) | (0x7<<8);
	// channel 2 clock = SCLKEPLL/2 = 48M
	CLK_DIV4_REG = (CLK_DIV4_REG & (~(0xf<<8))) | (0x1<<8);

#elif (HSMMC_NUM == 3)
	// channel 3,GPG3[0:6] = CLK, CMD, CDn, DAT[0:3]
	GPG3CON_REG = 0x2222222;
	// pull up enable
	GPG3PUD_REG = 0x2aaa;
	GPG3DRV_REG = 0x3fff;
	// channel 3 clock src = SCLKEPLL = 96M
	CLK_SRC4_REG = (CLK_SRC4_REG & (~(0xf<<12))) | (0x7<<12);
	// channel 3 clock = SCLKEPLL/2 = 48M
	CLK_DIV4_REG = (CLK_DIV4_REG & (~(0xf<<12))) | (0x1<<12);

#endif
	// software reset for all 复位主机SoC控制器，而不是复位SD卡
	__REGb(HSMMC_BASE+SWRST_OFFSET) = 0x1;
	Timeout = 1000; // Wait max 10 ms
	while (__REGb(HSMMC_BASE+SWRST_OFFSET) & (1<<0)) {
		if (Timeout == 0) {
			return -1; // reset timeout
		}
		Timeout--;
		Delay_us(10);
	}
	// 上面设置的是SoC的SD控制器的时钟，现在设置的是SD卡的时钟
	Hsmmc_SetClock(400000); // 400k
	__REGb(HSMMC_BASE+TIMEOUTCON_OFFSET) = 0xe; // 最大超时时间
	__REGb(HSMMC_BASE+HOSTCTL_OFFSET) &= ~(1<<2); // 正常速度模式
	
	// 清除正常中断状态标志
	__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = __REGw(HSMMC_BASE+NORINTSTS_OFFSET);
	// 清除错误中断状态标志
	__REGw(HSMMC_BASE+ERRINTSTS_OFFSET) = __REGw(HSMMC_BASE+ERRINTSTS_OFFSET);
	
	__REGw(HSMMC_BASE+NORINTSTSEN_OFFSET) = 0x7fff; // [14:0]中断状态使能
	__REGw(HSMMC_BASE+ERRINTSTSEN_OFFSET) = 0x3ff; // [9:0]错误中断状态使能
	__REGw(HSMMC_BASE+NORINTSIGEN_OFFSET) = 0x7fff; // [14:0]中断信号使能
	__REGw(HSMMC_BASE+ERRINTSIGEN_OFFSET) = 0x3ff; // [9:0]错误中断信号使能

	// 从这里开始和SD卡通信，通信其实就是发命令然后收响应
	Hsmmc_IssueCommand(CMD0, 0, 0, CMD_RESP_NONE); // 复位所有卡到空闲状态

	CardType = UNUSABLE; // 卡类型初始化不可用
	if (Hsmmc_IssueCommand(CMD8, 0x1aa, 0, CMD_RESP_R7)) { // 没有回复,MMC/sd v1.x/not card
			for (i=0; i<100; i++) {
				// CMD55 + CMD41 = ACMD41
				Hsmmc_IssueCommand(CMD55, 0, 0, CMD_RESP_R1);
				if (!Hsmmc_IssueCommand(CMD41, 0, 0, CMD_RESP_R3)) { // CMD41有回复说明为sd卡
					OCR = __REG(HSMMC_BASE+RSPREG0_OFFSET); // 获得回复的OCR(操作条件寄存器)值
					if (OCR & 0x80000000) { // 卡上电是否完成上电流程,是否busy
						CardType = SD_V1; // 正确识别出sd v1.x卡
						Debug("SD card version 1.x is detected\r\n");
						break;
					}
				} else {
					// MMC卡识别
					Debug("MMC card is not supported\r\n");
					return -1;
				}
				Delay_us(1000);
			}
	} else { // sd v2.0
		Temp = __REG(HSMMC_BASE+RSPREG0_OFFSET);
		if (((Temp&0xff) == 0xaa) && (((Temp>>8)&0xf) == 0x1)) { // 判断卡是否支持2.7~3.3v电压
			OCR = 0;
			for (i=0; i<100; i++) {
				OCR |= (1<<30);
				Hsmmc_IssueCommand(CMD55, 0, 0, CMD_RESP_R1);
				Hsmmc_IssueCommand(CMD41, OCR, 0, CMD_RESP_R3); // reday态
				OCR = __REG(HSMMC_BASE+RSPREG0_OFFSET);
				if (OCR & 0x80000000) { // 卡上电是否完成上电流程,是否busy
					if (OCR & (1<<30)) { // 判断卡为标准卡还是高容量卡
						CardType = SD_HC; // 高容量卡
						Debug("SDHC card is detected\r\n");
					} else {
						CardType = SD_V2; // 标准卡
						Debug("SD version 2.0 standard card is detected\r\n");
					}
					break;
				}
				Delay_us(1000);
			}
		}
	}
	if (CardType == SD_HC || CardType == SD_V1 || CardType == SD_V2) {
		Hsmmc_IssueCommand(CMD2, 0, 0, CMD_RESP_R2); // 请求卡发送CID(卡ID寄存器)号,进入ident
		Hsmmc_IssueCommand(CMD3, 0, 0, CMD_RESP_R6); // 请求卡发布新的RCA(卡相对地址),Stand-by状态
		RCA = (__REG(HSMMC_BASE+RSPREG0_OFFSET) >> 16) & 0xffff; // 从卡回复中得到卡相对地址
		
		Hsmmc_IssueCommand(CMD7, RCA<<16, 0, CMD_RESP_R1); // 选择已标记的卡,transfer状态

		Hsmmc_Get_SCR(&SCR);
		if (SCR.SD_SPEC == 0) { // sd 1.0 - sd 1.01
			// Version 1.0 doesn't support switching
			Hsmmc_SetClock(24000000); // 设置SDCLK = 48M/2 = 24M
		} else { // sd 1.10 / sd 2.0
			Temp = 0;
			for (i=0; i<4; i++) {
				if (Hsmmc_Switch(0, 0, 1, SwitchStatus) == 0) { // switch check
					if (!(SwitchStatus[34] & (1<<1))) { // Group 1, function 1 high-speed bit 273
						// The high-speed function is ready
						if (SwitchStatus[50] & (1<<1)) { // Group, function 1 high-speed support bit 401
							// high-speed is supported
							if (Hsmmc_Switch(1, 0, 1, SwitchStatus) == 0) { // switch
								if ((SwitchStatus[47] & 0xf) == 1) { // function switch in group 1 is ok?
									// result of the switch high-speed in function group 1
									Debug("Switch to high speed mode: CLK @ 50M\r\n");
									Hsmmc_SetClock(48000000); // 设置SDCLK = 48M
									Temp = 1;
								}
							}
						}
						break;
					}
				}
			}
			if (Temp == 0) {
				Hsmmc_SetClock(24000000); // 设置SDCLK = 48M/2 = 24M
			}
		}

		if (!Hsmmc_SetBusWidth(4)) {
			Debug("Set bus width error\r\n");
			return -1; // 位宽设置出错
		}
		if (Hsmmc_GetCardState() == CARD_TRAN) { // 此时卡应在transfer态
			if (!Hsmmc_IssueCommand(CMD16, 512, 0, CMD_RESP_R1)) { // 设置块长度为512字节
				__REGw(HSMMC_BASE+NORINTSTS_OFFSET) = 0xffff; // 清除中断标志
				Hsmmc_Get_CSD(CSD);
				if ((CSD[15]>>6) == 0) { // CSD v1.0->sd V1.x, sd v2.00 standar
					read_bl_len = CSD[10] & 0xf; // [83:80]
					c_size_multi = ((CSD[6] & 0x3) << 1) + ((CSD[5] & 0x80) >> 7); // [49:47]
					c_size = ((int32_t)(CSD[9]&0x3) << 10) + ((int32_t)CSD[8]<<2) + (CSD[7]>>6); // [73:62]
					Capacity = (c_size + 1) << ((c_size_multi + 2) + (read_bl_len-9)); // block(512 byte)
				} else {
					c_size = ((CSD[8]&0x3f) << 16) + (CSD[7]<<8) + CSD[6]; // [69:48]
					// 卡容量为字节(c_size+1)*512K byte,以1扇区512 byte字,卡的扇区数为
					Capacity = (c_size+1) << 10;// block (512 byte)
				}
				Debug("Card Initialization succeed\r\n");
				Debug("Capacity: %ldMB\r\n", Capacity / (1024*1024 / 512));
				return 0; // 初始化成功
			}
		}
	}
	Debug("Card Initialization failed\r\n");
	return -1; // 卡工作异常
}
