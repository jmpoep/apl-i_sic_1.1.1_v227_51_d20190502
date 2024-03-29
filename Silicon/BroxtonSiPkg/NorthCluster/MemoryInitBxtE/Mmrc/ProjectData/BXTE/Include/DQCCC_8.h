/** @file

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

@par Specification Reference:
**/
#ifndef _DQCCC_8_H_
#define _DQCCC_8_H_

#include "DataTypes.h"

#if USE_64_BIT_VARIABLES
#define UINTX UINT64
#else
#define UINTX UINT32
#endif

#define DIGCTL_DQCCC_8_REG                                0x00000000
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved bits
    //
    // Bits[1:0], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved503 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Single data rate v/s Double Data rate [br]0: Double data rate (LP3)[br]1: Single
    // data rate (LP4)
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX lpddr34mode : 1;
    //
    // SPID clk sel [br]00: 0.5xmode[br]01: 0.25x mode[br]10: 0.1x mode[br]11: NA
    //
    // Bits[4:3], RW, default = 0x0
    //
    UINTX spidclksel : 2;
    //
    // This is the burstlength configuration register[br]1:BL16 and BL32[br]0:BL4 and
    // BL8
    //
    // Bits[5:5], RW, default = 0x0
    //
    UINTX bl1632xbl48b : 1;
    //
    // Reserved bits
    //
    // Bits[8:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved502 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // Programmable MCP for Tx FIFO read pointer path.
    //
    // Bits[9:9], RW, default = 0x0
    //
    UINTX rdmcpsel : 1;
    //
    // This is the txFIFO final flop preset enable[br]1 - iobufact_b assert will preset
    // the final flop, otherwise resets the final flop
    //
    // Bits[10:10], RW, default = 0x0
    //
    UINTX txfifo_csn_ffloppreseten : 1;
    //
    // Delay the wrcmd 1 spidclk before write into the fifo.[br]1: 1 spidclk delay[br]0:
    // no delay[br]With clock gating enable, the write pointer is only valid @ the next
    // spid clock edge of write command. -> delay 1 spidclk so we can capture it into
    // the fifo.[br]Set to 1 if do clock gate. Else set it to 0.
    //
    // Bits[11:11], RW, default = 0x0
    //
    UINTX slowpath : 1;
    //
    // Reserved bits
    //
    // Bits[12:12], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved501 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // This is the stretch mode signal.[br]00- normal read[br]01- x4 stretch mode [br]10-
    // x8 stretch mode[br]11- x16 stretch mode
    //
    // Bits[14:13], RW, default = 0x0
    //
    UINTX stretchmode : 2;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved500 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // Delay between the phy update request to ack [br]in terms of SPID clocks.
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX phyupd_dly : 8;
    //
    // Memory frequency
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX memfreq : 4;
    //
    // Reserved bits
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX memtech : 4;
  } Bits;
  UINTX Data;
} DIGCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define FIFOPTRCTL_DQCCC_8_REG                            0x00000004
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the double FIFO offset configuration(in terms of 2xclock).
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX bl_doublefifooffset : 4;
    //
    // This is the double FIFO offset configuration(in terms of 2xclock).
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX cc_doublefifooffset : 4;
    //
    // This is the ref2x read pointer init configuration.
    //
    // Bits[12:8], RW, default = 0x0
    //
    UINTX ref2xrdptrinit : 5;
    //
    // This is the ref1xread pointer offset configuration.
    //
    // Bits[15:13], RW, default = 0x0
    //
    UINTX ref1xrdptroffset : 3;
    //
    // Configure DQCCC as DQ or CCC[br]0: CCC[br]1: DQ
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dqxccb : 1;
    //
    // This is the ref1x read pointer init configuration.
    //
    // Bits[20:17], RW, default = 0x0
    //
    UINTX ref1xrdptrinit : 4;
    //
    // DLL comp read pointer offset.
    //
    // Bits[23:21], RW, default = 0x0
    //
    UINTX compclkrdptroffset : 3;
    //
    // DLL comp init configuration.
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX compclkrdptrinit : 4;
    //
    // Reserved bits
    //
    // Bits[31:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved504 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} FIFOPTRCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CCCCTL_DQCCC_8_REG                                0x00000008
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Command tristate override select
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX cmdtristateovrsel : 1;
    //
    // Command tristate override value
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX cmdtristateovrval : 1;
    //
    // Start phase of CK
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX clknegxposb : 1;
    //
    // Reserved bits
    //
    // Bits[4:3], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved511 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[5:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved510 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[6:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved509 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved508 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // CMD buffer enable[br]1: Enable buffer[br]0: Tristate buffer
    //
    // Bits[17:8], RW, default = 0x0
    //
    UINTX cmdtxdrven : 10;
    //
    // CS buffer enable[br]1: Enable buffer[br]0: Tristate buffer
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX cstxdrven : 1;
    //
    // CKE buffer enable[br]1: Enable buffer[br]0: Tristate buffer
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX cketxdrven : 1;
    //
    // CK buffer enable[br]1: Enable buffer[br]0: Tristate buffer
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX clktxdrven : 1;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved507 : 3;
#else
    UINTX reserved_4 : 3;
#endif // MEM_SV_HOOKS
    //
    // CA training mode
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX ca_training : 1;
    //
    // Reserved bits
    //
    // Bits[27:25], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved506 : 3;
#else
    UINTX reserved_5 : 3;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved505 : 4;
#else
    UINTX reserved_6 : 4;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CCCCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LP4RSTCTL_DQCCC_8_REG                             0x0000000C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Set for the LP4 reset buffer
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX rstbtx_set : 1;
    //
    // Reset for the LP4 reset buffer
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX rstbtx_reset : 1;
    //
    // Tri-state control for LP4 reset buffer
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX rstbtx_enbrst : 1;
    //
    // Reserved bits
    //
    // Bits[31:3], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved512 : 29;
#else
    UINTX reserved : 29;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LP4RSTCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DIGRSTCTL_DQCCC_8_REG                             0x00000010
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reset for functional logic in upartition (except compensation and power management
    // logic)  [br]0: Reset assertion  [br]1: Reset de-assertion
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX spidrst_b : 1;
    //
    // Rxfifo reset (active low)[br]1: deasserted[br]0 : asserted
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX rxfiforst_b : 1;
    //
    // Reserved bits
    //
    // Bits[31:2], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved513 : 30;
#else
    UINTX reserved : 30;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DIGRSTCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define FIFOPTREN_DQCCC_8_REG                             0x00000014
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Config-controlled Write pointer enable. 0=config control disabled, 1=config control
    // enabled. This bit qualifies the pm_wrptren, which eventually goes to DQPTRGEN
    // Fub.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX wrptren : 1;
    //
    // Reserved bits
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved514 : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} FIFOPTREN_DQCCC_8_STRUCT;
#endif // ASM_INC

#define IOBUFACT_AFE_DQCCC_8_REG                          0x00000018
#ifndef ASM_INC
typedef union {
  struct {
    //
    // IO buffer activate - DQ Analog[br]Tristates DQ and DQS IO buffers [br]Disable
    // Rx sections of IO buffers [br]0 : reset [br]1 : out of reset
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX lvl_bl_iobufactafe : 1;
    //
    // IO buffer activate - CCC Analog[br]Tristates DQ and DQS IO buffers [br]Disable
    // Rx sections of IO buffers [br]0 : reset [br]1 : out of reset
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX lvl_cc_iobufactafe : 1;
    //
    // Reserved bits
    //
    // Bits[31:2], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved515 : 30;
#else
    UINTX reserved : 30;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} IOBUFACT_AFE_DQCCC_8_STRUCT;
#endif // ASM_INC

#define IOBUFACT_DIG_DQCCC_8_REG                          0x0000001C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // IO buffer activate - DQ Digital[br]Bytelane 1 SW reset for final TX flops in all
    // upartitions.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX bl_iobufactdig : 1;
    //
    // IO buffer activate - CCC Digital[br]Bytelane 1 SW reset for final TX flops in
    // all upartitions.
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX cc_iobufactdig : 1;
    //
    // Reserved bits
    //
    // Bits[31:2], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved516 : 30;
#else
    UINTX reserved : 30;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} IOBUFACT_DIG_DQCCC_8_STRUCT;
#endif // ASM_INC

#define SPIDIC_DQCCC_8_REG                                0x00000020
#ifndef ASM_INC
typedef union {
  struct {
    //
    // SPID init complete
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX spid_init_complete : 1;
    //
    // Reserved bits
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved517 : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} SPIDIC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define WRDQCTL_DQCCC_8_REG                               0x00000024
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the DQS configuration for first phase [br](2'b01-dqs_hi_lo,2'b00-dqs_lo_lo,2'b11-dqs_hi_hi).
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX dqdrvenbpream0 : 2;
    //
    // Last phase - DQ Drive Enable
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX dqdrvenblastph : 2;
    //
    // This is the two write preambles enable
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX dqdrvenbpream1 : 2;
    //
    // Data mask drive enable
    //
    // Bits[7:6], RW, default = 0x0
    //
    UINTX dqdrvenbph1 : 2;
    //
    // Reserved bits
    //
    // Bits[9:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved519 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:10], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved518 : 22;
#else
    UINTX reserved_1 : 22;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} WRDQCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define WRDQSCTL_DQCCC_8_REG                              0x00000028
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Write command to DQS start delay
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX wrcmd2dqsstart : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved522 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the DQS configuration during first preamble[br](2'b01-dqs_hi_lo,2'b00-dqs_lo_lo,2'b11-dqs_hi_hi).
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX dqsdatapream0 : 2;
    //
    // This is the DQS configuration for last phase [br](2'b01-dqs_hi_lo,2'b00-dqs_lo_lo,2'b11-dqs_hi_hi).
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX dqsdatalastph : 2;
    //
    // This is the DQS configuration during second preamble[br](2'b01-dqs_hi_lo,2'b00-dqs_lo_lo,2'b11-dqs_hi_hi).
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX dqsdatapream1 : 2;
    //
    // This is the DQS configuration for first phase [br](2'b01-dqs_hi_lo,2'b00-dqs_lo_lo,2'b11-dqs_hi_hi).
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX dqsdataph1 : 2;
    //
    // DQS Drive Enable - Idle
    //
    // Bits[17:16], RW, default = 0x3
    //
    UINTX dqsdrvenbidle : 2;
    //
    // TBD
    //
    // Bits[18:18], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved521 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // This is the two write preambles enable
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX wrpream0en : 1;
    //
    // Data mask drive enable
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX dmdrven : 1;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved520 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // Preamble 0 - DQS Drive enable
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX dqsdrvenbpream0 : 2;
    //
    // Last phase - DQS Drive enable
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX dqsdrvenblastph : 2;
    //
    // Preamble 1 - DQS drive enable
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX dqsdrvenbpream1 : 2;
    //
    // Phase 1 - DQS Drive enable
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX dqsdrvenbph1 : 2;
  } Bits;
  UINTX Data;
} WRDQSCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RDVLDCTL_DQCCC_8_REG                              0x0000002C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the read command to read valid register.[br]This is in terms of spidclk.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX rdcmd2rdvld : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved526 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the early read valid configuration register [br]0: no early read valid
    // geneartion[br]1/2/3/4: 1/2/3/4/spidclk early valid[br] >4: Illegal
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX earlyrdvld : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved525 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Read command 2 read valid plus 1 sel.
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX rdcmd2rdvld_plus1sel : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved524 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Read command 2 read bonus bits.
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX rdcmd2rdvld_bonus : 8;
    //
    // Reserved bits
    //
    // Bits[31:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved523 : 8;
#else
    UINTX reserved_3 : 8;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RDVLDCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RK2RKCTL1_DQCCC_8_REG                             0x00000030
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the rank to rank enable for the receive enable.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX bl_rk2rken_txrcven : 1;
    //
    // This is the rank to rank override.(1 - no rank to rank switch enable).
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX bl_rk2rken_rxdq_pi : 1;
    //
    // This is the rank to rank override.(1 - no rank to rank switch enable).
    //
    // Bits[5:2], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved528 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
    //
    // This is the rank to rank override.(1 - no rank to rank switch enable).
    //
    // Bits[6:6], RW, default = 0x0
    //
    UINTX bl_rk2rken_txdq_pi : 1;
    //
    // This is the rank to rank override.(1 - no rank to rank switch enable).
    //
    // Bits[7:7], RW, default = 0x0
    //
    UINTX bl_rk2rken_txdqs_pi : 1;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[8:8], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_txrcven : 1;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[9:9], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_txrden : 1;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[10:10], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_iodrven : 1;
    //
    // This is the rank to rank override.(1 - no rank to rank switch enable).
    //
    // Bits[31:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved527 : 21;
