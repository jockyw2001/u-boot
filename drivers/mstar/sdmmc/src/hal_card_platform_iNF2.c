/***************************************************************************************************************
 *
 * FileName hal_card_platform_iNF3.c
 *     @author jeremy.wang (2016/11/29)
 * Desc:
 *     The platform setting of all cards will run here.
 *     Because register setting that doesn't belong to FCIE/SDIO may have different register setting at different projects.
 *     The goal is that we don't need to change "other" HAL_XX.c Level code. (Timing, FCIE/SDIO)
 *
 *     The limitations were listed as below:
 *     (1) Each Project will have XX project name for different hal_card_platform_XX.c files.
 *     (2) IP init, PAD , clock, power and miu setting belong to here.
 *     (4) Timer setting doesn't belong to here, because it will be included by other HAL level.
 *     (5) FCIE/SDIO IP Reg Setting doesn't belong to here.
 *     (6) If we could, we don't need to change any code of hal_card_platform.h
 *
 ***************************************************************************************************************/

#include "../inc/hal_card_platform.h"
#include "../inc/hal_card_timer.h"

//***********************************************************************************************************
// Config Setting (Internel)
//***********************************************************************************************************

// Platform Register Basic Address
//------------------------------------------------------------------------------------
#define A_CHIPTOP_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x80F00)  //Bank: 0x101E
#define A_PADTOP0_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x81280)  //Bank: 0x1025
#define A_PADTOP1_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x81300)  //Bank: 0x1026
#define A_CLKGEN2_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x80500)  //Bank: 0x100A
#define A_PM_GPIO_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x00780)  //Bank: 0xF


#define A_SD_PLL_POS(IP)    GET_CARD_BANK(IP, 3)


// Clock Level Setting (From High Speed to Low Speed)
//-----------------------------------------------------------------------------------------------------------
#define CLK1_F          48000000
#define CLK1_E          43200000
#define CLK1_D          40000000
#define CLK1_C          36000000
#define CLK1_B          32000000
#define CLK1_A          20000000
#define CLK1_9          12000000
#define CLK1_8          300000
#define CLK1_7          0
#define CLK1_6          0
#define CLK1_5          0
#define CLK1_4          0
#define CLK1_3          0
#define CLK1_2          0
#define CLK1_1          0
#define CLK1_0          0

#define CLK2_F          48000000
#define CLK2_E          43200000
#define CLK2_D          40000000
#define CLK2_C          36000000
#define CLK2_B          32000000
#define CLK2_A          20000000
#define CLK2_9          12000000
#define CLK2_8          300000
#define CLK2_7          0
#define CLK2_6          0
#define CLK2_5          0
#define CLK2_4          0
#define CLK2_3          0
#define CLK2_2          0
#define CLK2_1          0
#define CLK2_0          0

#define CLK3_F          48000000
#define CLK3_E          43200000
#define CLK3_D          40000000
#define CLK3_C          36000000
#define CLK3_B          32000000
#define CLK3_A          20000000
#define CLK3_9          12000000
#define CLK3_8          300000
#define CLK3_7          0
#define CLK3_6          0
#define CLK3_5          0
#define CLK3_4          0
#define CLK3_3          0
#define CLK3_2          0
#define CLK3_1          0
#define CLK3_0          0


// Reg Dynamic Variable
//-----------------------------------------------------------------------------------------------------------
static volatile BusTimingEmType ge_BusTiming[3] = {0};


