////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
#include <command.h>
#include <asm/io.h>
#include <asm/types.h>
#include <malloc.h>
#include <MsDebug.h>
//#include <stdio.h>
#include "MsTypes.h"
//#include "MsOS.h"
#include "sata_drv.h"
//#include <string.h>


#define FPGA 0

//#define SATA_DEBUG
#ifdef SATA_DEBUG
    #define sata_debug(fmt, args...) printf("[%s] " fmt, __FUNCTION__, ##args)
#else
    #define sata_debug(fmt, args...) do {} while(0)
#endif

static MS_U32 hba_base;
static MS_U32 port_base;
static MS_U32 misc_base;

#define sata_reg_write8(val, addr) { (*((volatile MS_U8*)(addr))) = (MS_U8)(val);}
#define sata_reg_read8(addr) (*(volatile MS_U8*)(addr))

#define sata_reg_write16(val, addr) { (*((volatile MS_U16*)(addr))) = (MS_U16)(val); }
#define sata_reg_read16(addr) (*(volatile MS_U16*)(addr))

static void print_prdt(struct prdte * pt, MS_U8 num);
static void print_cmd_header(hal_cmd_header *p);
static void sata_reg_write32(MS_U32 data, MS_U32 reg_addr)
{
    sata_reg_write16(data & 0xFFFF, reg_addr);
    sata_reg_write16((data >> 16) & 0xFFFF, (reg_addr + 0x04));
}

static MS_U32 sata_reg_read32(MS_U32 reg_addr)
{
    MS_U32 data;
    data = (sata_reg_read16(reg_addr + 0x04) << 16) + sata_reg_read16(reg_addr);
    return data;
}

void MsOS_DelayTask (MS_U32 u32Ms)
{
    //sleep in spinlock will cause deadlock
#ifdef MSOS_MUTEX_USE_SEM
    msleep_interruptible((unsigned int)u32Ms);
#else
    mdelay(u32Ms);
#endif
}

static MS_U32 mstar_sata_wait_reg(MS_U32 reg_addr, MS_U32 mask, MS_U32 val, unsigned long interval, unsigned long timeout)
{
    MS_U32 temp;
    unsigned long timeout_vale = 0;

    temp = sata_reg_read32(reg_addr);

    while((temp & mask) == val)
    {
        MsOS_DelayTask(interval);
        timeout_vale += interval;
        if (timeout_vale > timeout)
            break;
        temp = sata_reg_read32(reg_addr);
    }

    return temp;
}

static int mstar_ahci_stop_engine(void)
{
    MS_U32 temp;

    //printf("begin %s\n", __func__);
    temp = sata_reg_read32(PORT_CMD + port_base);

    /* check if the HBA is idle */
    if ((temp & (PORT_CMD_START | PORT_CMD_LIST_ON)) == 0)
        return 0;

    /* setting HBA to idle */
    temp &= ~PORT_CMD_START;
    sata_reg_write32(temp, PORT_CMD + port_base);

    temp = mstar_sata_wait_reg(PORT_CMD + port_base, PORT_CMD_LIST_ON, PORT_CMD_LIST_ON, 1, 500);

    if (temp & PORT_CMD_LIST_ON)
        return -1;

    //printf("done%s\n", __func__);
    return 0;
}

static void mstar_ahci_start_engine(void)
{
    MS_U32 temp;

    //printf("begin %s\n", __func__);

    /* Start Port DMA */
    temp = sata_reg_read32(PORT_CMD + port_base);
    temp |= PORT_CMD_START;
    sata_reg_write32(temp, PORT_CMD + port_base);
    sata_reg_read32(PORT_CMD + port_base); /* Flush */
    //printf("done %s\n", __func__);
}

static void mstar_ahci_start_fis_rx(struct sata_mstar_port_priv *pp)
{
    MS_U32 tmp;

    // set Port registers
    // CLBA - Command List Base Address
    tmp = pp->cmd_slot_dma;
    sata_reg_write32(tmp, PORT_LST_ADDR + port_base);

    // rx Fis Base address
    tmp = pp->rx_fis_dma;
    sata_reg_write32(tmp, PORT_FIS_ADDR + port_base);

    // enable FIS reception
    tmp = sata_reg_read32(PORT_CMD + port_base);
    tmp |= PORT_CMD_FIS_RX;
    sata_reg_write32(tmp, PORT_CMD + port_base);

    // flush
    tmp = sata_reg_read32(PORT_CMD + port_base);
}

static int mstar_ahci_stop_fis_rx(void)
{
    MS_U32 tmp;

    // Disable FIS reception
    tmp = sata_reg_read32(PORT_CMD + port_base);
    tmp &= ~PORT_CMD_FIS_RX;
    sata_reg_write32(tmp, PORT_CMD + port_base);

    // Wait FIS reception Stop for 1000ms
    tmp = mstar_sata_wait_reg(PORT_CMD + port_base, PORT_CMD_FIS_ON, PORT_CMD_FIS_ON, 10, 1000);

    if (tmp & PORT_CMD_FIS_ON)
        return -1;

    return 0;
}

