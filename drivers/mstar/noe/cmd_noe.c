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

/*
 * NOE Read/Write Utilities
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <net.h>

/*-----------------------------------------------------------------------
 * Definitions
 */

typedef enum {
    E_NOE_CMD_READ = 0,
    E_NOE_CMD_WRITE,
} EN_NOE_CMD_TYPE;

typedef enum {
    E_NOE_CMD_BIST_MODE_NORMAL = 0,
    E_NOE_CMD_BIST_MODE_PHY_LOOPBACK,
    E_NOE_CMD_BIST_MODE_MAC_LOOPBACK,
} EN_NOE_CMD_BIST_MODE;

#define NOE_INPUT_LEN           2
#define NOE_ADDR_INPUT_LEN      7

#define MACADDR_ENV    "ethaddr"
#define MACADDR_FORMAT "XX:XX:XX:XX:XX:XX"

extern u8 MY_MAC[6];
extern int noe_init;
extern void MDrv_NOE_Dump_Register(void);
extern int MDrv_NOE_Initialize(const bd_t * bis);
extern u8 MDrv_NOE_Bist(u8 bEnable);
extern int MDrv_NOE_Set_PinMux(u8 cmd, u32 eport, u32 connect);
extern int MDrv_NOE_Set_Phyaddr(u8 cmd, u32 phyaddr);
extern void MHal_NOE_Write_SA1_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5);
extern int MDrv_NOE_eWave(int mode, int subitem);
extern int MDrv_NOE_Loopback_Is_Failed(void);

static char last_op[2];
static char last_phyaddr_op[2];

int do_emac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    bd_t * bis = NULL;
    MDrv_NOE_Initialize(bis);
    printf("(Re)start NOE...\n");
    return 0;
}

int do_setMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cmdline_buf = NULL;
    if ((argc < NOE_ADDR_INPUT_LEN) || (argc > NOE_ADDR_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MY_MAC[0]    = simple_strtoul(argv[1], NULL, 16);
    MY_MAC[1]    = simple_strtoul(argv[2], NULL, 16);
    MY_MAC[2]    = simple_strtoul(argv[3], NULL, 16);
    MY_MAC[3]    = simple_strtoul(argv[4], NULL, 16);
    MY_MAC[4]    = simple_strtoul(argv[5], NULL, 16);
    MY_MAC[5]    = simple_strtoul(argv[6], NULL, 16);

    cmdline_buf = malloc(64);
    if(cmdline_buf == NULL) {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__); //cyber
        return -1;
    }
    sprintf(cmdline_buf, "setenv -f "MACADDR_ENV" %02X:%02X:%02X:%02X:%02X:%02X", MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);
    run_command(cmdline_buf, 0);
    free(cmdline_buf);

    printf("MAC Address %02X:%02X:%02X:%02X:%02X:%02X\n",MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    // Set MAC address ------------------------------------------------------
    MHal_NOE_Write_SA1_MAC_Address(MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);
    //MHal_NOE_Write_SA2_MAC_Address(MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);
    //MHal_NOE_Write_SA3_MAC_Address(MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);
    //MHal_NOE_Write_SA4_MAC_Address(MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    return 0;
}

int noe_cmd_dump_reg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }
    printf("Dump NOE Register\n");
    MDrv_NOE_Dump_Register();
    return 0;
}


int do_bistMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 count = simple_strtoul(argv[1], NULL, 16);

    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }

    if (0 == count) {
        printf("Usage:\n ebist count (count:0~255 times)\n Ex: ebist 10\n");
    }
    else {
        u8 idx= 0;
        char buffer[256], ip_addr[][15] =
            {
                "10.0.0.253",
                "127.0.0.1",
                "172.16.90.254",
                "192.168.0.1",
                "224.30.0.253",
                "0"
            };

        if (MDrv_NOE_Bist(E_NOE_CMD_BIST_MODE_PHY_LOOPBACK)) {
            for (; idx < count; idx++) {
                memset(buffer, 0 , sizeof(buffer));
                sprintf(buffer, "ping %s", ip_addr[idx%5]);
                printf("\n=========>  %s for PHY Loopback test.....\n", buffer);
                run_command(buffer, 0);
            }
            if (MDrv_NOE_Loopback_Is_Failed()) {
                printf("\n### NOE PHY loopback test FAILED ###\n");
            }
            else {
                printf("\n### NOE PHY loopback test SUCCEEDED ###\n");
            }
        }
        MDrv_NOE_Bist(E_NOE_CMD_BIST_MODE_NORMAL);
    }

    return 0;
}

int do_loadspi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ulong ram_start;
    ulong spi_start;
    ulong len,i;
    if (argc != 4) {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    spi_start = simple_strtoul(argv[1], NULL, 10);
    ram_start = simple_strtoul(argv[2], NULL, 10);
    len       = simple_strtoul(argv[3], NULL, 10);
    printf("copy from spi(%lux) to ram(%lux) with len=%lux\n",spi_start,ram_start,len);

    for(i=0;i<len;i+=4)
    {
        if(0==i%1024)printf(".");
        *((ulong*)(ram_start+i))=*((ulong*)(spi_start+i));
        //printf("[%x]=%x,[%x]=%x\n",ram_start+i,*((ulong*)(ram_start+i)),spi_start+i,*((ulong*)(spi_start+i)));
    }

    return 0;
}

