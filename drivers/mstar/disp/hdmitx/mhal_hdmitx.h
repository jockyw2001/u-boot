////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2011 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
/**
 *  @file MHAL_HDMITX.h
 *  @brief MHAL_HDMITX Driver interface
 */

/**
 * \defgroup HAL_DISP_group  MHAL_HDMITX driver
 * @{
 */
#ifndef __MHAL_HDMITX_H__
#define __MHAL_HDMITX_H__


typedef enum
{
    E_MHAL_HDMITX_CD_8,
    E_MHAL_HDMITX_CD_10,
    E_MHAL_HDMITX_CD_12,
    E_MHAL_HDMITX_CD_16,
    E_MHAL_HDMITX_CD_NUM,
}MHalHdmitxCDType_e;


typedef enum
{
    E_MHAL_HDMITX_COLOR_RGB444,
    E_MHAL_HDMITX_COLOR_YUV444,
    E_MHAL_HDMITX_COLOR_YUV422,
    E_MHAL_HDMITX_COLOR_NUM,
}MHalHdmitxColorType_e;


//=============================================================================
// API
//=============================================================================
#ifndef __MHAL_HDMITX_C__
#define INTERFACE extern
#else
#define INTERFACE
#endif

INTERFACE MS_BOOL MHalHdmitxInit(void);
INTERFACE MS_BOOL MHalHdmitxChangeResolution(MHAL_DISP_DeviceTiming_e enTiming);
INTERFACE MS_BOOL MHalHdmitxChangeColorDepth(MHalHdmitxCDType_e enCD);
INTERFACE MS_BOOL MHalHdmitxChangeOutColor(MHalHdmitxCDType_e enOutputColor);
INTERFACE MS_BOOL MHalHdmitxPowerOff(MS_BOOL bEn);


#undef INTERFACE
#endif //
/** @} */ // end of HAL_DISP_group