#else
    UINTX reserved_1 : 21;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RK2RKCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RK2RKCTL0_DQCCC_8_REG                             0x00000034
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the pattern for first 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX rk2rkrdencode_1st : 2;
    //
    // This is the pattern for second 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX rk2rkrdencode_2nd : 2;
    //
    // This is the pattern for first 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).[br]Applicable for both DQ and DQS drvenb.
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX rk2rkdrvenbcode_1st : 2;
    //
    // This is the pattern for second 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).[br]Applicable for both DQ and DQS drvenb.
    //
    // Bits[7:6], RW, default = 0x0
    //
    UINTX rk2rkdrvenbcode_2nd : 2;
    //
    // This is the pattern for first 1x clock of rk2rk_iodrvenb[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).
    //
    // Bits[13:8], RW, default = 0x0
    //
    UINTX rdcmd2rkchglatency : 6;
    //
    // Reserved bits
    //
    // Bits[14:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved529 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // This is the pattern for second 1x clock of rk2rk_iodrvenb[br](bit[0] - 1xhigh
    // phase value, bit[1] - 1xlow phase value).
    //
    // Bits[20:15], RW, default = 0x0
    //
    UINTX wrcmd2rkchglatency : 6;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_txdq : 1;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_txdqs : 1;
    //
    // pushing out the update of rank information 1 more 2xclk if set to 1.
    //
    // Bits[23:23], RW, default = 0x0
    //
    UINTX rk2rkmio2xclkpushout_rxdq : 1;
    //
    // This is the pattern for third 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rk2rkrdencode_3rd : 4;
    //
    // This is the pattern for third 1x clock of rk2rk_rden[br](bit[0] - 1xhigh phase
    // value, bit[1] - 1xlow phase value).[br]Applicable for both DQ and DQS drvenb.
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rk2rkdrvenbcode_3rd : 4;
  } Bits;
  UINTX Data;
} RK2RKCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXODTSEGCTL_CC_DQCCC_8_REG                        0x00000038
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive ODT segment enable latency configuration register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX cc_rdcmd2rxodtseg : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved531 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive ODT segment enable configuration register - preamble 0(rise
    // & fall) [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX cc_rxodtsegpre0 : 2;
    //
    // This is the receive ODT segment enable configuration register - postamble 0 -
    // new(rise & fall)[br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High &
    // Low[br]10 - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX cc_rxodtsegpst0 : 2;
    //
    // This is the receive ODT segment enable configuration register - preamble 1(rise
    // & fall) [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX cc_rxodtsegpre1 : 2;
    //
    // This is the receive ODT segment enable configuration register - postamble 1 -
    //  new(rise & fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High
    // & Low[br]10 - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX cc_rxodtsegpst1 : 2;
    //
    // This is the receive ODT segment enable length configuration register.[br]It's
    // in number of 1x clocks.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX cc_rxodtseglenbl4 : 6;
    //
    // This is the RXODTSEGEN one hot enable[br]0- rxodtsegen: 001->010->100[br]1-rxodtsegen:
    // 011->110->101
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX cc_rxodtsegtwohot : 1;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved530 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Receive ODT segment enable read pointer offset.
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rk1_odtsegenrdptroffset : 4;
    //
    // Receive ODT segment enable override select.[br]0= Dynamic  control[br]1= Static
    //  control
    //
    // Bits[28:28], RW, default = 0x0
    //
    UINTX cc_rxodtsegovrsel : 1;
    //
    // Receive ODT segment enable override value.
    //
    // Bits[31:29], RW, default = 0x0
    //
    UINTX cc_rxodtsegovrval : 3;
  } Bits;
  UINTX Data;
} RXODTSEGCTL_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXODTSEGCTL_DQ_DQCCC_8_REG                        0x0000003C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive ODT segment enable latency configuration register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX bl_rdcmd2rxodtseg : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved533 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive ODT segment enable configuration register - preamble 0(rise
    // & fall) [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX bl_rxodtsegpre0 : 2;
    //
    // This is the receive ODT segment enable configuration register - postamble 0 -
    // new(rise & fall)[br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High &
    // Low[br]10 - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX bl_rxodtsegpst0 : 2;
    //
    // This is the receive ODT segment enable configuration register - preamble 1(rise
    // & fall) [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX bl_rxodtsegpre1 : 2;
    //
    // This is the receive ODT segment enable configuration register - postamble 1 -
    //  new(rise & fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High
    // & Low[br]10 - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX bl_rxodtsegpst1 : 2;
    //
    // This is the receive ODT segment enable length configuration register.[br]It's
    // in number of 1x clocks.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX bl_rxodtseglenbl4 : 6;
    //
    // This is the RXODTSEGEN one hot enable[br]0- rxodtsegen: 001->010->100[br]1-rxodtsegen:
    // 011->110->101
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX bl_rxodtsegtwohot : 1;
    //
    // Reseved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved532 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Receive ODT segment enable read pointer offset.
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rk0_odtsegenrdptroffset : 4;
    //
    // Receive ODT segment enable override select.[br]0= Dynamic  receive ODT control[br]1=
    // Static  receive ODT control
    //
    // Bits[28:28], RW, default = 0x0
    //
    UINTX bl_rxodtsegovrsel : 1;
    //
    // Receive ODT segment enable override value. [br]0: De-assert receive ODT enable
    // [br]1: Assert receive ODT enable
    //
    // Bits[31:29], RW, default = 0x0
    //
    UINTX bl_rxodtsegovrval : 3;
  } Bits;
  UINTX Data;
} RXODTSEGCTL_DQ_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXODTCTL_CC_DQCCC_8_REG                           0x00000040
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive ODT enable latency configuration register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX cc_rdcmd2rxodten : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved536 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive ODT enable configuration register - preamble 0(rise & fall)
    // [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise
    // & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX cc_rxodtenpre0 : 2;
    //
    // This is the receive ODT enable configuration register - postamble 0 - new(rise
    // & fall)[br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX cc_rxodtenpst0 : 2;
    //
    // This is the receive ODT enable configuration register - preamble 1(rise & fall)
    // [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise
    // & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX cc_rxodtenpre1 : 2;
    //
    // This is the receive ODT enable configuration register - postamble 1 -  new(rise
    // & fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX cc_rxodtenpst1 : 2;
    //
    // This is the receive ODT enable length configuration register.[br]It's in number
    // of 1x clocks.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX cc_rxodtenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved535 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // Receive ODT enable override select.[br]0= Dynamic RCVEN control[br]1= Static RCVEN
    // control
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX cc_rxodtenovrsel : 1;
    //
    // Receive ODT enable override value. [br]0: De-assert receive enable [br]1: Assert
    // receive enable
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX cc_rxodtenovrval : 1;
    //
    // Reseved bits
    //
    // Bits[27:26], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved534 : 2;
#else
    UINTX reserved_2 : 2;
#endif // MEM_SV_HOOKS
    //
    // This is receive ODT enable read pointer offset.
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rk1_odtenrdptroffset : 4;
  } Bits;
  UINTX Data;
} RXODTCTL_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXODTCTL_DQ_DQCCC_8_REG                           0x00000044
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive ODT enable latency configuration register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX bl_rdcmd2rxodten : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved539 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive ODT enable configuration register - preamble 0(rise & fall)
    // [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise
    // & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX bl_rxodtenpre0 : 2;
    //
    // This is the receive ODT enable configuration register - postamble 0 - new(rise
    // & fall)[br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX bl_rxodtenpst0 : 2;
    //
    // This is the receive ODT enable configuration register - preamble 1(rise & fall)
    // [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise
    // & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX bl_rxodtenpre1 : 2;
    //
    // This is the receive ODT enable configuration register - postamble 1 -  new(rise
    // & fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX bl_rxodtenpst1 : 2;
    //
    // This is the receive ODT enable length configuration register.[br]It's in number
    // of 1x clocks.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX bl_rxodtenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved538 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // Receive ODT enable override select.[br]0= Dynamic RCVEN control[br]1= Static RCVEN
    // control
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX bl_rxodtenovrsel : 1;
    //
    // Receive ODT enable override value. [br]0: De-assert receive enable [br]1: Assert
    // receive enable
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX bl_rxodtenovrval : 1;
    //
    // Reserved bits
    //
    // Bits[27:26], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved537 : 2;
#else
    UINTX reserved_2 : 2;
