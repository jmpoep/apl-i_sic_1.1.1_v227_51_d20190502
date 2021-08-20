/** @file
  Header file for all external hooks used by modular MRC.

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

#ifndef _MMRC_HOOKS_H_
#define _MMRC_HOOKS_H_

#include "DataTypes.h"
#include "Printf.h"
#include "OemHooks.h"
#include "MmrcData.h"
#include "RegAccess.h"
#if SIM
#include "vpi_user.h"
#include "mrc_wrapper.h"
#endif


#define CEILING(x,y)  ((x%y)?(x/y+1):(x/y))

#if !SIM && !JTAG
  #define MySimStall(n)
#endif

#if SIM || JTAG || MINIBIOS
  #define CpuDeadLoop()
#endif

#define CHECKPOINT(CP)        (CheckpointSet(CP))

typedef enum {
  MrcMsgDimm0NotPopulated = 0,
  MrcMsgOneChannelModeOnly,
  MrcMsgFreqNotSupported,
  MrcMsgFreqConfigMaxError
} CHECK_FREQ_CONFIG_ERROR;

MMRC_STATUS
InitializeJedec_DDR3_Nom60_Wr0(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Cpgc VA test

  @param[in]  ModMrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  Success
  @retval  Failure
**/
extern
MMRC_STATUS
CpgcVATest (
  MMRC_DATA         *ModMrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Mc stall nano second

  @param[in]  DelayHPET

  @retval  None
**/
extern
VOID
McStallNanoSecond (
  IN      UINT32    DelayHPET
);

/**
  Mc stall

  @param[in]  DelayHPET

  @retval  None
**/
extern
VOID
McStall (
  IN      UINT32    DelayHPET
);

/**
  Set slice channel hash

  @param[in,out]  MrcData

  @retval  None
**/
extern
VOID
SetSliceChannelHash (
  IN OUT MMRC_DATA *MrcData
);

extern UINT32 RestoreDataDescriptorTableCount;
extern RESTORE_DATA_DESCRIPTOR RestoreDataDescriptorTable[];

/**
  Fill channel bit mapping

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
FillChannelBitMapping(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Receive enable exit

  @param[in,out]  MrcData
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
ReceiveEnableExit(
  MMRC_DATA         *MrcData,
  UINT8             Channel
);

/**
  Program CKE2x control

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
ProgramCKE2xControl (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  LP4 delay test

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
  LP4DelayTest(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
  );

/**
  Get and set Dq delay

  @param[in,out]  MrcData
  @param[in]  Socket
  @param[in]  Channel
  @param[in]  Dimm
  @param[in]  Rank
  @param[in]  Strobe
  @param[in]  Bit
  @param[in]  FrequencyIndex
  @param[in]  IoLevel
  @param[in]  Cmd
  @param[in,out]  Value

  @retval  MMRC_STATUS
**/
MMRC_STATUS
GetSetDqDelay (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Socket,
  IN        UINT8         Channel,
  IN        UINT8         Dimm,
  IN        UINT8         Rank,
  IN        UINT8         Strobe,
  IN        UINT8         Bit,
  IN        UINT8         FrequencyIndex,
  IN        UINT8         IoLevel,
  IN        UINT8         Cmd,
  IN  OUT   UINT32        *Value
);

/**
  Get and set Tx Dq bit delay

  @param[in,out]  MrcData
  @param[in]  Socket
  @param[in]  Channel
  @param[in]  Dimm
  @param[in]  Rank
  @param[in]  Strobe
  @param[in]  Bit
  @param[in]  FrequencyIndex
  @param[in]  IoLevel
  @param[in]  Cmd
  @param[in,out]  Value

  @retval  MMRC_STATUS
**/
MMRC_STATUS
GetSetTxDqBitDelay (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Socket,
  IN        UINT8         Channel,
  IN        UINT8         Dimm,
  IN        UINT8         Rank,
  IN        UINT8         Strobe,
  IN        UINT8         Bit,
  IN        UINT8         FrequencyIndex,
  IN        UINT8         IoLevel,
  IN        UINT8         Cmd,
  IN  OUT   UINT32        *Value
);

/**
  Get and set Tx Vref

  @param[in,out]  MrcData
  @param[in]  Socket
  @param[in]  Channel
  @param[in]  Dimm
  @param[in]  Rank
  @param[in]  Strobe
  @param[in]  Bit
  @param[in]  FrequencyIndex
  @param[in]  IoLevel
  @param[in]  Cmd
  @param[in,out]  Value

  @retval  MMRC_STATUS
**/
MMRC_STATUS
GetSetTxVref (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Socket,
  IN        UINT8         Channel,
  IN        UINT8         Dimm,
  IN        UINT8         Rank,
  IN        UINT8         Strobe,
  IN        UINT8         Bit,
  IN        UINT8         FrequencyIndex,
  IN        UINT8         IoLevel,
  IN        UINT8         Cmd,
  IN  OUT   UINT32        *Value
);

/**
  Get and set Tx Ca Vref

  @param[in,out]  MrcData
  @param[in]  Socket
  @param[in]  Channel
  @param[in]  Dimm
  @param[in]  Rank
  @param[in]  Strobe
  @param[in]  Bit
  @param[in]  FrequencyIndex
  @param[in]  IoLevel
  @param[in]  Cmd
  @param[in,out]  Value

  @retval  MMRC_STATUS
**/
MMRC_STATUS
GetSetTxCaVref (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Socket,
  IN        UINT8         Channel,
  IN        UINT8         Dimm,
  IN        UINT8         Rank,
  IN        UINT8         Strobe,
  IN        UINT8         Bit,
  IN        UINT8         FrequencyIndex,
  IN        UINT8         IoLevel,
  IN        UINT8         Cmd,
  IN  OUT   UINT32        *Value
);

/**
  Perform DQ reset

  @param[in,out]  MrcData
  @param[in]  Channel
  @param[in]  Rank


  @retval  MMRC_STATUS
**/
MMRC_STATUS
PerformDQReset (
  IN OUT   MMRC_DATA      *MrcData,
  IN       UINT8          Channel,
  IN       UINT8          Rank
);

/**
  Only keep 10

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
OnlyKeep10 (
  IN  OUT MMRC_DATA         *MrcData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
);

/**
  Display input parameters

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DisplayInputParameters(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Display MMRC version

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
extern
MMRC_STATUS
DISPLAYMMRCVERSION (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Display Lpddr information

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DisplayLpddrInfo (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Executes and prints (if enabled) the MR values

  @param[in,out]  MrcData
  @param[in]  Channel

  @retval  Data
**/
UINT32
SendDramCmd (
  MMRC_DATA         *MrcData,
  UINT8             Channel
);

/**
  Setup MRC freq A

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
SetupMRCFreqA (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Setup MRC freq B

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
SetupMRCFreqB (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

//Bank 0
//#define ACTIVATE_LP4    0x3001
//Bank 1
#define ACTIVATE_LP4    0x3041
#define ACTIVATE_LP3    2
#define PREA_LP3    0x1b
#define PREA_LP4    0x30
typedef struct {
  UINT32  txdll_stat_fbmuxsel_max;
  UINT32  txdll_stat_fbmuxsel_min;
  UINT32  txdll_error;
  UINT8   muxcode_min;
  UINT8   muxcode_max;
  UINT16  code_count;
  UINT16  code_same_count;
  UINT32  last_max;
} MAXPI_INFO;
typedef enum {
  MAXPI_INIT,
  MAXPI_DLLINIT,
  MAXPI_TXDLL_ERROR,
  MAXPI_CHECK_ACROSS_FAMILY,
  MAXPI_SETUP_WIDER_RANGE,
  MAXPI_FINAL_VALUES,
  MAXPI_ERROR
}
MAXPI_STATES;

/**
  Setup max PI

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS SetupMaxPI(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
);

/**
  wiogrp6

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS wiogrp6(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
);

/**
  Convert linear to physical value

  @param[in,out]  MrcData
  @param[in]  DimIndex
  @param[in]  LinearValue

  @retval  Data
**/
UINT32
ConvertLinearToPhysicalValue (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT16        DimIndex,
  IN        UINT32        LinearValue
);

/**
  Convert physical to linear value

  @param[in,out]  MrcData
  @param[in]  DimIndex
  @param[in]  PhysicalValue

  @retval  Data
**/
UINT32
ConvertPhysicalToLinearValue (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT16         DimIndex,
  IN        UINT32        PhysicalValue
);

/**
  Mmrc memcpy

  @param[in]  dst
  @param[in]  src
  @param[in]  cnt

  @retval  Data
**/
extern
void *
Mmrcmemcpy(
  IN      void *dst,
  IN      void *src,
  IN      UINT32 cnt
);
//
// DRAM Initialization Structures used in JEDEC Message Bus Commands
//

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    BL              : 2;            // Burst Length, CDV:1
    UINT32    CL0             : 1;            // CL Reserved
    UINT32    RBT             : 1;            // Read Burst Type
    UINT32    CL1             : 3;            // cas Latency
    UINT32    TM              : 1;            // Test mode
    UINT32    dllReset        : 1;            // DLL Reset
    UINT32    writeRecovery   : 3;            // Write Recovery for Auto Pre-Charge: 001=2,010=3,011=4,100=5,101=6
    UINT32    PPD             : 1;            // DLL Control for Precharge Power-Down CDV:1
    UINT32    reserved1       : 3;
    UINT32    bankAddress     : 3;            // Bank Address (BA[2:0])
    UINT32    WE              : 1;            // Rank Select
    UINT32    CAS             : 1;
    UINT32    RAS             : 1;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    reserved        : 10;
  } Bits;
