//=======================================================================
//  MStar Semiconductor - Unified eMMC Driver
//
//  eMMC_platform.c - Storage Team, 2011/02/25
//
//  Design Notes: defines common platform-dependent functions.
//
//    1. 2012/05/02 - support A3 uboot platform
//
//=======================================================================
#include "../../inc/common/eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

#if (defined(eMMC_DRV_INFINITY2_UBOOT)&&eMMC_DRV_INFINITY2_UBOOT)
// check some fix value, print only when setting wrong

extern void udelay(unsigned long usec);

U32 eMMC_hw_timer_delay(U32 u32us)
{
    #if 0
    volatile int i = 0;

    for (i = 0; i < (u32us >> 0); i++)
    {
        #if 1
        volatile int j = 0, tmp;
        for (j = 0; j < 0x2; j++)
        {
            tmp = j;
        }
        #endif
    }
    #else

    udelay(u32us);

    #endif

    return u32us;
}

U32 eMMC_hw_timer_sleep(U32 u32ms)
{
    U32 u32_us = u32ms * 1000;

    while(u32_us > 1000)
    {
        eMMC_hw_timer_delay(1000); // uboot has no context switch
        u32_us -= 1000;
    }

    eMMC_hw_timer_delay(u32_us);

    return u32ms;
}
U32 eMMC_hw_timer_start(void)
{
    // Reset PIU Timer1
    return 0;
}

U32 eMMC_hw_timer_tick(void)
{
    return 0;
}

void eMMC_DumpPadClk(void)
{
    //---------------------------------------------------------------------
    eMMC_debug(0, 0, "[pad setting]:\r\n");
    switch (g_eMMCDrv.u8_PadType)
    {
        case FCIE_eMMC_BYPASS:  eMMC_debug(0, 0, "Bypass\r\n");  break;
        default:
            eMMC_debug(0, 0, "eMMC Err: Pad unknown, %d\r\n", g_eMMCDrv.u8_PadType); eMMC_die("\r\n");
            break;
    }
}

// set pad first, then config clock
U32 eMMC_pads_switch(U32 u32Mode)
{
    g_eMMCDrv.u8_PadType = u32Mode;

    // pad top
    REG_FCIE_CLRBIT(reg_all_pad_in, BIT_ALL_PAD_IN);
    REG_FCIE_CLRBIT(reg_sd_config, BIT_SD_MODE_MASK);
    REG_FCIE_CLRBIT(reg_nand_config, BIT_NAND_MODE_MASK);
    REG_FCIE_SETBIT(reg_emmc_config, BIT11);
    REG_FCIE_W(reg_padtop1_0x22, 0xB6D9);
    REG_FCIE_W(reg_padtop1_0x23, 0x826D);
    REG_FCIE_W(reg_padtop1_0x24, 0x036D);
    REG_FCIE_SETBIT(reg_padtop0_0x16, BIT4);

    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_MACRO_MODE_MASK);
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT14|BIT13|BIT10);

    return eMMC_ST_SUCCESS;

}
// Notice!!! you need to set pad before config clock