static void build_cmd_prdt(void *base_address, MS_U32 *pprdt, MS_U8 prdt_num)
{
    // 0x1800 1280
    //printf("%s\n", __func__);
#if (SATA_CMD_TYPE != TYPE_RIU)
    void *cmd_address = (void *)((MS_U32)base_address + SATA_KA9_CMD_DESC_OFFSET_TO_PRDT);

#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x00, SATA_MISC_ACCESS_MODE + misc_base);
#endif

    memcpy(cmd_address, pprdt, sizeof(MS_U32)*prdt_num * 4);
    //print_prdt(cmd_address, prdt_num);

#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x01, SATA_MISC_ACCESS_MODE + misc_base);
#endif
#else
    MS_U32 address = (MS_U32)base_address + SATA_KA9_CMD_DESC_OFFSET_TO_PRDT;
    MS_U32 * dptr = pprdt;
    MS_U8 offset;

    for (offset = 0; offset < sizeof(MS_U32) * prdt_num * 4; offset += 4)
    {
        // which address to write?
        sata_reg_write32(address, misc_base + SATA_MISC_CFIFO_ADDRL);
        //printf("write data 0x%8.8x to addr 0x%x\n", address, misc_base + SATA_MISC_CFIFO_ADDRL);

        // what data to write?
        sata_reg_write32(*dptr, misc_base + SATA_MISC_CFIFO_WDATAL);
        //printf("write data 0x%8.8x to addr 0x%x\n", *dptr, misc_base + SATA_MISC_CFIFO_WDATAL);

        // read(0) or write(1)? normally write
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_RORW);
        //printf("write data 0x0001 to addr 0x%8.8x\n", misc_base + SATA_MISC_CFIFO_RORW);

        // trigger
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_ACCESS);
        //printf("write data 0x0001 to addr 0x%x\n", misc_base + SATA_MISC_CFIFO_ACCESS);

        address += 4;
        dptr++;
    }

#endif
}

static void build_cmd_header(void *cmd_slot, MS_U32 u32offset_address, MS_U32 *pcmdheader)
{
    // 0x21000000 or 0x18001000
    //printf("%s\n", __func__);
#if (SATA_CMD_TYPE != TYPE_RIU)
    void *cmd_address = (void *)((MS_U32)cmd_slot + u32offset_address);

#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x00, SATA_MISC_ACCESS_MODE + misc_base);
#endif

    memcpy(cmd_address, pcmdheader, SATA_KA9_CMD_HDR_SIZE);
    //print_cmd_header(cmd_address);

#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x01, SATA_MISC_ACCESS_MODE + misc_base);
#endif
#else
    MS_U32 address = (MS_U32)cmd_slot + u32offset_address;
    MS_U32 * dptr = pcmdheader;
    MS_U8 offset;

    for (offset = 0; offset < SATA_KA9_CMD_HDR_SIZE; offset += 4)
    {
        // which address to write?
        sata_reg_write32(address, misc_base + SATA_MISC_CFIFO_ADDRL);
        //printf("write data 0x%8.8x to addr 0x%x\n", address, misc_base + SATA_MISC_CFIFO_ADDRL);

        // what data to write?
        sata_reg_write32(*dptr, misc_base + SATA_MISC_CFIFO_WDATAL);
        //printf("write data 0x%8.8x to addr 0x%x\n", *dptr, misc_base + SATA_MISC_CFIFO_WDATAL);

        // read(0) or write(1)? normally write
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_RORW);
        //printf("write data 0x0001 to addr 0x%x\n", misc_base + SATA_MISC_CFIFO_RORW);

        // trigger
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_ACCESS);
        //printf("write data 0x0001 to addr 0x%x\n", misc_base + SATA_MISC_CFIFO_ACCESS);

        address += 4;
        dptr++;
    }
#endif
}

static void print_fis(hal_cmd_h2dfis *pfis)
{
    hal_cmd_h2dfis * p = pfis;

    printf("fis @ 0x%8.8x, fis size = 0x%x\n", (int)pfis, sizeof(hal_cmd_h2dfis));
    printf("fis type = 0x%x\n", p->u8fis_type);
    printf("PMP:4\t = 0x%x\n", p->u8MPM);
    printf("rev:3\t = 0x%x\n", p->reserved_0);
    printf("iscmd:1\t = 0x%x\n", p->iscommand);
    printf("ata cmd\t = 0x%x\n", p->ata_cmd);
    printf("feat.\t = 0x%x\n", p->fearure);
    printf("lba_l\t = 0x%x\n", p->lba_l);
    printf("lba_m\t = 0x%x\n", p->lba_m);
    printf("lba_h\t = 0x%x\n", p->lba_h);
    printf("dev\t = 0x%x\n", p->device);
    printf("lba_l_e\t = 0x%x\n", p->lba_l_exp);
    printf("lba_m_e\t = 0x%x\n", p->lba_m_exp);
    printf("lba_h_e\t = 0x%x\n", p->lba_h_exp);
    printf("feat. ex = 0x%x\n", p->fearure_exp);
    printf("sector\t = 0x%x (u16)\n", p->u16sector_cnt);
    printf("rev\t = 0x%x\n", p->reserved_1);
    printf("ctrl\t = 0x%x\n", p->control);
    printf("rev \t = 0x%x (u32)\n", (int)p->reserved_2);
}

