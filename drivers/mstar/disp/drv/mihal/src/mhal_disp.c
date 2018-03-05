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

#define __MHAL_DISP_C__
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
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

#include "cam_os_wrapper.h"
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
#include "drvGPIO.h"

#include "drv_scl_mgwin_io_st.h"
#include "drv_scl_mgwin_verchk.h"
#include "drv_scl_mgwin_io_wrapper.h"

#include "mhal_disp_datatype.h"
#include "mhal_disp.h"
#include "mhal_pql.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define MHAL_FPGA_EN                    0

#define XC_FBL_EN                       1

#if XC_FBL_EN
#define XC_SC1_MAIN_DNR_BUF_SIZE        0x00000000
#define XC_SC2_MAIN_DNR_BUF_SIZE        0x00000000
#else
#define XC_SC1_MAIN_DNR_BUF_SIZE        0 //0x03f48000
#define XC_SC2_MAIN_DNR_BUF_SIZE        0x008f7000
#endif

#define VE_BUF_SIZE                     0x870000 //(720/3*288*32bytes*4field)
#define VE_BUF_SHIFT_OFFSET             (8*32)
#define VE_BUF_ADJUST_OFFSET            8

#if defined(CONFIG_MBOOT)
#define ENABLE_PQL                      0
#define ENABLE_UTPA_PQ                  0
#define ENABLE_UTPA_ACE                 0
#define ENABLE_UTPA_VE                  0
#define ENABLE_UTPA_XC                  1
#else
#define ENABLE_PQL                      1
#define ENABLE_UTPA_PQ                  1
#define ENABLE_UTPA_ACE                 1
#define ENABLE_UTPA_VE                  1
#define ENABLE_UTPA_XC                  1
#endif

#define SC1_OUTPUT_COLOR                DEVICE_OUTPUT_COLOR_YUV
#define DEVICE_OUTPUT_COLOR_RGB         0
#define DEVICE_OUTPUT_COLOR_YUV         1


#define MHAL_DISP_VIDEOLAYER_MASTER     0
#define MHAL_DISP_VIDEOLAYER_SECONDARY  1

#define MHAL_DISP_VIDEOLAYER_BIND_MAX   2
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_NOS)

#define MHAL_DISP_DBG(lvl, _fmt, _args...)                          \
    do                                                              \
    {                                                               \
        if( lvl & gu32MHAL_DISP_DbgLevel)                             \
        {                                                           \
            if( lvl & E_MHAL_DISP_DBG_ERR)                          \
            {                                                       \
                printf("\33[1;33m");                                 \
            }                                                        \
            printf("----- [%s,%5d] ----- ",__FUNCTION__,__LINE__);   \
            printf(_fmt, ## _args);                      \
            if( lvl & E_MHAL_DISP_DBG_ERR)                          \
            {                                                       \
                printf("\33[m");                                    \
            }                                                        \
        }                                                           \
    } while(0);

#else

#define MHAL_DISP_DBG(lvl, _fmt, _args...)                          \
    do                                                              \
    {                                                               \
        if( lvl & gu32MHAL_DISP_DbgLevel)                             \
        {                                                           \
            if( lvl & E_MHAL_DISP_DBG_ERR)                          \
            {                                                       \
                printf("\33[1;33m");                                 \
            }                                                        \
            printk(KERN_INFO "----- [%s,%5d] ----- ",__FUNCTION__,__LINE__);   \
            printk(KERN_INFO _fmt, ## _args);                      \
            if( lvl & E_MHAL_DISP_DBG_ERR)                          \
            {                                                       \
                printf("\33[m");                                    \
            }                                                        \
        }                                                           \
    } while(0);

#endif

#define MHAL_DISP_XC_XTAL_CLOCK_HZ                  (12000000UL)    /* Temp define */

#define FILL_UTPA_DEVICE_STRUCT(id, ver, val)   \
    do                                          \
    {                                           \
        id=val;                                 \
        ver=0;                                  \
    }while(0)


#define PARSING_MHAL_DISP_PIXEL_FORMAT(x)   ( x == E_MHAL_DISP_PIXEL_FRAME_YUV422_YUYV              ?  "YUV422_YUYV"         : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_ARGB8888                 ?  "ARGB8888"            : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_ABGR8888                 ?  "ABGR8888"            : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_RGB565                   ?  "RGB565"              : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_ARGB1555                 ?  "ARGB1555"            : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_I2                       ?  "I2"                  : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_I4                       ?  "I4"                  : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_I8                       ?  "I8"                  : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_422       ?  "YUV_SEMIPLANAR_422"  : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_420       ?  "YUV_SEMIPLANAR_420"  : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YUV_MST_420              ?  "YUV_MST_420"         : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE1_H264      ?  "YC420_MSTTILE1_H264" : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE2_H265      ?  "YC420_MSTTILE2_H265" : \
                                              x == E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE3_H265      ?  "YC420_MSTTILE3_H265" : \
                                                                                                       "UNKNOWN")

#define PARSING_MHAL_DISP_COMPRESS_MD(x)    ( x == E_MHAL_DISP_COMPRESS_MODE_NONE   ?  "COMPRESS_MODE_NONE"  : \
                                              x == E_MHAL_DISP_COMPRESS_MODE_SEG    ?  "COMPRESS_MODE_SEG"   : \
                                              x == E_MHAL_DISP_COMPRESS_MODE_LINE   ?  "COMPRESS_MODE_LINE"  : \
                                              x == E_MHAL_DISP_COMPRESS_MODE_FRAME  ?  "COMPRESS_MODE_FRAME" : \
                                                                                       "UNKNOWN")



#define PARSING_MHAL_DISP_INTERFACE(x)      (   (x & MHAL_DISP_INTF_HDMI)                           ==  MHAL_DISP_INTF_HDMI                         ?  "HDMI"    :\
                                                (x & MHAL_DISP_INTF_CVBS)                           ==  MHAL_DISP_INTF_CVBS                         ?  "CVBS"    :\
                                                (x & MHAL_DISP_INTF_VGA)                            ==  MHAL_DISP_INTF_VGA                          ?  "VGA"     :\
                                                (x & MHAL_DISP_INTF_YPBPR)                          == MHAL_DISP_INTF_YPBPR                         ?  "YPBPR"   :\
                                                (x & MHAL_DISP_INTF_LCD)                            == MHAL_DISP_INTF_LCD                           ?  "LCD"     :\
                                                (x & (MHAL_DISP_INTF_HDMI | MHAL_DISP_INTF_CVBS))   == (MHAL_DISP_INTF_HDMI | MHAL_DISP_INTF_CVBS)  ?  "HDMI | CVBS"  :\
                                                                                                                                                       "UNKNOWN")


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

#define PARSING_CSC_MATRIX(x)               (   x == E_MHAL_DISP_CSC_MATRIX_BYPASS            ?   "BYPASS" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_BT601_TO_BT709    ?   "BT601_TO_BT709" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_BT709_TO_BT601    ?   "BT709_TO_BT601" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_BT601_TO_RGB_PC   ?   "BT601_TO_RGB" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_BT709_TO_RGB_PC   ?   "BT709_TO_RGB" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_RGB_TO_BT601_PC   ?   "RGB_TO_BT601" : \
                                                x == E_MHAL_DISP_CSC_MATRIX_RGB_TO_BT709_PC   ?   "RGB_TO_BT709" : \
                                                                                                  "UNKNOWN")


#define DAC_FOR_VE      FALSE
#define DAC_FOR_VGA     TRUE
//-------------------------------------------------------------------------------------------------
//  Local enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MHAL_DISP_DEVICE_ID_0 = 0,
    E_MHAL_DISP_DEVICE_ID_1 = 1,
    E_MHAL_DISP_DEVICE_ID_NUM,
}MHalDispDeviceIdType_e;

typedef enum
{
    E_MHAL_DISP_XC_WIN_ID_MAIN = 0,
    E_MHAL_DISP_XC_WIN_ID_SUB = 1,
    E_MHAL_DISP_XC_WIN_ID_NUM,
}MHalDispXCWindowType_e;


typedef enum
{
    E_MHAL_DISP_VIDEO_LAYER_ID_0,
    E_MHAL_DISP_VIDEO_LAYER_ID_1,
    E_MHAL_DISP_VIDEO_LAYER_ID_NUM,
}MHalDispVideoLayerIdType_e;

typedef enum
{
    E_MHAL_DISP_INPUT_PORT_ID_0,
    E_MHAL_DISP_INPUT_PORT_ID_1,
    E_MHAL_DISP_INPUT_PORT_ID_2,
    E_MHAL_DISP_INPUT_PORT_ID_3,
    E_MHAL_DISP_INPUT_PORT_ID_4,
    E_MHAL_DISP_INPUT_PORT_ID_5,
    E_MHAL_DISP_INPUT_PORT_ID_6,
    E_MHAL_DISP_INPUT_PORT_ID_7,
    E_MHAL_DISP_INPUT_PORT_ID_8,
    E_MHAL_DISP_INPUT_PORT_ID_9,
    E_MHAL_DISP_INPUT_PORT_ID_10,
    E_MHAL_DISP_INPUT_PORT_ID_11,
    E_MHAL_DISP_INPUT_PORT_ID_12,
    E_MHAL_DISP_INPUT_PORT_ID_13,
    E_MHAL_DISP_INPUT_PORT_ID_14,
    E_MHAL_DISP_INPUT_PORT_ID_15,
    E_MHAL_DISP_INPUT_PORT_ID_NUM,
}MHalDispInputPortIdType_e;

typedef enum
{
    E_MHAL_DISP_XC_INPUT_SRC_MGWIN,
    E_MHAL_DISP_XC_INPUT_SRC_OP,
    E_MHAL_DISP_XC_INPUT_SRC_MVOP,
    E_MHAL_DISP_XC_INPUT_SRC_NUM,
}MHalDispXcInputSourceType_e;


typedef enum
{
    E_MHAL_DISP_PNL_RES_MIN = 0,

    E_MHAL_DISP_PNL_SEC32_LE32A_FULLHD = E_MHAL_DISP_PNL_RES_MIN,
    // For Normal LVDS panel
    E_MHAL_DISP_PNL_RES_SXGA          = 1,            // 1280x1024, Pnl_AU17_EN05_SXGA
    E_MHAL_DISP_PNL_RES_WXGA          = 2,            // 1366x768, Pnl_AU20_T200XW02_WXGA,
    E_MHAL_DISP_PNL_RES_WXGA_PLUS     = 3,            // 1440x900, Pnl_CMO19_M190A1_WXGA, Pnl_AU19PW01_WXGA
    E_MHAL_DISP_PNL_RES_WSXGA         = 4,            // 1680x1050, Pnl_AU20_M201EW01_WSXGA,
    E_MHAL_DISP_PNL_RES_FULL_HD       = 5,            // 1920x1080, Pnl_AU37_T370HW01_HD, Pnl_CMO216H1_L01_HD.h

    // For DAC/HDMI Tx output
    E_MHAL_DISP_PNL_DACOUT_480I       = 6,            // just for U3 Dac output 480i timing usage
    E_MHAL_DISP_PNL_DACOUT_480P       = 7,            // just for U3 Dac output 480p timing usage
    E_MHAL_DISP_PNL_DACOUT_576I       = 8,            // just for U3 Dac output 576i timing usage
    E_MHAL_DISP_PNL_DACOUT_576P       = 9,            // just for U3 Dac output 576p timing usage
    E_MHAL_DISP_PNL_DACOUT_720P_50    = 10,           // just for U3 Dac output 720p timing usage
    E_MHAL_DISP_PNL_DACOUT_720P_60    = 11,           // just for U3 Dac output 720p timing usage
    E_MHAL_DISP_PNL_DACOUT_1080I_50   = 12,           // just for U3 Dac output 1080i timing usage
    E_MHAL_DISP_PNL_DACOUT_1080I_60   = 13,           // just for U3 Dac output 1080i timing usage
    E_MHAL_DISP_PNL_DACOUT_1080P_24   = 14,           // just for U3 Dac output 1080p timing usage
    E_MHAL_DISP_PNL_DACOUT_1080P_25   = 15,           // just for U3 Dac output 1080p timing usage
    E_MHAL_DISP_PNL_DACOUT_1080P_30   = 16,           // just for U3 Dac output 1080p timing usage
    E_MHAL_DISP_PNL_DACOUT_1080P_50   = 17,           // just for U3 Dac output 1080p timing usage
    E_MHAL_DISP_PNL_DACOUT_1080P_60   = 18,           // just for U3 Dac output 1080p timing usage
    E_MHAL_DISP_PNL_DACOUT_640X480P   = 19,
    E_MHAL_DISP_PNL_DACOUT_1920X2205P_24 = 20,
    E_MHAL_DISP_PNL_DACOUT_1280X1470P_50 = 21,
    E_MHAL_DISP_PNL_DACOUT_1280X1470P_60 = 22,
    E_MHAL_DISP_PNL_DACOUT_3840X2160P_24 = 23,
    E_MHAL_DISP_PNL_DACOUT_3840X2160P_25 = 24,
    E_MHAL_DISP_PNL_DACOUT_3840X2160P_30 = 25,
    E_MHAL_DISP_PNL_DACOUT_3840X2160P_50 = 26,
    E_MHAL_DISP_PNL_DACOUT_3840X2160P_60 = 27,
    E_MHAL_DISP_PNL_DACOUT_4096X2160P_24 = 28,
    E_MHAL_DISP_PNL_DACOUT_4096X2160P_25 = 29,
    E_MHAL_DISP_PNL_DACOUT_4096X2160P_30 = 30,
    E_MHAL_DISP_PNL_DACOUT_4096X2160P_50 = 31,
    E_MHAL_DISP_PNL_DACOUT_4096X2160P_60 = 32,
    E_MHAL_DISP_PNL_DACOUT_1600X1200P_60 = 33,           // Dac output 1600X1200p timing usage
    E_MHAL_DISP_PNL_DACOUT_1440X900P_60  = 34,           // Dac output 1440X900P timing usage
    E_MHAL_DISP_PNL_DACOUT_1280X1024P_60 = 35,           // Dac output 1280X1024P timing usage
    E_MHAL_DISP_PNL_DACOUT_1024X768P_60  = 36,           // Dac output 1024X768P timing usage
    E_MHAL_DISP_PNL_DACOUT_720P_24       = 37,
    E_MHAL_DISP_PNL_DACOUT_720P_25       = 38,
    E_MHAL_DISP_PNL_DACOUT_720P_30       = 39,
    E_MHAL_DISP_PNL_DACOUT_800X600P_60   = 40,
    E_MHAL_DISP_PNL_DACOUT_1280X800P_60  = 41,
    E_MHAL_DISP_PNL_DACOUT_1366X768P_60  = 42,
    E_MHAL_DISP_PNL_DACOUT_1680X1050P_60 = 43,
    E_MHAL_DISP_PNL_DACOUT_1920X1200P_60 = 44,
    E_MHAL_DISP_PNL_CMO_CMO260J2_WUXGA   = 45,           // 1920*1200
    E_MHAL_DISP_PNL_RES_MAX_NUM          = 46,
}MHalDispPnlResolutionType_e;


typedef enum
{
    E_MHAL_DISP_DBG_FUNC                = 0x00000001,
    E_MHAL_DISP_DBG_INPUT_PORT_FUNC     = 0x00000002,
    E_MHAL_DISP_DBG_MUTE_FUNC           = 0x00000004,
    E_MHAL_DISP_DBG_INTTERUPT           = 0x00000008,
    E_MHAL_DISP_DBG_XC                  = 0x00000010,
    E_MHAL_DISP_DBG_CHECK_FUNC          = 0x00000020,
    E_MHAL_DISP_DBG_INPUT_PORT_FLIP     = 0x00000040,
    E_MHAL_DISP_DBG_ERR                 = 0x10000000,
    E_MHAL_DISP_DBG_ALL                 = 0xFFFFFFFF,
}MHalDispDebugLevelType_e;

typedef enum
{
    E_MHAL_DISP_OUPUT_INTERFACE_0,
    E_MHAL_DISP_OUPUT_INTERFACE_1,
    E_MHAL_DISP_OUPUT_INTERFACE_MAX,
}MHalDispDeviceOutputInterfaceType_e;

typedef enum
{
    E_MHAL_DISP_PICTURE_CONTRAST = 0,
    E_MHAL_DISP_PICTURE_BRIGHTNESS,
    E_MHAL_DISP_PICTURE_SATURATION,
    E_MHAL_DISP_PICTURE_SHARPNESS,
    E_MHAL_DISP_PICTURE_HUE,
    E_MHAL_DISP_PICTURE_NUM
}MHalDispPictureType_e;

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MHalDispVideoLayerIdType_e enVideoLayerId;
    s32 s32Handler;
    MHalDispDeviceIdType_e enDeviceId;
    MS_BOOL bUpdateReg;
    MS_U32 u32InputPortEnFlag;
    MS_BOOL bEn;
}MHalDispVideoLayerCtx_t;

typedef struct
{
    MHalDispDeviceIdType_e enDeviceId;
    MHalDispVideoLayerCtx_t *pVideLayerCtx[E_MHAL_DISP_VIDEO_LAYER_ID_NUM];
    XC_EX_INITDATA stXcInitData;
    PNL_EX_PanelType *pstPnlType;
    MHalDispXcInputSourceType_e enInputSrc;
    MHAL_DISP_DeviceTiming_e  enDeviceTiming[E_MHAL_DISP_OUPUT_INTERFACE_MAX];
    MHAL_DISP_VidWinRect_t stSrcWin;
    MHAL_DISP_VidWinRect_t stDispWin;
    MS_U32 u32Interface;
    MS_U8  u8BindVideoLayerNum;
}MHalDispDeviceCtx_t;

typedef struct
{
    MS_U32 u32Xc0MainFbAddr;
    MS_U32 u32Xc0MainFbSize;
    MS_U32 u32Xc1MainFbAddr;
    MS_U32 u32Xc1MainFbSize;
    MS_U32 u32VeFbAddr;
    MS_U32 u32VeFbSize;
}MHalDispMemCfg_t;

typedef struct
{
    MHalDispInputPortIdType_e enInputPortId;
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx;
    MS_BOOL bShow;
    MS_BOOL bEn;
    MS_BOOL bFlip;
}MHalDispInputPortCtx_t;


typedef struct
{
    MS_U16 u16SrcHtotal;
    MS_U16 u16SrcVtotal;
    MS_U16 u16SrcX;
    MS_U16 u16SrcY;
    MS_U16 u16SrcWidth;
    MS_U16 u16SrcHeight;
    MS_U16 u16SrcFreqx10;
    MS_VE_VIDEOSYS enVideoStd;
    MS_BOOL bSrcUpdated;
}MHalDispVeConfig_t;

typedef struct
{
    MS_U32 u32Addr;
    MS_U8  u8value;
}MHalDispScript_t;

typedef struct
{
    MS_U8 u8OSD_0;
    MS_U8 u8OSD_25;
    MS_U8 u8OSD_50;
    MS_U8 u8OSD_75;
    MS_U8 u8OSD_100;
} MHalDispPictureNonLinearCurveType_t;

typedef struct
{
    MHalDispPictureNonLinearCurveType_t stHdmiPictureCurve[E_MHAL_DISP_PICTURE_NUM];
    MHalDispPictureNonLinearCurveType_t stVgaPictureCurve[E_MHAL_DISP_PICTURE_NUM];
}MHalDispPictureNonLinearCurveConfig_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern  PNL_EX_PanelType * tPanelIndexTbl[E_MHAL_DISP_PNL_RES_MAX_NUM];
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
const MHAL_DISP_AllocPhyMem_t *gstDeviceAllocMemFunc = NULL;
MHalDispDeviceCtx_t stMHalDispDeviceCtx[E_MHAL_DISP_DEVICE_ID_NUM];
MHalDispVideoLayerCtx_t stMHalDispLayerCtx[E_MHAL_DISP_VIDEO_LAYER_ID_NUM];
MHalDispInputPortCtx_t stMHalDispInputPortCtx[E_MHAL_DISP_VIDEO_LAYER_ID_NUM][E_MHAL_DISP_INPUT_PORT_ID_NUM];
MHalDispVeConfig_t stVeCfg;
MHAL_DISP_PanelConfig_t *gpstPanelConfig = NULL;
MS_U8 gu8PanelSize = 0;
MHAL_DISP_MmapInfo_t stMmapInfo[E_MHAL_DISP_MMAP_MAX];
MHalDispMemCfg_t stMhalMemConfig;

