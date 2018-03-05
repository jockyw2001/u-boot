//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
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
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __MHAL_DISP_PNL_TBL_C__
#define __MHAL_DISP_PNL_TBL_C__


#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiPNL.h"
#include "apiPNL_EX.h"


#define LVDS_PN_SWAP_L                  0x00
#define LVDS_PN_SWAP_H                  0x00
#define PANEL_SWAP_LVDS_POL             0x00

//temp
#define PANEL_PDP_10BIT                 1
#define PANEL_SWAP_PORT                 0

PNL_EX_PanelType  tPanelDACOUT640X480P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_640X480P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        96,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        48,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        33,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        96+48,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //2+33,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        640,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        800 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        800,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        800 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        525 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        525,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        525 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (800*525*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (800*525*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (800*525*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x73FBDE,   //   define PANEL_MAX_SET
       0x3555E9,   //   define PANEL_MIN_SET
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT480I_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_480I_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        62,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        60,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        30,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        62+60,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //6+30,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        858 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        858,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        858 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        525 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        525,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        525 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (858*525*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (858*525*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (858*525*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x2CD3FE,
       0x1ED4C4,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT480P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_480P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        62,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        60,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        30,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        62+60,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //6+30,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        858 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        858,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        858 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        525 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        525,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        525 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (858*525*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (858*525*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (858*525*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x73FBDE,
       0x3555E9,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT576I_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_576I_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
            PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power
