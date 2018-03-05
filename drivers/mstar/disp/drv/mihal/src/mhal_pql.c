#include <linux/slab.h>
#include <linux/delay.h>

#include "MsTypes.h"
#include "apiXC.h"
#include "drvPQ_Define.h"
#include "drvPQ.h"
#include "drvMVOP.h"
#include "apiXC_Ace.h"
#include "apiPNL.h"

#include "mhal_pql_datatype.h"
#include "mhal_pql_datacfg.h"
#include "mhal_pql.h"


MS_S16 *ps16MhalPqColorCorrectTbl = s16DefaultColorCorrectionMatrix;

MS_BOOL MDrv_PQ_Get_PqBypass(PQ_WIN eWindow)
{
    //Only Main window support PQ bypass
    if(eWindow == PQ_MAIN_WINDOW)
    {
        return FALSE;
    }
    else
    {
        return FALSE;
    }
}


PQ_WIN _SDK2DriverPQWinTypeTrans(MAPI_PQ_WIN enPQWin)
{
    PQ_WIN enWin;

    switch(enPQWin)
    {
        case MAPI_PQ_MAIN_WINDOW:
            enWin = PQ_MAIN_WINDOW;
            break;
        case MAPI_PQ_SUB_WINDOW:
            enWin = PQ_SUB_WINDOW;
            break;
        default:
            ASSERT(0);
            break;
    }

    return enWin;
}

static SCALER_WIN _SDKPQWin2DriverScalerWinTypeTrans(MAPI_PQ_WIN enPQWin)
{
    SCALER_WIN enWin;

    if(enPQWin == MAPI_PQ_MAIN_WINDOW)
        enWin = MAIN_WINDOW;
    else
        enWin = SUB_WINDOW;
    return enWin;
}

#if (DUAL_XC_ENABLE == 1)
void Init_PQ_Ex(MS_U16 u16InitDisplayWidth, MS_U16 u16InitDisplayVTotal)
{

    XC_DEVICE_ID stXC_DeviceId = {SC1_DRIVER_VERSION, SC1_DEVICE_ID};

    PQ_EX_Function_Info function_Info;
    memset(&function_Info, 0, sizeof(function_Info));
    MS_U32 u32InitDataLen = 0;
    function_Info.pq_deside_srctype = NULL;
    function_Info.pq_disable_filmmode = NULL;
    function_Info.pq_load_scalingtable = NULL;
    function_Info.pq_set_420upsampling = NULL;
    function_Info.pq_set_csc = NULL;
    function_Info.pq_set_memformat = NULL;
    function_Info.pq_set_modeInfo = NULL;
    function_Info.pq_get_memyuvfmt = NULL;
#if(PQ_ENABLE_IOCTL)
    function_Info.pq_ioctl = NULL;
#endif
    MApi_XC_EX_PQ_LoadFunction(&stXC_DeviceId, &function_Info , u32InitDataLen);
}
#endif


#if ENABLE_DYNAMIC_NR
void DNR_Init_Noise(const MS_U8* const pNRTbl, const MAPI_PQL_DYNAMIC_NR_MISC_PARAM_NOISE* const pstMisc)
{
    MS_U8 i;

    for(i = 0; i < DYNAMIC_NR_TBL_REG_NUM; i++)
    {
        m_u16PreDynamicNR_Y0[i] = (MS_U16)(pNRTbl[i] & 0x0F) * MApi_XC_ACE_DNR_Get_PrecisionShift();
        m_u16PreDynamicNR_Y1[i] = (MS_U16)((pNRTbl[i] & 0xF0) >> 4) * MApi_XC_ACE_DNR_Get_PrecisionShift();
        m_u8DynamicNR_Y[i]        = pNRTbl[i];
    }
    m_stDynamicNR_MISC_Noise.u8CoringOffset   = pstMisc->u8CoringOffset;
    m_stDynamicNR_MISC_Noise.u8SharpnessOffset = pstMisc->u8SharpnessOffset;
    m_stDynamicNR_MISC_Noise.u8NM_H_0         = pstMisc->u8NM_H_0;
    m_stDynamicNR_MISC_Noise.u8NM_H_1         = pstMisc->u8NM_H_1;
    m_stDynamicNR_MISC_Noise.u8NM_V           = pstMisc->u8NM_V;
    m_stDynamicNR_MISC_Noise.u8SC_Coring      = pstMisc->u8SC_Coring;
    m_stDynamicNR_MISC_Noise.u8GNR_0          = pstMisc->u8GNR_0;
    m_stDynamicNR_MISC_Noise.u8GNR_1          = pstMisc->u8GNR_1;
    m_stDynamicNR_MISC_Noise.u8SpikeNR_0      = pstMisc->u8SpikeNR_0;
    m_stDynamicNR_MISC_Noise.u8SpikeNR_1      = pstMisc->u8SpikeNR_1;
    m_stDynamicNR_MISC_Noise.u8CP             = pstMisc->u8CP;
    m_stDynamicNR_MISC_Noise.u8DP             = pstMisc->u8DP;
    m_stDynamicNR_MISC_Noise.u8AGC_Gain_Offset = pstMisc->u8AGC_Gain_Offset;
    m_stDynamicNR_MISC_Noise.u8Gray_Guard_En  = pstMisc->u8Gray_Guard_En;
    m_stDynamicNR_MISC_Noise.u8Gray_Guard_Gain = pstMisc->u8Gray_Guard_Gain;

    m_u16PreCoringOffset[0] = ((MS_U16)pstMisc->u8CoringOffset & 0x0FL) * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreCoringOffset[1] = (((MS_U16)pstMisc->u8CoringOffset & 0xF0L) >> 4) * MApi_XC_ACE_DNR_Get_PrecisionShift();

    m_u16PreSharpOffset     = (MS_U16)pstMisc->u8SharpnessOffset * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreSC_Coring       = (MS_U16)pstMisc->u8SC_Coring * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreGNR_0           = (MS_U16)pstMisc->u8GNR_0 * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreSpikeNR_1       = (MS_U16)pstMisc->u8SpikeNR_1 * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreGrayGuard_Gain  = (MS_U16)pstMisc->u8Gray_Guard_Gain * MApi_XC_ACE_DNR_Get_PrecisionShift();

    MApi_XC_ACE_DNR_Init_Noise();
}

