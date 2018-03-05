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

#ifndef _MHAL_NOE_H_
#define _MHAL_NOE_H_

#include <common.h>
#include <command.h>
#include "../arch/arm/include/asm/arch-infinity2/mach/platform.h"

#if defined(CONFIG_VERSION_FPGA)
#define MSTAR_NOE_USE_FPGA_ENV
#endif

#define RIU_REG_BASE                    MS_BASE_REG_RIU_PA
#define MSTAR_RIU_ADDR(reg)             ((reg) << 1)
#define MSTAR_RIU_BANK(bank)            (RIU_REG_BASE + (bank << 9))
#define MSTAR_RIU_REG(bank,reg)        (*((volatile unsigned short*)(MSTAR_RIU_BANK(bank) + MSTAR_RIU_ADDR(reg))))

#define MSTAR_RIU8_ADDR(reg)            ((reg << 1) - (reg & 1))
#define MSTAR_RIU8_REG(bank,reg)       (*((volatile unsigned char*)(MSTAR_RIU_BANK(bank) + MSTAR_RIU8_ADDR(reg))))

#define RIU_NOE_OFFSET                  0x00C00000
#define ETHDMASYS_BASE                  (RIU_REG_BASE + RIU_NOE_OFFSET)

#define ETHDMASYS_SYSCTL_BASE           (ETHDMASYS_BASE + 0x00000000)
#define ETHDMASYS_RBUS_MATRIXCTL_BASE   (ETHDMASYS_BASE + 0x00000400)
#define ETHDMASYS_GDMA_BASE             (ETHDMASYS_BASE + 0x00002800)
#define ETHDMASYS_HS_DMA_BASE           (ETHDMASYS_BASE + 0x00007000)
#define ETHDMASYS_FRAME_ENGINE_BASE     (ETHDMASYS_BASE + 0x00100000)
#define ETHDMASYS_ETH_MAC_BASE          (ETHDMASYS_BASE + 0x00110000)

// for backward-compatible
#define NOE_FRAME_ENGINE_BASE           ETHDMASYS_FRAME_ENGINE_BASE
#define NOE_SYSCTL_BASE                 ETHDMASYS_SYSCTL_BASE
#define NOE_ETH_MAC_BASE                ETHDMASYS_ETH_MAC_BASE
#define NOE_GDMA_BASE                   ETHDMASYS_GDMA_BASE
#define NOE_HS_DMA_BASE                 ETHDMASYS_HS_DMA_BASE
#define NOE_11N_MAC_BASE                0   //unused for rt_rdm usage

// Reset Control Register
#define RSTCTL_SYS_RST                  (1<<0)
#define RSTCTL_MCM_RST                  (1<<2)
#define RSTCTL_HSDMA_RST                (1<<5)
#define RSTCTL_FE_RST                   (1<<6)
#define RSTCTL_SPDIF_RST                (1<<7)
#define RSTCTL_TIMER_RST                (1<<8)
#define RSTCTL_CIRQ_RST                 (1<<9)
#define RSTCTL_MC_RST                   (1<<10)
#define RSTCTL_PCM_RST                  (1<<11)
#define RSTCTL_GPIO_RST                 (1<<13)
#define RSTCTL_GDMA_RST                 (1<<14)
#define RSTCTL_NAND_RST                 (1<<15)
#define RSTCTL_I2C_RST                  (1<<16)
#define RSTCTL_I2S_RST                  (1<<17)
#define RSTCTL_SPI_RST                  (1<<18)
#define RSTCTL_UART0_RST                (1<<19)
#define RSTCTL_UART1_RST                (1<<20)
#define RSTCTL_UART2_RST                (1<<21)
#define RSTCTL_UPHY_RST                 (1<<22)
#define RSTCTL_ETH_RST                  (1<<23)
#define RSTCTL_PCIE0_RST                (1<<24)
#define RSTCTL_PCIE1_RST                (1<<25)
#define RSTCTL_PCIE2_RST                (1<<26)
#define RSTCTL_AUX_STCK_RST             (1<<28)
#define RSTCTL_CRYPT_RST                (1<<29)
#define RSTCTL_SDXC_RST                 (1<<30)
#define RSTCTL_PWM_RST                  (1<<31)

// for backward-compatible
#define NOE_FE_RST                      RSTCTL_FE_RST
#define SYSCFG1                         (NOE_SYSCTL_BASE + 0x14)