/*
        126,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        138,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        19,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        126+138,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+19,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)
*/
        64,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        68,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        39,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        64+68,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //5+39,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        576,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        864 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        864,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        864 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        625 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        625,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        625 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (864*625*50)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (864*625*50)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (864*625*50)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x2CE213,
       0x1EDB6D,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT576P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_576P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
            PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        64,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        68,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        39,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        64+68,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //5+39,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        576,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        864 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        864,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        864 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        625 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        625,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        625 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (864*625*50)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (864*625*50)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (864*625*50)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x742B06,
       0x355FE0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT720P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //6+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1650 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1650,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1650 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1650*750*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1650*750*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1650*750*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT720P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1980 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1980,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1980 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1980*750*50)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1980*750*50)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1980*750*50)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080I_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080I_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        15,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+15,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2640 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2640,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2640 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2640*1125*50)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2640*1125*50)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2640*1125*50)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080I_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080I_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        15,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+15,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2640 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2640,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2640 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2640*1125*50)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2640*1125*50)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2640*1125*50)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_24",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2750 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2750,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2750 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2750*1125*24)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2750*1125*24)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2750*1125*24)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT1080P_25[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_25",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2640 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2640,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2640 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2640*1125*25)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2640*1125*25)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2640*1125*25)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1080P_30[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_30",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*30)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*30)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*30)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1920X2205P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1920X2205P_24",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2205,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2750 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2750,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2750 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2750*2250*24)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2750*2250*24)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2750*2250*24)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1280X1470P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1280X1470P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1470,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1980 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1980,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1980 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1500 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1500,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1500 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1980*1500*50)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1980*1500*50)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1980*1500*50)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1280X1470P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1280X1470P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //6+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1470,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1650 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1650,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1650 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1500 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1500,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1500 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1650*1500*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1650*1500*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1650*1500*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT3840X2160P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_3840X2160P_24",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //6+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        3840,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5500 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5500,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5500 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5500*2250*24)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5500*2250*24)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5500*2250*24)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT3840X2160P_25[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_3840X2160P_25",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        3840,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5280 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5280,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5280 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5280*2250*25)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5280*2250*25)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5280*2250*25)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT3840X2160P_30[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_3840X2160P_30",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        3840,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        4400 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        4400,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        4400 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (4400*2250*30)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (4400*2250*30)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (4400*2250*30)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT3840X2160P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_3840X2160P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        3840,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5280 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5280,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5280 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5280*2250*50)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5280*2250*50)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5280*2250*50)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT3840X2160P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_3840X2160P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        3840,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        4400 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        4400,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        4400 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (4400*2250*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (4400*2250*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (4400*2250*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT4096X2160P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_4096X2160P_24",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 296,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+296,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        4096,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5500 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5500,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5500 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5500*2250*24)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5500*2250*24)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5500*2250*24)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT4096X2160P_25[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_4096X2160P_25",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        128,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+128,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        4096,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5280 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5280,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5280 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5280*2250*25)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5280*2250*25)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5280*2250*25)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT4096X2160P_30[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_4096X2160P_30",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        128,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+128,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        4096,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        4400 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        4400,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        4400 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (4400*2250*30)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (4400*2250*30)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (4400*2250*30)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT4096X2160P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_4096X2160P_50",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        128,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+128,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        4096,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        5280 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        5280,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        5280 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (5280*2250*50)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (5280*2250*50)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (5280*2250*50)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT4096X2160P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_4096X2160P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        88,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        128,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        10,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        72,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        88+128,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //10+72,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        4096,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        2160,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        4400 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        4400,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        4400 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2250 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        2250,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        2250 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        158, //overflowed, original is (4400*2250*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        148, //overflowed, original is (4400*2250*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        138, //overflowed, original is (4400*2250*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1AAB29,
       0x1A94BF,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT1600X1200P_60[] =
{
    {
        "DACOUT_1600X1200P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        192,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255, //overflowed, original is 304, //This setting is useless, the real HBP setting is in Hal_dac_tbl.c   //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH,

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        46,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        (192+304),      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+46,            //(3+46),       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1600,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1200,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2160 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2160,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2160 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1250 + 10,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1250,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1250 - 10,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2160*1250*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2160*1250*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2160*1250*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};
PNL_EX_PanelType  tPanelDACOUT1440X900P_60[] =
{
    { //Dac Out for U3 bringup
        "DACOUT_1440X90P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        152,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        232,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        25,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        (152 + 232),      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+25,            //6 + 25,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1440,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        900,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1904 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1904,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1904 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        934 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        934,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        934 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1904*934*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1904*934*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1904*934*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};
PNL_EX_PanelType  tPanelDACOUT1280X1024P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1280X1024P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        112,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        248,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        38,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        (112 + 248),      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+38,            // 3 + 38,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1024,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1688 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1688,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1688 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1066 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1066,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1066 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1688*1066*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1688*1066*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1688*1066*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_EX_CHG_VTOTAL,
    },
};
PNL_EX_PanelType  tPanelDACOUT1024X768P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1024X768P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        136,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        160,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        29,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        (136 + 160),      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+29,            // 6 + 29 ,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1024,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        768,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1344 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1344,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1344 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        806 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        806,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        806 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1344*806*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1344*806*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1344*806*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT720P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_24",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        3300 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        3300,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        3300 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (3300*750*24)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (3300*750*24)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (3300*750*24)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT720P_25[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_25",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        3960 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        3960,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        3960 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (3960*750*25)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (3960*750*25)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (3960*750*25)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelDACOUT720P_30[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_30",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,            //5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        3300 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        3300,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        3300 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (3300*750*30)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (3300*750*30)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (3300*750*30)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_EX_CHG_VTOTAL,
    },
};

PNL_EX_PanelType  tPanelSXGA[] =
{
    {
        "AU17EN05",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        1,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        1,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;        //PANEL_EVEN_DATA_CURRENT    // even data current

        10,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        250,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        100,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        24,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        38,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+24,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH-1)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1024,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1800,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1688,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1664,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2047,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1066,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL
        1035,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        140,        //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        108,        //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        100,        //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,        //m_ucDimmingCtl
        0xFF,        //m_ucMaxPWMVal;
        0x50,         //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_4_3,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x277F44,
       0x1C3655,
    },
};//


PNL_EX_PanelType  tPanelWXGA[] =
{
    {//Auo20" T200XW02V0
        "AU20_T200XW02",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        20,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        40,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        4,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        34,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        20+40,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1366,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        768,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1722ul,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1560ul,//1430,  //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1414ul,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        822,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        806ul,      //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        789,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        88, //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        76, //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        65, //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1E61E6,
       0x167109,
    },
};//!<Panel Data ?ú?å ±¸?¶?¼.(Multi Panel ´ë???» ?§?? »ç¿ë)