#endif // MEM_SV_HOOKS
    //
    // This is receive ODT enable read pointer offset.
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rk0_odtenrdptroffset : 4;
  } Bits;
  UINTX Data;
} RXODTCTL_DQ_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RCVENCTL_CC_DQCCC_8_REG                           0x00000048
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive enable latency configuration register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX cc_rdcmd2rcven : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved542 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive enable configuration register - preamble 0(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX cc_rcvenpre0 : 2;
    //
    // This is the receive enable configuration register - postamble 0 - new(rise & fall)[br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX cc_rcvenpst0 : 2;
    //
    // This is the receive enable configuration register - preamble 1(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX cc_rcvenpre1 : 2;
    //
    // This is the receive enable configuration register - postamble 1 -  new(rise &
    // fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX cc_rcvenpst1 : 2;
    //
    // This is the receive enable length(number of 1x clock) configuration register.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX cc_rcvenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved541 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // Receive enable override select.[br]0= Dynamic RCVEN control[br]1= Static RCVEN
    // control
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX cc_rcvenovrsel : 1;
    //
    // Receive enable override value. [br]0: De-assert receive enable [br]1: Assert receive
    // enable
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX cc_rcvenovrval : 1;
    //
    // Receive enable margining data
    //
    // Bits[26:26], RW, default = 0x0
    //
    UINTX cc_rcvenmargin : 1;
    //
    // DQS counter reset
    //
    // Bits[27:27], RW, default = 0x0
    //
    UINTX drx_cc_dqscountrstb : 1;
    //
    // Reserved bits
    //
    // Bits[30:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved540 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // AFE status register - RCVSMPL - CC
    //
    // Bits[31:31], RO, default = 0x0
    //
    UINTX drx_cc_rcvsmpl : 1;
  } Bits;
  UINTX Data;
} RCVENCTL_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RCVENCTL_DQ_DQCCC_8_REG                           0x0000004C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the receive enable latency configuration register.[br]RL - 1: maps to
    // DRAM RL.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX bl_rdcmd2rcven : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved545 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the receive enable configuration register - preamble 0(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX bl_rcvenpre0 : 2;
    //
    // This is the receive enable configuration register - postamble 0 - new(rise & fall)[br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX bl_rcvenpst0 : 2;
    //
    // This is the receive enable configuration register - preamble 1(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX bl_rcvenpre1 : 2;
    //
    // This is the receive enable configuration register - postamble 1 -  new(rise &
    // fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX bl_rcvenpst1 : 2;
    //
    // This is the receive enable length(number of 1x clock) configuration register.
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX bl_rcvenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved544 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // Receive enable override select.[br]0= Dynamic RCVEN control[br]1= Static RCVEN
    // control
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX bl_rcvenovrsel : 1;
    //
    // Receive enable override value. [br]0: De-assert receive enable [br]1: Assert receive
    // enable
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX bl_rcvenovrval : 1;
    //
    // Receive enable margining data
    //
    // Bits[26:26], RW, default = 0x0
    //
    UINTX bl_rcvenmargin : 1;
    //
    // DQS counter reset
    //
    // Bits[27:27], RW, default = 0x0
    //
    UINTX drx_bl_dqscountrstb : 1;
    //
    // Reseved bits
    //
    // Bits[30:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved543 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // AFE status register - RCVSMPL - DQ
    //
    // Bits[31:31], RO, default = 0x0
    //
    UINTX drx_bl_rcvsmpl : 1;
  } Bits;
  UINTX Data;
} RCVENCTL_DQ_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DIFFAMPCTL_CC_DQCCC_8_REG                         0x00000050
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the Diffamp enable latency register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX cc_rdcmd2diffampen : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved547 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the diffamp enable length(number of 1x clock) configuration register.
    //
    // Bits[13:8], RW, default = 0x0
    //
    UINTX cc_diffampenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved546 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the diffamp enable configuration register - preamble 0(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX cc_diffampenpre0 : 2;
    //
    // This is the diffamp enable configuration register - postamble 0 - new(rise & fall)[br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX cc_diffampenpst0 : 2;
    //
    // This is the diffamp enable configuration register - preamble 1(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX cc_diffampenpre1 : 2;
    //
    // This is the diffamp enable configuration register - postamble 1 -  new(rise &
    // fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[23:22], RW, default = 0x0
    //
    UINTX cc_diffampenpst1 : 2;
    //
    // Override select for DQ/DM Diffamp enable - DQ Lane
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX cc_dqdmdiffampenovrsel : 1;
    //
    // Override value for DQ/DM Diffamp enable - DQ Lane
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX cc_dqdmdiffampenovrval : 1;
    //
    // Override select for DQS Diffamp enable - DQ Lane
    //
    // Bits[26:26], RW, default = 0x0
    //
    UINTX cc_dqsdiffampenovrsel : 1;
    //
    // Override value for DQS Diffamp enable - DQ Lane
    //
    // Bits[27:27], RW, default = 0x0
    //
    UINTX cc_dqsdiffampenovrval : 1;
    //
    // Differential amplifier read pointer offset control.
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rk1_diffampenrdptroffset : 4;
  } Bits;
  UINTX Data;
} DIFFAMPCTL_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DIFFAMPCTL_DQ_DQCCC_8_REG                         0x00000054
#ifndef ASM_INC
typedef union {
  struct {
    //
    // This is the Diffamp enable latency register.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX bl_rdcmd2diffampen : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved549 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the diffamp enable length(number of 1x clock) configuration register.
    //
    // Bits[13:8], RW, default = 0x0
    //
    UINTX bl_diffampenlenbl4 : 6;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved548 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the diffamp enable configuration register - preamble 0(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX bl_diffampenpre0 : 2;
    //
    // This is the diffamp enable configuration register - postamble 0 - new(rise & fall)[br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX bl_diffampenpst0 : 2;
    //
    // This is the diffamp enable configuration register - preamble 1(rise & fall) [br]00
    // - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10 - Rise & Fall
    // - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX bl_diffampenpre1 : 2;
    //
    // This is the diffamp enable configuration register - postamble 1 -  new(rise &
    // fall). [br]00 - Rise & Fall - Low & Low[br]01 - Rise & Fall - High & Low[br]10
    // - Rise & Fall - Low & High[br]11 - Rise & Fall - High & High
    //
    // Bits[23:22], RW, default = 0x0
    //
    UINTX bl_diffampenpst1 : 2;
    //
    // Override select for DQ/DM Diffamp enable - DQ Lane
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX bl_dqdmdiffampenovrsel : 1;
    //
    // Override value for DQ/DM Diffamp enable - DQ Lane
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX bl_dqdmdiffampenovrval : 1;
    //
    // Override select for DQS Diffamp enable - DQ Lane
    //
    // Bits[26:26], RW, default = 0x0
    //
    UINTX bl_dqsdiffampenovrsel : 1;
    //
    // Override value for DQS Diffamp enable - DQ Lane
    //
    // Bits[27:27], RW, default = 0x0
    //
    UINTX bl_dqsdiffampenovrval : 1;
    //
    // Differential amplifier read pointer offset control.
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rk0_diffampenrdptroffset : 4;
  } Bits;
  UINTX Data;
} DIFFAMPCTL_DQ_DQCCC_8_STRUCT;
#endif // ASM_INC

#define WRLVL_DQCCC_8_REG                                 0x00000058
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQTRAINSTS - WRLVL Byte0 sample OR combination
    //
    // Bits[0:0], RO, default = 0x0
    //
    UINTX wrlvl_or_sample_b0 : 1;
    //
    // DQTRAINSTS - WRLVL Byte1 sample OR combination
    //
    // Bits[1:1], RO, default = 0x0
    //
    UINTX wrlvl_or_sample_b1 : 1;
    //
    // DQTRAINSTS - WRLVL Byte0 sample AND combination
    //
    // Bits[2:2], RO, default = 0x0
    //
    UINTX wrlvl_and_sample_b0 : 1;
    //
    // DQTRAINSTS - WRLVL Byte1 sample AND combination
    //
    // Bits[3:3], RO, default = 0x0
    //
    UINTX wrlvl_and_sample_b1 : 1;
    //
    // Reserved bits
    //
    // Bits[7:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved551 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
    //
    // DQTRAINSTS - WRLVL sampling from all DQ bit lanes of Bytelane 0
    //
    // Bits[15:8], RO, default = 0x0
    //
    UINTX wrlvl_sample_b0 : 8;
    //
    // DQTRAINSTS - WRLVL sampling from all DQ bit lanes of Bytelane 1
    //
    // Bits[23:16], RO, default = 0x0
    //
    UINTX wrlvl_sample_b1 : 8;
    //
    // Write Levelization enable - BL Byte Labe
    //
    // Bits[24:24], RW, default = 0x0
    //
    UINTX bl_wrlvl : 1;
    //
    // Write Levelization enable - CC Byte Lane
    //
    // Bits[25:25], RW, default = 0x0
    //
    UINTX cc_wrlvl : 1;
    //
    // Reserved bits
    //
    // Bits[31:26], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved550 : 6;
#else
    UINTX reserved_1 : 6;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} WRLVL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CLKGATERDCTL_DQCCC_8_REG                          0x0000005C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Clock Gating register : IDLE timer counter max value for clock gating on tx/rx
    // . Counter timeout based on last WRCMD/RDCMD received. 6 bits are good enough since
    // it increments in spidclk.
    //
    // Bits[5:0], RW, default = 0x3F
    //
    UINTX readclkgate_maxcntr : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved555 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the delay in terms of clocks the drven is enabled, once wrcmd/rdcmd is
    // received.
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX read_startrden : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved554 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // This is the delay in terms of clocks the dven is disabled before clock gated.
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX read_stoprden : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved553 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // TBD
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dynreadspidclken : 1;
    //
    // TBD
    //
    // Bits[17:17], RW, default = 0x0
    //
    UINTX dynreadref1xclken : 1;
    //
    // Enable Dynamic REF2XCLK control. 1= ref2xclken  controlled by the dynamic clock
    // gating logic (DYNDLLFSM), 0= Statically-controlled REF2XCLKEN
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX dynreadref2xclken : 1;
    //
    // Enable Dynamic SIGGRP 2x clock control. 1= SIGGRPEN  controlled by the dynamic
    // clock gating logic (DYNDLLFSM), 0= Statically-controlled SIGGRPEN
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX dynreadsiggrpen : 1;
    //
    // Reserved bits
    //
    // Bits[31:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved552 : 12;
#else
    UINTX reserved_3 : 12;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CLKGATERDCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CLKGATEWRCTL_DQCCC_8_REG                          0x00000060
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Clock Gating register : IDLE timer counter max value for clock gating on tx/rx
    // . Counter timeout based on last WRCMD/RDCMD received. 6 bits are good enough since
    // it increments in spidclk.
    //
    // Bits[5:0], RW, default = 0x3F
    //
    UINTX writeclkgate_maxcntr : 6;
    //
    // Reserved bits
    //
    // Bits[7:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved559 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // This is the delay in terms of clocks the drven is enabled, once wrcmd/rdcmd is
    // received.
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX write_startdrv : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved558 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // This is the delay in terms of clocks the dven is disabled before clock gated.
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX write_stopdrv : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved557 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // TBD
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dynwritespidclken : 1;
    //
    // TBD
    //
    // Bits[17:17], RW, default = 0x0
    //
    UINTX dynwriteref1xclken : 1;
    //
    // Enable Dynamic REF2XCLK control. 1= ref2xclken  controlled by the dynamic clock
    // gating logic (DYNDLLFSM), 0= Statically-controlled REF2XCLKEN
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX dynwriteref2xclken : 1;
    //
    // Enable Dynamic SIGGRP 2x clock control. 1= SIGGRPEN  controlled by the dynamic
    // clock gating logic (DYNDLLFSM), 0= Statically-controlled SIGGRPEN
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX dynwritesiggrpen : 1;
    //
    // Reserved bits
    //
    // Bits[31:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved556 : 12;
#else
    UINTX reserved_3 : 12;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CLKGATEWRCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CLKGATECTL_DQCCC_8_REG                            0x00000064
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Clock Gating register : IDLE timer counter max value for clock gating on tx/rx
    // . Counter timeout based on last WRCMD/RDCMD received. 6 bits are good enough since
    // it increments in spidclk.
    //
    // Bits[5:0], RW, default = 0x3F
    //
    UINTX clkgate_maxcntr : 6;
    //
    // Reserved bits
    //
    // Bits[15:6], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved561 : 10;
#else
    UINTX reserved : 10;
#endif // MEM_SV_HOOKS
    //
    // TBD
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dynspidclken : 1;
    //
    // TBD
    //
    // Bits[17:17], RW, default = 0x0
    //
    UINTX dynref1xclken : 1;
    //
    // Enable Dynamic REF2XCLK control. 1= ref2xclken  controlled by the dynamic clock
    // gating logic (DYNDLLFSM), 0= Statically-controlled REF2XCLKEN
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX dynref2xclken : 1;
    //
    // Enable Dynamic SIGGRP 2x clock control. 1= SIGGRPEN  controlled by the dynamic
    // clock gating logic (DYNDLLFSM), 0= Statically-controlled SIGGRPEN
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX dynsiggrpen : 1;
    //
    // Reserved bits
    //
    // Bits[31:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved560 : 12;