MS_BOOL gbDeviceCreated[E_MHAL_DISP_DEVICE_ID_NUM] = {0, 0};
MS_BOOL gbVideoLayerIrqEnable[E_MHAL_DISP_VIDEO_LAYER_ID_NUM] = {0, 0};
MS_BOOL gbXcAceInited[E_MHAL_DISP_DEVICE_ID_NUM] = {0, 0};
MS_BOOL gbDacInited[E_MHAL_DISP_DEVICE_ID_NUM] = {0, 0};
MS_BOOL gbDeviceAttach = FALSE;
MS_BOOL gbVideoLayerCreated[E_MHAL_DISP_VIDEO_LAYER_ID_NUM] = {0, 0};
MS_BOOL gbVeInited = 0;
MS_U32  gu32MHAL_DISP_DbgLevel = E_MHAL_DISP_DBG_ERR | E_MHAL_DISP_DBG_FUNC;

#if ENABLE_UTPA_ACE
static MS_S16  S16DefaultColorCorrectionMatrix[32] =
{
   0x03EA, 0x0035,-0x0020,-0x0003, 0x0420,-0x001D, 0x000A,-0x0043,
   0x0438,-0x034B, 0x0196,-0x068B, 0x03C9,-0x0439, 0x0032,-0x0004,
  -0x07EE, 0x04E7, 0x07CB,-0x04C3, 0x0404, 0x023B,-0x023E, 0x01D5,
  -0x0831, 0x0100,-0x0001, 0x0100,-0x0000, 0x0000, 0x0000, 0x0000,
};

//DAC output bypass color matrix
MS_S16 S16ByPassColorCorrectionMatrix[32] =
{
   0x0400, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000,
   0x0400,-0x02E6, 0x0288,-0x05BB, 0x07A4,-0x062C, 0x06F3,-0x073C,
  -0x0024, 0x01BF, 0x07EF,-0x0116, 0x01EE, 0x052C,-0x03BB, 0x00B1,
  -0x0831, 0x0100,-0x0000, 0x0000,-0x0000, 0x0000, 0x0000, 0x0000,
};

MS_S16 S16DACColorCorrectionMatrix[32] =
{
   0x0476, 0x0000, 0x0000, 0x0000, 0x0360, 0x0000, 0x0000, 0x0000,
   0x0400,-0x02E6, 0x0288,-0x05BB, 0x07A4,-0x062C, 0x06F3,-0x073C,
  -0x0024, 0x01BF, 0x07EF,-0x0116, 0x01EE, 0x052C,-0x03BB, 0x00B1,
  -0x0831, 0x0100,-0x0000, 0x0000,-0x0000, 0x0000, 0x0000, 0x0000,
};

static MS_S16  S16BoxYUVtoRGB[3][3] =
{
    { 1024, 0,      0 },
    { 0,    1024,   0 },
    { 0,    0,      1024 }
};


MS_S16  S16DefaultRGB[3][3] =
{
    {   0x0731, 0x04AC,  0x0000 }, // 1.7978,  1.1679, 0
    {  -0x0223, 0x04AC, -0x00DB }, // -0.5341, 1.1679, -0.2138
    {   0x0000, 0x04AC,  0x0879 }  // 0,      1.1679, 2.1181

};

MS_S16 S16YUV601To709FULL[32] =
{
    0x0419, 0x0000, 0x004D,-0x0F2C, 0x03FF,-0x0F8B, 0x0074, 0x0000,
    0x0412,-0x02E6, 0x0288,-0x05BB, 0x07A4,-0x062C, 0x06F3,-0x073C,
   -0x0024, 0x01BF, 0x07EF,-0x0116, 0x01EE, 0x052C,-0x03BB, 0x00B1,
   -0x0831, 0x0100,-0x0000, 0x0000,-0x0000, 0x0000, 0x0000, 0x0000,
};


MS_S16 S16YUV709To601FULL[32] =
{
  0x03EF, 0x0000,-0x0FB7, 0x00C4, 0x03FF, 0x0066,-0x0F90, 0x0000,
  0x03F6,-0x02E6, 0x0288,-0x05BB, 0x07A4,-0x062C, 0x06F3,-0x073C,
 -0x07EE, 0x04E7, 0x07CB,-0x04C3, 0x0404, 0x023B,-0x023E, 0x01D5,
 -0x0831, 0x0100,-0x0000, 0x0000,-0x0000, 0x0000, 0x0000, 0x0000,
};
#endif

MHalDispPictureNonLinearCurveConfig_t stPictureNonLiearSetting =
{
    { // Hdmi
        {0, 80, 128, 160, 200},  // Contrast
        {0, 64, 128, 192, 255},  // Brightness
        {0, 96, 128, 160, 192},  // Color
        {0, 8, 15, 23, 63},      // Sharpness
        {30, 40, 50, 60, 70},    // Tint
    },


    { // Vga
        {0, 80, 128, 160, 200},  // Contrast
        {0, 64, 128, 192, 255},  // Brightness
        {0, 96, 128, 160, 192},  // Color
        {0, 8, 15, 23, 63},      // Sharpness
        {30, 40, 50, 60, 70},    // Tint
    },
};


MHalDispScript_t Sc1PeakingSetting[] =
{
    {0x139920, 0x01},  // Post Peaking En
    {0x139921, 0x0F},  // Band1~4 En
    {0x139930, 0x16},  // band1 coef
    {0x139931, 0x14},  // band2 coef
    {0x139932, 0x10},  // band3 coef
    {0x139933, 0x0E},  // band4 coef
    {0x139940, 0xA0},  // band1 overshoot limit
    {0x139941, 0x40},  // band2 overshoot limit
    {0x139942, 0x04},  // band3 overshoot limit
    {0x139943, 0x00},  // band4 overshoot limit
    {0x139948, 0xA0},  // band1 undershoot limit
    {0x139949, 0x40},  // band2 undershoot limit
    {0x13994A, 0x04},  // band3 undershoot limit
    {0x13994B, 0x00},  // band4 undershoot limit
    {0x139927, 0x0F},  // Sharpness adjust
    {0xFFFFFF, 0x00},
};






#if MHAL_FPGA_EN

MHalDispScript_t Scl_0x102E_Scirpt[] =
{
    {0x102e24, 0xd0},
    {0x102e25, 0x02},
    {0x102e26, 0x10},
    {0x102e27, 0x00},
    {0x102e28, 0x3e},
    {0x102e29, 0x00},
    {0x102e2a, 0x59},
    {0x102e2b, 0x03},
    {0x102e2c, 0xe0},
    {0x102e2d, 0x01},
    {0x102e2e, 0x06},
    {0x102e2f, 0x00},
    {0x102e30, 0x09},
    {0x102e31, 0x00},
    {0x102e32, 0x0c},
    {0x102e33, 0x02},
    {0x102e34, 0x02},
    {0x102e35, 0x04},
    {0x102e36, 0x00},
    {0x102e37, 0x00},
    {0x102e38, 0x00},
    {0x102e39, 0x02},
    {0x102e3a, 0xff},
    {0x102e3b, 0x03},
    {0x102e3c, 0x00},
    {0x102e3d, 0x02},
    {0x102e3e, 0x00},
    {0x102e3f, 0x00},
    {0x102e02, 0xf1},
    {0x102e03, 0x00},
    {0x102e20, 0x01},
    {0x102e21, 0x00},
    {0x101e06, 0x00},
    {0x101e07, 0xC0},
    {0x101e09, 0x00},
    {0x102f00, 0x25},
    {0x102f5c, 0x01},
    {0x102600, 0x00}, //[0]:reg_all_pad_in
    {0x102617, 0x01}, //[0]:reg_ttl_out
    //{0x130180, 0x03},
    //{0x130184, 0x1f},
    //{0x130185, 0x1f},
    //{0x130220, 0x00},
    //{0x130221, 0x00},
    {0xFFFFFF, 0x00},
};

#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
void _MHalDispDumpScript(MHalDispScript_t *pScript)
{
    MS_U32 i;
    for(i=0; ; i++)
    {
        if(pScript[i].u32Addr == 0xFFFFFF)
        {
            break;
        }
    #if ENABLE_UTPA_XC
        MApi_XC_WriteByte(pScript[i].u32Addr, pScript[i].u8value);
    #endif
    }
}


MS_BOOL _MHalDispInitVariable(MS_U32 u32DeviceId)
{
    MS_BOOL bRet = TRUE;
    if(u32DeviceId >= E_MHAL_DISP_DEVICE_ID_NUM)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " DeviceId(%d) out of range\n", u32DeviceId);
        bRet = FALSE;
    }
    else
    {
        memset(&stMHalDispDeviceCtx[u32DeviceId], 0, sizeof(MHalDispDeviceCtx_t));

        stMHalDispDeviceCtx[u32DeviceId].enDeviceId = u32DeviceId == 0 ? E_MHAL_DISP_DEVICE_ID_0 : E_MHAL_DISP_DEVICE_ID_1;

        stMHalDispDeviceCtx[u32DeviceId].enInputSrc = E_MHAL_DISP_XC_INPUT_SRC_NUM;
        stMHalDispDeviceCtx[u32DeviceId].pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER] = NULL;
        stMHalDispDeviceCtx[u32DeviceId].pVideLayerCtx[MHAL_DISP_VIDEOLAYER_SECONDARY] = NULL;

        stMHalDispDeviceCtx[u32DeviceId].enDeviceTiming[E_MHAL_DISP_OUPUT_INTERFACE_0] = E_MHAL_DISP_OUTPUT_MAX;
        stMHalDispDeviceCtx[u32DeviceId].enDeviceTiming[E_MHAL_DISP_OUPUT_INTERFACE_1] = E_MHAL_DISP_OUTPUT_MAX;

        if(u32DeviceId == 0)
        {
            memset(&stVeCfg, 0 , sizeof(MHalDispVeConfig_t));
            stVeCfg.enVideoStd = MS_VE_NTSC;
            stVeCfg.u16SrcHtotal = 858;
            stVeCfg.u16SrcVtotal = 525;
            stVeCfg.u16SrcX      = 122;
            stVeCfg.u16SrcY      = 0;
            stVeCfg.u16SrcWidth  = 720;
            stVeCfg.u16SrcHeight = 480;
            stVeCfg.u16SrcFreqx10 = 600;
        }


        if(gpstPanelConfig == NULL)
        {
        #if defined(MSOS_TYPE_NOS)
            gpstPanelConfig = malloc((sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX));
        #else
            gpstPanelConfig = kmalloc((sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX), GFP_KERNEL);
        #endif
            memset(gpstPanelConfig, 0, (sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX));
        }
        bRet = TRUE;
    }

    return bRet;
}

PNL_EX_PanelType *_MHalDispGetPanelSpec(MHalDispPnlResolutionType_e enPnlIdx)
{
    if(enPnlIdx < E_MHAL_DISP_PNL_RES_MAX_NUM)
    {
        return tPanelIndexTbl[enPnlIdx];
    }
    else
    {
        return NULL;
    }
}

u16 _MHalDispDevTiming2Freqx10(MHAL_DISP_DeviceTiming_e enTiming)
{
    u16 u16Freqx10;

    switch(enTiming)
    {
        case E_MHAL_DISP_OUTPUT_1080P24:
            u16Freqx10 = 240;
            break;

        case E_MHAL_DISP_OUTPUT_1080P25:
            u16Freqx10 = 250;
            break;

        case E_MHAL_DISP_OUTPUT_1080P30:
        case E_MHAL_DISP_OUTPUT_3840x2160_30:
           u16Freqx10 = 300;
            break;

        case E_MHAL_DISP_OUTPUT_1080I50:
            u16Freqx10 = 500;
            break;

        case E_MHAL_DISP_OUTPUT_1080I60:
            u16Freqx10 = 600;
            break;


        case E_MHAL_DISP_OUTPUT_720P50:
        case E_MHAL_DISP_OUTPUT_1080P50:
        case E_MHAL_DISP_OUTPUT_576P50:
            u16Freqx10 = 500;
            break;

        case E_MHAL_DISP_OUTPUT_720P60:
        case E_MHAL_DISP_OUTPUT_1080P60:
        case E_MHAL_DISP_OUTPUT_480P60:
        case E_MHAL_DISP_OUTPUT_640x480_60:
        case E_MHAL_DISP_OUTPUT_800x600_60:
        case E_MHAL_DISP_OUTPUT_1024x768_60:
        case E_MHAL_DISP_OUTPUT_1280x1024_60:
        case E_MHAL_DISP_OUTPUT_1366x768_60:
        case E_MHAL_DISP_OUTPUT_1440x900_60:
        case E_MHAL_DISP_OUTPUT_1280x800_60:
        case E_MHAL_DISP_OUTPUT_1680x1050_60:
        case E_MHAL_DISP_OUTPUT_1920x2160_30:
        case E_MHAL_DISP_OUTPUT_1600x1200_60:
        case E_MHAL_DISP_OUTPUT_1920x1200_60:
        case E_MHAL_DISP_OUTPUT_2560x1440_30:
        case E_MHAL_DISP_OUTPUT_2560x1600_60:
        case E_MHAL_DISP_OUTPUT_3840x2160_60:
        default:
            u16Freqx10 = 600;
            break;
    }

    return u16Freqx10;
}


MS_BOOL _MHalDispDevTiming2VideoTimingType(MHAL_DISP_DeviceTiming_e enTiming, E_OUTPUT_VIDEO_TIMING_TYPE *penVideTiming)
{
    MS_BOOL bRet = TRUE;

    switch(enTiming)
    {
        case E_MHAL_DISP_OUTPUT_1080P24:
            *penVideTiming = E_RES_1920x1080P_24Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080P25:
            *penVideTiming = E_RES_1920x1080P_25Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080P30:
            *penVideTiming = E_RES_1920x1080P_30Hz;
            break;

        case E_MHAL_DISP_OUTPUT_720P50:
            *penVideTiming = E_RES_1280x720P_50Hz;
            break;

        case E_MHAL_DISP_OUTPUT_720P60:
            *penVideTiming = E_RES_1280x720P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080I50:
            *penVideTiming = E_RES_1920x1080I_50Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080I60:
            *penVideTiming = E_RES_1920x1080I_50Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080P50:
            *penVideTiming = E_RES_1920x1080P_50Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1080P60:
            *penVideTiming = E_RES_1920x1080P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_576P50:
            *penVideTiming = E_RES_720x576P_50Hz;
            break;

        case E_MHAL_DISP_OUTPUT_480P60:
            *penVideTiming = E_RES_720x480P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_640x480_60:
            *penVideTiming = E_RES_640x480P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_800x600_60:
            *penVideTiming = E_RES_800x600P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1024x768_60:
            *penVideTiming= E_RES_1024x768P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1280x1024_60:
            *penVideTiming= E_RES_1280x1024P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1366x768_60:
            *penVideTiming = E_RES_1366x768P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1440x900_60:
            *penVideTiming = E_RES_1440x900P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1280x800_60:
            *penVideTiming = E_RES_1280x800P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1680x1050_60:
            *penVideTiming = E_RES_1680x1050P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1920x2160_30:
            bRet = FALSE;
            break;

        case E_MHAL_DISP_OUTPUT_1600x1200_60:
            *penVideTiming = E_RES_1600x1200P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_1920x1200_60:
            *penVideTiming = E_RES_1920x1200P_60Hz;
            break;

        case E_MHAL_DISP_OUTPUT_2560x1440_30:
        case E_MHAL_DISP_OUTPUT_2560x1600_60:
            bRet = FALSE;
            break;

        case E_MHAL_DISP_OUTPUT_3840x2160_30:
            *penVideTiming = E_RES_3840x2160P_30Hz;
            break;

        case E_MHAL_DISP_OUTPUT_3840x2160_60:
        default:
            bRet = FALSE;
            break;
    }

    if(bRet == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Time: %s is not Support\n", PARSING_MHAL_DISP_OUTPUT_TIMING(enTiming));
    }

    return bRet;
}



MHalDispPnlResolutionType_e _MHalDispDevTiming2PnlResType(MHAL_DISP_DeviceTiming_e enTiming)
{
    MHalDispPnlResolutionType_e enPnlResType = E_MHAL_DISP_PNL_RES_MAX_NUM;
    switch(enTiming)
    {
        case E_MHAL_DISP_OUTPUT_1080P24:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080P_24;
            break;

        case E_MHAL_DISP_OUTPUT_1080P25:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080P_25;
            break;
        case E_MHAL_DISP_OUTPUT_1080P30:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080P_30;
            break;

        case E_MHAL_DISP_OUTPUT_720P50:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_720P_50;
            break;
        case E_MHAL_DISP_OUTPUT_720P60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_720P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1080I50:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080I_50;
            break;
        case E_MHAL_DISP_OUTPUT_1080I60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080I_60;
            break;
        case E_MHAL_DISP_OUTPUT_1080P50:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080P_50;
            break;
        case E_MHAL_DISP_OUTPUT_1080P60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1080P_60;
            break;

        case E_MHAL_DISP_OUTPUT_576P50:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_576P;
            break;

        case E_MHAL_DISP_OUTPUT_480P60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_480P;
            break;

        case E_MHAL_DISP_OUTPUT_640x480_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_640X480P;
            break;

        case E_MHAL_DISP_OUTPUT_800x600_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_800X600P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1024x768_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1024X768P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1280x1024_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1280X1024P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1366x768_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1366X768P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1440x900_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1440X900P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1280x800_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1280X800P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1680x1050_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1680X1050P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1920x2160_30:
            enPnlResType = E_MHAL_DISP_PNL_RES_MAX_NUM;
            break;

        case E_MHAL_DISP_OUTPUT_1600x1200_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1600X1200P_60;
            break;

        case E_MHAL_DISP_OUTPUT_1920x1200_60:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_1920X1200P_60;
            break;

        case E_MHAL_DISP_OUTPUT_2560x1440_30:
        case E_MHAL_DISP_OUTPUT_2560x1600_60:
            enPnlResType = E_MHAL_DISP_PNL_RES_MAX_NUM;
            break;

        case E_MHAL_DISP_OUTPUT_3840x2160_30:
            enPnlResType = E_MHAL_DISP_PNL_DACOUT_3840X2160P_30;
            break;

        case E_MHAL_DISP_OUTPUT_3840x2160_60:
        default:
            enPnlResType = E_MHAL_DISP_PNL_RES_MAX_NUM;
            break;
    }
    return enPnlResType;
}

DrvSclMgwinIoSubWinIdType_e _MHalDispInputPortId2SubWinId(MHalDispInputPortIdType_e enInputPortId)
{
    DrvSclMgwinIoSubWinIdType_e enSubWinId;
    enSubWinId = enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_0  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_0 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_1  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_1 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_2  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_2 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_3  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_3 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_4  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_4 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_5  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_5 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_6  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_6 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_7  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_7 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_8  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_8 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_9  ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_9 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_10 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_10 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_11 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_11 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_12 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_12 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_13 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_13 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_14 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_14 :
                 enInputPortId == E_MHAL_DISP_INPUT_PORT_ID_15 ? E_DRV_SCLMGWIN_IO_SUBWIN_ID_15 :
                                                                 E_DRV_SCLMGWIN_IO_SUBWIN_ID_NUM;

    return enSubWinId;
}


MS_BOOL _MHalDispPixelFmt2MemFmt(MHAL_DISP_PixelFormat_e enPixelFmt, MS_BOOL bCompress, DrvSclMgwinIoMemFormat_e *penMemFmt, MS_U32 *pu32Stride)
{
    MS_BOOL bRet = FALSE;

    switch(enPixelFmt)
    {
        case E_MHAL_DISP_PIXEL_FRAME_YUV422_YUYV:
            *penMemFmt = bCompress ? E_DRV_SCLMGWIN_IO_MEM_FMT_YUV422_8CE : E_DRV_SCLMGWIN_IO_MEM_FMT_YUV422_8B;
            *pu32Stride = *pu32Stride / 2;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_ARGB8888:
            *penMemFmt = E_DRV_SCLMGWIN_IO_MEM_FMT_ARGB8888;
            *pu32Stride = *pu32Stride / 4;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_ABGR8888:
            *penMemFmt = E_DRV_SCLMGWIN_IO_MEM_FMT_ABGR8888;
            *pu32Stride = *pu32Stride / 4;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_RGB565:
            *penMemFmt = E_DRV_SCLMGWIN_IO_MEM_FMT_RGB565;
            *pu32Stride = *pu32Stride / 2;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_ARGB1555:
        case E_MHAL_DISP_PIXEL_FRAME_I2:
        case E_MHAL_DISP_PIXEL_FRAME_I4:
        case E_MHAL_DISP_PIXEL_FRAME_I8:
            *penMemFmt = E_DRV_SCLMGWIN_IO_MEM_FMT_NUM;
            *pu32Stride = *pu32Stride / 1;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_422:
            *penMemFmt = bCompress ? E_DRV_SCLMGWIN_IO_MEM_FMT_YUV422_8CE : E_DRV_SCLMGWIN_IO_MEM_FMT_YUV422_8B;
            *pu32Stride = *pu32Stride / 2;
            break;

        case E_MHAL_DISP_PIXEL_FRAME_YUV_MST_420:
            *penMemFmt = bCompress ? E_DRV_SCLMGWIN_IO_MEM_FMT_YUV420_8CE : E_DRV_SCLMGWIN_IO_MEM_FMT_YUV420_8B;
            *pu32Stride = *pu32Stride * 2 / 3;
            break;


        case E_MHAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_420:
        case E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE1_H264:
        case E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE2_H265:
        case E_MHAL_DISP_PIXEL_FRAME_YC420_MSTTILE3_H265:
            *penMemFmt = bCompress ? E_DRV_SCLMGWIN_IO_MEM_FMT_YUV420_8CE : E_DRV_SCLMGWIN_IO_MEM_FMT_YUV420_8B;
            *pu32Stride = *pu32Stride / 1;
            break;

        default:
            *penMemFmt = E_DRV_SCLMGWIN_IO_MEM_FMT_NUM;
            *pu32Stride = *pu32Stride / 1;
            break;
    }

    bRet = *penMemFmt == E_DRV_SCLMGWIN_IO_MEM_FMT_NUM ? FALSE : TRUE;
    return  bRet;
}

