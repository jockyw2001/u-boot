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
 *  @file HAL_DISP.h
 *  @brief HAL_DISP Driver interface
 */

/**
 * \defgroup HAL_DISP_group  HAL_DISP driver
 * @{
 */
#ifndef __MHAL_DISP_H__
#define __MHAL_DISP_H__
#include <linux/string.h>
#include "mhal_disp_datatype.h"
//=============================================================================
// API
//=============================================================================
#ifndef __MHAL_DISP_C__
#define INTERFACE extern
#else
#define INTERFACE
#endif

// Driver Physical memory: MI

//----------------------------------- Device -----------------------------------------------

INTERFACE MS_BOOL MHAL_DISP_GetDevIrq(void *pDevCtx, MS_U32* pu32DevIrq);
INTERFACE MS_BOOL MHAL_DISP_EnableDevIrq(void *pDevCtx, MS_U32 u32DevIrq, MS_BOOL bEnable);
INTERFACE MS_BOOL MHAL_DISP_ClearDevInterrupt(void *pDevCtx, void* pData);
INTERFACE MS_BOOL MHAL_DISP_GetDevIrqFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag);

INTERFACE MS_BOOL MHAL_DISP_InitPanelConfig(MHAL_DISP_PanelConfig_t* pstPanelConfig, MS_U8 u8Size);

INTERFACE MS_BOOL MHAL_DISP_InitMmapConfig(MHAL_DISP_MmapType_e eMemType, MHAL_DISP_MmapInfo_t* pstMmapInfo);

// Device: Create/Distroy (scaler0/1)
INTERFACE MS_BOOL MHAL_DISP_DeviceCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32DeviceId, void **pDevCtx);
INTERFACE MS_BOOL MHAL_DISP_DeviceDestroyInstance(void *pDevCtx);

// Device: Enbale/Disable
INTERFACE MS_BOOL MHAL_DISP_DeviceEnable(void *pDevCtx, const MS_BOOL bEnable);

// Device: Scaler0 attach/deattach Scaler1
INTERFACE MS_BOOL MHAL_DISP_DeviceAttach(const void *pSrcDevCtx, const void *pDstDevCtx);
INTERFACE MS_BOOL MHAL_DISP_DeviceDetach(const void *pSrcDevCtx, const void *pDstDevCtx);

// Device: Set/Get BackGround Color in RGB format
INTERFACE MS_BOOL MHAL_DISP_DeviceSetBackGroundColor(void *pDevCtx, const MS_U32 u32BgColor);

INTERFACE MS_BOOL MHAL_DISP_DeviceAddOutInterface(void *pDevCtx, const MS_U32 u32Interface);

// Device: Set output Timing
INTERFACE MS_BOOL MHAL_DISP_DeviceSetOutputTiming(void *pDevCtx, const MS_U32 u32Interface, const MHAL_DISP_DeviceTimingInfo_t *pstTimingInfo);

// Device: Set VGA Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetVgaParam(void *pDevCtx, const MHAL_DISP_VgaParam_t *pstVgaInfo);

// Device: Set HDMI Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetHdmiParam(void *pDevCtx, const MHAL_DISP_HdmiParam_t *pstHdmiInfo);

// Device: Set CVBS Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetCvbsParam(void *pDevCtx, const MHAL_DISP_CvbsParam_t *pstCvbsInfo);

//----------------------------------- Video Layer --------------------------------------------
// Video Layer: Create/Destroy
INTERFACE MS_BOOL MHAL_DISP_VideoLayerCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32LayerId, void **pVidLayerCtx);
INTERFACE MS_BOOL MHAL_DISP_VideoLayerDestoryInstance(void *pVidLayerCtx);

// Video Layer: Enbale/Disable
INTERFACE MS_BOOL MHAL_DISP_VideoLayerEnable(void *pVidLayerCtx, const MS_BOOL bEnable);

// Video Layer: Binder/Unbinder device
INTERFACE MS_BOOL MHAL_DISP_VideoLayerBind(void *pVidLayerCtx, void *pDevCtx);
INTERFACE MS_BOOL MHAL_DISP_VideoLayerUnBind(void *pVidLayerCtx, void *pDevCtx);

// Video Layer: Set/Get Attribute
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetAttr(void *pVidLayerCtx, const MHAL_DISP_VideoLayerAttr_t *pstAttr);

// Video layer: Set/Get compress
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetCompress(void *pVidLayerCtx, const MHAL_DISP_CompressAttr_t* pstCompressAttr);

// Video layer: Set/Get display priority
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetPriority(void *pVidLayerCtx, const MS_U32 u32Priority);

// SW Trigger MGWIN switch double buffer
INTERFACE MS_BOOL MHAL_DISP_VideoLayerBufferFire(void *pVidLayerCtx);

// SW check MGWIN double buffer already run
INTERFACE MS_BOOL MHAL_DISP_VideoLayerCheckBufferFired(void *pVidLayerCtx);

//----------------------------------- Input Port ---------------------------------------------
// Input port: Create/Destroy
INTERFACE MS_BOOL MHAL_DISP_InputPortCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, void *pVidLayerCtx, const MS_U32 u32PortId, void **pCtx);

INTERFACE MS_BOOL MHAL_DISP_InputPortDestroyInstance(const void *pCtx);

// Input port: Enbale/Disbale
INTERFACE MS_BOOL MHAL_DISP_InputPortEnable(void *pInputPortCtx, const MS_BOOL bEnable);

// Input port: Enbale/Disbale
INTERFACE MS_BOOL MHAL_DISP_InputPortSetAttr(void *pInputPortCtx, const MHAL_DISP_InputPortAttr_t *pstAttr);

// Input port: Show
INTERFACE MS_BOOL MHAL_DISP_InputPortShow(void *pInputPortCtx);

// Input port: Hide
INTERFACE MS_BOOL MHAL_DISP_InputPortHide(void *pInputPortCtx);

// Input port: Attr batch process, All Channel setting enter batch process.
INTERFACE MS_BOOL MHAL_DISP_InputPortAttrBegin(void *pVidLayerCtx);
INTERFACE MS_BOOL MHAL_DISP_InputPortAttrEnd(void *pVidLayerCtx);

// Input port: Flip
INTERFACE MS_BOOL MHAL_DISP_InputPortFlip(void *pInputPortCtx, MHAL_DISP_VideoFrameData_t *pstVideoFrameBuffer);

INTERFACE MS_BOOL MHAL_DISP_DbgLevel(void *p);

#undef INTERFACE

#endif //
/** @} */ // end of HAL_DISP_group