#else
    UINTX reserved_1 : 12;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CLKGATECTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP5CTL_DQCCC_8_REG                       0x00000068
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 5 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp05picode : 8;
    //
    // Read pointer offset for sig group 5 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp05rdptroffset : 4;
    //
    // Deadband select for sig group 5 rank 0
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp05dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved566 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[23:16], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved565 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[27:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved564 : 4;
#else
    UINTX reserved_2 : 4;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[29:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved563 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved562 : 2;
#else
    UINTX reserved_4 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP5CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP4CTL_DQCCC_8_REG                       0x0000006C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 4 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp04picode : 8;
    //
    // Read pointer offset for sig group 4 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp04rdptroffset : 4;
    //
    // Deadband select for sig group 4 rank 0
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp04dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved571 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[23:16], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved570 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[27:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved569 : 4;
#else
    UINTX reserved_2 : 4;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[29:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved568 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved567 : 2;
#else
    UINTX reserved_4 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP4CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP3CTL_DQCCC_8_REG                       0x00000070
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 3 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp03picode : 8;
    //
    // Read pointer offset for sig group 3 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp03rdptroffset : 4;
    //
    // Deadband select for sig group 3 rank 0
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp03dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved576 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[23:16], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved575 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[27:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved574 : 4;
#else
    UINTX reserved_2 : 4;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[29:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved573 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved572 : 2;
#else
    UINTX reserved_4 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP3CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP2CTL_DQCCC_8_REG                       0x00000074
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 2 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp02picode : 8;
    //
    // Read pointer offset for sig group 2 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp02rdptroffset : 4;
    //
    // Deadband select for sig group 2 rank 0
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp02dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved578 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // PI code for sig group 2 rank 1
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp02picode : 8;
    //
    // Read pointer offset for sig group 2 rank 1
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp02rdptroffset : 4;
    //
    // Deadband select for sig group 2 rank 1
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp02dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved577 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP2CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP1CTL_DQCCC_8_REG                       0x00000078
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 1 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp01picode : 8;
    //
    // Read pointer offset for sig group 1 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp01rdptroffset : 4;
    //
    // Deadband select for sig group 1 rank 0
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp01dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved580 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // PI code for sig group 1 rank 1
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp01picode : 8;
    //
    // Read pointer offset for sig group 1 rank 1
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp01rdptroffset : 4;
    //
    // Deadband select for sig group 1 rank 1
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp01dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved579 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP1CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP0CTL_DQCCC_8_REG                       0x0000007C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PI code for sig group 0 rank 0
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp00picode : 8;
    //
    // Read pointer offset for sig group 0 rank 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp00rdptroffset : 4;
    //
    // Reserved bits
    //
    // Bits[15:12], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved582 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
    //
    // PI code for sig group 0 rank 1
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp00picode : 8;
    //
    // Read pointer offset for sig group 0 rank 1
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp00rdptroffset : 4;
    //
    // Reserved bits
    //
    // Bits[31:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved581 : 4;
#else
    UINTX reserved_1 : 4;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP0CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define WKPUPPDNCTL_DQCCC_8_REG                           0x00000080
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Weak Pull Down Control code for DQS - DQ Lane
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX lvl_bl_weakpdn : 6;
    //
    // Weak Pull Down Control static bit for DQS - DQ Lane
    //
    // Bits[6:6], RW, default = 0x0
    //
    UINTX lvl_bl_weakpdnstatic : 1;
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved586 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // Weak Pull Up Control code for DQS - DQ Lane
    //
    // Bits[13:8], RW, default = 0x0
    //
    UINTX lvl_bl_weakpup : 6;
    //
    // Weak Pull Up Control static bit for DQS - DQ Lane
    //
    // Bits[14:14], RW, default = 0x0
    //
    UINTX lvl_bl_weakpupstatic : 1;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved585 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Weak Pull Down Control - CC Lane
    //
    // Bits[21:16], RW, default = 0x0
    //
    UINTX lvl_cc_weakpdn : 6;
    //
    // Weak Pull Down Control - CC Lane
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX lvl_cc_weakpdnstatic : 1;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved584 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Weak Pull Up Control - CC Lane
    //
    // Bits[29:24], RW, default = 0x0
    //
    UINTX lvl_cc_weakpup : 6;
    //
    // Weak Pull Up Control - CC Lane
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX lvl_cc_weakpupstatic : 1;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved583 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} WKPUPPDNCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define AFEMISC1_DQCCC_8_REG                              0x00000084
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Bypasses slewrate staggering
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX tx_bl_allfire : 1;
    //
    // Bypasses slewrate staggering
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX tx_cc_allfire : 1;
    //
    // Bypasses slewrate staggering
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX dtx_bl_allfire : 1;
    //
    // Bypasses slewrate staggering
    //
    // Bits[3:3], RW, default = 0x0
    //
    UINTX dtx_cc_allfire : 1;
    //
    // set cap tuning for pi
    //
    // Bits[6:4], RW, default = 0x1
    //
    UINTX txdll_picapsel : 3;
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved593 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // set cap tuning for pi
    //
    // Bits[10:8], RW, default = 0x1
    //
    UINTX rxdll_bl_picapsel : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved592 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // set cap tuning for pi
    //
    // Bits[14:12], RW, default = 0x1
    //
    UINTX rxdll_cc_picapsel : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved591 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Segment enable for CC buffer. Used for per leg testing in HVM
    //
    // Bits[18:16], RW, default = 0x7
    //
    UINTX tx_bl_drvsegen : 3;
    //
    // Reserved bits
    //
    // Bits[19:19], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved590 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // Segment enable for CC buffer. Used for per leg testing in HVM
    //
    // Bits[22:20], RW, default = 0x7
    //
    UINTX dtx_bl_drvsegen : 3;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved589 : 1;
#else
    UINTX reserved_4 : 1;
#endif // MEM_SV_HOOKS
    //
    // Segment enable for CC buffer. Used for per leg testing in HVM
    //
    // Bits[26:24], RW, default = 0x7
    //
    UINTX tx_cc_drvsegen : 3;
    //
    // Reserved bits
    //
    // Bits[27:27], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved588 : 1;
#else
    UINTX reserved_5 : 1;
#endif // MEM_SV_HOOKS
    //
    // Segment enable for CC buffer. Used for per leg testing in HVM
    //
    // Bits[30:28], RW, default = 0x7
    //
    UINTX dtx_cc_drvsegen : 3;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved587 : 1;
#else
    UINTX reserved_6 : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} AFEMISC1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define AFEMISC0_DQCCC_8_REG                              0x00000088
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Static pull up code - ODT DQS/DQSB
    //
    // Bits[0:0], RW, default = 0x1
    //
    UINTX pupodtstaticdqs : 1;
    //
    // Static pull up code - ODT DQ
    //
    // Bits[1:1], RW, default = 0x1
    //
    UINTX pupodtstatic : 1;
    //
    // Static pull up code - DRV DQS/DQSB
    //
    // Bits[2:2], RW, default = 0x1
    //
    UINTX pupdrvstaticdqs : 1;
    //
    // Static pull up code - DRV DQ
    //
    // Bits[3:3], RW, default = 0x1
    //
    UINTX pupdrvstatic : 1;
    //
    // Static pull down code - ODT DQS/DQSB
    //
    // Bits[4:4], RW, default = 0x1
    //
    UINTX pdnodtstaticdqs : 1;
    //
    // Static pull down code - ODT DQ
    //
    // Bits[5:5], RW, default = 0x1
    //
    UINTX pdnodtstatic : 1;
    //
    // Static pull down code - DRV DQS/DQSB
    //
    // Bits[6:6], RW, default = 0x1
    //
    UINTX pdndrvstaticdqs : 1;
    //
    // Static pull down code - DRV DQ
    //
    // Bits[7:7], RW, default = 0x1
    //
    UINTX pdndrvstatic : 1;
    //
    // changes the common mode of receiver from low common mode to vcc/2 based on lpddr3/
    // lpddr4.
    //
    // Bits[8:8], RW, default = 0x1
    //
    UINTX rx_bl_rxcommonmode : 1;
    //
    // changes the common mode of receiver from low common mode to vcc/2 based on lpddr3/
    // lpddr4.
    //
    // Bits[9:9], RW, default = 0x1
    //
    UINTX rx_cc_rxcommonmode : 1;
    //
    // changes the common mode of receiver from low common mode to vcc/2 based on lpddr3/
    // lpddr4.
    //
    // Bits[10:10], RW, default = 0x1
    //
    UINTX drx_bl_rxcommonmode : 1;
    //
    // changes the common mode of receiver from low common mode to vcc/2 based on lpddr3/
    // lpddr4.
    //
    // Bits[11:11], RW, default = 0x1
    //
    UINTX drx_cc_rxcommonmode : 1;
    //
    // Reserved bits
    //
    // Bits[15:12], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved594 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
    //
    // 00 = low power rx;[br]01 = high power rx (default)
    //
    // Bits[17:16], RW, default = 0x1
    //
    UINTX rx_bl_rxpwrmode : 2;
    //
    // 00 = low power rx;[br]01 = high power rx (default)
    //
    // Bits[19:18], RW, default = 0x1
    //
    UINTX drx_bl_rxpwrmode : 2;
    //
    // 00 = low power rx;[br]01 = high power rx (default)
    //
    // Bits[21:20], RW, default = 0x1
    //
    UINTX rx_cc_rxpwrmode : 2;
    //
    // 00 = low power rx;[br]01 = high power rx (default)
    //
    // Bits[23:22], RW, default = 0x1
    //
    UINTX drx_cc_rxpwrmode : 2;
    //
    // Streches rising edge of drivenable to relax data to enable skew.
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX tx_bl_drvenstretch : 2;
    //
    // Streches rising edge of drivenable to relax data to enable skew.
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX tx_cc_drvenstretch : 2;
    //
    // Streches rising edge of drivenable to relax data to enable skew.
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX dtx_bl_drvenstretch : 2;
    //
    // Streches rising edge of drivenable to relax data to enable skew.
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX dtx_cc_drvenstretch : 2;
  } Bits;
  UINTX Data;
} AFEMISC0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define AFEBONUS_DQCCC_8_REG                              0x0000008C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Bonus bits - DQ Lane
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX lvl_bl_bonus : 4;
    //
    // Bonus bits - CC Lane
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX lvl_cc_bonus : 4;
    //
    // Reserved bits
    //
    // Bits[31:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved595 : 24;
#else
    UINTX reserved : 24;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} AFEBONUS_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DIGOBSCTL1_DQCCC_8_REG                            0x00000090
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Digital observation select mux port 0
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX rxdll_cc_digobs0sel : 4;
    //
    // Digital observation select mux port 1
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX rxdll_cc_digobs1sel : 4;
    //
    // Digital observation select mux port 0
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX rxdll_bl_digobs0sel : 4;
    //
    // Digital observation select mux port 1
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX rxdll_bl_digobs1sel : 4;
    //
    // Reserved bits
    //
    // Bits[17:16], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved597 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // digital buf obs enable (1=enable;0=disable)
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX dtx_bl_digobsen : 1;
    //
    // digital buf obs enable (1=enable;0=disable)
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX txdll_digobsen : 1;
    //
    // Reserved bits
    //
    // Bits[20:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved596 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // digital buf obs enable (1=enable;0=disable)
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX dtx_cc_digobsen : 1;
    //
    // digital buf obs enable (1=enable;0=disable)
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX rxdll_bl_digobsen : 1;
    //
    // digital buf obs enable (1=enable;0=disable)
    //
    // Bits[23:23], RW, default = 0x0
    //
    UINTX rxdll_cc_digobsen : 1;
    //
    // Digital observation select mux port 0
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX txdll_digobs0sel : 4;
    //
    // Digital observation select mux port 1
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX txdll_digobs1sel : 4;
  } Bits;
  UINTX Data;
} DIGOBSCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DIGOBSCTL0_DQCCC_8_REG                            0x00000094
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Digital observation select mux port 0
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX tx_bl_digobs0sel : 3;
    //
    // Reserved bits
    //
    // Bits[3:3], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved605 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 1
    //
    // Bits[6:4], RW, default = 0x0
    //
    UINTX tx_bl_digobs1sel : 3;
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved604 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 0
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX tx_cc_digobs0sel : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved603 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 1
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX tx_cc_digobs1sel : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved602 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 0
    //
    // Bits[18:16], RW, default = 0x0
    //
    UINTX dtx_bl_digobs0sel : 3;
    //
    // Reserved bits
    //
    // Bits[19:19], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved601 : 1;
#else
    UINTX reserved_4 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 1
    //
    // Bits[22:20], RW, default = 0x0
    //
    UINTX dtx_bl_digobs1sel : 3;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved600 : 1;
#else
    UINTX reserved_5 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 0
    //
    // Bits[26:24], RW, default = 0x0
    //
    UINTX dtx_cc_digobs0sel : 3;
    //
    // Reserved bits
    //
    // Bits[27:27], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved599 : 1;
#else
    UINTX reserved_6 : 1;
#endif // MEM_SV_HOOKS
    //
    // Digital observation select mux port 1
    //
    // Bits[30:28], RW, default = 0x0
    //
    UINTX dtx_cc_digobs1sel : 3;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved598 : 1;
#else
    UINTX reserved_7 : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DIGOBSCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXDLLCTL_DQBLDM_DQCCC_8_REG                       0x00000098
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved bits
    //
    // Bits[15:0], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved610 : 16;
#else
    UINTX reserved : 16;
#endif // MEM_SV_HOOKS
    //
    // Rx DLL enable
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX rxdll_bl_rxdllen : 1;
    //
    // enable data output from dq and dm pi's. for each byte
    //
    // Bits[17:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved609 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // enable strobe output from rx sdll.for each byte
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX rxdll_bl_dqsouten : 1;
    //
    // enable strobe output from rx sdll.for each byte
    //
    // Bits[19:19], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved608 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // RxDLL flop reset
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX rxdll_dllfloprstb : 1;
    //
    // TxDLL flop reset
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX txdll_dllfloprstb : 1;
    //
    // enable strobe output from rx sdll.for each byte
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved607 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // enable strobe output from rx sdll.for each byte
    //
    // Bits[28:24], RW, default = 0x0
    //
    UINTX rxdll_rxmuxsel : 5;
    //
    // Reserved bits
    //
    // Bits[31:29], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved606 : 3;
#else
    UINTX reserved_4 : 3;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RXDLLCTL_DQBLDM_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXDLLCTL_CCBLDM_DQCCC_8_REG                       0x0000009C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // enable tx ref pi phase 0 enable
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX txdll_ref2xph0pien : 1;
    //
    // Reserved bits
    //
    // Bits[15:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved613 : 15;
#else
    UINTX reserved : 15;
#endif // MEM_SV_HOOKS
    //
    // Rx DLL enable
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX rxdll_cc_rxdllen : 1;
    //
    // enable data output from dq and dm pi's. for each byte
    //
    // Bits[17:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved612 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // enable strobe output from rx sdll.for each byte
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX rxdll_cc_dqsouten : 1;
    //
    // Reserved bits
    //
    // Bits[31:19], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved611 : 13;
#else
    UINTX reserved_2 : 13;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RXDLLCTL_CCBLDM_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLCTL_DQCCC_8_REG                              0x000000A0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reseved bits
    //
    // Bits[0:0], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved618 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // enable tx ref pi phase 90 enable
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX txdll_ref2xph90pien : 1;
    //
    // enable tx ref pi phase 180 enable
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX txdll_ref2xph180pien : 1;
    //
    // enable tx ref pi phase 270 enable
    //
    // Bits[3:3], RW, default = 0x0
    //
    UINTX txdll_ref2xph270pien : 1;
    //
    // Reserved bits
    //
    // Bits[7:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved617 : 4;
#else
    UINTX reserved_1 : 4;