void DNR_Init_Motion(const MS_U8* const pNRTbl)
{
    MS_U8 i;

    for(i = 0; i < DYNAMIC_NR_TBL_REG_NUM; i++)
    {
        m_u16PreDynamicNR_Y0[i] = (MS_U16)(pNRTbl[i] & 0x0F) * MApi_XC_ACE_DNR_Get_PrecisionShift();
        m_u16PreDynamicNR_Y1[i] = (MS_U16)((pNRTbl[i] & 0xF0) >> 4) * MApi_XC_ACE_DNR_Get_PrecisionShift();
        m_u8DynamicNR_Y[i]        = pNRTbl[i];
    }

    MApi_XC_ACE_DNR_Init_Motion();
    m_u8PreMotion = MApi_XC_ACE_DNR_GetMotion();
}

void DNR_Init_Luma(const MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA* const pstMisc)
{
    m_stDynamicNR_MISC_Luma.u8CoringOffset   = pstMisc->u8CoringOffset;
    m_stDynamicNR_MISC_Luma.u8SNROffset      = pstMisc->u8SNROffset;

    m_u16PreCoringOffset[0] = ((MS_U16)pstMisc->u8CoringOffset & 0x0FL) * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreCoringOffset[1] = (((MS_U16)pstMisc->u8CoringOffset & 0xF0L) >> 4) * MApi_XC_ACE_DNR_Get_PrecisionShift();
    m_u16PreSNROffset       = (MS_U16)pstMisc->u8SNROffset * MApi_XC_ACE_DNR_Get_PrecisionShift();

    MApi_XC_ACE_DNR_Init_Motion();
    MApi_XC_ACE_DNR_Init_Luma();

    m_u8PreMotion = MApi_XC_ACE_DNR_GetMotion();
    #if ENABLE_DLC
    m_u8PreAverageLuma = MApi_XC_DLC_GetAverageValue();
    #endif
}

void DNR_Handler_Luma(MAPI_PQL_DYNAMIC_NR_PARAM * pstDymaicNRParam)
{
#if ENABLE_DLC
    MS_U8 u8LumaTh[DYNAMIC_NR_TBL_LUMA_LEVEL_NUM] = {0x1A, 0x26, 0x32, 0x3E, 0x4A, 0x56, 0x62, 0x6E};// 16 ~ 110
#endif
    MS_U8 u8Weight;
    MS_U8 u8Step;
    MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA stNewMisc;
    MS_U8 u8CurLumaLvl = 0xFF;
    MS_U8 u8AverageLuma = 0;

#if ENABLE_DLC
    u8AverageLuma =  MApi_XC_DLC_GetAverageValue();// get average luma
    u8CurLumaLvl = 0xFF;
    for(i = 0; i < DYNAMIC_NR_TBL_LUMA_LEVEL_NUM; i++)
    {
        if(u8AverageLuma < u8LumaTh[i])
        {
            u8CurLumaLvl = i;
            break;
        }
    }
#endif

    if(u8CurLumaLvl == 0xFF)
        u8CurLumaLvl = DYNAMIC_NR_TBL_LUMA_LEVEL_NUM - 1;

    if(u8CurLumaLvl > pstDymaicNRParam->u8LumaEndLvl)
        u8CurLumaLvl = pstDymaicNRParam->u8LumaEndLvl;
    if(u8CurLumaLvl < pstDymaicNRParam->u8LumaStartLvl)
        u8CurLumaLvl = pstDymaicNRParam->u8LumaStartLvl;

#if DNR_DBG
    printk("Cur: AverageLuma=%02x\n", u8AverageLuma);
    printk("Pre: AverageLuma=%02x\n", m_u8PreAverageLuma);
    printk("lvl start=%bu, end=%bu\n", pstDymaicNRParam->u8LumaStartLvl, pstDymaicNRParam->u8LumaEndLvl);
    printk("Cur: LumaLvl=%02x\n", u8CurLumaLvl);
    printk("Pre: LumaLvl=%02x\n", m_u8PreLumaLvl);
#endif

    // for coverity check...
    if(u8CurLumaLvl >= DYNAMIC_NR_TBL_LUMA_LEVEL_NUM)
    {
        u8CurLumaLvl = DYNAMIC_NR_TBL_LUMA_LEVEL_NUM - 1;
        printf("u8CurLumaLvl value error!!!\n");
    }

    //-------------------------- New coring & sharp ----------------------------
    stNewMisc.u8CoringOffset = tDynamicNRTbl_MISC_Luma[u8CurLumaLvl].u8CoringOffset;
    stNewMisc.u8SNROffset    = tDynamicNRTbl_MISC_Luma[u8CurLumaLvl].u8SNROffset;

    NR_DBG(printf("New coring=%02x\n", stNewMisc.u8CoringOffset));
    NR_DBG(printf("New SNR   =%02x\n", stNewMisc.u8SNROffset));


    u8Weight = MApi_XC_ACE_DNR_GetLuma_Weight(u8AverageLuma,
               m_u8PreAverageLuma,
               u8CurLumaLvl,
               m_u8PreLumaLvl,
               pstDymaicNRParam->u8L_DeFi_Th,
               pstDymaicNRParam->u8DeFlicker_Step1);

    u8Step = pstDymaicNRParam->u8DeFlicker_Step2;
    if(u8Step > MApi_XC_ACE_DNR_Get_PrecisionShift())
        u8Step = MApi_XC_ACE_DNR_Get_PrecisionShift();

    NR_DBG(printf("StableCnt=%bu, weight =%bu\n", g_u8LumaStableCnt, u8Weight));
    //------ Coring -----
    m_u16PreCoringOffset[0] = MApi_XC_ACE_DNR_Blending_MISC((stNewMisc.u8CoringOffset & 0x0F), m_u16PreCoringOffset[0], u8Weight, u8Step);
    m_u16PreCoringOffset[1] = MApi_XC_ACE_DNR_Blending_MISC(((stNewMisc.u8CoringOffset & 0xF0) >> 4), m_u16PreCoringOffset[1], u8Weight, u8Step);
    m_stDynamicNR_MISC_Luma.u8CoringOffset = (MS_U8)(((m_u16PreCoringOffset[1] / MApi_XC_ACE_DNR_Get_PrecisionShift()) << 4) | (m_u16PreCoringOffset[0] / MApi_XC_ACE_DNR_Get_PrecisionShift()));

    //------ SNR -----
    m_u16PreSNROffset = MApi_XC_ACE_DNR_Blending_MISC(stNewMisc.u8SNROffset, m_u16PreSNROffset, u8Weight, u8Step);
    m_stDynamicNR_MISC_Luma.u8SNROffset = (MS_U8)(m_u16PreSNROffset / MApi_XC_ACE_DNR_Get_PrecisionShift());

    NR_DBG(printf("Cur coring=%02x\n", m_stDynamicNR_MISC_Luma.u8CoringOffset));
    NR_DBG(printf("Cur SNR   =%02x\n", m_stDynamicNR_MISC_Luma.u8SNROffset));

    m_u8PreAverageLuma = u8AverageLuma;
    m_u8PreLumaLvl     = u8CurLumaLvl;
}

