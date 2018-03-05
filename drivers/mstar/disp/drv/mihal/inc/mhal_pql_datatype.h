#ifndef _MHAL_PQL_DATATYPE_H
#define _MHAL_PQL_DATATYPE_H

#include "MsTypes.h"
//#include "drvXC_IOPort.h"
//#include "apiXC.h"
//#include "apiPNL.h"
#include "apiXC_Dlc.h"

#define MAPI_ENABLE_10_BIT_DLC   0
#define DYNAMIC_NR_TBL_NOISE_LEVEL_NUM     5

typedef enum
{
    ///  MFC level off
    E_MFC_LEVEL_OFF,
    ///  MFC level low
    E_MFC_LEVEL_LOW,
    ///  MFC level middle
    E_MFC_LEVEL_MID,
    ///  MFC level high
    E_MFC_LEVEL_HIGH,
    ///  MFC level bypass, its same as bypass 64 mode
    E_MFC_LEVEL_BYPASS,
    ///  MFC level bypass 64, one kind framerate converter of bypass.
    E_MFC_LEVEL_BYPASS_64 = E_MFC_LEVEL_BYPASS,
    ///  MFC level bypass 55, one kind framerate convertr of bypass.
    E_MFC_LEVEL_BYPASS_55,
}MAPI_PQL_MFC_LEVEL;

typedef enum
{
    E_MAPI_PQL_NR_OFF       = 0x00,
    E_MAPI_PQL_NR_LOW       = 0x01,
    E_MAPI_PQL_NR_MID       = 0x02,
    E_MAPI_PQL_NR_HIGH      = 0x03,
    E_MAPI_PQL_NR_AUTO      = 0x04,
    E_MAPI_PQL_NR_DEFAULT   = 0xFF,
} MAPI_PQL_NR_LEVEL;

typedef enum
{
    E_MAPI_PQL_PIP_MODE_OFF,
    E_MAPI_PQL_PIP_MODE_PIP,
    E_MAPI_PQL_PIP_MODE_POP,
    E_MAPI_PQL_PIP_MODE_TRAVELING,
    E_MAPI_PQL_PIP_MODE_INVALID,
} MAPI_PQL_PIP_MODE;

typedef enum
{
    E_MAPI_PQL_MPEG_NR_OFF     = 0x00,
    E_MAPI_PQL_MPEG_NR_LOW     = 0x01,
    E_MAPI_PQL_MPEG_NR_MIDDLE  = 0x02,
    E_MAPI_PQL_MPEG_NR_HIGHT   = 0x03,
    E_MAPI_PQL_MPEG_NR_DEFAULT = 0xFF,
} MAPI_PQL_MPEG_NR_LEVEL_TYPE;

typedef enum
{
    E_MAPI_AUTO_NR_STATUS_OFF = 0x00,
    E_MAPI_AUTO_NR_STATUS_LOW = 0x01,
    E_MAPI_AUTO_NR_STATUS_MID = 0x02,
    E_MAPI_AUTO_NR_STATUS_HIGH = 0x03,
} MAPI_PQL_AUTO_NR_STATUS;