void _MHalDsipXCInputSourceInputPortMapping(XC_EX_INPUT_SOURCE_TYPE src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count )
{
    // prog input soruce
    switch (src_ids)
    {
        default:
            *u8port_count = 0;
            return;
        case E_XC_EX_INPUT_SOURCE_DTV:
        case E_XC_EX_INPUT_SOURCE_STORAGE:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_MVOP;
            break;
        case E_XC_EX_INPUT_SOURCE_DTV2:
        case E_XC_EX_INPUT_SOURCE_STORAGE2:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_MVOP2;
            break;
        case E_XC_EX_INPUT_SOURCE_HDMI:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_DVI0;
            break;
        case E_XC_EX_INPUT_SOURCE_SCALER_OP:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_SCALER_OP;
            break;
    }
}

void _MHalDispPatchVsyncStart(MHalDispDeviceCtx_t *pMHalDispCtx, PNL_EX_PanelType *pPnlInfo)
{
    // For HDMITx
    // VDE start = 0,  Vsync Start = Vtotal - Vsync Width - Vsync BackPorch
    // For VGA
    // VDE start = Vsync width + Vsync BackPorch ,  VSync Start = 0
    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {     // Device0 is for HDMI Tx , the VDE start must be 0 and Vsync start = Vtotal -Vsync Widht - VsyncBackPorch
        pPnlInfo->m_wPanelVStart = 0;
    }
    else
    {      //Device0 is for HDMI Tx , the VDE start must be 0 and Vsync start = 0
        pPnlInfo->m_wPanelVStart = pPnlInfo->m_ucPanelVSyncWidth  + pPnlInfo->m_ucPanelVBackPorch;
    }
}


PNL_EX_PanelType *_MHalDispGetPanelInfo(MHAL_DISP_DeviceTiming_e eDeviceTiming)
{
    MS_U8 u8Count = 0;
    PNL_EX_PanelType *pstPanelInfo = NULL;

    for(u8Count = 0; u8Count < gu8PanelSize; u8Count++)
    {
        if((gpstPanelConfig[u8Count].bValid == TRUE) && (gpstPanelConfig[u8Count].eTiming == eDeviceTiming))
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "eTiming = %d found!!!!\n",  gpstPanelConfig[u8Count].eTiming);
            pstPanelInfo = (PNL_EX_PanelType *)&gpstPanelConfig[u8Count].stPanelAttr;
        }
    }

    if(pstPanelInfo == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "eTiming = %s Pnl not found \n",  PARSING_MHAL_DISP_OUTPUT_TIMING(eDeviceTiming));
    }

    return pstPanelInfo;
}


MS_BOOL _MHalDispInitPnl(MHalDispDeviceCtx_t *pCtx)
{
    MS_BOOL bRet ;
    PNL_DeviceId stPnlDeviceId;

    FILL_UTPA_DEVICE_STRUCT(stPnlDeviceId.u32Id, stPnlDeviceId.u32Version, pCtx->enDeviceId);

    MApi_PNL_EX_SetDbgLevel(FALSE);//enable debug msg for XC

    MApi_PNL_EX_SkipTimingChange(&stPnlDeviceId, FALSE);

    bRet = MApi_PNL_EX_Init(&stPnlDeviceId, pCtx->pstPnlType);

    return bRet;
}


MS_BOOL _MHalDispInitXC(MHalDispDeviceCtx_t *pCtx)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_XC
    XC_DEVICE_ID stXcDeviceId;
    XC_EX_INITDATA *pXcInitData = &pCtx->stXcInitData;

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pCtx->enDeviceId);

    pXcInitData->u32XTAL_Clock = MHAL_DISP_XC_XTAL_CLOCK_HZ;

    pXcInitData->u32Main_FB_Start_Addr = pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 ?
                                         stMhalMemConfig.u32Xc0MainFbAddr :
                                         stMhalMemConfig.u32Xc1MainFbAddr;

    pXcInitData->u32Main_FB_Size       = pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 ?
                                         stMhalMemConfig.u32Xc0MainFbSize :
                                         stMhalMemConfig.u32Xc1MainFbSize;

    pXcInitData->u32Sub_FB_Start_Addr = 0;
    pXcInitData->u32Sub_FB_Size       = 0;

    // Chip related.
    pXcInitData->bIsShareGround = FALSE;

    pXcInitData->bCEC_Use_Interrupt = FALSE;

    pXcInitData->bEnableIPAutoCoast = ENABLE_IP_AUTO_COAST;
    pXcInitData->bMirror = DISABLE;

    // panel info
    pXcInitData->stPanelInfo.u16HStart = g_IPanel.HStart();      // DE H start
    pXcInitData->stPanelInfo.u16VStart = g_IPanel.VStart();
    pXcInitData->stPanelInfo.u16Width  = g_IPanel.Width();
    pXcInitData->stPanelInfo.u16Height = g_IPanel.Height();
    pXcInitData->stPanelInfo.u16HTotal = g_IPanel.HTotal();
    pXcInitData->stPanelInfo.u16VTotal = g_IPanel.VTotal();

    pXcInitData->stPanelInfo.u16DefaultVFreq = g_IPanel.DefaultVFreq();
    pXcInitData->stPanelInfo.u8LPLL_Mode = g_IPanel.LPLL_Mode();
    pXcInitData->stPanelInfo.enPnl_Out_Timing_Mode = E_XC_PNL_CHG_VTOTAL;

    pXcInitData->stPanelInfo.u16DefaultHTotal = g_IPanel.HTotal();
    pXcInitData->stPanelInfo.u16DefaultVTotal = g_IPanel.VTotal();
    pXcInitData->stPanelInfo.u32MinSET = g_IPanel.MinSET();
    pXcInitData->stPanelInfo.u32MaxSET = g_IPanel.MaxSET();
    pXcInitData->stPanelInfo.eLPLL_Type = (E_XC_PNL_LPLL_TYPE) g_IPanel.LPLL_Type();
    pXcInitData->bDLC_Histogram_From_VBlank = FALSE;

    MApi_XC_SkipSWReset(FALSE);
    if(MApi_XC_EX_Init(&stXcDeviceId, pXcInitData, sizeof(XC_EX_INITDATA)) == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Xc Init fail\n");
        bRet = FALSE;
    }
    else
    {
        MApi_XC_SkipSWReset(ENABLE);

        // Set frame color and display window color to YUV black
        MApi_XC_EX_SetFrameColor(&stXcDeviceId, 0x00);
        MApi_XC_EX_SetDispWindowColor(&stXcDeviceId, 0x82, E_XC_EX_MAIN_WINDOW);
        MApi_XC_EX_SetDispWindowColor(&stXcDeviceId, 0x82, E_XC_EX_SUB_WINDOW);

        // Enable black Video to avoid garbage video. If bootlogo case, can't generate SC1 black video
        MApi_XC_EX_GenerateBlackVideo(&stXcDeviceId, ENABLE, E_XC_EX_MAIN_WINDOW);
        MApi_XC_EX_GenerateBlackVideo(&stXcDeviceId, ENABLE, E_XC_EX_SUB_WINDOW);

        // Enable fd_mask by default
        MApi_XC_EX_set_FD_Mask_byWin(&stXcDeviceId, ENABLE, E_XC_EX_MAIN_WINDOW);
        MApi_XC_EX_set_FD_Mask_byWin(&stXcDeviceId, ENABLE, E_XC_EX_SUB_WINDOW);

        // For frame buffer less mode
#if XC_FBL_EN == 1
        MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, ENABLE);
#else
        MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, (pXcInitData->u32Main_FB_Size == 0) ? ENABLE : DISABLE);
#endif
        MApi_XC_EX_SetFrameColor(&stXcDeviceId, 0);
    }
#endif
    return bRet;
}

MS_BOOL _MHalDispInitPQ(MHalDispDeviceCtx_t *pCtx)
{
#if ENABLE_UTPA_PQ
    MS_PQ_Init_Info sXcPqInitData;
    PQ_Function_Info function_Info;
    XC_DEVICE_ID stXcDeviceId;
    static MS_BOOL bInitPQ = FALSE;

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pCtx->enDeviceId);

    sXcPqInitData.bDDR2 = TRUE;
    sXcPqInitData.u32DDRFreq = 400;

    sXcPqInitData.u8BusWidth = 32;

    sXcPqInitData.u16PnlWidth = 1920;
    sXcPqInitData.u16OSD_hsize = 1920;
    sXcPqInitData.u16Pnl_vtotal = pCtx->stXcInitData.stPanelInfo.u16VTotal;

    sXcPqInitData.u8PQBinCnt = 0;
    sXcPqInitData.u8PQTextBinCnt =0;

    if(pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        memset(&function_Info, 0, sizeof(function_Info));

        function_Info.pq_deside_srctype = MDrv_PQ_DesideSrcType;
        function_Info.pq_disable_filmmode = MDrv_PQ_DisableFilmMode;
        function_Info.pq_load_scalingtable = MDrv_PQ_LoadScalingTable;
        function_Info.pq_set_420upsampling = MDrv_PQ_Set420upsampling;
        function_Info.pq_set_csc = MDrv_PQ_SetCSC;
        function_Info.pq_set_memformat = MDrv_PQ_SetMemFormat;
        function_Info.pq_set_modeInfo = MDrv_PQ_Set_ModeInfo;
        function_Info.pq_get_memyuvfmt = MDrv_PQ_Get_MemYUVFmt;
        function_Info.pq_ioctl = MDrv_PQ_IOCTL;
        MApi_XC_EX_PQ_LoadFunction(&stXcDeviceId, (PQ_EX_Function_Info*)(&function_Info), 0);
    }
    else
    {
        memset(&function_Info, 0, sizeof(function_Info));
        // Set PQ functions
        function_Info.pq_deside_srctype = NULL;
        function_Info.pq_disable_filmmode = NULL;
        function_Info.pq_load_scalingtable = NULL;
        function_Info.pq_set_420upsampling = NULL;
        function_Info.pq_set_csc = NULL;
        function_Info.pq_set_memformat = NULL;
        function_Info.pq_set_modeInfo = NULL;
        function_Info.pq_get_memyuvfmt = NULL;
        function_Info.pq_ioctl = NULL;

        MApi_XC_EX_PQ_LoadFunction(&stXcDeviceId, (PQ_EX_Function_Info*)(&function_Info), 0);
    }

    if(bInitPQ == FALSE && pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MDrv_PQ_Init(&sXcPqInitData);
#if ENABLE_PQL
        mapi_pql_customer_Init();
#endif
        bInitPQ = TRUE;
    }
#endif
    return TRUE;
}

MS_BOOL _MHalDispInitAce(MHalDispDeviceCtx_t *pCtx, MHalDispXCWindowType_e enWin)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stXcAceDeviceId;
    PNL_DeviceId stPnlDevice ;
    MS_BOOL bWindow = (E_MHAL_DISP_XC_WIN_ID_MAIN == enWin) ? 0 : 1;
    MS_S16 *psUserYUVtoRGBMatrix = NULL;
    MS_BOOL bRBRange;

    FILL_UTPA_DEVICE_STRUCT(stXcAceDeviceId.u32Id, stXcAceDeviceId.u32Version, pCtx->enDeviceId);
    FILL_UTPA_DEVICE_STRUCT(stPnlDevice.u32Id, stPnlDevice.u32Version, pCtx->enDeviceId);

    if (gbXcAceInited[pCtx->enDeviceId] == FALSE)
    {
        XC_ACE_EX_InitData stXcAceInitData;

        memset(&stXcAceInitData, 0, sizeof(XC_ACE_EX_InitData));

        // Set ACE init data
        stXcAceInitData.eWindow = bWindow;

        if ((g_IPanelEx.LPLL_Type(&stPnlDevice) == E_PNL_EX_LINK_DAC_I) || (g_IPanelEx.LPLL_Type(&stPnlDevice) == E_PNL_EX_LINK_DAC_P)) //DAC output
        {
            stXcAceInitData.S16ColorCorrectionMatrix = pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 ?
                                                       (MS_S16*) S16DACColorCorrectionMatrix :
                                                       (MS_S16*) S16ByPassColorCorrectionMatrix ;
        }
        else
        {
            stXcAceInitData.S16ColorCorrectionMatrix = (MS_S16*) S16DefaultColorCorrectionMatrix;
        }

        stXcAceInitData.S16RGB = (MS_S16*) S16BoxYUVtoRGB;
        stXcAceInitData.u16MWEHstart = g_IPanelEx.HStart(&stPnlDevice);
        stXcAceInitData.u16MWEVstart = g_IPanelEx.VStart(&stPnlDevice);
        stXcAceInitData.u16MWEWidth  = g_IPanelEx.Width(&stPnlDevice);
        stXcAceInitData.u16MWEHeight = g_IPanelEx.Height(&stPnlDevice);

        if(MApi_XC_ACE_EX_Init(&stXcAceDeviceId, &stXcAceInitData, sizeof(XC_ACE_EX_InitData)) == FALSE)
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Initializing ACE...Failed !\n");
            return FALSE;
        }

        gbXcAceInited[pCtx->enDeviceId] = TRUE;
    }


    if(stXcAceDeviceId.u32Id == E_MHAL_DISP_DEVICE_ID_0)
    {
        psUserYUVtoRGBMatrix = (MS_S16*)S16BoxYUVtoRGB;
        bRBRange = TRUE;
    }
    else
    {
    #if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        psUserYUVtoRGBMatrix = (MS_S16*)S16DefaultRGB;
        bRBRange = TRUE;
    #else
        psUserYUVtoRGBMatrix = (MS_S16*)S16BoxYUVtoRGB;
        bRBRange = FALSE;
    #endif
        _MHalDispDumpScript(Sc1PeakingSetting);
    }

    MApi_XC_ACE_EX_SelectYUVtoRGBMatrix(&stXcAceDeviceId, bWindow, E_XC_ACE_EX_YUV_TO_RGB_MATRIX_USER, psUserYUVtoRGBMatrix);
    MApi_XC_ACE_EX_SetColorCorrectionTable(&stXcAceDeviceId, bWindow);
    MApi_XC_ACE_EX_SetRBChannelRange(&stXcAceDeviceId, bWindow, bRBRange);
#endif

    return TRUE;
}


MS_BOOL _MHalDispInitDac(MHalDispDeviceCtx_t *pCtx)
{
    MS_BOOL bIsYPbPr;

    if(gbDacInited[0] == FALSE && gbDacInited[1] == FALSE)
    {
        MApi_DAC_Init();
    }


    if(gbDacInited[pCtx->enDeviceId] == FALSE)
    {
        bIsYPbPr = (pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0) ? FALSE : TRUE;

        MApi_DAC_Enable(ENABLE, bIsYPbPr);

        MApi_DAC_SetOutputSwapSel(E_DAC_R_G_B, bIsYPbPr);

        MApi_DAC_SetClkInv(ENABLE, TRUE); // Clk Inv for YPbpr

        // Init_DAC_Panel
        MApi_DAC_SetYPbPrOutputTiming(E_RES_1920x1080P_60Hz); //ToDo_Ryan


        if(pCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
        {
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT);
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT_SC);
        }
        else
        {
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_SC1_INIT);
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_SC1_INIT_SC);
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT_HDGEN);
            MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_DAC_PLL_INIT);

            MApi_DAC_SetVGAHsyncVsync(TRUE);
        }
        gbDacInited[pCtx->enDeviceId] = TRUE;
    }

    return TRUE;
}



MS_BOOL _MHalDispInitVe(MHalDispDeviceCtx_t *pCtx)
{
#if ENABLE_UTPA_VE
    MS_Switch_VE_Src_Info SwitchInputSrc;
    MS_Switch_VE_Dest_Info SwitchOutputDest;

    if(gbVeInited == FALSE)
    {
        MDrv_VE_DisableRegWrite(DISABLE);

        MDrv_VE_Init(stMhalMemConfig.u32VeFbAddr + VE_BUF_SHIFT_OFFSET);

        MDrv_VE_SetOSD(TRUE);
        MDrv_VE_SetOutputVideoStd(MS_VE_PAL);

        SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;
        MDrv_VE_SwitchInputSource(&SwitchInputSrc);

        SwitchOutputDest.OutputDstType = MS_VE_DEST_CVBS;
        MDrv_VE_SwitchOuputDest(&SwitchOutputDest);

        MDrv_VE_SetCaptureMode(FALSE);

        //MDrv_VE_SetDbgLevel(1);

        gbVeInited = TRUE;
    }
#endif
    return TRUE;
}


MS_BOOL _MHalDispSetXcConnect(MHalDispDeviceCtx_t *pMHalDispCtx, MHalDispXcInputSourceType_e enSrcType)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_XC
    MS_S16 s16PathId;
    XC_EX_MUX_PATH_INFO PathInfo;
    XC_DEVICE_ID stXcDeviceId;

    if(enSrcType == E_MHAL_DISP_XC_INPUT_SRC_NUM)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Src(%d) is not Correct\n", enSrcType);
        return FALSE;
    }

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    memset(&PathInfo, 0, sizeof(XC_EX_MUX_PATH_INFO));
    PathInfo.Path_Type = E_XC_EX_PATH_TYPE_SYNCHRONOUS;

    if(enSrcType == E_MHAL_DISP_XC_INPUT_SRC_OP)
    {
        PathInfo.src = E_XC_EX_INPUT_SOURCE_SCALER_OP;
    }
    else
    {
        if(pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER])
        {
            PathInfo.src =
                pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER]->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_1 ?
                                                              E_XC_EX_INPUT_SOURCE_DTV2 :
                                                              E_XC_EX_INPUT_SOURCE_DTV;
        }
        else
        {

            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VideoLayer/Device not bind\n");
            PathInfo.src = E_XC_EX_INPUT_SOURCE_DTV;
        }
    }

    PathInfo.dest = E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW;
    PathInfo.SyncEventHandler = NULL;
    PathInfo.DestOnOff_Event_Handler = NULL;
    PathInfo.path_thread = NULL;

    s16PathId = MApi_XC_EX_Mux_CreatePath(&stXcDeviceId, &PathInfo, sizeof(XC_EX_MUX_PATH_INFO));
    if (s16PathId == -1)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "CreatePath Fail\n");
        bRet = FALSE;
    }
    else
    {
        MApi_XC_EX_Mux_EnablePath(&stXcDeviceId, (MS_U16)s16PathId);
        pMHalDispCtx->enInputSrc = enSrcType;
        bRet = TRUE;
    }

    MApi_XC_EX_SetInputSource(&stXcDeviceId, PathInfo.src, E_XC_EX_MAIN_WINDOW);

#if MHAL_FPGA_EN == 1
    _MHalDispDumpScript(Scl_0x102E_Scirpt);
#endif
#endif
    return bRet;
}

MS_BOOL _MHalDispSetXcDisConnect(MHalDispDeviceCtx_t *pMHalDispCtx, MHalDispXcInputSourceType_e enSrcType)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_XC
    XC_DEVICE_ID stXcDeviceId;
    XC_EX_INPUT_SOURCE_TYPE enSrc;

    if(enSrcType == E_MHAL_DISP_XC_INPUT_SRC_NUM)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Src(%d) is not correct\n", enSrcType);
        return FALSE;
    }

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    if(pMHalDispCtx->enInputSrc == enSrcType)
    {
        if(enSrcType == E_MHAL_DISP_XC_INPUT_SRC_OP)
        {
            enSrc = E_XC_EX_INPUT_SOURCE_SCALER_OP;
        }
        else
        {
            if(pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER])
            {
                enSrc =
                pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER]->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_1 ?
                                                                       E_XC_EX_INPUT_SOURCE_DTV2 :
                                                                       E_XC_EX_INPUT_SOURCE_DTV;
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VideoLayer/Device not bind\n");
                enSrc = E_XC_EX_INPUT_SOURCE_DTV;
            }

        }

        if(-1 == MApi_XC_EX_Mux_DeletePath(&stXcDeviceId, enSrc, E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW))
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Delete Path fail, Id=%d, Src=%d\n", pMHalDispCtx->enDeviceId, enSrcType);
            bRet = FALSE;
        }
        else
        {
            pMHalDispCtx->enInputSrc = E_MHAL_DISP_XC_INPUT_SRC_NUM;
        }
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Src Type not match, Id=%d, (%d, %d)\n", pMHalDispCtx->enDeviceId, pMHalDispCtx->enInputSrc, enSrcType);
        bRet = FALSE;
    }