//***********************************************************************************************************
// IP Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_IPOnceSetting
 *     @author jeremy.wang (2015/7/17)
 * Desc: IP once setting , it's about platform setting.
 *
 * @param eIP : FCIE1/FCIE2/...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_IPOnceSetting(IPEmType eIP)
{

    CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PADTOP1_BANK, 0x00), BIT00_T);              //reg_all_pad_in => Close

    if(eIP == EV_IP_FCIE1)
    {
        //Enable Clock Source to avoid reset fail
        //CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T|BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T); //[5:2]: Clk_Sel [1]: Clk_i [0]: Clk_g
        //CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT06_T); // select clk_sdio_p1
    }
    else if(eIP == EV_IP_FCIE2)
    {
    }
    else if(eIP == EV_IP_FCIE3)
    {
    }


}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_IPBeginSetting
 *     @author jeremy.wang (2015/7/29)
 * Desc: IP begin setting before every operation, it's about platform setting.
 *
 * @param eIP : FCIE1/FCIE2/...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_IPBeginSetting(IPEmType eIP)
{

    if(eIP == EV_IP_FCIE1)
    {
    }
    else if(eIP == EV_IP_FCIE2)
    {
    }
    else if(eIP == EV_IP_FCIE3)
    {
    }


}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_IPEndSetting
 *     @author jeremy.wang (2015/7/29)
 * Desc: IP end setting after every operation, it's about platform setting.
 *
 * @param eIP : FCIE1/FCIE2/...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_IPEndSetting(IPEmType eIP)
{

    if(eIP == EV_IP_FCIE1)
    {
    }
    else if(eIP == EV_IP_FCIE2)
    {
    }
    else if(eIP == EV_IP_FCIE3)
    {
    }
}

//***********************************************************************************************************
// PAD Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetPADToPortPath
 *     @author jeremy.wang (2015/7/28)
 * Desc: Set PAD to connect IP Port
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param ePort : Port (But FCIE5 use it to decide FCIE or SDIO IP)
 * @param ePAD : PAD
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetPADToPortPath(IPEmType eIP, PortEmType ePort, PADEmType ePAD)
{
    SET_CARD_PORT(eIP, ePort);

    if(eIP == EV_IP_FCIE1)
    {
        if(ePAD == EV_PAD1)  //PAD_SDIO
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PADTOP1_BANK, 0x0A), BIT07_T);          //reg_sd30_mode = 1
        }

    }
    else if(eIP == EV_IP_FCIE2)
    {
        if(ePAD == EV_PAD2) //PAD_SD
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PADTOP1_BANK, 0x0A), BIT09_T|BIT08_T);  //reg_sdio_mode = 0
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PADTOP1_BANK, 0x0A), BIT08_T);          //reg_sdio_mode = 1

            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x12), BIT05_T|BIT04_T|BIT01_T|BIT00_T); //reg_rest_in/out_mode = 0
        }

    }

}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_InitPADPin
 *     @author jeremy.wang (2017/11/28)
 * Desc: Init PAD Pin Status ( pull enable, pull up/down, driving strength)
 *
 * @param eIP :  FCIE1/FCIE2
 * @param ePAD : PAD
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_InitPADPin(IPEmType eIP, PADEmType ePAD)
{

    if(eIP == EV_IP_FCIE1)
    {

        if(ePAD == EV_PAD1) //PAD_SDIO
        {

            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x47), BIT05_T|BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T);   //CLK, CMD, D3, D2, D1, D0=> pull en

            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x49), BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T);   //CMD, D3, D2, D1, D0=> ps up
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x49), BIT05_T);   //CLK => ps down

        }

    }
    else if(eIP == EV_IP_FCIE2)
    {
        if (ePAD == EV_PAD2) //PAD_SD
        {

        }
    }



}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_PullPADPin
 *     @author jeremy.wang (2017/11/28)
 * Desc: Pull PAD Pin for Special Purpose (Avoid Power loss.., Save Power)
 *
 * @param eIP : FCIE1/FCIE2/FCIE3
 * @param ePAD : PAD
 * @param ePinPull : Pull up/Pull down
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_PullPADPin(IPEmType eIP, PADEmType ePAD, PinPullEmType ePinPull)
{

    if(eIP == EV_IP_FCIE1)
    {

        if(ePAD == EV_PAD1) //PAD_SDIO
        {

            if(ePinPull ==EV_PULLDOWN)
            {
                CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x49), BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T);   //CMD, D3, D2, D1, D0=> ps up
            }
            else if(ePinPull == EV_PULLUP)
            {
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x49), BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T);   //CMD, D3, D2, D1, D0=> ps up
            }
        }

    }
    else if(eIP == EV_IP_FCIE2)
    {

    }


}


