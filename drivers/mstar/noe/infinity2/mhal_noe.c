//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2017 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************

#include <asm/types.h>
#include <miiphy.h>
#include "mhal_noe.h"

#define barrier() __asm__ __volatile__("": : :"memory")

#define TOGGLING_BIT_VALUE(value, mask, data) \
  (value) = (((value) & ~(mask)) | (data))

#define EWAVE_TESTMODE_NUM          3
#define EWAVE_SUBITEMS_NUM          4

#define EWAVE_10M_SUBITEMS_NUM      4
#define EWAVE_100M_SUBITEMS_NUM     3
#define EWAVE_1000M_SUBITEMS_NUM    3

typedef enum {
    E_MHAL_NOE_DELAY_NS_0,
    E_MHAL_NOE_DELAY_NS_2,
} EN_MHAL_NOE_DELAY_TIME;

typedef struct noe_hal_config_s {
    u32 eport;
    u32 connect;
    u32 phyaddr;
    u32 delay;
} noe_hal_config_t;

typedef enum {
    E_HAL_NOE_EWAVE_10M = 0,
    E_HAL_NOE_EWAVE_100M,
    E_HAL_NOE_EWAVE_1000M,
} EN_HAL_NOE_EWAVE_MODE;

typedef void (*ewave_test_cb)(u32 phyaddr);

/* functions declarations */
static void _MHal_NOE_GE1_To_ChipTop0(void);
static void _MHal_NOE_GE2_To_ChipTop1(void);
static void _MHal_NOE_Software_Reset(void);
static void _MHal_NOE_Init_Clock(void);
static void MHal_NOE_Enable_eWave(u32 phyaddr);
static void MHal_NOE_Disable_eWave(u32 phyaddr);
static void MHal_NOE_eWave_10M_Random_Pattern(u32 phyaddr);
static void MHal_NOE_eWave_10M_FixedFF_Pattern(u32 phyaddr);
static void MHal_NOE_eWave_10M_Fixed00_Pattern(u32 phyaddr);
static void MHal_NOE_eWave_10M_Finish(u32 phyaddr);
static void MHal_NOE_eWave_100M_MLT3_ChannelA(u32 phyaddr);
static void MHal_NOE_eWave_100M_MLT3_ChannelB(u32 phyaddr);
static void MHal_NOE_eWave_100M_Finish(u32 phyaddr);
static void MHal_NOE_eWave_1000M_TestMode1(u32 phyaddr);
static void MHal_NOE_eWave_1000M_TestMode4(u32 phyaddr);
static void MHal_NOE_eWave_1000M_Finish(u32 phyaddr);
extern u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data);

static noe_hal_config_t noe_hal_config = {
    .eport      = E_HAL_NOE_EPORT_INVALID,
    .connect    = E_HAL_NOE_GMAC_TO_INVALID,
    .phyaddr    = 0x0,
    .delay      = E_MHAL_NOE_DELAY_NS_2,
};

static ewave_test_cb ewave_table[EWAVE_TESTMODE_NUM][EWAVE_SUBITEMS_NUM] = {
    {MHal_NOE_eWave_10M_Random_Pattern, MHal_NOE_eWave_10M_FixedFF_Pattern,\
        MHal_NOE_eWave_10M_Fixed00_Pattern, MHal_NOE_eWave_10M_Finish},
    {MHal_NOE_eWave_100M_MLT3_ChannelA, MHal_NOE_eWave_100M_MLT3_ChannelB,\
        MHal_NOE_eWave_100M_Finish, NULL},
    {MHal_NOE_eWave_1000M_TestMode1, MHal_NOE_eWave_1000M_TestMode4,\
        MHal_NOE_eWave_1000M_Finish, NULL}
};

int   noe_init = 0;

void MHal_NOE_Set_Initialized(int value)
{
    noe_init = value;
}

int MHal_NOE_Is_Initialized(void)
{
    return noe_init;
}