PNL_EX_PanelType  tPanelWXGAPLUS[] =
{
    {//CMO 19" M190A1-L02, AU19_M190EW01

        "M190A1",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        1,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        500,            //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        150,            //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        104,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        24,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        6,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        104+24,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,           //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1440,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        900,            //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1920,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1760,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1520,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1050,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        950,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        910,            //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        112,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        100,     //DWORD m_dwPanelDCLK;          //PANEL_DCLK
        80,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0000,//0x0010,        //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x0000,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x68,       //m_ucDimmingCtl
        0xDD,                //m_ucMaxPWMVal;
        0x20,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x315F15,
       0x2343EB,
    },
};

PNL_EX_PanelType  tPanelWSXGA[] =
{
    {//CMO22" M220Z1-L01
        "M220Z1",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        1,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        500,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        120,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        16,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        40,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        8,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        16+40,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1680,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1050,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2000,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1840,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1780,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1195,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1080,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1060,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (150),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (119),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (110),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;            //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;            //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x50,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x23E80F,
       0x1A54D8,
    },
};

PNL_EX_PanelType  tPanelFullHD[] =
{
    {//Pnl_CMO216H1_L01_HD, Pnl_AU37_T370HW01_HD
        "M220Z1",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        1,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
#if 0 // ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A_HX))
    0,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE
#else
    1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE
#endif

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
#if 0 // ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A_HX))
    E_PNL_EX_TI_10BIT_MODE,               //8bit ti bit mode
#else
    E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
#endif
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x181568,
    },
};

PNL_EX_PanelType  tPanel_SEC_LE32A_FULLHD[] =
{
    {
        "SEC LE32A",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        0,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;          //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //0x0010,       //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,   //0x87,     //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,    //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x181568,
    },
};

PNL_EX_PanelType  tPanel_CMO_260J2_WUXGA[] =
{
    {
        "CMO260J2_WUXGA",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT
        1,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        600,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        300,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        26,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        0,          //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1200,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2159,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2141,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2060,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1245,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1235,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1209,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (170),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (158),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        100,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x015E,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x7D,       //m_ucDimmingCtl

        0xFF,        //m_ucMaxPWMVal;
        0x00,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x173BCE,
    },
};


PNL_EX_PanelType  tPanelDACOUT800X600P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_800X600P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        128,        //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        88,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        4,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        23,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        128+88,     //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        4+23,          //4+23,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        800,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        600,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1156,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1056,       //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        956,        //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        728,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        628,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        528,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        50,         //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        40,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        30,         //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x147DCB,   //   define PANEL_MAX_SET
       0x0CC923,   //   define PANEL_MIN_SET
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT1366X768P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1366X768P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        143,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        215,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        24,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        143+215,     //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+24,           //6+29,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1366,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        768,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1892,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1792,       //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1692,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        898,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        798,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        698,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

       101,         //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        85,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        70,         //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x26EF02,   //   define PANEL_MAX_SET
       0x1FF6D7,   //   define PANEL_MIN_SET
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT1680X1050P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1680X1050P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        176,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        255,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH  280 but, overflow for u8

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        30,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        176+280,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+30,           //6+30,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1680,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1050,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2340,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2240,       //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2140,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1139,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1089,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1039,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        159,        //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        146,        //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        133,        //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x17BCDD,   //   define PANEL_MAX_SET
       0x177CDD,   //   define PANEL_MIN_SET
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT1280X800P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1280X1024P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        128,         //PANEL_HSYNC_WIDTH
        200,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        22,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        128+200,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+22,           //6+22,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,        //WORD m_wPanelWidth;               //PANEL_WIDTH
        800,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1780,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1680,       //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1580,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        931,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        831,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        731,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        94,        //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        84,        //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        74,         //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       1,          //   double clock
       0x16E967,   //   define PANEL_MAX_SET
       0x1302B8,   //   define PANEL_MIN_SET
       E_PNL_EX_CHG_VTOTAL,
    },
};


PNL_EX_PanelType  tPanelDACOUT1920X1200P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1920X1200P_60",//m_pPanelName
#if !defined (__aarch64__)
        0L,         //u32AlignmentDummy0