#endif
} DramInitDDR3MRS0;                           // DDR3 Mode Register Set (MRS) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    dllEnabled      : 1;            // CDV=0
    UINT32    DIC0            : 1;            // Output Driver Impedance Control
    UINT32    rttNom0         : 1;            // RTT_nom[0]
    UINT32    MRC_AL          : 2;            // Additive Latency = 0
    UINT32    DIC1            : 1;            // Reserved
    UINT32    rttNom1         : 1;            // RTT_nom[1]
    UINT32    wlEnabled       : 1;            // Write Leveling Enable
    UINT32    reserved1       : 1;
    UINT32    rttNom2         : 1;            // RTT_nom[2]
    UINT32    reserved2       : 1;
    UINT32    TDQS            : 1;            // TDQS Enable
    UINT32    Qoff            : 1;            // Output Buffers Disabled
    UINT32    reserved3       : 3;
    UINT32    bankAddress     : 3;            // Bank Address (BA[2:0])
    UINT32    WE              : 1;            // Rank Select
    UINT32    CAS             : 1;
    UINT32    RAS             : 1;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    reserved        : 10;
  } Bits;
#endif
} DramInitDDR3EMR1;                           // DDR3 Extended Mode Register 1 Set (EMRS1) Command

typedef union {
  UINT32      Data;
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
#ifndef ASM_INC
  struct {
    UINT32    PASR            : 3;            // Partial Array Self-Refresh
    UINT32    CWL             : 3;            // CAS Write Latency
    UINT32    ASR             : 1;            // Auto Self-Refresh
    UINT32    SRT             : 1;            // SR Temperature Range = 0
    UINT32    reserved1       : 1;
    UINT32    rtt_WR          : 2;            // Rtt_WR
    UINT32    reserved2       : 5;
    UINT32    bankAddress     : 3;            // Bank Address (BA[2:0])
    UINT32    WE              : 1;            // Rank Select
    UINT32    CAS             : 1;
    UINT32    RAS             : 1;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    reserved        : 10;
  } Bits;
#endif
} DramInitDDR3EMR2;                           // DDR3 Extended Mode Register 2 Set (EMRS2) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {

    UINT32    MPR_Location    : 2;            // MPR Location
    UINT32    MPR             : 1;            // MPR: Multi Purpose Register
    UINT32    reserved1       : 13;
    UINT32    bankAddress     : 3;            // Bank Address (BA[2:0])
    UINT32    WE              : 1;            // Rank Select
    UINT32    CAS             : 1;
    UINT32    RAS             : 1;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    reserved        : 10;
  } Bits;