/*
 * System Controller
 *   Offset
 *   0x10  -- SYSCFG        System Configuration Register
 *   0x30  -- CLKCFG1       Clock Configuration Register
 *   0x34  -- RSTCTRL       Reset Control Register
 *   0x38  -- RSTSTAT       Reset Status Register
 *   0x60  -- GPIOMODE      GPIO Mode Control Register
 */

#define NOE_SYS_CNTL_BASE               (NOE_SYSCTL_BASE)
#define NOE_CHIP_REV_ID_REG             (NOE_SYS_CNTL_BASE+0x0c)
#define NOE_SYSCFG_REG                  (NOE_SYS_CNTL_BASE+0x10)
#define NOE_SYSCFG1_REG                 (NOE_SYS_CNTL_BASE+0x14)
#define NOE_CLKCFG1_REG                 (NOE_SYS_CNTL_BASE+0x30)
#define NOE_RSTCTRL_REG                 (NOE_SYS_CNTL_BASE+0x34)
#define NOE_RSTSTAT_REG                 (NOE_SYS_CNTL_BASE+0x38)
#define NOE_SYSCLKCFG_REG               (NOE_SYS_CNTL_BASE+0x3c)
#define NOE_GPIOMODE_REG                (NOE_SYS_CNTL_BASE+0x60)

#define NOE_PRGIO_ADDR                  (NOE_SYSCTL_BASE + 0x600) // Programmable I/O
#define NOE_REG_PIOINT                  (NOE_PRGIO_ADDR + 0)
#define NOE_REG_PIOEDGE                 (NOE_PRGIO_ADDR + 0x04)
#define NOE_REG_PIORENA                 (NOE_PRGIO_ADDR + 0x08)
#define NOE_REG_PIOFENA                 (NOE_PRGIO_ADDR + 0x0C)
#define NOE_REG_PIODATA                 (NOE_PRGIO_ADDR + 0x20)
#define NOE_REG_PIODIR                  (NOE_PRGIO_ADDR + 0x24)

#define NOE_CLKCFG0_REG                 (NOE_SYS_CNTL_BASE+0x2C)

#define NOE_REG(x)      (*((volatile u32 *)(x)))

/* ====================================== */
// GDMA1 uni-cast frames destination port
#define GDM_UFRC_P_CPU      ((u32)(~(0x7 << 12)))
#define GDM_UFRC_P_GDMA1    (1 << 12)
#define GDM_UFRC_P_GDMA2    (2 << 12)
#define GDM_UFRC_P_DROP     (7 << 12)
// GDMA1 broad-cast MAC address frames
#define GDM_BFRC_P_CPU      ((u32)(~(0x7 << 8)))
#define GDM_BFRC_P_GDMA1    (1 << 8)
#define GDM_BFRC_P_GDMA2    (2 << 8)
#define GDM_BFRC_P_PPE      (6 << 8)
#define GDM_BFRC_P_DROP     (7 << 8)
// GDMA1 multi-cast MAC address frames
#define GDM_MFRC_P_CPU      ((u32)(~(0x7 << 4)))
#define GDM_MFRC_P_GDMA1    (1 << 4)
#define GDM_MFRC_P_GDMA2    (2 << 4)
#define GDM_MFRC_P_PPE      (6 << 4)
#define GDM_MFRC_P_DROP     (7 << 4)
// GDMA1 other MAC address frames destination port
#define GDM_OFRC_P_CPU      ((u32)(~(0x7)))
#define GDM_OFRC_P_GDMA1    1
#define GDM_OFRC_P_GDMA2    2
#define GDM_OFRC_P_PPE      6
#define GDM_OFRC_P_DROP     7

#define RST_DRX_IDX0        BIT(16)
#define RST_DTX_IDX0        BIT(0)

#define TX_WB_DDONE         BIT(6)
#define RX_DMA_BUSY         BIT(3)
#define TX_DMA_BUSY         BIT(1)
#define RX_DMA_EN           BIT(2)
#define TX_DMA_EN           BIT(0)

#define GP1_FRC_EN          BIT(15)
#define GP1_FC_TX           BIT(11)
#define GP1_FC_RX           BIT(10)
#define GP1_LNK_DWN         BIT(9)
#define GP1_AN_OK           BIT(8)

/*
 * FE_INT_STATUS
 */
#define CNT_PPE_AF          BIT(31)
#define CNT_GDM1_AF         BIT(29)
#define PSE_P1_FC           BIT(22)
#define PSE_P0_FC           BIT(21)
#define PSE_FQ_EMPTY        BIT(20)
#define GE1_STA_CHG         BIT(18)
#define TX_COHERENT         BIT(17)
#define RX_COHERENT         BIT(16)