static void print_prdt(struct prdte * pt, MS_U8 num)
{
    struct prdte * p = pt;
    MS_U8 i;
    printf("prdt @ 0x%8.8x, prdte size = 0x%x, total = 0x%x\n", (int)pt, sizeof(struct prdte), sizeof(struct prdte) * num);

    for (i = 0; i < num; i++, p++)
    {
        printf("dba\t = 0x%x (u32)\n", (int)p->dba);
        printf("dbau\t = 0x%x (u32)\n", (int)p->dbau);
        printf("rev1\t = 0x%x (u32)\n", (int)p->rev1);
        printf("dbc\t = 0x%x (:22)\n", (int)p->dbc);
        printf("rev2\t = 0x%x (:9)\n", (int)p->rev2);
        printf("i\t = 0x%x (:1)\n", (int)p->i);
    }
}

static void print_cmd_header(hal_cmd_header *p)
{
    printf("command header @ 0x%8.8x, cmd header size = 0x%x\n", (int)p, sizeof(hal_cmd_header));

    printf("cmd fis len\t = 0x%x (:5)\n", p->cmd_fis_len);
    printf("isATAPI\t = 0x%x (:1)\n", p->isATA_PI);
    printf("isWrite\t = 0x%x (:1)\n", p->iswrite);
    printf("isprefetch\t = 0x%x (:1)\n", p->isprefetch);
    printf("isswrst\t = 0x%x (:1)\n", p->issoftreset);
    printf("isbist\t = 0x%x (:1)\n", p->isbist);
    printf("isclrok\t = 0x%x (:1)\n", p->isclearok);
    printf("rev1\t = 0x%x (:1)\n", p->reserverd);
    printf("PMPid\t = 0x%x (:4)\n", p->PMPid);
    printf("PRDlen\t = 0x%x (16)\n", p->PRDTlength);
    printf("PRDbyte\t = 0x%x (32)\n", (int)p->PRDBytes);
    printf("ctbal\t = 0x%x (32)\n", (int)p->ctba_lbase);
    printf("ctbah\t = 0x%x (32)\n", (int)p->ctba_hbase);
    printf("resv1\t = 0x%x (32)\n", (int)p->resv[0]);
    printf("resv2\t = 0x%x (32)\n", (int)p->resv[1]);
    printf("resv3\t = 0x%x (32)\n", (int)p->resv[2]);
    printf("resv4\t = 0x%x (32)\n", (int)p->resv[3]);
}
static void build_cmd_fis(void *cmd_tbl, hal_cmd_h2dfis *pfis)
{
    //printf("%s\n", __func__);
    // 0x18001200
#if (SATA_CMD_TYPE != TYPE_RIU)
#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x00, SATA_MISC_ACCESS_MODE + misc_base);
#endif

    memcpy(cmd_tbl, pfis, sizeof(hal_cmd_h2dfis));
    //print_fis((hal_cmd_h2dfis *)cmd_tbl);

#if (SATA_CMD_TYPE == TYPE_XIU)
    sata_reg_write16(0x01, SATA_MISC_ACCESS_MODE + misc_base);
#endif
#else // RIU mode
    MS_U32 address = (MS_U32)cmd_tbl;
    MS_U32 * dptr = (MS_U32 *)pfis;
    MS_U8 offset;

    address = (MS_U32)cmd_tbl;

    // loop through data
    for (offset = 0; offset < sizeof(hal_cmd_h2dfis); offset += 4)
    {
        // which address to write?
        sata_reg_write32(address, misc_base + SATA_MISC_CFIFO_ADDRL);
        //printf("write data 0x%8.8x to addr 0x%x\n", address, misc_base + SATA_MISC_CFIFO_ADDRL);

        // what data to write?
        sata_reg_write32(*dptr, misc_base + SATA_MISC_CFIFO_WDATAL);
        //printf("write data 0x%8.8x to addr 0x%x\n", *dptr, misc_base + SATA_MISC_CFIFO_WDATAL);

        // read(0) or write(1)? normally write
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_RORW);
        //printf("write data 0x0001 to addr 0x%x\n", misc_base + SATA_MISC_CFIFO_RORW);

        // trigger
        sata_reg_write16(0x01, misc_base + SATA_MISC_CFIFO_ACCESS);
        //printf("write data 0x0001 to addr 0x%x\n", misc_base + SATA_MISC_CFIFO_ACCESS);

        address += 4;
        dptr++;
    }
#endif
}

static void sata_mstar_bist_enable(void)
{
    // TODO: .... you know.
}

static void sata_mstar_setup_cmd_hdr_entry(struct sata_mstar_port_priv *pp,
        struct hal_queued_cmd *qc, MS_U8 num_prde)
{
    MS_U32 cmd_descriptor_address;
    hal_cmd_header cmd_header;
    void *cmd_slot = pp->cmd_slot;

    cmd_descriptor_address = pp->cmd_tbl_dma + (qc->SlotNum * SATA_KA9_CMD_DESC_SIZE);
    memset(&cmd_header, 0x0, sizeof(hal_cmd_header));

    cmd_header.isATA_PI = 0;
    cmd_header.isbist = 0;
    cmd_header.isprefetch = 0;
    cmd_header.issoftreset = 0;
    cmd_header.PMPid = 0;
    cmd_header.reserverd = 0;

    cmd_header.cmd_fis_len = 5;  // unit is DW, i.e. H2D FIS is 5 * 4 = 20 bytes
    cmd_header.PRDTlength = num_prde;
    cmd_header.isclearok = 0;
    cmd_header.PRDBytes = qc->Data_Length;
    cmd_header.ctba_hbase = 0;
    cmd_header.ctba_lbase = cmd_descriptor_address;