void MHal_NOE_Write_SA1_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
    u32 regValue_lsb, regValue_msb;
    u16 wTmp_lsb, wTmp_msb;
    if (noe_init == 1) {
        /* MSB */
        wTmp_msb = (u16)m0;
        regValue_msb = (wTmp_msb << 8) | m1;
        /* LSB */
        wTmp_lsb = (u16)m2;
        regValue_lsb = (wTmp_lsb << 8) | m3;
        regValue_lsb = regValue_lsb << 16;
        wTmp_lsb = (u16)m4;
        regValue_lsb |= (wTmp_lsb<<8) | m5;
        if(noe_hal_config.eport == E_HAL_NOE_EPORT_GE1) {
            NOE_REG(GDMA1_MAC_ADRH) = regValue_msb;
            NOE_REG(GDMA1_MAC_ADRL) = regValue_lsb;
        }
        else {
            NOE_REG(GDMA2_MAC_ADRH) = regValue_msb;
            NOE_REG(GDMA2_MAC_ADRL) = regValue_lsb;
        }
    }
}

void MHal_NOE_Write_SA2_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
    /* TBD */
}

void MHal_NOE_Write_SA3_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
    /* TBD */
}

void MHal_NOE_Write_SA4_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
    /* TBD */
}

void MHal_NOE_Init_PinMux_Config(void)
{
#if (CONFIG_USE_NOE_GE1)
    noe_hal_config.eport = E_HAL_NOE_EPORT_GE1;
    noe_hal_config.phyaddr = CONFIG_NOE_GMAC_PHY_ADDR;
    noe_hal_config.connect = E_HAL_NOE_GMAC_TO_CHIPTOP;
#else   // CONFIG_USE_NOE_GE2
    noe_hal_config.eport = E_HAL_NOE_EPORT_GE2;
    noe_hal_config.phyaddr = CONFIG_NOE_GMAC_PHY_ADDR;
    noe_hal_config.connect = E_HAL_NOE_GMAC_TO_CHIPTOP;
#endif
}

u32 MHal_NOE_Get_PinMux_Eport(void)
{
    return noe_hal_config.eport;
}

u32 MHal_NOE_Get_PinMux_Connect(void)
{
    return noe_hal_config.connect;
}

u32 MHal_NOE_Get_PinMux_Phyaddr(void)
{
    return noe_hal_config.phyaddr;
}

int MHal_NOE_Set_PinMux_Config(u32 eport, u32 connect)
{
    if (eport <= E_HAL_NOE_EPORT_INVALID || eport >= E_HAL_NOE_EPORT_MAX) {
        return ERR_PINMUX_INVALID_EPORT;
    }

    if (connect <= E_HAL_NOE_GMAC_TO_INVALID || connect >= E_HAL_NOE_GMAC_TO_MAX) {
        return ERR_PINMUX_INVALID_CONNECT;
    }

    noe_hal_config.eport    = eport;
    noe_hal_config.connect  = connect;
    return ERR_PINMUX_SUCCESS;
}

void MHal_NOE_Set_PinMux_Phyaddr(u32 phyaddr)
{
    noe_hal_config.phyaddr = phyaddr;
}

void MHal_NOE_Switch_MDIO(u32 eport)
{
    u32 regValue;

    if (eport == E_HAL_NOE_EPORT_GE1) {
        regValue = MSTAR_RIU_REG(0x1224, 0x04);
        MSTAR_RIU_REG(0x1224, 0x04) = TOGGLING_BIT_VALUE(regValue, 0x0003, 0x0003);
    }
    else {
        regValue = MSTAR_RIU_REG(0x1224, 0x04);
        MSTAR_RIU_REG(0x1224, 0x04) = TOGGLING_BIT_VALUE(regValue, 0x0002, 0x0002);
    }

    regValue = MSTAR_RIU_REG(0x1026, 0x0a);
    MSTAR_RIU_REG(0x1026, 0x0a) = TOGGLING_BIT_VALUE(regValue, 0x0040, 0x0040);
}


static void _MHal_NOE_Init_Clock(void)
{
    MSTAR_RIU8_REG(NOE_RIU_BANK_CLKGEN1, 0x2a) = 0x00;
    MSTAR_RIU8_REG(NOE_RIU_BANK_CLKGEN0, 0xc6) = 0x00;
    MSTAR_RIU8_REG(NOE_RIU_BANK_ANA_MISC_GMAC, 0xd2) = 0x14;
    MSTAR_RIU8_REG(NOE_RIU_BANK_ANA_MISC_GMAC, 0xc6) = 0x00;
    if (noe_hal_config.eport == E_HAL_NOE_EPORT_GE1) {
        MSTAR_RIU8_REG(NOE_RIU_BANK_CLKGEN2, 0x03) = 0x0c;
    }
    else {
        MSTAR_RIU8_REG(NOE_RIU_BANK_CLKGEN2, 0xa1) = 0x0c;
    }

    /* NOE system clkgen */
    MSTAR_RIU_REG(NOE_RIU_BANK_CLKGEN1, 0x26) = 0x0400;
    MSTAR_RIU_REG(NOE_RIU_BANK_CLKGEN1, 0x28) = 0x0004;
    MSTAR_RIU_REG(NOE_RIU_BANK_CLKGEN1, 0x2a) = 0x0000;

    _MHal_NOE_Software_Reset();
}