void DNR_Handler_Motion(MAPI_PQL_DYNAMIC_NR_PARAM * pstDymaicNRParam)
{
    MS_U8 i;
    MS_U8 u8MotionTh[DYNAMIC_NR_TBL_MOTION_LEVEL_NUM-1] = {0x15, 0x2A, 0x3F, 0x54, 0x69};// 0x00 ~ 0x7F
    MS_U8 u8NewNR_Y[8];
    MS_U8 u8Weight;
    MS_U8 u8Step;
    MS_U8 u8CurMotionLvl;
    MS_U8 u8Motion;

    u8Motion = MApi_XC_ACE_DNR_GetMotion();

    u8CurMotionLvl = 0xFF;
    for(i = 0; i < (DYNAMIC_NR_TBL_MOTION_LEVEL_NUM - 1); i++)
    {
        if(u8Motion < u8MotionTh[i])
        {
            u8CurMotionLvl = i;
            break;
        }
    }
    if(u8CurMotionLvl == 0xFF)
        u8CurMotionLvl = DYNAMIC_NR_TBL_MOTION_LEVEL_NUM - 1;


    if(u8CurMotionLvl > pstDymaicNRParam->u8MotionEndLvl)
        u8CurMotionLvl = pstDymaicNRParam->u8MotionEndLvl;
    if(u8CurMotionLvl < pstDymaicNRParam->u8MotionStartLvl)
        u8CurMotionLvl = pstDymaicNRParam->u8MotionStartLvl;

    // for coverity check...
    if(u8CurMotionLvl >= DYNAMIC_NR_TBL_MOTION_LEVEL_NUM)
    {
        u8CurMotionLvl = DYNAMIC_NR_TBL_MOTION_LEVEL_NUM - 1;
        printf("u8CurMotionLvl value error!!!\n");
    }


#if DNR_DBG
    printk("Cur: Motion=%02x\n", u8Motion);
    printk("Pre: Motion=%02x\n", m_u8PreMotion);
    printk("lvl: start=%bu, end=%bu\n", pstDymaicNRParam->u8MotionStartLvl, pstDymaicNRParam->u8MotionEndLvl);
    printk("Cur: MotionLvl=%02x, \n", u8CurMotionLvl);
    printk("Pre: MotionLvl=%02x, \n", m_u8PreMotionLvl);
#endif
    // New NR_Y & NR_C
    NR_DBG(printf("NewNR"));
    for(i = 0; i < DYNAMIC_NR_TBL_REG_NUM; i++)
    {
        u8NewNR_Y[i] =  m_ppu8tDynamicNRTbl_Motion[u8CurMotionLvl][i];
        NR_DBG(printf(" %02x, ", u8NewNR_Y[i]));
    }
    NR_DBG(printf("\n"));


    u8Weight = MApi_XC_ACE_DNR_GetMotion_Weight(u8Motion,
               m_u8PreMotion,
               u8CurMotionLvl,
               m_u8PreMotionLvl,
               pstDymaicNRParam->u8M_DeFi_Th,
               pstDymaicNRParam->u8DeFlicker_Step1);


    u8Step = pstDymaicNRParam->u8DeFlicker_Step2;
    if(u8Step > MApi_XC_ACE_DNR_Get_PrecisionShift())
        u8Step = MApi_XC_ACE_DNR_Get_PrecisionShift();

    MApi_XC_ACE_DNR_Blending_NRTbl(&u8NewNR_Y[0],
                                   u8Weight,
                                   u8Step,
                                   &m_u16PreDynamicNR_Y0[0],
                                   &m_u16PreDynamicNR_Y1[0],
                                   &m_u8DynamicNR_Y[0]);

    m_u8PreMotion      = u8Motion;
    m_u8PreMotionLvl   = u8CurMotionLvl;
}

void DNR_SetSpikeNR(MAPI_PQL_DYNAMIC_NR_MISC_PARAM *pParamTbl)
{
    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_SPIKE_NR_0, pParamTbl->u8Spike_NR_0);
    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_SPIKE_NR_1, pParamTbl->u8Spike_NR_1);

    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_GRAY_GROUND_EN, pParamTbl->u8Gray_Guard_En);
    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_GRAY_GROUND_GAIN, pParamTbl->u8Gray_Guard_Gain);
}


void DNR_SetValue_Luma(const MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA* const pParamTbl)
{
    MS_U16 u16Reg;

    u16Reg = pParamTbl->u8CoringOffset;
    u16Reg |= (u16Reg << 8);

    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_PEAKING_BANK_CORING, u16Reg);

    u16Reg = (((MS_U16)m_u8BK_SC_VPS_60_H + (MS_U16)pParamTbl->u8SNROffset) > 0xFF) ?
             0xFF :
             (m_u8BK_SC_VPS_60_H + pParamTbl->u8SNROffset);

    MApi_XC_ACE_DNR_SetParam(FALSE, E_ACE_DNR_GUASSIN_SNR_THRESHOLD, u16Reg);
}

void DNR_GetGuassinSNR(void)
{
   m_u8BK_SC_VPS_60_H = MApi_XC_ACE_DNR_GetParam(FALSE,  E_ACE_DNR_GUASSIN_SNR_THRESHOLD);
}


void DNR_GetSharpness(void)
{
   m_u8BK_SC_VPS_13_H = MApi_XC_ACE_DNR_GetParam(FALSE,  E_ACE_DNR_SHARPNESS_ADJUST);
}

