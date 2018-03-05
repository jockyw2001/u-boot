/*
 * [origin: Linux kernel drivers/watchdog/at91sam9_wdt.c]
 *
 * Watchdog driver for Atmel AT91SAM9x processors.
 *
 * Copyright (C) 2008 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 * Copyright (C) 2008 Renaud CERRATO r.cerrato@til-technologies.fr
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * The Watchdog Timer Mode Register can be only written to once. If the
 * timeout need to be set from U-Boot, be sure that the bootstrap doesn't
 * write to this register. Inform Linux to it too
 */

#include <common.h>
#include <watchdog.h>
#include "environment.h"
#include "asm/arch/mach/ms_types.h"
#include "asm/arch/mach/platform.h"
#include "asm/arch/mach/io.h"
#include "ms_version.h"

void hw_watchdog_disable(void)
{
	OUTREG16(GET_REG_ADDR(REG_ADDR_BASE_WDT, WDT_MAX_PRD_H), (((CONFIG_PIUTIMER_CLOCK * WDT_TIMEOUT_DEFAULT)>>16) & 0x0));
    OUTREG16(GET_REG_ADDR(REG_ADDR_BASE_WDT, WDT_MAX_PRD_L), ((CONFIG_PIUTIMER_CLOCK * WDT_TIMEOUT_DEFAULT) & 0x0));
}


void hw_watchdog_reset(void)
{
	OUTREG16(GET_REG_ADDR(REG_ADDR_BASE_WDT, WDT_WDT_CLR), 0x1); //clear
}

void hw_watchdog_init(void)
{
    char *pEnv = NULL;
	unsigned int u32Timer = WDT_TIMEOUT_DEFAULT;
//	pEnv = getenv(WDT_ENABLE_ENV_STR);

	pEnv = getenv(WDT_TIMEOUT_ENV_STR);

	if(pEnv != NULL)
	{
		u32Timer = (unsigned int)simple_strtol(pEnv, NULL, 10);
	}
	if(u32Timer < WDT_TIMEOUT_MIN) // NOTE: should not be too short to prevent infinite reboot
	{
//			UBOOT_INFO("%s: Incorrect WDT TIMEOUT, should be not less than '%u', at %d\n", __func__, WDT_TIMEOUT_MIN, __LINE__);
		u32Timer = WDT_TIMEOUT_MIN;
	}
	printf("[WDT] Timer:%d\n", u32Timer);
	OUTREG16(GET_REG_ADDR(REG_ADDR_BASE_WDT, WDT_MAX_PRD_H), (((CONFIG_PIUTIMER_CLOCK * u32Timer)>>16) & 0xFFFF));
	OUTREG16(GET_REG_ADDR(REG_ADDR_BASE_WDT, WDT_MAX_PRD_L), ((CONFIG_PIUTIMER_CLOCK * u32Timer) & 0xFFFF));
	hw_watchdog_reset();


}

