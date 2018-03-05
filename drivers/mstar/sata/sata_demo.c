////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007, 2016 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
//
// File name: demoSATA.c
// Copyright: MStar Semiconductor Inc.
// Description: Full SATA Test
//
////////////////////////////////////////////////////////////////////////////////
#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <malloc.h>
#include "ubifs.h"
//#include <stdio.h>
#include "MsTypes.h"
//#include "MsOS.h"
#include "sata_drv.h"
//#include "string.h"

//#undef getchar

#define SATA_HBA_ADDRESS         (0x1F000000 + (0x102B00 << 1))
#define SATA_PORT_ADDRESS        (0x1F000000 + (0x102C00 << 1))
#define SATA_MISC_ADDRESS        (0x1F000000 + (0x102D00 << 1))

#define SATA_DATA_SRC_ADDR        0x22000000
#define SATA_DATA_DST_ADDR        0x26000000

#define LENGTH 0x100000

struct sata_mstar_port_priv sata_info;
struct hal_queued_cmd sata_queued_cmd;
void sata_demo_dump_mem(MS_U32 mem);

void dbg_cmd(struct hal_queued_cmd cmd)
{
    //hal_cmd_h2dfis fis = cmd.cmd_h2dfis;

    //printf("cmd.addr = 0x%x\n", cmd.Data_Addr);
    //printf("cmd.len = 0x%x\n", cmd.Data_Length);
    //printf("cmd.fis = 0x%x\n", fis.u8fis_type);
    //printf("cmd.ata = 0x%x\n", fis.ata_cmd);
    //printf("cmd.sector_cnt = 0x%x\n", fis.u16sector_cnt);

}

void ata_command_write_pio_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 1;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x34;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}


void ata_command_write_pio(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 1;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x30;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

void ata_command_read_pio_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x24;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}


void ata_command_read_pio(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x20;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}


int ata_command_write_dma_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    //memset((void *)&sata_queued_cmd, 0, sizeof(struct hal_queued_cmd));
    //printf("write from DRAM 0x%x\n", DramAddr);
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 1;
    sata_queued_cmd.SlotNum = 1;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x35;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    return sata_mstar_qc_wait_complete(&sata_queued_cmd);

}


void ata_command_write_dma(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 1;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0xca;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

int ata_command_read_dma_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0x25;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    return sata_mstar_qc_wait_complete(&sata_queued_cmd);
}


