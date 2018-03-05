#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>

#include "cam_os_wrapper.h"
#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiPNL.h"
#include "apiPNL_EX.h"

#include "mhal_disp.h"
#include "mhal_disp_datatype.h"
#include "mhal_hdmitx.h"

//-------------------------------------------------------------------------------------------------
// Define & Macro
//-------------------------------------------------------------------------------------------------
#define SIMMI_DBG(dbglv, _fmt, _args...)             \
    do                                             \
    if(dbglv)                                      \
    {                                              \
            printf(_fmt, ## _args);       \
    }while(0)


#define SIMMI_ALIGN(x,y) ( x & ~(y-1))


typedef struct
{
    MS_U32 u32DevcieId[2];
    MS_U32 u32VideoLayerId[2];
    void *pDeviceCtx[2];
    void *pVideoLayerCtx[2];
    void *pInputPortCtx[2][16];
}DispVerifyCtxConfig_t;

//-------------------------------------------------------------------------------------------------
// Variable
//-------------------------------------------------------------------------------------------------
DispVerifyCtxConfig_t gstModeuleCtx;


//-------------------------------------------------------------------------------------------------
// Prototype
//-------------------------------------------------------------------------------------------------
extern MS_U32 UtopiaInit(void);


//-------------------------------------------------------------------------------------------------
// Funciton
//-------------------------------------------------------------------------------------------------
void Disp_Verify(int argc, char * const argv[])
{
    MHAL_DISP_InputPortAttr_t stInputPortAttrCfg;
    MHAL_DISP_AllocPhyMem_t StAlloc;
    MHAL_DISP_DeviceTimingInfo_t stTimingInfo;
    MHAL_DISP_VideoLayerAttr_t stVidLayerAttr;
    MHAL_DISP_InputPortAttr_t stInportAttr;
    MHAL_DISP_VideoFrameData_t stVideoFrameData;

    StAlloc.alloc = NULL;
    StAlloc.free  = NULL;

    printf("[ouput interface] [pixel format] [image width] [image height] [timing1] [timing2] [timing3] [xwin] [ywin] \n");


    UtopiaInit();

    if(MHAL_DISP_DeviceCreateInstance(&StAlloc, 0, &gstModeuleCtx.pDeviceCtx[0]) == FALSE)
    {
        SIMMI_DBG(1, "%s %d, CreaetInstance fail\n", __FUNCTION__, __LINE__);
        return ;
    }


    // Create VideoLayer Context
    if(MHAL_DISP_VideoLayerCreateInstance(&StAlloc, 0 ,&gstModeuleCtx.pVideoLayerCtx[0]) == FALSE)
    {
        SIMMI_DBG(1, "%s %d, CreateVideoLayer fail\n", __FUNCTION__, __LINE__);
        return;
    }
    gstModeuleCtx.u32VideoLayerId[0] = 0;

    // Create InpoutPort Context
    if(MHAL_DISP_InputPortCreateInstance(&StAlloc, gstModeuleCtx.pVideoLayerCtx[0], 0, &gstModeuleCtx.pInputPortCtx[0][0]) == FALSE)
    {
        SIMMI_DBG(1, "%s %d, CreateInputPort fail\n", __FUNCTION__, __LINE__);;
        return ;
    }


    MHalHdmitxInit();


    MHAL_DISP_DeviceAddOutInterface(gstModeuleCtx.pDeviceCtx[0], MHAL_DISP_INTF_HDMI);

    stTimingInfo.eTimeType = E_MHAL_DISP_OUTPUT_1080P60;
    MHAL_DISP_DeviceSetOutputTiming(gstModeuleCtx.pDeviceCtx[0], MHAL_DISP_INTF_HDMI, &stTimingInfo);
    MHalHdmitxChangeResolution(stTimingInfo.eTimeType);


    MHAL_DISP_VideoLayerBind(gstModeuleCtx.pVideoLayerCtx[0], gstModeuleCtx.pDeviceCtx[0]);

    stInputPortAttrCfg.stDispWin.u16X = 0;
    stInputPortAttrCfg.stDispWin.u16Y = 0;
    stInputPortAttrCfg.stDispWin.u16Width = 720;
    stInputPortAttrCfg.stDispWin.u16Height = 480;
    MHAL_DISP_InputPortSetAttr(gstModeuleCtx.pInputPortCtx[0][0], &stInputPortAttrCfg);


    stVidLayerAttr.stVidLayerSize.u32Width  = 1920;
    stVidLayerAttr.stVidLayerSize.u32Height = 1080;
    stVidLayerAttr.stVidLayerDispWin.u16X   = 0;
    stVidLayerAttr.stVidLayerDispWin.u16Y   = 0;
    stVidLayerAttr.stVidLayerDispWin.u16Width  = 1920;
    stVidLayerAttr.stVidLayerDispWin.u16Height = 1080;
    stVidLayerAttr.ePixFormat = E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_422;
    MHAL_DISP_VideoLayerSetAttr(gstModeuleCtx.pVideoLayerCtx[0], &stVidLayerAttr);


    stInportAttr.stDispWin.u16X       = 0;
    stInportAttr.stDispWin.u16Y       = 0;
    stInportAttr.stDispWin.u16Height  = 1080;
    stInportAttr.stDispWin.u16Width   = 1920;
    MHAL_DISP_InputPortSetAttr(gstModeuleCtx.pInputPortCtx[0][0], &stInportAttr);

    stVideoFrameData.eCompressMode = 0;
    stVideoFrameData.ePixelFormat = E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_422;

    stVideoFrameData.u32Width  = 1920;
    stVideoFrameData.u32Height = 1080;

    stVideoFrameData.au32Stride[0] = 1920 * 2;
    stVideoFrameData.aPhyAddr[0]   = 0x10000000;
    MHAL_DISP_InputPortFlip(gstModeuleCtx.pInputPortCtx[0][0], &stVideoFrameData);
    MHAL_DISP_InputPortEnable(gstModeuleCtx.pInputPortCtx[0][0], TRUE);

    MHAL_DISP_VideoLayerEnable(gstModeuleCtx.pVideoLayerCtx[0], TRUE);
    MHAL_DISP_DeviceEnable(gstModeuleCtx.pDeviceCtx[0], TRUE);
}