#endif
} DramInitDDR3EMR3;                           // DDR3 Extended Mode Register 2 Set (EMRS2) Command

typedef union {
  UINT32        Data;
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110  ZQ Calibration,111-NOP
    UINT32    bankAddress     : 3;            // Bank Address (BA[2:0])
    UINT32    multAddress     : 16;           // Multiplexed Address (MA[15:0])
    UINT32    rankSelect      : 2;            // Rank Select
    UINT32    reserved3       : 8;
  } Bits;
#endif
} DramInitMisc;                               // Miscellaneous DDRx Initialization Command

#if DDR4_SUPPORT == 1
typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    BurstLength     : 2;            // Burst Length [A1:A0] 00 = 8 (Fixed) 01 = BC4 or 8 (on the fly)10 = BC4 (Fixed)11 = Reserved
    UINT32    CL              : 1;            // cas Latency A2 lsb
    UINT32    readBurstType   : 1;            // Read Burst Type [A3]  0 = Sequential 1 = Interleave
    UINT32    casLatency      : 3;            // cas Latency [A6:A4] msb
    UINT32    TM              : 1;            // Test mode [A7]
    UINT32    dllReset        : 1;            // DLL Reset [A8]
    UINT32    writeRecovery   : 3;            // Write Recovery [A11:A9] for Auto Pre-Charge: Jedec DDR4 MR0 table 2 page 21
    UINT32    reserved        : 6;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4MRS0;                           // DDR4 Extended Mode Register 0 Set (EMRS0) Command


typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    dllEnabled      : 1;            // DLL Enable [A0] 0 = Disable2 1 = Enable
    UINT32    DIC0            : 2;            // Output Driver Impedance Control [A2:A1]: 00 RZQ/7 01 RZQ/5
    UINT32    MRC_AL          : 2;            // Additive Latency [A4:A3]: 00 = 0(AL disabled) 01 = CL-1 10 = CL-2
    UINT32    reserved0       : 2;            // [A6:A5]
    UINT32    wlEnabled       : 1;            // Write Leveling Enable [A7]
    UINT32    rttNom          : 3;            // RTT_nom [A10:A9:A8] Jedec page 22  RZQ/#
    UINT32    TDQS            : 1;            // TDQS Enable  [A11]
    UINT32    Qoff            : 1;            // Output Buffers Disabled [A12]
    UINT32    reserved1       : 5;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR1;                           // DDR4 Extended Mode Register 1 Set (EMRS1) Command


typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    TRRAddress      : 2;            // TRR Mode [A1:A0]
    UINT32    TRRBankG0       : 1;            // TRR Mode [A2]
    UINT32    CWL             : 3;            // CAS Write Latency [A5:A3]
    UINT32    LPASR           : 2;            // Low Power Array Self-Refresh [A7:A6]
    UINT32    TRRBankG1       : 1;            // TRR Mode [A8]
    UINT32    RTT_WR          : 3;            // RTT_WR[A11:A10:A9]
    UINT32    writeCRC        : 1;            // Write CRC [A12] 0 = Disable 1 = Enable
    UINT32    TRR             : 1;            // TRR [A13]
    UINT32    reserved        : 4;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR2;                           // DDR4 Extended Mode Register 2 Set (EMRS2) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    MPR_page        : 2;            // MPR page [A1:A0]
    UINT32    MPR_Operation   : 1;            // MPR Operation [A2]
    UINT32    Geardown        : 1;            // Geardown [A3]
    UINT32    PDA             : 1;            // Per DRAM Addressability [A3] 0 = Disable 1 = Enable
    UINT32    Temp_sensor     : 1;            // Temperature sensor readout [A5] 0 : disabled 1: enabled
    UINT32    FineGranRefresh : 3;            // Fine Granularity Refresh Mode [A8:A6]
    UINT32    WL_crcdm        : 2;            // Write CMD Latency when CRC and DM are enabled [A10:A9]
    UINT32    MPR_readformat  : 2;            // MPR Read Format [A12:A11] 00 = Serial 10 = Staggered 01 = Parallel 11 = Reserved
    UINT32    reserved        : 5;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR3;                           // DDR4 Extended Mode Register 3 Set (EMRS3) Command


typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    reserved0       : 1;            // 0 = must be programmed to 0 during MRS
    UINT32    Max_PD          : 1;            // Maximum Power Down [A1]
    UINT32    Temp_ctledrr    : 1;            // Temperature ControlledRefresh Range [A2] 0 = Normal 1 = Extended
    UINT32    Temp_ctledrm    : 1;            // Temperature Controlled Refresh Mode [A3] 0 = Disable 1 = Enable
    UINT32    Int_VrefMon     : 1;            // Internal Vref Monitor [A4] 0 : disabled 1: enabled
    UINT32    reserved1       : 1;            // 0 = must be programmed to 0 during MRS
    UINT32    CS2cmdaddrlat   : 3;            // CS to CMD/ADDR Latency Mode (cycles) [A8:A6]
    UINT32    SRabort         : 1;            // Self Refresh Abort[A9]
    UINT32    ReadPreamMode   : 1;            // Read Preamble Training Mode [A10] 0 = Disable 1 = Enable
    UINT32    ReadPream       : 1;            // Read Preamble Training Mode [A11] 0 = 1 nCK 1 = 2 nCK
    UINT32    WritePream      : 1;            // Write Preamble Training Mode [A12] 0 = 1 nCK 1 = 2 nCK
    UINT32    PPR             : 1;            // PPR [A13] 0 = Disable 1 = Enable
    UINT32    reserved2       : 4;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR4;                           // DDR4 Extended Mode Register 4 Set (EMRS4) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    CA_paritylat    : 3;            // C/A Parity Latency Mode [A2:A0]
    UINT32    Crc_errclear    : 1;            // CRC Error Clear [A3] 0 = Clear 1 = Error
    UINT32    CA_parerrsts    : 1;            // C/A Parity Error Status [A4] 0 = Clear 1 = Error
    UINT32    Odt_inbuff      : 1;            // ODT Input Buffer during Power Down mode [A5]
    UINT32    RTT_PARK        : 3;            // Internal Vref Monitor [A8:A6]
    UINT32    CA_parperserr   : 1;            // CA parity Persistent Error [A9] 0 = Disable1 = Enable
    UINT32    DM              : 1;            // Data Mask [A10] 0 = Disable 1 = Enable
    UINT32    WriteDBI        : 1;            // Write DBI [A11]0 = Disable 1 = Enable
    UINT32    ReadDBI         : 1;            // Read DBI [A12] 0 = Disable 1 = Enable
    UINT32    reserved        : 5;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR5;                           // DDR4 Extended Mode Register 5 Set (EMRS5) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    Vrefdq_Value    : 6;            // VrefDQ Training Value [A5:A0]
    UINT32    Vrefdq_Range    : 1;            // VrefDQ Training Range [A6]
    UINT32    Vrefdq_Enable   : 1;            // VrefDQ Training Enable [A7] 0 = Disable(Normal operation Mode) 1 = Enable(Training Mode)
    UINT32    reserved0       : 2;            // Must be 0 during MRS [A9:A8]
    UINT32    tCCD_L          : 3;            // tCCD_L [A12:A10]
    UINT32    reserved1       : 5;            // reserved
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4EMR6;                           // DDR4 Extended Mode Register 6 Set (EMRS6) Command

