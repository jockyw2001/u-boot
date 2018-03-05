#include <common.h>
#include <div64.h>
#include <pwm.h>
#include <asm/io.h>
#include "ms_pwm.h"

int pwm_enable(int pwm_id)
{
	bool bEnable = TRUE;

	if( !(HAL_PWM_Oen(pwm_id, bEnable)) )
    {
        return E_PWM_FAIL;
    }
    return E_PWM_OK;
}
//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
int pwm_init(int pwm_id, int div, int invert)
{

    /* Set All pad output and Is PWM. But it still needs to be enable */
    if( !HAL_PWM_Init(pwm_id) )
    {
        printf("PWM Hal Initial Fail\n");
        return E_PWM_FAIL;
    }

	//[APN] range 2 <= div <= 65536
	if(div < 2)
		div = 0;
	else if(div > 65536)
		div = 65535;
	else
	//[APN] PWM _DIV= (DIV+1)
		div = div-1;

	HAL_PWM_Polarity(pwm_id, invert);
	HAL_PWM_SetDiv(pwm_id, div);
	pwm_enable(pwm_id);
    return E_PWM_OK;
}

int pwm_config(int pwm_id, int duty_ns, int period_ns)
{
//	PWM_Result ret = E_PWM_FAIL;
    printf("%s (id %d duty_ns %d, period_ns %d)\r\n", __FUNCTION__, pwm_id, (int)duty_ns, (int)period_ns);

	HAL_PWM_Period(pwm_id, period_ns);
	HAL_PWM_DutyCycle(pwm_id, duty_ns);

	return E_PWM_OK;
}

void pwm_disable(int pwm_id)
{
	bool bEnable = FALSE;

	HAL_PWM_Oen(pwm_id, bEnable);
}