//***********************************************************************************************************
// Clock Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetClock
 *     @author jeremy.wang (2015/7/23)
 * Desc: Set Clock Level by Real Clock from IP
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param u32ClkFromIPSet : Clock Value From IP Source Set
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetClock(IPEmType eIP, U32_T u32ClkFromIPSet)
{

    if(eIP == EV_IP_FCIE1)
    {

        #if (D_FPGA_TEST)

        if(u32ClkFromIPSet==CLK1_8) //Low Speed
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT00_T);         //300KHz
        }
        else
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT00_T);         //1.5MHz
        }

        return;
        #endif


        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT13_T|BIT12_T|BIT11_T|BIT10_T|BIT09_T|BIT08_T); //[5:2]: Clk_Sel [1]: Clk_i [0]: Clk_g
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT14_T); // select clk_sdio_p1

        switch(u32ClkFromIPSet)
        {
            case CLK1_F:      //48000KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT13_T|BIT12_T|BIT11_T|BIT10_T); //15
                break;
            case CLK1_E:      //43200KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT12_T|BIT10_T); //5
                break;
            case CLK1_D:      //40000KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT12_T);  //4
                break;
            case CLK1_C:      //36000KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT11_T|BIT10_T); //3
                break;
            case CLK1_B:      //32000KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT11_T); // 2
                break;
            case CLK1_A:      //20000KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT10_T); //1
                break;
            case CLK1_9:      //12000KHz
                break;
            case CLK1_8:      //300KHz
                CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x3E), BIT13_T|BIT12_T|BIT10_T); //13
                break;
            /*
            case CLK1_7:
                break;
            case CLK1_6:
                break;
            case CLK1_5:
                break;
            case CLK1_4:
                break;
            case CLK1_3:
                break;
            case CLK1_2:
                break;
            case CLK1_1:
                break;
            case CLK1_0:
                break;*/

        }

    }
    else if(eIP == EV_IP_FCIE2)
    {

        #if (D_FPGA_TEST)

        if(u32ClkFromIPSet==CLK1_8) //Low Speed
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT00_T);         //300KHz
        }
        else
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT00_T);         //1.5MHz
        }

        return;
        #endif


        //CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T|BIT04_T|BIT03_T|BIT02_T|BIT01_T|BIT00_T); //[5:2]: Clk_Sel [1]: Clk_i [0]: Clk_g

        switch(u32ClkFromIPSet)
        {
            case CLK1_F:      //48000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT04_T|BIT02_T); //5
                break;
            case CLK1_E:      //43200KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT04_T|BIT03_T); //6
                break;
            case CLK1_D:      //40000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT04_T|BIT03_T|BIT02_T); //7
                break;
            case CLK1_C:      //36000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T); //8
                break;
            case CLK1_B:      //32000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T|BIT02_T); //9
                break;
            case CLK1_A:      //20000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T|BIT03_T); //10
                break;
            case CLK1_9:      //12000KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT03_T|BIT02_T);  //3
                break;
            case CLK1_8:      //300KHz
                //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_CLKGEN2_BANK,0x43), BIT05_T|BIT04_T); //12
                break;
            /*
            case CLK1_7:
                break;
            case CLK1_6:
                break;
            case CLK1_5:
                break;
            case CLK1_4:
                break;
            case CLK1_3:
                break;
            case CLK1_2:
                break;
            case CLK1_1:
                break;
            case CLK1_0:
                break;*/

        }

    }
    else if(eIP == EV_IP_FCIE3)
    {
        switch(u32ClkFromIPSet)
        {
            /*
            case CLK3_F:      //48000KHz
                break;
            case CLK3_E:      //43200KHz
                break;
            case CLK3_D:      //40000KHz
                break;
            case CLK3_C:      //36000KHz
                break;
            case CLK3_B:      //32000KHz
                break;
            case CLK3_A:      //24000KHz
                break;
            case CLK3_9:      //12000KHz
                break;
            case CLK3_8:      //300KHz
                break;
            case CLK2_4:
                break;
            case CLK_3:
                break;
            case CLK2_2:
                break;
            case CLK2_1:
                break;
            case CLK2_0:
                break;*/

        }

    }

}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_FindClockSetting
 *     @author jeremy.wang (2015/7/20)
 * Desc: Find Real Clock Level Setting by Reference Clock
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param u32ReffClk : Reference Clock Value
 * @param u8PassLevel : Pass Level to Clock Speed
 * @param u8DownLevel : Down Level to Decrease Clock Speed
 *
 * @return U32_T  : Real Clock
 ----------------------------------------------------------------------------------------------------------*/