typedef union {
  UINT32      Data;
  //
  // Remove bit field definitions from ASM include file due to MASM/H2INC limitations
  //
#ifndef ASM_INC
  struct {
    UINT32    command         : 3;            // Command: 000-MRS,001-Refresh,010-Pre-charge,011-Activate,110-ZQ,111-NOP
    UINT32    ACT_N           : 1;            // ACT_N
    UINT32    bankAddress     : 2;            // Bank Address (BA[1:0])
    UINT32    bankGroup       : 2;            // Bank Address (BG[1:0])
    UINT32    MemoryAddress   : 18;           // Memory Address: Sets the value of MA[17:0].
    UINT32    rankSelect      : 2;            // Rank Select [27:26]
    UINT32    BA              : 2;            // BA[1:0] to be used by read command when the MPR mode sequence is selected (bit 30 = 1)
    UINT32    MPR             : 1;            // 30:30 MPR Mode Sequence;  Setting this bit will issue the following sequence after the selected command: Read to same channel/rank with address=0, bank group=0 and bank address selected by bits 29:28 (definition below), followed by the same MRS command with A2=0 issued. This is intended to be used with an MRS command to MR3 with A2=1 and with the PREA command enabled (bit 31 = 1).  The read data is captured in MPRDATA registers.
    UINT32    PREA            : 1;            // 31:31 PREA issued prior to the command; intended to be used with an MRS command.
  } Bits;
#endif
} DramInitDDR4Misc;                           // DDR4 Extended Mode Misc Register Command

#endif // DDR4_SUPPORT


/*
MMRC_STATUS
GetSetDimmVref (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Socket,
  IN        UINT8         Channel,
  IN        UINT8         Dimm,
  IN        UINT8         Rank,
  IN        UINT8         Strobe,
  IN        UINT8         Bit,
  IN        UINT8         IoLevel,
  IN        UINT8         Cmd,
  IN  OUT   UINT32        *Value
);

VOID
WriteDimmVref (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT32        vref,
  IN        UINT8         Override
);
*/
#if !CODE_SIZE_REDUCE_EN
/**
  Vref override

  @param[in,out]  MrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
VrefOverride (
  IN  OUT MMRC_DATA         *MrcData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
);
#endif

/**
  Sets write VREF prior to start all MRC training to a known value in order to
  avoid failures during early trainings

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
EarlySetWriteVref (
  IN  OUT MMRC_DATA         *MrcData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
)
;

/**
  Configures a checkpoint to halt the CPU at any point in the MRC code, this
  function is used for debug, and never should be left in the code after debug
  is completed
  This always halts where the function is placed, in order to continue developer
  must write a '0' into the SSKPD4_DUNIT_REG register

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]   Name          Pointer with the Name of the Breakpoint

  @retval  None
**/
VOID
BreakpointMrc (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         *Name
)
;

/**
  This function copies the MR commands into the CPGC errors

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]   Channel          Current Channel being examined.

  @retval  None
**/
VOID
ShadowMrsToXbuf (
  IN  OUT   MMRC_DATA   *MrcData,
  IN        UINT8        Channel
)
;

/**
  Configures a checkpoint to halt the CPU in a specific memory training step

  @param[in]   MrcData        Host structure for all MRC global data.
  @param[in]   CheckPoint       Specifies the code related to an MRC step

  @retval  None
**/
VOID
HandleCheckpoint (
  IN        MMRC_DATA *MrcData,
  IN        UINT8      CheckPoint
)
;

/**
  Fills in the complete parameter list for the Modular MRC.  This includes all the Dynamic entries that are listed in the

  Input Spreadsheet, but also
  the PFCT values.

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
MmrcConvertParams (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Mmrc change Hi frequency

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
MmrcChangeHiFreq (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Mmrc change Lo frequency

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
MmrcChangeLoFreq (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Dump all DDR IO registers

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpAllDDRIORegisters(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Override Timing parameter based on user input/Setup option

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
OverrideTimingParams (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Simulate memory test

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
SimMemTest (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Complete Jedec initialize

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
CompleteJedecInit (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  );

/**
  Change Tx to fail simulation test

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
ChangeTxToFailSimTest (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Channel 10 only

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
Channel10Only (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Initializes all dynamic variables used in JEDEC init.

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
InitializeJedec (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  LP4MR programming_MPCZQ

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
  LP4MRProgramming_MPCZQ (
  IN  OUT MMRC_DATA   *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel);

/**
  LP4 all MR programming

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
LP4AllMRProgramming (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  LP4MR programming_ODTVREF

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
LP4MRProgramming_ODTVREF (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Handle address mirroring

  @param[in,out]  Data
  @param[in]      AddressMirroringSpdByte

  @retval  None
**/
VOID
HandleAddressMirroring (
  IN  OUT   UINT32 *Data,
  IN        UINT8   AddressMirroringSpdByte
);