#define TX_DONE_INT1        BIT(9)
#define TX_DONE_INT0        BIT(8)
#define RX_DONE_INT0        BIT(2)
#define TX_DLY_INT          BIT(1)
#define RX_DLY_INT          BIT(0)

/*
 * Ethernet chip registers.NOE
 */
/* Old FE with New PDMA */
#define PDMA_RELATED        0x0800
/* 1. PDMA */
#define TX_BASE_PTR0        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x000)
#define TX_MAX_CNT0         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x004)
#define TX_CTX_IDX0         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x008)
#define TX_DTX_IDX0         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x00C)

#define TX_BASE_PTR1        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x010)
#define TX_MAX_CNT1         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x014)
#define TX_CTX_IDX1         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x018)
#define TX_DTX_IDX1         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x01C)

#define TX_BASE_PTR2        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x020)
#define TX_MAX_CNT2         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x024)
#define TX_CTX_IDX2         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x028)
#define TX_DTX_IDX2         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x
#define TX_BASE_PTR3        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x030)
#define TX_MAX_CNT3         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x034)
#define TX_CTX_IDX3         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x038)
#define TX_DTX_IDX3         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x
#define RX_BASE_PTR0        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x100)
#define RX_MAX_CNT0         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x104)
#define RX_CALC_IDX0        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x108)
#define RX_DRX_IDX0         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x10c)
#define RX_BASE_PTR1        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x110)
#define RX_MAX_CNT1         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x114)
#define RX_CALC_IDX1        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x118)
#define RX_DRX_IDX1         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x
#define PDMA_INFO           (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x200)
#define PDMA_GLO_CFG        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x204)
#define PDMA_RST_IDX        (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x208)
#define PDMA_RST_CFG        (NOE_FRAME_ENGINE_BASE + PDMA_RST_IDX)
#define DLY_INT_CFG         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x20C)
#define FREEQ_THRES         (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x210)
#define INT_STATUS          (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x220) /* FIXME */
#define INT_MASK            (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x228) /* FIXME */
#define PDMA_WRR            (NOE_FRAME_ENGINE_BASE + PDMA_RELATED+0x280)
#define PDMA_SCH_CFG        (PDMA_WRR)

/* TODO: change FE_INT_STATUS->INT_STATUS
 * FE_INT_ENABLE->INT_MASK */
#define MDIO_ACCESS         NOE_FRAME_ENGINE_BASE + 0x00
#define MDIO_CFG            NOE_FRAME_ENGINE_BASE + 0x04
#define FE_DMA_GLO_CFG      NOE_FRAME_ENGINE_BASE + 0x08
#define FE_RST_GLO          NOE_FRAME_ENGINE_BASE + 0x0C
#define FE_INT_STATUS       NOE_FRAME_ENGINE_BASE + 0x10
#define FE_INT_ENABLE       NOE_FRAME_ENGINE_BASE + 0x14
#define FC_DROP_STA         NOE_FRAME_ENGINE_BASE + 0x18
#define FOE_TS_T            NOE_FRAME_ENGINE_BASE + 0x1C

#define PAD_RGMII2_MDIO_CFG NOE_SYSCTL_BASE + 0x58

#define GDMA1_RELATED       0x0500
#define GDMA1_FWD_CFG       (NOE_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x00)
#define GDMA1_SHRP_CFG      (NOE_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x04)
#define GDMA1_MAC_ADRL      (NOE_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x08)
#define GDMA1_MAC_ADRH      (NOE_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x0C)
#define GDMA2_RELATED       0x1500
#define GDMA2_FWD_CFG       (NOE_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x00)
#define GDMA2_SHRP_CFG      (NOE_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x04)
#define GDMA2_MAC_ADRL      (NOE_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x08)
#define GDMA2_MAC_ADRH      (NOE_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x0C)

#define PSE_RELATED         0x0040
#define PSE_FQFC_CFG        (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x00)
#define CDMA_FC_CFG         (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x04)
#define GDMA1_FC_CFG        (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x08)
#define GDMA2_FC_CFG        (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x0C)
#define CDMA_OQ_STA         (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x10)
#define GDMA1_OQ_STA        (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x14)
#define GDMA2_OQ_STA        (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x18)
#define PSE_IQ_STA          (NOE_FRAME_ENGINE_BASE + PSE_RELATED + 0x1C)