U32_T Hal_CARD_FindClockSetting(IPEmType eIP, U32_T u32ReffClk, U8_T u8PassLevel, U8_T u8DownLevel)
{
    U8_T  u8LV = u8PassLevel;
    U32_T u32RealClk = 0;
    U32_T u32ClkArr[3][16] = { \
        {CLK1_F, CLK1_E, CLK1_D, CLK1_C, CLK1_B, CLK1_A, CLK1_9, CLK1_8, CLK1_7, CLK1_6, CLK1_5, CLK1_4, CLK1_3, CLK1_2, CLK1_1, CLK1_0} \
       ,{CLK2_F, CLK2_E, CLK2_D, CLK2_C, CLK2_B, CLK2_A, CLK2_9, CLK2_8, CLK2_7, CLK2_6, CLK2_5, CLK2_4, CLK2_3, CLK2_2, CLK2_1, CLK2_0} \
       ,{CLK3_F, CLK3_E, CLK3_D, CLK3_C, CLK3_B, CLK3_A, CLK3_9, CLK3_8, CLK3_7, CLK3_6, CLK3_5, CLK3_4, CLK3_3, CLK3_2, CLK3_1, CLK3_0} };

    for(; u8LV<16; u8LV++)
    {
        if( (u32ReffClk >= u32ClkArr[eIP][u8LV]) || (u8LV==15) || (u32ClkArr[eIP][u8LV+1]==0) )
        {
            u32RealClk = u32ClkArr[eIP][u8LV];
            break;
        }
    }

    /****** For decrease clock speed******/
    if( (u8DownLevel) && (u32RealClk) && ((u8LV+u8DownLevel)<=15) )
    {
        if(u32ClkArr[eIP][u8LV+u8DownLevel]>0) //Have Level for setting
            u32RealClk = u32ClkArr[eIP][u8LV+u8DownLevel];
    }

    return u32RealClk;
}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetBusTiming
 *     @author jeremy.wang (2015/7/20)
 * Desc: Platform Setting for different Bus Timing
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param eBusTiming : LOW/DEF/HS/SDR12/DDR...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetBusTiming(IPEmType eIP, BusTimingEmType eBusTiming)
{
    ge_BusTiming[eIP] = eBusTiming;
}


//***********************************************************************************************************
// Power and Voltage Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_PowerOn
 *     @author jeremy.wang (2017/5/24)
 * Desc: Power ON Card Power
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param ePAD : PAD
 * @param u16DelayMs : Delay ms for stable power
----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_PowerOn(IPEmType eIP, PADEmType ePAD, U16_T u16DelayMs)
{
    if(ePAD==EV_PAD1) //PAD_SD0
    {
        #if (D_FPGA_TEST)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT08_T);         //PAD_PWRCTL : output mode
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT08_T);         //PAD_PWRCTL : output 0

        #else

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x06), BIT00_T);           // output mode
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x06), BIT01_T);           // output:0

        #endif

    }
    else if(ePAD==EV_PAD2) //PAD_SD1
    {

        #if (D_FPGA_TEST)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT08_T);         //PAD_PWRCTL : output mode
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT08_T);         //PAD_PWRCTL : output 0

        #else


        #endif

    }

    Hal_Timer_mDelay(u16DelayMs);

}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_PowerOff
 *     @author jeremy.wang (2017/5/24)
 * Desc: Power Off Card Power
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param ePAD : PAD
 * @param u16DelayMs :  Delay ms to confirm no any power