/**
  Write mrs command

  @param[in,out]  MrcData       Host structure for all data related to MMRC
  @param[in]      Channel
  @param[in]      Rank
  @param[in]      MiscCommand

  @retval  None
**/
VOID
WriteMrsCommand (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT8         Rank,
  IN        UINT32        MiscCommand
)
;

/**
  Read Mrs command

  @param[in,out]  MrcData       Host structure for all data related to MMRC
  @param[in]      Channel
  @param[in]      Rank
  @param[in]      Index

  @retval  Data
**/
UINT32
ReadMrsCommand (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT8         Rank,
  IN        UINT8         Index
)
;

/**
  Mirrors address bits

  @param[in]  data   Data for MRS write

  @retval  dataNew   Mirrored data
**/
UINT16
MirrorAddrBits (
  IN    UINT16 data
)
;

/**
  Mirrors bank address bits

  @param[in]  bank   Bank address to be mirrored

  @retval  bankNew   Mirrored bank address
**/
UINT8
MirrorBankBits (
  UINT8 bank
)
;

/**
  Output a progress data to port 80 for debug purpose
  Could be safely overriden to
  send checkpoints elsewhere, such as port 0x84 or a serial port

  @param[in]  Content   Hexadecimal value to be sent thru debug port

  @retval  None
**/
VOID
CheckpointSet (
  IN        UINT16    Content
)
;

/**
  Mrc delay

  @param[in]  MrcData
  @param[in]  Type
  @param[in]  Delay

  @retval  MMRC_STATUS
**/
MMRC_STATUS
MrcDelay (
  IN        MMRC_DATA *MrcData,
  IN        UINT8   Type,
  IN        UINT32  Delay
)
;

/**
  Dump Cpgc registers

  @param[in,out]  MrcData
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpCpgcRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
);

/**
  Dump registers

  @param[in,out]  MrcData

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpRegs (
  IN  OUT   MMRC_DATA   *MrcData
);

/**
  Dump DDR IO registers

  @param[in,out]  MrcData
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpDdrioRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
)
;

/**
  Dump D unit registers

  @param[in,out]  MrcData
  @param[in]  Channel

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpDunitRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
);

/**
  Dump B unit registers

  @param[in,out]  MrcData

  @retval  MMRC_STATUS
**/
MMRC_STATUS
DumpBunitRegisters (
  IN  OUT   MMRC_DATA *MrcData
);

/**
  Read MRTQ REF

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
ReadMRTQREF (
  MMRC_DATA         *MrcData,
    UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Post Mrc done settings

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  MMRC_STATUS
**/
MMRC_STATUS
PostMrcDoneSettings (
  IN  OUT   MMRC_DATA   *MrcData,
  UINT16                 CapsuleStartIndex,
  UINT16                 StringIndex,
  UINT8                  Channel
);

/**
  This routine attempts to acquire the SMBus

  @param[in]  SmbusBase

  @retval    MMRC_FAILURE as failed
  @retval    MMRC_SUCCESS as passed
**/
MMRC_STATUS
MrcAcquireBus (
  IN      UINT16    SmbusBase
)
;