#endif // MEM_SV_HOOKS
    //
    // PI enable for sig group 0 rank 0
    //
    // Bits[8:8], RW, default = 0x0
    //
    UINTX txdll_siggrp00pien : 1;
    //
    // PI enable for sig group 1 rank 0
    //
    // Bits[9:9], RW, default = 0x0
    //
    UINTX txdll_siggrp01pien : 1;
    //
    // PI enable for sig group 2 rank 0
    //
    // Bits[10:10], RW, default = 0x0
    //
    UINTX txdll_siggrp02pien : 1;
    //
    // PI enable for sig group 3 rank 0
    //
    // Bits[11:11], RW, default = 0x0
    //
    UINTX txdll_siggrp03pien : 1;
    //
    // PI enable for sig group 4 rank 0
    //
    // Bits[12:12], RW, default = 0x0
    //
    UINTX txdll_siggrp04pien : 1;
    //
    // PI enable for sig group 5 rank 0
    //
    // Bits[13:13], RW, default = 0x0
    //
    UINTX txdll_siggrp05pien : 1;
    //
    // Reserved bits
    //
    // Bits[15:14], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved616 : 2;
#else
    UINTX reserved_2 : 2;
#endif // MEM_SV_HOOKS
    //
    // MDLL enable
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX txdll_mdllen : 1;
    //
    // Reserved bits
    //
    // Bits[17:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved615 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[31:18], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved614 : 14;
#else
    UINTX reserved_4 : 14;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLREF2XCTL_DQCCC_8_REG                         0x000000A4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // pi code for ref pi phase 90 output
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX txdll_ref2xph90picode : 8;
    //
    // pi code for ref pi phase 180 output
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX txdll_ref2xph180picode : 8;
    //
    // pi code for ref pi phase 270 output
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX txdll_ref2xph270picode : 8;
    //
    // Reserved bits
    //
    // Bits[31:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved619 : 8;
#else
    UINTX reserved : 8;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLREF2XCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXDLLDQSRK1CTL_DQCCC_8_REG                        0x000000A8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved bits
    //
    // Bits[7:0], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved621 : 8;
#else
    UINTX reserved : 8;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[15:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved620 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
    //
    // pi code for rx falling dqs edge - DQ Lane
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX rxdll_bl_rk1dqslopicode : 8;
    //
    // pi code for rx rising dqs edge - DQ Lane
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX rxdll_bl_rk1dqshipicode : 8;
  } Bits;
  UINTX Data;
} RXDLLDQSRK1CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXDLLDQSRK0CTL_DQCCC_8_REG                        0x000000AC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // pi code for rx falling dqs edge  - CC Lane
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX rxdll_cc_rk0dqslopicode : 8;
    //
    // pi code for rx rising dqs edge  - CC Lane
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX rxdll_cc_rk0dqshipicode : 8;
    //
    // pi code for rx falling dqs edge - DQ Lane
    //
    // Bits[23:16], RW, default = 0x0
    //
    UINTX rxdll_bl_rk0dqslopicode : 8;
    //
    // pi code for rx rising dqs edge - DQ Lane
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX rxdll_bl_rk0dqshipicode : 8;
  } Bits;
  UINTX Data;
} RXDLLDQSRK0CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define VREFCTL1_DQCCC_8_REG                              0x000000B0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // enable signal for the tx vref unity gain buffer
    //
    // Bits[0:0], RW, default = 0x1
    //
    UINTX txvref_cmn_vrefbufen : 1;
    //
    // Reserved bits
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved622 : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} VREFCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define VREFCTL0_DQCCC_8_REG                              0x000000B4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // vref decode signal for the cc0 rx vref
    //
    // Bits[7:0], RW, default = 0x20
    //
    UINTX vref_rxccvrefctrl : 8;
    //
    // vref decode signal for the b0 rx vref
    //
    // Bits[15:8], RW, default = 0x20
    //
    UINTX vref_rxblvrefctrl : 8;
    //
    // common mode select to choose between lpddr3;lpddr4
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX vref_vrefcmctrl : 1;
    //
    // Reserved bits
    //
    // Bits[19:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved624 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // vrefiobufact - CC Lane
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX lvl_cc_vrefiobufact : 1;
    //
    // vrefiobufact - DQ Lane
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX lvl_bl_vrefiobufact : 1;
    //
    // Reserved bits
    //
    // Bits[23:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved623 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // vref decode signal for the lcomp
    //
    // Bits[31:24], RW, default = 0x0
    //
    UINTX vref_txlcompvrefctrl : 8;
  } Bits;
  UINTX Data;
} VREFCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXBL8PBDCTL_DQCCC_8_REG                           0x000000B8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX rxdll_bl_dq8rxpbdcode : 4;
    //
    // Reserved bits
    //
    // Bits[31:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved625 : 28;
#else
    UINTX reserved : 28;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RXBL8PBDCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXPBDBLCTL_DQCCC_8_REG                            0x000000BC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX rxdll_bl_dq0rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX rxdll_bl_dq1rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX rxdll_bl_dq2rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX rxdll_bl_dq3rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[19:16], RW, default = 0x0
    //
    UINTX rxdll_bl_dq4rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[23:20], RW, default = 0x0
    //
    UINTX rxdll_bl_dq5rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rxdll_bl_dq6rxpbdcode : 4;
    //
    // Rx Per bit De-skew code for dq0 valid codes
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rxdll_bl_dq7rxpbdcode : 4;
  } Bits;
  UINTX Data;
} RXPBDBLCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXPBDCTL1_DQCCC_8_REG                             0x000000C0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Per bit deskew enable - DTX - BL
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX dtx_bl_pbdstaticlegen : 1;
    //
    // Per bit deskew enable - DTX - CC
    //
    // Bits[1:1], RW, default = 0x0
    //
    UINTX dtx_cc_pbdstaticlegen : 1;
    //
    // Per bit deskew enable - TX - BL
    //
    // Bits[2:2], RW, default = 0x0
    //
    UINTX tx_bl_pbdstaticlegen : 1;
    //
    // Per bit deskew enable - TX - CC
    //
    // Bits[3:3], RW, default = 0x0
    //
    UINTX tx_cc_pbdstaticlegen : 1;
    //
    // Reserved bits
    //
    // Bits[21:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved627 : 18;
#else
    UINTX reserved : 18;
#endif // MEM_SV_HOOKS
    //
    // Rx Per Bit De-skew enable (1=enable, 0=disable)
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX rxdll_bl_rxpbden : 1;
    //
    // Rx Per Bit De-skew enable (1=enable, 0=disable), 0 for cc always
    //
    // Bits[23:23], RW, default = 0x0
    //
    UINTX rxdll_cc_rxpbden : 1;
    //
    // Reserved bits
    //
    // Bits[31:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved626 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPBDCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXPBDRK0CTL1_DQCCC_8_REG                          0x000000C4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[4:0], RW, default = 0x0
    //
    UINTX tx_bldm_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[31:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved628 : 27;
#else
    UINTX reserved : 27;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPBDRK0CTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXPBDRK0CTL0_DQCCC_8_REG                          0x000000C8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[4:0], RW, default = 0x0
    //
    UINTX dtx_bl_dqspbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved631 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[15:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved630 : 8;
#else
    UINTX reserved_1 : 8;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[20:16], RW, default = 0x0
    //
    UINTX dtx_bl_dqsbpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[31:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved629 : 11;
#else
    UINTX reserved_2 : 11;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPBDRK0CTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXPBDBLCTL1_DQCCC_8_REG                           0x000000CC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[4:0], RW, default = 0x0
    //
    UINTX tx_bldq4_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved635 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[12:8], RW, default = 0x0
    //
    UINTX tx_bldq5_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[15:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved634 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[20:16], RW, default = 0x0
    //
    UINTX tx_bldq6_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved633 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[28:24], RW, default = 0x0
    //
    UINTX tx_bldq7_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[31:29], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved632 : 3;
#else
    UINTX reserved_3 : 3;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPBDBLCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXPBDBLCTL0_DQCCC_8_REG                           0x000000D0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[4:0], RW, default = 0x0
    //
    UINTX tx_bldq0_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved639 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[12:8], RW, default = 0x0
    //
    UINTX tx_bldq1_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[15:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved638 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[20:16], RW, default = 0x0
    //
    UINTX tx_bldq2_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved637 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // Tx Per Bit De-skew code for dq0 valid codes
    //
    // Bits[28:24], RW, default = 0x0
    //
    UINTX tx_bldq3_txpbdcode : 5;
    //
    // Reserved bits
    //
    // Bits[31:29], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved636 : 3;
#else
    UINTX reserved_3 : 3;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPBDBLCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define OFFSETCNCLCTRL1_DQCCC_8_REG                       0x000000D4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX rx_bldm_ampoffset : 4;
    //
    // Offset cancellation code (DQS buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX drx_bl_ampoffset : 4;
    //
    // tuning bits for offset cancellation (can be tied off for CC)
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX rx_cc_ampoffset : 4;
    //
    // tuning bits for offset cancellation (can be tied off for CC)
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX drx_ccck0_ampoffset : 4;
    //
    // DQ Offset cancellation enable for Rx (1=enable, 0=disable)
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX rx_bl_ampoffseten : 1;
    //
    // DQS Offset cancellation enable for Rx (1=enable, 0=disable)
    //
    // Bits[17:17], RW, default = 0x1
    //
    UINTX drx_bl_ampoffseten : 1;
    //
    // Offset cancellation enable for Rx (not used for cc)
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX rx_cc_ampoffseten : 1;
    //
    // Offset cancellation enable for Rx (not used for cc)
    //
    // Bits[19:19], RW, default = 0x1
    //
    UINTX drx_cc_ampoffseten : 1;
    //
    // Reserved bits
    //
    // Bits[31:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved640 : 12;
#else
    UINTX reserved : 12;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} OFFSETCNCLCTRL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define OFFSETCNCLCTRL0_DQCCC_8_REG                       0x000000D8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX rx_bldq7_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX rx_bldq6_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX rx_bldq5_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX rx_bldq4_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[19:16], RW, default = 0x0
    //
    UINTX rx_bldq3_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[23:20], RW, default = 0x0
    //
    UINTX rx_bldq2_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rx_bldq1_ampoffset : 4;
    //
    // Offset cancellation code (DM/DQ buffers)[br]1100 -4[br]1011 -3[br]1010 -2[br]1001
    // -1[br]0000 0[br]0001 +1[br]0010 +2[br]0011 +3[br]0100 +4
    //
    // Bits[31:28], RW, default = 0x0
    //
    UINTX rx_bldq0_ampoffset : 4;
  } Bits;
  UINTX Data;
} OFFSETCNCLCTRL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CTLECTL2_DQCCC_8_REG                              0x000000DC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DRX N Bias Control - BL
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX drx_bl_nbiasctrl : 2;
    //
    // DRX Bias Command Control - BL
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX drx_bl_casbiasctrl : 2;
    //
    // Reserved bits
    //
    // Bits[31:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved641 : 28;
#else
    UINTX reserved : 28;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CTLECTL2_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CTLECTL1_DQCCC_8_REG                              0x000000E0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[1:0], RW, default = 0x1
    //
    UINTX rx_bldm_ctlecap : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX rx_bldm_ctleres : 2;
    //
    // bias control for ctle amp[br]00 2k&          0.25xnom[br]01 1k&          0.4xnom[br]10
    // 0.5k&     1xnom[br]11 0.25k&  1.7xnom
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX rx_bl_ctlebiasctrl : 2;
    //
    // Enable CTLE for DQ (1=enable, 0=disable)
    //
    // Bits[6:6], RW, default = 0x1
    //
    UINTX rx_bl_ctleen : 1;
    //
    // DRX Bias Enable - BL
    //
    // Bits[7:7], RW, default = 0x1
    //
    UINTX drx_bl_biasen : 1;
    //
    // cap tuning;00 = 0.5pf;01 = 1pf;10 = 1.5pf;11 = 2pf
    //
    // Bits[9:8], RW, default = 0x1
    //
    UINTX drx_bl_ctlecap : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX drx_bl_ctleres : 2;
    //
    // bias control for ctle amp[br]00 2k&          0.25xnom[br]01 1k&          0.4xnom[br]10
    // 0.5k&     1xnom[br]11 0.25k&  1.7xnom
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX drx_bl_ctlebiasctrl : 3;
    //
    // Enable CTLE for DQS (1=enable, 0=disable)
    //
    // Bits[15:15], RW, default = 0x1
    //
    UINTX drx_bl_ctleen : 1;
    //
    // cap tuning;00 = 0.5pf;01 = 1pf;10 = 1.5pf;11 = 2pf
    //
    // Bits[17:16], RW, default = 0x1
    //
    UINTX rx_cc_ctlecap : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX rx_cc_ctleres : 2;
    //
    // bias control for ctle amp[br]00 2k&          0.25xnom[br]01 1k&          0.4xnom[br]10
    // 0.5k&     1xnom[br]11 0.25k&  1.7xnom
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX rx_cc_ctlebiasctrl : 2;
    //
    // Enable CTLE (not used for CC)
    //
    // Bits[22:22], RW, default = 0x1
    //
    UINTX rx_cc_ctleen : 1;
    //
    // DRX Bias Ctle Control - CC
    //
    // Bits[25:23], RW, default = 0x0
    //
    UINTX drx_cc_ctlebiasctrl : 3;
    //
    // DRX N Bias Control - CC
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX drx_cc_nbiasctrl : 2;
    //
    // DRX Bias Command Control - CC
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX drx_cc_casbiasctrl : 2;
    //
    // DRX Bias Enable - CC
    //
    // Bits[30:30], RW, default = 0x1
    //
    UINTX drx_cc_biasen : 1;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved642 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} CTLECTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define CTLECTL0_DQCCC_8_REG                              0x000000E4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[1:0], RW, default = 0x1
    //
    UINTX rx_bldq7_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[3:2], RW, default = 0x1
    //
    UINTX rx_bldq6_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[5:4], RW, default = 0x1
    //
    UINTX rx_bldq5_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[7:6], RW, default = 0x1
    //
    UINTX rx_bldq4_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[9:8], RW, default = 0x1
    //
    UINTX rx_bldq3_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[11:10], RW, default = 0x1
    //
    UINTX rx_bldq2_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[13:12], RW, default = 0x1
    //
    UINTX rx_bldq1_ctlecap : 2;
    //
    // cap tuning;00 = 0.1pf;01 = 0.2pf;10 = 0.3pf;11 = 0.4pf
    //
    // Bits[15:14], RW, default = 0x1
    //
    UINTX rx_bldq0_ctlecap : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX rx_bldq7_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX rx_bldq6_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX rx_bldq5_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[23:22], RW, default = 0x0
    //
    UINTX rx_bldq4_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX rx_bldq3_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX rx_bldq2_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[29:28], RW, default = 0x0
    //
    UINTX rx_bldq1_ctleres : 2;
    //
    // resistor tuning;00=0.5kohm;01=1kohm;10=1.5kohm;11=2kohm
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX rx_bldq0_ctleres : 2;
  } Bits;
  UINTX Data;
} CTLECTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TCOCTL_DQCC_DQCCC_8_REG                           0x000000E8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQ TCO comp rise code - CC Lane
    //
    // Bits[4:0], RW, default = 0x1F
    //
    UINTX tx_cc_tcorise : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved646 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // DQ TCO comp fall code - CC Lane
    //
    // Bits[12:8], RW, default = 0x1F
    //
    UINTX tx_cc_tcofall : 5;
    //
    // Reserved bits
    //
    // Bits[14:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved645 : 2;
