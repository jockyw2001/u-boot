//------------------------------------------------------------------------------
//
// Copyright (c) 2010 MStar Semiconductor, Inc.  All rights reserved.
//
//------------------------------------------------------------------------------
// FILE
//
// DESCRIPTION
//      This header file defines some utilities.
//
// HISTORY
//
//------------------------------------------------------------------------------
#ifndef __MS_TYPES_H__
#define __MS_TYPES_H__

#ifndef    U8
#define    U8     unsigned char             // 1 byte
#endif
#ifndef    U16
#define    U16    unsigned short            // 2 byte
#endif
#ifndef    U32
#define    U32    unsigned int              // 4 byte
#endif
#ifndef    U64
#define    U64    unsigned long long        // 8 byte
#endif

#ifndef    S8
#define    S8     signed char               // 1 byte
#endif
#ifndef    S16
#define    S16    signed short              // 2 byte
#endif
#ifndef    S32
#define    S32    signed int                // 4 byte
#endif

#ifndef    B16
#define    B16    unsigned short            // 2 byte
#endif
#ifndef    BOOL
#define    BOOL   unsigned int              // 4 byte
#endif

#define BOOLEAN BOOL


typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef u32 dma_addr_t;


#ifndef NULL
#define NULL    ((void*)0)
#endif


#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                           1
/// definition for FALSE
#define FALSE                          0
#endif

#ifndef true
/// definition for true
#define true                           1
/// definition for false
#define false                          0
#endif


#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                           1
/// definition for FALSE
#define FALSE                          0
#endif


#if !defined(ENABLE) && !defined(DISABLE)
/// definition for ENABLE
#define ENABLE                         1
/// definition for DISABLE
#define DISABLE                        0
#endif


#if !defined(ON) && !defined(OFF)
/// definition for ON
#define ON                             1
/// definition for OFF
#define OFF                            0
#endif

/// @name BIT#
/// definition of one bit mask
/// @{
#if !defined(BIT0) && !defined(BIT1)
#define BIT0                           0x00000001
#define BIT1                           0x00000002
#define BIT2                           0x00000004
#define BIT3                           0x00000008
#define BIT4                           0x00000010
#define BIT5                           0x00000020
#define BIT6                           0x00000040
#define BIT7                           0x00000080
#define BIT8                           0x00000100
#define BIT9                           0x00000200
#define BIT10                          0x00000400
#define BIT11                          0x00000800
#define BIT12                          0x00001000
#define BIT13                          0x00002000
#define BIT14                          0x00004000
#define BIT15                          0x00008000
#define BIT16                          0x00010000
#define BIT17                          0x00020000
#define BIT18                          0x00040000
#define BIT19                          0x00080000
#define BIT20                          0x00100000
#define BIT21                          0x00200000
#define BIT22                          0x00400000
#define BIT23                          0x00800000
#define BIT24                          0x01000000
#define BIT25                          0x02000000
#define BIT26                          0x04000000
#define BIT27                          0x08000000
#define BIT28                          0x10000000
#define BIT29                          0x20000000
#define BIT30                          0x40000000
#define BIT31                          0x80000000
#endif
#define BK_REG(reg)             ((reg) << 2)

#if !defined(REG_ID_00) && !defined(REG_ID_01)

#define REG_ID_00      BK_REG(0x00)
#define REG_ID_01      BK_REG(0x01)
#define REG_ID_02      BK_REG(0x02)
#define REG_ID_03      BK_REG(0x03)
#define REG_ID_04      BK_REG(0x04)
#define REG_ID_05      BK_REG(0x05)
#define REG_ID_06      BK_REG(0x06)
#define REG_ID_07      BK_REG(0x07)
#define REG_ID_08      BK_REG(0x08)
#define REG_ID_09      BK_REG(0x09)
#define REG_ID_0A      BK_REG(0x0A)
#define REG_ID_0B      BK_REG(0x0B)
#define REG_ID_0C      BK_REG(0x0C)
#define REG_ID_0D      BK_REG(0x0D)
#define REG_ID_0E      BK_REG(0x0E)
#define REG_ID_0F      BK_REG(0x0F)


#define REG_ID_10      BK_REG(0x10)
#define REG_ID_11      BK_REG(0x11)
#define REG_ID_12      BK_REG(0x12)
#define REG_ID_13      BK_REG(0x13)
#define REG_ID_14      BK_REG(0x14)
#define REG_ID_15      BK_REG(0x15)
#define REG_ID_16      BK_REG(0x16)
#define REG_ID_17      BK_REG(0x17)
#define REG_ID_18      BK_REG(0x18)
#define REG_ID_19      BK_REG(0x19)
#define REG_ID_1A      BK_REG(0x1A)
#define REG_ID_1B      BK_REG(0x1B)
#define REG_ID_1C      BK_REG(0x1C)
#define REG_ID_1D      BK_REG(0x1D)
#define REG_ID_1E      BK_REG(0x1E)
#define REG_ID_1F      BK_REG(0x1F)