----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_PowerOff(IPEmType eIP, PADEmType ePAD, U16_T u16DelayMs)
{
    if( (ePAD==EV_PAD1)) //PAD_SD0
    {

        #if (D_FPGA_TEST)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT08_T);         //PAD_PWRCTL : output mode
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT08_T);         //PAD_PWRCTL : output 1

        #else

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x06), BIT00_T);           // output mode
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x06), BIT01_T);           // output:1

        #endif

    }
    else if(ePAD==EV_PAD2) //PAD_SD1
    {

        #if (D_FPGA_TEST)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT08_T);         //PAD_PWRCTL : output mode
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT08_T);         //PAD_PWRCTL : output 1

        #else


        #endif

    }

    Hal_Timer_mDelay(u16DelayMs);
}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetPADVdd
 *     @author jeremy.wang (2017/6/12)
 * Desc:
 *
 * @param eIP : FCIE1/FCIE2/...
 * @param ePAD : PAD
 * @param ePADVdd :
 * @param u16DelayMs : Delay ms after set PAD power
----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetPADVdd(IPEmType eIP, PADEmType ePAD, PADVddEmType ePADVdd, U16_T u16DelayMs)
{

    #if(!D_FPGA_TEST)
    U16_T u16Reg = 0;
    U8_T u8Timeout = 0;
    #endif

    if(ePAD == EV_PAD1) //Pad SD0
    {

        #if (D_FPGA_TEST)

        //FPGA
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT06_T);         //PAD_LSOE : output mode
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT06_T);         //PAD_LSOE : output 1 (using)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT07_T);         //PAD_VSEL : output mode
                                                                                      //
        if(ePADVdd==EV_NORVOL)
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT07_T);         //PAD_VSEL : output 0
        }
        else if(ePADVdd==EV_LOWVOL)
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT07_T);         //PAD_VSEL : output 1
        }

        #else

        //1. Disable OSP : FALSE =>(Enable)
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT15_T);

        //2. Set OSP counter[15:8] = 0x30
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT15_T|BIT14_T|BIT13_T|BIT12_T|BIT11_T|BIT10_T|BIT09_T|BIT08_T);
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT13_T|BIT12_T);

        //3. Switch to 3.3V
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT04_T|BIT02_T);

        //3. Turning on LDO  1->0
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT05_T);
        Hal_Timer_mDelay(10);
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT05_T);

        //4. Check if 3.3V power is ready
        do
        {

            u16Reg = CARD_REG(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x10)) & BIT12_T;

            if(u16Reg)
                break;

            u8Timeout++;
            Hal_Timer_mDelay(1);

        } while(u8Timeout<10);

        #endif


    }
    else if(ePAD == EV_PAD2) //PAD_SD1
    {

        #if (D_FPGA_TEST)

        //FPGA
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT06_T);         //PAD_LSOE : output mode
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT06_T);         //PAD_LSOE : output 1 (using)

        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x43), BIT07_T);         //PAD_VSEL : output mode
                                                                                      //
        if(ePADVdd==EV_NORVOL)
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT07_T);         //PAD_VSEL : output 0
        }
        else if(ePADVdd==EV_LOWVOL)
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x41), BIT07_T);         //PAD_VSEL : output 1
    }

        #else

        //1. Disable OSP : FALSE =>(Enable)
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x1D), BIT15_T);

        //2. Set OSP counter[15:8] = 0x30
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT15_T|BIT14_T|BIT13_T|BIT12_T|BIT11_T|BIT10_T|BIT09_T|BIT08_T);
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT13_T|BIT12_T);

        //3. Switch to 3.3V
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT04_T|BIT02_T);

        //3. Turning on LDO  1->0
        CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT05_T);
        Hal_Timer_mDelay(10);
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x37), BIT05_T);

        //4. Check if 3.3V power is ready
        do
        {

            u16Reg = CARD_REG(GET_CARD_REG_ADDR(A_SD_PLL_POS(eIP), 0x10)) & BIT12_T;

            if(u16Reg)
                break;

            u8Timeout++;
            Hal_Timer_mDelay(1);

        } while(u8Timeout<10);

        #endif


    }
    else if(ePAD == EV_PAD3)
    {
        if(ePADVdd == EV_NORVOL)
        {
        }
    }

    Hal_Timer_mDelay(u16DelayMs);

}


