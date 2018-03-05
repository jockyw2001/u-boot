////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

/**
* @file    hal_bus_i2c.c
* @version
* @brief   This file contains the bus I2C hard hal access functions.
*
*/

#define __HAL_BUS_I2C_C__

/*=============================================================*/
// Include files
/*=============================================================*/
#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <malloc.h>
#include "MsTypes.h"

MS_BOOL HAL_PWM_Oen(PWM_ChNum index, MS_BOOL letch);
void HAL_PWM_VDBen(PWM_ChNum index, MS_BOOL bVdbenPWM);
void HAL_PWM_Period(PWM_ChNum index, MS_U32 u32PeriodPWM);
MS_U32 HAL_PWM_GetPeriod(PWM_ChNum index);
void HAL_PWM_Polarity(PWM_ChNum index, MS_BOOL bPolPWM);
MS_BOOL HAL_PWM_GetPolarity(PWM_ChNum index);
void HAL_PWM_DutyCycle(PWM_ChNum index, MS_U32 u32DutyPWM);
MS_U32 HAL_PWM_GetDutyCycle(PWM_ChNum index);