#endif
    return bRet;
}

PNL_EX_PanelType *_MHalDispGetPnlType(MHalDispDeviceIdType_e enDeviceId)
{
    MS_U8 u8idx = enDeviceId == E_MHAL_DISP_DEVICE_ID_0 ? 0 : 1;
    return stMHalDispDeviceCtx[u8idx].pstPnlType;
}

MS_BOOL _MHalDsipSetXcWindow(MHalDispDeviceCtx_t *pMHalDispCtx, MHAL_DISP_VidWinRect_t *ptSrcWin, MHAL_DISP_VidWinRect_t *ptCropWin, MHAL_DISP_VidWinRect_t *ptDstWin)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_XC
    XC_DEVICE_ID stXcDeviceId;
    PNL_DeviceId stPnlDeviceId;
    PNL_EX_PanelType* pstPnlType = NULL;
    XC_EX_SETWIN_INFO stXcSetWinInfo;
#if ENABLE_UTPA_PQ
    MS_PQ_Dtv_Info stPQDTVInfo;
#endif

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    FILL_UTPA_DEVICE_STRUCT(stPnlDeviceId.u32Id, stPnlDeviceId.u32Version, pMHalDispCtx->enDeviceId);

#if XC_FBL_EN == 1
    MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, ENABLE);
#else
    MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, (pMHalDispCtx->stXcInitData.u32Main_FB_Size == 0) ? ENABLE : DISABLE);
#endif

    //ToDo_Ryan
    //MDrv_PQ_SetRFblMode(FALSE,FALSE);
    //MApi_XC_EX_EnableRequest_FrameBufferLess(&stXcDeviceId, TRUE);
#if ENABLE_UTPA_PQ
    MDrv_PQ_Set_DisplayType(g_IPanelEx.Width(&stPnlDeviceId), PQ_DISPLAY_ONE);
#endif
    MApi_XC_EX_EnableIPAutoNoSignal(&stXcDeviceId, FALSE, E_XC_EX_MAIN_WINDOW);

    memset(&stXcSetWinInfo, 0, sizeof(XC_EX_SETWIN_INFO));
    stXcSetWinInfo.enInputSourceType = pMHalDispCtx->enInputSrc == E_MHAL_DISP_XC_INPUT_SRC_OP ? E_XC_EX_INPUT_SOURCE_SCALER_OP : E_XC_EX_INPUT_SOURCE_DTV;

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1 &&  pMHalDispCtx->enInputSrc == E_MHAL_DISP_XC_INPUT_SRC_OP)
    {
        PNL_DeviceId stPnlDevice0;

        FILL_UTPA_DEVICE_STRUCT(stPnlDevice0.u32Id, stPnlDevice0.u32Version, E_MHAL_DISP_DEVICE_ID_0);

        pstPnlType = _MHalDispGetPnlType(E_MHAL_DISP_DEVICE_ID_0);

        stXcSetWinInfo.stCapWin.y = pstPnlType->m_ucPanelVBackPorch - 2;

        stXcSetWinInfo.stCapWin.x = (g_IPanelEx.HStart(&stPnlDevice0)- g_IPanelEx.HSynWidth(&stPnlDevice0)) - 1;
        stXcSetWinInfo.stCapWin.width = g_IPanelEx.Width(&stPnlDevice0);
        stXcSetWinInfo.stCapWin.height = g_IPanelEx.Height(&stPnlDevice0);

        stXcSetWinInfo.bHDuplicate    = FALSE; //OP does not have HDuplicate mode
        stXcSetWinInfo.u16InputVTotal = g_IPanelEx.HTotal(&stPnlDevice0);
        stXcSetWinInfo.u16DefaultHtotal = g_IPanelEx.VTotal(&stPnlDevice0);
        stXcSetWinInfo.bInterlace     = FALSE; //OP only have progressive mode

        stXcSetWinInfo.u16InputVFreq = g_IPanelEx.DefaultVFreq(&stPnlDevice0);
    #if ENABLE_UTPA_PQ
        stPQDTVInfo.eType = E_PQ_DTV_H264;
        MDrv_PQ_Set_DTVInfo(PQ_MAIN_WINDOW, &stPQDTVInfo);
    #endif
    }
    else if(pMHalDispCtx->enInputSrc == E_MHAL_DISP_XC_INPUT_SRC_MVOP)
    {
        //ToDo_Ryan
    }
    else
    {
        stXcSetWinInfo.bHDuplicate    = FALSE; //OP does not have HDuplicate mode
        stXcSetWinInfo.u16InputVTotal = g_IPanelEx.HTotal(&stPnlDeviceId);
        stXcSetWinInfo.u16DefaultHtotal = g_IPanelEx.VTotal(&stPnlDeviceId);
        stXcSetWinInfo.bInterlace     = FALSE; //OP only have progressive mode
        stXcSetWinInfo.u16InputVFreq = g_IPanelEx.DefaultVFreq(&stPnlDeviceId);
#if ENABLE_UTPA_PQ
        stPQDTVInfo.eType = E_PQ_DTV_H264;
        MDrv_PQ_Set_DTVInfo(PQ_MAIN_WINDOW, &stPQDTVInfo);
#endif
    }

    if (ptSrcWin)
    {
        stXcSetWinInfo.stCapWin.x = ptSrcWin->u16X;
        stXcSetWinInfo.stCapWin.y = ptSrcWin->u16Y;
        stXcSetWinInfo.stCapWin.width = ptSrcWin->u16Width;
        stXcSetWinInfo.stCapWin.height = ptSrcWin->u16Height;
    }

    if (ptCropWin)
    {
        stXcSetWinInfo.stCropWin.x = ptCropWin->u16X;
        stXcSetWinInfo.stCropWin.y = ptCropWin->u16Y;
        stXcSetWinInfo.stCropWin.width = ptCropWin->u16Width;
        stXcSetWinInfo.stCropWin.height = ptCropWin->u16Height;
    }
    else
    {
        stXcSetWinInfo.stCropWin.x = stXcSetWinInfo.stCapWin.x;
        stXcSetWinInfo.stCropWin.y = stXcSetWinInfo.stCapWin.y;
        stXcSetWinInfo.stCropWin.width = stXcSetWinInfo.stCapWin.width;
        stXcSetWinInfo.stCropWin.height = stXcSetWinInfo.stCapWin.height;
    }

    if(ptDstWin)
    {
        stXcSetWinInfo.stDispWin.x = ptDstWin->u16X;
        stXcSetWinInfo.stDispWin.y = ptDstWin->u16Y;
        stXcSetWinInfo.stDispWin.width = ptDstWin->u16Width;
        stXcSetWinInfo.stDispWin.height = ptDstWin->u16Height;
    }
    else
    {
        stXcSetWinInfo.stDispWin.x = 0;
        stXcSetWinInfo.stDispWin.y = 0;
        stXcSetWinInfo.stDispWin.width = g_IPanelEx.Width(&stPnlDeviceId);
        stXcSetWinInfo.stDispWin.height = g_IPanelEx.Height(&stPnlDeviceId);
    }

    stXcSetWinInfo.bHCusScaling = FALSE;
    stXcSetWinInfo.bVCusScaling = FALSE;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "Src(%d %d %d %d), Crop(%d %d %d %d), Disp(%d %d %d %d)\n",
        stXcSetWinInfo.stCapWin.x, stXcSetWinInfo.stCapWin.y, stXcSetWinInfo.stCapWin.width, stXcSetWinInfo.stCapWin.height,
        stXcSetWinInfo.stCropWin.x, stXcSetWinInfo.stCropWin.y, stXcSetWinInfo.stCropWin.width, stXcSetWinInfo.stCropWin.height,
        stXcSetWinInfo.stDispWin.x, stXcSetWinInfo.stDispWin.y, stXcSetWinInfo.stDispWin.width, stXcSetWinInfo.stDispWin.height);

    if(MApi_XC_EX_IsBlackVideoEnable(&stXcDeviceId, E_XC_EX_MAIN_WINDOW) == TRUE)
    {
        MApi_XC_EX_SkipWaitVsync(&stXcDeviceId, E_XC_EX_MAIN_WINDOW, ENABLE);
    }

    if(MApi_XC_EX_SetWindow(&stXcDeviceId, &stXcSetWinInfo, sizeof(XC_EX_SETWIN_INFO), E_XC_EX_MAIN_WINDOW) == FALSE)
    {
        bRet = FALSE;

        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Id=%d, SetWindow fail\n", pMHalDispCtx->enDeviceId);
    }

    MApi_XC_EX_SkipWaitVsync(&stXcDeviceId, E_XC_EX_MAIN_WINDOW, DISABLE);

#if XC_FBL_EN == 1
    MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, ENABLE);
#else
    MApi_XC_EX_EnableFrameBufferLess(&stXcDeviceId, (pMHalDispCtx->stXcInitData.u32Main_FB_Size == 0) ? ENABLE : DISABLE);
#endif

#endif

    return bRet;
}

MS_BOOL _MHalDispChangePnlTiming(MHalDispDeviceCtx_t *pMHalDispCtx, MHAL_DISP_DeviceTiming_e enTiming)
{
    PNL_DeviceId stPnlDeviceId;
    PNL_EX_PanelType * pstPnlType = NULL;
#if ENABLE_UTPA_XC
    XC_EX_PANEL_INFO stPanelInfo;
    XC_DEVICE_ID stXcDeviceId;
#endif


    pstPnlType = _MHalDispGetPanelInfo(enTiming);
    if(pstPnlType == NULL)
    {
        pstPnlType = _MHalDispGetPanelSpec(_MHalDispDevTiming2PnlResType(enTiming));
    }

    if(pstPnlType == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Time (%d) is not correct\n", enTiming);
        return FALSE;
    }

    _MHalDispPatchVsyncStart(pMHalDispCtx, pstPnlType);

    pMHalDispCtx->pstPnlType = pstPnlType;

    FILL_UTPA_DEVICE_STRUCT(stPnlDeviceId.u32Id, stPnlDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    if(enTiming == E_MHAL_DISP_OUTPUT_3840x2160_30)
    {
        MApi_PNL_EX_ForceSetPanelHStart(&stPnlDeviceId, 384, ENABLE);
        MApi_PNL_EX_ForceSetPanelDCLK(&stPnlDeviceId, (pstPnlType->m_wPanelHTotal*pstPnlType->m_wPanelVTotal*30)/1000000, ENABLE);
    }

    MApi_PNL_EX_ChangePanelType(&stPnlDeviceId, pstPnlType);

#if ENABLE_UTPA_XC
    stPanelInfo.u16HStart               = g_IPanelEx.HStart(&stPnlDeviceId);
    stPanelInfo.u16VStart               = g_IPanelEx.VStart(&stPnlDeviceId);
    stPanelInfo.u16Width                = g_IPanelEx.Width(&stPnlDeviceId);
    stPanelInfo.u16Height               = g_IPanelEx.Height(&stPnlDeviceId);
    stPanelInfo.u16HTotal               = g_IPanelEx.HTotal(&stPnlDeviceId);
    stPanelInfo.u16VTotal               = g_IPanelEx.VTotal(&stPnlDeviceId);
    stPanelInfo.u16DefaultVFreq         = g_IPanelEx.DefaultVFreq(&stPnlDeviceId);
    stPanelInfo.u8LPLL_Mode             = g_IPanelEx.LPLL_Mode(&stPnlDeviceId);
    stPanelInfo.enPnl_Out_Timing_Mode   = E_XC_EX_PNL_CHG_VTOTAL;
    stPanelInfo.u16DefaultHTotal        = g_IPanelEx.HTotal(&stPnlDeviceId);
    stPanelInfo.u16DefaultVTotal        = g_IPanelEx.VTotal(&stPnlDeviceId);
    stPanelInfo.u32MinSET               = g_IPanelEx.MinSET(&stPnlDeviceId);
    stPanelInfo.u32MaxSET               = g_IPanelEx.MaxSET(&stPnlDeviceId);

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    MApi_XC_EX_ChangePanelType(&stXcDeviceId, &stPanelInfo);
#endif

    return TRUE;
}



MS_BOOL _MHalDispChangeOutpuResolution(MHalDispDeviceCtx_t *pMHalDispCtx, MHAL_DISP_DeviceTiming_e enTiming, MHalDispDeviceOutputInterfaceType_e enInterface)
{
    E_OUTPUT_VIDEO_TIMING_TYPE enOutputVideoTiming = E_RES_1920x1080P_60Hz;
#if ENABLE_UTPA_XC
    PNL_DeviceId stPnlDeviceId;
    XC_DEVICE_ID stXcDeviceId;
    XC_EX_WINDOW_TYPE stXcDstWin;
#endif

    if( _MHalDispDevTiming2VideoTimingType(enTiming, &enOutputVideoTiming) == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Timing (%d) is not support\n", enTiming);
        return FALSE;
    }


    pMHalDispCtx->enDeviceTiming[enInterface] = enTiming;

    if( _MHalDispChangePnlTiming(pMHalDispCtx, enTiming) == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Change Pnl Timing fail\n");
        return FALSE;
    }

#if ENABLE_UTPA_XC
    FILL_UTPA_DEVICE_STRUCT(stPnlDeviceId.u32Id, stPnlDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    stXcDstWin.x      = g_IPanelEx.HStart(&stPnlDeviceId);
    stXcDstWin.y      = g_IPanelEx.VStart(&stPnlDeviceId);
    stXcDstWin.width  = g_IPanelEx.Width(&stPnlDeviceId);
    stXcDstWin.height = g_IPanelEx.Height(&stPnlDeviceId);

    // Set display window size to avoid gargate output
    MApi_XC_EX_SetDispWinToReg(&stXcDeviceId, &stXcDstWin,  E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_DisableSubWindow(&stXcDeviceId);
#endif

    MApi_DAC_SetYPbPrOutputTiming(enOutputVideoTiming);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT);
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT_SC);
    }
    else
    {
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_SC1_INIT);
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_SC1_INIT_SC);
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_INIT_HDGEN);
        MApi_DAC_DumpTable(NULL, E_DAC_DUMP_TABTYPE_DAC_PLL_INIT);
    }

#if ENABLE_UTPA_XC
    pMHalDispCtx->stDispWin.u16X      = stXcDstWin.x;
    pMHalDispCtx->stDispWin.u16Y      = stXcDstWin.y;
    pMHalDispCtx->stDispWin.u16Width  = stXcDstWin.width;
    pMHalDispCtx->stDispWin.u16Height = stXcDstWin.height;
#endif

    return TRUE;
}

void _MhalDispSetVESrcInfo(MHalDispVeConfig_t *pstVeCfg, MHAL_DISP_DeviceTiming_e enTiming)
{
#if ENABLE_UTPA_VE
    PNL_DeviceId stPnlDeviceId;
    FILL_UTPA_DEVICE_STRUCT(stPnlDeviceId.u32Id, stPnlDeviceId.u32Version, E_MHAL_DISP_DEVICE_ID_0);

    pstVeCfg->u16SrcHtotal  = g_IPanelEx.HTotal(&stPnlDeviceId);
    pstVeCfg->u16SrcVtotal  = g_IPanelEx.VTotal(&stPnlDeviceId);
    pstVeCfg->u16SrcX       = g_IPanelEx.HStart(&stPnlDeviceId);
    pstVeCfg->u16SrcY       = g_IPanelEx.VStart(&stPnlDeviceId);
    pstVeCfg->u16SrcWidth   = g_IPanelEx.Width(&stPnlDeviceId);
    pstVeCfg->u16SrcHeight  = g_IPanelEx.Height(&stPnlDeviceId);
    pstVeCfg->u16SrcFreqx10 = _MHalDispDevTiming2Freqx10(enTiming);
    pstVeCfg->bSrcUpdated   = 1;
#endif
}

MS_BOOL _MHalDispSetVESetMode(MHalDispVeConfig_t *pstVeCfg)
{
#if ENABLE_UTPA_VE
    MS_VE_Set_Mode_Type stVeSetModeType;
    MS_VE_Output_Ctrl stOutputCtrl;
    MS_BOOL u16Div = 1;
    if(pstVeCfg->bSrcUpdated == FALSE)
    {
        return TRUE;
    }

    u16Div = (pstVeCfg->u16SrcWidth == 3840 && pstVeCfg->u16SrcHeight == 2160) ? 2 : 1;

    stVeSetModeType.bHDuplicate      = FALSE;
    stVeSetModeType.u16H_CapSize     = pstVeCfg->u16SrcWidth / u16Div;
    stVeSetModeType.u16V_CapSize     = pstVeCfg->u16SrcHeight / u16Div;
    stVeSetModeType.u16H_CapStart    = pstVeCfg->u16SrcX;
    stVeSetModeType.u16V_CapStart    = pstVeCfg->u16SrcY;
    stVeSetModeType.u16H_SC_CapSize  = pstVeCfg->u16SrcWidth / u16Div;
    stVeSetModeType.u16V_SC_CapSize  = pstVeCfg->u16SrcHeight / u16Div;
    stVeSetModeType.u16H_SC_CapStart = pstVeCfg->u16SrcX;
    stVeSetModeType.u16V_SC_CapStart = pstVeCfg->u16SrcY;
    stVeSetModeType.u16InputVFreq    = pstVeCfg->u16SrcFreqx10;
    stVeSetModeType.bSrcInterlace    = FALSE;

    MDrv_VE_SetMode(&stVeSetModeType);
    stOutputCtrl.bEnable = TRUE;
    stOutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&stOutputCtrl);
#endif
    return TRUE;
}

MS_BOOL _MHalDispSetVEOutputVideoStd(MHalDispVeConfig_t *pstVeCfg)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_VE
    MS_VE_PLL_InCfg stInputCfg;

    if(pstVeCfg->enVideoStd >= MS_VE_PAL_N)
    {
        MDrv_VE_AdjustPositionBase(-VE_BUF_ADJUST_OFFSET, 0);
    }
    else
    {
        MDrv_VE_AdjustPositionBase(0, 0);
    }

    MDrv_VE_SetMemoryBaseAddr(stMhalMemConfig.u32VeFbAddr + VE_BUF_SHIFT_OFFSET, stMhalMemConfig.u32VeFbSize);
    MDrv_VE_SetOutputVideoStd(pstVeCfg->enVideoStd);

    //FrameLock
    if(pstVeCfg->bSrcUpdated)
    {
        stInputCfg.u16VttIn  = pstVeCfg->u16SrcVtotal;
        stInputCfg.u16HttIn  = pstVeCfg->u16SrcHtotal;
        stInputCfg.u16HdeIn  = pstVeCfg->u16SrcWidth;
        stInputCfg.u16VdeIn  = pstVeCfg->u16SrcHeight;
        stInputCfg.u16Vde_St = pstVeCfg->u16SrcY;
        stInputCfg.u16Hde_St = pstVeCfg->u16SrcX;

        if(MDrv_VE_SetFrameLock(&stInputCfg, TRUE))
        {

            bRet = TRUE;
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VE SetFrameLock Fail\n");
            bRet = FALSE;;
        }
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VE Src Not Update \n");
        bRet = FALSE;;
    }
#endif
    return bRet;
}

MHalDispPictureNonLinearCurveType_t * _MHalDispGetNonLinearCurve(MS_U32 u32Interface, MHalDispPictureType_e enPictureType)
{
    MHalDispPictureNonLinearCurveType_t *pNonLinearCruve = NULL;

    if( (u32Interface & MHAL_DISP_INTF_HDMI) == MHAL_DISP_INTF_HDMI)
    {
        pNonLinearCruve = &stPictureNonLiearSetting.stHdmiPictureCurve[enPictureType];
    }
    else if( (u32Interface & MHAL_DISP_INTF_VGA) == MHAL_DISP_INTF_VGA)
    {
        pNonLinearCruve = &stPictureNonLiearSetting.stVgaPictureCurve[enPictureType];

    }
    else
    {
        pNonLinearCruve = NULL;
    }

    return pNonLinearCruve;

}