    if (qc->IsWrite)
        cmd_header.iswrite = 1;
    else
        cmd_header.iswrite = 0;

    build_cmd_header(cmd_slot, qc->SlotNum * SATA_KA9_CMD_HDR_SIZE, (MS_U32 *)&cmd_header);
}

static MS_U8 sata_mstar_fill_sg(struct hal_queued_cmd *qc, void *cmd_tbl)
{
    MS_U32 prdt[SATA_KA9_USED_PRD * 4] = {0};
    MS_U8 PrdtIndex;
    MS_U32 Data_Addr = qc->Data_Addr;
    MS_U32 Data_Length = qc->Data_Length;

    for (PrdtIndex = 0; PrdtIndex < SATA_KA9_USED_PRD; PrdtIndex++)
    {

        sata_debug("[%d]-addr = 0x%x ; remain_length = 0x%x\n", PrdtIndex, Data_Addr, Data_Length);

        if ((PrdtIndex == (SATA_KA9_USED_PRD - 1)) && (Data_Length > SATA_PRDT_DATA_LENGTH_UNIT))
        {
            printf("[]-Error - setting indirect prde , prdt not enough \n");
            return -1;
        }

        // PRD DMA base address
        prdt[PrdtIndex * 4 + 0] = Data_Addr - 0x20000000;
        // PRD DMA base upper, for 64bits address.
        prdt[PrdtIndex * 4 + 1] = 0;
        // PRD reserved field. Fill with 0xFFFFFFFF or 0x00000000?
        //prdt[PrdtIndex*4 + 2] = 0xFFFFFFFF;
        prdt[PrdtIndex * 4 + 2] = 0x0;
        if (Data_Length > SATA_PRDT_DATA_LENGTH_UNIT)
        {
            prdt[PrdtIndex * 4 + 3] = SATA_PRDT_DATA_LENGTH_UNIT - 1;
            Data_Length -= SATA_PRDT_DATA_LENGTH_UNIT;
            Data_Addr += SATA_PRDT_DATA_LENGTH_UNIT;
        }
        else
        {
            prdt[PrdtIndex * 4 + 3] = Data_Length - 1;
            break;
        }
    }

    // total used prdt = PrdtIndex + 1
    PrdtIndex = PrdtIndex + 1;

    sata_debug("PrdtIndex = %d\n", PrdtIndex);

    build_cmd_prdt(cmd_tbl, &prdt[0], PrdtIndex);
    return PrdtIndex;
}


void sata_mstar_qc_prep(struct sata_mstar_port_priv *pp, struct hal_queued_cmd *qc)
{
    MS_U8 num_prde = 0;
    void *cmd_tbl;

    cmd_tbl = (void *)((MS_U32)pp->cmd_tbl + (qc->SlotNum * SATA_KA9_CMD_DESC_SIZE));

    sata_debug("cmd_tbl = 0x%x\n", (MS_U32)cmd_tbl);

    build_cmd_fis(cmd_tbl, &qc->cmd_h2dfis);

    if (qc->DataRequest)
    {
        num_prde = sata_mstar_fill_sg(qc, cmd_tbl);
    }
    sata_debug("number of prd entry %d\n", num_prde);

    sata_mstar_setup_cmd_hdr_entry(pp, qc, num_prde);
}

void sata_mstar_qc_issue(struct hal_queued_cmd *qc)
{
    //printf("issue 0x%x\n", qc->cmd_h2dfis.ata_cmd);
    sata_reg_write32(1 << qc->SlotNum, PORT_CMD_ISSUE + port_base);
}

void  sata_mstar_qc_multi_issue(MS_U32 ci)
{
    sata_reg_write32(ci, PORT_CMD_ISSUE + port_base);
}

int sata_mstar_qc_wait_complete(struct hal_queued_cmd *qc)
{
    MS_U32 u32temp;
    MS_U32 tries;

    u32temp = sata_reg_read32(PORT_CMD_ISSUE + port_base);

    //tries = 5000000;
    tries = 100;

    sata_debug("PORT_CMD_ISSUE = 0x%x\n", u32temp);

    while ((u32temp & (1 << qc->SlotNum)) && --tries)
        //while (u32temp & (1 << qc->SlotNum))
    {
        MsOS_DelayTask(1);
        u32temp = sata_reg_read32(PORT_CMD_ISSUE + port_base);
        //sata_debug("PORT_CMD_ISSUE = 0x%x\n", u32temp);
    }
#if 1
    if (tries == 0)
    {
        printf("*************** -Error - Wait Command Issue Clear Timeout ***************\n");
        return -1;
    }
#endif
    return 0;
}

int sata_mstar_qc_wait_complete_multi(void)
{
    MS_U32 u32temp;
    MS_U32 tries;

    u32temp = sata_reg_read32(PORT_CMD_ISSUE + port_base);

    //tries = 5000000;
    tries = 100;

    sata_debug("PORT_CMD_ISSUE = 0x%x\n", u32temp);

    while (u32temp  && --tries)
        //while (u32temp & (1 << qc->SlotNum))
    {
        MsOS_DelayTask(100);
        u32temp = sata_reg_read32(PORT_CMD_ISSUE + port_base);
        printf("ci = 0x%8.8x\n", (int)u32temp);
        //sata_debug("PORT_CMD_ISSUE = 0x%x\n", u32temp);
    }
    if (tries == 0)
    {
        printf("*************** -Error - Wait Command Issue Clear Timeout ***************\n");
        printf("ci = 0x%8.8x\n", (int)u32temp);
        return u32temp;
    }
    return 0;

}

