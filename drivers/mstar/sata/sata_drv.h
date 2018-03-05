#ifndef _SATA_DRV_H_
#define _SATA_DRV_H_

// Local Definition

// 0x1800xxxx -> HBA SRAM
#define AHCI_P0CLB                  0x18001000
#define AHCI_P0FB                   0x18001100
#define AHCI_CTBA0                  0x18001200

#define SATA_PORT_NUM               1
//#define SATA_SDMAP_RIU_BASE         0xFD2A0000 // K3s
#define SATA_SDMAP_RIU_BASE         0xFD3F0000 // K6 0x1F8x (x2 = 0x3f0)
#define SATA_CMD_HEADER_SIZE 		0x400
#define SATA_FIS_SIZE 				0x100

#define DRAM_P0PLB                  0x21000000

#define TYPE_XIU 0
#define TYPE_DRAM 1
#define TYPE_RIU 3

#define SATA_CMD_TYPE TYPE_DRAM
//#define SATA_CMD_TYPE TYPE_XIU
//#define SATA_CMD_TYPE TYPE_RIU
//
#define CFIFO_ACCESS_ADDR (0x10 << 1)

enum
{
    /* global controller registers */
    HOST_CAP        = 0x00,        /* host capabilities */
    HOST_CTL        = (0x04 << 1), /* global host control */
    HOST_IRQ_STAT   = (0x08 << 1), /* interrupt status */
    HOST_PORTS_IMPL = (0x0c << 1), /* bitmap of implemented ports */
    HOST_VERSION    = (0x10 << 1), /* AHCI spec. version compliancy */
    HOST_CAP2       = (0x24 << 1), /* host capabilities, extended */

    /* HOST_CTL bits - HOST_CAP, 0x00 */
    HOST_RESET      = (1 << 0),  /* reset controller; self-clear */
    HOST_IRQ_EN     = (1 << 1),  /* global IRQ enable */
    HOST_AHCI_EN    = (1 << 31), /* AHCI enabled */

    /* Registers for each SATA port */
    PORT_LST_ADDR       = 0x00, /* command list DMA addr */
    PORT_LST_ADDR_HI    = (0x04 << 1), /* command list DMA addr hi */
    PORT_FIS_ADDR       = (0x08 << 1), /* FIS rx buf addr */
    PORT_FIS_ADDR_HI    = (0x0c << 1), /* FIS rx buf addr hi */
    PORT_IRQ_STAT       = (0x10 << 1), /* interrupt status */
    PORT_IRQ_MASK       = (0x14 << 1), /* interrupt enable/disable mask */
    PORT_CMD            = (0x18 << 1), /* port command */
    PORT_TFDATA         = (0x20 << 1), /* taskfile data */
    PORT_SIG            = (0x24 << 1), /* device TF signature */
    PORT_SCR_STAT       = (0x28 << 1), /* SATA phy register: SStatus */
    PORT_SCR_CTL        = (0x2c << 1), /* SATA phy register: SControl */
    PORT_SCR_ERR        = (0x30 << 1), /* SATA phy register: SError */
    PORT_SCR_ACT        = (0x34 << 1), /* SATA phy register: SActive */
    PORT_CMD_ISSUE      = (0x38 << 1), /* command issue */
    PORT_SCR_NTF        = (0x3c << 1), /* SATA phy register: SNotification */
    PORT_DMA_CTRL       = (0x70 << 1), /* SATA phy register: SNotification */

    /* PORT_CMD bits */
    PORT_CMD_ASP        = (1 << 27), /* Aggressive Slumber/Partial */
    PORT_CMD_ALPE       = (1 << 26), /* Aggressive Link PM enable */
    PORT_CMD_ATAPI      = (1 << 24), /* Device is ATAPI */
    PORT_CMD_FBSCP      = (1 << 22), /* FBS Capable Port */
    PORT_CMD_PMP        = (1 << 17), /* PMP attached */
    PORT_CMD_LIST_ON    = (1 << 15), /* cmd list DMA engine running */
    PORT_CMD_FIS_ON     = (1 << 14), /* FIS DMA engine running */
    PORT_CMD_FIS_RX     = (1 << 4), /* Enable FIS receive DMA engine */
    PORT_CMD_CLO        = (1 << 3), /* Command list override */
    PORT_CMD_POWER_ON   = (1 << 2), /* Power up device */
    PORT_CMD_SPIN_UP    = (1 << 1), /* Spin up device */
    PORT_CMD_START      = (1 << 0), /* Enable port DMA engine */

    PORT_CMD_ICC_MASK   = (0xf << 28), /* i/f ICC state mask */
    PORT_CMD_ICC_ACTIVE = (0x1 << 28), /* Put i/f in active state */
    PORT_CMD_ICC_PARTIAL    = (0x2 << 28), /* Put i/f in partial state */
    PORT_CMD_ICC_SLUMBER    = (0x6 << 28), /* Put i/f in slumber state */

