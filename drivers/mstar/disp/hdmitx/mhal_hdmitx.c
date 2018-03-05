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

#define __MHAL_HDMITX_C__
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX_KERNEL)
#include <linux/pfn.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>          /* seems do not need this */
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/string.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>

#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/wait.h>




#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>

#elif defined(MSOS_TYPE_NOS)

#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>

#endif

#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiPNL.h"
#include "apiPNL_EX.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_PCMonitor_EX.h"
#include "drvPQ.h"
#include "drvPQ_cus.h"
#include "apiXC_Ace_EX.h"
#include "apiHDMITx.h"
#include "apiDAC.h"
#include "drvTVEncoder.h"

#include "drv_scl_mgwin_io_st.h"
#include "drv_scl_mgwin_verchk.h"
#include "drv_scl_mgwin_io_wrapper.h"

#include "mhal_disp_datatype.h"
#include "mhal_disp.h"
#include "mhal_hdmitx.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX_KERNEL)

#define MHAL_DISP_DBG(lvl, _fmt, _args...)                          \
    do                                                              \
    {                                                               \
        if( lvl & gu32MHAL_DISP_DbgLevel)                             \
        {                                                           \
            printk(KERN_INFO "----- [%s,%5d] ----- ",__FUNCTION__,__LINE__);   \
            printk(KERN_INFO _fmt, ## _args);                      \
        }                                                           \
    } while(0);

#elif defined(MSOS_TYPE_NOS)

#define MHAL_DISP_DBG(lvl, _fmt, _args...)                          \
    do                                                              \
    {                                                               \
        if( lvl & gu32MHAL_DISP_DbgLevel)                             \
        {                                                           \
            printf("----- [%s,%5d] ----- ",__FUNCTION__,__LINE__);   \
            printf(_fmt, ## _args);                      \
        }                                                           \
    } while(0);

#else

#define MHAL_DISP_DBG(lvl, _fmt, _args...)

#endif

#define MHAL_DISP_XC_XTAL_CLOCK_HZ                  (12000000UL)    /* Temp define */

#define FILL_UTPA_DEVICE_STRUCT(id, ver, val)   \
    do                                          \
    {                                           \
        id=val;                                 \
        ver=0;                                  \
    }while(0)



#define PARSING_MHAL_DISP_OUTPUT_TIMING(x)  (   x == E_MHAL_DISP_OUTPUT_PAL           ?  "PAL          " :\
                                                x == E_MHAL_DISP_OUTPUT_NTSC          ?  "NTSC         " :\
                                                x == E_MHAL_DISP_OUTPUT_960H_PAL      ?  "960H_PAL     " :\
                                                x == E_MHAL_DISP_OUTPUT_960H_NTSC     ?  "960H_NTSC    " :\
                                                x == E_MHAL_DISP_OUTPUT_1080P24       ?  "1080P24      " :\
                                                x == E_MHAL_DISP_OUTPUT_1080P25       ?  "1080P25      " :\
                                                x == E_MHAL_DISP_OUTPUT_1080P30       ?  "1080P30      " :\
                                                x == E_MHAL_DISP_OUTPUT_720P50        ?  "720P50       " :\
                                                x == E_MHAL_DISP_OUTPUT_720P60        ?  "720P60       " :\
                                                x == E_MHAL_DISP_OUTPUT_1080I50       ?  "1080I50      " :\
                                                x == E_MHAL_DISP_OUTPUT_1080I60       ?  "1080I60      " :\
                                                x == E_MHAL_DISP_OUTPUT_1080P50       ?  "1080P50      " :\
                                                x == E_MHAL_DISP_OUTPUT_1080P60       ?  "1080P60      " :\
                                                x == E_MHAL_DISP_OUTPUT_576P50        ?  "576P50       " :\
                                                x == E_MHAL_DISP_OUTPUT_480P60        ?  "480P60       " :\
                                                x == E_MHAL_DISP_OUTPUT_640x480_60    ?  "640x480_60   " :\
                                                x == E_MHAL_DISP_OUTPUT_800x600_60    ?  "800x600_60   " :\
                                                x == E_MHAL_DISP_OUTPUT_1024x768_60   ?  "1024x768_60  " :\
                                                x == E_MHAL_DISP_OUTPUT_1280x1024_60  ?  "1280x1024_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_1366x768_60   ?  "1366x768_60  " :\
                                                x == E_MHAL_DISP_OUTPUT_1440x900_60   ?  "1440x900_60  " :\
                                                x == E_MHAL_DISP_OUTPUT_1280x800_60   ?  "1280x800_60  " :\
                                                x == E_MHAL_DISP_OUTPUT_1680x1050_60  ?  "1680x1050_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_1920x2160_30  ?  "1920x2160_30 " :\
                                                x == E_MHAL_DISP_OUTPUT_1600x1200_60  ?  "1600x1200_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_1920x1200_60  ?  "1920x1200_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_2560x1440_30  ?  "2560x1440_30 " :\
                                                x == E_MHAL_DISP_OUTPUT_2560x1600_60  ?  "2560x1600_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_3840x2160_30  ?  "3840x2160_30 " :\
                                                x == E_MHAL_DISP_OUTPUT_3840x2160_60  ?  "3840x2160_60 " :\
                                                x == E_MHAL_DISP_OUTPUT_USER          ?  "USER         " :\
                                                                                         "UNKNOWN")


//-------------------------------------------------------------------------------------------------
//  Local enum
//-------------------------------------------------------------------------------------------------
extern MS_U32  gu32MHAL_DISP_DbgLevel;

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MHAL_DISP_DBG_DEVICE      = 0x00000001,
    E_MHAL_DISP_DBG_VID_LAYER   = 0x00000002,
    E_MHAL_DISP_DBG_INPUT_PORT  = 0x00000004,
    E_MHAL_DISP_DBG_TYPE_TRANS  = 0x00000008,
    E_MHAL_DISP_DBG_XC          = 0x00000010,
    E_MHAL_DISP_DBG_FUNC        = 0x00000020,
    E_MHAL_DISP_DBG_ERR         = 0x10000000,
    E_MHAL_DISP_DBG_ALL         = 0xFFFFFFFF,
}MHalDispDebugLevelType_e;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MHAL_DISP_DeviceTiming_e eHdmiTxTiming    = E_MHAL_DISP_OUTPUT_480P60;
MHalHdmitxCDType_e       enHdmiTxCD        = E_MHAL_HDMITX_CD_8;
MHalHdmitxColorType_e    enHdmiTxOutColor = E_MHAL_HDMITX_COLOR_YUV444;



//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#define HDMITX_ANALOG_TUNING_SD         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_HD         {0x01, 0x00, 0x00, 0x00, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_DEEP_HD    {0x00, 0x00, 0x00, 0x00, 0x00, 0x07}



//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
HDMITX_VIDEO_TIMING _MHalHdmitxTrans2VideoTiming(MHAL_DISP_DeviceTiming_e enTiming)
{
    HDMITX_VIDEO_TIMING enVideoTimiming;


    enVideoTimiming =   (enTiming == E_MHAL_DISP_OUTPUT_1080P24     ) ?  HDMITX_RES_1920x1080p_24Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080P25     ) ?  HDMITX_RES_1920x1080p_25Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080P30	    ) ?  HDMITX_RES_1920x1080p_30Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_720P50      ) ?  HDMITX_RES_1280x720p_50Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_720P60      ) ?  HDMITX_RES_1280x720p_60Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080I50     ) ?  HDMITX_RES_1920x1080i_50Hz  :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080I60     ) ?  HDMITX_RES_1920x1080i_60Hz  :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080P50     ) ?  HDMITX_RES_1920x1080p_50Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1080P60     ) ?  HDMITX_RES_1920x1080p_60Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_576P50      ) ?  HDMITX_RES_720x480p :
                        (enTiming == E_MHAL_DISP_OUTPUT_480P60      ) ?  HDMITX_RES_720x576p :
                        (enTiming == E_MHAL_DISP_OUTPUT_3840x2160_30) ?  HDMITX_RES_3840x2160p_30Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1024x768_60)  ?  HDMITX_RES_1024x768p_60Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1440x900_60)  ?  HDMITX_RES_1440x900p_60Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1600x1200_60) ?  HDMITX_RES_1600x1200p_60Hz :
                        (enTiming == E_MHAL_DISP_OUTPUT_1280x1024_60) ?  HDMITX_RES_1280x1024p_60Hz :
                                                                         HDMITX_RES_MAX;

    return enVideoTimiming;
}