void DNR_GetCoring(void)
{
   m_u8BK_SC_VPS_13_L = MApi_XC_ACE_DNR_GetParam(FALSE,  E_ACE_DNR_PEAKING_CORING_THRESHOLD);
}

void DNR_Handler(void)
{
#if (ENABLE_NEW_AUTO_NR == 1)

#else
    MAPI_PQL_DYNAMIC_NR_PARAM stDynamicNR_Param;
    if((!MApi_XC_IsMemoryFormat422(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin))) || (!MApi_XC_IsYUVSpace(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin))))
    {
        MApi_XC_Set_NR(DISABLE, _SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));
    }
    else
    {
        MApi_XC_Set_NR(ENABLE, _SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));
    }

    if(m_enCurrNRLevel != E_MAPI_PQL_NR_AUTO)
    {
        return;
    }

    stDynamicNR_Param.u8DeFlicker_Step1 = 0x01;
    stDynamicNR_Param.u8DeFlicker_Step2 = 0x01;

    stDynamicNR_Param.bMotionEn  = 1;
    stDynamicNR_Param.u8MotionEndLvl   = (0x14 & 0x0F);
    stDynamicNR_Param.u8MotionStartLvl = ((0x14 >> 4) & 0x0F);
    stDynamicNR_Param.u8M_DeFi_Th = 0x2A;


    m_stDynamicNR_MISC_Param.u8Spike_NR_0 = tDynamicNRTbl_MISC_Param[1].u8Spike_NR_0;
    m_stDynamicNR_MISC_Param.u8Spike_NR_1 = tDynamicNRTbl_MISC_Param[1].u8Spike_NR_1;

    m_stDynamicNR_MISC_Param.u8Gray_Guard_En   = tDynamicNRTbl_MISC_Param[1].u8Gray_Guard_En;
    m_stDynamicNR_MISC_Param.u8Gray_Guard_Gain = tDynamicNRTbl_MISC_Param[1].u8Gray_Guard_Gain;

    // init parameter for Luma Handler

    stDynamicNR_Param.bLumaEn           = 1;
    stDynamicNR_Param.u8LumaEndLvl      = (0x07 & 0x0F);
    stDynamicNR_Param.u8LumaStartLvl    = ((0x07 >> 4) & 0x0F);
    stDynamicNR_Param.u8L_DeFi_Th       = 0x2A;
    stDynamicNR_Param.u8DeFlicker_Step1 = 0x01;
    stDynamicNR_Param.u8DeFlicker_Step2 = 0x01;

    if(stDynamicNR_Param.bMotionEn)
    {
        DNR_Handler_Motion(&stDynamicNR_Param);
    }

    if(stDynamicNR_Param.bLumaEn)
    {
        DNR_Handler_Luma(&stDynamicNR_Param);
    }

    DNR_SetSpikeNR(&m_stDynamicNR_MISC_Param);

    DNR_SetValue_Luma(&m_stDynamicNR_MISC_Luma);

    MApi_XC_ACE_DNR_SetNRTbl(m_u8DynamicNR_Y);
#endif
}

#endif

void Mhal_PQL_Handler(MS_BOOL bRealTimeMonitorOnly)
{
    //FIXME: how to handle no signal
    //static MS_BOOL m_bHistogramReady = 0;
    if(MApi_XC_IsBlackVideoEnable((_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin))))
    {
        return;
    }

    if(bRealTimeMonitorOnly == FALSE)
    {
        MDrv_PQ_AdaptiveTuning();
    }

#if ENABLE_DYNAMIC_NR
    DNR_Handler();
#endif
    MApi_XC_Mux_SourceMonitor(FALSE);
}

void DNR_Init(void)
{
    if(IsSrcAnalog(m_enCurrInputSrcType))
    {
        DNR_Init_Noise(m_ppu8tDynamicNRTbl_Noise[0], &tDynamicNR_MISC_Noise[0]);
    }
    else
    {
        DNR_Init_Motion(m_ppu8tDynamicNRTbl_Motion[5]);
        DNR_Init_Luma(&tDynamicNRTbl_MISC_Luma[7]);
    }

#if (ENABLE_NEW_AUTO_NR == 1)
    if(m_AutoNrInitParam.u8DebugLevel & DNR_DBG_GRULE)
    {
        MDrv_PQ_SetDbgLevel(PQ_DBG_GRULE);
    }
    else
    {
        MDrv_PQ_SetDbgLevel(0x00);
    }
#endif
}

#if ENABLE_DLC
bool Init_DLC_From_Ext_File(void)
{
    unsigned char u8DLCTableCount = GetDLCTableCount();
     if(u8DLCTableCount == 0)
    {
        XC_DLC_init sXC_DLC_InitData;
        memset(&sXC_DLC_InitData, 0, sizeof(XC_DLC_init));
        sXC_DLC_InitData.u16CurveHStart                    = g_IPanel.Width() / 8;
        sXC_DLC_InitData.u16CurveHEnd                      = (g_IPanel.Width() * 7) / 8;
        sXC_DLC_InitData.u16CurveVStart                    = g_IPanel.Height() / 8;
        sXC_DLC_InitData.u16CurveVEnd                      = (g_IPanel.Height() * 7) / 8;
        if(MApi_XC_DLC_Init_Ex(&sXC_DLC_InitData, sizeof(XC_DLC_init)) == FALSE)
        {
            printf("L:%d, DLC_Init failed because of InitData wrong, please update header file and compile again\n", __LINE__);
            ASSERT(0);
        }
    }
    else
    {
        for (MS_U8 i = 0; i < u8DLCTableCount; i++)
        {
            SetDLCCurveInfo(i, (g_IPanel.Width() / 8), ((g_IPanel.Width() * 7) / 8), (g_IPanel.Height() / 8), ((g_IPanel.Height() * 7) / 8));
        }

        MAPI_XC_DLC_init *psXC_DLC_InitData = GetDLCInfo(DLC_DEFAULT_TABLE_INDEX);
        if(MApi_XC_DLC_Init_Ex((XC_DLC_init*)psXC_DLC_InitData, sizeof(XC_DLC_init)) == FALSE)
        {
            printf("L:%d, DLC_Init failed because of InitData wrong, please update header file and compile again\n", __LINE__);
        }
   }
    return true;
}

