// 时钟控制器基地址
#define ELFIN_CLOCK_POWER_BASE		0xE0100000	

// 时钟相关的寄存器相对时钟控制器基地址的偏移值
#define APLL_LOCK_OFFSET		0x00		
#define MPLL_LOCK_OFFSET		0x08

#define APLL_CON0_OFFSET		0x100
#define APLL_CON1_OFFSET		0x104
#define MPLL_CON_OFFSET			0x108

#define CLK_SRC0_OFFSET			0x200
#define CLK_SRC1_OFFSET			0x204
#define CLK_SRC2_OFFSET			0x208
#define CLK_SRC3_OFFSET			0x20c
#define CLK_SRC4_OFFSET			0x210
#define CLK_SRC5_OFFSET			0x214
#define CLK_SRC6_OFFSET			0x218
#define CLK_SRC_MASK0_OFFSET	0x280
#define CLK_SRC_MASK1_OFFSET	0x284

#define CLK_DIV0_OFFSET			0x300
#define CLK_DIV1_OFFSET			0x304
#define CLK_DIV2_OFFSET			0x308
#define CLK_DIV3_OFFSET			0x30c
#define CLK_DIV4_OFFSET			0x310
#define CLK_DIV5_OFFSET			0x314
#define CLK_DIV6_OFFSET			0x318
#define CLK_DIV7_OFFSET			0x31c

#define CLK_DIV0_MASK			0x7fffffff

// 这些M、P、S的配置值都是查数据手册中典型时钟配置值的推荐配置得来的。
// 这些配置值是三星推荐的，因此工作最稳定。如果是自己随便瞎拼凑出来的那就要
// 经过严格测试，才能保证一定对。 
// APLL输出1000M
#define APLL_MDIV      	 		0x7d		// 125
#define APLL_PDIV       		0x3
#define APLL_SDIV       		0x1

// MPLL输出667M
#define MPLL_MDIV				0x29b		// 667
#define MPLL_PDIV				0xc
#define MPLL_SDIV				0x1

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)
#define APLL_VAL			set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL			set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)


.global clock_init
clock_init:
	ldr	r0, =ELFIN_CLOCK_POWER_BASE
	
	// 1 设置各种时钟开关，暂时不使用PLL
	ldr	r1, =0x0
	// 芯片手册P378 寄存器CLK_SRC：Select clock source 0 (Main)
	str	r1, [r0, #CLK_SRC0_OFFSET]				

	// 2 设置锁定时间，使用默认值即可
	// 设置PLL后，时钟从Fin提升到目标频率时，需要一定的时间，即锁定时间
	ldr	r1,	=0x0000FFFF					
	str	r1,	[r0, #APLL_LOCK_OFFSET]				
	str r1, [r0, #MPLL_LOCK_OFFSET]	 				

	// 3 设置分频
	// 清bit[0~31]
	//得到 ARMCLK    = MOUT_MSYS / ( 0 + 1 ) = MOUT_MSYS       (1000M)
	//     HCLK_MSYS = ARMCLK    / ( 4 + 1 ) = ARMCLK    / 5   (200M)
	//	   PCLK_MSYS = HCLK_MSYS / ( 1 + 1 ) = HCLK_MSYS / 2   (100M)
	//     HCLK_DSYS = MOUT_DSYS / ( 3 + 1 ) = MOUT_DSYS / 4   (166M)
	//     PCLK_DSYS = HCLK_DSYS / ( 1 + 1 ) = HCLK_DSYS / 2   (83M)
	//	   HCLK_PSYS = MOUT_PSYS / ( 4 + 1 ) = MOUT_PSYS / 5   (133M)
	//	   PCLK_PSYS = HCLK_PSYS / ( 1 + 1 ) = CHLK_PSYS / 2   (66M)
	//ldr r1, [r0, #CLK_DIV0_OFFSET]					
	//ldr	r2, =CLK_DIV0_MASK					
	//bic	r1, r1, r2
	ldr	r2, =0x14131440						
	//orr	r1, r1, r2
	str	r2, [r0, #CLK_DIV0_OFFSET]

	// 4 设置PLL
	// FOUT = MDIV*FIN/(PDIV*2^(SDIV-1))=0x7d*24/(0x3*2^(1-1))=1000 MHz
	ldr	r1, =APLL_VAL						
	str	r1, [r0, #APLL_CON0_OFFSET]
	// FOUT = MDIV*FIN/(PDIV*2^SDIV)=0x29b*24/(0xc*2^1)= 667 MHz
	ldr	r1, =MPLL_VAL						
	str	r1, [r0, #MPLL_CON_OFFSET]

	// 5 设置各种时钟开关,使用PLL
	ldr	r1, [r0, #CLK_SRC0_OFFSET]
	ldr	r2, =0x10001111
	orr	r1, r1, r2
	str	r1, [r0, #CLK_SRC0_OFFSET]

	mov	pc, lr
