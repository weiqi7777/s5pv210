
#define ELFIN_CLOCK_POWER_BASE 0Xe0100000

#define APLL_LOCK_OFFSET    0x00
#define MPLL_LOCK_OFFSET    0X08

#define APLL_CON0_OFFSET    0X100
#define APLL_CON1_OFFSET    0X104
#define MPLL_CON_OFFSET		0X108

#define CLK_SRC0_OFFSET     0X200
#define CLK_SRC1_OFFSET     0X204
#define CLK_SRC2_OFFSET     0X208
#define CLK_SRC3_OFFSET     0X20C
#define CLK_SRC4_OFFSET     0X210
#define CLK_SRC5_OFFSET     0X214
#define CLK_SRC6_OFFSET 	0X218

#define CLK_SRC_MASK0_OFFSET  0X280
#define CLK_SRC_MASK1_OFFSET  0X284

#define CLK_DIV0_OFFSET		0X300
#define CLK_DIV1_OFFSET 	0X304
#define CLK_DIV2_OFFSET		0X308
#define CLK_DIV3_OFFSET		0X30C
#define CLK_DIV4_OFFSET		0X310
#define CLK_DIV5_OFFSET		0X314
#define CLK_DIV6_OFFSET		0X318
#define CLK_DIV7_OFFSET		0X31C

#define APLL_MDIV		0X7D
#define APLL_PDIV		0X3
#define APLL_SDIV		0X1

#define MPLL_MDIV		0X29B
#define MPLL_PDIV		0XC
#define MPLL_SDIV		0X1

#define set_pll(mdiv,pdiv,sdiv) (1<<31 | mdiv<<16 | pdiv<<8 | sdiv)
#define APLL_VAL	set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL	set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)

#define REG_CLK_SRC0    (ELFIN_CLOCK_POWER_BASE + CLK_SRC0_OFFSET)
#define REG_APLL_LOCK   (ELFIN_CLOCK_POWER_BASE + APLL_LOCK_OFFSET)
#define REG_MPLL_LOCK	(ELFIN_CLOCK_POWER_BASE + MPLL_LOCK_OFFSET)
#define REG_CLK_DIV0	(ELFIN_CLOCK_POWER_BASE + CLK_DIV0_OFFSET)
#define REG_APLL_CON0	(ELFIN_CLOCK_POWER_BASE + APLL_CON0_OFFSET)
#define REG_MPLL_CON	(ELFIN_CLOCK_POWER_BASE + MPLL_CON_OFFSET)

#define rREG_CLK_SRC0	(*(volatile unsigned long *)REG_CLK_SRC0)
#define rREG_APLL_LOCK	(*(volatile unsigned long *)REG_APLL_LOCK)
#define rREG_MPLL_LOCK	(*(volatile unsigned long *)REG_MPLL_LOCK)
#define rREG_CLK_DIV0	(*(volatile unsigned long *)REG_CLK_DIV0)
#define rREG_APLL_CON0	(*(volatile unsigned long *)REG_APLL_CON0)
#define rREG_MPLL_CON	(*(volatile unsigned long *)REG_MPLL_CON)

void clock_init(void)
{
	//set clocks switch , no use PLL
	rREG_CLK_SRC0 = 0X0;
	
	//set lock time
	//when set PLL, clock become from Fin to target frequence, it will cost times,
	//the times is called lock time 
	rREG_APLL_LOCK = 0X0000FFFF;
	rREG_MPLL_LOCK = 0X0000FFFF;
	
	/*
	set divided frequence value
	  ARMCLK    = MOUT_MSYS / ( 0 + 1 ) = MOUT_MSYS     (1000M)
	  HCLK_MSYS = ARMCLK    / ( 4 + 1 ) = ARMCLK / 5    (200M)
	  PCLK_MSYS = HCLK_MSYS / ( 1 + 1 ) = HCLK_MSYS / 2 (100M)
	  HCLK_DSYS = MOUT_DSYS / ( 3 + 1 ) = MOUT_DSYS / 4 (166M)
	  PCLK_DSYS = HCLK_DSYS / ( 1 + 1 ) = HCLK_DSYS / 2 (83M)
	  HCLK_PSYS = MOUT_PSYS / ( 4 + 1 ) = MOUT_PSYS / 5 (133M)
	  PCLK_PSYS = HCLK_PSYS / ( 1 + 1 ) = CHLK_PSYS / 2 (66M)
	*/
	rREG_CLK_DIV0 = 0X14131440;
	
	/*
	set PLL
		APLL_FOUT = APLL_MDIV*FIN/(APLL_PDIV*2^(APLL_SDIV-1)) = 0x7d*24/(0x3*2^(1-1))=1000 MHz
		MPLL_FOUT = MPLL_MDIV*FIN/(MPLL_PDIV*2^MPLL_SDIV)=0x29b*24/(0xc*2^1)= 667 MHz
	*/
	rREG_APLL_CON0 = APLL_VAL;
	rREG_MPLL_CON  = MPLL_VAL;

	//set clock switch , use PLL
	rREG_CLK_SRC0 = 0X10001111;
	
}