MS_U8 _MHalDispCalculateNonLinear(MHalDispPictureNonLinearCurveType_t *pNonLinearCurve, MS_U8 u8AdjustValue)
{
    MS_U8 u8Value,u8Y0,u8Y1,u8X0,u8X1,u8Intercept;
    MS_U16 u16DistanceOfY, u16DistanceOfX;

    if(pNonLinearCurve == NULL)
    {
        return 0;
    }

    if (u8AdjustValue < 25)
    {
          u8Y0 = pNonLinearCurve->u8OSD_0;
          u8Y1 = pNonLinearCurve->u8OSD_25;
          u8X0 = 0;
          u8X1 = 25;
    }
    else if (u8AdjustValue < 50)
    {
          u8Y0 = pNonLinearCurve->u8OSD_25;
          u8Y1 = pNonLinearCurve->u8OSD_50;
          u8X0 = 25;
          u8X1 = 50;
    }
    else if (u8AdjustValue < 75)
    {
          u8Y0 = pNonLinearCurve->u8OSD_50;
          u8Y1 = pNonLinearCurve->u8OSD_75;
          u8X0 = 50;
          u8X1 = 75;
    }
    else
    {
          u8Y0 = pNonLinearCurve->u8OSD_75;
          u8Y1 = pNonLinearCurve->u8OSD_100;
          u8X0 = 75;
          u8X1 = 100;
    }

    if (u8Y1 > u8Y0)
    {
        u16DistanceOfY = u8Y1 - u8Y0;
        u16DistanceOfX = u8X1 - u8X0;
        u8Intercept  = u8Y0;
        u8AdjustValue  = u8AdjustValue - u8X0;
    }
    else
    {
        u16DistanceOfY = u8Y0 - u8Y1;
        u16DistanceOfX = u8X1 - u8X0;
        u8Intercept  = u8Y1;
        u8AdjustValue  = u8X1 - u8AdjustValue;
    }

 // printf("u16DistanceOfY %u\n", u16DistanceOfY);
 // printf("u16DistanceOfX %u\n", u16DistanceOfX);
 // printf("u8Intercept %bu\n", u8Intercept);
 // printf("u8AdjustValue %bu\n", u8AdjustValue);

    u8Value = ((MS_U16)u16DistanceOfY*u8AdjustValue/(u16DistanceOfX)) + u8Intercept;
    return u8Value;
}


MS_BOOL _MHalDispAceSetSaturation(MHalDispDeviceCtx_t *pMHalDispCtx, MS_U8 u8Val)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stAceDeviceId;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Device =%d  %02x\n", pMHalDispCtx->enDeviceId, u8Val);

    FILL_UTPA_DEVICE_STRUCT(stAceDeviceId.u32Id, stAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    MApi_XC_ACE_EX_PicSetSaturation(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE, u8Val);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
    }
    else
    {
#if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
#else
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, FALSE);
#endif
    }
#endif
    return TRUE;
}

MS_BOOL _MHalDispAceSetHue(MHalDispDeviceCtx_t *pMHalDispCtx, MS_U8 u8Val)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stAceDeviceId;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Device =%d  %02x\n", pMHalDispCtx->enDeviceId, u8Val);

    FILL_UTPA_DEVICE_STRUCT(stAceDeviceId.u32Id, stAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    MApi_XC_ACE_EX_PicSetHue(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE, u8Val);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
    }
    else
    {
#if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
#else
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, FALSE);
#endif
    }
#endif
    return TRUE;
}

MS_BOOL _MHalDispAceSetContrast(MHalDispDeviceCtx_t *pMHalDispCtx, MS_U8 u8Val)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stAceDeviceId;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Device =%d  %02x\n", pMHalDispCtx->enDeviceId, u8Val);

    FILL_UTPA_DEVICE_STRUCT(stAceDeviceId.u32Id, stAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    MApi_XC_ACE_EX_PicSetContrast(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE, u8Val);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
    }
    else
    {
#if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
#else
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, FALSE);
#endif
    }
#endif
    return TRUE;
}

MS_BOOL _MHalDispAceSetSharpness(MHalDispDeviceCtx_t *pMHalDispCtx, MS_U8 u8Val)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stAceDeviceId;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Device =%d  %02x\n", pMHalDispCtx->enDeviceId, u8Val);

    FILL_UTPA_DEVICE_STRUCT(stAceDeviceId.u32Id, stAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    MApi_XC_ACE_EX_PicSetSharpness(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, u8Val);
#endif
    return TRUE;
}

MS_BOOL _MHalDispAceSetBrigtness(MHalDispDeviceCtx_t *pMHalDispCtx, MS_U8 u8Val)
{
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stAceDeviceId;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Device =%d  %02x\n", pMHalDispCtx->enDeviceId, u8Val);

    FILL_UTPA_DEVICE_STRUCT(stAceDeviceId.u32Id, stAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);

    MApi_XC_ACE_EX_PicSetBrightness(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, u8Val, u8Val, u8Val);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
    }
    else
    {
    #if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, TRUE);
    #else
        MApi_XC_ACE_EX_SetRBChannelRange(&stAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, FALSE);
    #endif
    }
#endif
    return TRUE;
}

MS_BOOL _MHalDispCheckHdmitxValidTiming(MHAL_DISP_DeviceTiming_e enTiming)
{
    if((enTiming >= E_MHAL_DISP_OUTPUT_1080P24 && enTiming <= E_MHAL_DISP_OUTPUT_480P60) ||
        enTiming == E_MHAL_DISP_OUTPUT_3840x2160_30 ||
        enTiming == E_MHAL_DISP_OUTPUT_1024x768_60  ||
        enTiming == E_MHAL_DISP_OUTPUT_1280x1024_60 ||
        enTiming == E_MHAL_DISP_OUTPUT_1440x900_60  ||
        enTiming == E_MHAL_DISP_OUTPUT_1600x1200_60)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL _MHalDispVideoLayerRegUpdate(MHalDispVideoLayerCtx_t *pstVideoLayerCtx)
{
    DrvSclMgwinIoDbfConfig_t stDbfCfg;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx)
    {
        FILL_VERCHK_TYPE(stDbfCfg, stDbfCfg.VerChk_Version, stDbfCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stDbfCfg.enWinId = pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                    E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;
        stDbfCfg.bTrigger = 0;

        if(_DrvSclMgwinIoSetDbfConfig(pstVideoLayerCtx->s32Handler, &stDbfCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            bRet = TRUE;
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame Path fail\n");
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

MS_BOOL _MHalDispDeviceMute(MHalDispDeviceCtx_t *pDevCtx, MS_BOOL bMute)
{
#if ENABLE_UTPA_XC
    XC_DEVICE_ID stXcDeviceId;

    if(pDevCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Device Ctx is NULL\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_MUTE_FUNC, " Device:%d Mute=%d\n", pDevCtx->enDeviceId, bMute);

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pDevCtx->enDeviceId);

    MApi_XC_EX_GenerateBlackVideo(&stXcDeviceId, bMute ? TRUE : FALSE, E_XC_EX_MAIN_WINDOW);
#endif
    return TRUE;
}

MS_BOOL _MHalDispVideoLayerMute(MHalDispVideoLayerCtx_t *pVideoLayerCtx, MS_BOOL bForceMute)
{
#if ENABLE_UTPA_XC
    XC_DEVICE_ID stXcDeviceId;
    MS_BOOL bSrcDisable = 0;
    MS_U8 i;
    MS_BOOL bVideoLayerEn = 0;
    MHalDispDeviceCtx_t *pDevCtx = &stMHalDispDeviceCtx[pVideoLayerCtx->enDeviceId];

    if(pVideoLayerCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer Ctx is NULL\n");
        return FALSE;
    }

    if(bForceMute)
    {
        bSrcDisable = TRUE;
    }
    else
    {
        if(pVideoLayerCtx->enDeviceId != E_MHAL_DISP_DEVICE_ID_NUM)
        {
            for(i=0; i< pDevCtx->u8BindVideoLayerNum; i++)
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_MUTE_FUNC, "DeviceId=%d, BindNum=%d, En=%d, Flag=%x\n", pVideoLayerCtx->enDeviceId, pDevCtx->u8BindVideoLayerNum, pDevCtx->pVideLayerCtx[i]->bEn, pDevCtx->pVideLayerCtx[i]->u32InputPortEnFlag);

                if(pDevCtx->pVideLayerCtx[i]->bEn && pDevCtx->pVideLayerCtx[i]->u32InputPortEnFlag != 0)
                {
                    bVideoLayerEn |= (1 << i);
                }
            }

            bSrcDisable = bVideoLayerEn ? FALSE : TRUE;
        }
        else
        {
            bSrcDisable = TRUE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer_%d Not Bind Device\n", pVideoLayerCtx->enDeviceId);
        }
    }


    MHAL_DISP_DBG(E_MHAL_DISP_DBG_MUTE_FUNC, "DeviceId=%d, bForceMute=%d, SrcDisable=%d\n", pVideoLayerCtx->enDeviceId, bForceMute, bSrcDisable);

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pVideoLayerCtx->enDeviceId);
    MApi_XC_EX_DisableInputSource(&stXcDeviceId, bSrcDisable, E_XC_EX_MAIN_WINDOW);
#endif
    return TRUE;
}


MS_BOOL _MHalDispInputPortMute(MHalDispInputPortCtx_t *pInputPortCtx, MS_BOOL bMute)
{
    DrvSclMgwinIoSubWinOnOffConfig_t stSubWinOnOffCfg;
    MHalDispVideoLayerCtx_t *pVideoLayerCtx = pInputPortCtx->pstVideoLayerCtx;
    MS_BOOL bRet = TRUE;


    if(pInputPortCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPor Ctx is NULL\n");
        return FALSE;
    }
    FILL_VERCHK_TYPE(stSubWinOnOffCfg, stSubWinOnOffCfg.VerChk_Version, stSubWinOnOffCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);

    stSubWinOnOffCfg.enWinId =
        pVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                          E_DRV_SCLMGWIN_IO_WIN_ID_1 :
                                          E_DRV_SCLMGWIN_IO_WIN_ID_2;

    stSubWinOnOffCfg.enSubWinId = _MHalDispInputPortId2SubWinId(pInputPortCtx->enInputPortId);
    stSubWinOnOffCfg.bEnable = FALSE;

    if(bMute)
    {
        stSubWinOnOffCfg.bEnable = TRUE;
    }
    else
    {
        stSubWinOnOffCfg.bEnable = (pInputPortCtx->bEn && pInputPortCtx->bFlip) ? FALSE : TRUE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_MUTE_FUNC, "PortId=%d, bMute=%d, MaskBlack=%d, \n", pInputPortCtx->enInputPortId, bMute, stSubWinOnOffCfg.bEnable);

    if(_DrvSclMgwinIoSetSubWinMaskBlackOnOffConfig(pVideoLayerCtx->s32Handler, &stSubWinOnOffCfg) != E_DRV_SCLMGWIN_IO_ERR_OK)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin SubWin MaskBlack(%d) fail\n", stSubWinOnOffCfg.bEnable);
        bRet = FALSE;
    }
    else
    {
        if(pInputPortCtx->pstVideoLayerCtx->bUpdateReg == TRUE)
        {
            bRet = _MHalDispVideoLayerRegUpdate(pInputPortCtx->pstVideoLayerCtx);
        }
    }

    return bRet;
}

MS_BOOL _MHalDispVideoLayerMuteInputPort(MHalDispVideoLayerCtx_t *pVideoLayerCtx, MS_BOOL bMute)
{
    MS_U8 i;
    MHalDispInputPortCtx_t *pInputPortCtx = NULL;
    MS_BOOL bRet = TRUE;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_MUTE_FUNC, "VieoLyaer_Id:%d, bMute:%d\n", pVideoLayerCtx->enVideoLayerId, bMute);

    if(pVideoLayerCtx)
    {
        for(i=0; i< E_MHAL_DISP_INPUT_PORT_ID_NUM; i++)
        {
            if(pVideoLayerCtx->u32InputPortEnFlag & (1<<1))
            {
                pInputPortCtx = &stMHalDispInputPortCtx[pVideoLayerCtx->enVideoLayerId][i];
                pInputPortCtx->bShow =bMute;
                _MHalDispInputPortMute(pInputPortCtx, bMute);

            }
        }
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer Ctx is Empty \n");
    }
    return bRet;
}


MS_BOOL _MHalDispInputPortEnable(MHalDispInputPortCtx_t *pMhalDispInputPortCtx, MS_BOOL bEnable)
{
    DrvSclMgwinIoSubWinOnOffConfig_t stSubWinOnOffCfg;
    MS_BOOL bRet = TRUE;

    FILL_VERCHK_TYPE(stSubWinOnOffCfg, stSubWinOnOffCfg.VerChk_Version, stSubWinOnOffCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
    stSubWinOnOffCfg.enWinId =
        pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                   E_DRV_SCLMGWIN_IO_WIN_ID_1 :
                                                                   E_DRV_SCLMGWIN_IO_WIN_ID_2;

    stSubWinOnOffCfg.enSubWinId = _MHalDispInputPortId2SubWinId(pMhalDispInputPortCtx->enInputPortId);
    stSubWinOnOffCfg.bEnable = bEnable;

    if(_DrvSclMgwinIoSetSubWinOnOffConfig(pMhalDispInputPortCtx->pstVideoLayerCtx->s32Handler, &stSubWinOnOffCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
    {
        if(pMhalDispInputPortCtx->pstVideoLayerCtx->bUpdateReg == TRUE)
        {
            bRet = _MHalDispVideoLayerRegUpdate(pMhalDispInputPortCtx->pstVideoLayerCtx);
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin SubWin OnOff fail\n");
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Pubic Functions
//-------------------------------------------------------------------------------------------------
// Device: Create/Distroy (scaler0/1)
MS_BOOL MHAL_DISP_DeviceCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32DeviceId, void **pDevCtx)
{
    XC_DEVICE_ID stXcDeviceId;
    MS_BOOL bRet = TRUE;
    static MS_BOOL bFirstTime = TRUE;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "%s %s, DeviceId=%d\n", __DATE__, __TIME__, u32DeviceId);

    if(bFirstTime == TRUE)
    {
    #if !defined(CONFIG_MBOOT)
        if(u32DeviceId == 0)
        {
            mdrv_gpio_init();
        }
    #endif
        memset(&stMhalMemConfig, 0, sizeof(MHalDispMemCfg_t));

        bFirstTime = FALSE;
    }

    if(pstAlloc)
    {
        gstDeviceAllocMemFunc = pstAlloc;
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "pstAlloc is NULL \n");
        return FALSE;
    }

    if(u32DeviceId >= E_MHAL_DISP_DEVICE_ID_NUM)
    {
        *pDevCtx = NULL;
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device ID is Not Correct \n");
    }
    else
    {
        if(gbDeviceCreated[u32DeviceId] == FALSE)
        {
            unsigned long long u64PhyAddr = NULL;

           if( _MHalDispInitVariable(u32DeviceId) == FALSE)
           {
               MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Allocate Buffer fail \n");
               return FALSE;

           }

            if(gstDeviceAllocMemFunc != NULL)
            {
                if(stMHalDispDeviceCtx[u32DeviceId].enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
                {
                    if(XC_SC1_MAIN_DNR_BUF_SIZE  && stMhalMemConfig.u32Xc0MainFbAddr == 0)
                    {
                        gstDeviceAllocMemFunc->alloc(NULL, XC_SC1_MAIN_DNR_BUF_SIZE, &u64PhyAddr);

                        stMhalMemConfig.u32Xc0MainFbAddr = u64PhyAddr;
                        stMhalMemConfig.u32Xc0MainFbSize = XC_SC1_MAIN_DNR_BUF_SIZE;
                        if(stMhalMemConfig.u32Xc0MainFbAddr == 0)
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Allocate Buffer fail \n");
                            return FALSE;
                        }
                        else
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "Allocate DNR Buffer,DeviceId=%d,  Base=%x Size=%d\n",
                                u32DeviceId, stMhalMemConfig.u32Xc0MainFbAddr, stMhalMemConfig.u32Xc0MainFbSize );
                        }
                    }
                #if ENABLE_UTPA_VE
                    if(stMhalMemConfig.u32VeFbAddr == 0)
                    {
                        gstDeviceAllocMemFunc->alloc(NULL, VE_BUF_SIZE, &u64PhyAddr);
                        stMhalMemConfig.u32VeFbAddr = u64PhyAddr;
                        stMhalMemConfig.u32VeFbSize = VE_BUF_SIZE;

                        if(stMhalMemConfig.u32VeFbAddr == 0)
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Allocate Buffer fail \n");
                            return FALSE;
                        }
                        else
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "Allocate VE Buffer, DeviceId=%d, Base=%x, Size=%d\n",
                                u32DeviceId, stMhalMemConfig.u32VeFbAddr, stMhalMemConfig.u32VeFbSize);
                        }
                    }
                    else
                    {
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VE Buffer Already Allocate \n");
                    }
                #endif
                }
                else
                {
                    if(XC_SC2_MAIN_DNR_BUF_SIZE && stMhalMemConfig.u32Xc1MainFbAddr == 0)
                    {
                        gstDeviceAllocMemFunc->alloc(NULL, XC_SC2_MAIN_DNR_BUF_SIZE, &u64PhyAddr);
                        stMhalMemConfig.u32Xc1MainFbAddr = u64PhyAddr;
                        stMhalMemConfig.u32Xc1MainFbSize = XC_SC2_MAIN_DNR_BUF_SIZE;

                        if(stMhalMemConfig.u32Xc1MainFbAddr  == NULL)
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Allocate Buffer fail \n");
                            return FALSE;
                        }
                        else
                        {
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "Allocate DNR Buffer,DeviceId=%d,  Base=%x Size=%d\n",
                                u32DeviceId, stMhalMemConfig.u32Xc1MainFbAddr, stMhalMemConfig.u32Xc1MainFbSize  );
                        }
                    }
                }
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "No Allocate Memory Func Point !!!! \n");
            }

            FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, stMHalDispDeviceCtx[u32DeviceId].enDeviceId);

            stMHalDispDeviceCtx[u32DeviceId].pstPnlType = _MHalDispGetPanelInfo(E_MHAL_DISP_OUTPUT_480P60);
            if(stMHalDispDeviceCtx[u32DeviceId].pstPnlType == NULL)
            {
                stMHalDispDeviceCtx[u32DeviceId].pstPnlType = _MHalDispGetPanelSpec(_MHalDispDevTiming2PnlResType(E_MHAL_DISP_OUTPUT_480P60) );
            }

            _MHalDispPatchVsyncStart(&stMHalDispDeviceCtx[u32DeviceId], stMHalDispDeviceCtx[u32DeviceId].pstPnlType);

            //Init Panel
            if(_MHalDispInitPnl(&stMHalDispDeviceCtx[u32DeviceId]) == FALSE)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Pnl Init Fail \n");
            }

            //Init XC
            if( _MHalDispInitXC(&stMHalDispDeviceCtx[u32DeviceId]) == FALSE)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Xc Init Fail \n");
            }

            // Init ACE for color setting
            _MHalDispInitAce(&stMHalDispDeviceCtx[u32DeviceId], E_MHAL_DISP_XC_WIN_ID_MAIN);

            //Init PQ
            if(_MHalDispInitPQ(&stMHalDispDeviceCtx[u32DeviceId]) == FALSE)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "PQ Init Fail \n");
            }

            //Init DLC
            //ToDo_Ryan

            // Init Mux
        #if ENABLE_UTPA_XC
            MApi_XC_EX_Mux_Init(&stXcDeviceId, _MHalDsipXCInputSourceInputPortMapping);
        #endif

            if( _MHalDispInitDac(&stMHalDispDeviceCtx[u32DeviceId]) == FALSE)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dac Init Fail \n");
            }


            if(stMHalDispDeviceCtx[u32DeviceId].enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
            {
                // Init VE/ DAC
                if(_MHalDispInitVe(&stMHalDispDeviceCtx[E_MHAL_DISP_DEVICE_ID_0]) == FALSE)
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "VE Init Fail \n");
                }
            }
            else
            {
                // Init VGA

            }
        #if ENABLE_UTPA_PQ
            MDrv_BW_LoadInitTable();
        #endif
            if(bRet == TRUE)
            {
                *pDevCtx = (void *)&stMHalDispDeviceCtx[u32DeviceId];
                gbDeviceCreated[u32DeviceId] = TRUE;
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Create Device fail\n");
            }
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Already Init: id=%d\n", u32DeviceId);
            bRet = FALSE;
        }
    }

    return bRet;
}