#else
    UINTX reserved_1 : 2;
#endif // MEM_SV_HOOKS
    //
    // TX TCO enable - CC
    //
    // Bits[15:15], RW, default = 0x0
    //
    UINTX tx_cc_tcostaticlegen : 1;
    //
    // DQ TCO comp rise code - DQ Lane
    //
    // Bits[20:16], RW, default = 0x1F
    //
    UINTX tx_bl_tcorise : 5;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved644 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // DQ TCO comp fall code - DQ Lane
    //
    // Bits[28:24], RW, default = 0x1F
    //
    UINTX tx_bl_tcofall : 5;
    //
    // Reserved bits
    //
    // Bits[30:29], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved643 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // TX TCO enable - BL
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX tx_bl_tcostaticlegen : 1;
  } Bits;
  UINTX Data;
} TCOCTL_DQCC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TCOCTL_DQ_DQCCC_8_REG                             0x000000EC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQS TCO comp rise code - DQ Lane
    //
    // Bits[4:0], RW, default = 0x1F
    //
    UINTX dtx_bl_tcorisedqs : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved650 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // DQSB TCO comp rise code - DQ Lane
    //
    // Bits[12:8], RW, default = 0x1F
    //
    UINTX dtx_bl_tcofalldqs : 5;
    //
    // Reserved bits
    //
    // Bits[15:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved649 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // DQS TCO comp fall code - DQ Lane
    //
    // Bits[20:16], RW, default = 0x1F
    //
    UINTX dtx_bl_tcorisedqsb : 5;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved648 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // DQSB TCO comp fall code - DQ Lane
    //
    // Bits[28:24], RW, default = 0x1F
    //
    UINTX dtx_bl_tcofalldqsb : 5;
    //
    // DTX TCO enable - BL
    //
    // Bits[29:29], RW, default = 0x0
    //
    UINTX dtx_bl_tcostaticlegen : 1;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved647 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TCOCTL_DQ_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TCOCTL_CC_DQCCC_8_REG                             0x000000F0
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQS TCO comp rise code - CC Lane
    //
    // Bits[4:0], RW, default = 0x1F
    //
    UINTX dtx_cc_tcorisedqs : 5;
    //
    // Reserved bits
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved654 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // DQS TCO comp fall code - CC Lane
    //
    // Bits[12:8], RW, default = 0x1F
    //
    UINTX dtx_cc_tcofalldqs : 5;
    //
    // Reserved bits
    //
    // Bits[15:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved653 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // DQSB TCO comp rise code - CC Lane
    //
    // Bits[20:16], RW, default = 0x1F
    //
    UINTX dtx_cc_tcorisedqsb : 5;
    //
    // Reserved bits
    //
    // Bits[23:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved652 : 3;
#else
    UINTX reserved_2 : 3;
#endif // MEM_SV_HOOKS
    //
    // DQSB TCO comp fall code - CC Lane
    //
    // Bits[28:24], RW, default = 0x1F
    //
    UINTX dtx_cc_tcofalldqsb : 5;
    //
    // DTX TCO enable - CC
    //
    // Bits[29:29], RW, default = 0x0
    //
    UINTX dtx_cc_tcostaticlegen : 1;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved651 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TCOCTL_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TXDLLSIGGRP6CTL_DQCCC_8_REG                       0x000000F4
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Read pointer offset for sig group 6 rank 0
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp06rdptroffset : 4;
    //
    // Dead band select for sig grp 6 rank 0
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX txdll_rk0siggrp06dbsel : 2;
    //
    // Read pointer offset for sig group 6 rank 1
    //
    // Bits[9:6], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp06rdptroffset : 4;
    //
    // Dead band select for sig grp 6 rank 1
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX txdll_rk1siggrp06dbsel : 2;
    //
    // Reserved bits
    //
    // Bits[31:12], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved655 : 20;
#else
    UINTX reserved : 20;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXDLLSIGGRP6CTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LDOCTL0_DQCCC_8_REG                               0x000000F8
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Read pointer offset for second port for LDO integration - DQ - RK1.
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX ckdrvsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQ - RK0.
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX rk0dqsdrvsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQ - RK1.
    //
    // Bits[11:8], RW, default = 0x0
    //
    UINTX rk1dqsdrvsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQ - RK0.
    //
    // Bits[15:12], RW, default = 0x0
    //
    UINTX rk0dqsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQ - RK1.
    //
    // Bits[19:16], RW, default = 0x0
    //
    UINTX rk1dqsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQDMDRV - RK0.
    //
    // Bits[23:20], RW, default = 0x0
    //
    UINTX rk0dqdrvsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQDMDRV - RK1.
    //
    // Bits[27:24], RW, default = 0x0
    //
    UINTX rk1dqdrvsecondrdportref2xrdptroffset : 4;
    //
    // Reserved bits
    //
    // Bits[31:28], RW, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved656 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LDOCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LDOCTL1_DQCCC_8_REG                               0x000000FC
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Read pointer offset for second port for LDO integration - DQ - RK1.
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX cmddrvsecondrdportref2xrdptroffset : 4;
    //
    // Read pointer offset for second port for LDO integration - DQ - RK0.
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX cssecondrdportref2xrdptroffset : 4;
    //
    // Reserved bits
    //
    // Bits[31:8], RW, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved657 : 24;
#else
    UINTX reserved : 24;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LDOCTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define PTDATAMUXSEL_DQCCC_8_REG                          0x00000100
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Reserved bits
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX data7sel : 3;
    //
    // Reserved bits
    //
    // Bits[5:3], RW, default = 0x0
    //
    UINTX data6sel : 3;
    //
    // Reserved bits
    //
    // Bits[8:6], RW, default = 0x0
    //
    UINTX data5sel : 3;
    //
    // Reserved bits
    //
    // Bits[11:9], RW, default = 0x0
    //
    UINTX data4sel : 3;
    //
    // Reserved bits
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX data3sel : 3;
    //
    // Reserved bits
    //
    // Bits[17:15], RW, default = 0x0
    //
    UINTX data2sel : 3;
    //
    // Reserved bits
    //
    // Bits[20:18], RW, default = 0x0
    //
    UINTX data1sel : 3;
    //
    // Reserved bits
    //
    // Bits[23:21], RW, default = 0x0
    //
    UINTX data0sel : 3;
    //
    // Reserved bits
    //
    // Bits[31:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved658 : 8;
#else
    UINTX reserved : 8;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} PTDATAMUXSEL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LPMODECTL0_DQCCC_8_REG                            0x00000104
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Low Power Mode Delays
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX lpmode_dasrtref2xrstb_dly : 3;
    //
    // Reserved bits
    //
    // Bits[3:3], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved667 : 1;