#endif
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        E_PNL_EX_LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        26,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+26,            //5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1200,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2180,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2080,       //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1980,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1335,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1235,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1135,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        164,        //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        154,        //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        144,        //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_EX_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       E_PNL_EX_TI_8BIT_MODE,               //8bit ti bit mode
       E_PNL_EX_OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D99CA,
       0x1A00B5,
       E_PNL_EX_CHG_VTOTAL,
    },
};

#define E_MHAL_DISP_PNL_RES_MAX_NUM 47
PNL_EX_PanelType * tPanelIndexTbl[E_MHAL_DISP_PNL_RES_MAX_NUM] =
{
    tPanel_SEC_LE32A_FULLHD, // LE32A_FULLHD
    tPanelSXGA,
    tPanelWXGA,
    tPanelWXGAPLUS,     // 1440*900  //CHR_060912_1 Resolution
    tPanelWSXGA,        // 1680*1050
    tPanelFullHD,       // 1920x1080
    tPanelDACOUT480I_60,    // DAC 720*480
    tPanelDACOUT480P_60,    // DAC 720*480
    tPanelDACOUT576I_50,    // DAC 720*576
    tPanelDACOUT576P_50,    // DAC 720*576
    tPanelDACOUT720P_50,    // DAC 1280*720
    tPanelDACOUT720P_60,    // DAC 1280*720
    tPanelDACOUT1080I_50,   // DAC 1920*1080I 50MHz
    tPanelDACOUT1080I_60,   // DAC 1920*1080I 60MHz
    tPanelDACOUT1080P_24,   // DAC 1920*1080P 50MHz
    tPanelDACOUT1080P_25,   // DAC 1920*1080P 50MHz
    tPanelDACOUT1080P_30,   // DAC 1920*1080P 50MHz
    tPanelDACOUT1080P_50,   // DAC 1920*1080P 50MHz
    tPanelDACOUT1080P_60,   // DAC 1920*1080P 60MHz
    tPanelDACOUT640X480P_60, // DAC 640*480P 60MHz
    tPanelDACOUT1920X2205P_24, // DAC 1920*2205P 24MHz
    tPanelDACOUT1280X1470P_50, // DAC 1280*1470P 50MHz
    tPanelDACOUT1280X1470P_60, // DAC 1280*1470P 60MHz
    tPanelDACOUT3840X2160P_24, // DAC 3840*2160P 24MHz
    tPanelDACOUT3840X2160P_25, // DAC 3840*2160P 25MHz
    tPanelDACOUT3840X2160P_30, // DAC 3840*2160P 30MHz
    tPanelDACOUT3840X2160P_50, // DAC 3840*2160P 50MHz
    tPanelDACOUT3840X2160P_60, // DAC 3840*2160P 60MHz
    tPanelDACOUT4096X2160P_24, // DAC 4096*2160P 24MHz
    tPanelDACOUT4096X2160P_25, // DAC 4096*2160P 25MHz
    tPanelDACOUT4096X2160P_30, // DAC 4096*2160P 30MHz
    tPanelDACOUT4096X2160P_50, // DAC 4096*2160P 50MHz
    tPanelDACOUT4096X2160P_60, // DAC 4096*2160P 60MHz
    tPanelDACOUT1600X1200P_60, // DAC 1600*1200P 60MHz
    tPanelDACOUT1440X900P_60, // DAC 1440*900P 60MHz
    tPanelDACOUT1280X1024P_60, // DAC 1280*1024P 60MHz
    tPanelDACOUT1024X768P_60, // DAC 1024*768P 60MHz
    tPanelDACOUT720P_24,        // DAC 1280*720P 24MHz
    tPanelDACOUT720P_25,        // DAC 1280*720P 25MHz
    tPanelDACOUT720P_30,        // DAC 1280*720P 30MHz
    tPanelDACOUT800X600P_60,    // DAC 800x480P   60MHz
    tPanelDACOUT1280X800P_60,   // DAC 1280x800P  60MHz
    tPanelDACOUT1366X768P_60,   // DAC 1366x768P  60MHz
    tPanelDACOUT1680X1050P_60,  // DAC 1680x1050P 60MHz
    tPanelDACOUT1920X1200P_60,  // DAC 1920x1200P 60MHz
    tPanel_CMO_260J2_WUXGA, // CMO_260J2_WUXGA
};



#endif