MS_BOOL MHAL_DISP_DeviceDestroyInstance(void *pDevCtx)
{
    MHalDispDeviceCtx_t *pMHalDispCtx = (MHalDispDeviceCtx_t *)pDevCtx;
#if ENABLE_UTPA_ACE
    XC_ACE_DeviceId stXcAceDeviceId;
#endif
#if ENABLE_UTPA_XC
    XC_EX_ApiStatus stXcApiStatus;
    XC_DEVICE_ID stXcDeviceId;
#endif

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d\n", pMHalDispCtx->enDeviceId);

    if(pMHalDispCtx->enDeviceId >= E_MHAL_DISP_DEVICE_ID_NUM || gbDeviceCreated[pMHalDispCtx->enDeviceId] == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device not inited\n");
        return FALSE;
    }


    _MHalDispDeviceMute(pMHalDispCtx, TRUE);

#if ENABLE_UTPA_XC
    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    MApi_XC_EX_GetStatus(&stXcDeviceId, &stXcApiStatus, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_Mux_DeletePath(&stXcDeviceId, stXcApiStatus.enInputSourceType, E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW);
    MApi_XC_EX_GetStatus(&stXcDeviceId, &stXcApiStatus, E_XC_EX_SUB_WINDOW);
    MApi_XC_EX_Mux_DeletePath(&stXcDeviceId, stXcApiStatus.enInputSourceType, E_XC_EX_OUTPUT_SCALER_SUB_WINDOW);
    MApi_XC_EX_MLoad_Enable(&stXcDeviceId, FALSE);
    MApi_XC_EX_Exit(&stXcDeviceId);
#endif
    gbDeviceCreated[pMHalDispCtx->enDeviceId] = FALSE;

#if ENABLE_UTPA_ACE
    FILL_UTPA_DEVICE_STRUCT(stXcAceDeviceId.u32Id, stXcAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);
    MApi_XC_ACE_EX_Exit(&stXcAceDeviceId);
#endif
    gbXcAceInited[pMHalDispCtx->enDeviceId] = FALSE;

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
#if ENABLE_UTPA_VE
        MDrv_VE_Exit();
#endif
        MApi_DAC_Enable(FALSE, DAC_FOR_VE);
    }
    else
    {
        MApi_DAC_Enable(FALSE, DAC_FOR_VGA);
    }

    return TRUE;
}

// Device: Enbale/Disable
MS_BOOL MHAL_DISP_DeviceEnable(void *pDevCtx, const MS_BOOL bEnable)
{
    MHalDispDeviceCtx_t *pMHalDispCtx = (MHalDispDeviceCtx_t *)pDevCtx;
    MS_BOOL bDAC;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, En=%d\n", pMHalDispCtx->enDeviceId, bEnable);

    bDAC = (pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0) ? DAC_FOR_VE : DAC_FOR_VGA;

    if(bEnable)
    {
        MApi_DAC_Enable(TRUE, bDAC);
        _MHalDispDeviceMute(pMHalDispCtx, FALSE);
    }
    else
    {
        _MHalDispDeviceMute(pMHalDispCtx, TRUE);
        MApi_DAC_Enable(FALSE, bDAC);
    }
    return TRUE;
}

// Device: Scaler0 attach/deattach Scaler1
MS_BOOL MHAL_DISP_DeviceAttach(const void *pSrcDevCtx, const void *pDstDevCtx)
{
    MHalDispDeviceCtx_t *pMHalDispSrcCtx = NULL;
    MHalDispDeviceCtx_t *pMHalDispDstCtx = NULL;
    MS_BOOL bRet = FALSE;

    pMHalDispSrcCtx = (MHalDispDeviceCtx_t *)pSrcDevCtx;
    pMHalDispDstCtx = (MHalDispDeviceCtx_t *)pDstDevCtx;

    if(pMHalDispSrcCtx == NULL || pMHalDispDstCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Src or Dst Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Src=%d, Dest=%d\n", pMHalDispSrcCtx->enDeviceId, pMHalDispDstCtx->enDeviceId);

    if(pMHalDispSrcCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 || pMHalDispDstCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1)
    {
        _MHalDispSetXcConnect(pMHalDispDstCtx, E_MHAL_DISP_XC_INPUT_SRC_OP);

        if(pMHalDispDstCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER])
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer(1) should be Empty \n");
        }
        else
        {
            MHalDispVideoLayerCtx_t *pstDstVideoLayerCtx = &stMHalDispLayerCtx[E_MHAL_DISP_VIDEO_LAYER_ID_1];

            if(_MHalDsipSetXcWindow(pMHalDispDstCtx, NULL, NULL, NULL) == FALSE)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Xc Set Window fail \n");
            }
            else
            {
                gbDeviceAttach = TRUE;

                pMHalDispDstCtx->u8BindVideoLayerNum = 1;
                pMHalDispDstCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER] = pstDstVideoLayerCtx;
                pstDstVideoLayerCtx->enDeviceId = E_MHAL_DISP_DEVICE_ID_1;
                pstDstVideoLayerCtx->u32InputPortEnFlag = 0x01;
                pstDstVideoLayerCtx->bEn = TRUE;

                _MHalDispVideoLayerMute(pstDstVideoLayerCtx, FALSE);

                bRet = TRUE;
            }
        }
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Not support, Src=%d, Dst=%d\n", pMHalDispSrcCtx->enDeviceId, pMHalDispSrcCtx->enDeviceId);
        bRet = FALSE;
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceDetach(const void *pSrcDevCtx, const void *pDstDevCtx)
{
    MHalDispDeviceCtx_t *pMHalDispSrcCtx = NULL;
    MHalDispDeviceCtx_t *pMHalDispDstCtx = NULL;
    MS_BOOL bRet = TRUE;

    pMHalDispSrcCtx = (MHalDispDeviceCtx_t *)pSrcDevCtx;
    pMHalDispDstCtx = (MHalDispDeviceCtx_t *)pDstDevCtx;


    if(pMHalDispSrcCtx == NULL || pMHalDispDstCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Src or Dst Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Src=%d, Dest=%d\n", pMHalDispSrcCtx->enDeviceId, pMHalDispDstCtx->enDeviceId);

    if(pMHalDispSrcCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 || pMHalDispDstCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1)
    {
        if(gbDeviceAttach)
        {
            MHalDispVideoLayerCtx_t *pstDstVideoLayerCtx = &stMHalDispLayerCtx[E_MHAL_DISP_VIDEO_LAYER_ID_1];

            _MHalDispVideoLayerMute(pstDstVideoLayerCtx, TRUE);

            if(_MHalDispSetXcDisConnect(pMHalDispDstCtx, pMHalDispDstCtx->enInputSrc))
            {
                gbDeviceAttach = FALSE;
                pMHalDispDstCtx->u8BindVideoLayerNum = 0;
                pMHalDispDstCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER] = NULL;
                pstDstVideoLayerCtx->enDeviceId = E_MHAL_DISP_DEVICE_ID_NUM;
                pstDstVideoLayerCtx->u32InputPortEnFlag = 0;
                pstDstVideoLayerCtx->bEn = FALSE;
                bRet = TRUE;
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "No Device Attached \n");
                bRet = FALSE;
            }
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "No Device Attached \n");
            bRet = FALSE;
        }
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Not support, Src=%d, Dst=%d\n", pMHalDispSrcCtx->enDeviceId, pMHalDispSrcCtx->enDeviceId);
        bRet = FALSE;
    }
    return bRet;
}

// Device: Set/Get BackGround Color in RGB format
MS_BOOL MHAL_DISP_DeviceSetBackGroundColor(void *pDevCtx, const MS_U32 u32BgColor)
{
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;
    XC_DEVICE_ID stXcDeviceId;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, Color=%d\n", pMHalDispCtx->enDeviceId, u32BgColor);

    FILL_UTPA_DEVICE_STRUCT(stXcDeviceId.u32Id, stXcDeviceId.u32Version, pMHalDispCtx->enDeviceId);
#if ENABLE_UTPA_XC
    MApi_XC_EX_SetFrameColor(&stXcDeviceId, u32BgColor);
#endif

    return TRUE;
}


// Device: Set/Get output to Interface: HDMI/VGA
MS_BOOL MHAL_DISP_DeviceAddOutInterface(void *pDevCtx, const MS_U32 u32Interface)
{
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;
    MS_BOOL bRet = TRUE;
    MS_U32 u32Mask;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, Interface=%x\n", pMHalDispCtx->enDeviceId, u32Interface);

    switch(pMHalDispCtx->enDeviceId)
    {
        case E_MHAL_DISP_DEVICE_ID_0:
            u32Mask = (MHAL_DISP_INTF_HDMI | MHAL_DISP_INTF_CVBS);
            break;

        case E_MHAL_DISP_DEVICE_ID_1:
            u32Mask = (MHAL_DISP_INTF_VGA);
            break;

        default:
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "DeviceId_%d is out of range \n", pMHalDispCtx->enDeviceId);
            return FALSE;
    }

    if( (u32Interface & ~u32Mask) ||  (u32Interface & u32Mask) == 0)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }


    if(bRet == FALSE)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device_Id=%d, Interface:%x, Not support\n", pMHalDispCtx->enDeviceId, u32Interface);
    }
    else
    {
        pMHalDispCtx->u32Interface |=  u32Interface;
    }

    return bRet;
}

// Device: Set output Timing
MS_BOOL MHAL_DISP_DeviceSetOutputTiming(void *pDevCtx, const MS_U32 u32Interface, const MHAL_DISP_DeviceTimingInfo_t *pstTimingInfo)
{
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;

    MS_BOOL bRet = TRUE;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, Interface=%s, Time=%s\n", pMHalDispCtx->enDeviceId, PARSING_MHAL_DISP_INTERFACE(u32Interface), PARSING_MHAL_DISP_OUTPUT_TIMING(pstTimingInfo->eTimeType));

    if(pstTimingInfo->eTimeType == E_MHAL_DISP_OUTPUT_USER)
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "User Definied TimingOut is not supported\n");
    }
    else
    {
        switch(pMHalDispCtx->enDeviceId)
        {
            case E_MHAL_DISP_DEVICE_ID_0:

                if(u32Interface == MHAL_DISP_INTF_CVBS)
                {

                    if(pstTimingInfo->eTimeType <= E_MHAL_DISP_OUTPUT_960H_NTSC)
                    {
                        if(pstTimingInfo->eTimeType == E_MHAL_DISP_OUTPUT_960H_PAL  || pstTimingInfo->eTimeType == E_MHAL_DISP_OUTPUT_960H_NTSC )
                        {
                            bRet = FALSE;
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Video Std (%d) Not Support\n", pstTimingInfo->eTimeType);
                        }
                        else
                        {
                            if(pMHalDispCtx->enDeviceTiming[E_MHAL_DISP_OUPUT_INTERFACE_0] == E_MHAL_DISP_OUTPUT_MAX)
                            {
                                MHAL_DISP_DeviceTiming_e enTiming = pstTimingInfo->eTimeType == E_MHAL_DISP_OUTPUT_PAL ?
                                                                                                E_MHAL_DISP_OUTPUT_576P50 :
                                                                                                E_MHAL_DISP_OUTPUT_480P60;

                                if(_MHalDispChangeOutpuResolution(pMHalDispCtx, enTiming, E_MHAL_DISP_OUPUT_INTERFACE_1) == FALSE)
                                {
                                    bRet = FALSE;
                                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d Change Output Resolution fail\n", pMHalDispCtx->enDeviceId);
                                }
                                else
                                {
                                    _MhalDispSetVESrcInfo(&stVeCfg, enTiming);
                                }
                            }
                            else
                            {
                                _MhalDispSetVESrcInfo(&stVeCfg, pMHalDispCtx->enDeviceTiming[E_MHAL_DISP_OUPUT_INTERFACE_0]);
                            }

                            stVeCfg.enVideoStd = pstTimingInfo->eTimeType == E_MHAL_DISP_OUTPUT_PAL ? MS_VE_PAL : MS_VE_NTSC;

                            if(_MHalDispSetVEOutputVideoStd(&stVeCfg) == FALSE)
                            {
                                bRet = FALSE;
                                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Set VE VideStd Fail\n");
                            }

                            if(_MHalDispSetVESetMode(&stVeCfg) == FALSE)
                            {
                                bRet = FALSE;
                                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Set VE SetMode Fail\n");
                            }
                        }
                    }
                    else
                    {
                        bRet = FALSE;
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Video Std (%d) Not Support\n", pstTimingInfo->eTimeType);
                    }
                }
                else if(u32Interface == MHAL_DISP_INTF_HDMI)
                {
                    if( _MHalDispCheckHdmitxValidTiming(pstTimingInfo->eTimeType) == FALSE )
                    {
                        bRet = FALSE;
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Timing is not support: %s\n", PARSING_MHAL_DISP_OUTPUT_TIMING(pstTimingInfo->eTimeType));
                    }
                    else
                    {
                        if(_MHalDispChangeOutpuResolution(pMHalDispCtx, pstTimingInfo->eTimeType, E_MHAL_DISP_OUPUT_INTERFACE_0) == FALSE)
                        {
                            bRet = FALSE;
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d Change Output Resolution fail\n", pMHalDispCtx->enDeviceId);
                        }
                        else
                        {
                            if( pMHalDispCtx->u32Interface &  MHAL_DISP_INTF_CVBS )
                            {
                                _MhalDispSetVESrcInfo(&stVeCfg, pstTimingInfo->eTimeType);

                                if(_MHalDispSetVEOutputVideoStd(&stVeCfg) == FALSE)
                                {
                                    bRet = FALSE;
                                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Set VE VideStd Fail\n");
                                }

                                if(_MHalDispSetVESetMode(&stVeCfg) == FALSE)
                                {
                                    bRet = FALSE;
                                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Set VE SetMode Fail\n");
                                }
                            }
                        }
                    }
                }
                else
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d , Interface=%d, Change Output Resolution fail\n", pMHalDispCtx->enDeviceId, u32Interface);
                }
                break;

            case E_MHAL_DISP_DEVICE_ID_1:
                if(u32Interface == MHAL_DISP_INTF_VGA)
                {
                    if(_MHalDispChangeOutpuResolution(pMHalDispCtx, pstTimingInfo->eTimeType, E_MHAL_DISP_OUPUT_INTERFACE_0) == FALSE)
                    {
                        bRet = FALSE;
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d Change Output Resolution fail\n", pMHalDispCtx->enDeviceId);
                    }
                }
                else
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d , Interface=%d, Change Output Resolution fail\n", pMHalDispCtx->enDeviceId, u32Interface);
                }
                break;

            default:
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d is not correct\n", pMHalDispCtx->enDeviceId);
                break;
        }
    }

    return bRet;
}

// Device: Set VGA Parameters
MS_BOOL MHAL_DISP_DeviceSetVgaParam(void *pDevCtx, const MHAL_DISP_VgaParam_t *pstVgaInfo)
{
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;
    MHalDispPictureNonLinearCurveType_t *pstNonLinearCurve;
    MS_U8 u8Value;
    MS_BOOL bRet = TRUE;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d Csc=%s, Con:%d, Bri:%d, Hue:%d Sat:%d, Sharpness:%d Gain:%d\n",
        pMHalDispCtx->enDeviceId, PARSING_CSC_MATRIX(pstVgaInfo->stCsc.eCscMatrix),
        pstVgaInfo->stCsc.u32Contrast, pstVgaInfo->stCsc.u32Luma, pstVgaInfo->stCsc.u32Hue, pstVgaInfo->stCsc.u32Saturation,
        pstVgaInfo->u32Sharpness, pstVgaInfo->u32Gain);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d is not correct\n",   pMHalDispCtx->enDeviceId);
        return FALSE;
    }
    else
    {

        if(pstVgaInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BT601_TO_RGB_PC || pstVgaInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BYPASS)
        {
        #if ENABLE_UTPA_XC
            MApi_XC_WriteByte(0x103000, 0x01);
            MApi_XC_Write2ByteMask((0x103000+(0x0D*2)), 0x0000, 0x0001);
        #endif
        }
        else if(pstVgaInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BT709_TO_RGB_PC)
        {
        #if ENABLE_UTPA_XC
            MApi_XC_WriteByte(0x103000, 0x01);
            MApi_XC_Write2ByteMask((0x103000+(0x0D*2)), 0x0001, 0x0001);
        #endif
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "CscMatrix not support: %s\n", PARSING_CSC_MATRIX(pstVgaInfo->stCsc.eCscMatrix));
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_VGA, E_MHAL_DISP_PICTURE_CONTRAST);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstVgaInfo->stCsc.u32Contrast);
            _MHalDispAceSetContrast(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Contrast NonLinearCurve NULL\n");
            bRet = FALSE;
        }

#if SC1_OUTPUT_COLOR == DEVICE_OUTPUT_COLOR_RGB
        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_VGA, E_MHAL_DISP_PICTURE_HUE);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstVgaInfo->stCsc.u32Hue);
            _MHalDispAceSetHue(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Hue NonLinearCurve NULL\n");
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_VGA, E_MHAL_DISP_PICTURE_SATURATION);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstVgaInfo->stCsc.u32Saturation);
            _MHalDispAceSetSaturation(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Saturation NonLinearCurve NULL\n");
            bRet = FALSE;
        }
#endif

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_VGA, E_MHAL_DISP_PICTURE_BRIGHTNESS);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstVgaInfo->stCsc.u32Luma);
            _MHalDispAceSetBrigtness(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Brightness NonLinearCurve NULL\n");
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_VGA, E_MHAL_DISP_PICTURE_SHARPNESS);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstVgaInfo->u32Sharpness);
            _MHalDispAceSetSharpness(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Sharpness NonLinearCurve NULL\n");
            bRet = FALSE;
        }

    }
    return bRet;
}

// Device: Set HDMI Parameters
MS_BOOL MHAL_DISP_DeviceSetHdmiParam(void *pDevCtx, const MHAL_DISP_HdmiParam_t *pstHdmiInfo)
{
    MS_BOOL bRet = TRUE;
#if ENABLE_UTPA_ACE
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;
    MHalDispPictureNonLinearCurveType_t *pstNonLinearCurve;
    MS_U8 u8Value;
    XC_ACE_DeviceId stXcAceDeviceId;
    MS_S16 *pColorCorrectTable = NULL;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, Csc:%s, Con:%d, Bri:%d, Hue:%d Sat:%d\n",
        pMHalDispCtx->enDeviceId, PARSING_CSC_MATRIX(pstHdmiInfo->stCsc.eCscMatrix), pstHdmiInfo->stCsc.u32Contrast,
        pstHdmiInfo->stCsc.u32Luma, pstHdmiInfo->stCsc.u32Hue, pstHdmiInfo->stCsc.u32Saturation);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d is not correct\n", pMHalDispCtx->enDeviceId);
        return FALSE;
    }
    else
    {
        FILL_UTPA_DEVICE_STRUCT(stXcAceDeviceId.u32Id, stXcAceDeviceId.u32Version, pMHalDispCtx->enDeviceId);
        if(pstHdmiInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BT601_TO_BT709)
        {
            pColorCorrectTable = S16YUV601To709FULL;
        }
        else if(pstHdmiInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BT709_TO_BT601)
        {
            pColorCorrectTable = S16YUV709To601FULL;
        }
        else if(pstHdmiInfo->stCsc.eCscMatrix == E_MHAL_DISP_CSC_MATRIX_BYPASS)
        {
            pColorCorrectTable = S16DACColorCorrectionMatrix;
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "CscMatrix not support: %s\n", PARSING_CSC_MATRIX(pstHdmiInfo->stCsc.eCscMatrix));
            bRet = FALSE;
        }

        if(bRet)
        {
        #if ENABLE_UTPA_ACE
            MApi_XC_ACE_EX_ColorCorrectionTable(&stXcAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN, pColorCorrectTable);
            MApi_XC_ACE_EX_SetColorCorrectionTable(&stXcAceDeviceId, E_MHAL_DISP_XC_WIN_ID_MAIN);
        #endif

        #if ENABLE_PQL
            Mhal_PQL_SetColorCorrectTbl(pColorCorrectTable);
        #endif
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_HDMI, E_MHAL_DISP_PICTURE_CONTRAST);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstHdmiInfo->stCsc.u32Contrast);
            _MHalDispAceSetContrast(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Contrast NonLinearCurve NULL\n");
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_HDMI, E_MHAL_DISP_PICTURE_HUE);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstHdmiInfo->stCsc.u32Hue);
            _MHalDispAceSetHue(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Hue NonLinearCurve NULL\n");
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_HDMI, E_MHAL_DISP_PICTURE_SATURATION);
        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstHdmiInfo->stCsc.u32Saturation);
            _MHalDispAceSetSaturation(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Saturation NonLinearCurve NULL\n");
            bRet = FALSE;
        }

        pstNonLinearCurve = _MHalDispGetNonLinearCurve(MHAL_DISP_INTF_HDMI, E_MHAL_DISP_PICTURE_BRIGHTNESS);

        if(pstNonLinearCurve)
        {
            u8Value = _MHalDispCalculateNonLinear(pstNonLinearCurve, (MS_U8)pstHdmiInfo->stCsc.u32Luma);
            _MHalDispAceSetBrigtness(pMHalDispCtx, u8Value);
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Brightness NonLinearCurve NULL\n");
            bRet = FALSE;
        }
    }