#else
    UINTX reserved : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[6:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved666 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[7:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved665 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[10:8], RW, default = 0x0
    //
    UINTX lpmode_asrtref2xrstb_dly : 3;
    //
    // Reserved bits
    //
    // Bits[11:11], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved664 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[14:12], RW, default = 0x0
    //
    UINTX lpmode_dasrtwrptren_dly : 3;
    //
    // Reserved bits
    //
    // Bits[15:15], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved663 : 1;
#else
    UINTX reserved_4 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[18:16], RW, default = 0x0
    //
    UINTX lpmode_dasrtiobufact_dly : 3;
    //
    // Reserved bits
    //
    // Bits[19:19], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved662 : 1;
#else
    UINTX reserved_5 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[22:20], RW, default = 0x0
    //
    UINTX lpmode_asrtdqcmddrvenb_dly : 3;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved661 : 1;
#else
    UINTX reserved_6 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[26:24], RW, default = 0x0
    //
    UINTX lpmode_asrtdqcmdsigpien_dly : 3;
    //
    // Reserved bits
    //
    // Bits[27:27], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved660 : 1;
#else
    UINTX reserved_7 : 1;
#endif // MEM_SV_HOOKS
    //
    // Low Power Mode Delays
    //
    // Bits[30:28], RW, default = 0x0
    //
    UINTX lpmode_dasrtdqcmddrvenb_dly : 3;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved659 : 1;
#else
    UINTX reserved_8 : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LPMODECTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LPMODECTL1_DQCCC_8_REG                            0x00000108
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Low Power Mode Delays
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX lpmode_asrtref2xsigpien_dly : 8;
    //
    // Reserved bits
    //
    // Bits[15:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved671 : 8;
#else
    UINTX reserved : 8;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX lpmode_ref2xrstbovrd : 1;
    //
    // Reserved bits
    //
    // Bits[17:17], RW, default = 0x0
    //
    UINTX lpmode_wrptrenovrd : 1;
    //
    // Reserved bits
    //
    // Bits[18:18], RW, default = 0x0
    //
    UINTX siggrpen_lpmode2or3ovrd : 1;
    //
    // Reserved bits
    //
    // Bits[19:19], RW, default = 0x0
    //
    UINTX siggrpen_lpmode1ovrd : 1;
    //
    // Reserved bits
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX lpmode_ref2xclkenovrd : 1;
    //
    // Reserved bits
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX lpmode_iobufactovrd : 1;
    //
    // Reserved bits
    //
    // Bits[22:22], RW, default = 0x0
    //
    UINTX lpmode_dqcmddrvenbovrd : 1;
    //
    // Reserved bits
    //
    // Bits[23:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved670 : 1;
#else
    UINTX reserved_1 : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[26:24], RW, default = 0x0
    //
    UINTX lpmode_asrtiobufact_dly : 3;
    //
    // Reserved bits
    //
    // Bits[27:27], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved669 : 1;
#else
    UINTX reserved_2 : 1;
#endif // MEM_SV_HOOKS
    //
    // Reserved bits
    //
    // Bits[30:28], RW, default = 0x0
    //
    UINTX lpmode_asrtwrptren_dly : 3;
    //
    // Reserved bits
    //
    // Bits[31:31], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved668 : 1;
#else
    UINTX reserved_3 : 1;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LPMODECTL1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define RXDLLRDCMDCTL0_DQCCC_8_REG                        0x0000010C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Read active length.
    //
    // Bits[5:0], RW, default = 0x0
    //
    UINTX rdactivelen : 6;
    //
    // Read command to active delay.
    //
    // Bits[11:6], RW, default = 0x0
    //
    UINTX rdcmd2rdactive : 6;
    //
    // RXDLL update enable.
    //
    // Bits[17:12], RW, default = 0x0
    //
    UINTX rxdllupdtlen : 6;
    //
    // Read command to RXDLL update.
    //
    // Bits[23:18], RW, default = 0x0
    //
    UINTX rdcmd2rxdllupdt : 6;
    //
    // Reserved bits
    //
    // Bits[31:24], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved672 : 8;
#else
    UINTX reserved : 8;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} RXDLLRDCMDCTL0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_RESET_DQCCC_8_REG                         0x00000600
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Power-on or software reset0: reset state1: norma operationSet to 1 to enter normal
    // operation.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX dllcomprst_b : 1;
    //
    // Reserved
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_RESET_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_INIT_DQCCC_8_REG                          0x00000604
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Activates initialization training sequence.0: initialization training sequence
    // not active.1: initialization training sequence active.Self clearing after successful
    // initialization training sequence.
    //
    // Bits[0:0], RW/1S/V, default = 0x0
    //
    UINTX txdll_init : 1;
    //
    // Reserved
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_INIT_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_LPDDRTYPE_DQCCC_8_REG                     0x00000608
#ifndef ASM_INC
typedef union {
  struct {
    //
    // LPDDR speed target. 0: LPDDR3, 1600 MHz. 1: LPDDR4, 3200 MHz.
    //
    // Bits[0:0], RW, default = 0x0
    //
    UINTX lpddr_type : 1;
    //
    // Reserved
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_LPDDRTYPE_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_CLKUNGATE_DQCCC_8_REG                     0x0000060C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Internal Clock control. 0 = allow clock gating for low power; 1 = allow free running
    // clock
    //
    // Bits[0:0], RW, default = 0x1
    //
    UINTX clkungate : 1;
    //
    // Reserved
    //
    // Bits[31:1], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 31;
#else
    UINTX reserved : 31;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_CLKUNGATE_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_ERROR_STATUS_DQCCC_8_REG                  0x00000610
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Set when Initialization sequence is successful. Cleared on a read or writing a
    // 0.
    //
    // Bits[0:0], RO/V, default = 0x0
    //
    UINTX txdll_init_done : 1;
    //
    // Set when initialization or fine periodic training passed the locking condition.
    // Valid only when scr_lockseq_en is set to 1.Cleared on a read or writing a 0.
    //
    // Bits[1:1], RO/V, default = 0x0
    //
    UINTX txdll_lock_flag : 1;
    //
    // Reserved
    //
    // Bits[3:2], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved0 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
    //
    // Set when Delay Cell count exceeds maximum value. Cleared on a read or writing
    // a 0.
    //
    // Bits[4:4], RO/V, default = 0x0
    //
    UINTX txdll_error : 1;
    //
    // Set when coarse code underflow or overflow occurs. Cleared on a read or writing
    // a 0.
    //
    // Bits[5:5], RO/V, default = 0x0
    //
    UINTX txdll_coarsep_error : 1;
    //
    // Set when fine code underflow or overflow occurs. Cleared on a read or writing
    // a 0.
    //
    // Bits[6:6], RO/V, default = 0x0
    //
    UINTX txdll_fine_error : 1;
    //
    // Set when initialization or fine periodic training failed the locking condition.
    // Cleared on a read or writing a 0.
    //
    // Bits[7:7], RO/V, default = 0x0
    //
    UINTX txdll_lock_error : 1;
    //
    // Reserved
    //
    // Bits[31:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved_1 : 24;
#else
    UINTX reserved_1 : 24;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_ERROR_STATUS_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_CODE_STATUS_DQCCC_8_REG                   0x00000614
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Fine code value during normal operation
    //
    // Bits[7:0], RO/V, default = 0x7F
    //
    UINTX txdll_stat_finecode : 8;
    //
    // Coarse code value during normal operation
    //
    // Bits[11:8], RO/V, default = 0xF
    //
    UINTX txdll_stat_coarsecode : 4;
    //
    // Coarse code value at initialization training sequence completion.
    //
    // Bits[15:12], RO/V, default = 0xF
    //
    UINTX txdll_stat_coarseicode : 4;
    //
    // Delay Cell count value during normal operation
    //
    // Bits[20:16], RO/V, default = 0x7
    //
    UINTX txdll_stat_fbmuxsel : 5;
    //
    // Reserved
    //
    // Bits[31:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 11;
#else
    UINTX reserved : 11;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_CODE_STATUS_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_LOCKCTL_DQCCC_8_REG                       0x00000618
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Maximum samples to count for lock condition. The counter decrements until the
    // value reaches 0 yielding scr_lockcount_size + 1 samples.
    //
    // Bits[3:0], RW, default = 0x3
    //
    UINTX lockcount_size : 4;
    //
    // Reserved
    //
    // Bits[29:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 26;
#else
    UINTX reserved : 26;
#endif // MEM_SV_HOOKS
    //
    // Ignore lock error. Valid when scr_lockseq_en = 1. 0: Exit and wait for initialization
    // training sequence. 1: ignore lock error and proceed to normal operation.
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX lockerror_ignore : 1;
    //
    // Enable lock sequence after initialization or fine periodic training sequence.
    // 0: disable lock sequence. 1: enable lock sequence.
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX lock_seq_en : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_LOCKCTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FBMUXSEL_DQCCC_8_REG                      0x0000061C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Delay cell minimum count value. Default start code.
    //
    // Bits[4:0], RW, default = 0x9
    //
    UINTX muxcode_min : 5;
    //
    // Reserved
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved1 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // Delay cell maximum count value. Used in overflow check
    //
    // Bits[12:8], RW, default = 0x1B
    //
    UINTX muxcode_max : 5;
    //
    // Reserved
    //
    // Bits[15:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved0 : 3;
#else
    UINTX reserved_1 : 3;
#endif // MEM_SV_HOOKS
    //
    // Delay cell fbmuxsel override value.
    //
    // Bits[20:16], RW, default = 0xF
    //
    UINTX muxcode_ovr : 5;
    //
    // Reserved
    //
    // Bits[30:21], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved_2 : 10;
#else
    UINTX reserved_2 : 10;
#endif // MEM_SV_HOOKS
    //
    // Override txdll_fbmuxsel[4:0] with scr_muxcode_ovr[4:0].0: disable override.1:
    // enable override.
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX muxcode_ovr_sel : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_FBMUXSEL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_COARSECODE_DQCCC_8_REG                    0x00000620
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Coarse code minimum count value. Used in underflow check.
    //
    // Bits[3:0], RW, default = 0x2
    //
    UINTX coarsecode_min : 4;
    //
    // Reserved
    //
    // Bits[7:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved2 : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
    //
    // Coarse code maximum count value. Default start value. Used in overflow check
    //
    // Bits[11:8], RW, default = 0xF
    //
    UINTX coarsecode_max : 4;
    //
    // Reserved
    //
    // Bits[15:12], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved1 : 4;
#else
    UINTX reserved_1 : 4;
#endif // MEM_SV_HOOKS
    //
    // Coarse code override value.
    //
    // Bits[19:16], RW, default = 0x2
    //
    UINTX coarsecode_ovr : 4;
    //
    // Reserved
    //
    // Bits[23:20], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved0 : 4;
#else
    UINTX reserved_2 : 4;
#endif // MEM_SV_HOOKS
    //
    // Coarsei code override value.
    //
    // Bits[27:24], RW, default = 0x2
    //
    UINTX coarseicode_ovr : 4;
    //
    // Reserved
    //
    // Bits[29:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved_3 : 2;
#else
    UINTX reserved_3 : 2;
#endif // MEM_SV_HOOKS
    //
    // Override txdll_coarseicode[3:0] with scr_coarseicode_ovr[3:0]. 0: disable override,
    // 1: enable override
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX coarseicode_ovr_sel : 1;
    //
    // Override txdll_coarsecode[3:0] with scr_coarsecode_ovr[3:0]. 0: disable override,
    // 1: enable override
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX coarsecode_ovr_sel : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_COARSECODE_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FINECODE_DQCCC_8_REG                      0x00000624
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Underflow check during fine continuous or fine periodic training.
    //
    // Bits[7:0], RW, default = 0x20
    //
    UINTX finecode_min : 8;
    //
    // Fine code maximum value. Used in overflow check during fine continuous or fine
    // periodic
    //
    // Bits[15:8], RW, default = 0xFF
    //
    UINTX finecode_max : 8;
    //
    // Fine code limit - minimum value. Defines fine code drift allowed towards finecode
    // minimum value before decrementing the coarse code for binary search training.
    //
    // Bits[23:16], RW, default = 0x34
    //
    UINTX finecodemin_limit : 8;
    //
    // Fine code limit - maximum value. Defines fine code drift allowed towards finecode
    // maximum value before incrementing the coarse code for binary search training.
    //
    // Bits[31:24], RW, default = 0xCF
    //
    UINTX finecodemax_limit : 8;
  } Bits;
  UINTX Data;
} DLLCOMP_FINECODE_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FINEMUXSEL_DQCCC_8_REG                    0x00000628
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Two bit field selects one of 4 possible finecode values for each 8:1 mux input.
    // Two bit field shared between -/+ index values (ex. -gf / +gf)
    //
    // Bits[7:0], RW, default = 0xE8
    //
    UINTX finecode_muxsel : 8;
    //
    // Reserved
    //
    // Bits[31:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 24;
#else
    UINTX reserved : 24;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_FINEMUXSEL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FINECTL_DQCCC_8_REG                       0x0000062C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Fine code start value to apply during binary search training
    //
    // Bits[7:0], RW, default = 0x7F
    //
    UINTX finecode_start : 8;
    //
    // Reserved
    //
    // Bits[28:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 21;
#else
    UINTX reserved : 21;
#endif // MEM_SV_HOOKS
    //
    // Propriety search algorithm version to apply during lock, fine continuous and fine
    // periodic training. 0: Apply short version. 1: Apply long version.
    //
    // Bits[29:29], RW, default = 0x0
    //
    UINTX searchalgo_long : 1;
    //
    // Fine priodic training enable after initialization training. 0: Fine periodic disable
    // (default). 1: Fine periodic enable. When both finec_mode and finep_mode are set
    // to 1, dllcomp functionality is unpredictable.
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX finep_mode : 1;
    //
    // Fine continuous training enable after initialization training. 0: Fine continuous
    // training disable. 1: Fine continuous training enable (default). When both finec_mode
    // and finep_mode are set to 1, dllcomp functionality is unpredictable.
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX finec_mode : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_FINECTL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FINECODEOVR_DQCCC_8_REG                   0x00000630
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Fine code to override with when scr_finecode_ovr_sel = 1
    //
    // Bits[7:0], RW, default = 0x13
    //
    UINTX finecode_ovr : 8;
    //
    // Reserved
    //
    // Bits[29:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 22;
#else
    UINTX reserved : 22;
#endif // MEM_SV_HOOKS
    //
    // When scr_finecode_ovr_sel is active high, this bit drives txdll_dllupdateovr
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX finecode_updateovr_ovr : 1;
    //
    // Fine code override select. 0: Apply computed fine code (default). 1: Apply scr_finecode_ovr[7:0]
    // .
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX finecode_ovr_sel : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_FINECODEOVR_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_FPTIMER_DQCCC_8_REG                       0x00000634
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Fine periodic interval timer start value. Defines the interval between back to
    // back fine periodic training sequences.
    //
    // Bits[15:0], RW, default = 0x3FFF
    //
    UINTX finepwait_interval : 16;
    //
    // Reserved
    //
    // Bits[31:16], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 16;
#else
    UINTX reserved : 16;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_FPTIMER_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_SAMPLEWAIT_DQCCC_8_REG                    0x00000638
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Wait interval between code update and sample ready. Applied during coarse initialization
    // and fine
    //
    // Bits[4:0], RW, default = 0x2
    //
    UINTX samplewait_min : 5;
    //
    // Reserved
    //
    // Bits[7:5], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved0 : 3;
#else
    UINTX reserved : 3;
#endif // MEM_SV_HOOKS
    //
    // Wait interval between code update and sample ready. Applied during lock, fine
    // continuous and fine
    //
    // Bits[12:8], RW, default = 0x6
    //
    UINTX samplewait_max : 5;
    //
    // Reserved
    //
    // Bits[31:13], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved_1 : 19;
#else
    UINTX reserved_1 : 19;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DLLCOMP_SAMPLEWAIT_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_RXCOARSEOVR_DQCCC_8_REG                   0x0000063C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Coarse code override value for rxdll_coarsecode[3:0]
    //
    // Bits[3:0], RW, default = 0x2
    //
    UINTX rxdll_coarsecode_ovr : 4;
    //
    // Reserved
    //
    // Bits[30:4], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 27;
#else
    UINTX reserved : 27;
#endif // MEM_SV_HOOKS
    //
    // Override rxdll_coarsecode[3:0] with scr_rxdll_coarsecode_ovr[3:0] . 0: disable
    // override. 1: enable override.
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX rxdll_coarsecode_ovr_sel : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_RXCOARSEOVR_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DLLCOMP_RXFINEOVR_DQCCC_8_REG                     0x00000640
#ifndef ASM_INC
typedef union {
  struct {
    //
    // FIne code override value for rxdll_finecode[7:0]
    //
    // Bits[7:0], RW, default = 0x13
    //
    UINTX rxdll_finecode_ovr : 8;
    //
    // Reserved
    //
    // Bits[29:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 22;
#else
    UINTX reserved : 22;
#endif // MEM_SV_HOOKS
    //
    // When scr_rxdll_finecode_ovr_sel is active high, this bit drives rxdll_rxdqsdllupdateovr
    //
    // Bits[30:30], RW, default = 0x0
    //
    UINTX rxdll_finecode_updateovr_ovr : 1;
    //
    // Override rxdll_finecode[7:0] with scr_rxdll_finecode_ovr[7:0] . 0: disable override.
    // 1: enable override.
    //
    // Bits[31:31], RW, default = 0x0
    //
    UINTX rxdll_finecode_ovr_sel : 1;
  } Bits;
  UINTX Data;
} DLLCOMP_RXFINEOVR_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LOWER_DFXVISAULM0_DQCCC_8_REG                     0x00000700
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Lower Level ULM0 Lane (DATA) Select
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dfxlowervisaulm0_datasel : 8;
    //
    // VISA Lower Level ULM0 CLK Select
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dfxlowervisaulm0_clksel : 8;
    //
    // VISA Lower Level Bypass ULM0
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dfxlowervisaulm0_bypass : 1;
    //
    // Reserved
    //
    // Bits[31:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved673 : 15;
#else
    UINTX reserved : 15;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LOWER_DFXVISAULM0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LOWER_DFXVISAULM1_DQCCC_8_REG                     0x00000704
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Lower Level ULM1 Lane (DATA) Select
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dfxlowervisaulm1_datasel : 8;
    //
    // VISA Lower Level ULM1 CLK Select
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dfxlowervisaulm1_clksel : 8;
    //
    // VISA Lower Level Bypass ULM1
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dfxlowervisaulm1_bypass : 1;
    //
    // Reserved
    //
    // Bits[31:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved674 : 15;
#else
    UINTX reserved : 15;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LOWER_DFXVISAULM1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define LOWER_DFXVISAPATTGEN_DQCCC_8_REG                  0x00000708
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Lower Level ULM Pattern Generation
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX dfxlowervisapattgen_lsb : 4;
    //
    // VISA Lower Level ULM Pattern Generation
    //
    // Bits[6:4], RW, default = 0x0
    //
    UINTX dfxlowervisapattgen_msb : 3;
    //
    // Reserved
    //
    // Bits[31:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved675 : 25;
#else
    UINTX reserved : 25;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} LOWER_DFXVISAPATTGEN_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TOP_DFXVISAULM0_DQCCC_8_REG                       0x0000070C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Top Level ULM0 Lane (DATA) Select
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dfxtopvisaulm0_datasel : 8;
    //
    // VISA Top Level ULM0 CLK Select
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dfxtopvisaulm0_clksel : 8;
    //
    // VISA Top Level Bypass ULM0
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dfxtopvisaulm0_bypass : 1;
    //
    // Reserved
    //
    // Bits[31:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved676 : 15;
