#ifndef _MS_PWM_H_
#define _MS_PWM_H_

#include "MsTypes.h"
//------------------------------------------------------------------------------
//  Constants
//------------------------------------------------------------------------------
//Common PWM registers
#define u16REG_PWM_SHIFT_L      (0x0 << 2)
#define u16REG_PWM_SHIFT_H      (0x1 << 2)
#define u16REG_PWM_DUTY_L       (0x2 << 2)
#define u16REG_PWM_DUTY_H       (0x3 << 2)
#define u16REG_PWM_PERIOD_L     (0x4 << 2) //reg_pwm0_period
#define u16REG_PWM_PERIOD_H     (0x5 << 2)
#define u16REG_PWM_DIV          (0x6 << 2)
#define u16REG_PWM_CTRL         (0x7 << 2)
    #define VDBEN_SW_BIT           0
    #define DBEN_BIT               1
    #define DIFF_P_EN_BIT          2
    #define SHIFT_GAT_BIT          3
    #define POLARITY_BIT           4

#define u16REG_PWM_SHIFT2       (0x8 << 2)
#define u16REG_PWM_DUTY2        (0x9 << 2)
#define u16REG_PWM_SHIFT3       (0xA << 2)
#define u16REG_PWM_DUTY3        (0xB << 2)
#define u16REG_PWM_SHIFT4       (0xC << 2)
#define u16REG_PWM_DUTY4        (0xD << 2)

#define u16REG_SW_RESET         (0x7F << 2)

typedef enum _PWM_ChNum
{
    E_PWM_CH0,
    E_PWM_CH1,
    E_PWM_CH2,
    E_PWM_CH3,
    E_PWM_CH4,
    E_PWM_CH5,
    E_PWM_CH6,
    E_PWM_CH7,
}PWM_ChNum;

typedef enum _PWM_Result
{
    E_PWM_OK,
    E_PWM_FAIL
} PWM_Result;

typedef enum _PWM_Property
{
    E_PWM_GetPeriod,
    E_PWM_GetDutyCycle,
    E_PWM_GetShift,
    E_PWM_GetDiv,
    E_PWM_GetPolarity,
    E_PWM_GetVdben,
    E_PWM_GetRsten,
    E_PWM_GetDben,
    E_PWM_GetOen
}PWM_Property;

typedef struct _PWM_DrvStatus
{
    MS_BOOL PWM0;
    MS_BOOL PWM1;
    MS_BOOL PWM2;
    MS_BOOL PWM3;
    MS_BOOL PWM4;
    MS_BOOL PWM5;
    MS_BOOL PWM6;
    MS_BOOL PWM7;
} PWM_DrvStatus;
/// Description : Show the PWM_PAD is PWM(True) or GPIO(false)
/// @ingroup G_PWM_COMMON
/// @param  pStatus \b OUT: output PWM driver status
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetStatus(PWM_DrvStatus *pStatus);

//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @ingroup G_PWM_INIT
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_PWM_Init(int index);
//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_PWM_Oen(int index, MS_BOOL letch);
void HAL_PWM_SetDiv(int index, int div);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
void HAL_PWM_Period(int index, int u32PeriodPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @return u16PeriodPWM
//-------------------------------------------------------------------------------------------------
MS_U32 HAL_PWM_GetPeriod(int index);
//-------------------------------------------------------------------------------------------------
void HalPWMPadSet(int id, int val);
void HAL_PWM_Polarity(int index, MS_BOOL bPolPWM);
void HAL_PWM_DutyCycle(int index, int u32DutyPWM);
#endif