#endif
    return bRet;
}

// Device: Set CVBS Parameters
MS_BOOL MHAL_DISP_DeviceSetCvbsParam(void *pDevCtx, const MHAL_DISP_CvbsParam_t *pstCvbsInfo)
{
    MHalDispDeviceCtx_t *pMHalDispCtx =(MHalDispDeviceCtx_t *)pDevCtx;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d\n", pMHalDispCtx->enDeviceId);

    if(pMHalDispCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Device Id_%d is not correct\n", pMHalDispCtx->enDeviceId);
        return FALSE;
    }
    else
    {
        MS_VE_Output_Ctrl stOutputCtrl;
        stOutputCtrl.bEnable = pstCvbsInfo->bEnable;
        stOutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
        MDrv_VE_SetOutputCtrl(&stOutputCtrl);
    }
    return TRUE;
}

// Video Layer: Create/Destroy
MS_BOOL MHAL_DISP_VideoLayerCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32LayerId, void **pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;
    static MS_BOOL bVideoLayerFirstCreate = TRUE;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d\n", u32LayerId);

    if(bVideoLayerFirstCreate == TRUE)
    {
        MS_U16 i;
        if(_DrvSclMgwinIoInit() == FALSE)
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "[MHAL_DISP] SclMgwin Init Fail\n");
            return FALSE;
        }
        else
        {
            for(i=0; i< E_MHAL_DISP_VIDEO_LAYER_ID_NUM; i++)
            {
                stMHalDispLayerCtx[i].enVideoLayerId = E_MHAL_DISP_VIDEO_LAYER_ID_NUM;
                stMHalDispLayerCtx[i].s32Handler = -1;
                stMHalDispLayerCtx[i].enDeviceId = E_MHAL_DISP_DEVICE_ID_NUM;
                stMHalDispLayerCtx[i].bUpdateReg = 1;
                stMHalDispLayerCtx[i].u32InputPortEnFlag = 0;
                stMHalDispLayerCtx[i].bEn = 0;
            }
            bVideoLayerFirstCreate = FALSE;
        }
    }

    if(u32LayerId >= E_MHAL_DISP_VIDEO_LAYER_ID_NUM)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "[MHAL_DISP] layer id is out out Range\n");
        return FALSE;
    }

    if(gbVideoLayerCreated[u32LayerId] == FALSE)
    {
        stMHalDispLayerCtx[u32LayerId].enVideoLayerId = u32LayerId == 0 ? E_MHAL_DISP_VIDEO_LAYER_ID_0 : E_MHAL_DISP_VIDEO_LAYER_ID_1;

        stMHalDispLayerCtx[u32LayerId].s32Handler = _DrvSclMgwinIoOpen();

        stMHalDispLayerCtx[u32LayerId].enDeviceId = E_MHAL_DISP_DEVICE_ID_NUM;

        stMHalDispLayerCtx[u32LayerId].u32InputPortEnFlag = 0;
        stMHalDispLayerCtx[u32LayerId].bEn = 0;
        stMHalDispLayerCtx[u32LayerId].bUpdateReg = 1;

        if(stMHalDispLayerCtx[u32LayerId].s32Handler != -1)
        {
            bRet = TRUE;
            *pVidLayerCtx = &stMHalDispLayerCtx[u32LayerId];
            _MHalDispVideoLayerMute(&stMHalDispLayerCtx[u32LayerId], TRUE);
            gbVideoLayerCreated[u32LayerId] = TRUE;
        }
        else
        {
            bRet = FALSE;
            *pVidLayerCtx = NULL;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Open Fail\n");
        }
    }
    else
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "[MHAL_DISP] layer Already Create\n");
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerDestoryInstance(void *pVidLayerCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MS_BOOL bRet = FALSE;

    if(pstVideoLayerCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d\n", pstVideoLayerCtx->enVideoLayerId);

        _MHalDispVideoLayerMute(pstVideoLayerCtx, TRUE);

        if(_DrvSclMgwinIoRelease(pstVideoLayerCtx->s32Handler) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            gbVideoLayerCreated[pstVideoLayerCtx->enVideoLayerId] = FALSE;
            pstVideoLayerCtx->enVideoLayerId = E_MHAL_DISP_VIDEO_LAYER_ID_NUM;
            pstVideoLayerCtx->enDeviceId = E_MHAL_DISP_DEVICE_ID_NUM;
            pstVideoLayerCtx->s32Handler = -1;
            pstVideoLayerCtx->u32InputPortEnFlag = 0;
            pstVideoLayerCtx->bEn = 0;
            bRet = TRUE;
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Release fail\n");
        }
    }

    return bRet;
}

// Video Layer: Enbale/Disable
MS_BOOL MHAL_DISP_VideoLayerEnable(void *pVidLayerCtx, const MS_BOOL bEnable)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MHalDispDeviceCtx_t  *pDeviceCtx = NULL;
    MS_BOOL bRet = FALSE;

    if(pstVideoLayerCtx)
    {
        DrvSclMgwinIoFrameOnOffConfig_t stFrameOnOffCfg;

        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d, bEn=%d\n", pstVideoLayerCtx->enVideoLayerId, bEnable);

        FILL_VERCHK_TYPE(stFrameOnOffCfg, stFrameOnOffCfg.VerChk_Version, stFrameOnOffCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stFrameOnOffCfg.enWinId =
            pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;
        stFrameOnOffCfg.bEn = bEnable;

        if(bEnable)
        {
            if(_DrvSclMgwinIoSetFrameOnOffConfig(pstVideoLayerCtx->s32Handler, &stFrameOnOffCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
            {
                bRet = TRUE;
            }
            else
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame OnOff fail\n");
            }
            pstVideoLayerCtx->bEn = TRUE;
            _MHalDispVideoLayerMuteInputPort(pstVideoLayerCtx, FALSE);
            _MHalDispVideoLayerMute(pstVideoLayerCtx, FALSE);
        }
        else
        {
            MS_BOOL bPass = TRUE;
            if(pstVideoLayerCtx->enDeviceId != E_MHAL_DISP_DEVICE_ID_NUM)
            {
                pDeviceCtx = &stMHalDispDeviceCtx[pstVideoLayerCtx->enDeviceId];

                if(pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 && pDeviceCtx->u8BindVideoLayerNum == MHAL_DISP_VIDEOLAYER_BIND_MAX)
                {
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Device_%d, is Binding 2 VideoLayer \n", pDeviceCtx->enDeviceId);
                    bPass = FALSE;
                }
            }

            if(bPass)
            {
                pstVideoLayerCtx->bEn = FALSE;
                _MHalDispVideoLayerMute(pstVideoLayerCtx, FALSE);
                if(_DrvSclMgwinIoSetFrameOnOffConfig(pstVideoLayerCtx->s32Handler, &stFrameOnOffCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
                {
                    bRet = TRUE;
                }
                else
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame OnOff fail\n");
                }
            }
            else
            {
                _MHalDispVideoLayerMuteInputPort(pstVideoLayerCtx, TRUE);
                bRet = TRUE;
            }
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}

// Video Layer: Binder/Unbinder device
MS_BOOL MHAL_DISP_VideoLayerBind(void *pVidLayerCtx, void *pDevCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MHalDispDeviceCtx_t *pDeviceCtx = (MHalDispDeviceCtx_t *)pDevCtx;
    DrvSclMgwinIoFramePathConfig_t stFramepathcCfg;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx && pDeviceCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d, DeviceId=%d\n", pstVideoLayerCtx->enVideoLayerId, pDeviceCtx->enDeviceId);


        if((pDeviceCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0  && pDeviceCtx->u8BindVideoLayerNum >= MHAL_DISP_VIDEOLAYER_BIND_MAX) ||
           (pDeviceCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_1  && pDeviceCtx->u8BindVideoLayerNum >= (MHAL_DISP_VIDEOLAYER_BIND_MAX-1))
          )
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Bind Number is out of range, Device=%d, Video=%d, Num=%d\n",
                    pDeviceCtx->enDeviceId, pstVideoLayerCtx->enVideoLayerId, pDeviceCtx->u8BindVideoLayerNum);
        }
        else
        {
            stFramepathcCfg.enWinId =
                pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                    E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;

            stFramepathcCfg.enPathType =
                pDeviceCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0 ?
                                         E_DRV_SCLMGWIN_IO_PATH_SC1 : E_DRV_SCLMGWIN_IO_PATH_SC2;

            FILL_VERCHK_TYPE(stFramepathcCfg, stFramepathcCfg.VerChk_Version, stFramepathcCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
            if(_DrvSclMgwinIoSetFramePathConfig(pstVideoLayerCtx->s32Handler, &stFramepathcCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
            {
                bRet = TRUE;
                pstVideoLayerCtx->enDeviceId = pDeviceCtx->enDeviceId;

                if(pDeviceCtx->u8BindVideoLayerNum <= MHAL_DISP_VIDEOLAYER_BIND_MAX)
                {
                    pDeviceCtx->pVideLayerCtx[pDeviceCtx->u8BindVideoLayerNum] = pstVideoLayerCtx;

                }

                pDeviceCtx->pVideLayerCtx[pDeviceCtx->u8BindVideoLayerNum] = pstVideoLayerCtx;
                pDeviceCtx->u8BindVideoLayerNum++;

                if(pDeviceCtx->u8BindVideoLayerNum == 1)
                {
                    if(_MHalDispSetXcConnect(pDeviceCtx, E_MHAL_DISP_XC_INPUT_SRC_MGWIN) == FALSE)
                    {
                        bRet = FALSE;
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Xc Connect Fail \n");
                    }
                }
            }
            else
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame Path fail\n");
            }


        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}


MS_BOOL MHAL_DISP_VideoLayerUnBind(void *pVidLayerCtx, void *pDevCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MHalDispDeviceCtx_t *pDeviceCtx = (MHalDispDeviceCtx_t *)pDevCtx;
    DrvSclMgwinIoFramePathConfig_t stFramepathcCfg;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx && pDeviceCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d, DeviceId=%d\n", pstVideoLayerCtx->enVideoLayerId, pDeviceCtx->enDeviceId);

        if(pDeviceCtx->u8BindVideoLayerNum == 0)
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Device=%d, BindNum is NULL \n", pDeviceCtx->enDeviceId);
        }
        else
        {
            if(pstVideoLayerCtx->enDeviceId == pDeviceCtx->enDeviceId)
            {
                stFramepathcCfg.enWinId =
                    pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                        E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;

                stFramepathcCfg.enPathType = E_DRV_SCLMGWIN_IO_PATH_NUM;

                FILL_VERCHK_TYPE(stFramepathcCfg, stFramepathcCfg.VerChk_Version, stFramepathcCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
                if(_DrvSclMgwinIoSetFramePathConfig(pstVideoLayerCtx->s32Handler, &stFramepathcCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
                {
                    pstVideoLayerCtx->enDeviceId = E_MHAL_DISP_DEVICE_ID_NUM;
                    pDeviceCtx->pVideLayerCtx[pDeviceCtx->u8BindVideoLayerNum] = NULL;
                    pDeviceCtx->u8BindVideoLayerNum--;

                    if(pDeviceCtx->u8BindVideoLayerNum == 0)
                    {
                        if(_MHalDispSetXcDisConnect(pDeviceCtx, E_MHAL_DISP_XC_INPUT_SRC_MGWIN) == FALSE)
                        {
                            bRet = FALSE;
                            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " XC DisConnect Fail \n");
                        }
                        else
                        {
                            bRet = TRUE;
                        }
                    }
                }
                else
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame Path fail\n");
                }
            }
            else
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Layer UnBindFail, %x != %x\n",
                    pstVideoLayerCtx->enDeviceId, pDeviceCtx->enDeviceId);
            }
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// Video Layer: Set/Get Attribute
MS_BOOL MHAL_DISP_VideoLayerSetAttr(void *pVidLayerCtx, const MHAL_DISP_VideoLayerAttr_t *pstAttr)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    DrvSclMgwinIoFrameConfig_t stFrameCfg;
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx && pstAttr)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DevId=%d, VidLayerId=%d, (%d %d %d %d) (%d %d), PixelFmt=%s\n",
            pstVideoLayerCtx->enDeviceId, pstVideoLayerCtx->enVideoLayerId, pstAttr->stVidLayerDispWin.u16X,
            pstAttr->stVidLayerDispWin.u16Y, pstAttr->stVidLayerDispWin.u16Width, pstAttr->stVidLayerDispWin.u16Height,
            pstAttr->stVidLayerSize.u32Width, pstAttr->stVidLayerSize.u32Height,
            PARSING_MHAL_DISP_PIXEL_FORMAT(pstAttr->ePixFormat));

        if(pstAttr->stVidLayerDispWin.u16X != 0 || pstAttr->stVidLayerDispWin.u16Y != 0)
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLay: X Y (%d %d)must be 0 \n", pstAttr->stVidLayerDispWin.u16X, pstAttr->stVidLayerDispWin.u16Y);
        }

        FILL_VERCHK_TYPE(stFrameCfg, stFrameCfg.VerChk_Version, stFrameCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stFrameCfg.enWinId = pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                 E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;
        stFrameCfg.stWin.u16X = 0;
        stFrameCfg.stWin.u16Y = 0;
        stFrameCfg.stWin.u16Width  = pstAttr->stVidLayerSize.u32Width;
        stFrameCfg.stWin.u16Height = pstAttr->stVidLayerSize.u32Height;

        if(_DrvSclMgwinIoSetFrameConfig(pstVideoLayerCtx->s32Handler, &stFrameCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            MHalDispDeviceCtx_t *pstDevceCtx = NULL;

            if(pstVideoLayerCtx->enDeviceId >= E_MHAL_DISP_DEVICE_ID_NUM)
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Device Id(%d) is not crrect \n", pstVideoLayerCtx->enDeviceId);
            }
            else
            {
                pstDevceCtx = &stMHalDispDeviceCtx[pstVideoLayerCtx->enDeviceId];
                pstDevceCtx->stSrcWin.u16X = 0;
                pstDevceCtx->stSrcWin.u16Y = 0;
                pstDevceCtx->stSrcWin.u16Width = pstAttr->stVidLayerSize.u32Width;
                pstDevceCtx->stSrcWin.u16Height = pstAttr->stVidLayerSize.u32Height;

                pstDevceCtx->stDispWin.u16X = pstAttr->stVidLayerDispWin.u16X;
                pstDevceCtx->stDispWin.u16Y = pstAttr->stVidLayerDispWin.u16Y;
                pstDevceCtx->stDispWin.u16Width  = pstAttr->stVidLayerDispWin.u16Width;
                pstDevceCtx->stDispWin.u16Height = pstAttr->stVidLayerDispWin.u16Height;

                if(_MHalDsipSetXcWindow(pstDevceCtx, &pstDevceCtx->stSrcWin, NULL, &pstDevceCtx->stDispWin) == FALSE)
                {
                    bRet = FALSE;
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Xc Set Window fail \n");
                }
                else
                {
                    bRet = TRUE;
                }
            }
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame Config fail\n");
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// Video layer: Set/Get compress
MS_BOOL MHAL_DISP_VideoLayerSetCompress(void *pVidLayerCtx, const MHAL_DISP_CompressAttr_t* pstCompressAttr)
{
    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Not Support\n");
    return FALSE;
}

// Video layer: Set/Get display priority
MS_BOOL MHAL_DISP_VideoLayerSetPriority(void *pVidLayerCtx, const MS_U32 u32Priority)
{
    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, "Not Support\n");
    return FALSE;
}

// SW Trigger MGWIN switch double buffer
MS_BOOL MHAL_DISP_VideoLayerBufferFire(void *pVidLayerCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx)
    {
        //MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d\n", pstVideoLayerCtx->enVideoLayerId);

        bRet = _MHalDispVideoLayerRegUpdate(pstVideoLayerCtx);
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// SW check MGWIN double buffer already run
MS_BOOL MHAL_DISP_VideoLayerCheckBufferFired(void *pVidLayerCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    DrvSclMgwinIoDbfConfig_t stDbfCfg;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_CHECK_FUNC, "VidLayerId=%d\n", pstVideoLayerCtx->enVideoLayerId);

        FILL_VERCHK_TYPE(stDbfCfg, stDbfCfg.VerChk_Version, stDbfCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stDbfCfg.enWinId = pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                    E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;
        stDbfCfg.bTrigger = 0;

        if(_DrvSclMgwinIoGetDbfConfig(pstVideoLayerCtx->s32Handler, &stDbfCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            bRet = stDbfCfg.bTrigger;
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Frame Path fail\n");
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// Input port: Create/Destroy
MS_BOOL MHAL_DISP_InputPortCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, void *pVidLayerCtx, const MS_U32 u32PortId, void **pCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MS_BOOL bRet = TRUE;
    static MS_BOOL bInputPortFristInit = TRUE;

    if(bInputPortFristInit == TRUE)
    {
        MS_U16 i, j;

        for(i=0; i<E_MHAL_DISP_VIDEO_LAYER_ID_NUM; i++)
        {
            for(j=0; j<E_MHAL_DISP_INPUT_PORT_ID_NUM; j++)
            {
                stMHalDispInputPortCtx[i][j].enInputPortId = j;
                stMHalDispInputPortCtx[i][j].pstVideoLayerCtx = NULL;
                stMHalDispInputPortCtx[i][j].bShow = TRUE;
                stMHalDispInputPortCtx[i][j].bEn = FALSE;
                stMHalDispInputPortCtx[i][j].bFlip = FALSE;
            }
        }
        bInputPortFristInit = FALSE;
    }

    if(pstVideoLayerCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "VidLayerId=%d, Port=%d\n", pstVideoLayerCtx->enVideoLayerId, u32PortId);

        if(u32PortId >= E_MHAL_DISP_INPUT_PORT_ID_NUM)
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Input Port (%d) out of Range\n", u32PortId);
            bRet = FALSE;
        }
        else
        {
            if(pstVideoLayerCtx->s32Handler != -1)
            {
                DrvSclMgwinIoSubWinOnOffConfig_t stSubWinOnOffCfg;

                stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].pstVideoLayerCtx = pstVideoLayerCtx;
                stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].bShow = TRUE;
                stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].bEn = FALSE;
                stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].bFlip = FALSE;
                stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].pstVideoLayerCtx->u32InputPortEnFlag &= ~(1<<u32PortId);

                *pCtx = &stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId];

                FILL_VERCHK_TYPE(stSubWinOnOffCfg, stSubWinOnOffCfg.VerChk_Version, stSubWinOnOffCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);

                stSubWinOnOffCfg.enWinId =  pstVideoLayerCtx->enVideoLayerId ==  E_MHAL_DISP_VIDEO_LAYER_ID_0 ? E_DRV_SCLMGWIN_IO_WIN_ID_1 : E_DRV_SCLMGWIN_IO_WIN_ID_2;
                stSubWinOnOffCfg.enSubWinId = _MHalDispInputPortId2SubWinId(stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId].enInputPortId);
                stSubWinOnOffCfg.bEnable = TRUE;

                bRet = _MHalDispInputPortMute(&stMHalDispInputPortCtx[pstVideoLayerCtx->enVideoLayerId][u32PortId], TRUE);
            }
            else
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer not Created \n");
            }
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortDestroyInstance(const void *pCtx)
{
    MHalDispInputPortCtx_t *pInputPortCtx = (MHalDispInputPortCtx_t *)pCtx;
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "PortId=%d, VideoLyaerId=%d \n",
            pInputPortCtx->enInputPortId, pInputPortCtx->pstVideoLayerCtx->enVideoLayerId);

        if(pInputPortCtx->pstVideoLayerCtx)
        {
            if( _MHalDispInputPortMute(pInputPortCtx, TRUE))
            {
                pInputPortCtx->bEn = 0;
                pInputPortCtx->bFlip = 0;
                pInputPortCtx->pstVideoLayerCtx->u32InputPortEnFlag &= ~(1<<pInputPortCtx->enInputPortId);
                pInputPortCtx->pstVideoLayerCtx = NULL;
            }
            else
            {
            bRet = TRUE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort Mute Fail \n");
            }
        }
        else
        {
            bRet = TRUE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Inpout Port (%d %d) Not Created\n",
               pInputPortCtx->pstVideoLayerCtx->enVideoLayerId, pInputPortCtx->enInputPortId);
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// Input port: Enbale/Disbale
MS_BOOL MHAL_DISP_InputPortEnable(void *pInputPortCtx, const MS_BOOL bEnable)
{
    MHalDispInputPortCtx_t *pMhalDispInputPortCtx = (MHalDispInputPortCtx_t *)pInputPortCtx;
    MS_BOOL bRet = TRUE;

    if(pMhalDispInputPortCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "PortId=%d, VideoLyaerId=%d, bEn=%d\n",
            pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId, pMhalDispInputPortCtx->enInputPortId, bEnable);

        if(bEnable)
        {
            pMhalDispInputPortCtx->pstVideoLayerCtx->u32InputPortEnFlag |= (1<<pMhalDispInputPortCtx->enInputPortId);
            pMhalDispInputPortCtx->bEn = TRUE;


            if(_MHalDispInputPortEnable(pMhalDispInputPortCtx, TRUE))
            {
                if(_MHalDispInputPortMute(pMhalDispInputPortCtx, FALSE))
                {
                    _MHalDispVideoLayerMute(pMhalDispInputPortCtx->pstVideoLayerCtx, FALSE);
                    bRet = TRUE;
                }
                else
                {
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort Un-Mute fail\n");
                    bRet = FALSE;
                }
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort Enable fail\n");
                bRet = FALSE;
            }
        }
        else
        {
            pMhalDispInputPortCtx->pstVideoLayerCtx->u32InputPortEnFlag &= ~(1<<pMhalDispInputPortCtx->enInputPortId);
            pMhalDispInputPortCtx->bEn = FALSE;
            pMhalDispInputPortCtx->bFlip = FALSE;

            _MHalDispVideoLayerMute(pMhalDispInputPortCtx->pstVideoLayerCtx, FALSE);

            if(_MHalDispInputPortMute(pMhalDispInputPortCtx, TRUE))\
            {
                if(_MHalDispInputPortEnable(pMhalDispInputPortCtx, FALSE))
                {
                    bRet = TRUE;
                }
                else
                {
                    MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort Disable fail\n");
                    bRet = FALSE;
                }
            }
            else
            {
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort Mute fail\n");
                bRet = FALSE;
            }
        }

    #if ENABLE_PQL
        if(bRet && pMhalDispInputPortCtx->pstVideoLayerCtx->enDeviceId == E_MHAL_DISP_DEVICE_ID_0)
        {
            Mhal_PQL_UpdatePicture(NULL);
            Mhal_PQL_SetQuality();
        }
    #endif
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}