void Init_DLC(void)
{
    if(Init_DLC_From_Ext_File() == true)
    {
        return;
    }
    else
    {
        printf("Init DLC from DLC.ini has problem\n");
        ASSERT(0);
    }
}
#endif

static const MS_S16 s16DefaultRGB[3][3] =
{
    { 1024, 0,    0 }, // R
    { 0,  1024,   0 }, // G
    { 0,   0,  1024 }  // B
};
extern XC_PNL_OBJ g_IPanel;
extern E_APIXC_ReturnValue MApi_XC_SetForceWrite( EN_MUTE_TYPE enMuteType, MS_BOOL bIsForceWrite) __attribute__((weak));

#if ENABLE_ACE
void Init_XC_ACE(void)
{
    XC_ACE_InitData sXC_ACE_InitData;

    memset(&sXC_ACE_InitData, 0, sizeof(XC_ACE_InitData));

    // Init ACE
    sXC_ACE_InitData.eWindow = MAIN_WINDOW; // FALSE: MAIN_WINDOW

    sXC_ACE_InitData.S16ColorCorrectionMatrix = (MS_S16*)(s16DefaultColorCorrectionMatrix);

    sXC_ACE_InitData.S16RGB = (MS_S16*) s16DefaultRGB;
    sXC_ACE_InitData.u16MWEHstart = g_IPanel.HStart();
    sXC_ACE_InitData.u16MWEVstart = g_IPanel.VStart();
    sXC_ACE_InitData.u16MWEWidth  = g_IPanel.Width();
    sXC_ACE_InitData.u16MWEHeight = g_IPanel.Height();

    sXC_ACE_InitData.bMWE_Enable = FALSE;

    /*if(mapi_syscfg_fetch::Get_bootlogo_display() == TRUE)
    {
        MApi_XC_ACE_SetBypassColorMatrix(TRUE);
    }*/

#ifdef UFO_XC_FORCEWRITE
    if(MApi_XC_SetForceWrite)
    {
        MApi_XC_SetForceWrite(E_MAIN_WINDOW_MUTE,TRUE);
        //MApi_XC_SetForceWrite(E_SUB_WINDOW_MUTE,TRUE);
    }
#endif

    MApi_XC_ACE_SetBypassColorMatrix(TRUE);

    if(MApi_XC_ACE_Init(&sXC_ACE_InitData, sizeof(XC_ACE_InitData)) == FALSE)
    {
        printk("L:%d, ACE_Init failed because of InitData wrong, please update header file and compile again\n", __LINE__);
    }
#ifdef UFO_XC_FORCEWRITE
    if(MApi_XC_SetForceWrite)
    {
        MApi_XC_SetForceWrite(E_MAIN_WINDOW_MUTE,FALSE);
        //MApi_XC_SetForceWrite(E_SUB_WINDOW_MUTE,FALSE);
    }
#endif

    /*if(mapi_syscfg_fetch::Get_bootlogo_display() == TRUE)
    {
        MApi_XC_ACE_SetBypassColorMatrix(FALSE);
    }*/

    MApi_XC_ACE_SetBypassColorMatrix(FALSE);
    MApi_XC_ACE_SelectYUVtoRGBMatrix(TRUE, E_XC_ACE_YUV_TO_RGB_MATRIX_USER, (MS_S16*) s16DefaultColorCorrectionMatrix);
    MApi_XC_ACE_SelectYUVtoRGBMatrix(FALSE, E_XC_ACE_YUV_TO_RGB_MATRIX_USER, (MS_S16*) s16DefaultColorCorrectionMatrix);
    MApi_XC_ACE_SetColorCorrectionTable(MAIN_WINDOW);
    //MApi_XC_ACE_SetColorCorrectionTable(SUB_WINDOW);
    MApi_XC_ACE_SetColorMatrixControl(MAIN_WINDOW, TRUE); // FALSE: MAIN_WINDOW
    //MApi_XC_ACE_SetColorMatrixControl(SUB_WINDOW, TRUE); // FALSE: MAIN_WINDOW
    MApi_XC_ACE_SetRBChannelRange(MAIN_WINDOW, TRUE); // FALSE: MAIN_WINDOW
    //MApi_XC_ACE_SetRBChannelRange(SUB_WINDOW, TRUE); // FALSE: MAIN_WINDOW
}
#endif


void Initialize(void)
{
/**read config/pq/Bandwidth_RegTable.bin**/
 //   MDrv_BW_LoadInitTable();
#if ENABLE_ACE
    Init_XC_ACE();
#endif

#if ENABLE_DLC
    Init_DLC();
#endif

    //_MHAL_DISP_PqInit(u16InitDisplayWidth,u16InitDisplayVTotal);

#if (DUAL_XC_ENABLE == 1)
    Init_PQ_Ex(0, 0);
#endif

#if ENABLE_DYNAMIC_NR
    DNR_Init();
#endif
}

mhal_pql_ColorTemperature_t _stPQLColorTemperature;
MS_U8 *pGammTable[3] = {NULL};

void SetContrast(MS_U8 u8Contrast)
{
#if ENABLE_ACE
    MS_BOOL bWin;
    MS_BOOL bYUVSpace;

    bWin = (m_enPQWin == MAPI_PQ_MAIN_WINDOW) ? FALSE : TRUE;
    bYUVSpace = MDrv_PQ_IsForceVideoInputMode(_SDK2DriverPQWinTypeTrans(m_enPQWin));
    bYUVSpace |= MApi_XC_IsYUVSpace(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));
    MApi_XC_ACE_PicSetContrast(bWin, bYUVSpace, u8Contrast);
#endif
}

void SetHue(MS_U8 u8Hue)
{
#if ENABLE_ACE
    MS_BOOL bWin;
    MS_BOOL bYUVSpace;

    bWin = (m_enPQWin == MAPI_PQ_MAIN_WINDOW) ? FALSE : TRUE;
    bYUVSpace = MDrv_PQ_IsForceVideoInputMode(_SDK2DriverPQWinTypeTrans(m_enPQWin));
    bYUVSpace |= MApi_XC_IsYUVSpace(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));

    MApi_XC_ACE_PicSetHue(bWin, bYUVSpace, u8Hue);
#endif
}