/**
  This routine reads SysCtl registers

  @param[in]  SmbusBase   SMBUS Base Address
  @param[in]  SlvAddr     Targeted Smbus Slave device address
  @param[in]  Operation   Which SMBus protocol will be used
  @param[in]  Offset      Offset of the register
  @param[in]  Length      Number of bytes
  @param[in]  Buffer      Buffer contains values read from registers

  @retval    Others as failed
  @retval    MMRC_SUCCESS as passed
**/
MMRC_STATUS
MrcSmbusExec (
  IN        UINT16    SmbusBase,
  IN        UINT8     SlvAddr,
  IN        UINT8     Operation,
  IN        UINT8     Offset,
  IN        UINT8     *Length,
  IN        UINT8     *Buffer
)
;

/**
  Use BreakStrobeLoop to break the cycles for Delays that don't need to complete the strobe cycle

  @param[in]  MrcData      Host structure for all data related to MMRC
  @param[in]  Channel      Current Channel being examined.
  @param[in]  Rank         Current Rank being examined.
  @param[in]  Dim1Index    Delay Signal
  @param[in]  Strobe       Strobe

  @retval    Others as failed
  @retval    MMRC_SUCCESS as passed
**/
MMRC_STATUS
BreakStrobeLoop (
  IN   MMRC_DATA    *MrcData,
  IN   UINT8        Channel,
  IN   UINT8        Rank,
  IN   UINT16       Dim1Index,
  IN   UINT8        *Strobe
)
;

/**
  Signal upper bound

  @param[in]  MrcData      Host structure for all data related to MMRC
  @param[in]  Channel      Current Channel being examined.
  @param[in]  Rank         Current Rank being examined.
  @param[in]  Dim1Index    Delay Signal

  @retval    Data
**/
UINT32
SignalUpperBound (
  IN         MMRC_DATA    *MrcData,
  IN         UINT8        Channel,
  IN         UINT8        Rank,
  IN         UINT8        Dim1Index
);

/**
  Configures steps prior to execute Pass Gate Test

  @param[in]  MrcData      Host structure for all data related to MMRC
  @param[in]  Channel      Current Channel being examined.
  @param[in]  Swizzle      Pointer to define if Swizzle or Sequencial

  @retval    MMRC_STATUS
**/
MMRC_STATUS
PassGateTestEntryHooks (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
      OUT   UINT8         *Swizzle
)
;

/**
  Configures steps after execute Pass Gate Test

  @param[in]  MrcData      Host structure for all data related to MMRC
  @param[in]  Channel      Current Channel being examined.

  @retval    MMRC_STATUS
**/
MMRC_STATUS
PassGateTestExitHooks (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel
)
;

/**
  Restore algos

  @param[in]  ModMrcData
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
RestoreAlgos (
  MMRC_DATA         *ModMrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  Save algos

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
SaveAlgos(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
);

/**
  S0_DunitWake

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel            Current Channel being examined.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
S0_DunitWake(
  IN  OUT   MMRC_DATA   *MrcData,
  UINT16                CapsuleStartIndex,
  UINT16                StringIndex,
  UINT8                 Channel
);

/**
  Setup periodic DDRIO swizzle

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  Channel            Current Channel being examined.

  @retval  None
**/
void SetupPeriodicDDRIOSwizzle(
  MMRC_DATA         *MrcData,
  UINT8             Channel
  );

/**
  Get manual initial index

  @param[in,out]  MrcData        Host structure for all MRC global data.
  @param[in]  Rank
  @param[in]  Channel
  @param[in]  ro_table
  @param[in]  RestorePath

  @retval  Data
**/
UINT32 GetManualInitialIndex(
  MMRC_DATA         *MrcData,
  UINT8             Rank,
  UINT8             Channel,
  UINT16            *ro_table,
  BOOLEAN           RestorePath
  );

/**
  Find the common timing for MD+SODIMM configuration

  @param[in,out]  MrcData       Host structure for all data related to MMRC

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
  DetermineCommonTiming (
  IN  OUT   MMRC_DATA            *MrcData
  );

/**
  Find the common freq

  @param[in,out]  MrcData       Host structure for all data related to MMRC

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
  DetermineCommonFreq (
  IN  OUT   MMRC_DATA            *MrcData
  );
#endif