//***********************************************************************************************************
// Card Detect and GPIO Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_InitGPIO
 *     @author jeremy.wang (2017/9/15)
 * Desc: Init GPIO Setting for CDZ or other GPIO (Pull high/low and driving, base SD/GPIO mode setting)
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/..
 * @param bEnable : Enable GPIO or disable GPIO to avoid loss power
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_InitGPIO(GPIOEmType eGPIO, PADEmType ePAD, BOOL_T bEnable)
{

    if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {
        if(bEnable)
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x04E), BIT00_T);         //input mode
        }
        else
        {
        }
    }
    else if( eGPIO==EV_GPIO2 ) //EV_GPIO2 for Slot 1
    {
        if(bEnable)
        {
            //CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x04E), BIT06_T);         //input mode

        }
        else
        {
        }
    }

}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_GetGPIOState
 *     @author jeremy.wang (2017/9/15)
 * Desc: Get GPIO input mode value (Include input mode setting)
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/..
 *
 * @return BOOL_T  : TRUE or FALSE
 ----------------------------------------------------------------------------------------------------------*/
BOOL_T Hal_CARD_GetGPIOState(GPIOEmType eGPIO, PADEmType ePAD)
{
    U16_T u16Reg = 0;

    if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {
        u16Reg = CARD_REG(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x4E)) & BIT02_T;


        if(!u16Reg) //Low Active
            return (TRUE);
        else
            return (FALSE);

    }
    if( eGPIO==EV_GPIO2 ) //EV_GPIO2 for Slot 1
    {

        //u16Reg = CARD_REG(GET_CARD_REG_ADDR(A_PM_GPIO_BANK, 0x4E)) & BIT02_T;


        if(!u16Reg) //Low Active
            return (TRUE);
        else
            return (FALSE);

    }

    return (FALSE);
}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetGPIOState
 *     @author jeremy.wang (2017/9/15)
 * Desc: Set GPIO output mode value (Include output mode setting), it's for SDIO WIFI control using
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/..
 * @param bOutputState : TRUE or FALSE
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetGPIOState(GPIOEmType eGPIO, PADEmType ePAD, BOOL_T bOutputState)
{

    /*if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {
        CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PMGPIO_BANK, 0x05), BIT00_T);           //PMU_GPIO_OUT_EN

        if(bOutputState)
        {
            CARD_REG_SETBIT(GET_CARD_REG_ADDR(A_PMGPIO_BANK, 0x07), BIT00_T);        //PMU_GPIO_OUT=1
        }
        else
        {
            CARD_REG_CLRBIT(GET_CARD_REG_ADDR(A_PMGPIO_BANK, 0x07), BIT00_T);       //PMU_GPIO_OUT=0
        }
    }
    if( eGPIO==EV_GPIO2 ) //EV_GPIO2 for Slot 1
    {
    }
    if( eGPIO==EV_GPIO3 ) //EV_GPIO2 for Slot 1
    {
    }

    // Add a 500ms Delay after card removing to avoid the next card inserting issue
    if(bOutputState==1)
    {
        Hal_Timer_mSleep(500);
    }
    else // For 0->1, 1->0 stable
    {
        Hal_Timer_mSleep(1);
    }*/

}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_GetGPIONum
 *     @author jeremy.wang (2017/9/15)
 * Desc: Get GPIO number for special platform (like Linux) to use it get irq number
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/...
 *
 * @return U32_T  : GPIO number
 ----------------------------------------------------------------------------------------------------------*/
U32_T Hal_CARD_GetGPIONum(GPIOEmType eGPIO, PADEmType ePAD)
{
    S32_T s32GPIO = -1;

    /*if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {
        //s32GPIO = DrvPadmuxGetGpio(IO_CHIP_INDEX_SD_CDZ);
    }
    else if( eGPIO==EV_GPIO2)
    {
    }*/

    if(s32GPIO>0)
        return (U32_T)s32GPIO;
    else
        return 0;
}