void sata_mstar_interrupt_freeze(void)
{
    sata_reg_write32(0, PORT_IRQ_MASK + port_base);
}

void sata_mstar_interrupt_thaw(void)
{
    MS_U32 u32Temp = 0;

    // clear IRQ
    u32Temp = sata_reg_read32(PORT_IRQ_STAT + port_base);
    sata_reg_write32(u32Temp, PORT_IRQ_STAT + port_base);

    // Clear Port 0 IRQ on HBA
    u32Temp = sata_reg_read32(HOST_IRQ_STAT + hba_base);
    sata_reg_write32(u32Temp, HOST_IRQ_STAT + hba_base);

    // Enable Host Interrupt
    u32Temp = sata_reg_read32(HOST_CTL + hba_base);
    u32Temp |= HOST_IRQ_EN;
    sata_reg_write32(u32Temp, HOST_CTL + hba_base);

    // Enable Port Interrupt
    sata_reg_write32(DEF_PORT_IRQ, PORT_IRQ_MASK + port_base);
    sata_reg_read32(PORT_IRQ_MASK + port_base);

}

static unsigned int sata_mstar_dev_classify(void)
{
    MS_U32 temp = 0;
    MS_U8 nsect;
    MS_U8 lbal;
    MS_U8 lbam;
    MS_U8 lbah;

    temp = sata_reg_read32(PORT_SIG + port_base);

    lbah = (temp >> 24) & 0xff;
    lbam = (temp >> 16) & 0xff;
    lbal = (temp >> 8) & 0xff;
    nsect = temp & 0xff;

    if ((lbam == 0) && (lbah == 0))
    {
        printf("\nFound ATA device by sig\n");
        return ATA_DEV_ATA;
    }

    if ((lbam == 0x14) && (lbah == 0xeb))
    {
        printf("\nFound ATAPI device by sig\n");
        return ATA_DEV_ATAPI;
    }

    if ((lbam == 0x69) && (lbah == 0x96))
    {
        printf("\nFound PMP device by sig\n");
        return ATA_DEV_PMP;
    }

    if ((lbam == 0x3c) && (lbah == 0xc3))
    {
        printf("\nFound SEMB device by sig (could be ATA device)\n");
        return ATA_DEV_SEMB;
    }

    printf("\nUnknown device\n");
    return ATA_DEV_UNKNOWN;
}

int sata_mstar_hardreset(void)
{
    int tries;
    MS_U32 scontrol;
    MS_U32 u32temp = 0;
    unsigned int DeviceClass = 0;

    //printf("begin %s\n", __func__);
    mstar_ahci_stop_engine();

    /* issue phy wake/reset */
    scontrol = sata_reg_read32(PORT_SCR_CTL + port_base);

    scontrol = (scontrol & 0x0f0) | 0x301;

    sata_reg_write32(scontrol, PORT_SCR_CTL + port_base);

    scontrol = sata_reg_read32(PORT_SCR_CTL + port_base);

    /*
     * Writes to SControl sometimes get ignored under certain
     * controllers (ata_piix SIDPR).  Make sure DET actually is
     * cleared.
     */

    tries = 10;

    do
    {
        scontrol = (scontrol & 0x0f0) | 0x300;
        sata_reg_write32(scontrol, PORT_SCR_CTL + port_base);

        /*
         * Some PHYs react badly if SStatus is pounded
         * immediately after resuming.  Delay 200ms before
         * debouncing.
        */
        MsOS_DelayTask(200);

        /* is SControl restored correctly? */
        scontrol = sata_reg_read32(PORT_SCR_CTL + port_base);
    }
    while ((scontrol & 0xf0f) != 0x300 && --tries);

    if (tries == 0)
    {
        printf("[]-Error, SCR_CTL reset can't clear\n");
        return -1;
    }

    // wait some time to detect device
    MsOS_DelayTask(100);

    // clear error status after reset
    u32temp = sata_reg_read32(PORT_SCR_ERR + port_base);
    sata_reg_write32(u32temp, PORT_SCR_ERR + port_base);

    // check busy bit clear
    u32temp = sata_reg_read32(PORT_TFDATA + port_base);

    tries = 10;

    sata_debug("PORT_TFDATA = 0x%x\n", u32temp);

    while ((u32temp & ATA_BUSY) && --tries)
    {
        MsOS_DelayTask(200);
        u32temp = sata_reg_read32(PORT_TFDATA + port_base);
        sata_debug("PORT_TFDATA = 0x%x\n", u32temp);
    }

    mstar_ahci_start_engine();

    DeviceClass = sata_mstar_dev_classify();
    //printf("done %s\n", __func__);

    return DeviceClass;
}