// Input port: Enbale/Disbale
MS_BOOL MHAL_DISP_InputPortSetAttr(void *pInputPortCtx, const MHAL_DISP_InputPortAttr_t *pstAttr)
{
    MHalDispInputPortCtx_t *pMhalDispInputPortCtx = (MHalDispInputPortCtx_t *)pInputPortCtx;
    DrvSclMgwinIoSubWinConfig_t stSubWinCfg;
    MS_BOOL bRet = TRUE;

    if(pMhalDispInputPortCtx)
    {

        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "PortId=%d, VideoLyaerId=%d, (%d %d %d %d)\n",
            pMhalDispInputPortCtx->enInputPortId,
            pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId,
            pstAttr->stDispWin.u16X, pstAttr->stDispWin.u16Y, pstAttr->stDispWin.u16Width, pstAttr->stDispWin.u16Height);

        FILL_VERCHK_TYPE(stSubWinCfg, stSubWinCfg.VerChk_Version, stSubWinCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stSubWinCfg.enWinId =
            pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                       E_DRV_SCLMGWIN_IO_WIN_ID_1 :
                                                                       E_DRV_SCLMGWIN_IO_WIN_ID_2;

        stSubWinCfg.enSubWinId = _MHalDispInputPortId2SubWinId(pMhalDispInputPortCtx->enInputPortId);
        stSubWinCfg.stWin.u16X      = pstAttr->stDispWin.u16X;
        stSubWinCfg.stWin.u16Y      = pstAttr->stDispWin.u16Y;
        stSubWinCfg.stWin.u16Width  = pstAttr->stDispWin.u16Width;
        stSubWinCfg.stWin.u16Height = pstAttr->stDispWin.u16Height;

        if(_DrvSclMgwinIoSetSubWinConfig(pMhalDispInputPortCtx->pstVideoLayerCtx->s32Handler, &stSubWinCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            if(pMhalDispInputPortCtx->pstVideoLayerCtx->bUpdateReg == TRUE)
            {
                bRet = _MHalDispVideoLayerRegUpdate(pMhalDispInputPortCtx->pstVideoLayerCtx);
            }
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin SubWin OnOff fail\n");
        }

    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}

// Input port: Show
MS_BOOL MHAL_DISP_InputPortShow(void *pInputPortCtx)
{
    MHalDispInputPortCtx_t *pMhalDispInputPortCtx = (MHalDispInputPortCtx_t *)pInputPortCtx;
    MS_BOOL bRet = TRUE;

    if(pMhalDispInputPortCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "PortId=%d VieoLyaerId=%d\n",
            pMhalDispInputPortCtx->enInputPortId, pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId);
        bRet = _MHalDispInputPortMute(pMhalDispInputPortCtx, FALSE);
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}

// Input port: Hide
MS_BOOL MHAL_DISP_InputPortHide(void *pInputPortCtx)
{
    MHalDispInputPortCtx_t *pMhalDispInputPortCtx = (MHalDispInputPortCtx_t *)pInputPortCtx;
    MS_BOOL bRet = TRUE;

    if(pMhalDispInputPortCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "PortId=%d VieoLyaerId=%d\n",
            pMhalDispInputPortCtx->enInputPortId, pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId);
        bRet = _MHalDispInputPortMute(pMhalDispInputPortCtx, TRUE);
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }

    return bRet;
}


// Input port: Attr batch process, All Channel setting enter batch process.
MS_BOOL MHAL_DISP_InputPortAttrBegin(void *pVidLayerCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "VidLayId=%d\n", pstVideoLayerCtx->enVideoLayerId);
        pstVideoLayerCtx->bUpdateReg = FALSE;
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}
MS_BOOL MHAL_DISP_InputPortAttrEnd(void *pVidLayerCtx)
{
    MHalDispVideoLayerCtx_t *pstVideoLayerCtx = (MHalDispVideoLayerCtx_t *)pVidLayerCtx;
    MS_BOOL bRet = TRUE;

    if(pstVideoLayerCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FUNC, "VidLayId=%d\n", pstVideoLayerCtx->enVideoLayerId);

        bRet = _MHalDispVideoLayerRegUpdate(pstVideoLayerCtx);
        pstVideoLayerCtx->bUpdateReg = TRUE;
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

// Input port: Flip
MS_BOOL MHAL_DISP_InputPortFlip(void *pInputPortCtx, MHAL_DISP_VideoFrameData_t *pstVideoFrameBuffer)
{
    MHalDispInputPortCtx_t *pMhalDispInputPortCtx = (MHalDispInputPortCtx_t *)pInputPortCtx;
    DrvSclMgwinIoSubWinFlipConfig_t stSubWinFlipCfg;
    MS_BOOL bRet = TRUE;
    MS_U32 u32Stride;

    if(pMhalDispInputPortCtx)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_INPUT_PORT_FLIP, "PortId=%d, VideoLyaerId:%d Pixel=%s, Compress=%s, (%d %d) (%x %x)\n",
            pMhalDispInputPortCtx->enInputPortId,
            pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId,
            PARSING_MHAL_DISP_PIXEL_FORMAT(pstVideoFrameBuffer->ePixelFormat),
            PARSING_MHAL_DISP_COMPRESS_MD(pstVideoFrameBuffer->eCompressMode),
            pstVideoFrameBuffer->u32Width, pstVideoFrameBuffer->u32Height,
            (unsigned int)pstVideoFrameBuffer->aPhyAddr[0], pstVideoFrameBuffer->au32Stride[0]);

        FILL_VERCHK_TYPE(stSubWinFlipCfg, stSubWinFlipCfg.VerChk_Version, stSubWinFlipCfg.VerChk_Size, DRV_SCLMGWIN_VERSION);
        stSubWinFlipCfg.enWinId =
            pMhalDispInputPortCtx->pstVideoLayerCtx->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0 ?
                                                                       E_DRV_SCLMGWIN_IO_WIN_ID_1 :
                                                                       E_DRV_SCLMGWIN_IO_WIN_ID_2;

        stSubWinFlipCfg.enSubWinId = _MHalDispInputPortId2SubWinId(pMhalDispInputPortCtx->enInputPortId);

        u32Stride = pstVideoFrameBuffer->au32Stride[0];

        if(_MHalDispPixelFmt2MemFmt(
            pstVideoFrameBuffer->ePixelFormat,
            pstVideoFrameBuffer->eCompressMode == E_MHAL_DISP_COMPRESS_MODE_NONE ? FALSE : TRUE,
            &stSubWinFlipCfg.enMemFmt,
            &u32Stride))
        {
            stSubWinFlipCfg.u32BaseAddr = pstVideoFrameBuffer->aPhyAddr[0];
            stSubWinFlipCfg.u32Pitch    = u32Stride;

            if(_DrvSclMgwinIoSetSubWinFlipConfig(pMhalDispInputPortCtx->pstVideoLayerCtx->s32Handler, &stSubWinFlipCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
            {
                pMhalDispInputPortCtx->bFlip = TRUE;

                if(pMhalDispInputPortCtx->bShow)
                {
                    if(_MHalDispInputPortMute(pMhalDispInputPortCtx, FALSE))
                    {
                        bRet = TRUE;
                    }
                    else
                    {
                        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " InputPort un-Mute fail\n");
                        bRet = FALSE;
                    }
                }

                if(bRet && pMhalDispInputPortCtx->pstVideoLayerCtx->bUpdateReg == TRUE)
                {
                    bRet = _MHalDispVideoLayerRegUpdate(pMhalDispInputPortCtx->pstVideoLayerCtx);
                }
            }
            else
            {
                bRet = FALSE;
                MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin SubWin Flip fail\n");
            }
        }
        else
        {
            bRet = FALSE;
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Pixel Format (P:%d, C:%d)is not support\n",pstVideoFrameBuffer->ePixelFormat,  pstVideoFrameBuffer->eCompressMode);
        }
    }
    else
    {
        bRet = FALSE;
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Context is Empty\n");
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DbgLevel(void *p)
{
    MS_U32 *pu32Val = (MS_U32 *)p;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, " DbgVal=%d\n", *pu32Val);

    gu32MHAL_DISP_DbgLevel = *pu32Val | E_MHAL_DISP_DBG_ERR;

    return TRUE;
}


MS_BOOL MHAL_DISP_InitPanelConfig(MHAL_DISP_PanelConfig_t* pstPanelConfig, MS_U8 u8Size)
{
    MS_U8 u8Count = 0;

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "Size=%d\n", u8Size);

    if(pstPanelConfig == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " pstPanelConfig is NULL\n");
        return FALSE;
    }

    if(u8Size > E_MHAL_DISP_OUTPUT_MAX)
    {
        u8Size = E_MHAL_DISP_OUTPUT_MAX;
    }

    if(gpstPanelConfig == NULL)
    {
#if defined( MSOS_TYPE_NOS)
        gpstPanelConfig = malloc((sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX));
#else
        gpstPanelConfig = kmalloc((sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX), GFP_KERNEL);
#endif
        memset(gpstPanelConfig, 0, (sizeof(MHAL_DISP_PanelConfig_t) * E_MHAL_DISP_OUTPUT_MAX));
    }

    memcpy(gpstPanelConfig, pstPanelConfig, (sizeof(MHAL_DISP_PanelConfig_t) * u8Size));
    gu8PanelSize = u8Size;
    for(u8Count = 0; u8Count < u8Size; u8Count++)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_XC, "Timing = %d, dev = %d, bValid = %d \n",
            gpstPanelConfig[u8Count].eTiming, gpstPanelConfig[u8Count].u32OutputDev, gpstPanelConfig[u8Count].bValid == TRUE);
    }
    return TRUE;
}

MS_BOOL MHAL_DISP_InitMmapConfig(MHAL_DISP_MmapType_e eMemType, MHAL_DISP_MmapInfo_t* pstMmapInfo)
{
    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "eMemType=%d\n", eMemType);

    if(pstMmapInfo == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " pstPanelConfig is NULL\n");
        return FALSE;
    }


    if(eMemType >= E_MHAL_DISP_MMAP_MAX)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " eMemType(%d) is out of range\n", eMemType);
        return FALSE;
    }

    stMmapInfo[eMemType].u32Addr = pstMmapInfo->u32Addr;
    stMmapInfo[eMemType].u32Align = pstMmapInfo->u32Align;
    stMmapInfo[eMemType].u8MiuNo = pstMmapInfo->u8MiuNo;
    stMmapInfo[eMemType].u8CMAHid = pstMmapInfo->u8CMAHid;
    stMmapInfo[eMemType].u8Gid = pstMmapInfo->u8Gid;
    stMmapInfo[eMemType].u8Layer = pstMmapInfo->u8Layer;
    stMmapInfo[eMemType].u32MemoryType = pstMmapInfo->u32MemoryType;
    stMmapInfo[eMemType].u32Size = pstMmapInfo->u32Size;

    return TRUE;
}

MS_BOOL MHAL_DISP_GetDevIrq(void *pDevCtx, MS_U32* pu32DevIrq)
{
    MHalDispDeviceCtx_t *pMHalDispCtx = (MHalDispDeviceCtx_t *)pDevCtx;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Dev Ctx is Empty\n");
        return FALSE;
    }

    MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d\n", pMHalDispCtx->enDeviceId);

    *pu32DevIrq = 42;

    return TRUE;
}


MS_BOOL MHAL_DISP_EnableDevIrq(void *pDevCtx, MS_U32 u32DevIrq, MS_BOOL bEnable)
{
    #define REG_MGWIN_0_INT_MSK   (0x130000 + (0x54<<1))
    #define REG_MGWIN_1_INT_MSK   (0x130000 + (0x55<<1))
    #define BIT_MGWIN_0_INT_FIRE  (0x0100)
    #define BIT_MGWIN_1_INT_FIRE  (0x0010)
    MS_U8 i;
    MHalDispDeviceCtx_t *pMHalDispCtx = (MHalDispDeviceCtx_t *)pDevCtx;
    DrvSclMgwinIoRegisterConfig_t stRegsterCfg;
    MS_BOOL bRet = TRUE;

    if(pMHalDispCtx == NULL)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " Dev Ctx is Empty\n");
        return FALSE;
    }

    if(pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_MASTER] == NULL &&  pMHalDispCtx->pVideLayerCtx[MHAL_DISP_VIDEOLAYER_SECONDARY])
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " VideoLayer Ctx is Empty\n");
        return FALSE;
    }

    for(i=0; i<pMHalDispCtx->u8BindVideoLayerNum; i++)
    {
        MHAL_DISP_DBG(E_MHAL_DISP_DBG_FUNC, "DeviceId=%d, VideoLayerId=%d, Irq=%d, En=%d\n",
            pMHalDispCtx->enDeviceId, pMHalDispCtx->pVideLayerCtx[i]->enVideoLayerId, u32DevIrq, bEnable);

        if(pMHalDispCtx->pVideLayerCtx[i]->enVideoLayerId == E_MHAL_DISP_VIDEO_LAYER_ID_0)
        {
            stRegsterCfg.u32Addr = REG_MGWIN_0_INT_MSK;
            stRegsterCfg.u16Val  = bEnable ?  0 : BIT_MGWIN_0_INT_FIRE;
            stRegsterCfg.u16Msk  = BIT_MGWIN_0_INT_FIRE;
        }
        else
        {
            stRegsterCfg.u32Addr = REG_MGWIN_1_INT_MSK;
            stRegsterCfg.u16Val  = bEnable ? 0 :BIT_MGWIN_1_INT_FIRE;
            stRegsterCfg.u16Msk  = BIT_MGWIN_1_INT_FIRE;
        }

        if(_DrvSclMgwinIoSetRegisterConfig(pMHalDispCtx->pVideLayerCtx[i]->s32Handler, &stRegsterCfg) == E_DRV_SCLMGWIN_IO_ERR_OK)
        {
            gbVideoLayerIrqEnable[pMHalDispCtx->pVideLayerCtx[i]->enVideoLayerId] = bEnable;
            bRet = TRUE;
        }
        else
        {
            MHAL_DISP_DBG(E_MHAL_DISP_DBG_ERR, " SclMgwin Set Register fail\n");
            bRet = FALSE;
            break;
        }
    }

    return bRet;
}


MS_BOOL MHAL_DISP_ClearDevInterrupt(void *pDevCtx, void* pData)
{
    #define REG_MGWIN_0_INT_CLEAR   (0x130000 + (0x52<<1) + 1)
    #define REG_MGWIN_1_INT_CLEAR   (0x130000 + (0x53<<1))
    #define BIT_MGWIN_0_INT_CLEAR    (0x01)
    #define BIT_MGWIN_1_INT_CLEAR    (0x10)
    MS_BOOL bRet =TRUE;

    MHAL_DISP_IRQFlag_t *pstIrqFlag = (MHAL_DISP_IRQFlag_t *)pData;

    if((pstIrqFlag->u32IrqMask & 0x01) && (pstIrqFlag->u32IrqFlag & 0x01))
    {
    #if ENABLE_UTPA_XC
        MApi_XC_WriteByteMask(REG_MGWIN_0_INT_CLEAR, BIT_MGWIN_0_INT_CLEAR, BIT_MGWIN_0_INT_CLEAR);
    #endif
    }

    if((pstIrqFlag->u32IrqMask & 0x02) && (pstIrqFlag->u32IrqFlag & 0x02))
    {
    #if ENABLE_UTPA_XC
        MApi_XC_WriteByteMask(REG_MGWIN_1_INT_CLEAR, BIT_MGWIN_1_INT_CLEAR, BIT_MGWIN_1_INT_CLEAR);
    #endif
    }

    return bRet;
}

MS_BOOL MHAL_DISP_GetDevIrqFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag)
{
#define REG_MGWIN_0_INT_STATUS   (0x130000 + (0x50<<1) + 1)
#define REG_MGWIN_1_INT_STATUS   (0x130000 + (0x51<<1))
#define BIT_MGWIN_0_INT_STATUS   (0x01)
#define BIT_MGWIN_1_INT_STATUS   (0x10)

    pstIrqFlag->u32IrqMask  = 0;
    pstIrqFlag->u32IrqMask |= (gbVideoLayerIrqEnable[E_MHAL_DISP_VIDEO_LAYER_ID_0]) ? 0x01 : 0x00;
    pstIrqFlag->u32IrqMask |= (gbVideoLayerIrqEnable[E_MHAL_DISP_VIDEO_LAYER_ID_1]) ? 0x02 : 0x00;

#if ENABLE_UTPA_XC
    pstIrqFlag->u32IrqFlag = 0;
    pstIrqFlag->u32IrqFlag |= (MApi_XC_ReadByte(REG_MGWIN_0_INT_STATUS) & BIT_MGWIN_0_INT_STATUS) ?  1 : 0;
    pstIrqFlag->u32IrqFlag |= (MApi_XC_ReadByte(REG_MGWIN_1_INT_STATUS) & BIT_MGWIN_1_INT_STATUS) ?  2 : 0;
#endif
    return TRUE;;
}

#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MHAL_DISP_GetDevIrq);
EXPORT_SYMBOL(MHAL_DISP_EnableDevIrq);
EXPORT_SYMBOL(MHAL_DISP_ClearDevInterrupt);
EXPORT_SYMBOL(MHAL_DISP_InitPanelConfig);
EXPORT_SYMBOL(MHAL_DISP_InitMmapConfig);
EXPORT_SYMBOL(MHAL_DISP_DeviceCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_DeviceDestroyInstance);
EXPORT_SYMBOL(MHAL_DISP_DeviceEnable);
EXPORT_SYMBOL(MHAL_DISP_DeviceAttach);
EXPORT_SYMBOL(MHAL_DISP_DeviceDetach);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetBackGroundColor);
EXPORT_SYMBOL(MHAL_DISP_DeviceAddOutInterface);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetOutputTiming);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetVgaParam);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetHdmiParam);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetCvbsParam);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerDestoryInstance);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerEnable);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerBind);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerUnBind);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerSetAttr);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerSetCompress);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerSetPriority);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerBufferFire);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerCheckBufferFired);
EXPORT_SYMBOL(MHAL_DISP_InputPortCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_InputPortDestroyInstance);
EXPORT_SYMBOL(MHAL_DISP_InputPortEnable);
EXPORT_SYMBOL(MHAL_DISP_InputPortSetAttr);
EXPORT_SYMBOL(MHAL_DISP_InputPortShow);
EXPORT_SYMBOL(MHAL_DISP_InputPortHide);
EXPORT_SYMBOL(MHAL_DISP_InputPortAttrBegin);
EXPORT_SYMBOL(MHAL_DISP_InputPortAttrEnd);
EXPORT_SYMBOL(MHAL_DISP_InputPortFlip);
EXPORT_SYMBOL(MHAL_DISP_DbgLevel);
EXPORT_SYMBOL(gu32MHAL_DISP_DbgLevel);
EXPORT_SYMBOL(MHAL_DISP_GetDevIrqFlag);
#endif