void ata_command_read_dma(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = Length;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0xc8;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xe0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = Length >> 9;
    sata_queued_cmd.cmd_h2dfis.lba_l = LBA & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    dbg_cmd(sata_queued_cmd);
    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

void ata_command_set_features(MS_U8 fearure, MS_U8 value)
{
    sata_queued_cmd.DataRequest = 0;
    sata_queued_cmd.Data_Addr = 0;
    sata_queued_cmd.Data_Length = 0;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0xef;
    sata_queued_cmd.cmd_h2dfis.fearure = fearure;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xa0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = (MS_U16)value;
    sata_queued_cmd.cmd_h2dfis.lba_l = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h = 0;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

void ata_command_identify(MS_U32 DramAddr)
{
    sata_queued_cmd.DataRequest = 1;
    sata_queued_cmd.Data_Addr = DramAddr;
    sata_queued_cmd.Data_Length = 512;
    sata_queued_cmd.IsWrite = 0;
    sata_queued_cmd.SlotNum = 0;

    sata_queued_cmd.cmd_h2dfis.u8fis_type = 0x27;
    sata_queued_cmd.cmd_h2dfis.u8MPM = 0;
    sata_queued_cmd.cmd_h2dfis.iscommand = 1;
    sata_queued_cmd.cmd_h2dfis.ata_cmd = 0xec;
    sata_queued_cmd.cmd_h2dfis.fearure = 0;
    sata_queued_cmd.cmd_h2dfis.control = 0x8;
    sata_queued_cmd.cmd_h2dfis.device = 0xa0;
    sata_queued_cmd.cmd_h2dfis.u16sector_cnt = 0x0;
    sata_queued_cmd.cmd_h2dfis.lba_l = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h = 0;
    sata_queued_cmd.cmd_h2dfis.lba_l_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_m_exp = 0;
    sata_queued_cmd.cmd_h2dfis.lba_h_exp = 0;

    sata_mstar_qc_prep(&sata_info, &sata_queued_cmd);

    sata_mstar_qc_issue(&sata_queued_cmd);

    sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

MS_U32 MsOS_GetSystemTime (void)
{
#if 0
#if 1  //

    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000000ULL + tv.tv_usec * 1000;

#else
    struct timespec         ts;

    getnstimeofday(&ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
#endif
    return get_timer(0);
}

void sata_demo_performance_write_pio(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1e00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 TransferNum = 10;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("PIO Write Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);

    printf("Count: ");
    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_write_pio_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}

void sata_demo_performance_read_pio(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1e00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 TransferNum = 10;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("PIO Read Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);

    printf("Count: ");
    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_read_pio_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}


void sata_demo_performance_write_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1f00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 TransferNum = 32;
    //MS_U32 TransferNum = 1;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("DMA Write Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);

    printf("Count: ");
    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_write_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
        //ata_command_write_dma((MS_U32)TestAddr, Length, LBA);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}
void sata_demo_performance_write_dma(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1e00000;
    MS_U32 LBA = 0x0;
    MS_U32 TransferNum = 10;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("DMA Write Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);

    printf("Count: ");
    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_write_dma((MS_U32)TestAddr, Length, LBA);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}

void sata_demo_performance_read_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1f00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 TransferNum = 32;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("DMA Read Ext Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);
    printf("Count: ");

    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_read_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}

void sata_demo_performance_read_dma(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = 0x1e00000;
    MS_U32 LBA = 0x0;
    MS_U32 TransferNum = 10;
    MS_U32 i;
    MS_U32 Time_1 = 0;
    MS_U32 Time_2 = 0;

    printf("DMA Read Performance Measurement : %d(%d*%d)MB\n",
           (int)((Length >> 20)*TransferNum), (int)(Length >> 20), (int)TransferNum);
    printf("Count: ");

    Time_1 = MsOS_GetSystemTime();

    for (i = 0; i < TransferNum; i++)
    {
        //printf("%d ", i);
        ata_command_read_dma((MS_U32)TestAddr, Length, LBA);
        LBA += 0xf000;
    }

    Time_2 = MsOS_GetSystemTime();

    printf("\nResult : %d MB/s, Used Time = %dms\n",
           (int)((((Length >> 10)*TransferNum)) / (Time_2 - Time_1)), (int)(Time_2 - Time_1));
}

void sata_demo_rw_pio_test(void)
{
    MS_U32 *SrcAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    MS_U32 *DstAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 i;

    printf("PIO R/W Test with %d MB\n", (int)(Length >> 20));

    printf("Prepare Data--\n");
#if 0
    for (i = 0; i < (Length >> 2); i++)
    {
        SrcAddr[i] = 0xdeadbeef;
        DstAddr[i] = 0x00;
        //SrcAddr[i] = i;
        //DstAddr[i] = 0xffeeaadd;
    }
#else
    memset(SrcAddr, 0xdeadbeef, sizeof(SrcAddr));
    printf("Clear Dst\n");
    memset(DstAddr, 0x0, sizeof(DstAddr));
#endif

    printf("\nDMA Write Data to Disk--\n");

    ata_command_write_pio_ext((MS_U32)SrcAddr, Length, LBA, LBA_EXT);

    printf("DMA Read Data from Disk--\n");

    ata_command_read_pio_ext((MS_U32)DstAddr, Length, LBA, LBA_EXT);

    printf("Compare Data--\n");

    for (i = 0; i < (Length >> 2); i++)
    {
        printf(".");
        if (DstAddr[i] != SrcAddr[i])
        {
            printf("\nCompare Error : Src[0x%x]=0x%x ; Dsr[0x%x]=0x%x\n", (int)&SrcAddr[i], (int)SrcAddr[i], (int)&DstAddr[i], (int)DstAddr[i]);
            return;
        }
    }

    printf("\nCompare Success\n");
}


void sata_demo_write_pio(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    //MS_U32 Length = 0x1000000;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    printf("PIO Write with %d MB\n", (int)(Length >> 20));

    time1 = MsOS_GetSystemTime();
    ata_command_write_pio_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();
    //printf("p %x %x\n", *TestAddr, (MS_U32)TestAddr);

    printf("PIO Write Finish, took %d ms\n", (int)(time2 - time1));
}

void sata_demo_read_pio(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    //MS_U32 Length = 0x1000000;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    printf("PIO Read with %d MB\n", (int)(Length >> 20));

    time1 = MsOS_GetSystemTime();
    ata_command_read_pio_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();

    printf("PIO Read Finish, took %d ms\n", (int)(time2 - time1));
}

void sata_demo_rw_dma_test(void)
{
    MS_U32 *SrcAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    MS_U32 *DstAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U32 Length = LENGTH;
    //MS_U32 Length = 0x1000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 i;

    printf("DMA R/W Test with %d MB\n", (int)(Length >> 20));

    printf("Prepare Data--");

#if 0
    for (i = 0; i < (Length >> 2); i++)
    {
        SrcAddr[i] = 0xdeadbeef;
        printf(".");
        //printf("src[%d] = 0x%x\n", i, SrcAddr[i]);
        //SrcAddr[i] = i;
        //DstAddr[i] = 0xaabbccdd;
    }
#else
    memset(SrcAddr, 0xdeadc0ee, sizeof(SrcAddr));
    printf("Clear Dst\n");
    memset(DstAddr, 0x0, sizeof(DstAddr));

#endif
    printf("Done\n");

    printf("DMA Write Data to Disk--\n");

    if (ata_command_write_dma_ext((MS_U32)SrcAddr, Length, LBA, LBA_EXT) == -1)
    {
        printf("Failed to write.\n");
        return;
    }

    printf("DMA Read Data from Disk--\n");
    memset(DstAddr, 0, Length);

    if (ata_command_read_dma_ext((MS_U32)DstAddr, Length, LBA, LBA_EXT) == -1)
    {
        printf("Failed to read.\n");
        return;
    }

    printf("Compare Data--\n");

    for (i = 0; i < (Length >> 2); i++)
    {
        //printf(".");
        if (DstAddr[i] != SrcAddr[i])
        {
            printf("\nCompare Error : Src[0x%x]=0x%x ; Dsr[0x%x]=0x%x\n",
                   (int)&SrcAddr[i], (int)SrcAddr[i], (int)&DstAddr[i], (int)DstAddr[i]);
            return;
        }
    }

    printf("\nCompare Success\n");
}

void sata_demo_write_dma(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    //MS_U32 TestAddr = (MS_U32)SATA_DATA_SRC_ADDR;
    //MS_U32 Length = 0x00001000;
    //MS_U32 Length = 0x1000000;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 time1, time2;

    //printf("DMA Write with %d MB\n", Length >> 20);
    printf("DMA Read with %d MB or %d bytes\n", (int)(Length >> 20), (int)Length);

    time1 = MsOS_GetSystemTime();
    ata_command_write_dma((MS_U32)TestAddr, Length, LBA);
    time2 = MsOS_GetSystemTime();
    //ata_command_write_dma(SATA_DATA_SRC_ADDR, Length, LBA);

    printf("DMA Write Finish, took %d ms\n", (int)(time2 - time1));
}

void sata_demo_write_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    //MS_U32 Length = 0x00001000; // 4k
    //MS_U32 Length = 0x1000000;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    //printf("DMA Write ext with %d MB\n", Length >> 20);
    printf("DMA Read with %d MB or %d bytes\n", (int)(Length >> 20), (int)(Length));

    time1 = MsOS_GetSystemTime();
    ata_command_write_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();

    printf("DMA Write ext Finish, took %d ms\n", (int)(time2 - time1));
}

void sata_demo_read_dma(void)
{
    //MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    //MS_U32 Length = 0x1000000;
    //MS_U32 Length = 0x00001000; // 4k
    //MS_U32 Length = 0x1000000;
    MS_U32 Length = LENGTH;
    MS_U32 LBA = 0x0;
    MS_U32 time1, time2;

    printf("DMA Read with %d MB or %d bytes\n", (int)(Length >> 20), (int)Length);

    //ata_command_read_dma((MS_U32)TestAddr, Length, LBA);
    time1 = MsOS_GetSystemTime();
    ata_command_read_dma(SATA_DATA_DST_ADDR, Length, LBA);
    time2 = MsOS_GetSystemTime();
    //sata_demo_dump_mem(SATA_DATA_DST_ADDR);
    printf("DMA Read Finish, took %d ms\n", (int)(time2 - time1));
}

void sata_demo_read_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    //MS_U32 Length = 0x00100000;
    MS_U32 Length = LENGTH;
    //MS_U32 Length = 0x00001000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    //printf("DMA Read ext with %d MB\n", Length >> 20);
    printf("DMA Read with %d MB or %d bytes\n", (int)(Length >> 20), (int)Length);

    time1 = MsOS_GetSystemTime();
    ata_command_read_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();

    printf("DMA Read ext Finish, took %d ms\n", (int)(time2 - time1));
}

#if 1
int multi_slot_write_dma_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    int i;
    struct hal_queued_cmd multi_cmd[32];

    for (i = 0; i < 32; i++)
    {
        LBA += 0xF000;
        LBA_EXT += 0xF000;

        multi_cmd[i].DataRequest = 1;
        multi_cmd[i].Data_Addr = DramAddr;
        multi_cmd[i].Data_Length = Length;
        multi_cmd[i].IsWrite = 1;
        multi_cmd[i].SlotNum = i;

        multi_cmd[i].cmd_h2dfis.u8fis_type = 0x27;
        multi_cmd[i].cmd_h2dfis.u8MPM = 0;
        multi_cmd[i].cmd_h2dfis.iscommand = 1;
        multi_cmd[i].cmd_h2dfis.ata_cmd = 0x35;
        //multi_cmd[i].cmd_h2dfis.ata_cmd = 0x61;
        multi_cmd[i].cmd_h2dfis.fearure = 0;
        multi_cmd[i].cmd_h2dfis.control = 0x8;
        multi_cmd[i].cmd_h2dfis.device = 0xe0;
        multi_cmd[i].cmd_h2dfis.u16sector_cnt = Length >> 9;
        multi_cmd[i].cmd_h2dfis.lba_l = LBA & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

        //dbg_cmd(sata_queued_cmd);
        sata_mstar_qc_prep(&sata_info, &multi_cmd[i]);

    }
    //sata_mstar_qc_issue(&sata_queued_cmd);
    //sata_reg_write32(0xFFFFFFFF, PORT_CMD_ISSUE + port_base);
    sata_mstar_qc_multi_issue(0xFFFFFFFF);
    sata_mstar_qc_wait_complete_multi();



    //return sata_mstar_qc_wait_complete(&sata_queued_cmd);

}
#endif
void sata_multi_write_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    MS_U32 Length = 0x1f00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    //printf("DMA Write ext with %d MB\n", Length >> 20);
    printf("multi DMA write ext with %d MB or %d bytes\n", (int)((Length >> 20) * 32), (int)Length);

    time1 = MsOS_GetSystemTime();
    multi_slot_write_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();

    printf("DMA Write ext Finish, took %d ms, average = %d MS/s\n", (int)(time2 - time1), (int)(((Length >> 10) * 32) / (time2 - time1)));


}

int multi_slot_read_dma_ext(MS_U32 DramAddr, MS_U32 Length, MS_U32 LBA, MS_U32 LBA_EXT)
{
    int i;
    struct hal_queued_cmd multi_cmd[32];

    for (i = 0; i < 32; i++)
    {
        LBA += 0xF000;
        LBA_EXT += 0xF000;

        multi_cmd[i].DataRequest = 1;
        multi_cmd[i].Data_Addr = DramAddr;
        multi_cmd[i].Data_Length = Length;
        multi_cmd[i].IsWrite = 0;
        multi_cmd[i].SlotNum = i;

        multi_cmd[i].cmd_h2dfis.u8fis_type = 0x27;
        multi_cmd[i].cmd_h2dfis.u8MPM = 0;
        multi_cmd[i].cmd_h2dfis.iscommand = 1;
        multi_cmd[i].cmd_h2dfis.ata_cmd = 0x25;
        multi_cmd[i].cmd_h2dfis.fearure = 0;
        multi_cmd[i].cmd_h2dfis.control = 0x8;
        multi_cmd[i].cmd_h2dfis.device = 0xe0;
        multi_cmd[i].cmd_h2dfis.u16sector_cnt = Length >> 9;
        multi_cmd[i].cmd_h2dfis.lba_l = LBA & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_m = (LBA >> 8) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_h = (LBA >> 16) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_l_exp = LBA_EXT & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_m_exp = (LBA_EXT >> 8) & 0xff;
        multi_cmd[i].cmd_h2dfis.lba_h_exp = (LBA_EXT >> 16) & 0xff;

        //dbg_cmd(sata_queued_cmd);
        sata_mstar_qc_prep(&sata_info, &multi_cmd[i]);

    }
    //sata_mstar_qc_issue(&sata_queued_cmd);
    //sata_reg_write32(0xFFFFFFFF, PORT_CMD_ISSUE + port_base);
    sata_mstar_qc_multi_issue(0xFFFFFFFF);
    sata_mstar_qc_wait_complete_multi();



    //return sata_mstar_qc_wait_complete(&sata_queued_cmd);

}

void sata_multi_read_dma_ext(void)
{
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    MS_U32 Length = 0x1f00000;
    MS_U32 LBA = 0x0;
    MS_U32 LBA_EXT = 0x0;
    MS_U32 time1, time2;

    //printf("DMA Write ext with %d MB\n", Length >> 20);
    printf("multi DMA read ext with %d MB or %d bytes\n", (int)((Length >> 20) * 32), (int)Length);

    time1 = MsOS_GetSystemTime();
    multi_slot_read_dma_ext((MS_U32)TestAddr, Length, LBA, LBA_EXT);
    time2 = MsOS_GetSystemTime();

    printf("DMA read ext Finish, took %d ms, average = %d MS/s\n", (int)(time2 - time1), (int)(((Length >> 10) * 32) / (time2 - time1)));

}

void sata_demo_set_udma_mode(void)
{
    MS_U8 u8Mode = 0;

    printf("Please Select UDMA Mode (0~6)\n");
    u8Mode = (MS_U8)getc();

    switch (u8Mode)
    {
        case '0':
            u8Mode = 0;
            break;
        case '1':
            u8Mode = 1;
            break;
        case '2':
            u8Mode = 2;
            break;
        case '3':
            u8Mode = 3;
            break;
        case '4':
            u8Mode = 4;
            break;
        case '5':
            u8Mode = 5;
            break;
        case '6':
            u8Mode = 6;
            break;
        default:
            printf("UDMA Mode select incorrect\n");
            return;
    }

    if ((1 << u8Mode) & (sata_info.PortFlags & ATA_PFLAG_UDMA_MASK))
    {
        u8Mode = u8Mode | 0x40;
        ata_command_set_features(0x3, u8Mode);
    }
    else
    {
        printf("UDMA Mode %d not support\n", u8Mode);
    }
}

void sata_demo_read_capability(void)
{
    //MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_SRC_ADDR;
    MS_U32 *TestAddr = (MS_U32 *)SATA_DATA_DST_ADDR;
    MS_U16 *u16Ptr = (MS_U16 *)TestAddr;
    MS_U8 *u8Ptr = (MS_U8 *)TestAddr;
    unsigned long long SectorSize = 0;
    int i;

    ata_command_identify((MS_U32)TestAddr);

    printf("\nModel Number : ");
    for (i = 0; i < 40; i += 2)
    {
        printf("%c%c", u8Ptr[54 + i + 1], u8Ptr[54 + i]);
    }
    printf("\n");

    printf("Serial Number : ");
    for (i = 0; i < 20; i += 2)
    {
        printf("%c%c", u8Ptr[20 + i + 1], u8Ptr[20 + i]);
    }
    printf("\n");

    printf("Firmware Revision : ");
    for (i = 0; i < 8; i += 2)
    {
        printf("%c%c", u8Ptr[46 + i + 1], u8Ptr[46 + i]);
    }
    printf("\n");


    printf("Address Support : ");
    if (u16Ptr[83] & (1 << 10))
    {
        printf("48-Bit\n");
        sata_info.PortFlags |= ATA_PFLAG_LBA_EXT;
    }
    else
    {
        printf("24-Bit\n");
        sata_info.PortFlags &= ~ATA_PFLAG_LBA_EXT;
    }

    printf("Queue depth = %d\n", u16Ptr[75] & 0x000F);


    SectorSize = u16Ptr[100] + (u16Ptr[101] << 16) + ((unsigned long long)u16Ptr[102] << 32) + ((unsigned long long)u16Ptr[103] << 48);
    printf("Disk Size : %llu 512-byte logical blocks (%llu GiB)\n", SectorSize, (SectorSize << 9) >> 30);

    printf("UDMA Mode Support : ");
    if (u16Ptr[88] & (1 << 0))
    {
        printf("UDMA0 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_0;
    }
    if (u16Ptr[88] & (1 << 1))
    {
        printf("UDMA1 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_1;
    }
    if (u16Ptr[88] & (1 << 2))
    {
        printf("UDMA2 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_2;
    }
    if (u16Ptr[88] & (1 << 3))
    {
        printf("UDMA3 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_3;
    }
    if (u16Ptr[88] & (1 << 4))
    {
        printf("UDMA4 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_4;
    }
    if (u16Ptr[88] & (1 << 5))
    {
        printf("UDMA5 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_5;
    }
    if (u16Ptr[88] & (1 << 6))
    {
        printf("UDMA6 ");
        sata_info.PortFlags |= ATA_PFLAG_UDMA_6;
    }
    printf("\n");

    printf("UDMA Mode Selected : ");
    if (u16Ptr[88] & (1 << 8))
    {
        printf("UDMA0 ");
    }
    else if (u16Ptr[88] & (1 << 9))
    {
        printf("UDMA1 ");
    }
    else if (u16Ptr[88] & (1 << 10))
    {
        printf("UDMA2 ");
    }
    else if (u16Ptr[88] & (1 << 11))
    {
        printf("UDMA3 ");
    }
    else if (u16Ptr[88] & (1 << 12))
    {
        printf("UDMA4 ");
    }
    else if (u16Ptr[88] & (1 << 13))
    {
        printf("UDMA5 ");
    }
    else if (u16Ptr[88] & (1 << 14))
    {
        printf("UDMA6 ");
    }
    else
    {
        printf("not selected");
    }
    printf("\n");

}
void sata_x32_wr_test(void)
{
    MS_U32 * x32_mem = (MS_U32 *)SATA_SDMAP_RIU_BASE;
    MS_U32 * pmem = x32_mem;
    MS_U8 i;
    MS_U8 selection;

    printf("X32_SATA_MAC: 0x%x\n", SATA_SDMAP_RIU_BASE);
    printf("0 for write all Zeros, 1 for incrementing byte: ");
    selection = getc();
    switch (selection)
    {
        case '0':
            memset(pmem, 0x0, 0xff);
            break;
        case '1':
            for (i = 0; i < 0xff; i ++)
            {
                *pmem++ = i;
            }
            break;
        default:
            printf("invalid option.\n");
            return;
            break;
    }
    printf("Dump %x\n", SATA_SDMAP_RIU_BASE);
    sata_demo_dump_mem(SATA_SDMAP_RIU_BASE);
}

void sata_demo_dump_mem(MS_U32 mem)
{
    MS_U8 *DstAddr = (MS_U8 *)mem;
    MS_U32 i;

    printf("\t===============================================\n");
    printf("\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    printf("\t-----------------------------------------------\n");
    for (i = 0; i < 0x100; i += 16)
    {
        printf("%02x\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
               i,
               DstAddr[i],
               DstAddr[i + 1],
               DstAddr[i + 2],
               DstAddr[i + 3],
               DstAddr[i + 4],
               DstAddr[i + 5],
               DstAddr[i + 6],
               DstAddr[i + 7],
               DstAddr[i + 8],
               DstAddr[i + 9],
               DstAddr[i + 10],
               DstAddr[i + 11],
               DstAddr[i + 12],
               DstAddr[i + 13],
               DstAddr[i + 14],
               DstAddr[i + 15]
              );
    }
    printf("\t===============================================\n");
}

void sata_demo_clr_mem(MS_U32 mem)
{
    MS_U32 * dst = (MS_U32 *)mem;
    memset(dst, 0, 0x100000);
}

void sata_demo(void)
{
    MS_U8 u8TestItem = 0;
    unsigned int DeviceClass = 0;

    //while (1);
    printf("3\n");
    mstar_sata_set_base(SATA_HBA_ADDRESS, SATA_PORT_ADDRESS, SATA_MISC_ADDRESS);
    printf("hba base = 0x%x, port base 0x%x,misc base= %x\n", SATA_HBA_ADDRESS, SATA_PORT_ADDRESS, SATA_MISC_ADDRESS);
    printf("4\n");

#if 1
    mstar_sata_hardware_init();
#endif
    printf("5\n");
#if 1
    sata_mstar_port_start(&sata_info);
#endif
    printf("6\n");

    DeviceClass = sata_mstar_hardreset();
#if 1
    if (DeviceClass == ATA_DEV_UNKNOWN)
    {
        printf("unknown device, stay in demo\n");
        //return;
    }
#endif

    printf("7\n");
#if 1
    sata_mstar_show_speed();
#endif
    printf("8\n");

    //sata_mstar_interrupt_thaw();

    while(1)
    {
        printf("\n********** SATA Test - type " );
        if (SATA_CMD_TYPE == TYPE_DRAM)
            printf("DRAM *********\n");
        else if (SATA_CMD_TYPE == TYPE_XIU)
            printf("XIU *********\n");
        else
            printf("RIU *********\n");

        printf("0.Read Capability\n");
        printf("1.Setup UDMA Mode\n");
        printf("/**********************/\n");
        printf("2.Disk DMA Read Ext\n");
        printf("3.Disk DMA Write Ext\n");
        printf("4.Disk DMA R/W Test\n");
        printf("5.Disk PIO ext Read\n");
        printf("6.Disk PIO ext Write\n");
        printf("7.Disk PIO R/W Test\n");
        printf("8.Disk DMA read\n");
        printf("9.Disk DMA write\n");
        printf("/**********************/\n");
        printf("a.Performance Measurment : Read DMA ext\n");
        printf("b.Performance Measurment : Write DMA ext\n");
        printf("c.Performance Measurment : Read PIO ext\n");
        printf("d.Performance Measurment : Write PIO ext\n");
        printf("e.Performance Measurment : Read DMA \n");
        printf("f.Performance Measurment : Write DMA\n");
        printf("/*******************************/\n");
        printf("p.Print 0x100 of Src addr content\n");
        printf("P.Print 0x100 of Dst addr content\n");
        printf("r.Perform Hard Reset\n");
        printf("x.Dump Misc registers\n");
        printf("z.Dump Port registers\n");
        printf("Z.Dump GHC registers\n");
        printf("m.clear SRC mem\n");
        printf("M.clear DST mem\n");
        printf("t.write X32_sata_mac\n");
        printf("C.perform large data write/read\n");

        u8TestItem = getc();

        printf("\e[1;1H\e[2J");
        switch (u8TestItem)
        {
            case '0':
                sata_demo_read_capability();
                break;
            case '1':
                sata_demo_set_udma_mode();
                break;
            case '2':
                sata_demo_read_dma_ext();
                break;
            case '3':
                sata_demo_write_dma_ext();
                break;
            case '4':
                sata_demo_rw_dma_test();
                break;
            case '5':
                sata_demo_read_pio();
                break;
            case '6':
                sata_demo_write_pio();
                break;
            case '7':
                sata_demo_rw_pio_test();
                break;
            case '8':
                sata_demo_read_dma();
                break;
            case '9':
                sata_demo_write_dma();
                break;
            case 'a':
                sata_demo_performance_read_dma_ext();
                break;
            case 'b':
                sata_demo_performance_write_dma_ext();
                break;
            case 'c':
                sata_demo_performance_read_pio();
                break;
            case 'C':
                break;
            case 'd':
                sata_demo_performance_write_pio();
                break;
            case 'e':
                sata_demo_performance_read_dma();
                break;
            case 'f':
                sata_demo_performance_write_dma();
                break;
            case 'p':
                printf("Dumping SRC (0x%x):\n", SATA_DATA_SRC_ADDR);
                sata_demo_dump_mem(SATA_DATA_SRC_ADDR);
                break;
            case 'P':
                printf("Dumping DST (0x%x):\n", SATA_DATA_DST_ADDR);
                sata_demo_dump_mem(SATA_DATA_DST_ADDR);
                break;
            case 'z':
                printf("Dumping Port registers ");
                sata_mstar_dump_full_reg(SATA_PORT_ADDRESS);
                break;
            case 'Z':
                printf("Dumping GHC registers ");
                sata_mstar_dump_full_reg(SATA_HBA_ADDRESS);
                break;
            case 'x':
                printf("Dumping MISC registers ");
                sata_mstar_dump_full_reg(SATA_MISC_ADDRESS);
                break;
            case 'r':
                sata_mstar_hardreset();
                break;
            case 'm':
                printf("Clearing SRC memory, please wait...\n");
                sata_demo_clr_mem(SATA_DATA_SRC_ADDR);
                break;
            case 'M':
                printf("Clearing DST memory, please wait...\n");
                sata_demo_clr_mem(SATA_DATA_DST_ADDR);
                break;
            case 'q':
                return;
                break;
            case 't':
                sata_x32_wr_test();
                break;
            case 'n':
                sata_multi_write_dma_ext();
                break;
            case 'N':
                sata_multi_read_dma_ext();
                break;
            default:
                printf("Test Item Select Incorrect\n");
        }
    }
}