int sata_mstar_port_start(struct sata_mstar_port_priv *pp)
{
    MS_U32 temp;

    //printf("begin %s\n", __func__);

    pp->PortFlags = 0;

#if (SATA_CMD_TYPE == TYPE_XIU)
    pp->cmd_slot = (void *)(SATA_SDMAP_RIU_BASE + (AHCI_P0CLB & 0xfff));
    pp->rx_fis = (void *)(SATA_SDMAP_RIU_BASE + (AHCI_P0FB & 0xfff));
    pp->cmd_tbl = (void *)(SATA_SDMAP_RIU_BASE + (AHCI_CTBA0 & 0xfff));

    pp->cmd_slot_dma = AHCI_P0CLB;
    pp->rx_fis_dma = AHCI_P0FB;
    pp->cmd_tbl_dma = AHCI_CTBA0;

#elif (SATA_CMD_TYPE == TYPE_DRAM)

    pp->cmd_slot = (void *)DRAM_P0PLB;
    pp->cmd_slot_dma = (MS_U32)DRAM_P0PLB;

    pp->rx_fis = (void *)((MS_U32)pp->cmd_slot + SATA_CMD_HEADER_SIZE);
    pp->rx_fis_dma = pp->cmd_slot_dma + SATA_CMD_HEADER_SIZE;

    pp->cmd_tbl = (void *)((MS_U32)pp->rx_fis + SATA_FIS_SIZE);
    pp->cmd_tbl_dma = pp->rx_fis_dma + SATA_FIS_SIZE;
#elif (SATA_CMD_TYPE == TYPE_RIU)
    pp->cmd_slot = (void *)(AHCI_P0CLB);
    pp->rx_fis = (void *)(AHCI_P0FB);
    pp->cmd_tbl = (void *)(AHCI_CTBA0);

    pp->cmd_slot_dma = AHCI_P0CLB;
    pp->rx_fis_dma = AHCI_P0FB;
    pp->cmd_tbl_dma = AHCI_CTBA0;

#else
#error "SATA_CMD_TYPE Unknown"
#endif

    sata_debug("cmd_slot = 0x%x ; cmd_slot_dma = 0x%x\n", (MS_U32)pp->cmd_slot, pp->cmd_slot_dma);
    sata_debug("rx_fis = 0x%x ; rx_fis_dma = 0x%x\n", (MS_U32)pp->rx_fis, pp->rx_fis_dma);
    sata_debug("cmd_tbl = 0x%x ; cmd_tbl_dma = 0x%x\n", (MS_U32)pp->cmd_tbl, pp->cmd_tbl_dma);

    temp = sata_reg_read32(PORT_CMD + port_base) & ~PORT_CMD_ICC_MASK;

    // spin up device
    temp |= PORT_CMD_SPIN_UP;
    sata_reg_write32(temp, PORT_CMD + port_base);

    // wake up link
    sata_reg_write32((temp | PORT_CMD_ICC_ACTIVE), PORT_CMD + port_base);

    // start FIS RX
    mstar_ahci_start_fis_rx(pp);

    // Clear IS , Interrupt Status
    sata_reg_write32(0xFFFFFFFF, PORT_IRQ_STAT + port_base);
    sata_reg_write32(0xFFFFFFFF, PORT_SCR_ERR + port_base);

    // set to speed limit with gen 1, gen 2 or auto
    temp = sata_reg_read32(PORT_SCR_CTL + port_base);
    //temp = temp & (~E_PORT_SPEED_MASK); //Auto
#if FPGA
    // FPGA is limited to Gen1
    printf("FPGA - Force connection to GEN1\n");
    temp = temp | E_PORT_SPEED_GEN1;  //Gen1 Limit
#else
    temp = temp | E_PORT_SPEED_GEN2;  // Gen2 Limit
    //temp = temp | E_PORT_SPEED_GEN3;  // no restriction

#endif

    sata_reg_write32(temp, PORT_SCR_CTL + port_base);

    // Start DMA Engine
    mstar_ahci_start_engine();
    //printf("done %s\n", __func__);

    return 0;
}

void sata_mstar_show_speed(void)
{
    MS_U32 u32temp;

    //printf("begin %s\n", __func__);

    u32temp = sata_reg_read32(PORT_SCR_STAT + port_base);

    u32temp = u32temp & E_PORT_SPEED_MASK;

    sata_debug("PORT_SCR_STAT = 0x%x\n", u32temp);


    switch (u32temp)
    {
        case E_PORT_SPEED_NO_RESTRICTION:
            printf("SATA Speed no restriction\n");
            break;
        case E_PORT_SPEED_GEN1:
            printf("SATA Speed GEN1 : 1.5Gbps\n");
            break;
        case E_PORT_SPEED_GEN2:
            printf("SATA Speed GEN2 : 3.0Gbps\n");
            break;
        case E_PORT_SPEED_GEN3:
            printf("SATA Speed GEN3 : 6.0Gbps\n");
            break;
        default:
            printf("SATA Speed detect Error\n");
    }
    //printf("end %s\n", __func__);

}

void sata_mstar_dump_full_reg(MS_U32 base)
{
    MS_U16 temp;
    int i;

    printf("(0x%06x)\n", (int)(base & 0x00ffffff) >> 1);

    for (i = 0; i < 0x100; i += 2)
    {
        temp = sata_reg_read16(base + (i << 1));
        if (i % 16 == 0)
            printf("\n%02x\t", i);
        printf("%04x ", temp);
    }
    printf("\n");
}