void SetSaturation(MS_U8 u8Saturation)
{
#if ENABLE_ACE
    MS_BOOL bWin;
    MS_BOOL bYUVSpace;

    bWin = (m_enPQWin == MAPI_PQ_MAIN_WINDOW) ? FALSE : TRUE;
    bYUVSpace = MDrv_PQ_IsForceVideoInputMode(_SDK2DriverPQWinTypeTrans(m_enPQWin));
    bYUVSpace |= MApi_XC_IsYUVSpace(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));
    MApi_XC_ACE_PicSetSaturation(bWin, bYUVSpace, u8Saturation);
#endif
}

void SetSharpness(MS_U8 u8Sharpness)
{
#if ENABLE_ACE
    MS_BOOL bWin;
    bWin = (m_enPQWin == MAPI_PQ_MAIN_WINDOW) ? FALSE : TRUE;
    MApi_XC_ACE_PicSetSharpness(bWin, u8Sharpness);
#endif
}

void SetBrightness(MS_U8 u8Birhgtness_R, MS_U8 u8Birhgtness_G, MS_U8 u8Birhgtness_B)
{
#if ENABLE_ACE
    MS_BOOL bWin;
    bWin = (m_enPQWin == MAPI_PQ_MAIN_WINDOW) ? FALSE : TRUE;
    MApi_XC_ACE_PicSetBrightness(bWin, u8Birhgtness_R, u8Birhgtness_G, u8Birhgtness_B);
#endif
}

void SetColorTemperatureEx(const MAPI_PQL_COLOR_TEMPEX_DATA* const pstColorTmp)
{
    XC_ACE_color_temp_ex2 colorTemp;

    colorTemp.cRedColor    = pstColorTmp->u16RedGain;
    colorTemp.cGreenColor  = pstColorTmp->u16GreenGain;
    colorTemp.cBlueColor   = pstColorTmp->u16BlueGain;
    colorTemp.cRedOffset   = pstColorTmp->u16RedOffset;
    colorTemp.cGreenOffset = pstColorTmp->u16GreenOffset;
    colorTemp.cBlueOffset  = pstColorTmp->u16BlueOffset;
    if (m_enPQWin == MAPI_PQ_MAIN_WINDOW)
    {
        MApi_XC_ACE_PicSetPostColorTemp_V02(FALSE , (XC_ACE_color_temp_ex2 *) &colorTemp);
    }
    else
    {
        MApi_XC_ACE_PicSetPostColorTemp_V02(TRUE , (XC_ACE_color_temp_ex2 *) &colorTemp);
    }
}

void EnableScalerGamma(MS_BOOL bEnable)
{
    MDrv_PQ_EnableScalerGamma(PQ_MAIN_WINDOW, bEnable);
}

void SetScalerGamma(APIPNL_GAMMA_TYPE enGammType, MS_U8 **pu8GammTable, APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode)
{
    g_IPanel.SetGammaTbl(enGammType, pu8GammTable, enGammaMappingMode);
}

void SetModeStart(void)
{
    MApi_XC_ACE_SkipWaitVsync(_SDK2DriverPQWinTypeTrans(m_enPQWin), ENABLE);
}

void SetModeEnd(void)
{
    MApi_XC_ACE_SkipWaitVsync(_SDK2DriverPQWinTypeTrans(m_enPQWin), DISABLE);
}

MS_U16 Mhal_PQL_GetPQDelayTime(void)
{
    return MDrv_PQ_GetDelayTime(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin));
}

MS_BOOL Mhal_PQL_WaitVideoStable(MAPI_SCALER_WIN enScalerWinType, MS_U32 u32DelayTime)
{
    MS_BOOL bFDMaskEnable = FALSE;
    XC_ApiStatus stXCApiStatus;
    SCALER_WIN eWindow = MAIN_WINDOW;

    memset(&stXCApiStatus, 0, sizeof(XC_ApiStatus));
    if(enScalerWinType == MAPI_SUB_WINDOW)
        eWindow = SUB_WINDOW;
    else
        eWindow = MAIN_WINDOW;

    if (MApi_XC_GetStatus(&stXCApiStatus, eWindow) == FALSE)
    {
        XCPERFMSG(printf("MApi_XC_GetStatus failed.\n"));
    }

    bFDMaskEnable = MApi_XC_Get_FD_Mask();

    if((!MApi_XC_IsCurrentFrameBufferLessMode())
        && (ENABLE == bFDMaskEnable))
    {
        MS_BOOL bFDMaskClr = TRUE;
        MVOP_Handle stMvopHd = { E_MVOP_MODULE_MAX };

        if  (enScalerWinType == MAPI_MAIN_WINDOW)
        {
            if (MApi_XC_Get_FD_Mask_byWin(MAIN_WINDOW))
            {
                MApi_XC_set_FD_Mask_byWin(DISABLE, MAIN_WINDOW);
            }
        }

        //STB projects support mvop FD mask now, but TV projects doesnt
        //But if TV projects supported mvop FD mask someday, we have to call the below command
        //The below mvop command just support STB projects now, which be controlled by mvop hal. 20150505.

        stMvopHd.eModuleNum = E_MVOP_MODULE_MAIN;

        MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_STB_FD_MASK_CLR, &bFDMaskClr);
        MDrv_MVOP_SEL_OP_FIELD(TRUE);
    }

    //wait pq delay
    msleep(u32DelayTime);

    if((!MApi_XC_IsCurrentFrameBufferLessMode())
        && (ENABLE == bFDMaskEnable))
    {
        MS_BOOL bFDMaskClr = FALSE;
        MVOP_Handle stMvopHd = { E_MVOP_MODULE_MAX };

        if  (enScalerWinType == MAPI_MAIN_WINDOW)
        {
            if (MApi_XC_Get_FD_Mask_byWin(MAIN_WINDOW))
            {
                MApi_XC_set_FD_Mask_byWin(ENABLE, MAIN_WINDOW);
            }
        }

        //STB projects support mvop FD mask now, but TV projects doesnt
        //But if TV projects supported mvop FD mask someday, we have to call the below command
        //The below mvop command just support STB projects now, which be controlled by mvop hal. 20150505.

        stMvopHd.eModuleNum = E_MVOP_MODULE_MAIN;

        MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_STB_FD_MASK_CLR, &bFDMaskClr);
        MDrv_MVOP_SEL_OP_FIELD(FALSE);
    }

    return TRUE;
}