int noe_cmd_set_pinmux(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char op[2];
    u32 eport = 0, connect = 0;

    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }

    if (argc < 2) {
        return cmd_usage(cmdtp);
    }

    /*
     * We use the last specified parameters, unless new ones are
     * entered.
     */
    op[0] = last_op[0];
    op[1] = last_op[1];

    if ((flag & CMD_FLAG_REPEAT) == 0) {
        op[0] = argv[1][0];
        if (strlen(argv[1]) > 1) {
            op[1] = argv[1][1];
        }
        else {
            op[1] = '\0';
        }
        if (argc >= 3) {
            eport = simple_strtoul(argv[2], NULL, 10);
        }
        if (argc >= 4) {
            connect = simple_strtoul(argv[3], NULL, 10);
        }
    }

    /*
     * check read/set.
     */
    if (op[0] == 'r') {
        MDrv_NOE_Set_PinMux(E_NOE_CMD_READ, eport, connect);
    }
    else if (op[0] == 'w') {
        if (argc < 4) {
            printf(
                "The noe_pinmux write command only formats the "
                "<ethernet_port> <connect_to>\n");
            return 1;
        }
        if (MDrv_NOE_Set_PinMux(E_NOE_CMD_WRITE, eport, connect) != 0) {
            return 1;
        }
    }
    else {
        return cmd_usage(cmdtp);
    }

    /*
     * Save the parameters for repeats.
     */
    last_op[0] = op[0];
    last_op[1] = op[1];

    return 0;
}

int noe_cmd_set_phyaddr(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char op[2];
    u32 phyaddr = 0;
    if (argc < 2) {
        return cmd_usage(cmdtp);
    }
    /*
     * We use the last specified parameters, unless new ones are
     * entered.
     */
    op[0] = last_phyaddr_op[0];
    op[1] = last_phyaddr_op[1];

    if ((flag & CMD_FLAG_REPEAT) == 0) {
        op[0] = argv[1][0];
        if (strlen(argv[1]) > 1) {
            op[1] = argv[1][1];
        }
        else {
            op[1] = '\0';
        }
        if (argc >= 3) {
            phyaddr = simple_strtoul(argv[2], NULL, 16);
        }
    }

    /*
     * check read/set.
     */
    if (op[0] == 'r') {
        MDrv_NOE_Set_Phyaddr(E_NOE_CMD_READ, phyaddr);
    }
    else if (op[0] == 'w') {
        if (argc < 3) {
            printf(
                "The noe_phyaddr write command only formats the "
                "<addr> \n");
            return 1;
        }
        MDrv_NOE_Set_Phyaddr(E_NOE_CMD_WRITE, phyaddr);
    }
    else {
        return cmd_usage(cmdtp);
    }

    last_phyaddr_op[0] = op[0];
    last_phyaddr_op[1] = op[1];

    return 0;
}

int noe_cmd_ewave(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 mode     = simple_strtoul(argv[1], NULL, 10);
    u8 subitem  = simple_strtoul(argv[2], NULL, 10);

    if (!noe_init)
    {
        printf("Please start NOE first!\n");
        return 0;
    }

    if (argc < 3) {
        return cmd_usage(cmdtp);
    }

    printf("select NOE ewavetest (mode, subitem): (%d, %d)...\n", mode, subitem);
    if (MDrv_NOE_eWave(mode, subitem)) {
        return cmd_usage(cmdtp);
    }
    else {
        return 0;
    }
}

/***************************************************/

U_BOOT_CMD(
    estart, NOE_INPUT_LEN, 1,  do_emac,
    "NOE start",
    "reset  - reset NOE controller\n"
    "start  - start NOE controller\n"
);

U_BOOT_CMD(
    macaddr, NOE_ADDR_INPUT_LEN,   1,  do_setMac,
    "setup NOE MAC addr",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
    noe_dump_reg,   4,  1,  noe_cmd_dump_reg,
    "noe_dump_reg   - Dump NOE relevant register !!\n",
    "noe_dump_reg   - Dump NOE relevant register !!\n"
);

U_BOOT_CMD(
    noe_pinmux,   4,  1,  noe_cmd_set_pinmux,
    "NOE pinmux commands",
    "read                    - read pinmux info !!\n"
    "noe_pinmux write <eport> <connect> - set NOE pinmux EthernetPort <eport> connect to <connect>!!\n"
    "eport:   1 - Use GE1\n"
    "         2 - Use GE2\n"
    "connect: 1 - Internal GPHY\n"
    "         2 - PM Pad for External GPHY\n"
    "         3 - Chiptop Pad for External GPHY\n"
);

U_BOOT_CMD(
    noe_phyaddr,   3,  1,  noe_cmd_set_phyaddr,
    "NOE PHY commands",
    "read         - read PHY address !!\n"
    "noe_phyaddr write <addr> - write PHY <hex:addr> address !!\n"
);

#ifdef CONFIG_MINIUBOOT
#else

U_BOOT_CMD(
    loadspi, 4, 4,  do_loadspi,
    "load data from SPI",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
    ebist, NOE_INPUT_LEN,  1,  do_bistMac,
    "PHY loopback test",
    "ebist - PHY loopback test\n"
);

U_BOOT_CMD(
    noe_ewave,   3,  1,  noe_cmd_ewave,
    "NOE ewave commands",
    "noe_ewave <mode> <subitem> - select ewave test <mode> <subitem> !!\n"
    "mode[0]-10M:\n"
    "         subitem[0] - Pseudo-random pattern\n"
    "         subitem[1] - FF pattern\n"
    "         subitem[2] - 00 pattern\n"
    "         subitem[3] - Finish 10M measurement\n"
    "mode[1]-100M:\n"
    "         subitem[0] - Output MLT-3 from channel A\n"
    "         subitem[1] - Output MLT-3 from channel B\n"
    "         subitem[2] - Finish 100M measurement\n"
    "mode[2]-1000M:\n"
    "         subitem[0] - Enable test Mode 1\n"
    "         subitem[1] - Enable test Mode 4\n"
    "         subitem[2] - Finish 1000M measurement\n"
);

#endif

