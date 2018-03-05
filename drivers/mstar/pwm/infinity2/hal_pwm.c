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
#include "ms_pwm.h"
#include "../arch/arm/include/asm/arch-infinity2/mach/platform.h"
#include "../../gpio/infinity2/gpio.h"
#include "asm/arch/mach/io.h"

//#define PWM_BASE_ADDR 0x1F003400
//u32 xtalClock = 12000000;

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Init
/// @brief \b Function   \b Description: Initial PWM
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Init(int pwm_id)
{
    MS_BOOL ret = FALSE;
	switch(pwm_id)
	{
		case 0:
		HalPWMPadSet(0, PAD_SNR3_D0);
		break;
		case 1:
		HalPWMPadSet(1, PAD_SNR3_D1);
		break;
		case 2:
		HalPWMPadSet(2, PAD_GPIO8);
		break;
		case 3:
		HalPWMPadSet(3, PAD_GPIO9);
		break;
		case 4:
		HalPWMPadSet(4, PAD_GPIO10);
		break;
		case 5:
		HalPWMPadSet(5, PAD_SNR3_D9);
		break;
		case 6:
		HalPWMPadSet(6, PAD_PM_LED0);
		printf("PWM 6 -> PAD_PM_LED1\n");
		break;
		case 7:
		HalPWMPadSet(7, PAD_PM_LED1);
		printf("PWM 7 -> PAD_PM_LED1\n");
		break;
	}
    ret = TRUE;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_Oen
/// @brief \b Function  \b Description: Enable PWM or not.
/// @param <IN>         \b MS_U16 : index
/// @param <IN>         \b MS_BOOL : letch
/// @param <OUT>      \b None :
/// @param <RET>       \b MS_BOOL :
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Oen(int index, MS_BOOL letch)
{
    MS_BOOL ret = TRUE;

	if(letch)
        CLRREG16((REG_ADDR_PWM + u16REG_SW_RESET), 1<<index);
    else
        SETREG16((REG_ADDR_PWM + u16REG_SW_RESET), 1<<index);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_VDBen
/// @brief \b Function   \b Description : Set the Vsync Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_VDBen(PWM_ChNum index, MS_BOOL bVdbenPWM)
{
	if(bVdbenPWM)
		OUTREGMSK16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_CTRL, (1<<DBEN_BIT), (0x1<<DBEN_BIT));
	else
		OUTREGMSK16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_CTRL, (0<<DBEN_BIT), (0x1<<DBEN_BIT));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Period
/// @brief \b Function   \b Description : Set the period of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U32 : the 18-bit Period value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Period(int index, int u32PeriodPWM)
{
	int u32Period = 0;
//	printf("u32PeriodPWM %d\n", u32PeriodPWM);
	u32Period = XTAL_12000K/u32PeriodPWM;

	//[APN] range 2<=Period<=262144
	if(u32Period < 2)
		u32Period = 2;
	if(u32Period > 262144)
		u32Period = 262144;

	//[APN] PWM _PERIOD= (REG_PERIOD+1)
	u32Period = u32Period - 1;
    printf("reg=0x%08X period=0x%d\n", (U32)(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_PERIOD_L),  u32Period);

    OUTREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_PERIOD_L, (u32Period & 0xFFFF));
    OUTREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_PERIOD_H, ((u32Period>>16) & 0x3));
}

MS_U32 HAL_PWM_GetPeriod(int index)
{
    MS_U16  Period_L=0, Period_H=0;

    return ((MS_U32)Period_H << 16 | Period_L);
}
void HAL_PWM_SetDiv(int index, int div)
{
	OUTREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_DIV, (div & 0xFFFF));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Polarity
/// @brief \b Function   \b Description : Set the Polarity of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Polarity(int index, MS_BOOL bPolPWM)
{
	OUTREGMSK16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_CTRL, (bPolPWM<<POLARITY_BIT), (0x1<<POLARITY_BIT));
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetPolarity
/// @brief \b Function   \b Description : Get the Polarity of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL : 1 for enable; 0 for disable
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetPolarity(PWM_ChNum index)
{
    MS_BOOL bPolarity = FALSE;
    return bPolarity;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_DutyCycle
/// @brief \b Function   \b Description : Set the Duty of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U32 : the 18-bit Duty value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_DutyCycle(int index, int u32DutyPWM)
{
    int u32Period;
    int u32Duty;

	u32Period = INREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_PERIOD_L) + ((INREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_PERIOD_H)&0x3)<< 16);

	if(u32DutyPWM == 0) //allows output low, to avoid 1T pulse
	{
		OUTREGMSK16(REG_ADDR_PWM + u16REG_SW_RESET, BIT0<<index, BIT0<<index);
	}
	else
	{
		u32Duty = ((u32Period * u32DutyPWM) / 100);

	    printf("reg=0x%08X clk=%d, u32Duty=0x%x\n", (U32)(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_DUTY_L), (U32)(XTAL_12000K), u32Duty);
		OUTREGMSK16(REG_ADDR_PWM + u16REG_SW_RESET, ~(BIT0<<index), BIT0<<index);
		OUTREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_DUTY_L, (u32Duty&0xFFFF));
	    OUTREG16(REG_ADDR_PWM + (index*0x80) + u16REG_PWM_DUTY_H, ((u32Duty>>16)&0x3));
	}
}