    /*  Status Error  */
    AHCI_PORT_SERR_RDIE  = (1 << 0),    /* Recovered Data Integrity Error */
    AHCI_PORT_SERR_RCE   = (1 << 1),    /* Recovered Communications Error */
    /* Bit 2 ~ 7 Reserved */
    AHCI_PORT_SERR_TDIE  = (1 << 8),    /* Transient Data Integrity Error */
    AHCI_PORT_SERR_PCDIE = (1 << 9),    /* Persistent    Communication    or    Data    Integrity    Error */
    AHCI_PORT_SERR_PE    = (1 << 10),   /* Protocol  Error */
    AHCI_PORT_SERR_IE    = (1 << 11),   /* Internal  Error */
    /* Bit 15 ~ 12 Reserved */
    AHCI_PORT_SERR_PRDYC = (1 << 16),   /* PhyRdy  Change */
    AHCI_PORT_SERR_PIE   = (1 << 17),   /* Phy  Internal  Error */
    AHCI_PORT_SERR_COMW  = (1 << 18),   /* Comm Wake Detect */
    AHCI_PORT_SERR_TDE   = (1 << 19),   /* 10B  to  8B  Decode  Error  */
    AHCI_PORT_SERR_DE    = (1 << 20),   /* Disparity Error <= Not Use by AHCI  */
    AHCI_PORT_SERR_CRCE  = (1 << 21),   /* CRC Error  */
    AHCI_PORT_SERR_HE    = (1 << 22),   /* Handshake  Error */
    AHCI_PORT_SERR_LSE   = (1 << 23),   /* Link Sequence Error  */
    AHCI_PORT_SERR_TSTE  = (1 << 24),   /* Transport  state  transition  error  */
    AHCI_PORT_SERR_UFIS  = (1 << 25),   /* Unknown FIS Type  */
    AHCI_PORT_SERR_EXC   = (1 << 26),   /* Exchanged :  a  change  in device  presence  has  been  detected */
    /* Bit 31 ~ 27 Reserved */
};

enum
{
    SATA_KA9_USED_PRD    = 24,
    SATA_KA9_MAX_PRD    = 24,
    SATA_KA9_CMD_HDR_SIZE   = 0x20,

    SATA_KA9_CMD_DESC_CFIS_SZ   = 64,
    SATA_KA9_CMD_DESC_ACMD_SZ   = 16,
    SATA_KA9_CMD_DESC_RSRVD     = 48,

    SATA_PRDT_DATA_LENGTH_UNIT = 0x200000,

    // 512 bytes
    SATA_KA9_CMD_DESC_SIZE  = (SATA_KA9_CMD_DESC_CFIS_SZ +
                               SATA_KA9_CMD_DESC_ACMD_SZ +
                               SATA_KA9_CMD_DESC_RSRVD +
                               SATA_KA9_MAX_PRD * 16),

    //128 bytes
    SATA_KA9_CMD_DESC_OFFSET_TO_PRDT    =
        (SATA_KA9_CMD_DESC_CFIS_SZ +
         SATA_KA9_CMD_DESC_ACMD_SZ +
         SATA_KA9_CMD_DESC_RSRVD),
};

enum
{
    E_PORT_SPEED_MASK = (0xF << 4),
    E_PORT_SPEED_NO_RESTRICTION = (0x0 << 4),
    E_PORT_SPEED_GEN1 = (0x1 << 4),
    E_PORT_SPEED_GEN2 = (0x2 << 4),
    E_PORT_SPEED_GEN3 = (0x3 << 4),

    E_PORT_DET_MASK = (0xF << 0),  //Device  Detection  Initialization
    E_PORT_DET_NODEVICE_DETECT = 0x0,
    E_PORT_DET_HW_RESET = 0x1, // Cause HW send initial sequence
    E_PORT_DET_DISABLE_PHY = 0x4, // Put PHY into Offline
    E_PORT_DET_DEVICE_NOEST = 0x1, // not established
    E_PORT_DET_DEVICE_EST = 0x3,  // established
    E_PORT_DET_PHY_OFFLINE = 0x4, // Put PHY into Offline

    DATA_COMPLETE_INTERRUPT = (1 << 31),

    /* PORT_IRQ_{STAT,MASK} bits */
    PORT_IRQ_COLD_PRES	= (1 << 31), /* cold presence detect */
    PORT_IRQ_TF_ERR 	= (1 << 30), /* task file error */
    PORT_IRQ_HBUS_ERR	= (1 << 29), /* host bus fatal error */
    PORT_IRQ_HBUS_DATA_ERR	= (1 << 28), /* host bus data error */
    PORT_IRQ_IF_ERR 	= (1 << 27), /* interface fatal error */
    PORT_IRQ_IF_NONFATAL	= (1 << 26), /* interface non-fatal error */
    PORT_IRQ_OVERFLOW	= (1 << 24), /* xfer exhausted available S/G */
    PORT_IRQ_BAD_PMP	= (1 << 23), /* incorrect port multiplier */