HDMITX_VIDEO_COLORDEPTH_VAL _MHalHdmitxTransColorDepth(MHalHdmitxCDType_e enCD)
{
    HDMITX_VIDEO_COLORDEPTH_VAL enColorDepth;
    enColorDepth = enCD == E_MHAL_HDMITX_CD_8  ? HDMITX_VIDEO_CD_24Bits :
                   enCD == E_MHAL_HDMITX_CD_10 ? HDMITX_VIDEO_CD_30Bits :
                   enCD == E_MHAL_HDMITX_CD_12 ? HDMITX_VIDEO_CD_36Bits :
                   enCD == E_MHAL_HDMITX_CD_16 ? HDMITX_VIDEO_CD_48Bits :
                                                 HDMITX_VIDEO_CD_24Bits;
    return enColorDepth;
}

MS_U8 _MHalHdmitxTransDacHdmitxDivider(MHalHdmitxCDType_e enCD)
{
    MS_U8 u8DacHdmitxDivider;

    u8DacHdmitxDivider = enCD == E_MHAL_HDMITX_CD_8  ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_8BIT_Divider :
                         enCD == E_MHAL_HDMITX_CD_10 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_10BIT_Divider :
                         enCD == E_MHAL_HDMITX_CD_12 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_12BIT_Divider :
                         enCD == E_MHAL_HDMITX_CD_16 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_16BIT_Divider :
                                                        E_DAC_DUMP_TABTYPE_INIT_HDMITX_8BIT_Divider;
    return u8DacHdmitxDivider;
}


