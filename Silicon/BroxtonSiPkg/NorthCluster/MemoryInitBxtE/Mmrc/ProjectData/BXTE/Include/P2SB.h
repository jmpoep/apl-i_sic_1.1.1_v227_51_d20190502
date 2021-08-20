/** @file
  File name:    P2SB.h
  Input File:   BXTE_SOC.xlsx

  THIS FILE IS AUTO-GENERATED BY THE MMRC TOOL. DO NOT CHANGE THIS CODE.

  If edits are needed in this file, they must be done via the MMRC tool.

  If there is additional project-specific data required by the MMRC, it
  can be placed in P2SB.h, which is used for non-tool-generated
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
#ifndef _P2SB_H_
#define _P2SB_H_

#include "DataTypes.h"

#if USE_64_BIT_VARIABLES
#define UINTX UINT64
#else
#define UINTX UINT32
#endif

#define PCIID_P2SB_REG                                     0x00000000
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Indicates Intel
    //
    // Bits[15:0], RO, default = 0x8086
    //
    UINTX vid : 16;
    //
    // The upper 8-bits of this field can be overridden by the SetID IOSF-SB Message.
    //
    // Bits[31:16], RO, default = 0xC5C5
    //
    UINTX did : 16;
  } Bits;
  UINTX Data;
} PCIID_P2SB_STRUCT;
#endif // ASM_INC

#define PCICMD_P2SB_REG                                    0x00000004
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Legacy regions are unaffected by this bit.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX iose : 1;
    //
    // Will control the P2SB acceptance of PCI MMIO BARs only. Other legacy regions are
    // unaffected by this bit.
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX mse : 1;
    //
    // Bus mastering cannot be disabled as this device acts as a proxy for non-PCI devices.
    //
    // Bits[2:2], RO, default = 0x1
    //
    UINTX bme : 1;
    //
    // Not applicable.
    //
    // Bits[3:3], RO, default = 0x0
    //
    UINTX sce : 1;
    //
    // Not applicable.
    //
    // Bits[4:4], RO, default = 0x0
    //
    UINTX mwie : 1;
    //
    // Not applicable.
    //
    // Bits[5:5], RO, default = 0x0
    //
    UINTX vga : 1;
    //
    // No support for parity error detection.
    //
    // Bits[6:6], RO, default = 0x0
    //
    UINTX per : 1;
    //
    // Reserved
    //
    // Bits[7:7], RO, default = 0x0
    //
    UINTX rsvd_1 : 1;
    //
    // P2SB does not issue SERR# on its own behalf.
    //
    // Bits[8:8], RO, default = 0x0
    //
    UINTX se : 1;
    //
    // Not applicable
    //
    // Bits[9:9], RO, default = 0x0
    //
    UINTX fb2be : 1;
    //
    // P2SB does not issue any interrupts on its own behalf
    //
    // Bits[10:10], RO, default = 0x0
    //
    UINTX intd : 1;
    //
    // Reserved
    //
    // Bits[15:11], RO, default = 0x0
    //
    UINTX rsvd : 5;
  } Bits;
  UINTX Data;
} PCICMD_P2SB_STRUCT;
#endif // ASM_INC

#define PCISTS_P2SB_REG                                    0x00000006
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved
    //
    // Bits[15:0], RO, default = 0x0
    //
    UINTX rsvd : 16;
  } Bits;
  UINTX Data;
} PCISTS_P2SB_STRUCT;
#endif // ASM_INC

#define PCIRID_P2SB_REG                                    0x00000008
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Indicates the part revision. This will reset to 0 but will overridden by the SetID
    // IOSFSB message during the power-up sequence
    //
    // Bits[7:0], RO, default = 0x0
    //
    UINTX rid : 8;
  } Bits;
  UINTX Data;
} PCIRID_P2SB_STRUCT;
#endif // ASM_INC

#define PCICC_P2SB_REG                                     0x00000009
#ifndef ASM_INC
typedef union {
  struct {
    //
    // No programming interface.
    //
    // Bits[7:0], RO, default = 0x0
    //
    UINTX pi : 8;
    //
    // Indicates an unspecified other memory controller.
    //
    // Bits[15:8], RO, default = 0x80
    //
    UINTX scc : 8;
    //
    // Indicates a memory controller device class.
    //
    // Bits[23:16], RO, default = 0x5
    //
    UINTX bcc : 8;
    //
    // Missing bits not found in register description spreadsheet.
    // Bits[31:24], Unknown, default = Unknown
    //
    UINTX missing_from_register_description : 8;
  } Bits;
  UINTX Data;
} PCICC_P2SB_STRUCT;
#endif // ASM_INC

#define PCIMLT_P2SB_REG                                    0x0000000D
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved
    //
    // Bits[7:0], RO, default = 0x0
    //
    UINTX rsvd : 8;
  } Bits;
  UINTX Data;
} PCIMLT_P2SB_STRUCT;
#endif // ASM_INC

#define PCIHTYPE_P2SB_REG                                  0x0000000E
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Indicates a generic device header.
    //
    // Bits[6:0], RO, default = 0x0
    //
    UINTX htype : 7;
    //
    // Indicates that this is part of a multi-function device.
    //
    // Bits[7:7], RO, default = 0x0
    //
    UINTX mfd : 1;
  } Bits;
  UINTX Data;
} PCIHTYPE_P2SB_STRUCT;
#endif // ASM_INC

#define SBREG_BAR_P2SB_REG                                 0x00000010
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Indicates memory space
    //
    // Bits[0:0], RO, default = 0x0
    //
    UINTX stype : 1;
    //
    // Indicates that this can be placed anywhere in 64b space.
    //
    // Bits[2:1], RO, default = 0x2
    //
    UINTX atype : 2;
    //
    // Indicates this is not prefetchable.
    //
    // Bits[3:3], RO, default = 0x0
    //
    UINTX pref : 1;
    //
    // Hardwired to 0 to request a BAR of 16MB
    //
    // Bits[23:4], RO, default = 0x0
    //
    UINTX hw202rb16mb : 20;
    //
    // Lower DWORD of the base address for the sideband register access BAR.
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX rba : 8;
  } Bits;
  UINTX Data;
} SBREG_BAR_P2SB_STRUCT;
#endif // ASM_INC

#define SBREG_BARH_P2SB_REG                                0x00000014
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Upper DWORD of the base address for the sideband register access BAR.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX rbah : 32;
  } Bits;
  UINTX Data;
} SBREG_BARH_P2SB_STRUCT;
#endif // ASM_INC

#define PCIHSS_P2SB_REG                                    0x0000002C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Written by BIOS. Not used by hardware.
    //
    // Bits[15:0], RW/O, default = 0x0
    //
    UINTX ssvid : 16;
    //
    // Written by BIOS. Not used by hardware.
    //
    // Bits[31:16], RW/O, default = 0x0
    //
    UINTX ssid : 16;
  } Bits;
  UINTX Data;
} PCIHSS_P2SB_STRUCT;
#endif // ASM_INC

#define VBDF_P2SB_REG                                      0x00000050
#ifndef ASM_INC
typedef union {
  struct {
    //
    // N/A
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX func : 3;
    //
    // N/A
    //
    // Bits[7:3], RW, default = 0x1F
    //
    UINTX dev : 5;
    //
    // N/A
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX bus : 8;
  } Bits;
  UINTX Data;
} VBDF_P2SB_STRUCT;
#endif // ASM_INC

#define EBDF_P2SB_REG                                      0x00000052
#ifndef ASM_INC
typedef union {
  struct {
    //
    // N/A
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX func : 3;
    //
    // N/A
    //
    // Bits[7:3], RW, default = 0x1F
    //
    UINTX dev : 5;
    //
    // N/A
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX bus : 8;
  } Bits;
  UINTX Data;
} EBDF_P2SB_STRUCT;
#endif // ASM_INC

#define RCFG_P2SB_REG                                      0x00000054
#ifndef ASM_INC
typedef union {
  struct {
    //
    // When set, all IO writes to the RTC will be also sent to the PMC.  This allows
    // cases where the battery backed storage is in an external PMIC.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX rse : 1;
    //
    // N/A
    //
    // Bits[7:1], RW, default = 0x0
    //
    UINTX rsvd_1 : 7;
    //
    // Specifies the  IOSF-SB destination ID for sending Reserved Page Register cycles
    // (e.g. Port 80h).  By default this will load to the ID of the LPC or eSPI device
    // depending on which has been strapped active in the system.
    //
    // Bits[15:8], RW, default = 0xD2
    //
    UINTX rprid : 8;
    //
    // N/A
    //
    // Bits[31:16], RW, default = 0x0
    //
    UINTX rsvd : 16;
  } Bits;
  UINTX Data;
} RCFG_P2SB_STRUCT;
#endif // ASM_INC

#define HPTC_P2SB_REG                                      0x00000060
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This 2-bit field selects 1 of 4 possible memory address ranges for the High Performance
    // Timer functionality. The encodings are:
    // 00 : FED0_0000h - FED0_03FFFh
    // 01 : FED0_1000h - FED0_13FFFh
    // 10 : FED0_2000h - FED0_23FFFh
    // 11 : FED0_3000h - FED0_33FFFh
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX as : 2;
    //
    // N/A
    //
    // Bits[6:2], RO, default = 0x0
    //
    UINTX rsvd : 5;
    //
    // When set, the P2SB will decode the High Performance Timer memory address range
    // selected by bits 1:0 below.
    //
    // Bits[7:7], RW, default = 0x0
    //
    UINTX ae : 1;
  } Bits;
  UINTX Data;
} HPTC_P2SB_STRUCT;
#endif // ASM_INC

#define IOAC_P2SB_REG                                      0x00000064
#ifndef ASM_INC
typedef union {
  struct {
    //
    // These bits define address bits 19:12 for the IOxAPIC range. The default value
    // of 00h enables compatibility with prior products as an initial value. This value
    // must not be changed unless the IOxAPIC Enable bit is cleared.
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX asel : 8;
    //
    // When set, the P2SB will decode the IOxAPIC memory address range selected by bits
    // 7:0 below.
    //
    // Bits[8:8], RW, default = 0x0
    //
    UINTX ae : 1;
    //
    // N/A
    //
    // Bits[15:9], RO, default = 0x0
    //
    UINTX rsvd : 7;
  } Bits;
  UINTX Data;
} IOAC_P2SB_STRUCT;
#endif // ASM_INC

#define IBDF_P2SB_REG                                      0x0000006C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // N/A
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX func : 3;
    //
    // N/A
    //
    // Bits[7:3], RW, default = 0x1F
    //
    UINTX dev : 5;
    //
    // N/A
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX bus : 8;
  } Bits;
  UINTX Data;
} IBDF_P2SB_STRUCT;
#endif // ASM_INC

#define HBDF_P2SB_REG                                      0x00000070
#ifndef ASM_INC
typedef union {
  struct {
    //
    // N/A
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX func : 3;
    //
    // N/A
    //
    // Bits[7:3], RW, default = 0x1F
    //
    UINTX dev : 5;
    //
    // N/A
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX bus : 8;
  } Bits;
  UINTX Data;
} HBDF_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED0_P2SB_REG                              0x00000080
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 31-0.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted0 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED0_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED1_P2SB_REG                              0x00000084
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 63-32.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted1 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED1_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED2_P2SB_REG                              0x00000088
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 95-64.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted2 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED2_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED3_P2SB_REG                              0x0000008C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 127-96.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted3 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED3_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED4_P2SB_REG                              0x00000090
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 159-128.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted4 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED4_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED5_P2SB_REG                              0x00000094
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 191-160.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted5 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED5_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED6_P2SB_REG                              0x00000098
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 223-192.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted6 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED6_P2SB_STRUCT;
#endif // ASM_INC

#define SBREGPOSTED7_P2SB_REG                              0x0000009C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for setting SBREG to posted posted for IOSF-SB endpoint IDs 255-224.
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX sbregposted7 : 32;
  } Bits;
  UINTX Data;
} SBREGPOSTED7_P2SB_STRUCT;
#endif // ASM_INC

#define DISPBDF_P2SB_REG                                   0x000000A0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // The function number of the Display in the CPU Complex
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX func : 3;
    //
    // The bus number of the Display in the CPU Complex.
    //
    // Bits[7:3], RW, default = 0x2
    //
    UINTX dev : 5;
    //
    // The bus number of the Display in the CPU Complex.
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX bus : 8;
    //
    // This register contains the Target BLK field that will be used when sending RAVDM
    // messages to the CPU Complex North Display.
    //
    // Bits[18:16], RW, default = 0x6
    //
    UINTX dtblk : 3;
    //
    // N/A
    //
    // Bits[31:19], RO, default = 0x0
    //
    UINTX rsvd : 13;
  } Bits;
  UINTX Data;
} DISPBDF_P2SB_STRUCT;
#endif // ASM_INC

#define ICCOS_P2SB_REG                                     0x000000A4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This specifies the upper 8b for the 16b address that will be used for sending
    // RAVDM access that target the Buffer range of the ICC (FFE00h - FFEFFh).
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX bufbase : 8;
    //
    // This specifies the upper 8b for the 16b address that will be used for sending
    // RAVDM access that target the Modulator Control range of the ICC (FFF00h - FFFFFh).
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX modbase : 8;
  } Bits;
  UINTX Data;
} ICCOS_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK0_P2SB_REG                                   0x000000B0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 31-0.
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask0 : 32;
  } Bits;
  UINTX Data;
} EPMASK0_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK1_P2SB_REG                                   0x000000B4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 63-32.
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask1 : 32;
  } Bits;
  UINTX Data;
} EPMASK1_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK2_P2SB_REG                                   0x000000B8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 95-64
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask2 : 32;
  } Bits;
  UINTX Data;
} EPMASK2_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK3_P2SB_REG                                   0x000000BC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 127-96
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask3 : 32;
  } Bits;
  UINTX Data;
} EPMASK3_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK4_P2SB_REG                                   0x000000C0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 128-159
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask4 : 32;
  } Bits;
  UINTX Data;
} EPMASK4_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK5_P2SB_REG                                   0x000000C4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 191-160
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask5 : 32;
  } Bits;
  UINTX Data;
} EPMASK5_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK6_P2SB_REG                                   0x000000C8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 223-192
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask6 : 32;
  } Bits;
  UINTX Data;
} EPMASK6_P2SB_STRUCT;
#endif // ASM_INC

#define EPMASK7_P2SB_REG                                   0x000000CC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // One hot masks for disabling IOSF-SB endpoint IDs 255-224
    //
    // Bits[31:0], RW/L, default = 0x0
    //
    UINTX epmask7 : 32;
  } Bits;
  UINTX Data;
} EPMASK7_P2SB_STRUCT;
#endif // ASM_INC

#define SBIADDR_P2SB_REG                                   0x000000D0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Register address offset.  The content of this register field is sent in the IOSF
    // Sideband Message Register Access address(15:0) field.
    //
    // Bits[15:0], RW, default = 0x0
    //
    UINTX offset : 16;
    //
    // Destination IOSF-SB Root Space.
    // *Note: This register may only be written during manufacturing test.  P2SB will
    // only accept writes to this register from transactions with a SAI equal to the
    // SBI_RS_ACCESS_SAI parameter.  This should be assigned to the SAI used by the functional
    // test module (typically TAM) that will perform this register write on IOSF-P.
    //
    // Bits[19:16], RW, default = 0x0
    //
    UINTX rs : 4;
    //
    // N/A
    //
    // Bits[23:20], RO, default = 0x0
    //
    UINTX rsvd : 4;
    //
    // The content of this register field is sent in the IOSF Sideband Message Register
    // Access dest field.
    // See the IOSF Sideband C-spec chapter for Destination ID assignments.
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX destid : 8;
  } Bits;
  UINTX Data;
} SBIADDR_P2SB_STRUCT;
#endif // ASM_INC

#define SBIDATA_P2SB_REG                                   0x000000D4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // The content of this register field is sent on the IOSF sideband Message Register
    // Access data(31:0) field.
    //
    // Bits[31:0], RW/V, default = 0x0
    //
    UINTX data : 32;
  } Bits;
  UINTX Data;
} SBIDATA_P2SB_STRUCT;
#endif // ASM_INC

#define SBISTAT_P2SB_REG                                   0x000000D8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // 0:  The IOSF sideband interface is ready for a new transaction
    // 1:  The IOSF sideband interface is busy with the previous transaction.
    // A write to set this register bit to 1 will trigger an IOSF sideband message on
    // the private IOSF sideband interface. The message will be formed based on the values
    // programmed in the Sideband Message Interface Register Access registers.
    // Software needs to ensure that the interface is not busy (SBISTAT.INITRDY is clear)
    // before writing to this register.
    //
    // Bits[0:0], RW/1S, default = 0x0
    //
    UINTX initrdy : 1;
    //
    // 00 - Successful
    // 01 - Unsuccessful / Not Supported
    // 10 - Powered Down
    // 11 - Multi-cast Mixed
    //
    // This register reflects the response status for the previously completed transaction.
    //  The value of this register is only meaningful if SBISTAT.INITRDY is zero.
    //
    // Bits[2:1], RO/V, default = 0x0
    //
    UINTX response : 2;
    //
    // N/A
    //
    // Bits[6:3], RO, default = 0x0
    //
    UINTX rsvd : 4;
    //
    // When set to 1, the message will be send as a posted message instead of non-posted.
    //  This should only be used if the receiver is known to support posted operations
    // for the specified operation.
    //
    // Bits[7:7], RW, default = 0x0
    //
    UINTX posted : 1;
    //
    // This is the Opcode sent in the IOSF sideband message.
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX opcode : 8;
  } Bits;
  UINTX Data;
} SBISTAT_P2SB_STRUCT;
#endif // ASM_INC

#define SBIRID_P2SB_REG                                    0x000000DA
#ifndef ASM_INC
typedef union {
  struct {
    //
    // The contents of this field are sent in the IOSF Sideband Register access FID field.
    // This field should generally remain at zero unless specifically required by a particular
    // application.
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX fid : 8;
    //
    // The contents of this field are sent in the IOSF Sideband Register Access BAR field.
    //  This should be zero performing a Memory Mapped operation to a PCI compliant device.
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX bar : 3;
    //
    // N/A
    //
    // Bits[11:11], RO, default = 0x0
    //
    UINTX rsvd : 1;
    //
    // The content of this field is sent in the IOSF Sideband Register Access FBE field.
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX fbe : 4;
  } Bits;
  UINTX Data;
} SBIRID_P2SB_STRUCT;
#endif // ASM_INC

#define SBIEXTADDR_P2SB_REG                                0x000000DC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // The content of this register field is sent on the IOSF sideband Message Register
    // Access address(48:32) field.
    // This must be set to all 0 if 16b addressing is desired.
    //
    // Bits[31:0], RW/V, default = 0x0
    //
    UINTX addr : 32;
  } Bits;
  UINTX Data;
} SBIEXTADDR_P2SB_STRUCT;
#endif // ASM_INC

#define P2SBC_P2SB_REG                                     0x000000E0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This controls the max number of outstanding writes on IOSF-SB initiated by MMIO
    // writes to the SBREG_BAR.
    // Once the number of SBREG_BAR writes issued but not completed on IOSF-SB is equal
    // to this value no new requests will be forwarded until completions are received.
    //
    // A value of zero will have the same behavior as the value of one (single write
    // outstanding).
    //
    // Bits[2:0], RW, default = 0x7
    //
    UINTX maxw : 3;
    //
    // N/A
    //
    // Bits[7:3], RW, default = 0x0
    //
    UINTX rsvd_2 : 5;
    //
    // When this bit is set, the P2SB will return 1s on any PCI Configuration Read on
    // IOSF-P.  All other transactions including PCI Configuration Writes are unaffected
    // by this.  This does not affect reads performed on the IOSF-SB interface.
    //
    // Bits[8:8], RW, default = 0x0
    //
    UINTX hide : 1;
    //
    // Hardware will not allocate these bits for any purpose in the future so that BIOS
    // can always write to the HIDE field with this byte enabled without having to remember
    // any other field values since it will not be possible to do a read-modify-write
    // when the device is hidden.
    //
    // Bits[15:9], RW, default = 0x0
    //
    UINTX rsvd_1 : 7;
    //
    // When asserted the P2SB can de-assert the clock request to disable the PGCB clock
    // dynamically when it reaches the power down idle state.
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX pgcbcge : 1;
    //
    // Locks the value of the EPMASK[0-7] registers.  Once this value is written to a
    // one it may only be cleared by a reset.
    //
    // Bits[17:17], RW/O, default = 0x0
    //
    UINTX masklock : 1;
    //
    // N/A
    //
    // Bits[31:18], RW, default = 0x0
    //
    UINTX rsvd : 14;
  } Bits;
  UINTX Data;
} P2SBC_P2SB_STRUCT;
#endif // ASM_INC

#define PCE_P2SB_REG                                       0x000000E4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // When set to 1, the P2SB will engage power gating if it is idle and the pmc_p2sb_sw_pg_req_b
    // signal is asserted.
    //
    // Bits[0:0], RW, default = 0x1
    //
    UINTX pmcpg_en : 1;
    //
    // No support for S0i3 power gating.
    //
    // Bits[1:1], RO, default = 0x0
    //
    UINTX i3e : 1;
    //
    // No support for D3 Hot power gating.
    //
    // Bits[2:2], RO, default = 0x0
    //
    UINTX d3he : 1;
    //
    // N/A
    //
    // Bits[4:3], RO, default = 0x0
    //
    UINTX rsvd_1 : 2;
    //
    // When set, the P2SB will automatically engage power gating when it has reached
    // its idle condition.
    //
    // Bits[5:5], RW, default = 0x0
    //
    UINTX hae : 1;
    //
    // N/A
    //
    // Bits[7:6], RO, default = 0x0
    //
    UINTX rsvd : 2;
  } Bits;
  UINTX Data;
} PCE_P2SB_STRUCT;
#endif // ASM_INC

#define PGCBT_P2SB_REG                                     0x000000E6
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Value representing the minimum number of pgcb_clk cyles required between the assertion
    // of pgcb_sleep (and the deassertion of pgcb_isol_latchen) to the assertion of pgcb_isol_en_b.
    // 00:  1 clock
    // 01:  2 clocks (default)
    // 10:  8 clocks
    // 11:  256 clocks
    //
    //
    // Bits[1:0], RW, default = 0x1
    //
    UINTX tsleepact : 2;
    //
    // alue representing the minimum number of pgcb_clk cyles required between the assertion
    // of pgcb_isol_en_b to the assertion of pgcb_force_prim_rst_b and pgcb_force_rst_b.
    // 00:  1 clock
    // 01:  2 clocks (default)
    // 10:  8 clocks
    // 11:  256 clocks
    //
    //
    // Bits[3:2], RW, default = 0x1
    //
    UINTX tisolate : 2;
    //
    // Value representing the minimum number of delay clocks required between the assertion
    // of pgcb_force_prim_rst_b (and pgcb_force_rst_b) to either the assertion of pgcb_pmc_pg_req_b
    // for state retention or pgcb_sleep for non-retention.
    // 00:  1 clock
    // 01:  2 clocks (default)
    // 10:  8 clocks
    // 11:  256 clocks
    //
    //
    // Bits[5:4], RW, default = 0x1
    //
    UINTX tresetact : 2;
    //
    // Value representing the minimum number of delay clocks required between the deassertion
    // of pgcb_sleep to the deassertion of either pgcb_ip_*_lock_req_b from state retention
    // or pgcb_isol_en_b from a non-retention state.
    // 00:  1 clock
    // 01:  2 clocks (default)
    // 10:  8 clocks
    // 11:  256 clocks
    //
    //
    // Bits[7:6], RW, default = 0x1
    //
    UINTX tslpinactv : 2;
    //
    // Value representing the minimum number of delay clocks required between the deassertion
    // of pgcb_isol_en_b until the restore flow begins.
    //                00:  1 clock
    //                01:  2 clocks (default)
    //                10:  8 clocks
    //                11:  256 clocks
    //
    //
    // Bits[9:8], RW, default = 0x1
    //
    UINTX tdeisolate : 2;
    //
    // Value representing the number of delay clocks required between the deassertion
    // of pgcb_force_prim_rst_b and the next state in the FSM (varies).
    //                00:  1 clock
    //                01:  2 clocks (default)
    //                10:  8 clocks
    //                11:  256 clocks
    //
    //
    // Bits[11:10], RW, default = 0x1
    //
    UINTX tirstup : 2;
    //
    // N/A
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX rsvd : 4;
  } Bits;
  UINTX Data;
} PGCBT_P2SB_STRUCT;
#endif // ASM_INC

#define PDOMAIN_P2SB_REG                                   0x000000E8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // When set to 1, the local clock gating for the IOSF-P clock domain within the P2SB
    // will be disabled.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX cgd : 1;
    //
    // N/A
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX ckreqd : 1;
    //
    // N/A
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX rsvd : 2;
    //
    // Defines the amount of idle time required before local clock gating will be engaged
    // (if enabled).  This field defines the exponent such that the actual delay = 2^
    // CGATEHO.
    //
    // Bits[7:4], RW, default = 0x3
    //
    UINTX cgateho : 4;
    //
    // Defines the amount of idle time required between local clock gating and locking
    // for power gate preparation (if power gating is enabled).  This field defines the
    // exponent such that the actual delay = 2^ LOCKHO.
    //
    // Bits[11:8], RW, default = 0x3
    //
    UINTX lockho : 4;
    //
    // Defines the amount of idle time required between locking for power gate preparation
    // and deassertion of the prim_clkreq signal.  This field defines the exponent such
    // that the actual delay = 2^ CKREQHO.
    //
    // Bits[15:12], RW, default = 0x3
    //
    UINTX ckreqho : 4;
  } Bits;
  UINTX Data;
} PDOMAIN_P2SB_STRUCT;
#endif // ASM_INC

#define SDOMAIN_P2SB_REG                                   0x000000EA
#ifndef ASM_INC
typedef union {
  struct {
    //
    // When set to 1, the local clock gating for the IOSF-P clock domain within the P2SB
    // will be disabled.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX cgd : 1;
    //
    // N/A
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX ckreqd : 1;
    //
    // N/A
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX rsvd : 2;
    //
    // Defines the amount of idle time required before local clock gating will be engaged
    // (if enabled).  This field defines the exponent such that the actual delay = 2^
    // CGATEHO.
    //
    // Bits[7:4], RW, default = 0x3
    //
    UINTX cgateho : 4;
    //
    // Defines the amount of idle time required between local clock gating and locking
    // for power gate preparation (if power gating is enabled).  This field defines the
    // exponent such that the actual delay = 2^ PLOCKHO.
    //
    // Bits[11:8], RW, default = 0x3
    //
    UINTX lockho : 4;
    //
    // Defines the amount of idle time required between locking for power gate preparation
    // and deassertion of the prim_clkreq signal.  This field defines the exponent such
    // that the actual delay = 2^ SCKREQHO.
    //
    // Bits[15:12], RW, default = 0x3
    //
    UINTX ckreqho : 4;
  } Bits;
  UINTX Data;
} SDOMAIN_P2SB_STRUCT;
#endif // ASM_INC

#define MANID_P2SB_REG                                     0x000000F8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Indicates the process.  The dot portion of the process is reflected in bits (27:24)
    //
    // Bits[7:0], RO, default = 0x0
    //
    UINTX proc : 8;
    //
    // 0Fh = Intel
    //
    // Bits[15:8], RO, default = 0xF
    //
    UINTX mid : 8;
    //
    // This field is incremented for each stepping of the part.
    //
    // Note that this field can be used by software to differentiate steppings when the
    // Stepping Revision ID may not change.
    //
    // Bits[23:16], RO, default = 0x0
    //
    UINTX msid : 8;
    //
    // Indicates the dot process
    //
    // Bits[27:24], RO, default = 0x0
    //
    UINTX dot : 4;
    //
    // N/A
    //
    // Bits[31:28], RO, default = 0x0
    //
    UINTX rsvd : 4;
  } Bits;
  UINTX Data;
} MANID_P2SB_STRUCT;
#endif // ASM_INC

#define BASE_ADDRESS_LO_P2SB_REG                           0x00000010
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Missing bits not found in register description spreadsheet.
    // Bits[23:0], Unknown, default = Unknown
    //
    UINTX missing_from_register_description : 24;
    //
    // Base address for P2SB
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX baseaddress : 8;
  } Bits;
  UINTX Data;
} BASE_ADDRESS_LO_P2SB_STRUCT;
#endif // ASM_INC

#define BASE_ADDRESS_HI_P2SB_REG                           0x00000014
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Base address for P2SB
    //
    // Bits[31:0], RW, default = 0x0
    //
    UINTX baseaddress : 32;
  } Bits;
  UINTX Data;
} BASE_ADDRESS_HI_P2SB_STRUCT;
#endif // ASM_INC


#endif // _P2SB_H_