/// the input source type
typedef enum
{
    MAPI_INPUT_SOURCE_VGA,          ///<VGA input     0
    MAPI_INPUT_SOURCE_ATV,          ///<TV input      1

    MAPI_INPUT_SOURCE_CVBS,         ///<AV 1           2
    MAPI_INPUT_SOURCE_CVBS2,        ///<AV 2           3
    MAPI_INPUT_SOURCE_CVBS3,        ///<AV 3           4
    MAPI_INPUT_SOURCE_CVBS4,        ///<AV 4           5
    MAPI_INPUT_SOURCE_CVBS5,        ///<AV 5           6
    MAPI_INPUT_SOURCE_CVBS6,        ///<AV 6           7
    MAPI_INPUT_SOURCE_CVBS7,        ///<AV 7           8
    MAPI_INPUT_SOURCE_CVBS8,        ///<AV 8           9
    MAPI_INPUT_SOURCE_CVBS_MAX,     ///<AV max         10

    MAPI_INPUT_SOURCE_SVIDEO,       ///<S-video 1      11
    MAPI_INPUT_SOURCE_SVIDEO2,      ///<S-video 2      12
    MAPI_INPUT_SOURCE_SVIDEO3,      ///<S-video 3      13
    MAPI_INPUT_SOURCE_SVIDEO4,      ///<S-video 4      14
    MAPI_INPUT_SOURCE_SVIDEO_MAX,   ///<S-video max    15

    MAPI_INPUT_SOURCE_YPBPR,        ///<Component 1    16
    MAPI_INPUT_SOURCE_YPBPR2,       ///<Component 2    17
    MAPI_INPUT_SOURCE_YPBPR3,       ///<Component 3    18
    MAPI_INPUT_SOURCE_YPBPR_MAX,    ///<Component max   19

    MAPI_INPUT_SOURCE_SCART,        ///<Scart 1         20
    MAPI_INPUT_SOURCE_SCART2,       ///<Scart 2         21
    MAPI_INPUT_SOURCE_SCART_MAX,    ///<Scart max       22

    MAPI_INPUT_SOURCE_HDMI,         ///<HDMI 1          23
    MAPI_INPUT_SOURCE_HDMI2,        ///<HDMI 2          24
    MAPI_INPUT_SOURCE_HDMI3,        ///<HDMI 3          25
    MAPI_INPUT_SOURCE_HDMI4,        ///<HDMI 4          26
    MAPI_INPUT_SOURCE_HDMI_MAX,     ///<HDMI max         27

    MAPI_INPUT_SOURCE_DTV,          ///<DTV              28

    MAPI_INPUT_SOURCE_DVI,          ///<DVI 1            29
    MAPI_INPUT_SOURCE_DVI2,         ///<DVI 2            30
    MAPI_INPUT_SOURCE_DVI3,         ///<DVI 2            31
    MAPI_INPUT_SOURCE_DVI4,         ///<DVI 4            32
    MAPI_INPUT_SOURCE_DVI_MAX,      ///<DVI max          33

    // Application source
    MAPI_INPUT_SOURCE_STORAGE,  ///<Storage              34
    MAPI_INPUT_SOURCE_KTV,      ///<KTV                  35
    MAPI_INPUT_SOURCE_JPEG,     ///<JPEG                 36
    MAPI_INPUT_SOURCE_DTV2,      ///<DTV2                37
    MAPI_INPUT_SOURCE_STORAGE2,     ///<Storage2          38
    MAPI_INPUT_SOURCE_DTV3,      ///<DTV3                 39
	MAPI_INPUT_SOURCE_SCALER_OP, ///< video from op        40
    MAPI_INPUT_SOURCE_RVU,          ///<RVU            41
    MAPI_INPUT_SOURCE_NUM,      ///<number of the source   42
    MAPI_INPUT_SOURCE_NONE = MAPI_INPUT_SOURCE_NUM,    ///<NULL input
} MAPI_INPUT_SOURCE_TYPE;

/// the PQ window
typedef enum
{
    /// main window
    MAPI_PQ_MAIN_WINDOW = 0,
    /// sub window
    MAPI_PQ_SUB_WINDOW = 1,
    /// counts of this enum
    MAPI_PQ_MAX_WINDOW,
} MAPI_PQ_WIN;

typedef struct mhal_pql_ColorTemperature_s{
    MS_U32 u32R_Gain;
    MS_U32 u32G_Gain;
    MS_U32 u32B_Gain;
    MS_U32 u32R_Offset;
    MS_U32 u32G_Offset;
    MS_U32 u32B_Offset;

    MS_U32 u32Brightness;
    MS_U32 u32Contrast;
    MS_U32 u32Hue;
    MS_U32 u32Saturation;
    MS_U32 u32Sharpness;
}mhal_pql_ColorTemperature_t;