void MHal_NOE_Set_PinMux(u32 eport, u32 connect)
{
    if (eport == E_HAL_NOE_EPORT_GE1 && connect == E_HAL_NOE_GMAC_TO_CHIPTOP) {
         _MHal_NOE_GE1_To_ChipTop0();
    }
    else if (eport == E_HAL_NOE_EPORT_GE2 && connect == E_HAL_NOE_GMAC_TO_CHIPTOP) {
        _MHal_NOE_GE2_To_ChipTop1();
    }
}


static void _MHal_NOE_GE1_To_ChipTop0(void)
{
    MSTAR_RIU_REG(NOE_RIU_BANK_PADTOP1, 0x0a) = 0x00C0;
    MSTAR_RIU8_REG(NOE_RIU_BANK_PADTOP1, 0x00) = 0x00;
    MSTAR_RIU8_REG(NOE_RIU_BANK_GMAC4, 0x23) = 0x00;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x6e) = 0x1108;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x22) = 0x0088;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x24) = 0x2191;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x26) = 0x6ee2;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x20) = 0xa200;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x04) = 0x0103;
}

static void _MHal_NOE_GE2_To_ChipTop1(void)
{
    u8 regValue8;

    regValue8 = MSTAR_RIU8_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x04);
    MSTAR_RIU8_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x04) = TOGGLING_BIT_VALUE(regValue8, 0x02, 0x02);

    regValue8 = MSTAR_RIU8_REG(NOE_RIU_BANK_PADTOP1, 0x0a);
    MSTAR_RIU8_REG(NOE_RIU_BANK_PADTOP1, 0x0a) = TOGGLING_BIT_VALUE(regValue8, 0x40, 0x40);

    MSTAR_RIU8_REG(NOE_RIU_BANK_PADTOP1, 0x0b) = 0x02;
    MSTAR_RIU8_REG(NOE_RIU_BANK_PADTOP1, 0x00) = 0x00;
    MSTAR_RIU8_REG(NOE_RIU_BANK_GMAC4, 0x23) = 0x00;

    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0xee) = 0x1108;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0xa2) = 0x0088;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0xa4) = 0x2191;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0xa6) = 0x6ee2;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0xa0) = 0xa200;
    MSTAR_RIU_REG(NOE_RIU_BANK_ETHMAC_CTRL, 0x84) = 0x0103;
}


static void _MHal_NOE_Software_Reset(void)
{
    MSTAR_RIU_REG(NOE_RIU_BANK_NOE_MISC, 0x00) = 0x0003;
    udelay(100);
    MSTAR_RIU_REG(NOE_RIU_BANK_NOE_MISC, 0x00) = 0x0000;
}

void MHal_NOE_Init_System(void)
{
    _MHal_NOE_Init_Clock();
    /* GMAC1= RGMII mode */
    NOE_REG(NOE_SYSCFG1_REG) &= ~(0x3 << 12);
    /* GMAC2= RGMII mode */
    NOE_REG(NOE_SYSCFG1_REG) &= ~(0x3 << 14);
    MHal_NOE_Set_Initialized(1);
}

void MHal_NOE_Set_GDM_Forward_Port(u32 eport)
{
    u32 regValue;
    if (eport == E_HAL_NOE_EPORT_GE1) {
        regValue = NOE_REG(GDMA1_FWD_CFG);
        regValue = regValue & GDM_UFRC_P_CPU;
        regValue = regValue & GDM_BFRC_P_CPU;
        regValue = regValue & GDM_MFRC_P_CPU;
        regValue = regValue & GDM_OFRC_P_CPU;
        NOE_REG(GDMA1_FWD_CFG) = regValue;
    }
    else {
        regValue = NOE_REG(GDMA2_FWD_CFG);
        regValue = regValue & GDM_UFRC_P_CPU;
        regValue = regValue & GDM_BFRC_P_CPU;
        regValue = regValue & GDM_MFRC_P_CPU;
        regValue = regValue & GDM_OFRC_P_CPU;
        NOE_REG(GDMA2_FWD_CFG) = regValue;
    }
}