void Mhal_PQL_UpdatePicture(mhal_pql_ColorTemperature_t *pstPQLColorTemperature)
{
    MAPI_PQL_COLOR_TEMPEX_DATA pstColorTemperatureEx;
    APIPNL_GAMMA_TYPE enGammType = E_APIPNL_GAMMA_12BIT;
    APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode = E_APIPNL_GAMMA_8BIT_MAPPING;

    if(pstPQLColorTemperature == NULL)
        pstPQLColorTemperature = &_stPQLColorTemperature;

    pstColorTemperatureEx.u16RedGain = (MS_U16)pstPQLColorTemperature->u32R_Gain;
    pstColorTemperatureEx.u16GreenGain = (MS_U16)pstPQLColorTemperature->u32G_Gain;
    pstColorTemperatureEx.u16BlueGain = (MS_U16)pstPQLColorTemperature->u32B_Gain;
    pstColorTemperatureEx.u16RedOffset = (MS_U16)pstPQLColorTemperature->u32R_Offset;
    pstColorTemperatureEx.u16GreenOffset = (MS_U16)pstPQLColorTemperature->u32G_Offset;
    pstColorTemperatureEx.u16BlueOffset = (MS_U16)pstPQLColorTemperature->u32B_Offset;

    SetModeStart();
    SetColorTemperatureEx(&pstColorTemperatureEx);
    SetBrightness((pstPQLColorTemperature->u32Brightness)&0xFF,
              (pstPQLColorTemperature->u32Brightness)&0xFF,
              (pstPQLColorTemperature->u32Brightness)&0xFF);
    SetContrast((MS_U8)(pstPQLColorTemperature->u32Contrast));
    SetHue((MS_U8)pstPQLColorTemperature->u32Hue);
    SetSaturation((MS_U8)pstPQLColorTemperature->u32Saturation);
    SetSharpness((MS_U8)pstPQLColorTemperature->u32Sharpness);
    SetModeEnd();

    EnableScalerGamma(TRUE);
    SetScalerGamma(enGammType , pGammTable, enGammaMappingMode);

}

void SetPQLparam(void)
{
    _stPQLColorTemperature.u32R_Gain = 1024;
    _stPQLColorTemperature.u32G_Gain = 1024;
    _stPQLColorTemperature.u32B_Gain = 1024;
    _stPQLColorTemperature.u32R_Offset = 1024;
    _stPQLColorTemperature.u32G_Offset = 1024;
    _stPQLColorTemperature.u32B_Offset = 1024;


    _stPQLColorTemperature.u32Brightness = 128;
    _stPQLColorTemperature.u32Contrast = 128;//same with britness
    _stPQLColorTemperature.u32Hue  = 50;//no effect
    _stPQLColorTemperature.u32Saturation = 100;//no effect
    _stPQLColorTemperature.u32Sharpness =40;//no effect

    pGammTable[0] = (MS_U8*) (au8NormalGammaR);
    pGammTable[1] = (MS_U8*) (au8NormalGammaG);
    pGammTable[2] = (MS_U8*) (au8NormalGammaB);
}

void mapi_pql_customer_Init(void )
{
    MS_U16 i=0;

    STATIC_ASSERT((int)ENABLE_10_BIT_DLC == (int)MAPI_ENABLE_10_BIT_DLC);

    m_enCurrNRLevel = E_MAPI_PQL_NR_OFF;
    m_enCurrInputSrcType = MAPI_INPUT_SOURCE_NONE;
    m_bEnableDLC = FALSE;

#if ENABLE_DYNAMIC_NR
    memset(&m_u16PreDynamicNR_Y0, 0, sizeof(MS_U16)*DYNAMIC_NR_TBL_REG_NUM);
    memset(&m_u16PreDynamicNR_Y1, 0, sizeof(MS_U16)*DYNAMIC_NR_TBL_REG_NUM);
    memset(&m_u16PreCoringOffset, 0, sizeof(MS_U16) * 2);

    m_u8PreMotionLvl = 0;
    m_u8PreLumaLvl = 0;
    m_u8PreMotion = 0;
    m_u8PreAverageLuma = 0;
    m_u16PreSNROffset = 0;

    m_u16PreSharpOffset = 0;
    m_u16PreSC_Coring = 0;
    m_u16PreGNR_0 = 0;
    m_u16PreSpikeNR_1 = 0;
    m_u16PreGrayGuard_Gain = 0;
    m_u8PreNoiseMag = 0;

    memset(m_u8DynamicNR_Y, 0x00, sizeof(MS_U8)*DYNAMIC_NR_TBL_REG_NUM);
    memset(&m_stDynamicNR_MISC_Noise, 0x00, sizeof(MAPI_PQL_DYNAMIC_NR_MISC_PARAM_NOISE));
    memset(&m_stDynamicNR_MISC_Luma, 0x00, sizeof(MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA));
    memset(&m_stDynamicNR_MISC_Param, 0x00, sizeof(MAPI_PQL_DYNAMIC_NR_MISC_PARAM));

    m_u8NoiseMagReadCnt = 0;
    m_u16NoiseMag = 0;
    m_u8NoiseMag = 0;
    m_u8BK_SC_VPS_60_H = 0;
    m_u8BK_SC_VPS_13_L = 0;
    m_u8BK_SC_VPS_13_H = 0;
    m_u8BK_AFEC_44 = 0;
#endif

    m_ePIPMode  = E_MAPI_PQL_PIP_MODE_OFF;
    m_ePrePIPMode = E_MAPI_PQL_PIP_MODE_OFF;

    tDynamicNRTbl_MISC_Luma = (MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA *)kmalloc((sizeof(MAPI_PQL_DYNAMIC_NR_MISC_PARAM_LUMA)), GFP_KERNEL);
    tDynamicNRTbl_MISC_Param = (MAPI_PQL_DYNAMIC_NR_MISC_PARAM *)kmalloc((sizeof(MAPI_PQL_DYNAMIC_NR_MISC_PARAM)), GFP_KERNEL);
    if(tDynamicNRTbl_MISC_Param == NULL || tDynamicNRTbl_MISC_Luma == NULL)
        printk("kmalloc fail line %d \r\n", __LINE__);

    for(i=0; i<m_u8tDynamicNRTbl_MISC_Luma_rows; i++)
    {
        tDynamicNRTbl_MISC_Luma[i].u8CoringOffset =       m_ppu8tDynamicNRTbl_MISC_Luma[i][0];
        tDynamicNRTbl_MISC_Luma[i].u8SNROffset =          m_ppu8tDynamicNRTbl_MISC_Luma[i][1];
    }

    for(i =0;i< m_u8tDynamicNRTbl_MISC_Param_rows;i++)
    {
        tDynamicNRTbl_MISC_Param[i].u8Spike_NR_0 =       m_ppu8tDynamicNRTbl_MISC_Param[i][0];
        tDynamicNRTbl_MISC_Param[i].u8Spike_NR_1 =       m_ppu8tDynamicNRTbl_MISC_Param[i][1];
        tDynamicNRTbl_MISC_Param[i].u8Gray_Guard_En =    m_ppu8tDynamicNRTbl_MISC_Param[i][2];
        tDynamicNRTbl_MISC_Param[i].u8Gray_Guard_Gain =  m_ppu8tDynamicNRTbl_MISC_Param[i][3];
    }

#if (ENABLE_NEW_AUTO_NR == 1)

#endif
    SetPQLparam();
    Initialize();
}