MS_U32 HAL_PWM_GetDutyCycle(int index)
{
    int  Duty_L=0, Duty_H=0;

    return ((MS_U32)Duty_H << 16 | Duty_L);
}


void HalPWMPadSet(int id, int val)
{
    //reg_pwm0_mode [1:0]
    //reg_pwm1_mode [3:2]
    //reg_pwm2_mode [5:4]
    //reg_pwm3_mode [7:6]
    //reg_pwm4_mode [9:8]
    //reg_pwm5_mode [11:10]
    //reg_pwm6_mode [13:12]
    //reg_pwm7_mode [15:14]
    if( 0 == id )
    {
        if(val==PAD_PWM0){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT0, BIT1|BIT0);
        }else if(val==PAD_MIPI_TX_IO0){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT1, BIT1|BIT0);
        }else if(val==PAD_SNR3_D0){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT1|BIT0, BIT1|BIT0);
        }else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT1|BIT0);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");
        }
    }
    else if( 1 == id )
    {
        if(val==PAD_PWM1){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT2, BIT2|BIT3);
        }else if(val==PAD_MIPI_TX_IO1){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT3, BIT2|BIT3);
        }else if(val==PAD_SNR3_D1){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT2|BIT3, BIT2|BIT3);
        }else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT2|BIT3);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");
        }

    }
    else if( 2 == id )
    {
        if(val==PAD_GPIO8){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT4, BIT5|BIT4);
        }else if(val==PAD_MIPI_TX_IO2){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT5, BIT5|BIT4);
        }else if(val==PAD_SNR3_D2){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT5|BIT4, BIT5|BIT4);
		}else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT5|BIT4);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");

        }
    }
    else if( 3 == id )
    {
        if(val==PAD_GPIO9){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT6, BIT7|BIT6);
        }else if(val==PAD_MIPI_TX_IO3){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT7, BIT7|BIT6);
        }else if(val==PAD_SNR3_D3){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT7|BIT6, BIT7|BIT6);
		}else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT7|BIT6);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");
        }
    }
    else if( 4 == id )
    {
        if(val==PAD_GPIO10){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT8, BIT9|BIT8);
        }else if(val==PAD_MIPI_TX_IO4){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT9, BIT9|BIT8);
		}else if(val==PAD_SNR3_D8){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT9|BIT8, BIT9|BIT8);
        }else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT9|BIT8);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");
        }
    }
    else if( 5 == id )
    {
        if(val==PAD_GPIO11){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT10, BIT11|BIT10);
        }else if(val==PAD_MIPI_TX_IO5){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT11, BIT11|BIT10);
        }else if(val==PAD_SNR3_D9){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT11|BIT10, BIT11|BIT10);
		}else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT11|BIT10);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");

        }
    }
    else if( 6 == id )
    {
        if(val==PAD_GPIO12){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT12, BIT13|BIT12);
        }else if(val==PAD_MIPI_TX_IO6){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT13, BIT13|BIT12);
        }else if(val==PAD_PM_LED0){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT13|BIT12, BIT13|BIT12);
		}else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT13|BIT12);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");

        }
    }
    else if( 7 == id )
    {
        if(val==PAD_GPIO13){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT14, BIT15|BIT14);
        }else if(val==PAD_MIPI_TX_IO7){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT15, BIT15|BIT14);
        }else if(val==PAD_PM_LED1){
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, BIT15|BIT14, BIT15|BIT14);
		}else{
            OUTREGMSK16(REG_ADDR_BASE_PADTOP1 + REG_ID_09, 0x0, BIT15|BIT14);
            if(val!=PAD_UNKNOWN)
                printf("PWM pad set failed\n");

        }
    }
    else
    {
        printf( "void DrvPWMEnable error!!!! (%x, %x)\r\n", id, val);
    }

}