#else
    UINTX reserved : 15;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TOP_DFXVISAULM0_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TOP_DFXVISAULM1_DQCCC_8_REG                       0x00000710
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Top Level ULM1 Lane (DATA) Select
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX dfxtopvisaulm1_datasel : 8;
    //
    // VISA Top Level ULM1 CLK Select
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX dfxtopvisaulm1_clksel : 8;
    //
    // VISA Top Level Bypass ULM1
    //
    // Bits[16:16], RW, default = 0x0
    //
    UINTX dfxtopvisaulm1_bypass : 1;
    //
    // Reserved
    //
    // Bits[31:17], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved677 : 15;
#else
    UINTX reserved : 15;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TOP_DFXVISAULM1_DQCCC_8_STRUCT;
#endif // ASM_INC

#define TOP_DFXVISAPATTGEN_DQCCC_8_REG                    0x00000714
#ifndef ASM_INC
typedef union {
  struct {
    //
    // VISA Top Level ULM Pattern Generation
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX dfxtopvisapattgen_lsb : 4;
    //
    // VISA Top Level ULM Pattern Generation
    //
    // Bits[6:4], RW, default = 0x0
    //
    UINTX dfxtopvisapattgen_msb : 3;
    //
    // Reserved
    //
    // Bits[31:7], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved678 : 25;
#else
    UINTX reserved : 25;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TOP_DFXVISAPATTGEN_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DFXMRG_BL_DQCCC_8_REG                             0x00000718
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase3 Fall Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 3 Rise SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdfall_ph3 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase2 Fall Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 2 Fall SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdfall_ph2 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase1 Fall Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 1 Fall SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdfall_ph1 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase0 Fall Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 0 Fall SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[7:6], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdfall_ph0 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase1 Rise Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 3 Rise SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdrise_ph3 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase0 Rise Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 2 Rise SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdrise_ph2 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase1 Rise Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 1 Rise SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdrise_ph1 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQ Phase0 Rise Tx SubFIFO load value. The value
    // loaded will apply to all DQ BL Phase 0 Rise SubFIFOs  There isnt a valid or invalid
    // value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdrise_ph0 : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQS/B Fall Tx SubFIFO data load value. The value
    // loaded will apply to all DQ Bytelane DQS/B Fall SubFIFOs. Recommended setting
    // for HVM = 2'b00
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdqsfall : 2;
    //
    // DQ Bytelane Tx FIFO Margin Mode DQS/B Rise Tx SubFIFO data load value. The value
    // loaded will apply to all DQ Bytelane DQS/B Rise SubFIFOs. Recommended setting
    // for HVM = 2'b00
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxdqsrise : 2;
    //
    // DQ Bytelane TxFIFO Margin Mode Latch enable override value used to make TxFIFO
    // latches transparent allowing the loading of all TxFIFO Latches with data programmed
    // in DFXMRGTXFIFO[23:0], if dfxmrgtxovren is asserted(1'b1)  1'b1 - All DQ/DM/DQS
    // Tx FIFO Latches Transparent  0'b0 - All DQ/DM/DQS TxFIFO Latches CLosed
    //
    // Bits[20:20], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxwren : 1;
    //
    // DQ Bytelane Tx FIFO Margin Mode config override enable for Tx data path and Latch
    // enables via  DFXMRGTXFIFO[24:0]    1'b1  - Enable TxFIFO DFx overrides on Latch
    // enables and TxData  1'b0 - Functional Mode of operation
    //
    // Bits[21:21], RW, default = 0x0
    //
    UINTX bl_dfxmrgtxovren : 1;
    //
    // Reserved bits
    //
    // Bits[31:22], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved679 : 10;
#else
    UINTX reserved : 10;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DFXMRG_BL_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DFXMRG_CC_DQCCC_8_REG                             0x0000071C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase3 SDR Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 3 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdsdr_ph3 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase2 SDR Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 2 SDR SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b11
    //
    // Bits[3:2], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdsdr_ph2 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase1 SDR Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 1 SDR SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[5:4], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdsdr_ph1 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase0 SDR Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 0 SDR SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[7:6], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdsdr_ph0 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase3 Fall Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 3 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[9:8], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdfall_ph3 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase2 Fall Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 2 Fall SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[11:10], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdfall_ph2 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase1 Fall Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 1 Fall SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[13:12], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdfall_ph1 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase0 Fall Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 0 Fall SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[15:14], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdfall_ph0 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase1 Rise Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 3 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[17:16], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdrise_ph3 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase0 Rise Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 2 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[19:18], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdrise_ph2 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase1 Rise Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 1 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[21:20], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdrise_ph1 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCC Phase0 Rise Tx SubFIFO load value. The
    // value loaded will apply to all DQCCC BL Phase 0 Rise SubFIFOs  There isnt a valid
    // or invalid value for this field, recommended setting for HVM = 2'b00
    //
    // Bits[23:22], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdrise_ph0 : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCCS/B Fall Tx SubFIFO data load value. The
    // value loaded will apply to all DQCCC Bytelane DQCCCS/B Fall SubFIFOs. Recommended
    // setting for HVM = 2'b00
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdqsfall : 2;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode DQCCCS/B Rise Tx SubFIFO data load value. The
    // value loaded will apply to all DQCCC Bytelane DQCCCS/B Rise SubFIFOs. Recommended
    // setting for HVM = 2'b00
    //
    // Bits[27:26], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxdqsrise : 2;
    //
    // DQCCC Bytelane TxFIFO Margin Mode Latch enable override value used to make TxFIFO
    // latches transparent allowing the loading of all TxFIFO Latches with data programmed
    // in DFXMRGTXFIFO[23:0], if dfxmrgtxovren is asserted(1'b1)  1'b1 - All DQCCC/DM/DQCCCS
    // Tx FIFO Latches Transparent  0'b0 - All DQCCC/DM/DQCCCS TxFIFO Latches CLosed
    //
    // Bits[28:28], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxwren : 1;
    //
    // DQCCC Bytelane Tx FIFO Margin Mode config override enable for Tx data path and
    // Latch enables via  DFXMRGTXFIFO[24:0]    1'b1  - Enable TxFIFO DFx overrides on
    // Latch enables and TxData  1'b0 - Functional Mode of operation
    //
    // Bits[29:29], RW, default = 0x0
    //
    UINTX cc_dfxmrgtxovren : 1;
    //
    // Reserved bits
    //
    // Bits[31:30], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved680 : 2;
#else
    UINTX reserved : 2;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DFXMRG_CC_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DFXMRG_BLRX_DQCCC_8_REG                           0x00000720
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DFx Margin Mode DQ Bytelane Rx Compare value; Used to compare Actual RxFIFO value
    // vs Expected Rx FIFO value(TxFIFO Unloaded)
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX bl_dfxmrgrx_cmpval : 8;
    //
    // Reserved bits
    //
    // Bits[31:8], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved681 : 24;
#else
    UINTX reserved : 24;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DFXMRG_BLRX_DQCCC_8_STRUCT;
#endif // ASM_INC

#define DFXMRG_CCRX_DQCCC_8_REG                           0x00000724
#ifndef ASM_INC
typedef union {
  struct {
    //
    // DFx Margin Mode CCC Bytelane Rx Compare value for SDR pins; Used to compare Actual
    // RxFIFO value vs Expected Rx FIFO value(TxFIFO Unloaded)
    //
    // Bits[7:0], RW, default = 0x0
    //
    UINTX cc_dfxmrgrx_sdrcmpval : 8;
    //
    // DFx Margin Mode CCC Bytelane Rx Compare value for DDR pins; Used to compare Actual
    // RxFIFO value vs Expected Rx FIFO value(TxFIFO Unloaded)
    //
    // Bits[15:8], RW, default = 0x0
    //
    UINTX cc_dfxmrgrx_ddrcmpval : 8;
    //
    // DFx Margin Mode SDR vs DDR Rx Compare Values
    //
    // Bits[24:16], RW, default = 0x0
    //
    UINTX cc_dfxmrgrx_sdrsel : 9;
    //
    // Reserved bits
    //
    // Bits[31:25], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved682 : 7;
#else
    UINTX reserved : 7;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} DFXMRG_CCRX_DQCCC_8_STRUCT;
#endif // ASM_INC


#endif // _DQCCC_8_H_