void sata_mstar_interrupt(void)
{
    MS_U32 host_status, port_status;

    host_status = sata_reg_read32(HOST_IRQ_STAT + hba_base);

    port_status = sata_reg_read32(PORT_IRQ_STAT + port_base);

    //printf("@@--%s--port_status = 0x%x\n", __func__, port_status);

    sata_reg_write32(port_status, PORT_IRQ_STAT + port_base);

    sata_reg_write32(host_status, HOST_IRQ_STAT + hba_base);

}


int mstar_sata_hardware_init(void)
{
    MS_U32 i;
    MS_U32 u32Temp = 0;
    MS_U16 u16Temp;

    //printf("4-1\n");
    //printf("skip sata phy clock\n");
    // Open SATA PHY Clock
#if 0
    sata_reg_write16(0x0030, 0xfd220000 + (0x28a2 << 1));
#endif
    //printf("4-2\n");
    //printf("kip sata miu1 switch\n");

    // Set SATA MIU1 switch
#if 0
    u16Temp = sata_reg_read16(0xfd200000 + (0x06f6 << 1));
    u16Temp |= 0x8000;
    sata_reg_write16(u16Temp, 0xfd200000 + (0x06f6 << 1));
#endif

    //printf("4-3\n");
    //printf("write misc base \n");

#if FPGA
    sata_reg_write16(0x0001, misc_base + SATA_MISC_HOST_SWRST);
    sata_reg_write16(0x0000, misc_base + SATA_MISC_HOST_SWRST); // relase sw reset
    sata_reg_write16(0x0003, misc_base + SATA_MISC_FPGA_EN);    // FPGA mode: enable apu_init.v
    sata_reg_write16(0x0000, misc_base + SATA_MISC_AMBA_MUXRST);
    sata_reg_write16(0x008C, misc_base + SATA_MISC_AMBA_ARBIT);
#else
    // ECO
    u16Temp = sata_reg_read16(misc_base + SATA_MISC_AGEN_F_VAL);
    u16Temp |= 0x8000;
    sata_reg_write16(u16Temp, misc_base + SATA_MISC_AGEN_F_VAL);

    // enable MAC clock
    sata_reg_write16(0x000c, 0x1f000000 + (0x100b64 << 1));

    sata_reg_write16(0x0001, misc_base + SATA_MISC_HOST_SWRST);
    sata_reg_write16(0x0000, misc_base + SATA_MISC_HOST_SWRST);
    sata_reg_write16(0x0000, misc_base + SATA_MISC_AMBA_MUXRST);
    sata_reg_write16(0x008C, misc_base + SATA_MISC_AMBA_ARBIT);

    // PHY setting
    // SATA Phy Setting, based on 20160701 21:05 script
    sata_reg_write16(0x0000, 0x1f000000 + (0x103922 << 1));
    sata_reg_write16(0x0000, 0x1f000000 + (0x103924 << 1));
    sata_reg_write16(0x8304, 0x1f000000 + (0x10390c << 1));
    sata_reg_write16(0x0200, 0x1f000000 + (0x10390e << 1));
    sata_reg_write16(0x0000, 0x1f000000 + (0x103910 << 1));
    sata_reg_write16(0x08b8, 0x1f000000 + (0x10390a << 1));
    sata_reg_write16(0x8087, 0x1f000000 + (0x103902 << 1));
    sata_reg_write16(0x808e, 0x1f000000 + (0x103902 << 1));

    // for Gen2 connection
    u16Temp = sata_reg_read16(0x1f000000 + (0x103902 << 1));
    u16Temp &= 0xF3FF;
    u16Temp |= (0x1 << 10);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103902 << 1)));

    u16Temp = sata_reg_read16(0x1f000000 + (0x10390c << 1));
    u16Temp &= 0xF0FF;
    u16Temp |= (0x1 << 8);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x10390c << 1)));

    sata_reg_write16(0x2320, 0x1f000000 + (0x103918 << 1)); // ?
    sata_reg_write16(0x0040, 0x1f000000 + (0x10397c << 1));

    // Gen1
    // Tx swing
    sata_reg_write16(0x2031, 0x1f000000 + (0x103994 << 1));
    sata_reg_write16(0x3803, 0x1f000000 + (0x103996 << 1));
    // EQ BW
    u16Temp = sata_reg_read16(0x1f000000 + (0x103992 << 1));
    u16Temp &= ~0x0006;
    u16Temp |= (0x0006 & 0x0020);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103992 << 1)));
    // EQ STR
    u16Temp = sata_reg_read16(0x1f000000 + (0x103992 << 1));
    u16Temp &= ~0x001f;
    u16Temp |= (0x001f & 0x0000);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103992 << 1)));

    // Gen2
    // Tx swing
    sata_reg_write16(0x3935, 0x1f000000 + (0x10392c << 1));
    sata_reg_write16(0x3920, 0x1f000000 + (0x10392e << 1));
    // EQ BW
    u16Temp = sata_reg_read16(0x1f000000 + (0x103950 << 1));
    u16Temp &= ~0x1800;
    u16Temp |= (0x1800 & 0x0800);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103950 << 1)));
    // EQ STR
    u16Temp = sata_reg_read16(0x1f000000 + (0x103926 << 1));
    u16Temp &= ~0x001f;
    u16Temp |= (0x001f & 0x0000);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103926 << 1)));

    // Gen3
    // Tx swing
    sata_reg_write16(0x3937, 0x1f000000 + (0x10399a << 1));
    sata_reg_write16(0x3921, 0x1f000000 + (0x10399c << 1));
    // EQ BW
    u16Temp = sata_reg_read16(0x1f000000 + (0x103998 << 1));
    u16Temp &= ~0x0006;
    u16Temp |= (0x0006 & 0x0000);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103998 << 1)));
    // EQ STR
    u16Temp = sata_reg_read16(0x1f000000 + (0x103998 << 1));
    u16Temp &= ~0x001f;
    u16Temp |= (0x001f & 0x0000);
    sata_reg_write16(u16Temp, 0x1f000000 + ((0x103998 << 1)));

    // SSC setup
    sata_reg_write16(0x3007, 0x1f000000 + (0x103964 << 1));
    sata_reg_write16(0xdf0a, 0x1f000000 + (0x103960 << 1));
    sata_reg_write8(0x24, 0x1f000000 + (0x103962 << 1));
    sata_reg_write16(0x02e8, 0x1f000000 + (0x103966 << 1));

    sata_reg_write16(0x0001, hba_base + HOST_CTL); // reset HBA
    printf("sata hardware init done\n");