void mapi_pql_customer_DeInit(void)
{

#if (ENABLE_NEW_AUTO_NR == 1)
    //pthread_mutex_destroy(&m_mutex_PqlProcess);
#endif
}

MS_BOOL loadBWTableByContext(void)
{
#if ENABLE_BW
    PQ_WIN enPQWin = _SDK2DriverPQWinTypeTrans(m_enPQWin);
    //MS_BOOL bCusMode = GetBwCusMode();
     MS_BOOL bCusMode = FALSE;

    if (bCusMode != TRUE)
    {
        MDrv_BW_LoadTableByContext(enPQWin);
    }
#endif

    return TRUE;
}

MS_BOOL getPqBypass(PQ_WIN eWindow)
{
    return MDrv_PQ_Get_PqBypass(eWindow);
}

MS_BOOL SetMfcLevel(MAPI_PQL_MFC_LEVEL enMfcLevel)
{
    //printf("Not Support\n");
    return TRUE;
}


void Mhal_PQL_SetQuality(void)
{
    PQ_WIN enPQWin;
    MS_BOOL ret = TRUE;
    static MS_BOOL bPqBypass  = FALSE;
    XCPERFMSG(printf("Start of XC Set PQ [%lu]\n", MsOS_GetSystemTime()));
    enPQWin = _SDK2DriverPQWinTypeTrans(m_enPQWin);

    loadBWTableByContext();

    if(MDrv_PQ_Get_PqBypass(enPQWin) == FALSE  && bPqBypass == TRUE)//MDrv_PQ_Get_PqBypass i2
    {
         MDrv_PQ_SkipDuplicatedSetting(enPQWin, FALSE);
    }

    MDrv_PQ_LoadSettings(enPQWin);

    if(MDrv_PQ_Get_PqBypass(enPQWin) == FALSE  && bPqBypass == TRUE)
    {
         bPqBypass = FALSE;
         MDrv_PQ_SkipDuplicatedSetting(enPQWin, TRUE);
    }

    //TODO:wait to utopia release about UFSC
    //MDrv_PQ_LoadUFSCSettings(enPQWin);

    // CustomerSetting need to be loaded after Main&UFSC settings
    MDrv_PQ_LoadCustomerSettings(enPQWin);

    //if(mapi_video::GetDisplayOPColorFormat() == E_OP_COLOR_FORMAT_RGB)
    {
        if(MDrv_PQ_Get_PqBypass(enPQWin))
        {
            MDrv_PQ_LoadPTPTable(enPQWin, PQ_PTP_PTP);
            bPqBypass = TRUE;
        }
#if ENABLE_DLC
        MApi_XC_DLC_CGC_Init();
#endif

        //MAPI_COLOR_MATRIX *pColorMatrix = mapi_syscfg_fetch::GetInstance()->GetColorMatrix();
        //MAPI_COLOR_MATRIX *pColorMatrix = GetColorMatrix();

        // set color correction table
        if(MDrv_PQ_Get_PqBypass(enPQWin))
        {
            MApi_XC_ACE_ColorCorrectionTable( MAIN_WINDOW, (MS_S16*) S16BypassColorCorrectionMatrix);
        }
        else
        {
            MApi_XC_ACE_ColorCorrectionTable(m_enPQWin, ps16MhalPqColorCorrectTbl);
        }

#if ENABLE_DLC
        if(MApi_XC_DLC_GetLumaCurveStatus())
        {
            MApi_XC_DLC_SetOnOff(ENABLE, FALSE); // FALSE: MAIN_WINDOW
            m_bEnableDLC = TRUE; //FIXME
        }
        else
        {
            m_bEnableDLC = FALSE; //FIXME
        }
#endif

        if(MApi_XC_IsYUVSpace(_SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin)))
        {
            MApi_XC_ACE_SetColorCorrectionTable(MAIN_WINDOW);
        }
        else
        {
            MApi_XC_ACE_SetPCYUV2RGB( _SDKPQWin2DriverScalerWinTypeTrans(m_enPQWin), FALSE);
            //set 3x3 to reg
            MApi_XC_ACE_SetPCsRGBTable((MS_BOOL)m_enPQWin);
        }

    }

    // if Media type is photo ,need close MEMC
    if(TRUE == getPqBypass(enPQWin))
    {
        ret = SetMfcLevel(E_MFC_LEVEL_BYPASS);
    }

    if(FALSE == ret)
    {
        printf("%s @ line %d SetMfcLevel fail !\n", __FUNCTION__,__LINE__);
    }

#if ENABLE_DYNAMIC_NR
    DNR_GetGuassinSNR();
    DNR_GetSharpness();
    DNR_GetCoring();
#endif
    XCPERFMSG(printf("End of XC Set PQ [%lu]\n", MsOS_GetSystemTime()));
}

void Mhal_PQL_SetColorCorrectTbl(MS_S16 *pColorCorrectTbl)
{
    ps16MhalPqColorCorrectTbl = pColorCorrectTbl;
}


EXPORT_SYMBOL(Mhal_PQL_SetQuality);
EXPORT_SYMBOL(mapi_pql_customer_Init);
EXPORT_SYMBOL(Mhal_PQL_Handler);
EXPORT_SYMBOL(Mhal_PQL_SetColorCorrectTbl);