typedef struct MAPI_PQL_COLOR_TEMPEX_DATA_s
{
    MS_U16 u16RedGain;
    MS_U16 u16GreenGain;
    MS_U16 u16BlueGain;
    MS_U16 u16RedOffset;
    MS_U16 u16GreenOffset;
    MS_U16 u16BlueOffset;
} MAPI_PQL_COLOR_TEMPEX_DATA;

typedef struct MAPI_PWM_INIT_PARAM_s
{
    /// the period value
    MS_U32 u32PeriodPWM;
    /// the duty value
    MS_U32 u32DutyPWM;
    /// PWM channel number
    MS_U16 PWMPort ;
    /// the div value
    MS_U16 u16DivPWM;
    /// True/False for enable/disable
    MS_BOOL bOenPWM;
    /// True/False for Inverse/Non-inverse
    MS_BOOL bPolPWM;
    /// True/False for Enable/Disable
    MS_BOOL bVdbenPWM;
    /// True/False for Enable/Disable
    MS_BOOL bDbenPWM;
    /// True/False for Enable/Disable
    MS_BOOL bBakclightFreq2Vfreq;
}MAPI_PWM_INIT_PARAM;

typedef struct MAPI_AUTO_NR_INIT_PARAM_s
{
    MS_U16 u16AutoNr_L2M_Thr;
    MS_U16 u16AutoNr_M2L_Thr;
    MS_U16 u16AutoNr_M2H_Thr;
    MS_U16 u16AutoNr_H2M_Thr;
    MS_U8 u8DebugLevel;
}MAPI_AUTO_NR_INIT_PARAM;

typedef enum
{
    ///< main window if with PIP or without PIP
    MAPI_MAIN_WINDOW = 0,
    ///< sub window if PIP
    MAPI_SUB_WINDOW  = 1,
    MAPI_MAX_WINDOW
} MAPI_SCALER_WIN;

typedef struct
{
    MS_U8 u8CoringOffset;
    MS_U8 u8SharpnessOffset;
    MS_U8 u8NM_V;
    MS_U8 u8NM_H_0;
    MS_U8 u8NM_H_1;
    MS_U8 u8SC_Coring;
    MS_U8 u8GNR_0;
    MS_U8 u8GNR_1;
    MS_U8 u8SpikeNR_0;
    MS_U8 u8SpikeNR_1;
    MS_U8 u8CP;
    MS_U8 u8DP;
    MS_U8 u8AGC_Gain_Offset;
    MS_U8 u8Gray_Guard_En;
    MS_U8 u8Gray_Guard_Gain;
} MAPI_PQL_DYNAMIC_NR_MISC_PARAM_NOISE;

typedef struct
{
    MS_U8 u8CoringOffset;
    MS_U8 u8SNROffset;
} MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA;

typedef struct
{
    MS_U8 u8Spike_NR_0;
    MS_U8 u8Spike_NR_1;
    MS_U8 u8Gray_Guard_En;
    MS_U8 u8Gray_Guard_Gain;
} MAPI_PQL_DYNAMIC_NR_MISC_PARAM;