MS_U8 _MHalHdmitxTransDacHdmitx(MHalHdmitxCDType_e enCD)
{
    MS_U8 u8DacHdmitx;

    u8DacHdmitx = enCD == E_MHAL_HDMITX_CD_8  ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_8BIT :
                  enCD == E_MHAL_HDMITX_CD_10 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_10BIT :
                  enCD == E_MHAL_HDMITX_CD_12 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_12BIT :
                  enCD == E_MHAL_HDMITX_CD_16 ?  E_DAC_DUMP_TABTYPE_INIT_HDMITX_16BIT :
                                                 E_DAC_DUMP_TABTYPE_INIT_HDMITX_8BIT;
    return u8DacHdmitx;
}

HDMITX_VIDEO_COLOR_FORMAT _MHalHdmiTxTransColor(MHalHdmitxColorType_e enColor)
{
    HDMITX_VIDEO_COLOR_FORMAT enColorFmt;

    enColorFmt = enColor == E_MHAL_HDMITX_COLOR_RGB444 ? HDMITX_VIDEO_COLOR_RGB444 :
                 enColor == E_MHAL_HDMITX_COLOR_YUV444 ? HDMITX_VIDEO_COLOR_YUV444 :
                 enColor == E_MHAL_HDMITX_COLOR_YUV422 ? HDMITX_VIDEO_COLOR_YUV422 :
                                                         HDMITX_VIDEO_COLOR_RGB444;


    return enColorFmt;
}