#if (D_OS == D_OS__LINUX)
#include <linux/irq.h>
#include "../../../mstar/include/infinity/irqs.h"
extern struct irq_chip gic_arch_extn;
#endif


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetGPIOIntAttr
 *     @author jeremy.wang (2017/9/15)
 * Desc: Set GPIO Interrupt Attribute (Option 1..5), it could design for different requirement
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/...
 * @param eGPIOOPT : Option1/Option2/...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetGPIOIntAttr(GPIOEmType eGPIO, PADEmType ePAD, GPIOOptEmType eGPIOOPT)
{

#if (D_OS == D_OS__LINUX)
    struct irq_data sd_irqdata;
    //struct irq_chip *chip;
    if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {

        if(ePAD==EV_PAD1) //PAD_SD
        {
            sd_irqdata.irq = (INT_FIQ_SD_CDZ+32);
            sd_irqdata.hwirq = (INT_FIQ_SD_CDZ+32);
        }
        else if(ePAD==EV_PAD2) //PAD_NAND
        {
            sd_irqdata.irq = (INT_PMSLEEP_GPIO_7+160);
            sd_irqdata.hwirq = (INT_PMSLEEP_GPIO_7+160);
        }
    }
    else if( eGPIO==EV_GPIO2)
    {
        if(ePAD==EV_PAD1) //PAD_SD
        {
            sd_irqdata.irq = (INT_FIQ_SD_CDZ+32);
            sd_irqdata.hwirq = (INT_FIQ_SD_CDZ+32);
        }

    }
    else if(eGPIO==EV_GPIO3)
    {
    }

#ifdef CONFIG_ARM_GIC
    //chip= &gic_arch_extn;
#else
    //chip = irq_get_chip(sd_irqdata.irq);
#endif

    if(eGPIOOPT==EV_GPIO_OPT1) //clear interrupt
    {
        //chip->irq_ack(&sd_irqdata);
    }
    else if((eGPIOOPT==EV_GPIO_OPT2))
    {
    }
    else if((eGPIOOPT==EV_GPIO_OPT3))  //sd polarity _HI Trig for remove
    {
        //chip->irq_set_type(&sd_irqdata,(IRQ_TYPE_EDGE_FALLING|IRQ_TYPE_LEVEL_HIGH));
    }
    else if((eGPIOOPT==EV_GPIO_OPT4)) //sd polarity _LO Trig for insert
    {
        //chip->irq_set_type(&sd_irqdata,(IRQ_TYPE_EDGE_RISING|IRQ_TYPE_LEVEL_LOW));
    }


#endif


}


/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_GPIOIntFilter
 *     @author jeremy.wang (2017/9/15)
 * Desc: GPIO Interrupt Filter, it could design to filter GPIO Interrupt (Many sources belong to the same one)
 *
 * @param eGPIO : GPIO1/GPIO2/...
 * @param ePAD : PAD1/PAD2/...
 *
 * @return BOOL_T  : TRUE or FALSE
 ----------------------------------------------------------------------------------------------------------*/
BOOL_T Hal_CARD_GPIOIntFilter(GPIOEmType eGPIO, PADEmType ePAD)
{

    if( eGPIO==EV_GPIO1 ) //EV_GPIO1 for Slot 0
    {
        return (TRUE);
    }
    else if( eGPIO==EV_GPIO2 )
    {
         return (TRUE);
    }
    else if( eGPIO==EV_GPIO3 )
    {
         return (TRUE);
    }

    return (FALSE);
}


//***********************************************************************************************************
// MIU Setting for Card Platform
//***********************************************************************************************************

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_TransMIUAddr
 *     @author jeremy.wang (2015/7/31)
 * Desc: Transfer original address to HW special dma address (MIU0/MIU1)
 *
 * @param u32Addr : Original address
 *
 * @return U32_T  : DMA address
 ----------------------------------------------------------------------------------------------------------*/
U32_T Hal_CARD_TransMIUAddr(U32_T u32Addr)
{
    return u32Addr;
}