typedef struct
{
    /// Default luma curve
    MS_U8 ucLumaCurve[16];
    /// Default luma curve 2a
    MS_U8 ucLumaCurve2_a[16];
    /// Default luma curve 2b
    MS_U8 ucLumaCurve2_b[16];
    /// Default luma curve 2
    MS_U8 ucLumaCurve2[16];

    /// default value: 10
    MS_U8 u8_L_L_U;
    /// default value: 10
    MS_U8 u8_L_L_D;
    /// default value: 10
    MS_U8 u8_L_H_U;
    /// default value: 10
    MS_U8 u8_L_H_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_D;
    /// -31 ~ 31 (bit7 = minus, ex. 0x88 => -8)
    MS_U8 ucCGCCGain_offset;
    /// 0x00~0x6F
    MS_U8 ucCGCChroma_GainLimitH;
    /// 0x00~0x10
    MS_U8 ucCGCChroma_GainLimitL;
    /// 0x01~0x20
    MS_U8 ucCGCYCslope;
    /// 0x01
    MS_U8 ucCGCYth;
    /// Compare difference of max and min bright
    MS_U8 ucDlcPureImageMode;
    /// n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcLevelLimit;
    /// n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDelta;
    /// n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    MS_U8 ucDlcAvgDeltaStill;
    /// min 17 ~ max 32
    MS_U8 ucDlcFastAlphaBlending;
    /// some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
    MS_U8 ucDlcSlowEvent;
    /// for IsrApp.c
    MS_U8 ucDlcTimeOut;
    /// for force to do fast DLC in a while
    MS_U8 ucDlcFlickAlphaStart;
    /// default value: 128
    MS_U8 ucDlcYAvgThresholdH;
    /// default value: 0
    MS_U8 ucDlcYAvgThresholdL;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcBLEPoint;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint;
    /// 1: enable; 0: disable
    MS_U8 bCGCCGainCtrl : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableBLE : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableWLE : 1;
} MAPI_XC_DLC_MFinit;


typedef struct
{
    MS_U32 u32DLC_MFinit_Ex_Version;   ///<Version of current structure. Please always set to "DLC_MFINIT_EX_VERSION" as input
    /// Default luma curve
    MS_U8 ucLumaCurve[16];
    /// Default luma curve 2a
    MS_U8 ucLumaCurve2_a[16];
    /// Default luma curve 2b
    MS_U8 ucLumaCurve2_b[16];
    /// Default luma curve 2
    MS_U8 ucLumaCurve2[16];

    /// default value: 10
    MS_U8 u8_L_L_U;
    /// default value: 10
    MS_U8 u8_L_L_D;
    /// default value: 10
    MS_U8 u8_L_H_U;
    /// default value: 10
    MS_U8 u8_L_H_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_D;
    /// -31 ~ 31 (bit7 = minus, ex. 0x88 => -8)
    MS_U8 ucCGCCGain_offset;
    /// 0x00~0x6F
    MS_U8 ucCGCChroma_GainLimitH;
    /// 0x00~0x10
    MS_U8 ucCGCChroma_GainLimitL;
    /// 0x01~0x20
    MS_U8 ucCGCYCslope;
    /// 0x01
    MS_U8 ucCGCYth;
    /// Compare difference of max and min bright
    MS_U8 ucDlcPureImageMode;
    /// n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcLevelLimit;
    /// n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDelta;
    /// n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    MS_U8 ucDlcAvgDeltaStill;
    /// min 17 ~ max 32
    MS_U8 ucDlcFastAlphaBlending;
    /// some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
    MS_U8 ucDlcSlowEvent;
    /// for IsrApp.c
    MS_U8 ucDlcTimeOut;
    /// for force to do fast DLC in a while
    MS_U8 ucDlcFlickAlphaStart;
    /// default value: 128
    MS_U8 ucDlcYAvgThresholdH;
    /// default value: 0
    MS_U8 ucDlcYAvgThresholdL;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcBLEPoint;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint;
    /// 1: enable; 0: disable
    MS_U8 bCGCCGainCtrl : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableBLE : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableWLE : 1;
    ///
    MS_U8 ucDlcYAvgThresholdM;
    /// Compare difference of max and min bright
    MS_U8 ucDlcCurveMode;
    /// min 00 ~ max 128
    MS_U8 ucDlcCurveModeMixAlpha;
    ///
    MS_U8 ucDlcAlgorithmMode;
    /// Dlc Histogram Limit Curve
    MS_U8 ucDlcHistogramLimitCurve[DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM];
    ///
    MS_U8 ucDlcSepPointH;
    ///
    MS_U8 ucDlcSepPointL;
    ///
    MS_U16 uwDlcBleStartPointTH;
    ///
    MS_U16 uwDlcBleEndPointTH;
    ///
    MS_U8 ucDlcCurveDiff_L_TH;
    ///
    MS_U8 ucDlcCurveDiff_H_TH;
    ///
    MS_U16 uwDlcBLESlopPoint_1;
    ///
    MS_U16 uwDlcBLESlopPoint_2;
    ///
    MS_U16 uwDlcBLESlopPoint_3;
    ///
    MS_U16 uwDlcBLESlopPoint_4;
    ///
    MS_U16 uwDlcBLESlopPoint_5;
    ///
    MS_U16 uwDlcDark_BLE_Slop_Min;
    ///
    MS_U8 ucDlcCurveDiffCoringTH;
    ///
    MS_U8 ucDlcAlphaBlendingMin;
    ///
    MS_U8 ucDlcAlphaBlendingMax;
    ///
    MS_U8 ucDlcFlicker_alpha;
    ///
    MS_U8 ucDlcYAVG_L_TH;
    ///
    MS_U8 ucDlcYAVG_H_TH;
    ///
    MS_U8 ucDlcDiffBase_L;
    ///
    MS_U8 ucDlcDiffBase_M;
    ///
    MS_U8 ucDlcDiffBase_H;
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    /// Left Max Threshold
    MS_U8 u8LMaxThreshold;
    /// Left Min Threshold
    MS_U8 u8LMinThreshold;
    /// Left Max Correction
    MS_U8 u8LMaxCorrection;
    /// Left Min Correction
    MS_U8 u8LMinCorrection;
    /// Right Max Threshold
    MS_U8 u8RMaxThreshold;
    /// Right Min Threshold
    MS_U8 u8RMinThreshold;
    /// Right Max Correction
    MS_U8 u8RMaxCorrection;
    /// Right Min Correction
    MS_U8 u8RMinCorrection;
    /// Allow Lose Contrast
    MS_U8 u8AllowLoseContrast;
#endif
} MAPI_XC_DLC_MFinit_Ex;

