/** @file
  File name:    DQDPLVUG_BOXTYPE.h
  Input File:   BXTP_SOC.xlsx

  THIS FILE IS AUTO-GENERATED BY THE MMRC TOOL. DO NOT CHANGE THIS CODE.

  If edits are needed in this file, they must be done via the MMRC tool.

  If there is additional project-specific data required by the MMRC, it
  can be placed in DQDPLVUG_BOXTYPE.h, which is used for non-tool-generated
  data

  Includes register defines specific to a project.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2016 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

  @par Specification
**/
#ifndef _DQDPLVUG_BOXTYPE_H_
#define _DQDPLVUG_BOXTYPE_H_

#include "DataTypes.h"

#if USE_64_BIT_VARIABLES
#define UINTX UINT64
#else
#define UINTX UINT32
#endif

#define PTCTL0_DQDPLVUG_BOXTYPE_REG                        0x00000000
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX pten : 1;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX bytelane1datasel : 1;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX numpi_perindex : 1;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[5:3], RW, default = 0x0
    //
    UINTX ptrdtype2rdvld_plus1sel : 3;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[9:6], RW, default = 0x0
    //
    UINTX ptloadcode_dly : 4;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[16:10], RW, default = 0x0
    //
    UINTX rk0initialindex : 7;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[23:17], RW, default = 0x0
    //
    UINTX rk1initialindex : 7;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[29:24], RW, default = 0x0
    //
    UINTX ptrdtype2rdvld : 6;
    //
    // Reserved
    //
    // Bits[31:30], RO, default = 0x0
    //
    UINTX reserved : 2;
  } Bits;
  UINTX Data;
} PTCTL0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTCTL1_DQDPLVUG_BOXTYPE_REG                        0x00000004
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[0:0], RO, default = 0x0
    //
    UINTX reserved1 : 1;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[7:1], RO/V, default = 0x0
    //
    UINTX rk0currentindex : 7;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[8:8], RO, default = 0x0
    //
    UINTX reserved0 : 1;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[15:9], RO/V, default = 0x0
    //
    UINTX rk1currentindex : 7;
    //
    // LP4 Periodic Training Control 1
    //
    // Bits[31:16], RO, default = 0x0
    //
    UINTX reserved : 16;
  } Bits;
  UINTX Data;
} PTCTL1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT12_DQDPLVUG_BOXTYPE_REG                     0x00000008
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 12
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount2 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 12
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount1 : 16;
  } Bits;
  UINTX Data;
} PTROCNT12_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT34_DQDPLVUG_BOXTYPE_REG                     0x0000000C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 34
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount4 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 34
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount3 : 16;
  } Bits;
  UINTX Data;
} PTROCNT34_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT56_DQDPLVUG_BOXTYPE_REG                     0x00000010
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 56
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount6 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 56
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount5 : 16;
  } Bits;
  UINTX Data;
} PTROCNT56_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT78_DQDPLVUG_BOXTYPE_REG                     0x00000014
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 78
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount8 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 78
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount7 : 16;
  } Bits;
  UINTX Data;
} PTROCNT78_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT910_DQDPLVUG_BOXTYPE_REG                    0x00000018
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 910
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount10 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 910
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount9 : 16;
  } Bits;
  UINTX Data;
} PTROCNT910_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT1112_DQDPLVUG_BOXTYPE_REG                   0x0000001C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 1112
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount12 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 1112
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount11 : 16;
  } Bits;
  UINTX Data;
} PTROCNT1112_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT1314_DQDPLVUG_BOXTYPE_REG                   0x00000020
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 1314
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount14 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 1314
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount13 : 16;
  } Bits;
  UINTX Data;
} PTROCNT1314_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT1516_DQDPLVUG_BOXTYPE_REG                   0x00000024
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 1516
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount16 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 1516
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount15 : 16;
  } Bits;
  UINTX Data;
} PTROCNT1516_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT1718_DQDPLVUG_BOXTYPE_REG                   0x00000028
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 1718
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount18 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 1718
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount17 : 16;
  } Bits;
  UINTX Data;
} PTROCNT1718_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT1920_DQDPLVUG_BOXTYPE_REG                   0x0000002C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 1920
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount20 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 1920
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount19 : 16;
  } Bits;
  UINTX Data;
} PTROCNT1920_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT2122_DQDPLVUG_BOXTYPE_REG                   0x00000030
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 2122
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount22 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 2122
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount21 : 16;
  } Bits;
  UINTX Data;
} PTROCNT2122_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT2324_DQDPLVUG_BOXTYPE_REG                   0x00000034
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 2324
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount24 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 2324
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount23 : 16;
  } Bits;
  UINTX Data;
} PTROCNT2324_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT2526_DQDPLVUG_BOXTYPE_REG                   0x00000038
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 2526
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount26 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 2526
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount25 : 16;
  } Bits;
  UINTX Data;
} PTROCNT2526_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT2728_DQDPLVUG_BOXTYPE_REG                   0x0000003C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 2728
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount28 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 2728
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount27 : 16;
  } Bits;
  UINTX Data;
} PTROCNT2728_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT2930_DQDPLVUG_BOXTYPE_REG                   0x00000040
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 2930
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount30 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 2930
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount29 : 16;
  } Bits;
  UINTX Data;
} PTROCNT2930_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT3132_DQDPLVUG_BOXTYPE_REG                   0x00000044
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 3132
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount32 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 3132
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount31 : 16;
  } Bits;
  UINTX Data;
} PTROCNT3132_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT3334_DQDPLVUG_BOXTYPE_REG                   0x00000048
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 3334
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount34 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 3334
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount33 : 16;
  } Bits;
  UINTX Data;
} PTROCNT3334_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT3536_DQDPLVUG_BOXTYPE_REG                   0x0000004C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 3536
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount36 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 3536
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount35 : 16;
  } Bits;
  UINTX Data;
} PTROCNT3536_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT3738_DQDPLVUG_BOXTYPE_REG                   0x00000050
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 3738
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount38 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 3738
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount37 : 16;
  } Bits;
  UINTX Data;
} PTROCNT3738_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT3940_DQDPLVUG_BOXTYPE_REG                   0x00000054
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 3940
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount40 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 3940
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount39 : 16;
  } Bits;
  UINTX Data;
} PTROCNT3940_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT4142_DQDPLVUG_BOXTYPE_REG                   0x00000058
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 4142
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount42 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 4142
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount41 : 16;
  } Bits;
  UINTX Data;
} PTROCNT4142_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT4344_DQDPLVUG_BOXTYPE_REG                   0x0000005C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 4344
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount44 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 4344
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount43 : 16;
  } Bits;
  UINTX Data;
} PTROCNT4344_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT4546_DQDPLVUG_BOXTYPE_REG                   0x00000060
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 4546
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount46 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 4546
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount45 : 16;
  } Bits;
  UINTX Data;
} PTROCNT4546_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT4748_DQDPLVUG_BOXTYPE_REG                   0x00000064
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 4748
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount48 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 4748
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount47 : 16;
  } Bits;
  UINTX Data;
} PTROCNT4748_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT4950_DQDPLVUG_BOXTYPE_REG                   0x00000068
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 4950
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount50 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 4950
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount49 : 16;
  } Bits;
  UINTX Data;
} PTROCNT4950_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT5152_DQDPLVUG_BOXTYPE_REG                   0x0000006C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 5152
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount52 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 5152
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount51 : 16;
  } Bits;
  UINTX Data;
} PTROCNT5152_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT5354_DQDPLVUG_BOXTYPE_REG                   0x00000070
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 5354
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount54 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 5354
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount53 : 16;
  } Bits;
  UINTX Data;
} PTROCNT5354_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT5556_DQDPLVUG_BOXTYPE_REG                   0x00000074
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 5556
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount56 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 5556
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount55 : 16;
  } Bits;
  UINTX Data;
} PTROCNT5556_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT5758_DQDPLVUG_BOXTYPE_REG                   0x00000078
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 5758
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount58 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 5758
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount57 : 16;
  } Bits;
  UINTX Data;
} PTROCNT5758_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT5960_DQDPLVUG_BOXTYPE_REG                   0x0000007C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 5960
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount60 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 5960
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount59 : 16;
  } Bits;
  UINTX Data;
} PTROCNT5960_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT6162_DQDPLVUG_BOXTYPE_REG                   0x00000080
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 6162
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount62 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 6162
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount61 : 16;
  } Bits;
  UINTX Data;
} PTROCNT6162_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT6364_DQDPLVUG_BOXTYPE_REG                   0x00000084
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 6364
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount64 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 6364
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount63 : 16;
  } Bits;
  UINTX Data;
} PTROCNT6364_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT6566_DQDPLVUG_BOXTYPE_REG                   0x00000088
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 6566
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount66 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 6566
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount65 : 16;
  } Bits;
  UINTX Data;
} PTROCNT6566_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT6768_DQDPLVUG_BOXTYPE_REG                   0x0000008C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 6768
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount68 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 6768
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount67 : 16;
  } Bits;
  UINTX Data;
} PTROCNT6768_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define PTROCNT6970_DQDPLVUG_BOXTYPE_REG                   0x00000090
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Ring Oscillator Count Pair 6970
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX rocount70 : 16;
    //
    // LP4 PT Ring Oscillator Count Pair 6970
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rocount69 : 16;
  } Bits;
  UINTX Data;
} PTROCNT6970_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DBREGIONCTL1_DQDPLVUG_BOXTYPE_REG                  0x00000094
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Dead Band Region Region 3 Marker
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dbregion3_marker : 8;
    //
    // LP4 PT Dead Band Region Region 2 Marker
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dbregion2_marker : 8;
    //
    // LP4 PT Dead Band Region Region 1 Marker
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX dbregion1_marker : 8;
    //
    // LP4 PT Dead Band Region MAXPI
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX maxpi_per2xclk : 8;
  } Bits;
  UINTX Data;
} DBREGIONCTL1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DBREGIONCTL2_DQDPLVUG_BOXTYPE_REG                  0x00000098
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Dead Band Region Region 7 Marker
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dbregion7_marker : 8;
    //
    // LP4 PT Dead Band Region Region 6 Marker
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dbregion6_marker : 8;
    //
    // LP4 PT Dead Band Region Region 5 Marker
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX dbregion5_marker : 8;
    //
    // LP4 PT Dead Band Region Region 4 Marker
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX dbregion4_marker : 8;
  } Bits;
  UINTX Data;
} DBREGIONCTL2_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DBREGIONCTL3_DQDPLVUG_BOXTYPE_REG                  0x0000009C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX rk0dbregion7_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX rk0dbregion6_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX rk0dbregion5_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[7:6], RW, default = 0x0
    //
    UINTX rk0dbregion4_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX rk0dbregion3_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX rk0dbregion2_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX rk0dbregion1_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Offset Control
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX rk0dbregion0_dyndqdboffset : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX rk0dbregion7_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX rk0dbregion6_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX rk0dbregion5_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[23:22], RW, default = 0x0
    //
    UINTX rk0dbregion4_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX rk0dbregion3_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX rk0dbregion2_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX rk0dbregion1_dyndqdbsel : 2;
    //
    // LP4 PT Dead Band Rk0 DB Control
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX rk0dbregion0_dyndqdbsel : 2;
  } Bits;
  UINTX Data;
} DBREGIONCTL3_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_OVR1_DQDPLVUG_BOXTYPE_REG              0x000000A0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // grp1 pull down override code
    //
    // Bits[6:0], RW, default = 0x0
    //
    UINTX lrcomp_pdgrp1_pd_override : 7;
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
    UINTX reserved2 : 1;
    //
    // grp1 pull up override code
    //
    // Bits[14:8], RW, default = 0x0
    //
    UINTX lrcomp_pdgrp1_pu_override : 7;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
    UINTX reserved1 : 1;
    //
    // grp0 pull down override code
    //
    // Bits[22:16], RW, default = 0x0
    //
    UINTX lrcomp_pdgrp0_pd_override : 7;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
    UINTX reserved0 : 1;
    //
    // grp0 pull up override code
    //
    // Bits[30:24], RW, default = 0x0
    //
    UINTX lrcomp_pdgrp0_pu_override : 7;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
    UINTX reserved : 1;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_OVR1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_PUPDOVR_DQDPLVUG_BOXTYPE_REG           0x000000AC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Pull up override enable
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX lrcomppu_grp_override_en : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
    UINTX reserved0 : 2;
    //
    // Pull down override enable
    //
    // Bits[13:8], RW, default = 0x0
    //
    UINTX lrcomppd_grp_override_en : 6;
    //
    // Reserved bits
    //
    // Bits[31:14], RO, default = 0x0
    //
    UINTX reserved : 18;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_PUPDOVR_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_CODES_0_DQDPLVUG_BOXTYPE_REG           0x000000B0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PU / PD Code
    //
    // Bits[7:0], RO/V, default = 0x8
    //
    UINTX lrcomp_pd_code_grp1 : 8;
    //
    // PU / PD Code
    //
    // Bits[15:8], RO/V, default = 0x7
    //
    UINTX lrcomp_pu_code_grp1 : 8;
    //
    // PU / PD Code
    //
    // Bits[23:16], RO/V, default = 0x0
    //
    UINTX lrcomp_pd_code_grp0 : 8;
    //
    // PU / PD Code
    //
    // Bits[31:24], RO/V, default = 0x0
    //
    UINTX lrcomp_pu_code_grp0 : 8;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_CODES_0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_CFG2_DQDPLVUG_BOXTYPE_REG              0x000000E0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // indicates the division ratio of the c73p1ddrlp34l64x1top_comp clock w.r.t spid
    // clk
    //
    // Bits[1:0], RW, default = 0x1
    //
    UINTX lrcomp_msgbus_clk_div : 2;
    //
    // set it to '1' to select the membus form gcomp, '0' to select it from previous
    // module.
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX lcomp_sel_membus_frm_gcomp : 1;
    //
    // Rcomp sample time 3
    //
    // Bits[10:3], RW, default = 0xE
    //
    UINTX lrcomp_sampdel3 : 8;
    //
    // Rcomp sample time 2
    //
    // Bits[18:11], RW, default = 0xE
    //
    UINTX lrcomp_sampdel2 : 8;
    //
    // Rcomp sample time 1
    //
    // Bits[26:19], RW, default = 0xE
    //
    UINTX lrcomp_sampdel1 : 8;
    //
    // Reserved bits
    //
    // Bits[31:27], RO, default = 0x0
    //
    UINTX reserved : 5;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_CFG2_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_OVR0_DQDPLVUG_BOXTYPE_REG              0x000000E8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // not used
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX lrcomp_pd_puref_override_en : 1;
    //
    // not used
    //
    // Bits[7:1], RW, default = 0x0
    //
    UINTX lrcomp_pd_puref_override : 7;
    //
    // Reserved bits
    //
    // Bits[13:8], RO, default = 0x0
    //
    UINTX reserved0 : 6;
    //
    // override enable for override for pull down live rcomp code going to AFE
    //
    // Bits[14:14], RW, default = 0x0
    //
    UINTX lrcomp_pd_live_override_en : 1;
    //
    // override for pull down live rcomp code going to AFE
    //
    // Bits[21:15], RW, default = 0x1
    //
    UINTX lrcomp_pd_live_override : 7;
    //
    // Reserved bits
    //
    // Bits[31:22], RO, default = 0x0
    //
    UINTX reserved : 10;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_OVR0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_CTL0_DQDPLVUG_BOXTYPE_REG              0x000000EC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // config contorl for disabling rcomp code per group.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX lrcomp_grp_update_disable : 6;
    //
    // enable for config concifg control for updating rcomp codes.
    //
    // Bits[6:6], RW, default = 0x0
    //
    UINTX lrcomp_updtbyen : 1;
    //
    // config control for updating rcomp codes.
    //
    // Bits[7:7], RW, default = 0x0
    //
    UINTX lrcomp_update : 1;
    //
    // no of cycles needed for code update.
    //
    // Bits[11:8], RW, default = 0x8
    //
    UINTX lrcomp_update_len : 4;
    //
    // no of cycles needed for stage 2 update.
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX lrcomp_stg2_width : 4;
    //
    // not used.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX lrcomp_general_timer : 6;
    //
    // Reserved bits
    //
    // Bits[22:22], RO, default = 0x0
    //
    UINTX reserved0 : 1;
    //
    // static leg signal for PD c73p1ddrlp34l64x1top_comp buffer.
    //
    // Bits[23:23], RW, default = 0x1
    //
    UINTX lrcomp_pu_static : 1;
    //
    // static live signal for PD c73p1ddrlp34l64x1top_comp buffer.
    //
    // Bits[24:24], RW, default = 0x1
    //
    UINTX lrcomp_pd_staticlive : 1;
    //
    // selects the bus_quiet_time_gnt signal for lcomp buffer.
    //
    // Bits[26:25], RW, default = 0x1
    //
    UINTX sel_bqt_for_lcomp : 2;
    //
    // Reserved bits
    //
    // Bits[31:27], RO, default = 0x0
    //
    UINTX reserved : 5;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_CTL0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_CTL1_DQDPLVUG_BOXTYPE_REG              0x000000F0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // enable indirect update, when '1', it enables indirect update for respective group,
    // when '0', it disables indirect update for that group.
    //
    // Bits[5:0], RW, default = 0x3F
    //
    UINTX lrcomp_en_indirect_update : 6;
    //
    // tells the rcomp which groups are valid.
    //
    // Bits[11:6], RW, default = 0x3F
    //
    UINTX lrcomp_pdcode_valid_bits : 6;
    //
    // tells the rcomp to copy the adjecent code instead of evaluating it
    //
    // Bits[16:12], RW, default = 0x0
    //
    UINTX lcomp_pdcode_copy_bits : 5;
    //
    // Reserved bits
    //
    // Bits[31:17], RO, default = 0x0
    //
    UINTX reserved : 15;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_CTL1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_RSTCTL_DQDPLVUG_BOXTYPE_REG            0x000000F4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // active low reset for lcomp.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX lcomprst_b : 1;
    //
    // Reserved bits
    //
    // Bits[31:1], RO, default = 0x0
    //
    UINTX reserved : 31;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_RSTCTL_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_TXDLYCOMP_CLKGATE_DQDPLVUG_BOXTYPE_REG        0x000000F8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Clock gating control for txdlycomp
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX ltxdlycomp_clkgate_en : 1;
    //
    // Clock gating control for rcomp.
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX lrcomp_clkgate_en : 1;
    //
    // Reserved bits
    //
    // Bits[31:2], RO, default = 0x0
    //
    UINTX reserved : 30;
  } Bits;
  UINTX Data;
} DPIC_TXDLYCOMP_CLKGATE_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_TXDLYCOMP_INIT_DQDPLVUG_BOXTYPE_REG           0x000000FC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // enable initial txdlycomp
    //
    // Bits[0:0], RW/1S/V, default = 0x0
    //
    UINTX ltxdlycomp_init_comp : 1;
    //
    // Reserved bits
    //
    // Bits[31:1], RO, default = 0x0
    //
    UINTX reserved : 31;
  } Bits;
  UINTX Data;
} DPIC_TXDLYCOMP_INIT_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LCOMP_READBACK_DQDPLVUG_BOXTYPE_REG           0x00000104
#ifndef ASM_INC
typedef union {
  struct {
    //
    // read back value of txdlycomp limit error set flag
    //
    // Bits[0:0], RO/V, default = 0x0
    //
    UINTX ltxdlycomp_limit_hit_error_set : 1;
    //
    // read back value from AFE
    //
    // Bits[4:1], RO/V, default = 0x0
    //
    UINTX lcomp_cmn_obonus : 4;
    //
    // Reserved bits
    //
    // Bits[31:5], RO, default = 0x0
    //
    UINTX reserved : 27;
  } Bits;
  UINTX Data;
} DPIC_LCOMP_READBACK_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LRCOMP_CTL2_DQDPLVUG_BOXTYPE_REG              0x00000108
#ifndef ASM_INC
typedef union {
  struct {
    //
    // For future designs, where there might be a need to generate a stage3 enable for
    // AFE. It needs to be '1' otherwise.
    //
    // Bits[3:0], RW, default = 0x1
    //
    UINTX lrcomp_enable_width : 4;
    //
    // For future designs, where there might be a need to generate a stage3 enable for
    // AFE. It needs to be '1' otherwise.
    //
    // Bits[7:4], RW, default = 0x1
    //
    UINTX lrcomp_hold_width : 4;
    //
    // For future designs, where there might be a need to generate a stage3 enable for
    // AFE. It needs to be '1' otherwise.
    //
    // Bits[11:8], RW, default = 0x1
    //
    UINTX lrcomp_setup_width : 4;
    //
    // the number of cycles after lcomp receives the bus_quiet_time_gnt signal, and before
    // it takes any action. This allows the phy to finish off any running read/write
    // cycles
    //
    // Bits[18:12], RW, default = 0x1
    //
    UINTX lrcomp_bqt_settle_width : 7;
    //
    // Reserved bits
    //
    // Bits[31:19], RO, default = 0x0
    //
    UINTX reserved : 13;
  } Bits;
  UINTX Data;
} DPIC_LRCOMP_CTL2_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_TXDLYCOMP_0_DQDPLVUG_BOXTYPE_REG              0x00000110
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved bits
    //
    // Bits[0:0], RO, default = 0x0
    //
    UINTX reserved1 : 1;
    //
    // When set, Tx Delay c73p1ddrlp34l64x1top_Comp evaluation is restricted to only
    // one group
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX ltxdlycomp_1group : 1;
    //
    // continuous enable mode for txdlycomp
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX ltxdlycomp_continuous_en : 1;
    //
    // Reserved bits
    //
    // Bits[3:3], RO, default = 0x0
    //
    UINTX reserved0 : 1;
    //
    // read back value of txdlycomp limit error set flag
    //
    // Bits[4:4], RW, default = 0x0
    //
    UINTX ltxdlycomp_limit_hit_error : 1;
    //
    // config value for grp0 fine code.
    //
    // Bits[12:5], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp0_fine_override : 8;
    //
    // config value for grp0 coarse code.
    //
    // Bits[20:13], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp0_coarse_override : 8;
    //
    // bits for override control of grp0 code
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp0_override_en : 1;
    //
    // bits to disable for group0 code update.
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp0_update_disable : 1;
    //
    // select the delay tap node in AFE for group0
    //
    // Bits[27:23], RW, default = 0x6
    //
    UINTX ltxdlycomp_grp0_tapsel : 5;
    //
    // enable for config update contorl for txdlycomp code.
    //
    // Bits[28:28], RW, default = 0x0
    //
    UINTX ltxdlycomp_updtbyen : 1;
    //
    // config update contorl for txdlyocmp code
    //
    // Bits[29:29], RW, default = 0x0
    //
    UINTX ltxdlycomp_update : 1;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
    UINTX reserved : 2;
  } Bits;
  UINTX Data;
} DPIC_TXDLYCOMP_0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_TXDLYCOMP_1_DQDPLVUG_BOXTYPE_REG              0x00000114
#ifndef ASM_INC
typedef union {
  struct {
    //
    // config value for grp1 fine code.
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp1_fine_override : 8;
    //
    // config value for grp1 coarse code.
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp1_coarse_override : 8;
    //
    // bits for override control of grp1 code
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp1_override_en : 1;
    //
    // bits to disable for group1 code update.
    //
    // Bits[17:17], RW, default = 0x0
    //
    UINTX ltxdlycomp_grp1_update_disable : 1;
    //
    // select the delay tap node in AFE for group1
    //
    // Bits[22:18], RW, default = 0x6
    //
    UINTX ltxdlycomp_grp1_tapsel : 5;
    //
    // Reserved bits
    //
    // Bits[31:23], RO, default = 0x0
    //
    UINTX reserved : 9;
  } Bits;
  UINTX Data;
} DPIC_TXDLYCOMP_1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LTXDLYCOMP_CODES_0_DQDPLVUG_BOXTYPE_REG       0x00000120
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQS fine code value.
    //
    // Bits[7:0], RO/V, default = 0x8
    //
    UINTX ltxdlycomp_fine_code_grp1 : 8;
    //
    // DQS coarse code value.
    //
    // Bits[15:8], RO/V, default = 0x7
    //
    UINTX ltxdlycomp_coarse_code_grp1 : 8;
    //
    // DQ fine code value.
    //
    // Bits[23:16], RO/V, default = 0x0
    //
    UINTX ltxdlycomp_fine_code_grp0 : 8;
    //
    // DQ coarse code value.
    //
    // Bits[31:24], RO/V, default = 0x0
    //
    UINTX ltxdlycomp_coarse_code_grp0 : 8;
  } Bits;
  UINTX Data;
} DPIC_LTXDLYCOMP_CODES_0_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC

#define DPIC_LTXDLYCOMP_CODES_1_DQDPLVUG_BOXTYPE_REG       0x00000124
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Unused - Inputs for readout are tied to 'h0
    //
    // Bits[7:0], RO/V, default = 0x8
    //
    UINTX ltxdlycomp_fine_code_grp3 : 8;
    //
    // Unused - Inputs for readout are tied to 'h0
    //
    // Bits[15:8], RO/V, default = 0x7
    //
    UINTX ltxdlycomp_coarse_code_grp3 : 8;
    //
    // Unused - Inputs for readout are tied to 'h0
    //
    // Bits[23:16], RO/V, default = 0x0
    //
    UINTX ltxdlycomp_fine_code_grp2 : 8;
    //
    // Unused - Inputs for readout are tied to 'h0
    //
    // Bits[31:24], RO/V, default = 0x0
    //
    UINTX ltxdlycomp_coarse_code_grp2 : 8;
  } Bits;
  UINTX Data;
} DPIC_LTXDLYCOMP_CODES_1_DQDPLVUG_BOXTYPE_STRUCT;
#endif // ASM_INC


#endif // _DQDPLVUG_BOXTYPE_H_