void _MHalHdmitxUpdate(void)
{
    MApi_HDMITx_SetHDMITxMode(HDMITX_HDMI);

    MApi_DAC_DumpTable(NULL, _MHalHdmitxTransDacHdmitxDivider(enHdmiTxCD));
    MApi_DAC_DumpTable(NULL, _MHalHdmitxTransDacHdmitx(enHdmiTxCD));
    MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_YUV444, _MHalHdmiTxTransColor(enHdmiTxOutColor));
    MApi_HDMITx_SetHDMITxMode_CD(HDMITX_HDMI, _MHalHdmitxTransColorDepth(enHdmiTxCD));
    MApi_HDMITx_SetVideoOutputTiming(eHdmiTxTiming);
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);

    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
    MApi_HDMITx_SetAudioSourceFormat(HDMITX_AUDIO_FORMAT_PCM);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioOnOff(TRUE);

    MApi_HDMITx_Exhibit();
#if defined(MSOS_TYPE_LINUX_KERNEL)
    msleep(1000);
#endif
}

MS_BOOL MHalHdmitxInit(void)
{
    //HDMITX_ANALOG_TUNING stDeepHDInfo = HDMITX_ANALOG_TUNING_DEEP_HD;
    HDMITX_ANALOG_TUNING stHDInfo = HDMITX_ANALOG_TUNING_HD;
    //HDMITX_ANALOG_TUNING stSDInfo = HDMITX_ANALOG_TUNING_SD;


    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "HDMITx Init\n");

#if !defined(CONFIG_MBOOT)
    MApi_HDMITx_SetHPDGpioPin(0x19);
#endif

    MApi_HDMITx_Init();

    //Always load HDCP key and set HDCP fail control for HDMI_SetTxOutputMode()
    //HDCP Configuration, to pass simply/quantum data HDCP test, we defaultly choose Bluescreen if HDCP failed
    MApi_HDMITx_UnHDCPRxControl(E_UNHDCPRX_BLUE_SCREEN);
    MApi_HDMITx_HDCPRxFailControl(E_UNHDCPRX_BLUE_SCREEN);

    MApi_HDMITx_SetHDCPOnOff(FALSE);
    MApi_HDMITx_HDCP_StartAuth(FALSE);

    MApi_HDMITx_AnalogTuning(&stHDInfo);

    //MApi_HDMITx_SetDbgLevel(0xFF);
    //MApi_HDMITx_Exhibit();

    return TRUE;
}

MS_BOOL MHalHdmitxChangeResolution(MHAL_DISP_DeviceTiming_e enTiming)
{
    HDMITX_VIDEO_TIMING enVideoTiming = _MHalHdmitxTrans2VideoTiming(enTiming);
    MS_BOOL bRet = TRUE;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Time=%s\n", PARSING_MHAL_DISP_OUTPUT_TIMING(enTiming));


    if(enVideoTiming == HDMITX_RES_MAX)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Time is out of range\n");
        bRet = FALSE;
    }
    else
    {
        eHdmiTxTiming = enVideoTiming;
        _MHalHdmitxUpdate();
        bRet = TRUE;
    }

    return bRet;
}


MS_BOOL MHalHdmitxChangeColorDepth(MHalHdmitxCDType_e enCD)
{
    enHdmiTxCD = enCD;
    return TRUE;
}

MS_BOOL MHalHdmitxChangeOutColor(MHalHdmitxCDType_e enOutputColor)
{
    enHdmiTxOutColor = enOutputColor;
    return TRUE;
}

MS_BOOL MHalHdmitxPowerOff(MS_BOOL bEn)
{
    MApi_HDMITx_SetTMDSOnOff(bEn);
    MApi_XC_Write2ByteMask(0x173072, 0xFFFF, 0xFFFF); //0x39
    MApi_XC_Write2ByteMask(0x173074, 0xFFFF, 0xFFFF); //0x3A

    return TRUE;
}