#define CDMA_RELATED        0x0080
#define CDMA_CSG_CFG        (NOE_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00)
#define CDMA_SCH_CFG        (NOE_FRAME_ENGINE_BASE + CDMA_RELATED + 0x04)

#define NOE_MAC_PPSC        (NOE_ETH_MAC_BASE+0x0000)
#define NOE_MAC_PIAC        (NOE_ETH_MAC_BASE+0x0004)
#define NOE_MAC_GPC         (NOE_ETH_MAC_BASE+0x0008)
#define NOE_MAC_MISC        (NOE_ETH_MAC_BASE+0x000C)
#define NOE_MAC_P1_MCR      (NOE_ETH_MAC_BASE+0x0100)
#define NOE_MAC_P2_MCR      (NOE_ETH_MAC_BASE+0x0200)
#define NOE_MAC_P1_SR       (NOE_ETH_MAC_BASE+0x0108)
#define NOE_MAC_P2_SR       (NOE_ETH_MAC_BASE+0x0208)

#define BUS_MIU0_BASE_CACHE         CONFIG_SYS_MIU0_CACHE       //Beginning cachable bus address of MIU0
#define BUS_MIU0_BASE_NONCACHE      CONFIG_SYS_MIU0_NON_CACHE   //Beginning non-cachable bus address of MIU0

//-------------------------------------------------------------------------------------------------
//  SYS BANK
//-------------------------------------------------------------------------------------------------
#define NOE_RIU_BANK_ETHMAC_CTRL    (0x1224)
#define NOE_RIU_BANK_GMAC4          (0x121F)
#define NOE_RIU_BANK_ALBANY0        (0x0035)
#define NOE_RIU_BANK_ALBANY1        (0x0036)
#define NOE_RIU_BANK_CHIPTOP        (0x101E)
#define NOE_RIU_BANK_CLKGEN2        (0x100A)
#define NOE_RIU_BANK_PMSLEEP        (0x000E)
#define NOE_RIU_BANK_PMTOP          (0x001E)
#define NOE_RIU_BANK_ANA_MISC_GMAC  (0x110C)
#define NOE_RIU_BANK_CLKGEN0        (0x100B)
#define NOE_RIU_BANK_CLKGEN1        (0x1033)
#define NOE_RIU_BANK_NOE_MISC       (0x1214)
#define NOE_RIU_BANK_PADTOP1        (0x1026)

enum noe_hal_pinmux_err {
    ERR_PINMUX_SUCCESS         = 0,
    ERR_PINMUX_INVALID_EPORT   = -1,
    ERR_PINMUX_INVALID_CONNECT = -2
};

typedef enum {
    E_HAL_NOE_EPORT_INVALID = 0,
    E_HAL_NOE_EPORT_GE1,
    E_HAL_NOE_EPORT_GE2,
    E_HAL_NOE_EPORT_MAX,
} EN_HAL_NOE_EPORT;

typedef enum {
    E_HAL_NOE_GMAC_TO_INVALID = 0,
    E_HAL_NOE_GMAC_TO_INTERNAL_GPHY,
    E_HAL_NOE_GMAC_TO_PM,
    E_HAL_NOE_GMAC_TO_CHIPTOP,
    E_HAL_NOE_GMAC_TO_MAX,
} EN_HAL_NOE_GMAC_CONNECT;

void MHal_NOE_Write_SA1_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5);
void MHal_NOE_Write_SA2_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5);
void MHal_NOE_Write_SA3_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5);
void MHal_NOE_Write_SA4_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5);
void MHal_NOE_Set_PinMux(u32 eport, u32 connect);
void MHal_NOE_Init_System(void);
void MHal_NOE_Switch_MDIO(u32 eport);
void MHal_NOE_Init_MAC_PHY(void);
void MHal_NOE_Software_Reset(void);
void MHal_NOE_Set_Initialized(int value);
int MHal_NOE_Is_Initialized(void);
void MHal_NOE_Init_PinMux_Config(void);
int MHal_NOE_Set_PinMux_Config(u32 eport, u32 connect);
void MHal_NOE_Set_PinMux_Phyaddr(u32 phyaddr);
void MHal_NOE_Set_GDM_Forward_Port(u32 eport);
u32 MHal_NOE_Get_PinMux_Eport(void);
u32 MHal_NOE_Get_PinMux_Connect(void);
u32 MHal_NOE_Get_PinMux_Phyaddr(void);
int MHal_NOE_eWave(int mode, int subitem);

#endif  /* _MHAL_NOE_H_ */