    PORT_IRQ_PHYRDY 	= (1 << 22), /* PhyRdy changed */
    PORT_IRQ_DEV_ILCK	= (1 << 7), /* device interlock */
    PORT_IRQ_CONNECT	= (1 << 6), /* port connect change status */
    PORT_IRQ_SG_DONE	= (1 << 5), /* descriptor processed */
    PORT_IRQ_UNK_FIS	= (1 << 4), /* unknown FIS rx'd */
    PORT_IRQ_SDB_FIS	= (1 << 3), /* Set Device Bits FIS rx'd */
    PORT_IRQ_DMAS_FIS	= (1 << 2), /* DMA Setup FIS rx'd */
    PORT_IRQ_PIOS_FIS	= (1 << 1), /* PIO Setup FIS rx'd */
    PORT_IRQ_D2H_REG_FIS	= (1 << 0), /* D2H Register FIS rx'd */

    PORT_IRQ_FREEZE		= PORT_IRQ_HBUS_ERR |
                          PORT_IRQ_IF_ERR |
                          PORT_IRQ_CONNECT |
                          PORT_IRQ_PHYRDY |
                          PORT_IRQ_UNK_FIS |
                          PORT_IRQ_BAD_PMP,
    PORT_IRQ_ERROR		= PORT_IRQ_FREEZE |
                          PORT_IRQ_TF_ERR |
                          PORT_IRQ_HBUS_DATA_ERR,
    DEF_PORT_IRQ		= PORT_IRQ_ERROR | PORT_IRQ_SG_DONE |
                          PORT_IRQ_SDB_FIS | PORT_IRQ_DMAS_FIS |
                          PORT_IRQ_PIOS_FIS | PORT_IRQ_D2H_REG_FIS,

    ATA_BUSY		= (1 << 7),	/* BSY status bit */
    ATA_DRDY		= (1 << 6),	/* device ready */
    ATA_DF			= (1 << 5),	/* device fault */
    ATA_DSC			= (1 << 4),	/* drive seek complete */
    ATA_DRQ			= (1 << 3),	/* data request i/o */
    ATA_CORR		= (1 << 2),	/* corrected data error */
    ATA_IDX			= (1 << 1),	/* index */
    ATA_ERR			= (1 << 0),	/* have an error */

    ATA_DEV_UNKNOWN		= 0,	/* unknown device */
    ATA_DEV_ATA		= 1,	/* ATA device */
    ATA_DEV_ATA_UNSUP	= 2,	/* ATA device (unsupported) */
    ATA_DEV_ATAPI		= 3,	/* ATAPI device */
    ATA_DEV_ATAPI_UNSUP	= 4,	/* ATAPI device (unsupported) */
    ATA_DEV_PMP		= 5,	/* SATA port multiplier */
    ATA_DEV_PMP_UNSUP	= 6,	/* SATA port multiplier (unsupported) */
    ATA_DEV_SEMB		= 7,	/* SEMB */
    ATA_DEV_SEMB_UNSUP	= 8,	/* SEMB (unsupported) */
    ATA_DEV_NONE		= 9,	/* no device */

    /* struct ata_port pflags */
    ATA_PFLAG_UDMA_MASK         = (0x7f),
    ATA_PFLAG_UDMA_0			= (1 << 0),
    ATA_PFLAG_UDMA_1			= (1 << 1),
    ATA_PFLAG_UDMA_2			= (1 << 2),
    ATA_PFLAG_UDMA_3			= (1 << 3),
    ATA_PFLAG_UDMA_4			= (1 << 4),
    ATA_PFLAG_UDMA_5			= (1 << 5),
    ATA_PFLAG_UDMA_6			= (1 << 6),
    ATA_PFLAG_LBA_EXT			= (1 << 7),	/* Support 48-bits address */

};