#define REG_ID_20      BK_REG(0x20)
#define REG_ID_21      BK_REG(0x21)
#define REG_ID_22      BK_REG(0x22)
#define REG_ID_23      BK_REG(0x23)
#define REG_ID_24      BK_REG(0x24)
#define REG_ID_25      BK_REG(0x25)
#define REG_ID_26      BK_REG(0x26)
#define REG_ID_27      BK_REG(0x27)
#define REG_ID_28      BK_REG(0x28)
#define REG_ID_29      BK_REG(0x29)
#define REG_ID_2A      BK_REG(0x2A)
#define REG_ID_2B      BK_REG(0x2B)
#define REG_ID_2C      BK_REG(0x2C)
#define REG_ID_2D      BK_REG(0x2D)
#define REG_ID_2E      BK_REG(0x2E)
#define REG_ID_2F      BK_REG(0x2F)


#define REG_ID_30      BK_REG(0x30)
#define REG_ID_31      BK_REG(0x31)
#define REG_ID_32      BK_REG(0x32)
#define REG_ID_33      BK_REG(0x33)
#define REG_ID_34      BK_REG(0x34)
#define REG_ID_35      BK_REG(0x35)
#define REG_ID_36      BK_REG(0x36)
#define REG_ID_37      BK_REG(0x37)
#define REG_ID_38      BK_REG(0x38)
#define REG_ID_39      BK_REG(0x39)
#define REG_ID_3A      BK_REG(0x3A)
#define REG_ID_3B      BK_REG(0x3B)
#define REG_ID_3C      BK_REG(0x3C)
#define REG_ID_3D      BK_REG(0x3D)
#define REG_ID_3E      BK_REG(0x3E)
#define REG_ID_3F      BK_REG(0x3F)


#define REG_ID_40      BK_REG(0x40)
#define REG_ID_41      BK_REG(0x41)
#define REG_ID_42      BK_REG(0x42)
#define REG_ID_43      BK_REG(0x43)
#define REG_ID_44      BK_REG(0x44)
#define REG_ID_45      BK_REG(0x45)
#define REG_ID_46      BK_REG(0x46)
#define REG_ID_47      BK_REG(0x47)
#define REG_ID_48      BK_REG(0x48)
#define REG_ID_49      BK_REG(0x49)
#define REG_ID_4A      BK_REG(0x4A)
#define REG_ID_4B      BK_REG(0x4B)
#define REG_ID_4C      BK_REG(0x4C)
#define REG_ID_4D      BK_REG(0x4D)
#define REG_ID_4E      BK_REG(0x4E)
#define REG_ID_4F      BK_REG(0x4F)


#define REG_ID_50      BK_REG(0x50)
#define REG_ID_51      BK_REG(0x51)
#define REG_ID_52      BK_REG(0x52)
#define REG_ID_53      BK_REG(0x53)
#define REG_ID_54      BK_REG(0x54)
#define REG_ID_55      BK_REG(0x55)
#define REG_ID_56      BK_REG(0x56)
#define REG_ID_57      BK_REG(0x57)
#define REG_ID_58      BK_REG(0x58)
#define REG_ID_59      BK_REG(0x59)
#define REG_ID_5A      BK_REG(0x5A)
#define REG_ID_5B      BK_REG(0x5B)
#define REG_ID_5C      BK_REG(0x5C)
#define REG_ID_5D      BK_REG(0x5D)
#define REG_ID_5E      BK_REG(0x5E)
#define REG_ID_5F      BK_REG(0x5F)


#define REG_ID_60      BK_REG(0x60)
#define REG_ID_61      BK_REG(0x61)
#define REG_ID_62      BK_REG(0x62)
#define REG_ID_63      BK_REG(0x63)
#define REG_ID_64      BK_REG(0x64)
#define REG_ID_65      BK_REG(0x65)
#define REG_ID_66      BK_REG(0x66)
#define REG_ID_67      BK_REG(0x67)
#define REG_ID_68      BK_REG(0x68)
#define REG_ID_69      BK_REG(0x69)
#define REG_ID_6A      BK_REG(0x6A)
#define REG_ID_6B      BK_REG(0x6B)
#define REG_ID_6C      BK_REG(0x6C)
#define REG_ID_6D      BK_REG(0x6D)
#define REG_ID_6E      BK_REG(0x6E)
#define REG_ID_6F      BK_REG(0x6F)


#define REG_ID_70      BK_REG(0x70)
#define REG_ID_71      BK_REG(0x71)
#define REG_ID_72      BK_REG(0x72)
#define REG_ID_73      BK_REG(0x73)
#define REG_ID_74      BK_REG(0x74)
#define REG_ID_75      BK_REG(0x75)
#define REG_ID_76      BK_REG(0x76)
#define REG_ID_77      BK_REG(0x77)
#define REG_ID_78      BK_REG(0x78)
#define REG_ID_79      BK_REG(0x79)
#define REG_ID_7A      BK_REG(0x7A)
#define REG_ID_7B      BK_REG(0x7B)
#define REG_ID_7C      BK_REG(0x7C)
#define REG_ID_7D      BK_REG(0x7D)
#define REG_ID_7E      BK_REG(0x7E)
#define REG_ID_7F      BK_REG(0x7F)

#endif

/// @}
#endif // __MS_TYPE_H__
