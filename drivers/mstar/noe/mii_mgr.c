//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2017 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************

#include <common.h>
#include <command.h>
#include "mhal_noe.h"

#if defined (CONFIG_MIPS16)
#define outw(address, value)    *((volatile uint32_t *)(address)) = (value)
#define inw(address)            (*(volatile u32 *)(address))
#else
#define outw(address, value)    *((volatile uint32_t *)(address)) = cpu_to_le32(value)
#define inw(address)            le32_to_cpu(*(volatile u32 *)(address))
#endif

#define PHY_CONTROL_0       0x0004
#define MDIO_PHY_CONTROL_0  (NOE_ETH_MAC_BASE + PHY_CONTROL_0)

#define enable_mdio(x)

#define CFG_HZ              1000

u32 __mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{
    u32 volatile            status  = 0;
    u32 volatile            data    = 0;
    u32             rc      = 0;
    unsigned long volatile  t_start = get_timer(0);

    //printf("\n MDIO Read operation!!\n");
    // make sure previous read operation is complete
    t_start = get_timer(0);
    while(1)
    {
        // 0 : Read/write operation complet
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            break;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Read operation is ongoing !!\n");
            return rc;
        }
        udelay(10);
    }

    data  = (0x01 << 16) | (0x02 << 18) | (phy_addr << 20) | (phy_register << 25);
    outw(MDIO_PHY_CONTROL_0, data);
    data |= (1<<31);
    outw(MDIO_PHY_CONTROL_0, data);
    //printf("\n Set Command [0x%08X] to PHY 0x%8x!!\n", data, MDIO_PHY_CONTROL_0);


    // make sure read operation is complete
    t_start = get_timer(0);
    while(1)
    {
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            status = inw(MDIO_PHY_CONTROL_0);
            *read_data = (u32)(status & 0x0000FFFF);
            //printf("\n MDIO_PHY_CONTROL_0: 0x%8x!!\n", status);

            return 1;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Read operation is ongoing and Time Out!!\n");
            return 0;
        }
        udelay(10);
    }
}

u32 __mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
    unsigned long volatile  t_start=get_timer(0);
    u32 volatile  data;

    // make sure previous write operation is complete
    t_start = get_timer(0);
    while(1)
    {
        if (!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31)))
        {
            break;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Write operation is ongoing !!\n");
            return 0;
        }
        udelay(10);
    }
    udelay(1);//make sequencial write more robust

    data = (0x01 << 16) | (1<<18) | (phy_addr << 20) | (phy_register << 25) | write_data;
    outw(MDIO_PHY_CONTROL_0, data);
    data |= (1<<31);
    outw(MDIO_PHY_CONTROL_0, data); //start operation
    //printf("\n Set Command [0x%08X] to PHY 0x%8x!!\n", data, MDIO_PHY_CONTROL_0);

    t_start = get_timer(0);

    // make sure write operation is complete
    while(1)
    {
        if(!( inw(MDIO_PHY_CONTROL_0) & (0x1 << 31))) // 0 : Read/write operation complete
        {
            return 1;
        }
        else if((get_timer(0)-t_start) > (5 * CFG_HZ))
        {
            printf("\n MDIO Write operation is ongoing and Time Out!!\n");
            return 0;
        }
        udelay(10);
    }
}

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{

    {
        if(__mii_mgr_read(phy_addr, phy_register, read_data)) {
//          printf("%s: phy_addr=%x phy_register=%x *read_data=%x\n",__FUNCTION__, phy_addr, phy_register, *read_data);
            return 1;
        }
    }

    return 0;
}

u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
//  printf("%s: phy_addr=%x phy_register=%x write_data=%x\n", __FUNCTION__, phy_addr, phy_register, write_data);
    {
        if(__mii_mgr_write(phy_addr, phy_register, write_data)) {
            return 1;
        }
    }

    return 0;
}


int noe_mii_read(const char *devname, unsigned char phy_addr, unsigned char phy_register, unsigned short *read_data)
{
    u32 regValue;
    if (mii_mgr_read(phy_addr, phy_register, &regValue)) {
        /* mdio read operation is successfully executed */
        *read_data = (unsigned short)regValue;
        return 0;
    }
    else {
        return 1;
    }
}