/*
*          Host Controller MISC Register
*/
enum
{
    SATA_MISC_CFIFO_ADDRL     = ((0x10 << 1 ) << 1),
    SATA_MISC_CFIFO_ADDRH     = ((0x11 << 1 ) << 1),
    SATA_MISC_CFIFO_WDATAL    = ((0x12 << 1 ) << 1),
    SATA_MISC_CFIFO_WDATAH    = ((0x13 << 1 ) << 1),
    SATA_MISC_CFIFO_RDATAL    = ((0x14 << 1 ) << 1),
    SATA_MISC_CFIFO_RDATAH    = ((0x15 << 1 ) << 1),
    SATA_MISC_CFIFO_RORW      = ((0x16 << 1 ) << 1),
    SATA_MISC_CFIFO_ACCESS    = ((0x17 << 1 ) << 1),
    SATA_MISC_ACCESS_MODE     = ((0x18 << 1 ) << 1),
    SATA_MISC_AMBA_MUXRST     = ((0x21 << 1 ) << 1),
    SATA_MISC_HBA_LADDR       = ((0x24 << 1 ) << 1),
    SATA_MISC_HBA_HADDR       = ((0x25 << 1 ) << 1),
    SATA_MISC_CMD_LADDR       = ((0x26 << 1 ) << 1),
    SATA_MISC_CMD_HADDR       = ((0x27 << 1 ) << 1),
    SATA_MISC_DATA_ADDR       = ((0x28 << 1 ) << 1),
    SATA_MISC_ENRELOAD        = ((0x29 << 1 ) << 1),
    SATA_MISC_AMBA_ARBIT      = ((0x2A << 1 ) << 1),
    SATA_MISC_AMBA_PGEN       = ((0x30 << 1 ) << 1),
    SATA_MISC_AGEN_F_VAL      = ((0x35 << 1 ) << 1),
    SATA_MISC_HOST_SWRST      = ((0x50 << 1 ) << 1),
    SATA_MISC_HOST_NEAR       = ((0x51 << 1 ) << 1),
    SATA_MISC_FPGA_EN         = ((0x55 << 1 ) << 1),
};


typedef struct sata_cmd_header
{
    MS_U8     cmd_fis_len : 5;
    MS_U8     isATA_PI    : 1;
    MS_U8     iswrite     : 1;
    MS_U8     isprefetch  : 1; // enable only PRDT not zero
    MS_U8     issoftreset : 1;
    MS_U8     isbist      : 1;
    MS_U8     isclearok   : 1;
    MS_U8     reserverd   : 1;
    MS_U8     PMPid       : 4;
    MS_U16    PRDTlength  ;
    MS_U32    PRDBytes    ;
    MS_U32    ctba_lbase  ; // 0~6 is reserved
    MS_U32    ctba_hbase  ;
    MS_U32    resv[4]     ;

} hal_cmd_header;


//typedef struct __attribute__((__packed__)) sata_cmd_h2dfis
typedef struct sata_cmd_h2dfis
{
    MS_U8 u8fis_type       ;
    MS_U8 u8MPM           : 4;
    MS_U8 reserved_0      : 3;
    MS_U8 iscommand         : 1;
    MS_U8 ata_cmd          ;
    MS_U8 fearure          ;
    MS_U8 lba_l            ;
    MS_U8 lba_m            ;
    MS_U8 lba_h            ;
    MS_U8 device           ;
    MS_U8 lba_l_exp        ;
    MS_U8 lba_m_exp        ;
    MS_U8 lba_h_exp        ;
    MS_U8 fearure_exp      ;
    MS_U16 u16sector_cnt   ;
    MS_U8 reserved_1       ;
    MS_U8 control          ;
    MS_U32 reserved_2      ;
} hal_cmd_h2dfis;

struct hal_queued_cmd
{
    hal_cmd_h2dfis cmd_h2dfis;
    MS_U8 SlotNum;
    MS_BOOL IsWrite;
    MS_BOOL DataRequest;

    MS_U32 Data_Addr;
    MS_U32 Data_Length;
};

struct prdte
{
    MS_U32 dba;
    MS_U32 dbau;
    MS_U32 rev1;

    MS_U32 dbc: 22;
    MS_U32 rev2: 9;
    MS_U32 i: 1;
};

struct sata_mstar_port_priv
{
    MS_U32 PortFlags;
    void	*cmd_slot;
    void	*cmd_tbl;
    void	*rx_fis;
    MS_U32	cmd_slot_dma;
    MS_U32	cmd_tbl_dma;
    MS_U32	rx_fis_dma;
};

void sata_mstar_qc_prep(struct sata_mstar_port_priv *pp, struct hal_queued_cmd *qc);
void sata_mstar_interrupt_freeze(void);
void sata_mstar_interrupt_thaw(void);
int sata_mstar_hardreset(void);
void sata_mstar_show_speed(void);
void sata_mstar_interrupt_thaw(void);
void sata_mstar_qc_issue(struct hal_queued_cmd *qc);
int sata_mstar_qc_wait_complete(struct hal_queued_cmd *qc);
int sata_mstar_port_start(struct sata_mstar_port_priv *pp);
int mstar_sata_hardware_init(void);
void mstar_sata_set_base(MS_U32 u32hba_base, MS_U32 u32port_base, MS_U32 u32misc_base);

void sata_mstar_dump_full_reg(MS_U32 base);

#endif