#endif

    //printf("4-4\n");
    // AHB Data FIFO  Setting
    sata_reg_write16(0x0000, misc_base + SATA_MISC_HBA_HADDR);
    sata_reg_write16(0x0000, misc_base + SATA_MISC_HBA_LADDR);
    sata_reg_write16(0x1800, misc_base + SATA_MISC_CMD_HADDR);
    sata_reg_write16(0x1000, misc_base + SATA_MISC_CMD_LADDR);
    sata_reg_write16(0x0000, misc_base + SATA_MISC_DATA_ADDR);
    //sata_reg_write16(0x0040, misc_base + SATA_MISC_DATA_ADDR);
    sata_reg_write16(0x0001, misc_base + SATA_MISC_ENRELOAD);
    sata_reg_write16(0x0001, misc_base + SATA_MISC_ACCESS_MODE);

    //printf("4-5\n");
    //printf("skip phy setting\n");
    // SATA Phy Setting
#if 0
    sata_reg_write16(0x0000, 0xfd240000 + (0x2322 << 1));
    sata_reg_write16(0x2514, 0xfd240000 + (0x2324 << 1));
    sata_reg_write16(0x0302, 0xfd240000 + (0x230c << 1));
    sata_reg_write16(0x2200, 0xfd240000 + (0x230e << 1));
    sata_reg_write16(0x0300, 0xfd240000 + (0x2310 << 1));
    sata_reg_write16(0x2190, 0xfd240000 + (0x2368 << 1));
    sata_reg_write16(0x0000, 0xfd240000 + (0x237e << 1));
    sata_reg_write16(0x0a3d, 0xfd240000 + (0x2360 << 1));
    sata_reg_write16(0x0017, 0xfd240000 + (0x2362 << 1));
    sata_reg_write16(0x8087, 0xfd240000 + (0x2302 << 1));
    sata_reg_write16(0x8086, 0xfd240000 + (0x2302 << 1));
#endif

    //printf("4-6\n");
    printf("write hba_base: HOST_RESET\n");
    printf("writing 0x%x with 0x%d\n", (int)(hba_base + HOST_CTL, (int)HOST_RESET));
    u32Temp = hba_base + HOST_CTL;
    printf("hba_base = %x\n", (int)u32Temp);

    sata_reg_write16(HOST_RESET, hba_base + (HOST_CTL));

    //printf("4-7\n");
    u32Temp = mstar_sata_wait_reg(HOST_CTL + hba_base, HOST_RESET, HOST_RESET, 1, 500);
    //printf("4-8\n");

    if (u32Temp & HOST_RESET)
        return -1;

    //printf("5-9\n");
    //  Turn on AHCI_EN
    u32Temp = sata_reg_read32(HOST_CTL + hba_base);
    if (u32Temp & HOST_AHCI_EN)
        goto FW_INITIAL;
    //printf("4-10\n");

    // Try AHCI_EN Trurn on for a few time
    for (i = 0; i < 5; i++)
    {
        u32Temp |= HOST_AHCI_EN;
        sata_reg_write32(u32Temp, HOST_CTL + hba_base);
        u32Temp = sata_reg_read32(HOST_CTL + hba_base);
        if (u32Temp & HOST_AHCI_EN)
            break;
        MsOS_DelayTask(10);
    }

    //printf("4-11\n");
FW_INITIAL:

    //  Init FW to trigger controller
    sata_reg_write16(0x0000, hba_base + (HOST_CAP));
    sata_reg_write16(0x0000, hba_base + (HOST_CAP + 0x4));
    //printf("4-12\n");

    // Port Implement
    sata_reg_write16(0x0001, hba_base + (HOST_PORTS_IMPL));
    sata_reg_write16(0x0000, port_base + (PORT_CMD));
    sata_reg_write16(0x0000, port_base + (PORT_CMD + 0x4));

    return 0;
}

void mstar_sata_set_base(MS_U32 u32hba_base, MS_U32 u32port_base, MS_U32 u32misc_base)
{
    hba_base = u32hba_base;
    port_base = u32port_base;
    misc_base = u32misc_base;
}