int noe_mii_write(const char *devname, unsigned char phy_addr, unsigned char phy_register, unsigned short write_data)
{
    if (mii_mgr_write(phy_addr, phy_register, write_data)) {
        /* mdio write operation is successfully executed */
        return 0;
    }
    else {
        return 1;
    }
}


void dump_phy_reg(int port_no, int from, int to, int is_local, int page_no)
{
    u32 i=0;
    u32 temp=0;
    u32 r31=0;

    if(is_local==0) {
        printf("\n\nGlobal Register Page %d\n",page_no);
        printf("===============");
        r31 = 0 << 15; //global
        r31 = page_no&0x7 << 12; //page no
        mii_mgr_write(0, 31, r31); //select global page x
        for(i=16;i<32;i++) {
        if(i%8==0) {
            printf("\n");
        }
        mii_mgr_read(port_no,i, &temp);
        printf("%02d: %04X ",i, temp);
        }
    }else {
        printf("\n\nLocal Register Port %d Page %d\n",port_no, page_no);
        printf("===============");
        r31 = 1 << 15; //local
        r31 = page_no&0x7 << 12; //page no
        mii_mgr_write(0, 31, r31); //select local page x
        for(i=16;i<32;i++) {
        if(i%8==0) {
            printf("\n");
        }
        mii_mgr_read(port_no,i, &temp);
        printf("%02d: %04X ",i, temp);
        }
    }
    printf("\n");
}