U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN);

    switch(u16_ClkParam) {
        case BIT_FCIE_CLK_1X_XTAL:  g_eMMCDrv.u32_ClkKHz =   3000;  break;
        case BIT_FCIE_CLK_1X_12M:   g_eMMCDrv.u32_ClkKHz =  12000;  break;
        case BIT_FCIE_CLK_1X_15_5M: g_eMMCDrv.u32_ClkKHz =  15500;  break;
        case BIT_FCIE_CLK_1X_18M:   g_eMMCDrv.u32_ClkKHz =  18000;  break;
        case BIT_FCIE_CLK_1X_21_5M: g_eMMCDrv.u32_ClkKHz =  21500;  break;
        case BIT_FCIE_CLK_1X_27M:   g_eMMCDrv.u32_ClkKHz =  27000;  break;
        case BIT_FCIE_CLK_1X_36M:   g_eMMCDrv.u32_ClkKHz =  36000;  break;
        case BIT_FCIE_CLK_1X_40M:   g_eMMCDrv.u32_ClkKHz =  40000;  break;
        case BIT_FCIE_CLK_1X_48M:   g_eMMCDrv.u32_ClkKHz =  48000;  break;
        case BIT_FCIE_CLK_1X_54M:   g_eMMCDrv.u32_ClkKHz =  54000;  break;
        case BIT_FCIE_CLK_1X_60M:   g_eMMCDrv.u32_ClkKHz =  60000;  break;
        case BIT_FCIE_CLK_1X_88_5M: g_eMMCDrv.u32_ClkKHz =  88500;  break;
        case BIT_FCIE_CLK_1X_75K:   g_eMMCDrv.u32_ClkKHz =     75;  break;
        case BIT_FCIE_CLK_1X_80M:   g_eMMCDrv.u32_ClkKHz =  80000;  break;

        case BIT_FCIE_CLK_4X_XTAL:  g_eMMCDrv.u32_ClkKHz =  12000;  break;
        case BIT_FCIE_CLK_4X_48M:   g_eMMCDrv.u32_ClkKHz =  48000;  break;
        case BIT_FCIE_CLK_4X_62M:   g_eMMCDrv.u32_ClkKHz =  62000;  break;
        case BIT_FCIE_CLK_4X_72M:   g_eMMCDrv.u32_ClkKHz =  72000;  break;
        case BIT_FCIE_CLK_4X_86M:   g_eMMCDrv.u32_ClkKHz =  86000;  break;
        case BIT_FCIE_CLK_4X_108M:  g_eMMCDrv.u32_ClkKHz = 108000;  break;
        case BIT_FCIE_CLK_4X_144M:  g_eMMCDrv.u32_ClkKHz = 144000;  break;
        case BIT_FCIE_CLK_4X_160M:  g_eMMCDrv.u32_ClkKHz = 160000;  break;
        case BIT_FCIE_CLK_4X_192M:  g_eMMCDrv.u32_ClkKHz = 192000;  break;
        case BIT_FCIE_CLK_4X_216M:  g_eMMCDrv.u32_ClkKHz = 216000;  break;
        case BIT_FCIE_CLK_4X_240M:  g_eMMCDrv.u32_ClkKHz = 240000;  break;
        case BIT_FCIE_CLK_4X_354M:  g_eMMCDrv.u32_ClkKHz = 354000;  break;
        case BIT_FCIE_CLK_4X_300K:  g_eMMCDrv.u32_ClkKHz =    300;  break;
        case BIT_FCIE_CLK_4X_320M:  g_eMMCDrv.u32_ClkKHz = 320000;  break;

        default:
            eMMC_debug(1, 1, "eMMC Err: clkgen %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
            eMMC_die(" ");
            return eMMC_ST_ERR_INVALID_PARAM; break;
    }

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE(reg_ckg_fcie);
    udelay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING|BIT_FCIE_CLK_INVERSE|BIT_CLKGEN_FCIE_MASK|BIT_CKG_NFIE_DVI4_EN);

    if(u16_ClkParam & eMMC_4X_FLAG) {
        //eMMC_debug(0,0,"eMMC PLL: %Xh\n", u16_ClkParam);
        if( g_eMMCDrv.u32_DrvFlag & DRV_FLAG_DDR_MODE ) // DDR52
        {
            REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
            g_eMMCDrv.u32_ClkKHz >>= 2;
        }
        else if( g_eMMCDrv.u32_DrvFlag & DRV_FLAG_SPEED_HIGH ) // HS
        {
            REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
            g_eMMCDrv.u32_ClkKHz >>= 2;
        }
        else
            REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
    }
    else
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);

    REG_FCIE_SETBIT(reg_ckg_fcie, (u16_ClkParam & 0xF)<<10);
    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

    g_eMMCDrv.u16_ClkRegVal = (U16)u16_ClkParam;

    return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_gating(void)
{
	eMMC_PlatformResetPre();
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN);
	eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	return (u32_DMAAddr-RAM_START_ADDR);
}
U32 eMMC_PlatformResetPre(void)
{
	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_pads_switch(FCIE_eMMC_BYPASS);
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//static U32 sgu32_MemGuard0 = 0xA55A;
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//static U32 sgu32_MemGuard1 = 0x1289;

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

void *eMMC_memcpy (void *destaddr, void const *srcaddr, unsigned int len)
{
    char *dest = destaddr;
    char const *src = srcaddr;

    while (len-- > 0)
        *dest++ = *src++;
    return destaddr;
}

U32 eMMC_FCIE_DetectDDRTiming(void)
{
    return eMMC_ST_SUCCESS;
}

U32 eMMC_BootPartitionHandler_WR(U8 *pDataBuf, U16 u16_PartType, U32 u32_StartSector, U32 u32_SectorCnt, U8 u8_OP)
{
    return eMMC_ST_SUCCESS;
}


U32 eMMC_BootPartitionHandler_E(U16 u16_PartType)
{
    return eMMC_ST_SUCCESS;
}


#else
  #error "Error! no platform functions."
#endif
#endif