typedef struct
{
    /// Scaler DCL MF init
    MAPI_XC_DLC_MFinit DLC_MFinit;
    /// Scaler DCL MF init Ex
    MAPI_XC_DLC_MFinit_Ex DLC_MFinit_Ex;
    /// Curve Horizontal start
    MS_U16 u16CurveHStart;
    /// Curve Horizontal end
    MS_U16 u16CurveHEnd;
    /// Curve Vertical start
    MS_U16 u16CurveVStart;
    /// Curve Vertical end
    MS_U16 u16CurveVEnd;
    /// Scaler DLC MF init
#if (MAPI_ENABLE_10_BIT_DLC)
    /// DLC init ext
    MS_BOOL b10BitsEn;
#endif
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    MS_U8 u8DlcMode;
    MS_U8 u8TmoMode;
#endif

} MAPI_XC_DLC_init;

typedef struct
{
    MS_U8   u8DeFlicker_Step1;
    MS_U8   u8DeFlicker_Step2;
    MS_U8   u8M_DeFi_Th;
    MS_U8   u8L_DeFi_Th;
    MS_U8   u8NoiseThreshold[DYNAMIC_NR_TBL_NOISE_LEVEL_NUM-1];
    MS_U8   u8NoiseMag;
    MS_BOOL bMotionEn;
    MS_BOOL bLumaEn;
    MS_U8   u8MotionStartLvl;
    MS_U8   u8MotionEndLvl;
    MS_U8   u8LumaStartLvl;
    MS_U8   u8LumaEndLvl;
} MAPI_PQL_DYNAMIC_NR_PARAM;

typedef enum
{
    /// Photo
    E_MAPI_PQ_MULTIMEDIA_PHOTO,
    /// Movie
    E_MAPI_PQ_MULTIMEDIA_MOVIE,
}MAPI_PQ_MULTIMEDIA_TYPE;

typedef struct DLL_PUBLIC
{
    /// PQ MultiMedia information
    MAPI_PQ_MULTIMEDIA_TYPE eType;
}MAPI_PQ_MuliMedia_Info;
#endif