#ifndef ON_BOARD_NAND_FLASH_COMPONENT
#define MDIO_DBG_CMD
#endif
int noe_mdio_access(cmd_tbl_t *cmdtp, int flag, int argc,  char * const argv[])
{
    u32 addr;
    u32 phy_addr;
    u32 value = 0,bit_offset,temp;
    u32 i=0;

    if(!memcmp(argv[0],"mdio.anoff",sizeof("mdio.anoff")))
    {
        value = inw(MDIO_PHY_CONTROL_0);
        value |= (1<<15);
        outw(MDIO_PHY_CONTROL_0,value);
        puts("\n GMAC1 Force link status enable !! \n");
    }
    else if(!memcmp(argv[0],"mdio.anon",sizeof("mdio.anon")))
    {
        value = inw(MDIO_PHY_CONTROL_0);
        value &= ~(1 << 15);
        outw(MDIO_PHY_CONTROL_0,value);
        puts("\n GMAC1 Force link status disable !! \n");
    }
    else if(!memcmp(argv[0],"mdio.r",sizeof("mdio.r")))
    {
        if (argc != 3) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
            }
        phy_addr = simple_strtoul(argv[1], NULL, 10);

        if(phy_addr == 31) {
            addr = simple_strtoul(argv[2], NULL, 16);
        } else {
            addr = simple_strtoul(argv[2], NULL, 10);
        }

        phy_addr &=0x1f;

        if(mii_mgr_read(phy_addr, addr, &value))
            printf("\n mdio.r addr[0x%08X]=0x%08X\n",addr,value);
        else {
            printf("\n Read addr[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.w",sizeof("mdio.w")))
    {
        if (argc != 4) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
            }
        phy_addr = simple_strtoul(argv[1], NULL, 10);

        if(phy_addr == 31) {
            addr = simple_strtoul(argv[2], NULL, 16);
        } else {
            addr = simple_strtoul(argv[2], NULL, 10);
        }

        value = simple_strtoul(argv[3], NULL, 16);
        phy_addr &=0x1f;

        if(mii_mgr_write(phy_addr, addr,value)) {
            printf("\n mdio.w addr[0x%08X]  value[0x%08X]\n",addr,value);
        }
        else {
            printf("\n Write[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.wb",sizeof("mdio.wb")))
    {
        if (argc != 4) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
        addr = simple_strtoul(argv[1], NULL, 10);
        bit_offset = simple_strtoul(argv[2], NULL, 10);
        value = simple_strtoul(argv[3], NULL, 10);

        if(mii_mgr_read(31, addr,&temp)) {

        }
        else {
            printf("\n Rasd PHY fail while mdio.wb was called\n");
            return 1;
        }

        if(value) {
            printf("\n Set bit[%d] to '1' \n",bit_offset);
            temp |= (1<<bit_offset);
        }
        else {
            printf("\n Set bit[%d] to '0' \n",bit_offset);
            temp &= ~(1<<bit_offset);
        }

        if(mii_mgr_write(31, addr,temp)) {
            printf("\n mdio.wb addr[0x%08X]  value[0x%08X]\n",addr,temp);
        }
        else {
            printf("\n Write[0x%08X] is Fail!!\n",addr);
        }
    }
    else if(!memcmp(argv[0],"mdio.d",sizeof("mdio.d")))
    {
        //RT6855A, RT6855, MT7620, MT7621
        /* SPEC defined Register 0~15
         * Global Register 16~31 for each page
         * Local Register 16~31 for each page
         */
        printf("SPEC defined Register\n");
        printf("===============");
        for(i=0;i<=16;i++) {
            if(i%8==0) {
            printf("\n");
            }
            mii_mgr_read(0,i, &temp);
            printf("%02d: %04X ",i, temp);
        }

        if (argc == 2) {
            addr = simple_strtoul(argv[1], NULL, 10);
            dump_phy_reg(addr, 16, 31, 0, i);
            dump_phy_reg(addr, 16, 31, 1, 0); //dump local page 0
            dump_phy_reg(addr, 16, 31, 1, 1); //dump local page 1
            dump_phy_reg(addr, 16, 31, 1, 2); //dump local page 2
            dump_phy_reg(addr, 16, 31, 1, 3); //dump local page 3
        }else {

            for(i=0;i<4;i++) { //global register  page 0~4
            dump_phy_reg(0, 16, 31, 0, i);
            }

            for(i=0;i<5;i++) { //local register port 0-port4
            dump_phy_reg(i, 16, 31, 1, 0); //dump local page 0
            dump_phy_reg(i, 16, 31, 1, 1); //dump local page 1
            dump_phy_reg(i, 16, 31, 1, 2); //dump local page 2
            dump_phy_reg(i, 16, 31, 1, 3); //dump local page 3
            }
        }
    }
    else if(!memcmp(argv[0],"mdio.s",sizeof("mdio.s")))
    {
#define MSTAR_ADDR(reg)  ((reg)<<1)
#define MSTAR_BANK(bank) (RIU_REG_BASE + (bank << 9))
#define MSTART_REG(bank,reg)  *((volatile unsigned int*)(MSTAR_BANK(bank)+MSTAR_ADDR(reg)))

        if (argc != 2) {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
        value = simple_strtoul(argv[1], NULL, 3);

        i = MSTART_REG( 0x121f,0x3c);
        if ( value ==0 )
        {
            printf ("MDIO switch to ChipTop Pad!\n\n");
            i&=~(1<14);
        }
        else
        {
            printf ("MDIO switch to PM Pad!\n\n");
            i|=(1<14);
        }
        MSTART_REG( 0x121f,0x3c) = i;
    }
    return 0;
}

U_BOOT_CMD(
    mdio,   4,  1,  noe_mdio_access,
    "mdio   - Ralink PHY register R/W command !!\n",
    "mdio.r [phy_addr(dec)] [reg_addr(dec)] \n"
    "mdio.w [phy_addr(dec)] [reg_addr(dec)] [data(HEX)] \n"
    "mdio.anoff GMAC1 Force link status enable !!  \n"
    "mdio.anon GMAC1 Force link status disable !!  \n"
    "mdio.wb [phy register(dec)] [bit offset(Dec)] [Value(0/1)]  \n"
    "mdio.d - dump all Phy registers \n"
    "mdio.d [phy register(dec)] - dump Phy registers \n"
    "mdio.s [0- ChipTop , 1- PM ] - switch MDIO Pad \n");