static void MHal_NOE_Enable_eWave(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0005);
    mii_mgr_write(phyaddr, 5,  0x8b86);
    mii_mgr_write(phyaddr, 6,  0xe200);
    mii_mgr_write(phyaddr, 31, 0x0007);
    mii_mgr_write(phyaddr, 30, 0x0020);
    mii_mgr_write(phyaddr, 21, 0x0108);
    mii_mgr_write(phyaddr, 31, 0x0000);
}

static void MHal_NOE_Disable_eWave(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0005);
    mii_mgr_write(phyaddr, 5,  0x8b86);
    mii_mgr_write(phyaddr, 6,  0xe201);
    mii_mgr_write(phyaddr, 31, 0x0007);
    mii_mgr_write(phyaddr, 30, 0x0020);
    mii_mgr_write(phyaddr, 21, 0x1108);
    mii_mgr_write(phyaddr, 31, 0x0000);
}


static void MHal_NOE_eWave_10M_Random_Pattern(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0006);
    mii_mgr_write(phyaddr, 0,  0x5a21);
}

static void MHal_NOE_eWave_10M_FixedFF_Pattern(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0006);
    mii_mgr_write(phyaddr, 2,  0x05ee);
    mii_mgr_write(phyaddr, 0,  0xff21);
}

static void MHal_NOE_eWave_10M_Fixed00_Pattern(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0006);
    mii_mgr_write(phyaddr, 2,  0x05ee);
    mii_mgr_write(phyaddr, 0,  0x0021);
}

static void MHal_NOE_eWave_10M_Finish(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0006);
    mii_mgr_write(phyaddr, 0,  0x5a00);
    mii_mgr_write(phyaddr, 31, 0x0000);
}


static void MHal_NOE_eWave_100M_MLT3_ChannelA(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0007);
    mii_mgr_write(phyaddr, 30, 0x002f);
    mii_mgr_write(phyaddr, 23, 0xd818);
    mii_mgr_write(phyaddr, 30, 0x002d);
    mii_mgr_write(phyaddr, 24, 0xf060);
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 16, 0x00ae);
}

static void MHal_NOE_eWave_100M_MLT3_ChannelB(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0007);
    mii_mgr_write(phyaddr, 30, 0x002f);
    mii_mgr_write(phyaddr, 23, 0xd818);
    mii_mgr_write(phyaddr, 30, 0x002d);
    mii_mgr_write(phyaddr, 24, 0xf060);
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 16, 0x008e);
}

static void MHal_NOE_eWave_100M_Finish(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0007);
    mii_mgr_write(phyaddr, 30, 0x002f);
    mii_mgr_write(phyaddr, 23, 0xd88f);
    mii_mgr_write(phyaddr, 30, 0x002d);
    mii_mgr_write(phyaddr, 24, 0xf050);
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 16, 0x006e);
}

static void MHal_NOE_eWave_1000M_TestMode1(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 9,  0x2000);
}

static void MHal_NOE_eWave_1000M_TestMode4(u32 phyaddr)
{
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 9,  0x8000);
}

static void MHal_NOE_eWave_1000M_Finish(u32 phyaddr)
{
    /* Disable test mode 1/4 */
    mii_mgr_write(phyaddr, 31, 0x0000);
    mii_mgr_write(phyaddr, 9,  0x0000);
}

int MHal_NOE_eWave(int mode, int subitem)
{
    u32 phyaddr = noe_hal_config.phyaddr;
    switch(mode) {
        case E_HAL_NOE_EWAVE_10M:
            if (subitem >= EWAVE_10M_SUBITEMS_NUM) {
                return 1;
            }
            break;
        case E_HAL_NOE_EWAVE_100M:
            if (subitem >= EWAVE_100M_SUBITEMS_NUM) {
                return 1;
            }
            break;
        case E_HAL_NOE_EWAVE_1000M:
            if (subitem >= EWAVE_1000M_SUBITEMS_NUM) {
                return 1;
            }
            break;
        default:
            return 1;
    }
    MHal_NOE_Enable_eWave(phyaddr);
    ewave_table[mode][subitem](phyaddr);
    MHal_NOE_Disable_eWave(phyaddr);
    return 0;
}

