/** @file
  File name:    LDO.h
  Input File:   BXTM_SOC.xlsx

  THIS FILE IS AUTO-GENERATED BY THE MMRC TOOL. DO NOT CHANGE THIS CODE.

  If edits are needed in this file, they must be done via the MMRC tool.

  If there is additional project-specific data required by the MMRC, it
  can be placed in LDO.h, which is used for non-tool-generated
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
#ifndef _LDO_H_
#define _LDO_H_

#include "DataTypes.h"

#if USE_64_BIT_VARIABLES
#define UINTX UINT64
#else
#define UINTX UINT32
#endif

#define TXFFDQ0_DVFS_LDO_REG                               0x00000000
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 0
    //
    // Bits[3:0], RW/V, default = 0x0
    //
    UINTX coeff0 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 1
    //
    // Bits[7:4], RW/V, default = 0x0
    //
    UINTX coeff1 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 2
    //
    // Bits[11:8], RW/V, default = 0x0
    //
    UINTX coeff2 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 3
    //
    // Bits[15:12], RW/V, default = 0x0
    //
    UINTX coeff3 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 4
    //
    // Bits[19:16], RW/V, default = 0x0
    //
    UINTX coeff4 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 5
    //
    // Bits[23:20], RW/V, default = 0x0
    //
    UINTX coeff5 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 6
    //
    // Bits[26:24], RW/V, default = 0x0
    //
    UINTX coeff6 : 3;
    //
    // Scale LSB of all coefficient by this number
    //
    // Bits[29:27], RW, default = 0x2
    //
    UINTX coeffscale : 3;
    //
    // Indicates what type of channel this is and what mapping of input DQ pattern to
    // coefficient should be used
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX bgach : 2;
  } Bits;
  UINTX Data;
} TXFFDQ0_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXFFDQ1_DVFS_LDO_REG                               0x00000004
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 0
    //
    // Bits[3:0], RW/V, default = 0x0
    //
    UINTX coeff0 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 1
    //
    // Bits[7:4], RW/V, default = 0x0
    //
    UINTX coeff1 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 2
    //
    // Bits[11:8], RW/V, default = 0x0
    //
    UINTX coeff2 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 3
    //
    // Bits[15:12], RW/V, default = 0x0
    //
    UINTX coeff3 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 4
    //
    // Bits[19:16], RW/V, default = 0x0
    //
    UINTX coeff4 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 5
    //
    // Bits[23:20], RW/V, default = 0x0
    //
    UINTX coeff5 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 6
    //
    // Bits[26:24], RW/V, default = 0x0
    //
    UINTX coeff6 : 3;
    //
    // Scale LSB of all coefficient by this number
    //
    // Bits[29:27], RW, default = 0x2
    //
    UINTX coeffscale : 3;
    //
    // Indicates what type of channel this is and what mapping of input DQ pattern to
    // coefficient should be used
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX bgach : 2;
  } Bits;
  UINTX Data;
} TXFFDQ1_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXFFCA0_DVFS_LDO_REG                               0x00000008
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 0
    //
    // Bits[3:0], RW/V, default = 0x0
    //
    UINTX coeff0 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 1
    //
    // Bits[7:4], RW/V, default = 0x0
    //
    UINTX coeff1 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 2
    //
    // Bits[11:8], RW/V, default = 0x0
    //
    UINTX coeff2 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 3
    //
    // Bits[15:12], RW/V, default = 0x0
    //
    UINTX coeff3 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 4
    //
    // Bits[19:16], RW/V, default = 0x0
    //
    UINTX coeff4 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 5
    //
    // Bits[23:20], RW/V, default = 0x0
    //
    UINTX coeff5 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 6
    //
    // Bits[26:24], RW/V, default = 0x0
    //
    UINTX coeff6 : 3;
    //
    // Scale LSB of all coefficient by this number
    //
    // Bits[29:27], RW, default = 0x2
    //
    UINTX coeffscale : 3;
    //
    // Indicates what type of channel this is and what mapping of input CA pattern to
    // coefficient should be used
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX bgach : 2;
  } Bits;
  UINTX Data;
} TXFFCA0_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXFFCA1_DVFS_LDO_REG                               0x0000000C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 0
    //
    // Bits[3:0], RW/V, default = 0x0
    //
    UINTX coeff0 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 1
    //
    // Bits[7:4], RW/V, default = 0x0
    //
    UINTX coeff1 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 2
    //
    // Bits[11:8], RW/V, default = 0x0
    //
    UINTX coeff2 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 3
    //
    // Bits[15:12], RW/V, default = 0x0
    //
    UINTX coeff3 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 4
    //
    // Bits[19:16], RW/V, default = 0x0
    //
    UINTX coeff4 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 5
    //
    // Bits[23:20], RW/V, default = 0x0
    //
    UINTX coeff5 : 4;
    //
    // Feedforward coefficient for number of PMOS legs to enable for one lane pattern
    // 6
    //
    // Bits[26:24], RW/V, default = 0x0
    //
    UINTX coeff6 : 3;
    //
    // Scale LSB of all coefficient by this number
    //
    // Bits[29:27], RW, default = 0x2
    //
    UINTX coeffscale : 3;
    //
    // Indicates what type of channel this is and what mapping of input CA pattern to
    // coefficient should be used
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX bgach : 2;
  } Bits;
  UINTX Data;
} TXFFCA1_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXTCODE0_DVFS_LDO_REG                              0x00000010
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving CA &
    // DQ bits
    //
    // Bits[5:0], RW/V, default = 0x0
    //
    UINTX tcodecadq : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving DQ bits
    //
    // Bits[11:6], RW/V, default = 0x0
    //
    UINTX tcodedq : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving CA bits
    //
    // Bits[17:12], RW/V, default = 0x0
    //
    UINTX tcodeca : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, idle operation
    //
    // Bits[23:18], RW/V, default = 0x0
    //
    UINTX tcodeidle : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE low, clock stop operation
    //
    // Bits[29:24], RW/V, default = 0x0
    //
    UINTX tcodeclkstop : 6;
    //
    // Read DQ data from FIFO for feedforward path N cycles before reading DQ for transmitter
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX dqrdrdpointersep : 2;
  } Bits;
  UINTX Data;
} TXTCODE0_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXTCODE1_DVFS_LDO_REG                              0x00000014
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving CA &
    // DQ bits
    //
    // Bits[5:0], RW/V, default = 0x0
    //
    UINTX tcodecadq : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving DQ bits
    //
    // Bits[11:6], RW/V, default = 0x0
    //
    UINTX tcodedq : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, driving CA bits
    //
    // Bits[17:12], RW/V, default = 0x0
    //
    UINTX tcodeca : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE high, idle operation
    //
    // Bits[23:18], RW/V, default = 0x0
    //
    UINTX tcodeidle : 6;
    //
    // Number of PMOS legs to enable in feedback regulator for CKE low, clock stop operation
    //
    // Bits[29:24], RW/V, default = 0x0
    //
    UINTX tcodeclkstop : 6;
    //
    // Read DQ data from FIFO for feedforward path N cycles before reading DQ for transmitter
    //
    // Bits[31:30], RW, default = 0x0
    //
    UINTX dqrdrdpointersep : 2;
  } Bits;
  UINTX Data;
} TXTCODE1_DVFS_LDO_STRUCT;
#endif // ASM_INC

#define TXSAOFFSET_LDO_REG                                 0x00000018
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Strong Arm Latch 3 offset. 2s compliment signed offset with step size of Vddq/384
    //
    // Bits[5:0], RW/V, default = 0x0
    //
    UINTX saoffset0 : 6;
    //
    // Strong Arm Latch 3 offset. 2s compliment signed offset with step size of Vddq/384
    //
    // Bits[11:6], RW/V, default = 0x0
    //
    UINTX saoffset1 : 6;
    //
    // Strong Arm Latch 3 offset. 2s compliment signed offset with step size of Vddq/384
    //
    // Bits[17:12], RW/V, default = 0x0
    //
    UINTX saoffset2 : 6;
    //
    // Strong Arm Latch 3 offset. 2s compliment signed offset with step size of Vddq/384
    //
    // Bits[23:18], RW/V, default = 0x0
    //
    UINTX saoffset3 : 6;
    //
    // Trigger sensor offset correction. When set to 1'b1, offset compensation will start.
    // Self reset when compensation is done.
    //
    // Bits[24:24], RW/V, default = 0x0
    //
    UINTX enoffsetcorrection : 1;
    //
    // Reserved bits
    //
    // Bits[31:25], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 7;
#else
    UINTX reserved : 7;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXSAOFFSET_LDO_STRUCT;
#endif // ASM_INC

#define TXFB_LDO_REG                                       0x0000001C
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Enables fully independent tracking modes (default) or consolidates modes (1-3).
    // Modes based on setting = {0: (0,1,2,3,4) 1: (0,1,2-4) 2: (0,1-4) 3: (0-4)}.[br]Ex:
    // A value of 0 enables tracking 5 independent Tcode values in the feedback regulator.[br]Ex:
    // A value of 3 enables tracking a single shared TCode value in the feedback regulator.
    //
    // Bits[1:0], RW, default = 0x0
    //
    UINTX enmode : 2;
    //
    // Enable feedback regulator driver (Disable during feedforward training)
    //
    // Bits[2:2], RW, default = 0x1
    //
    UINTX enfeedbackdrv : 1;
    //
    // Master enable for the feedback/feedforward/panic regulator. If this bit is 0,
    // all parts of the regulator turn off.[br]If BypassRegulator is 1, the value of
    // EnRegulator is don't care and the feedback/feedforward/panic regulator is off.[br]If
    // BypassRegulator and EnRegulator is 0, all PMOS legs turn off and VccFAR is HiZ
    // (Reset Default).
    //
    // Bits[3:3], RW, default = 0x0
    //
    UINTX enregulator : 1;
    //
    // Limit |Code-Tcode| to be <= (MaxRipple+1)
    //
    // Bits[6:4], RW, default = 0x4
    //
    UINTX maxripple : 3;
    //
    // Must have a net number of 4*(SlowBW+1) samples in the same direction before updating
    // Tcode
    //
    // Bits[9:7], RW, default = 0x3
    //
    UINTX slowbw : 3;
    //
    // Must have a net number of (FastBW+1) samples in the same direction before updating
    // Code
    //
    // Bits[12:10], RW, default = 0x3
    //
    UINTX fastbw : 3;
    //
    // Target VccFAR voltage value of Vref = (TargetV+64)/96
    //
    // Bits[17:13], RW, default = 0xA
    //
    UINTX targetv : 5;
    //
    // Turn off the regulator and turn on all PMOS legs to achieve the lowest possible
    // resistance
    //
    // Bits[18:18], RW, default = 0x1
    //
    UINTX bypassregulator : 1;
    //
    // Drive Tcode value from feedback regulator to TxRegFF for {0: Training Off, 1:
    // CACoEff0, & 7: CACoEff6, 8: DQCoEff0, & 14: DQCoEff6, 15: Rsvd}.[br]Writing this
    // to a non-zero value will atuotmatically reset Tcode, Code and the associated Tcode
    // CR to the middle value (one time reset pulse on WrEn).[br]While this register
    // is non-zero, IOSF will NOT be able to write TxRegFF registers.
    //
    // Bits[22:19], RW, default = 0x0
    //
    UINTX enfeedforwardtrain : 4;
    //
    // Reserved bits
    //
    // Bits[31:23], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 9;
#else
    UINTX reserved : 9;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXFB_LDO_STRUCT;
#endif // ASM_INC

#define TXPANIC_LDO_REG                                    0x00000020
#ifndef ASM_INC
typedef union {
  struct {
    //
    // PanicHi voltage level is -3*PanicHi*Vccdq/384. Value of 0 disables PanicLo sensor
    // and PanicLo driver
    //
    // Bits[2:0], RW, default = 0x0
    //
    UINTX panicvlo : 3;
    //
    // PanicHi voltage level is 3*PanicHi*Vccdq/384. Value of 0 disables PanicHi sensor
    // and PanicLo driver
    //
    // Bits[5:3], RW, default = 0x0
    //
    UINTX panicvhi : 3;
    //
    // Enable Panic during DQ traffic, extending for 2 dclks after DQ turns off. MSB/LSB
    // bit used for High/Low DVFS point
    //
    // Bits[7:6], RW, default = 0x1
    //
    UINTX enpanicdq : 2;
    //
    // Enable Panic during CA traffic, extending for 2 dclks after CA turns off. MSB/LSB
    // bit used for High/Low DVFS point
    //
    // Bits[9:8], RW, default = 0x1
    //
    UINTX enpanicca : 2;
    //
    // Enable Panic for 8 dclk after ClkStop exit. MSB/LSB bit used for High/Low DVFS
    // point
    //
    // Bits[11:10], RW, default = 0x1
    //
    UINTX enpanicclk : 2;
    //
    // Enable Panic on SR exit until the regulator has been in each mode for a certain
    // period of time
    //
    // Bits[12:12], RW, default = 0x1
    //
    UINTX enpanicsr : 1;
    //
    // During CKE power down, Target amp wakes for 4 dclks, active for 8 dclks (i.e.
    // allow Code/Tcode updates) and sleeps for 63 dclks.[br]During sleep/wake, set Code
    // equal to drive TcodeClkStop.
    //
    // Bits[13:13], RW, default = 0x0
    //
    UINTX enpdclkstop : 1;
    //
    // During CKE power down, Target amp wakes for 4 dclks, active for 8 dclks (i.e.
    // allow Code/Tcode updates) and sleeps for 24 dclks.[br]During sleep/wake, set Code
    // equal to drive TcodeIdle.
    //
    // Bits[14:14], RW, default = 0x0
    //
    UINTX enpdidle : 1;
    //
    // Chicken bit that disables all clock & power gating in LDO
    //
    // Bits[15:15], RW, default = 0x1
    //
    UINTX pdcbit : 1;
    //
    // Select digital signal to observe on Ch0 view pins
    //
    // Bits[19:16], RW, default = 0x0
    //
    UINTX viewdigch0 : 4;
    //
    // Select digital signal to observe on Ch1 view pins
    //
    // Bits[23:20], RW, default = 0x0
    //
    UINTX viewdigch1 : 4;
    //
    // Select analog signal to observe
    //
    // Bits[25:24], RW, default = 0x0
    //
    UINTX viewana : 2;
    //
    // Enable panic down compensation
    //
    // Bits[26:26], RW, default = 0x0
    //
    UINTX enpanicdowncomp : 1;
    //
    // Force panic to be on
    //
    // Bits[27:27], RW, default = 0x0
    //
    UINTX forcepanicon : 1;
    //
    // Reserved bits
    //
    // Bits[31:28], RO, default = 0x0
    //
#ifdef MEM_SV_HOOKS
    UINTX reserved : 4;
#else
    UINTX reserved : 4;
#endif // MEM_SV_HOOKS
  } Bits;
  UINTX Data;
} TXPANIC_LDO_STRUCT;
#endif // ASM_INC

#define TXPANICCOMP_LDO_REG                                0x00000024
#ifndef ASM_INC
typedef union {
  struct {
    //
    // Multiplier of NMOS compensation code for DVFS Frequency A
    //
    // Bits[3:0], RW, default = 0x0
    //
    UINTX paniccompnmult_a : 4;
    //
    // Multiplier of NMOS compensation code for DVFS Frequency B
    //
    // Bits[7:4], RW, default = 0x0
    //
    UINTX paniccompnmult_b : 4;
    //
    // PMOS Panic Driver Compensation code for DVFS Frequency A.[br]Overloaded such that
    // this stores PanicDnCount during IOLB on the DVFS value that is NOT being used.
    //
    // Bits[13:8], RW/V, default = 0x0
    //
    UINTX paniccompp_a : 6;
    //
    // NMOS Panic Driver Compensation code for DVFS Frequency A.[br]Overloaded such that
    // this stores PanicDnCount during IOLB on the DVFS value that is NOT being used.
    //
    // Bits[19:14], RW/V, default = 0x0
    //
    UINTX paniccompn_a : 6;
    //
    // PMOS Panic Driver Compensation code for DVFS Frequency B.[br]Overloaded such that
    // this stores PanicDnCount during IOLB on the DVFS value that is NOT being used.
    //
    // Bits[25:20], RW/V, default = 0x0
    //
    UINTX paniccompp_b : 6;
    //
    // NMOS Panic Driver Compensation code for DVFS Frequency B.[br]Overloaded such that
    // this stores PanicDnCount during IOLB on the DVFS value that is NOT being used.
    //
    // Bits[31:26], RW/V, default = 0x0
    //
    UINTX paniccompn_b : 6;
  } Bits;
  UINTX Data;
} TXPANICCOMP_LDO_STRUCT;
#endif // ASM_INC


#endif // _LDO_H_
