/**@file

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
/*++

Module Name:

  MmrcHooks.h

Abstract:

  Modular MRC calls functions in this file which perform tasks
  which are outside the realm of DDRIO initialization. Things
  like print routines, DUNIT routines which the MMRC needs,
  JEDEC init, etc, will be placed here. The routines in this
  file should NOT reference any host structure besides MMRC_DATA
  since this file is build in the MMRC standalone mode which
  does not include any MRC dependencies outside the MMRC.

--*/

#if defined(CAR)
#include "Delay.h"
#endif
#if (!defined (SIM) || (SIM == 0)) && (!defined (MINIBIOS) || (MINIBIOS == 0)) && (!defined (JTAG) || (JTAG == 0))
#include <Library/TimerLib.h>
#endif

#include "McFunc.h"
#include "MmrcHooks.h"
#include "MmrcProjectData.h"
#include "MmrcLibraries.h"
#include "cpgcapi.h"
#include "MmrcData.h"
#include "MrcFunc.h"

#if SIM || JTAG
#include <conio.h>
#endif

// Signals to be restored in RestoreAlgos
RESTORE_DATA_DESCRIPTOR RestoreDataDescriptorTable[] = {
  { CmdGrp0,          UNIQUE,                    TRUE },
  { CmdGrp1,          UNIQUE,                    TRUE },
  { CtlGrp0,          UNIQUE,                    TRUE },
  { CtlGrp1,          UNIQUE,                    TRUE },
  { CkGrp0,           UNIQUE,                    TRUE },
  { CkGrp1,           UNIQUE,                    TRUE },
  { RxDqsNDelay,      STROBE_BASED | RANK_BASED, FALSE },
  { RxDqsPDelay,      STROBE_BASED | RANK_BASED, FALSE },
  { RxVref,           STROBE_BASED | RANK_BASED, TRUE  },
  { TxDqsDelay,       STROBE_BASED | RANK_BASED, FALSE },
  { TxDqDelay,        STROBE_BASED | RANK_BASED, FALSE },
  { TxDqDelay_Actual, STROBE_BASED | RANK_BASED, FALSE },
  { TxVref,           STROBE_BASED | RANK_BASED, FALSE },
  { TxCaVref,         STROBE_BASED | RANK_BASED, FALSE },
  { RecEnDelay,       STROBE_BASED | RANK_BASED, FALSE },
  { Grp6_2x,          STROBE_BASED,              FALSE },
#if RxVocVal0
  { RxDiffampOvrDq,   STROBE_BASED,              FALSE },
  { RxDiffampOvrDqs,  STROBE_BASED,              FALSE },
  { RxDiffampValDq,   STROBE_BASED,              FALSE },
  { RxDiffampValDqs,  STROBE_BASED,              FALSE },
  { RxOdtOvr,         STROBE_BASED,              FALSE },
  { RxOdtVal,         STROBE_BASED,              FALSE },
  { RxVocVal0,        STROBE_BASED,              FALSE },
  { RxVocVal1,        STROBE_BASED,              FALSE },
  { RxVocVal2,        STROBE_BASED,              FALSE },
  { RxVocVal3,        STROBE_BASED,              FALSE },
  { RxVocVal4,        STROBE_BASED,              FALSE },
  { RxVocVal5,        STROBE_BASED,              FALSE },
  { RxVocVal6,        STROBE_BASED,              FALSE },
  { RxVocVal7,        STROBE_BASED,              FALSE },
  { RxVocEnDq,        STROBE_BASED,              FALSE },
  { RxVocEnDqs,       STROBE_BASED,              FALSE },
#endif
  //  These GetSets save/restore are handled by "SetupMaxPi"
  //  - INITIAL_INDEX; MuxcodeMin; MuxcodeMax
  //  { INITIAL_INDEX     , STROBE_BASED , FALSE },
  //  { MuxcodeMin,       STROBE_BASED | RANK_BASED, FALSE }, //RDDTTI:5024
  //  { MuxcodeMax,       STROBE_BASED | RANK_BASED, FALSE }, //RDDTTI:5023
  { CkeAll,           STROBE_BASED | RANK_BASED, FALSE },
  { OdtSegEnOff,      STROBE_BASED | RANK_BASED, FALSE },
  { OdtEnOff,         STROBE_BASED | RANK_BASED, FALSE },
  { DiffampOff,       STROBE_BASED | RANK_BASED, FALSE },
  { RdRk2RkLatency,   STROBE_BASED | RANK_BASED, FALSE },
  { WrRk2RkLatency,   STROBE_BASED | RANK_BASED, FALSE },
// Required for Per Bit De-skew
  { RxDqBitDelay,     STROBE_BASED | BIT_BASED , FALSE },
  { TxDqBitDelay,     STROBE_BASED | BIT_BASED , FALSE },
};

UINT32 RestoreDataDescriptorTableCount = sizeof(RestoreDataDescriptorTable) / sizeof(RESTORE_DATA_DESCRIPTOR);


//extern UINT32 WrVRef[MAX_RANKS][MAX_CHANNELS][MAX_STROBES][MAX_BITS];;
char *MAXPI_STATE_NAME[]={
  "MAXPI_INIT",
  "MAXPI_DLLINIT",
  "MAXPI_TXDLL_ERROR",
  "MAXPI_CHECK_ACROSS_FAMILY",
  "MAXPI_SETUP_WIDER_RANGE",
  "MAXPI_FINAL_VALUES",
  "MAXPI_ERROR"
};

/*++

Routine Description:

  Save/Restores platform specific trained values.

Arguments:

  *MrcData           :  Main MRC data structure.

Returns:

  Nothing

--*/
VOID
ProjectSpecificSaveRestores (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel,
  SAVE_RESTORE_TASK Task
)
{
  UINT8 Size;
  int x;
  if (Task == SAVE) {
    //MrcDeadLoop();
    MmrcDebugPrint((MMRC_DBG_MIN, "<> CH%02d Saving trained values <>\n", Channel));
    MmrcExecuteTask(MrcData, S0_fixes_GET, NO_PRINT, Channel);
    //Now we need to copy to the structure that actually gets saved to NVRAM
    ////Size = S0_Fixes17 - S0_Fixes0;
    Size = sizeof(MrcData->Channel[Channel].Storage) / sizeof(MrcData->Channel[Channel].Storage[0]);
    for (x = 0; x < Size; x++) {
      MrcData->Channel[Channel].Storage[x] = MrcData->DynamicVars[Channel][S0_Fixes0 + x];
      MmrcDebugPrint((MMRC_DBG_MAX, "<> CH%02d Value: 0x%08x <>\n", Channel, MrcData->DynamicVars[Channel][S0_Fixes0 + x]));
    }
  } else if (Task == RESTORE) {
    MmrcDebugPrint((MMRC_DBG_MIN, "<> CH%02d Restoring trained values <>\n", Channel));
    //Size = S0_Fixes17 - S0_Fixes0;
    Size = sizeof(MrcData->Channel[Channel].Storage) / sizeof(MrcData->Channel[Channel].Storage[0]);
    for (x = 0; x < Size; x++) {
      MrcData->DynamicVars[Channel][S0_Fixes0 + x] = MrcData->Channel[Channel].Storage[x];
      MmrcDebugPrint((MMRC_DBG_MAX, "<> CH%02d Value: 0x%08x <>\n", Channel, MrcData->DynamicVars[Channel][S0_Fixes0 + x]));
    }

    //  MrcData->NPKMSC1Base = 0xdeaddead;  //TODO delete

    MmrcExecuteTask(MrcData, S0_fixes_SET, NO_PRINT, Channel);

    //  MrcData->NPKMSC1Base = 0x0;  //TODO delete
  }

}

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
  ) {
    TXPBDBLCTL0_DQ_STRUCT  txpbdblctl0;
    TXPBDBLCTL1_DQ_STRUCT  txpbdblctl1;
    TXPBDRK0CTL1_DQ_STRUCT txbpbdrk0ctl1;
    REGISTER_ACCESS     Register;
    Register.Mask = 0xFFFFFFFF;
    Register.ShiftBit = 0;
    txpbdblctl0.Data   = 0;
    txpbdblctl1.Data   = 0;
    txbpbdrk0ctl1.Data = 0;

   if ( Bit >=0 && Bit <= 2) {
     Register.Offset = TXPBDBLCTL0_DQ_REG;
     txpbdblctl0.Data = MemRegRead (DQCCC, Channel, Strobe, Register);
   }
   if ( Bit >= 3 && Bit <= 6 ) {
     Register.Offset = TXPBDBLCTL1_DQ_REG;
     txpbdblctl1.Data = MemRegRead (DQCCC, Channel, Strobe, Register);
   }
   if (Bit == 7 ) {
     Register.Offset = TXPBDRK0CTL1_DQ_REG;
     txbpbdrk0ctl1.Data = MemRegRead (DQCCC, Channel, Strobe, Register);
   }

    switch (Bit) {
    case 0:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl0.Bits.tx_bldq1_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl0.Bits.tx_bldq1_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl0.Bits.tx_bldq1_txpbdcode;
      }
      break;
    case 1:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl0.Bits.tx_bldq2_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl0.Bits.tx_bldq2_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl0.Bits.tx_bldq2_txpbdcode;
      }
      break;
    case 2:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl0.Bits.tx_bldq3_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl0.Bits.tx_bldq3_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl0.Bits.tx_bldq3_txpbdcode;
      }
      break;
    case 3:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl1.Bits.tx_bldq4_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl1.Bits.tx_bldq4_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl1.Bits.tx_bldq4_txpbdcode;
      }
      break;
    case 4:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl1.Bits.tx_bldq5_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl1.Bits.tx_bldq5_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl1.Bits.tx_bldq5_txpbdcode;
      }
      break;
    case 5:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl1.Bits.tx_bldq6_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl1.Bits.tx_bldq6_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl1.Bits.tx_bldq6_txpbdcode;
      }
      break;
    case 6:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txpbdblctl1.Bits.tx_bldq7_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txpbdblctl1.Bits.tx_bldq7_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txpbdblctl1.Bits.tx_bldq7_txpbdcode;
      }
      break;
    default:
      if ( (Cmd & WR_OFF)==WR_OFF) {
        txbpbdrk0ctl1.Bits.tx_bldm_txpbdcode += *Value;
      }
      if (Cmd == FC_WR) {
        txbpbdrk0ctl1.Bits.tx_bldm_txpbdcode = *Value;
      }
      if (Cmd == RD_REG) {
        (*Value) = (UINT32) txbpbdrk0ctl1.Bits.tx_bldm_txpbdcode;
      }
      break;
    }

    if ( ((Cmd & WR_OFF)==WR_OFF || (Cmd & FC_WR) == FC_WR) &&
         (Bit >= 0 && Bit <= 2) ){
        Register.Offset = TXPBDBLCTL0_DQ_REG;
        MemRegWrite(DQCCC, Channel, Strobe, Register, txpbdblctl0.Data, 0xF);
    }
    if ( ((Cmd & WR_OFF)==WR_OFF || (Cmd & FC_WR) == FC_WR) &&
         (Bit >= 3 && Bit <= 6) ){
        Register.Offset = TXPBDBLCTL1_DQ_REG;
        MemRegWrite(DQCCC, Channel, Strobe, Register, txpbdblctl1.Data, 0xF);
    }
    if ( ((Cmd & WR_OFF)==WR_OFF || (Cmd & FC_WR) == FC_WR) &&
         (Bit == 7) ){
        Register.Offset = TXPBDRK0CTL1_DQ_REG;
        MemRegWrite(DQCCC, Channel, Strobe, Register, txbpbdrk0ctl1.Data, 0xF);
    }

    return MMRC_SUCCESS;
}

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
  ) {

  UINT8 VrefCoding;
#if RTWT_SIM==1
  if ((Cmd&WR_OFF)==WR_OFF) {
    MrcData->Channel[Channel].WrVref[Rank] += *Value;
  }
  if (Cmd == FC_WR) {
    MrcData->Channel[Channel].WrVref[Rank] = *Value;
  }
  if (Cmd==CMD_GET_REG) {
    *Value = MrcData->Channel[Channel].WrVref[Rank];
  }
  return MMRC_SUCCESS;
#endif
  if ((Cmd&WR_OFF)==WR_OFF) {
    MrcData->Channel[Channel].WrVref[Rank][0] += *Value;
  }
   if (Cmd == FC_WR) {
    MrcData->Channel[Channel].WrVref[Rank][0] = *Value;
  }
     if (Cmd==CMD_GET_REG || Cmd==RD_REG) {
    *Value = MrcData->Channel[Channel].WrVref[Rank][0];
    return MMRC_SUCCESS;
  }
  //return MMRC_SUCCESS;
  //
  // The value passed into GetSetTxVref is the percantage of change to Vddq.  This number is in
  // 10ths of a percentage.
  //
  if (MrcData->Channel[Channel].WrVref[Rank][0] > 420) {
    //
    // If Value is outside of the range of the programming.
    //
    return MMRC_SUCCESS;
  } else if (MrcData->Channel[Channel].WrVref[Rank][0] < 220) {
    //
    // Use Range 0 if in range.
    //
    VrefCoding = (UINT8) (MrcData->Channel[Channel].WrVref[Rank][0] - 100)/4;
  } else {
    //
    // Otherwise use Range 1
    //
    VrefCoding = (UINT8) (MrcData->Channel[Channel].WrVref[Rank][0] - 220)/4;
    //
    // Add the 1 extra bit for range 1 selection.
    //
    VrefCoding += (1<<6);
  }

  MrcData->DynamicVars[Channel][RANK]        = Rank;
  MrcData->DynamicVars[Channel][RDDCMD]      = 0;
  MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;
  MrcData->DynamicVars[Channel][REG_DATA]    = MRW_LP4(14, VrefCoding);
  SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

  return MMRC_SUCCESS;
}

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
  )
{
  UINT8 VrefCoding;

  if ((Cmd&WR_OFF)==WR_OFF) {
    MrcData->Channel[Channel].CaVref[Rank] += *Value;
  }
  if (Cmd == FC_WR) {
    MrcData->Channel[Channel].CaVref[Rank] = *Value;
  }

  if (Cmd==CMD_GET_REG || Cmd==RD_REG) {
    *Value = MrcData->Channel[Channel].CaVref[Rank];
    //MmrcDebugPrint ((MMRC_DBG_MIN, "Ch:%d Rk:%d CaVref:%d\n", Channel, Rank, *Value));
    return MMRC_SUCCESS;
  }

  //MmrcExecuteTask (MrcData, modmem_init_dramreset_seq, NO_PRINT, Channel);
  //InitializeJedec (MrcData, 0, NO_PRINT, Channel);
  //LP4AllMRProgramming (MrcData, 0, NO_PRINT, Channel);
  //LP4MRProgramming_MPCZQ (MrcData, 0, NO_PRINT, Channel);
  //MrcDelay (MrcData, MICRO_DEL, 100);

  if (MrcData->Channel[Channel].CaVref[Rank] > 420) {
    //
    // If Value is outside of the range of the programming.
    //
    //MmrcDebugPrint ((MMRC_DBG_MIN, "Invalid Vref: Ch:%d Rk:%d CaVref:%d - No Progrmaming\n", Channel, Rank, MrcData->Channel[Channel].CaVref[Rank]));
    return MMRC_SUCCESS;
  } else if (MrcData->Channel[Channel].CaVref[Rank] < 220) {
    //
    // Use Range 0 if in range.
    //
    VrefCoding = (UINT8) (MrcData->Channel[Channel].CaVref[Rank] - 100)/4;
  } else {
    //
    // Otherwise use Range 1
    //
    VrefCoding = (UINT8) (MrcData->Channel[Channel].CaVref[Rank] - 220)/4;
    //
    // Add the 1 extra bit for range 1 selection.
    //
    VrefCoding += (1<<6);
  }
  //VrefCoding = (MrcData->Channel[Channel].WrVref[Rank] & 0x3F) | BIT6;

  MrcData->DynamicVars[Channel][RANK]        = Rank;
  MrcData->DynamicVars[Channel][RDDCMD]      = 0;
  MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;
  MrcData->DynamicVars[Channel][REG_DATA]    = MRW_LP4(12, VrefCoding);
  //MmrcDebugPrint ((MMRC_DBG_MIN, "Ch:%d Rk:%d CaVrefCode:%d REGDATA:0x%08x\n", Channel, Rank, VrefCoding, MRW_LP4(12, VrefCoding)));
  SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

  MrcDelay(MrcData, NANO_DEL, 100);

  MySimStall(100);

  //MrcDelay(MrcData,MICRO_DEL, 50);

  return MMRC_SUCCESS;
}

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
  )
{
  UINT32 DQDriveValue2x;

  if ((Cmd & FC_WR) == FC_WR)  {
    GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, TxDqDelay_Actual, Cmd, Value);
    //GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, (GetOffsetIndex (MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_GET_REG, &DQDriveValue2x);
    DQDriveValue2x = *Value;
    DQDriveValue2x -= 2 * GetHalfClk(MrcData, Channel);
    GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, TxDqDrive, CMD_SET_VAL_FC_UC, &DQDriveValue2x);
  } else {
    GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, TxDqDelay_Actual, Cmd, Value);
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
ReceiveEnableExit(
  MMRC_DATA         *MrcData,
  UINT8             Channel
)
/*++

Routine Description:

  The function is to be called after Receive Enable and sets the ODT and Diffamp offsets to the same
  2x value of receive enable for each strobe.

Arguments:

  MrcData:       Host structure for all data related to MMRC
  Channel:       Current Channel being examined.

Returns:

  MMRC_SUCCESS as passed
  Others as failed

--*/
{
  UINT32 rcvenval;
  UINT8  rk, st;
  //
  // Loop through each active rank and strobe.
  //
  for (rk=0; rk<MAX_RANKS; rk++)
  {
    if (!RunOnThisChannel (MrcData, Channel, rk)) {
         continue;
    }
    for (st=0; st< MAX_STROBES; st++)
    {
      if (!IsStrobeValid (MrcData, Channel, rk, st)) {
        continue;
      }
      //
      // Read the original 2x value from the Receive Enable.
      //
      GetSetDdrioGroup2 (MrcData, Channel, rk, st, (RecEnDelay), CMD_GET_REG, &rcvenval);
      //
      // Determine the 2x value by dividing by the 2x size and rounding down.
      //
      rcvenval = rcvenval/GetHalfClk(MrcData,Channel);
      //
      // Set the OdtSeg, OdtEn, and DiffAmp offsets to the 2x Rcven value.
      //
      GetSetDdrioGroup2 (MrcData, Channel, rk, st, OdtSegEnOff, CMD_SET_VAL_FC_UC, &rcvenval);
      GetSetDdrioGroup2 (MrcData, Channel, rk, st, OdtEnOff, CMD_SET_VAL_FC_UC, &rcvenval);
      GetSetDdrioGroup2 (MrcData, Channel, rk, st, DiffampOff, CMD_SET_VAL_FC_UC, &rcvenval);
    }
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
  LP4DelayTest(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
  )
{
  UINT8  Rank, Strobe;
  UINT32 DQSValue, DQValue, HalfClock, PiInPs;

  HalfClock = GetHalfClk(MrcData, Channel);
  PiInPs = (MrcData->TwoXps + HalfClock/2) / HalfClock;

  for (Rank=0; Rank< MAX_RANKS; Rank++)
  {
    for (Strobe=0; Strobe < MrcData->Channel[Channel].MaxDq[Rank]; Strobe++)
    {
      if (!IsStrobeValid (MrcData, Channel, Rank, Strobe)) {
        continue;
      }
      GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, TxDqsDelay, CMD_GET_REG, &DQSValue);
      //
      // Tether DQS and DQ based on Memory Type (can be removed when running CWL)
      //
      DQValue = DQSValue + GetHalfClk (MrcData, Channel)/2 + GetHalfClk (MrcData, Channel);
      GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, TxDqDelay, CMD_SET_VAL_FC_UC, &DQValue);
    }
  }
  return MMRC_SUCCESS;
}


VOID
BreakpointMrc (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         *Name
)
/*++

Routine Description:

  Configures a checkpoint to halt the CPU at any point in the MRC code, this
  function is used for debug, and never should be left in the code after debug
  is completed.
  This always halts where the function is placed, in order to continue developer
  must write a '0' into the SSKPD4_DUNIT_REG register


Arguments:

  MrcData:       Host structure for all data related to MMRC
  Channel:          Current Channel being examined.
  Name:             Pointer with the Name of the Breakpoint


Returns:

  None

--*/
{
#if 0
  UINT32  Data32;
  REGISTER_ACCESS     Register;

  Register.Offset = SSKPD4_DUNIT_COMMON_REG;
  Register.Mask = 0xFFFFFFFF;
  MmrcDebugPrint ((MMRC_DBG_MIN, "BN: '%a'. Write '0' to SSKPD4 in order to continue\n", Name));

  MemRegWrite (DUNIT_COMMON, 0, 0, Register, 0x80, 0xf);
  do {
    Data32 = (UINT32) MemRegRead (DUNIT_COMMON, 0, 0, Register);
  } while (Data32 != 0);
  MemRegWrite (DUNIT_COMMON, 0, 0, Register, 0, 0xf);
#endif
}


UINTX ReadReg(UINT32 Offset, UINT8 ch, UINT8 dq)
{
  REGISTER_ACCESS Register;
  Register.Offset = Offset;
  Register.ShiftBit = 0;
  Register.Mask = 0xFFFFFFFF;
  return MemRegRead (DQCCC, ch, dq, Register);
}

#ifdef JTAG
void FineContinuousLoop(
  IN  OUT   MMRC_DATA  *MrcData
)
{
  UINT8                              ch, dq, rk;
  UINTX                              Value;
  DLLCOMP_CODE_STATUS_DQCCC_STRUCT*  dllcmpcode;
  UINT32                             local_txdll_stat_fbmuxsel, local_coarsecode, local_coarseicode, local_finecode;
  DLLCOMP_FINECTL_DQCCC_STRUCT*      finemode;
  REGISTER_ACCESS                    Register;

  for(ch=0; ch< MAX_CHANNELS; ch++)
  {
    if (MrcData->Channel[ch].Enabled == TRUE)
    {
      for(rk=0; rk< MAX_RANKS; rk++)
      {
        if (MrcData->Channel[ch].RankEnabled[rk] == TRUE)
        {
          for (dq=0; dq < MrcData->Channel[ch].MaxDq[rk]; dq++)
          {
            Value = ReadReg(DLLCOMP_FINECTL_DQCCC_REG, ch, dq);
            finemode = (DLLCOMP_FINECTL_DQCCC_STRUCT*) &Value;
            finemode->Bits.finec_mode=1;
            Register.Offset=DLLCOMP_FINECTL_DQCCC_REG;
            Register.Mask = 0xFFFFFFFF;
            Register.ShiftBit = 0;
            MemRegWrite (DQCCC, ch, dq, Register, Value, 0xf);
          }
        }
      }
    }
  }

  while (1)
  {
    for(ch=0; ch< MAX_CHANNELS; ch++)
    {
      if (MrcData->Channel[ch].Enabled == TRUE)
      {
        for(rk=0; rk< MAX_RANKS; rk++)
        {
          if (MrcData->Channel[ch].RankEnabled[rk] == TRUE)
          {
            if ((MrcData->Channel[ch].CurrentDdrType == T_LPDDR3) || (MrcData->Channel[ch].CurrentDdrType == T_LPDDR4))
            {
              for (dq=0; dq < MrcData->Channel[ch].MaxDq[rk]; dq++)
              {
                Value = ReadReg(DLLCOMP_CODE_STATUS_DQCCC_REG, ch, dq);
                dllcmpcode = (DLLCOMP_CODE_STATUS_DQCCC_STRUCT*) &Value;
                local_txdll_stat_fbmuxsel = (UINT32) dllcmpcode->Bits.txdll_stat_fbmuxsel;
                local_coarsecode = (UINT32) dllcmpcode->Bits.txdll_stat_coarsecode;
                local_coarseicode = (UINT32) dllcmpcode->Bits.txdll_stat_coarseicode;
                local_finecode = (UINT32) dllcmpcode->Bits.txdll_stat_finecode;

                MmrcDebugPrint ((MMRC_DBG_MIN, "Fine Cont Sampling: CH %d:DQ %d:RK %d:Val 0x%08x Coarse:%d Coarsei:%d Fine:%d\r\n", ch, dq, rk, local_txdll_stat_fbmuxsel, local_coarsecode, local_coarseicode, local_finecode));
                //Sleep(10000);
              }
            }
          }
        }
      }
    }
  }
}
#endif

void DoDllInit(
  IN  OUT   MMRC_DATA  *MrcData,
  IN  OUT   MAXPI_INFO *MaxPiInfo
)
{
  UINT8                              ch, dq, rk;
  UINTX                              Value;
  DLLCOMP_CODE_STATUS_DQCCC_STRUCT*  dllcmpcode;
  DLLCOMP_ERROR_STATUS_DQCCC_STRUCT* dllcomp_error_status;
  UINT32                             local_txdll_stat_fbmuxsel, local_txdll_error, local_coarsecode, local_coarseicode, local_finecode;
  UINT8                              local_ctlebias, local_dbselprocess;
  UINT32                             local_txdll_errorstatus;

  MaxPiInfo->txdll_stat_fbmuxsel_max=0;
  MaxPiInfo->txdll_error=0;
  //MaxPiInfo->muxcode_min=255;
  //MaxPiInfo->muxcode_max=0;
  MaxPiInfo->code_count=0;
  MaxPiInfo->code_same_count=0;
  MaxPiInfo->last_max = 0;
  //
  //  Find Max Code
  //
  for(ch=0; ch< MAX_CHANNELS; ch++)
  {
    if (MrcData->Channel[ch].Enabled == TRUE)
    {
      MmrcExecuteTask (MrcData, maxpi_set_codes, NO_PRINT, ch);
      MmrcExecuteTask (MrcData, maxpi_dll_init, NO_PRINT, ch);
      for(rk=0; rk< MAX_RANKS; rk++)
      {
        if (MrcData->Channel[ch].RankEnabled[rk] == TRUE)
        {
          if ((MrcData->Channel[ch].CurrentDdrType == T_LPDDR3) || (MrcData->Channel[ch].CurrentDdrType == T_LPDDR4))
          {
            for (dq=0; dq < MrcData->Channel[ch].MaxDq[rk]; dq++)
            {
              if (!IsStrobeValid (MrcData, ch, rk, dq))
                continue;
              MaxPiInfo->code_count++;
              //
              //  Read Comp Code per DQ (Can't use the spreadsheet since it returns only per channel)
              //
              Value = ReadReg(DLLCOMP_ERROR_STATUS_DQCCC_REG, ch, dq);
              //MmrcDebugPrint ((MMRC_DBG_MIN, "Waiting for CH %d:DQ %d to return !0\r\n", ch, dq));
              //while (Value ==0)
              //{
              //  Value = ReadReg(DLLCOMP_ERROR_STATUS_DQCCC_REG, ch, dq);
              //}
              dllcomp_error_status = (DLLCOMP_ERROR_STATUS_DQCCC_STRUCT*) &Value;
              //local_txdll_error = (UINT32) dllcomp_error_status->Bits.txdll_error;
              local_txdll_error = (UINT32) ((Value == 1) ? 0 : 1);
              MaxPiInfo->txdll_error |= local_txdll_error;
              local_txdll_errorstatus = (UINT32) Value;

              Value = ReadReg(DLLCOMP_CODE_STATUS_DQCCC_REG, ch, dq);
              dllcmpcode = (DLLCOMP_CODE_STATUS_DQCCC_STRUCT*) &Value;
              local_txdll_stat_fbmuxsel = (UINT32) dllcmpcode->Bits.txdll_stat_fbmuxsel;
              local_coarsecode = (UINT32) dllcmpcode->Bits.txdll_stat_coarsecode;
              local_coarseicode = (UINT32) dllcmpcode->Bits.txdll_stat_coarseicode;
              local_finecode = (UINT32) dllcmpcode->Bits.txdll_stat_finecode;


              if (local_txdll_error == 0)
              {
                if (local_txdll_stat_fbmuxsel > MaxPiInfo->txdll_stat_fbmuxsel_max)
                {
                  MaxPiInfo->last_max = MaxPiInfo->txdll_stat_fbmuxsel_max;
                  MaxPiInfo->txdll_stat_fbmuxsel_max = local_txdll_stat_fbmuxsel;
                }
                if (local_txdll_stat_fbmuxsel < MaxPiInfo->txdll_stat_fbmuxsel_min)
                {
                  MaxPiInfo->txdll_stat_fbmuxsel_min = local_txdll_stat_fbmuxsel;
                }
                //
                //  Track the txdll blocks with the largest values (All need to have the same value)
                //
                if ((MaxPiInfo->last_max == local_txdll_stat_fbmuxsel) || (MaxPiInfo->last_max == 0 && MaxPiInfo->last_max != local_txdll_stat_fbmuxsel))
                  MaxPiInfo->code_same_count++;
                if (MrcData->EnableVocCtle==TRUE) local_ctlebias = ProgramCtle(ch, dq, local_coarsecode);
                else MmrcMaxPiDllDebugPrint ((MMRC_DBG_MIN, "CTLEBIAS not being programmed\n"));
                if (MrcData->EnableMaxPiDbselProcess==TRUE) local_dbselprocess = ProgramProcess(ch, dq, MrcData->Channel[ch].CurrentFrequency, (UINT8) MaxPiInfo->txdll_stat_fbmuxsel_max, (UINT8)local_coarsecode);
                else MmrcMaxPiDllDebugPrint ((MMRC_DBG_MIN, "DbSelProcess not being programmed\n"));
              }
              //MmrcMaxPiDllDebugPrint ((MMRC_DBG_MIN, "CH %d:DQ %d:RK %d:Val 0x%08x:Max 0x%08x Err:%d Coarse:%d Coarsei:%d Fine:%d ctlebias=%d\n", ch, dq, rk, local_txdll_stat_fbmuxsel, MaxPiInfo->txdll_stat_fbmuxsel_max, MaxPiInfo->txdll_error, local_coarsecode, local_coarseicode, local_finecode, local_ctle));
              MmrcMaxPiDllDebugPrint ((MMRC_DBG_MIN, "CH %d:DQ %d:RK %d:Val 0x%08x:Max 0x%08x Err:%d Coarse:%d Coarsei:%d Fine:%d ctlebias=%d Process=%d ErrStat=0x%08x\n", ch, dq, rk, local_txdll_stat_fbmuxsel, MaxPiInfo->txdll_stat_fbmuxsel_max, MaxPiInfo->txdll_error, local_coarsecode, local_coarseicode, local_finecode, local_ctlebias, local_dbselprocess, local_txdll_errorstatus));
            }
          }
        }
      }
    }
  }
}

void CopyPiCodesToAllChannels(
  IN  OUT   MMRC_DATA  *MrcData,
  IN  OUT   MAXPI_INFO *MaxPiInfo,
  IN        UINT8      Channel
  )
{
  UINT8   ch;
  MaxPiInfo->muxcode_min = (UINT8) MrcData->DynamicVars[Channel][MUX_MIN];
  MaxPiInfo->muxcode_max = (UINT8) MrcData->DynamicVars[Channel][MUX_MAX];
  for(ch=0; ch< MAX_CHANNELS; ch++)
  {
    MrcData->DynamicVars[ch][FINE_MIN]   = MrcData->DynamicVars[Channel][FINE_MIN];
    MrcData->DynamicVars[ch][FINE_MAX]   = MrcData->DynamicVars[Channel][FINE_MAX];
    MrcData->DynamicVars[ch][MUX_MIN]    = MrcData->DynamicVars[Channel][MUX_MIN];
    MrcData->DynamicVars[ch][MUX_MAX]    = MrcData->DynamicVars[Channel][MUX_MAX];
    MrcData->DynamicVars[ch][COARSE_MIN] = MrcData->DynamicVars[Channel][COARSE_MIN];
    MrcData->DynamicVars[ch][COARSE_MAX] = MrcData->DynamicVars[Channel][COARSE_MAX];
  }
}

MMRC_STATUS PrintLimits(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
  )
{
  MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Fine Hard: 0x%x - 0x%x\r\n", (UINT32)MrcData->DynamicVars[Channel][FINE_MIN], (UINT32)MrcData->DynamicVars[Channel][FINE_MAX]));
  MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Mux      : 0x%x - 0x%x\r\n", (UINT32)MrcData->DynamicVars[Channel][MUX_MIN], (UINT32)MrcData->DynamicVars[Channel][MUX_MAX]));
  MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Coarse   : 0x%x - 0x%x\r\n", (UINT32)MrcData->DynamicVars[Channel][COARSE_MIN], (UINT32)MrcData->DynamicVars[Channel][COARSE_MAX]));
  MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Fine Soft: 0x%x - 0x%x\r\n", (UINT32)MrcData->DynamicVars[Channel][FINE_SOFT_MIN], (UINT32)MrcData->DynamicVars[Channel][FINE_SOFT_MAX]));
  return MMRC_SUCCESS;
}

UINT8 GetCtleCode(UINT32 coarse_code)
{
                          //Min|Max|CtleCode
  /*
  TXDLL coarse code(decimal)  drx_cc_ctlebiasctrl/
  drx_bl_ctlebiasctrl
0 to 2  2
3 to 5  3
4 to 6  4
7 to 11  5
11 to 15  6


  */
                          //Min|Max|CtleCode
  UINT8 coarsetable[][3] = { { 0,  2,  2},
                             { 3,  5,  3},
                             { 4,  6,  4},
                             { 7, 11,  5},
                             {11, 15,  6} };
  const int entries = sizeof(coarsetable)/3;
  int i;
  UINT8 min,max;

  for (i=0; i< entries; i++)
  {
    min=coarsetable[i][0];
    max=coarsetable[i][1];
    if (coarse_code >= min && coarse_code <= max)
      return coarsetable[i][2];
  }
  return 0xff;
}

UINT8 GetDbselProcessCode(INT32 freq, UINT8 muxcode, UINT8 coarsecode)
{

  /*

THIS VALUE OR HIGHER, DBSEL2 --> 0, else DBSEL --> 1, if N/A --> 0

TECH  SPEED  Values?  DBS2 DBS2 DBS2 DBS2
LP3  1866    B,C,D,E  N/A  N/A  N/A  N/A
LP3  1600    E,F,10   N/A  N/A  N/A  N/A
LP3  1333    E,F,10   N/A  N/A  N/A  N/A
LP3  1066    11,12,13 N/A  N/A  N/A  N/A
LP3  800     17,18,19 N/A  N/A  N/A  N/A
LP4  3200    6,7,8    12   11   9
LP4  2933    7,8,9    12   11   10
LP4  2667    7,8,9,A  12   11   11   9
LP4  2133    A,B,C    12   11   10
LP4  1866    B,C,D,E  12   12   11   9
LP4  1600    E,F,10   12   11   10
LP4  1066    12,13     9    9


  */
                              //freq|MuxC|CoarseCode
  INT32 processtable[][3] = { { MMRC_3200,   6,  12},
                              { MMRC_3200,   7,  11},
                              { MMRC_3200,   8,   9},
                              { MMRC_2933,   7,  12},
                              { MMRC_2933,   8,  11},
                              { MMRC_2933,   9,  10},
                              { MMRC_2666,   7,  12},
                              { MMRC_2666,   8,  11},
                              { MMRC_2666,   9,  11},
                              { MMRC_2666,  10,   9},
                              { MMRC_2133,  10,  12},
                              { MMRC_2133,  11,  11},
                              { MMRC_2133,  12,  10},
                              { MMRC_1866,  11,  12},
                              { MMRC_1866,  12,  12},
                              { MMRC_1866,  13,  11},
                              { MMRC_1866,  14,   9},
                              { MMRC_1600,  14,  12},
                              { MMRC_1600,  15,  11},
                              { MMRC_1600,  16,  10},
                              { MMRC_1066,  18,   9},
                              { MMRC_1066,  19,   9} };
  const int entries = sizeof(processtable)/(3 * sizeof(processtable[0][0]));
  int i;
  INT32 cc, f, mc;
  //MmrcDebugPrint ((MMRC_DBG_MIN, "F=%d cc=%d mc=%d\r\n", freq, coarsecode, muxcode));
  for (i=0; i< entries; i++)
  {
    f  =processtable[i][0];
    mc =processtable[i][1];
    cc =processtable[i][2];
    //MmrcDebugPrint ((MMRC_DBG_MIN, "->F=%d cc=%d mc=%d\r\n", f, cc, mc));
    if (freq == f && mc == muxcode)
      return (coarsecode < cc) ? 1:0;
  }
  return 0xff;
}

UINT8 ProgramCtle(UINT8 ch, UINT8 dq, UINT32 coarse_code)
{
  CTLECTL1_DQCCC_STRUCT*  dq0_ctlectl;
  UINTX                   Value;
  REGISTER_ACCESS         Register;
  UINT8                   ctleval = GetCtleCode(coarse_code);

  Register.ShiftBit=0;
  Register.Mask    = 0xffffffff;
  if (ctleval != 0xFF)
  {
    //
    //  Program BL, and CC in each DQ module
    //
    Register.Offset = CTLECTL1_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    dq0_ctlectl = (CTLECTL1_DQCCC_STRUCT *) &Value;
    dq0_ctlectl->Bits.drx_bl_ctlebiasctrl=ctleval;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
  }
  return ctleval;
}
UINT8 ProgramProcess(UINT8 ch, UINT8 dq, INT32 freq, UINT8 muxcode, UINT8 coarsecode)
{
  //DQCCC  txdllsiggrp0ctl  7C  14  14  txdll_rk0siggrp00dbsel_process
  TXDLLSIGGRP0CTL_DQCCC_STRUCT* siggrp0;
  TXDLLSIGGRP1CTL_DQCCC_STRUCT* siggrp1;
  TXDLLSIGGRP2CTL_DQCCC_STRUCT* siggrp2;
  TXDLLSIGGRP3CTL_DQCCC_STRUCT* siggrp3;
  TXDLLSIGGRP4CTL_DQCCC_STRUCT* siggrp4;
  TXDLLSIGGRP5CTL_DQCCC_STRUCT* siggrp5;
  TXDLLSIGGRP6CTL_DQCCC_STRUCT* siggrp6;
  UINT8 Process=GetDbselProcessCode(freq, muxcode, coarsecode);
  UINTX   Value;
  REGISTER_ACCESS Register;
  Register.ShiftBit = 0;
  Register.Mask = 0xffffffff;

  if (Process != 0xFF)
  {
    Register.Offset = TXDLLSIGGRP0CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp0 = (TXDLLSIGGRP0CTL_DQCCC_STRUCT *) &Value;
    siggrp0->Bits.txdll_rk0siggrp00dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP1CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp1 = (TXDLLSIGGRP1CTL_DQCCC_STRUCT *) &Value;
    siggrp1->Bits.txdll_rk0siggrp01dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP2CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp2 = (TXDLLSIGGRP2CTL_DQCCC_STRUCT *) &Value;
    siggrp2->Bits.txdll_rk0siggrp02dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP3CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp3 = (TXDLLSIGGRP3CTL_DQCCC_STRUCT *) &Value;
    siggrp3->Bits.txdll_rk0siggrp03dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP4CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp4 = (TXDLLSIGGRP4CTL_DQCCC_STRUCT *) &Value;
    siggrp4->Bits.txdll_rk0siggrp04dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP5CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp5 = (TXDLLSIGGRP5CTL_DQCCC_STRUCT *) &Value;
    siggrp5->Bits.txdll_rk0siggrp05dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

    Register.Offset = TXDLLSIGGRP6CTL_DQCCC_REG;
    Value = MemRegRead (DQCCC, ch, dq, Register);
    siggrp6 = (TXDLLSIGGRP6CTL_DQCCC_STRUCT *) &Value;
    siggrp6->Bits.txdll_rk0siggrp06dbsel_process = Process;
    MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
  }
  return Process;
}

MMRC_STATUS SetupMaxPI(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channelx // This variable should not be used in SetupMaxPi
  )
{
  //
  //  This assumes that all LP3/4 Chanels are running at the same frequency (So only one variable is used to store the max value
  //
  UINT8   ch, dq, rk = 0;
  UINTX   Value;
  REGISTER_ACCESS Register;
  //DLLCOMP_FBMUXSEL_DQCCC_STRUCT* dllfbmux;
  TXDLLREF2XCTL_DQCCC_STRUCT*    txdll;
  RXDLLCTL_DQBLDM_DQCCC_STRUCT*  rxdllctl_dqbldm;
  DBREGIONCTL1_DPLVUG_STRUCT*    dbregionctl1;
  DBREGIONCTL2_DPLVUG_STRUCT*    dbregionctl2;
  DLLCOMP_FINECTL_DQCCC_STRUCT*  finectl;
  DLLCOMP_CODE_STATUS_DQCCC_STRUCT*  dllcmpcode;
  DLLCOMP_RXCOARSEOVR_DQCCC_STRUCT*  rxcoarseovr;
  DLLCOMP_RXFINEOVR_DQCCC_STRUCT*    rxfineovr;
  //DLLCOMP_ERROR_STATUS_DQCCC_STRUCT* dllcomp_error_status;
  PLLCTL_DPIC_STRUCT*            pllctl;
  MAXPI_INFO  MaxPiInfo;
  UINT32  local_coarsecode, local_finecode;
  UINT32  Coarse_Code_Max = 13, Coarse_Code_Min = 9, Fine_Code_Min, Fine_Code_Max, Muxcode_Max, Muxcode_Min, MuxcodeMinLowerLimit = 0;
  UINT32  Init_Fine_Code_Min, Init_Fine_Code_Max, Init_Muxcode_Max, Init_Muxcode_Min;
  MAXPI_STATES maxpi_state = MAXPI_INIT;
  UINT32  HalfClkPiFull, HalfRate;
  UINT8   complete = 0;
  UINT8   FinalDecision = 0;
  UINT32  a0offset;
  UINT32  MaxPiNewLimit, MaxPiOldLimit, MaxPiOldValue[3], MaxPiNewValue[3], grp;
  BOOLEAN RestorePath = FALSE;
  Register.ShiftBit = 0;
  Register.Mask = 0xffffffff;

  MmrcMemset(&MaxPiInfo, 0, sizeof (MAXPI_INFO));

  a0offset = 0;

  //
  // MrcData->SetupMaxPiDone[] is a Non-Volatile variable. It's state is preserved
  // accross resets and available in the S0 and FB flow.
  //
  if (MrcData->SetupMaxPiDone[GetCurrentFrequencyIndex()]) {
    //
    // If SetupMaxPI was previously completed for the current frequency, use the restore flow
    //
    RestorePath = TRUE;
  } else {
    //
    // Else, set the "Done" flag and continue with the full flow.
    //
    MrcData->SetupMaxPiDone[GetCurrentFrequencyIndex()] = TRUE;
  }

  while (complete == 0)
  {
    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI State = %s\r\n", MAXPI_STATE_NAME[maxpi_state]));
    switch (maxpi_state)
    {
    case MAXPI_INIT:
      for(ch=0; ch< MAX_CHANNELS; ch++)
      {
        if (MrcData->Channel[ch].Enabled == TRUE)
        {
          MmrcExecuteTask (MrcData, maxpi_override_coarse_init, NO_PRINT, ch);
        }
        for (dq=0; dq < MrcData->Channel[ch].MaxDq[rk]; dq++)
        {
          //
          // finec
          //
          Register.Offset = DLLCOMP_FINECTL_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          finectl = (DLLCOMP_FINECTL_DQCCC_STRUCT *) &Value;
          finectl->Bits.finec_mode = 0;
          if (ch==0 && dq==0)
            MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "setting finec=0\r\n"));
          MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
        }
      }
      //
      //  Get Starting Values
      //
      for(ch=0; ch< MAX_CHANNELS; ch++)
      {
        if (MrcData->Channel[ch].Enabled == TRUE)
        {
          //MmrcExecuteTask (MrcData, maxpi_override_coarse_init, NO_PRINT, ch);
          MmrcExecuteTask (MrcData, maxpi_get_codes, NO_PRINT, ch);
          if (RestorePath)
          {
            Coarse_Code_Max=14;
            Coarse_Code_Min=8;
          }
          else
          {
              Coarse_Code_Max=13;
              Coarse_Code_Min=9;
          }
          MrcData->DynamicVars[ch][COARSE_MIN]= Coarse_Code_Min;
          MrcData->DynamicVars[ch][COARSE_MAX]= Coarse_Code_Max;
          //
          //MrcData->DynamicVars[ch][MUX_MIN]=0x6;
          //MrcData->DynamicVars[ch][MUX_MAX]=0x6;
          //
          CopyPiCodesToAllChannels(MrcData, &MaxPiInfo, ch);
          Fine_Code_Min = (UINT32) MrcData->DynamicVars[ch][FINE_MIN];
          Fine_Code_Max = (UINT32) MrcData->DynamicVars[ch][FINE_MAX];
          Muxcode_Min = (UINT32) MrcData->DynamicVars[ch][MUX_MIN];
          Muxcode_Max = (UINT32) MrcData->DynamicVars[ch][MUX_MAX];

          Init_Fine_Code_Min = (UINT32) MrcData->DynamicVars[ch][FINE_MIN];
          Init_Fine_Code_Max = (UINT32) MrcData->DynamicVars[ch][FINE_MAX];
          Init_Muxcode_Min = (UINT32) MrcData->DynamicVars[ch][MUX_MIN];
          Init_Muxcode_Max = (UINT32) MrcData->DynamicVars[ch][MUX_MAX];
          MuxcodeMinLowerLimit = Init_Muxcode_Min-2;
          break;
        }
      }
      MmrcDebugPrint ((MMRC_DBG_MIN, "A0 Offset %d\r\n", a0offset));
      PrintLimits(MrcData, 0);
      if (!RestorePath)
      {
        maxpi_state =  MAXPI_DLLINIT;  // New Calc
      }
      else
      {
        maxpi_state = MAXPI_FINAL_VALUES;  // Restore
        for(ch=0; ch< MAX_CHANNELS; ch++)
        {
          if (MrcData->Channel[ch].Enabled == TRUE)
          {
            for(rk=0; rk< MAX_RANKS; rk++)
            {
              if (MrcData->Channel[ch].RankEnabled[rk] == TRUE)
              {
                //
                //  MrcData->MuxcodeNv[] is a Non-Volatile variable. It's state is preserved
                // accross resets and available in the S0 and FB flow.
                //
                MaxPiInfo.txdll_stat_fbmuxsel_max = MrcData->MuxcodeNv[GetCurrentFrequencyIndex()];
                MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "WarmBoot: Muxcode=%d\r\n", MaxPiInfo.txdll_stat_fbmuxsel_max));
                break;
              } //if RankEnabled
            } //for rk
            break;
          } //if ChannelEnabled
        } //for ch
      }
      break;
    case MAXPI_DLLINIT:
      //
      //  Do the Dll Init and get the error status.  If the error status is good then do the next
      //
      DoDllInit(MrcData, &MaxPiInfo);
      if (MaxPiInfo.txdll_error)
        maxpi_state =  MAXPI_TXDLL_ERROR;
      else
        maxpi_state =  MAXPI_CHECK_ACROSS_FAMILY;
      break;
    case MAXPI_TXDLL_ERROR:
      //
      //  If there is an error, decrement muxcode_min and try again.
      //  If muxcode_min is too low, then try increasing coarse range and start the process overa again
      //  If everything is good, then do the next test which is seeing if the values are same across all the instances.
      //
      for (ch=0; ch<MAX_CHANNELS; ch++)
      {
        MrcData->DynamicVars[ch][MUX_MIN]--;
      }
      DoDllInit(MrcData, &MaxPiInfo);
      if (MaxPiInfo.txdll_error)
      {
        if ((FinalDecision==1) || (MrcData->DynamicVars[0][MUX_MIN]< MuxcodeMinLowerLimit))
        {
          //No Hope
          maxpi_state = MAXPI_ERROR; //If it reaches the limit
          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "No solution\r\n"));
        }
        else
        {
          maxpi_state = MAXPI_TXDLL_ERROR;
          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_TXDLL_ERROR: MUX_MIN set to %08x\r\n", (UINT32)MrcData->DynamicVars[0][MUX_MIN]));
        }
      }
      else
      {
        MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_TXDLL_ERROR: No erros\r\n"));
        maxpi_state = MAXPI_CHECK_ACROSS_FAMILY;
      }
      break;
    case MAXPI_CHECK_ACROSS_FAMILY:
      //
      //  Check if all the muxcode values are the same
      //
      if ((MaxPiInfo.code_count == MaxPiInfo.code_same_count) || (FinalDecision==1))
        maxpi_state = MAXPI_FINAL_VALUES;
      else
        maxpi_state = MAXPI_SETUP_WIDER_RANGE;
      //
      //  All the courses require this step.
      //
      for (ch=0; ch<MAX_CHANNELS; ch++)
      {
        //
        //  Try a wider coarse range to see if we can find a common value between all the ones
        //
        MrcData->DynamicVars[ch][COARSE_MIN] = 8;
        MrcData->DynamicVars[ch][COARSE_MAX] = 14;
        //
        // Use the Highest Muxcode to lock to
        //
        MrcData->DynamicVars[ch][MUX_MIN] = MaxPiInfo.txdll_stat_fbmuxsel_max; //Ritesh
        MrcData->DynamicVars[ch][MUX_MAX] = MaxPiInfo.txdll_stat_fbmuxsel_max;
        MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_CHECK_ACROSS_FAMILY: Setting Ch%d Coarse Range from %08x to %08x\r\n", ch, (UINT32)MrcData->DynamicVars[ch][COARSE_MIN], (UINT32)MrcData->DynamicVars[ch][COARSE_MAX]));
        MmrcExecuteTask (MrcData, maxpi_set_codes, NO_PRINT, ch);
        MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_CHECK_ACROSS_FAMILY: Setting Ch%d Muxcode Range from %08x to %08x\r\n", ch, (UINT32)MrcData->DynamicVars[ch][MUX_MIN], (UINT32)MrcData->DynamicVars[ch][MUX_MAX]));
      }
      FinalDecision=1;
      break;
    case MAXPI_SETUP_WIDER_RANGE:
      maxpi_state =  MAXPI_DLLINIT;
      break;
    case MAXPI_FINAL_VALUES:
      //
      //  Program final Values
      //
      if (MaxPiInfo.txdll_stat_fbmuxsel_max == 0)
      {
        MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_FINAL_VALUES: Error: txdll_stat_fbmuxsel_max=0.  Either No Channels are enabled, or PhyInit did not come up with a valid value\r\n"));
        return MMRC_FAILURE;
      }

      if (!RestorePath)
      {
        MrcData->MuxcodeNv[GetCurrentFrequencyIndex()] = MaxPiInfo.txdll_stat_fbmuxsel_max;
      }

      //
      //  Fill Max Code in Phy Registers
      //
      for(ch=0; ch< MAX_CHANNELS; ch++)
      {
        if (MrcData->Channel[ch].Enabled == TRUE)
        {
          //
          // {1x, 2x, PI}
          //
          MrcData->HalfClkPi   = (UINT16) ((MaxPiInfo.txdll_stat_fbmuxsel_max + 1 - a0offset) * 8);
          MrcData->GranularitySteps[MrcData->CurrentBlueprint][ADdll[MrcData->DigitalDll]][MrcData->Channel[ch].CurrentFrequency][2] = 1;
          MrcData->GranularitySteps[MrcData->CurrentBlueprint][ADdll[MrcData->DigitalDll]][MrcData->Channel[ch].CurrentFrequency][1] = MrcData->HalfClkPi;
          MrcData->GranularitySteps[MrcData->CurrentBlueprint][ADdll[MrcData->DigitalDll]][MrcData->Channel[ch].CurrentFrequency][0] = 2 * MrcData->HalfClkPi;
          HalfClkPiFull = (UINT16) ((MaxPiInfo.txdll_stat_fbmuxsel_max + 1) * 8);
          HalfRate      =  (725 * MrcData->HalfClkPi)/HalfClkPiFull;
          if (ch==0)
          {
            MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_FINAL_VALUES: MrcData->HalfClkPi = 0x%x(%d ps Estimate) FullRangePi = 0x%x(%d ps Estimate) \r\n", MrcData->HalfClkPi, HalfRate, HalfClkPiFull, 725));
            MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "MAXPI_FINAL_VALUES: Muxcode = 0x%x\r\n", (UINT32)MaxPiInfo.txdll_stat_fbmuxsel_max));
          }
          // MrcData->DynamicVars[ch][MAXPI_OLD_LIMIT] = HalfClk [ADdll [MrcData->DigitalDll]][MrcData->Channel[Channel].CurrentFrequency];
          // MrcData->DynamicVars[ch][MAXPI_NEW_LIMIT] = MrcData->HalfClkPi;
          for(rk=0; rk< MAX_RANKS; rk++)
          {
            if (MrcData->Channel[ch].RankEnabled[rk] == TRUE)
            {
              if ((MrcData->Channel[ch].CurrentDdrType == T_LPDDR3) || (MrcData->Channel[ch].CurrentDdrType == T_LPDDR4))
              {
                for (dq=0; dq < MrcData->Channel[ch].MaxDq[rk]; dq++)
                {
                  //
                  // Program the Mux Selects
                  //
                  GetSetDdrioGroup2 (MrcData, ch, rk, dq, MuxcodeMin, CMD_SET_VAL_FC_UC, &MaxPiInfo.txdll_stat_fbmuxsel_max);
                  GetSetDdrioGroup2 (MrcData, ch, rk, dq, MuxcodeMax, CMD_SET_VAL_FC_UC, &MaxPiInfo.txdll_stat_fbmuxsel_max);
                  //Register.Offset = DLLCOMP_FBMUXSEL_DQCCC_REG;
                  //Register.ShiftBit = 0;
                  //Register.Mask = 0xFFFFFFFF;
                  //Value = MemRegRead (DQCCC, ch, dq, Register);
                  //dllfbmux = (DLLCOMP_FBMUXSEL_DQCCC_STRUCT*) &Value;
                  //dllfbmux->Bits.muxcode_max = MaxPiInfo.txdll_stat_fbmuxsel_max;
                  //dllfbmux->Bits.muxcode_min = MaxPiInfo.txdll_stat_fbmuxsel_max;
                  //MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

                  //
                  // Rithesh/Justin/Senthil
                  //
                  //Register.Offset = DLLCOMP_COARSECODE_CCC_REG;
                  //Value = MemRegRead (DQCCC, ch, dq, Register);
                  //coarsecode = (DLLCOMP_COARSECODE_CCC_STRUCT*) &Value;
                  //coarsecode->Bits.coarsecode_ovr = MaxPiInfo.txdll_stat_fbmuxsel_max;
                  //coarsecode->Bits.coarseicode_ovr = MaxPiInfo.txdll_stat_fbmuxsel_max;
                  //MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

                  //
                  // Program Phase Limits
                  //
                  Register.Offset = TXDLLREF2XCTL_DQCCC_REG;
                  Value = MemRegRead (DQCCC, ch, dq, Register);
                  txdll = (TXDLLREF2XCTL_DQCCC_STRUCT *) &Value;
                  txdll->Bits.txdll_ref2xph90picode  = (MrcData->HalfClkPi * 1 /4) - 1; //Ritesh
                  txdll->Bits.txdll_ref2xph180picode = (MrcData->HalfClkPi * 2 /4) - 1;
                  txdll->Bits.txdll_ref2xph270picode = (MrcData->HalfClkPi * 3 /4) - 1;
                  MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
                  if (ch==0 && dq==0)
                    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "90:0x%x 180:0x%x 270:0x%x ", (UINT32)txdll->Bits.txdll_ref2xph90picode, (UINT32)txdll->Bits.txdll_ref2xph180picode, (UINT32)txdll->Bits.txdll_ref2xph270picode));

                  //
                  // Program Rx Mux Selects
                  //
                  Register.Offset = RXDLLCTL_DQBLDM_DQCCC_REG;
                  Value = MemRegRead (DQCCC, ch, dq, Register);
                  rxdllctl_dqbldm = (RXDLLCTL_DQBLDM_DQCCC_STRUCT *) &Value;
                  rxdllctl_dqbldm->Bits.rxdll_rxmuxsel = MIN((MaxPiInfo.txdll_stat_fbmuxsel_max * 5 / 4) + 1, 0x1F); //Limit of 31
                  MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
                  if (ch==0 && dq==0)
                    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "rxdll_rxmuxsel:0x%x ", (UINT32)rxdllctl_dqbldm->Bits.rxdll_rxmuxsel));

                  //
                  // DB Regions
                  //
                  Register.Offset = DBREGIONCTL1_DPLVUG_REG;
                  Value = MemRegRead (DPLVUG, ch, 0, Register);
                  dbregionctl1 = (DBREGIONCTL1_DPLVUG_STRUCT *) &Value;
                  dbregionctl1->Bits.maxpi_per2xclk   = MrcData->HalfClkPi;
                  dbregionctl1->Bits.dbregion1_marker = ((MrcData->HalfClkPi * 1) / 8) - 1;
                  dbregionctl1->Bits.dbregion2_marker = ((MrcData->HalfClkPi * 2) / 8) - 1;
                  dbregionctl1->Bits.dbregion3_marker = ((MrcData->HalfClkPi * 3) / 8) - 1;
                  if (ch==0 && dq==0)
                    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "db1:0x%x, db2:0x%x, db3:0x%x ", (UINT32)dbregionctl1->Bits.dbregion1_marker, (UINT32)dbregionctl1->Bits.dbregion2_marker, (UINT32)dbregionctl1->Bits.dbregion3_marker));
                  MemRegWrite(DPLVUG, ch, 0, Register, Value, 0xF);

                  //
                  // DB Regions
                  //
                  Register.Offset = DBREGIONCTL2_DPLVUG_REG;
                  Value = MemRegRead (DPLVUG, ch, 0, Register);
                  dbregionctl2 = (DBREGIONCTL2_DPLVUG_STRUCT *) &Value;
                  dbregionctl2->Bits.dbregion4_marker = ((MrcData->HalfClkPi * 4) / 8) - 1;
                  dbregionctl2->Bits.dbregion5_marker = ((MrcData->HalfClkPi * 5) / 8) - 1;
                  dbregionctl2->Bits.dbregion6_marker = ((MrcData->HalfClkPi * 6) / 8) - 1;
                  dbregionctl2->Bits.dbregion7_marker = ((MrcData->HalfClkPi * 7) / 8) - 1;
                  if (ch==0 && dq==0)
                    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "db4:0x%x, db5:0x%x, db6:0x%x, db7:0x%x\r\n", (UINT32)dbregionctl2->Bits.dbregion4_marker, (UINT32)dbregionctl2->Bits.dbregion5_marker, (UINT32)dbregionctl2->Bits.dbregion6_marker, (UINT32)dbregionctl2->Bits.dbregion7_marker));
                  MemRegWrite(DPLVUG, ch, 0, Register, Value, 0xF);

                  //
                  // finec
                  //
                  Register.Offset = DLLCOMP_FINECTL_DQCCC_REG;
                  Value = MemRegRead (DQCCC, ch, dq, Register);
                  finectl = (DLLCOMP_FINECTL_DQCCC_STRUCT *) &Value;
                  finectl->Bits.finec_mode = 0;
                  if (ch==0 && dq==0)
                    MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "setting finec=0\r\n"));
                  MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

                  //
                  // Error Status Bits
                  //
                  //Value = ReadReg(DLLCOMP_ERROR_STATUS_DQCCC_REG, ch, dq);
                  //dllcomp_error_status = (DLLCOMP_ERROR_STATUS_DQCCC_STRUCT*) &Value;
                  //MmrcMaxPiDllDebugPrint ((MMRC_DBG_MIN, "ERROR_STATUS: 0x%x\r\n", (UINT32) Value));
                }
              }
            }
          }
        }
        //
        // Read back the current codes after programing the Final Values.
        // This is expecially important during the "restore" flow since the
        // MUX_MIN/MUX_MAX dynamic variable are initialized with the reset
        // values during the MAXPI_INIT state. These variables must have the
        // current codes before DoDllInit() is called.
        //
        MmrcExecuteTask(MrcData, maxpi_get_codes, NO_PRINT, ch);
      }
      PrintLimits(MrcData, 0);
      DoDllInit(MrcData, &MaxPiInfo);
      for(ch=0; ch< MAX_CHANNELS; ch++)
      {
        MmrcExecuteTask (MrcData, maxpi_override_coarse_final, NO_PRINT, ch);
        if (!RestorePath)
        {
          //Not the Restore Path
          //        MmrcExecuteTask (MrcData, maxpi_cmdctl_rescale, NO_PRINT, ch);
          MmrcExecuteTask (MrcData, maxpi_read_value, NO_PRINT, ch);
          MaxPiOldValue[0] = (UINT32) MrcData->DynamicVars[ch][MAXPI_GRP5];
          MaxPiOldValue[1] = (UINT32) MrcData->DynamicVars[ch][MAXPI_GRP4];
          MaxPiOldValue[2] = (UINT32) MrcData->DynamicVars[ch][MAXPI_GRP3];
          MaxPiOldLimit = HalfClk [ADdll [MrcData->DigitalDll]][MrcData->Channel[ch].CurrentFrequency];
          MaxPiNewLimit = MrcData->HalfClkPi;
          for (grp=0; grp<3; grp++) {
            MaxPiNewValue[grp] = (MaxPiOldValue[grp] * MaxPiNewLimit) / MaxPiOldLimit;
          }
          MrcData->DynamicVars[ch][MAXPI_GRP5] = MaxPiNewValue[0];
          MrcData->DynamicVars[ch][MAXPI_GRP4] = MaxPiNewValue[1];
          MrcData->DynamicVars[ch][MAXPI_GRP3] = MaxPiNewValue[2];
          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Ch %d:ctl %d -> %d\n", ch, MaxPiOldValue[0], MaxPiNewValue[0]));
          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Ch %d:clk %d -> %d\n", ch, MaxPiOldValue[1], MaxPiNewValue[1]));
          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Ch %d:cmd %d -> %d\n", ch, MaxPiOldValue[2], MaxPiNewValue[2]));
          MmrcExecuteTask (MrcData, maxpi_write_value, NO_PRINT, ch);
        }
        Register.Offset = PLLCTL_DPIC_REG;
        Value = MemRegRead (DPIC, ch, 0, Register);
        pllctl = (PLLCTL_DPIC_STRUCT*) &Value;
        MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "Ch %d:fbclktrim:0x%x\n", ch, (UINT32)pllctl->Bits.pll_cmn_fbclktrim));
        for (dq=0; dq < MrcData->Channel[ch].MaxDq[0]; dq++)
        {
          if (!IsStrobeValid (MrcData, ch, 0, dq))
            continue;
          //
          //  Getting current fine/coarse codes
          //

          Register.Offset = DLLCOMP_CODE_STATUS_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          dllcmpcode = (DLLCOMP_CODE_STATUS_DQCCC_STRUCT*) &Value;
          local_coarsecode = (UINT32) dllcmpcode->Bits.txdll_stat_coarsecode;
          local_finecode = (UINT32) dllcmpcode->Bits.txdll_stat_finecode;

          //
          //  Setting Override values
          //
          Register.Offset = DLLCOMP_RXCOARSEOVR_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          rxcoarseovr = (DLLCOMP_RXCOARSEOVR_DQCCC_STRUCT*) &Value;
          rxcoarseovr->Bits.rxdll_coarsecode_ovr=local_coarsecode;
          MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

          Register.Offset = DLLCOMP_RXFINEOVR_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          rxfineovr = (DLLCOMP_RXFINEOVR_DQCCC_STRUCT*) &Value;
          rxfineovr->Bits.rxdll_finecode_ovr=local_finecode;
          MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);
          //
          //  Enabling Overrides
          //
          Register.Offset = DLLCOMP_RXCOARSEOVR_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          rxcoarseovr = (DLLCOMP_RXCOARSEOVR_DQCCC_STRUCT*) &Value;
          rxcoarseovr->Bits.rxdll_coarsecode_ovr_sel=1;
          MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);


          Register.Offset = DLLCOMP_RXFINEOVR_DQCCC_REG;
          Value = MemRegRead (DQCCC, ch, dq, Register);
          rxfineovr = (DLLCOMP_RXFINEOVR_DQCCC_STRUCT*) &Value;
          rxfineovr->Bits.rxdll_finecode_ovr_sel=1;
          rxfineovr->Bits.rxdll_finecode_updateovr_ovr=1;
          MemRegWrite(DQCCC, ch, dq, Register, Value, 0xF);

          MmrcMaxPiDllDebugPrint((MMRC_DBG_MIN, "ch:%d dq:%d fc=%d cc=%d\r\n", (UINT32)ch, dq, local_finecode, local_coarsecode));
        }
      }

      if (MaxPiInfo.txdll_error)
      {
        MmrcErrorPrint ((MMRC_DBG_MIN, "DEADLOOP: MAXPI_FINAL_VALUES did not work!\r\n"));
        MrcDeadLoop();
        return MMRC_FAILURE;
      }
      //FineContinuousLoop(MrcData);
      complete=1;

      break;
    case MAXPI_ERROR:
      MmrcErrorPrint ((MMRC_DBG_MIN, "DEADLOOP: MAXPI Failed to find a good working Combo!\r\n"));
      MrcDeadLoop();
      return MMRC_FAILURE;
    default:
      break;
    }
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
  FloorPlanByteMapping (
    MMRC_DATA         *MrcData)
{
  UINT8 st;
  UINT8 Channel;

  for (Channel = 0; Channel < MAX_CHANNELS; Channel++) {
    if (!MrcData->Channel[Channel].Enabled) { continue; }
    FillChannelBitMapping (MrcData,0,0,Channel);
    CalculateDeswizzleReverseMap(MrcData, Channel);

    for (st = 0; st < MrcData->Channel[Channel].MaxDq[0]; st++) {
      MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][st].Channel = FloorPlanGenerated[MrcData->CurrentBlueprint][Channel][st].Channel;
      MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][st].Strobelane = MrcData->Channel[Channel].Dimm2DunitMapping[FloorPlanGenerated[MrcData->CurrentBlueprint][Channel][st].Strobelane*8]/8;
    }
  }

  return MMRC_SUCCESS;
}

MMRC_STATUS
FillChannelBitMapping(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
  Mmrcmemcpy( MrcData->Channel[Channel].Dimm2DunitMapping, &(MrcData->DramPolicyData.ChSwizzle[Channel]), sizeof(MrcData->Channel[Channel].Dimm2DunitMapping));

       //o  CA[8:5, 3:0] tCK rise to the  DRAM => DQCALRDAT[14, 12, 10, 8, 6, 4, 2, 0]
   // CA[8:5, 3:0] tCK fall  to the  DRAM => DQCALRDAT[15, 13, 11, 9, 7, 5, 3, 1]
/*    for (i = 0; i < 8; i++) {
      data8 = (( (EarlyCommandPatternsRise[0]& 0x1E0) >>5 ) << 4) | (EarlyCommandPatternsRise[0] 0xF);
    } */


#if 0 //#ifndef SIM
  UINT8 *Dimm2DunitMapping = (UINT8 *) MrcData->Channel[Channel].Dimm2DunitMapping;
//  UINT8 Bit;
  if (Channel == 0)
  {
      Dimm2DunitMapping[0] = 5;
      Dimm2DunitMapping[1] = 4;
      Dimm2DunitMapping[2] = 6;
      Dimm2DunitMapping[3] = 7;
      Dimm2DunitMapping[4] = 3;
      Dimm2DunitMapping[5] = 2;
      Dimm2DunitMapping[6] = 0;
      Dimm2DunitMapping[7] = 1;

      Dimm2DunitMapping[8]  = 3;
      Dimm2DunitMapping[9]  = 2;
      Dimm2DunitMapping[10] = 1;
      Dimm2DunitMapping[11] = 0;
      Dimm2DunitMapping[12] = 4;
      Dimm2DunitMapping[13] = 7;
      Dimm2DunitMapping[14] = 5;
      Dimm2DunitMapping[15] = 6;

  }
  else if (Channel == 1)
  {
      Dimm2DunitMapping[0] = 1;
      Dimm2DunitMapping[1] = 0;
      Dimm2DunitMapping[2] = 3;
      Dimm2DunitMapping[3] = 2;
      Dimm2DunitMapping[4] = 4;
      Dimm2DunitMapping[5] = 5;
      Dimm2DunitMapping[6] = 7;
      Dimm2DunitMapping[7] = 6;

      //DQ15_c DQ13_c DQ14_c DQ12_c DQ10_c DQ11_c DQ8_c DQ9_c
      Dimm2DunitMapping[8] = 7;
      Dimm2DunitMapping[9] = 5;
      Dimm2DunitMapping[10] = 6;
      Dimm2DunitMapping[11] = 4;
      Dimm2DunitMapping[12] = 2;
      Dimm2DunitMapping[13] = 3;
      Dimm2DunitMapping[14] = 0;
      Dimm2DunitMapping[15] = 1;
  }
  else if (Channel == 2)
  {
      Dimm2DunitMapping[0] = 5;
      Dimm2DunitMapping[1] = 6;
      Dimm2DunitMapping[2] = 7;
      Dimm2DunitMapping[3] = 4;
      Dimm2DunitMapping[4] = 3;
      Dimm2DunitMapping[5] = 0;
      Dimm2DunitMapping[6] = 1;
      Dimm2DunitMapping[7] = 2;

      Dimm2DunitMapping[8] = 2;
      Dimm2DunitMapping[9] = 1;
      Dimm2DunitMapping[10] = 0;
      Dimm2DunitMapping[11] = 3;
      Dimm2DunitMapping[12] = 4;
      Dimm2DunitMapping[13] = 7;
      Dimm2DunitMapping[14] = 6;
      Dimm2DunitMapping[15] = 5;
  }
  else if (Channel == 3)
  {
      Dimm2DunitMapping[0] = 0;
      Dimm2DunitMapping[1] = 1;
      Dimm2DunitMapping[2] = 2;
      Dimm2DunitMapping[3] = 3;
      Dimm2DunitMapping[4] = 4;
      Dimm2DunitMapping[5] = 5;
      Dimm2DunitMapping[6] = 7;
      Dimm2DunitMapping[7] = 6;

      Dimm2DunitMapping[8] = 7;
      Dimm2DunitMapping[9] = 5;
      Dimm2DunitMapping[10] = 6;
      Dimm2DunitMapping[11] = 4;
      Dimm2DunitMapping[12] = 3;
      Dimm2DunitMapping[13] = 2;
      Dimm2DunitMapping[14] = 1;
      Dimm2DunitMapping[15] = 0;
  }

  /*
  if (Channel == 0)
  {
      Dimm2DunitMapping[0] = 6;
      Dimm2DunitMapping[1] = 7;
      Dimm2DunitMapping[2] = 5;
      Dimm2DunitMapping[3] = 4;
      Dimm2DunitMapping[4] = 1;
      Dimm2DunitMapping[5] = 0;
      Dimm2DunitMapping[6] = 2;
      Dimm2DunitMapping[7] = 3;

      Dimm2DunitMapping[8] = 5;
      Dimm2DunitMapping[9] = 4;
      Dimm2DunitMapping[10] = 6;
      Dimm2DunitMapping[11] = 7;
      Dimm2DunitMapping[12] = 3;
      Dimm2DunitMapping[13] = 2;
      Dimm2DunitMapping[14] = 0;
      Dimm2DunitMapping[15] = 1;
  }
  else if (Channel == 1)
  {
      Dimm2DunitMapping[0] = 1;
      Dimm2DunitMapping[1] = 0;
      Dimm2DunitMapping[2] = 3;
      Dimm2DunitMapping[3] = 2;
      Dimm2DunitMapping[4] = 4;
      Dimm2DunitMapping[5] = 5;
      Dimm2DunitMapping[6] = 7;
      Dimm2DunitMapping[7] = 6;

      Dimm2DunitMapping[8] = 5;
      Dimm2DunitMapping[9] = 6;
      Dimm2DunitMapping[10] = 7;
      Dimm2DunitMapping[11] = 4;
      Dimm2DunitMapping[12] = 3;
      Dimm2DunitMapping[13] = 0;
      Dimm2DunitMapping[14] = 1;
      Dimm2DunitMapping[15] = 2;
  }
  else if (Channel == 2)
  {
      Dimm2DunitMapping[0] = 5;
      Dimm2DunitMapping[1] = 6;
      Dimm2DunitMapping[2] = 7;
      Dimm2DunitMapping[3] = 4;
      Dimm2DunitMapping[4] = 3;
      Dimm2DunitMapping[5] = 0;
      Dimm2DunitMapping[6] = 1;
      Dimm2DunitMapping[7] = 2;

      Dimm2DunitMapping[8] = 7;
      Dimm2DunitMapping[9] = 5;
      Dimm2DunitMapping[10] = 6;
      Dimm2DunitMapping[11] = 4;
      Dimm2DunitMapping[12] = 2;
      Dimm2DunitMapping[13] = 3;
      Dimm2DunitMapping[14] = 0;
      Dimm2DunitMapping[15] = 1;
  }
  else if (Channel == 3)
  {
      Dimm2DunitMapping[0] = 0;
      Dimm2DunitMapping[1] = 1;
      Dimm2DunitMapping[2] = 2;
      Dimm2DunitMapping[3] = 3;
      Dimm2DunitMapping[4] = 4;
      Dimm2DunitMapping[5] = 5;
      Dimm2DunitMapping[6] = 7;
      Dimm2DunitMapping[7] = 6;

      Dimm2DunitMapping[8] = 7;
      Dimm2DunitMapping[9] = 5;
      Dimm2DunitMapping[10] = 6;
      Dimm2DunitMapping[11] = 4;
      Dimm2DunitMapping[12] = 3;
      Dimm2DunitMapping[13] = 2;
      Dimm2DunitMapping[14] = 1;
      Dimm2DunitMapping[15] = 0;
  }
  */
  /*
  if (Channel == 0)
  {
    Dimm2DunitMapping[0] = 3;
    Dimm2DunitMapping[1] = 2;
    Dimm2DunitMapping[2] = 1;
    Dimm2DunitMapping[3] = 0;
    Dimm2DunitMapping[4] = 4;
    Dimm2DunitMapping[5] = 7;
    Dimm2DunitMapping[6] = 5;
    Dimm2DunitMapping[7] = 6;

    Dimm2DunitMapping[8]  = 5;
    Dimm2DunitMapping[9]  = 4;
    Dimm2DunitMapping[10] = 6;
    Dimm2DunitMapping[11] = 7;
    Dimm2DunitMapping[12] = 3;
    Dimm2DunitMapping[13] = 2;
    Dimm2DunitMapping[14] = 0;
    Dimm2DunitMapping[15] = 1;
  }
  else if (Channel == 1)
  {
    Dimm2DunitMapping[0] = 2;
    Dimm2DunitMapping[1] = 1;
    Dimm2DunitMapping[2] = 0;
    Dimm2DunitMapping[3] = 3;
    Dimm2DunitMapping[4] = 4;
    Dimm2DunitMapping[5] = 7;
    Dimm2DunitMapping[6] = 6;
    Dimm2DunitMapping[7] = 5;

    Dimm2DunitMapping[8]  = 5;
    Dimm2DunitMapping[9]  = 6;
    Dimm2DunitMapping[10] = 7;
    Dimm2DunitMapping[11] = 4;
    Dimm2DunitMapping[12] = 3;
    Dimm2DunitMapping[13] = 0;
    Dimm2DunitMapping[14] = 1;
    Dimm2DunitMapping[15] = 2;
  }
  else if (Channel == 2)
  {
    Dimm2DunitMapping[0] = 1;
    Dimm2DunitMapping[1] = 0;
    Dimm2DunitMapping[2] = 3;
    Dimm2DunitMapping[3] = 2;
    Dimm2DunitMapping[4] = 4;
    Dimm2DunitMapping[5] = 5;
    Dimm2DunitMapping[6] = 7;
    Dimm2DunitMapping[7] = 6;

    Dimm2DunitMapping[8]  = 7;
    Dimm2DunitMapping[9] = 5;
    Dimm2DunitMapping[10] = 6;
    Dimm2DunitMapping[11] = 4;
    Dimm2DunitMapping[12] = 2;
    Dimm2DunitMapping[13] = 3;
    Dimm2DunitMapping[14] = 0;
    Dimm2DunitMapping[15] = 1;
  }
  else if (Channel == 3)
  {
    Dimm2DunitMapping[0] = 0;
    Dimm2DunitMapping[1] = 1;
    Dimm2DunitMapping[2] = 2;
    Dimm2DunitMapping[3] = 3;
    Dimm2DunitMapping[4] = 4;
    Dimm2DunitMapping[5] = 5;
    Dimm2DunitMapping[6] = 7;
    Dimm2DunitMapping[7] = 6;

    Dimm2DunitMapping[8]  = 7;
    Dimm2DunitMapping[9]  = 5;
    Dimm2DunitMapping[10] = 6;
    Dimm2DunitMapping[11] = 4;
    Dimm2DunitMapping[12] = 3;
    Dimm2DunitMapping[13] = 2;
    Dimm2DunitMapping[14] = 1;
    Dimm2DunitMapping[15] = 0;
  }
  */
#endif
  return MMRC_SUCCESS;
}

#if SIM || JTAG
MMRC_STATUS
DumpAllDDRIORegisters(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
    DumpDunitRegisters(MrcData, 0);
    DumpDdrioRegisters(MrcData, 0);
    DumpDunitRegisters(MrcData, 1);
    DumpDdrioRegisters(MrcData, 1);
    DumpDunitRegisters(MrcData, 2);
    DumpDdrioRegisters(MrcData, 2);
    DumpDunitRegisters(MrcData, 3);
    DumpDdrioRegisters(MrcData, 3);
    return MMRC_SUCCESS;
}

MMRC_STATUS
DumpDdrioRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
)
{
  UINT16 Offset;
  UINT32 Value;
  REGISTER_ACCESS Register;
  UINT8 MaxDQCCCInstances=2, Inst;

  Register.Mask   = 0xFFFFFFFF;

  //if (MrcData->Channel[Channel].CurrentDdrType = TypeLpDdr3)
  //{
  for (Inst=0; Inst < MaxDQCCCInstances; Inst++)
  {
    MmrcDebugPrint ((MMRC_DBG_MIN, "DQCCC CH%d_%d\n", Channel, Inst));

    for (Offset = 0; Offset <= 0x10C; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DQCCC, Channel, Inst, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    for (Offset = 0x600; Offset <= 0x640; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DQCCC, Channel, Inst, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    for (Offset = 0x700; Offset <= 0x724; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DQCCC, Channel, Inst, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
  }

    MmrcDebugPrint ((MMRC_DBG_MIN, "ADPLL CH%d\n", Channel));

    for (Offset = 0; Offset <= 0x34; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (ADPLL, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }

    MmrcDebugPrint ((MMRC_DBG_MIN, "DPIC CH%d\n", Channel));
    for (Offset = 0; Offset <= 0x140; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    //for (Offset = 0xC; Offset <= 0x30; Offset += 4) {
     // Register.Offset = Offset;
     // Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
     // MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    //}
   // for (Offset = 0x38; Offset <= 0x140; Offset += 4) {
     // Register.Offset = Offset;
     // Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
     // MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    //}
    for (Offset = 0x300; Offset <= 0x328; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    for (Offset = 0x500; Offset <= 0x560; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    for (Offset = 0x700; Offset <= 0x714; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPIC, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }

    MmrcDebugPrint ((MMRC_DBG_MIN, "DPLVUG CH%d\n", Channel));
    for (Offset = 0; Offset <= 0xFC; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPLVUG, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
    for (Offset = 0x100; Offset <= 0x124; Offset += 4) {
      Register.Offset = Offset;
      Value = (UINT32) MemRegRead (DPLVUG, Channel, 0, Register);
      MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
    }
  //}
  MmrcDebugPrint ((MMRC_DBG_MIN, "LDO CH%d\n", Channel));

  for (Offset = 0; Offset <= 0x24; Offset += 4) {
    Register.Offset = Offset;
    Value = (UINT32) MemRegRead (LDO, Channel, 0, Register);
    MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
  }

  return MMRC_SUCCESS;
}


MMRC_STATUS
OnlyKeep10 (
  IN  OUT MMRC_DATA         *MrcData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
)
{
  MrcData->Channel[8].Enabled  = FALSE;
  MrcData->Channel[9].Enabled  = FALSE;
  MrcData->Channel[10].Enabled = TRUE;
  MrcData->Channel[11].Enabled = FALSE;

  return MMRC_SUCCESS;
}

MMRC_STATUS
ChangeTxToFailSimTest (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT32 Results;

  Results = 0;
  GetSetDdrioGroup2 (MrcData, Channel, 0, 0, RxDqsDelay, CMD_SET_VAL_FC_UC, &Results);
#if SIM || JTAG
  isb32Write(0x1c, 0x8ac, 0x1, Results, 0xf);
  isb32Write(0x1c, 0xac, 0x1, Results, 0xf);
#endif
  return MMRC_SUCCESS;
}

#endif

MMRC_STATUS
CompleteJedecInit (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channelx
  )
{
  UINT8  Rank;
  UINT32 Value;
  UINT8 ch, rk=0;
  //
  //  There is only one Reset Pin in A0 that controls all channels.
  //  So we are setting the reset on all Bytelanes just incase this moves in the future.
  //
  //MmrcDebugPrint ((MMRC_DBG_MIN, "DRAM Resetting %d\n", Channel));
#ifdef SIM
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    MmrcExecuteTask(MrcData, modmem_init_dramreset_seq_sim, NO_PRINT, ch);
  }
#else
    //MmrcExecuteTask(MrcData, modmem_init_dramreset_seq, NO_PRINT, ch);
  DramPhysicalReset(MrcData, NO_PRINT, NO_PRINT, 0);
#endif
  if (MrcData->Channel[0].DramType == TypeLpDdr4) {
    HWSetBootFrequency(MrcData, NO_PRINT, NO_PRINT, 0);
  }
  //
  //  Since we have to reset all the channels, we need to restore all channels to working state.
  //
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    if (RunOnThisChannel (MrcData, ch, rk))
    {
      SWSetHiFrequency(MrcData, NO_PRINT, NO_PRINT, ch);
      if (MrcData->Channel[ch].DramType == TypeLpDdr4) {
        //MmrcDebugPrint ((MMRC_DBG_MIN, "ODT_VREF\n"));
        LP4MRProgramming_ODTVREF(MrcData, NO_PRINT, NO_PRINT, ch);
      }
      //MmrcDebugPrint ((MMRC_DBG_MIN, "DRAM Basic MR Programming\n"));
      InitializeJedec (MrcData, 0, NO_PRINT, ch);
      //MmrcDebugPrint ((MMRC_DBG_MIN, "DRAM All MR Programming\n"));
      LP4AllMRProgramming(MrcData, NO_PRINT,  NO_PRINT, ch);
      //MmrcDebugPrint ((MMRC_DBG_MIN, "DRAM ZQCAL\n"));
      LP4MRProgramming_MPCZQ(MrcData, NO_PRINT,  NO_PRINT, ch);

      for (Rank=0; Rank < MAX_RANKS; Rank++)
      {
        if (MrcData->Channel[ch].RankEnabled[Rank])
        {
          GetSetTxCaVref (MrcData, 0, ch, 0, Rank, 0, 0, 0, 0, CMD_GET_REG, &Value);
          GetSetTxCaVref (MrcData, 0, ch, 0, Rank, 0, 0, 0, 0, FC_WR, &Value);
          //MmrcDebugPrint ((MMRC_DBG_MIN, "CA VREF %d %d = %d\n", ch, Rank, Value));
          GetSetTxVref (MrcData, 0, ch, 0, Rank, 0, 0, 0, 0, CMD_GET_REG, &Value);
          GetSetTxVref (MrcData, 0, ch, 0, Rank, 0, 0, 0, 0, FC_WR, &Value);
          //MmrcDebugPrint ((MMRC_DBG_MIN, "DQ VREF %d %d = %d\n", ch, Rank, Value));
        }
      }
    }
  }
  if (MrcData->Channel[0].DramType == TypeLpDdr4) {
    MrcData->DefaultCmdClkCtl=FALSE;
    HWSetHiFrequency(MrcData, NO_PRINT, NO_PRINT, 0);
    MrcData->DefaultCmdClkCtl=TRUE;
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
DramPhysicalReset (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT8 origEnable;
  UINT8 ch;

  //Put DDRIO in reset
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    origEnable = MrcData->Channel[ch].Enabled;
    MrcData->Channel[ch].Enabled = TRUE;
    MmrcExecuteTask(MrcData, modmem_init_dramreset_seq_a, NO_PRINT, ch);
    MrcData->Channel[ch].Enabled = origEnable;
  }
  //Change the CKE (DUNIT)
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    MmrcExecuteTask(MrcData, modmem_init_dramreset_seq_b, NO_PRINT, ch);
  }
  //tINIT2=200us
  MrcDelay (MrcData, MICRO_DEL, 200);
  //Bring DDRIO from Reset
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    origEnable = MrcData->Channel[ch].Enabled;
    MrcData->Channel[ch].Enabled = TRUE;
    MmrcExecuteTask(MrcData, modmem_init_dramreset_seq_c, NO_PRINT, ch);
    MrcData->Channel[ch].Enabled = origEnable;
  }
  //tINIT3=2ms
  MrcDelay (MrcData, MILLI_DEL, 2);
  // Bring up CKE once clock is stable (DUNIT)
  for (ch=0; ch < MAX_CHANNELS;ch++)
  {
    MmrcExecuteTask(MrcData, modmem_init_dramreset_seq_d, NO_PRINT, ch);
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
SimMemTest (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
#if SIM
  UINT8 Rank;
  UINT32 BitErrorStatus0, BitErrorStatus1;
  UINT16 ByteErrorStatus0, ByteErrorStatus1;
  UINT32 ErrorCount[9];
  UINT32 temp1, temp2, Results;
#if !SIM && !JTAG
  UINT32 Address;
#endif

  L_CpgcEnable(MrcData, CapsuleStartIndex, StringIndex, Channel);
  //
  // 2^N where N  is number of bursts, (I.E. 2^0 = 1, 2^1=2, etc.)
  // 2^(N-1),for loop count.
  //
  MrcData->CpgcOptions.NumBursts = 0;
  MrcData->CpgcOptions.LoopCount = 1;
  MrcData->CpgcOptions.CmdSequence = CPGC_CMDSEQ_WRITE_READ;
  //
  // Call the setup routine and etup CPGC engine to do a simple write-read access with no VA patterns.
  //
  MySimStall(500);
  PrechargeAll(MrcData, Channel, 0);
  PerformDQReset(MrcData, Channel, 0);
  PerformFifoReset(MrcData, Channel, 0);

  L_CpgcSetup (MrcData, Channel, CPGC_CONFIG_EARLYTRAINING) ;

  for (Rank=0; Rank < MAX_RANKS; Rank++) {
    if (!RunOnThisChannel (MrcData, Channel, Rank))
      continue;
    //
    // Setup basic pattern to test.
    // Note, that pattern0 is used for general patterns, pattern0/1 are used for VA Testing.
    //
    MrcData->CpgcOptions.Pattern0 = 0xaaaaaaaa;
    MrcData->CpgcOptions.Pattern1 = 0x55555555;
    MrcData->CpgcOptions.Rank     = Rank;
    //
    // Setup the CPGC engine to use LFSR, note TRUE-> LFSR, FALSE->DPAT.
    //
    MrcData->CpgcOptions.DPAT_LFSR = TRUE;
    //
    // Reconfigure the CPGC engine with the above parameters.
    //
    L_CpgcReconfig (MrcData, Channel);
    //
    // Run the CPGC Engine
    //
    PerformFifoReset(MrcData, Channel, 0);
    if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
      MrcData->DynamicVars[Channel][RDDCMD] = 0;
      MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
      MrcData->DynamicVars[Channel][REG_DATA] = ACTIVATE_LP3;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    }
    if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
      MrcData->DynamicVars[Channel][RDDCMD] = 0;
      MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;
      MrcData->DynamicVars[Channel][REG_DATA] = ACTIVATE_LP4;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    }
    L_CpgcRunTest (MrcData, Channel, &ByteErrorStatus0, &BitErrorStatus0, (UINT32*) NULL, ErrorCount);

        Results = 0;
#if SIM || JTAG
    temp1 = isb32Read(0x1c, 0xac, 0);
    temp2 = isb32Read(0x1c, 0x8ac, 0);
    isb32Write(0x1c, 0x8ac, 0x1, Results, 0xf);
    isb32Write(0x1c, 0xac, 0x1, Results, 0xf);

#else
    Address = 0xD00000ac |     //assume same value for all channels
    ((InstancePortOffset[InstancePortMap[DQCCC].InstanceStartIndex + 0].Port << 16) + InstancePortOffset[InstancePortMap[DQCCC].InstanceStartIndex + 0].Offset);
    temp1 = * ( (volatile UINT32 *) Address);
    * ( (volatile UINT32 *) Address) = 0x0;
    Address = Address | 0x800;
    temp2 = * ( (volatile UINT32 *) Address);
    * ( (volatile UINT32 *) Address) = 0x0;
#endif

    PerformFifoReset(MrcData, Channel, 0);
    L_CpgcRunTest (MrcData, Channel, &ByteErrorStatus1, &BitErrorStatus1, (UINT32*) NULL, ErrorCount);
#if SIM || JTAG
    isb32Write(0x1c, 0xac, 0x1, temp1, 0xf);
    isb32Write(0x1c, 0x8ac, 0x1, temp2, 0xf);
#else
    Address = 0xD00000ac |     //assume same value for all channels
    ((InstancePortOffset[InstancePortMap[DQCCC].InstanceStartIndex + 0].Port << 16) + InstancePortOffset[InstancePortMap[DQCCC].InstanceStartIndex + 0].Offset);

    * ( (volatile UINT32 *) Address) = temp1;
    Address = Address | 0x800;
    * ( (volatile UINT32 *) Address) = temp2;
#endif
    PerformFifoReset(MrcData, Channel, 0);
    L_CpgcRunTest (MrcData, Channel, &ByteErrorStatus0, &BitErrorStatus0, (UINT32*) NULL, ErrorCount);
    if (ByteErrorStatus0==0 && ByteErrorStatus1!=0) {
      MmrcDebugPrint ((MMRC_DBG_MIN, "SimMemTest: Pass\n"));
    } else {
      MmrcDebugPrint ((MMRC_DBG_MIN, "SimMemTest: Fail\n"));
    }
  } // for Rank

#if 0
  //Results = 0x0;
  //isb32Write(0x1c, 0xac, 0x1, Results, 0xf);
    //GetSetDataSignal2 (MrcData, Channel, 0, 0, TX_DQ_DELAY, CMD_SET_VAL_FC_UC, &Results);
    //GetSetDataSignal2 (MrcData, Channel, 0, 1, TX_DQ_DELAY, CMD_SET_VAL_FC_UC, &Results);
#if 1
  //GetSetDataSignal2 (MrcData, Channel, 0, 0, TX_DQS_DELAY, CMD_SET_VAL_FC_UC, &Results);
  //GetSetDataSignal2 (MrcData, Channel, 0, 1, TX_DQS_DELAY, CMD_SET_VAL_FC_UC, &Results);
    //Results = 0;
    //isb32Write(0x1c, 0x78, 0x1, Results, 0xf);
    //isb32Write(0x1c, 0x8ac, 0x1, Results, 0xf);
    //isb32Write(0x1c, 0xac, 0x1, Results, 0xf);
    //Results = isb32Read(0x1c, 0x8ac);
#endif
     MrcData->DynamicVars[Channel][NUM_BURSTS_EXP] = 1;
   MrcData->DynamicVars[Channel][NUM_LOOPS_EXP]  = 1;
   MmrcExecuteTask (MrcData, CPGC_RESETUP_WRITE_READ, NO_PRINT, Channel);

  PerformDQReset(MrcData, Channel, 0);
  PerformFifoReset(MrcData, Channel, 0);
  //
  // Issue an activate
  //
#if LPDDR4_SUPPORT
  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    MrcData->DynamicVars[Channel][RDDCMD] = 0;
    MrcData->DynamicVars[Channel][REG_DATA] = ACTIVATE_LP4;
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  }
#endif
#if LPDDR3_SUPPORT
  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    MrcData->DynamicVars[Channel][RDDCMD] = 0;
    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][REG_DATA] = ACTIVATE_LP3;
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  }
#endif

  MmrcExecuteTask(MrcData, CPGC_ENABLE, NO_PRINT, Channel);
  MmrcExecuteTask(MrcData, CPGC_RUN_TEST, NO_PRINT, Channel);
  Results = (UINT32)  MrcData->DynamicVars[Channel][CPGC_RESULT];

  #if LPDDR4_SUPPORT
  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    MrcData->DynamicVars[Channel][RDDCMD] = 0;
    MrcData->DynamicVars[Channel][REG_DATA] = PREA_LP4;
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  }
#endif
#if LPDDR3_SUPPORT
  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    MrcData->DynamicVars[Channel][RDDCMD] = 0;
    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][REG_DATA] = PREA_LP3;
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  }
#endif

  MmrcDebugPrint ((MMRC_DBG_MIN, "SimMemTest: Results = 0x%02x\n", Results));
#endif
#endif  //if 0
  return MMRC_SUCCESS;
}

MMRC_STATUS
CpgcVATest (
  MMRC_DATA         *ModMrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
  UINT32 BitErrorStatus0, BitErrorStatus1;
  UINT16 ByteErrorStatus0;
  UINT32 ErrorCount[9];
  UINT8   Rank;
  UINT16  CompareValue=0;
  INT32   i;
  UINT8   VA_LOOP_COUNT;
  UINT8   VA_NUM_BURSTS;
#define IS_SIM ((1 << ModMrcData->Channel[Channel].CurrentPlatform) == P_SIM)
   MmrcDebugPrint((MMRC_DBG_MAX, "\n\nCPGC Results:\n\n"));

  PRINT_FUNCTION_INFO;
#if SIM
    VA_LOOP_COUNT=1;
    VA_NUM_BURSTS=3;
#else
    VA_LOOP_COUNT=11; //change to 14 for max stress
    VA_NUM_BURSTS=12; //change to 14 for richer pattern generation
#endif

  ModMrcData->CpgcOptions.LoopCount = VA_LOOP_COUNT;
  ModMrcData->CpgcOptions.NumBursts = VA_NUM_BURSTS;
  L_CpgcSetup( ModMrcData, Channel, CPGC_CONFIG_VA );


  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    //
    // Only perform WRLevel training on the ranks that are enabled.
    //
    if (ModMrcData->Channel[Channel].RankEnabled[Rank]) {
      ModMrcData->CpgcOptions.Rank = Rank;
      L_CpgcReconfig( ModMrcData, Channel );

      for (i=0; i< 1; i++)
      {
        //PrechargeAll (ModMrcData, Channel, Rank);  // Ron added this.  This will add greater determinism because we have refresh on, and so precharge
        PerformFifoReset(ModMrcData, Channel, Rank);
        //
        // Perform the required memory access.
        //
        L_CpgcRunTest (ModMrcData, Channel, &ByteErrorStatus0, &BitErrorStatus0, &BitErrorStatus1, ErrorCount);
        MmrcDebugPrint((MMRC_DBG_MIN, "Ch%d Rk%d Bits: %08X %08X  ", Channel, Rank, BitErrorStatus1, BitErrorStatus0));
        //MemDebugPrint((SDBG_MAX, "Chnk: %02X  \n", MrcData->DynamicVars[Channel][CHUNK_STAT]));
        CompareValue |= ByteErrorStatus0;
      }
    } // MrcData->Channel[Channel].RankEnabled[Rank]
  } // Rank loop ...

  MmrcDebugPrint ((MMRC_DBG_TRACE, "VA Results = 0x%02x\n", CompareValue));
  if (CompareValue) {
    MmrcErrorPrint((MMRC_DBG_MIN, "<!> CPGC Failure - cannot continue!\n"));
    MrcDeadLoop();
  }
  return MMRC_SUCCESS;

}

MMRC_STATUS
SetupMRCFreqA (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT8 TimingParamVar;
  TimingParamVar = (MrcData->DVFS_FreqA - 1);
  MrcData->Channel[Channel].CurrentFrequency = ConfigToSpeed[TimingParamHash (TimingParamVar, TRUE)];
  MrcData->CurrentFrequency = MrcData->Channel[Channel].CurrentFrequency;
  MrcData->TimingParamsConfig = MrcData->DVFS_FreqA;
  MrcData->DVFSFreqIndex = 0;

  return MMRC_SUCCESS;
}

MMRC_STATUS
SetupMRCFreqB (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT8 TimingParamVar;
  TimingParamVar = (MrcData->DVFS_FreqB - 1);
  if (MrcData->DVFS_Enable == 1) {
    MrcData->Channel[Channel].CurrentFrequency = ConfigToSpeed[TimingParamHash (TimingParamVar, TRUE)];
    MrcData->TimingParamsConfig = MrcData->DVFS_FreqB;
    MrcData->DVFSFreqIndex = 1;
  }

  return MMRC_SUCCESS;
}

MMRC_STATUS
MrcDelay (
  IN    MMRC_DATA      *MrcData,
  IN        UINT8   Type,
  IN        UINT32  Delay
)
{
  UINT32  TimeCounter;

  switch (Type) {
  case MILLI_DEL:
    for (TimeCounter = 0; TimeCounter < Delay; TimeCounter++) {
      MicroSecondDelay (1000);
    }
    break;
  case MICRO_DEL:
    MicroSecondDelay (Delay);
    break;
  case NANO_DEL:
    McStall(1);
    //McStallNanoSecond (Delay);
    break;
  default:
    MicroSecondDelay (Delay);
    break;
  }
  return MMRC_SUCCESS;
}

#if 0
MMRC_STATUS
VrefOverride (
  IN  OUT MMRC_DATA         *MrcData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
)
/*++

Routine Description:

  Overrides Vref

Arguments:

  MrcData:       Host structure for all data related to MMRC
  Channel:          Current Channel being examined.

Returns:

  None

--*/
{
  if (MrcData->VrefOverrideEnable) {
    //
    // Override Vref with Setup Option
    //
    WriteDimmVref (MrcData, Channel, MrcData->VrefOverrideValue, OVERRIDE);
    MmrcDebugPrint ((MMRC_DBG_MED, "DIMM Vref Override Val: %d\n", MrcData->VrefOverrideValue));
  }

  return MMRC_SUCCESS;
}
#endif

MMRC_STATUS
MrcAcquireBus (
  IN      UINT16    SmbusBase
)
/*++

Routine Description:

  This routine attempts to acquire the SMBus

Arguments:

  None

Returns:

  MMRC_FAILURE as failed
  MMRC_SUCCESS as passed

--*/
{
  UINT8 StsReg;

  StsReg  = 0;
  StsReg  = (UINT8) IoIn8 (SmbusBase + R_PCH_SMBUS_HSTS);
  if (StsReg & B_PCH_SMBUS_IUS) {
    return MMRC_FAILURE;
  } else if (StsReg & B_PCH_SMBUS_HBSY) {
    //
    // Clear Status Register and exit
    //
    // Wait for HSTS.HBSY to be clear
    do {
      StsReg = (UINT8) IoIn8 (SmbusBase + R_PCH_SMBUS_HSTS);
    } while ((StsReg & B_PCH_SMBUS_HBSY) != 0);

    // Clear all status bits
    IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, 0xFE);
    return MMRC_SUCCESS;
  } else {
    //
    // Clear out any odd status information (Will Not Clear In Use)
    //
    IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, StsReg);
    return MMRC_SUCCESS;
  }
}

MMRC_STATUS
MrcSmbusExec (
  IN        UINT16    SmbusBase,
  IN        UINT8     SlvAddr,
  IN        UINT8     Operation,
  IN        UINT8     Offset,
  IN        UINT8     *Length,
  IN        UINT8     *Buffer
)
/*++

Routine Description:

  This routine reads SysCtl registers

Arguments:

  SmbusBase -  SMBUS Base Address
  SlvAddr - Targeted Smbus Slave device address
  Operation - Which SMBus protocol will be used
  Offset - Offset of the register
  Length - Number of bytes
  Buffer - Buffer contains values read from registers

Returns:

  MMRC_SUCCESS as passed
  Others as failed

--*/
{

#if MINIBIOS
  return MMRC_SUCCESS;
#else

  MMRC_STATUS  Status;
  UINT8       SmbusOperation = 0;
  UINT8       BlockCount = 0;
#if !defined SIM  && !defined JTAG && !defined MINIBIOS
  UINT8       AuxcReg;
  UINT8       SlvAddrReg;
  UINT8       HostCmdReg;
  UINT8       Index;
  UINT8       *CallBuffer;
  BOOLEAN     BufferTooSmall;
  UINT8       StsReg;
#endif
  UINT8       RetryCount = BUS_TRIES;

  //
  // MrcSmbusExec supports byte and block read.
  // Only allow Byte or block access
  //
  if (! ((*Length  == VF_SC_BYTE_LEN) || (*Length == VF_SC_BLOCK_LEN) ) ) {
    return MMRC_FAILURE;
  }

#if SIM || JTAG || MINIBIOS

  *Buffer = myReadSPDByte (SmbusBase, SlvAddr, Offset);
  if (Offset == 0x2 && *Buffer == 0) {
    return MMRC_FAILURE;
  } else {
    return MMRC_SUCCESS;
  }

#else
  //
  // See if its ok to use the bus based upon INUSE_STS bit.
  //
  Status = MrcAcquireBus (SmbusBase);
  if (Status == MMRC_FAILURE) {
    return Status;
  }

  CallBuffer = Buffer;

  //
  //SmbStatus Bits of interest
  //[6] = IUS (In Use Status)
  //[4] = FAIL
  //[3] = BERR (Bus Error = transaction collision)
  //[2] = DERR (Device Error = Illegal Command Field, Unclaimed Cycle, Host Device Timeout, CRC Error)
  //[1] = INTR (Successful completion of last command)
  //[0] = HOST BUSY
  // This is the main operation loop.  If the operation results in a Smbus
  // collision with another master on the bus, it attempts the requested
  // transaction again at least BUS_TRIES attempts.
  //
  while (RetryCount--) {
    //
    // Operation Specifics (pre-execution)
    //
    Status          = MMRC_SUCCESS;
    SlvAddrReg      = SlvAddr;
    HostCmdReg      = Offset;
    AuxcReg         = 0;

    switch (Operation) {

    case SmbusWriteByte:
      IoOut8 (SmbusBase + R_PCH_SMBUS_HD0, CallBuffer[0]);
      SmbusOperation = V_PCH_SMBUS_SMB_CMD_BYTE_DATA;
      break;

    case SmbusReadByte:
      SmbusOperation = V_PCH_SMBUS_SMB_CMD_BYTE_DATA;
      SlvAddrReg |= B_PCH_SMBUS_RW_SEL_READ;
      if (*Length < 1) {
        Status = MMRC_FAILURE;
      }
      *Length = 1;
      break;

    case SmbusWriteBlock:
      SmbusOperation  = V_PCH_SMBUS_SMB_CMD_BLOCK;
      IoOut8 (SmbusBase + R_PCH_SMBUS_HD0, * (UINT8 *) Length);
      BlockCount = (UINT8) (*Length);
      if ((*Length < 1) || (*Length > 32) ) {
        Status = MMRC_FAILURE;
        break;
      }
      AuxcReg |= B_PCH_SMBUS_E32B;
      break;

    case SmbusReadBlock:
      SmbusOperation = V_PCH_SMBUS_SMB_CMD_BLOCK;
      SlvAddrReg |= B_PCH_SMBUS_RW_SEL_READ;
      if ((*Length < 1) || (*Length > 32) ) {
        Status = MMRC_FAILURE;
        break;
      }
      AuxcReg |= B_PCH_SMBUS_E32B;
      break;

    default:
      Status = MMRC_FAILURE;
      break;
    }

    //
    // Set Auxiliary Control register
    //
    IoOut8 (SmbusBase + R_PCH_SMBUS_AUXC, AuxcReg);

    //
    // Reset the pointer of the internal buffer
    //
    IoIn8 (SmbusBase + R_PCH_SMBUS_HCTL);

    //
    // Now that the 32 byte buffer is turned on, we can write th block data
    // into it
    //
    if (Operation == SmbusWriteBlock) {
      for (Index = 0; Index < BlockCount; Index++) {
        //
        // Write next byte
        //
        IoOut8 (SmbusBase + R_PCH_SMBUS_HBD, CallBuffer[Index]);
      }
    }

    //
    // Set SMBus slave address for the device to read
    //
    IoOut8 (SmbusBase + R_PCH_SMBUS_TSA, SlvAddrReg);

    //
    //
    // Set Command register for the offset to read
    //
    IoOut8 (SmbusBase + R_PCH_SMBUS_HCMD, HostCmdReg);

    //
    // Set Control Register to Set "operation command" protocol and start bit
    //
    IoOut8 (SmbusBase + R_PCH_SMBUS_HCTL, (UINT8) (SmbusOperation + B_PCH_SMBUS_START) );

    //
    // Wait for IO to complete
    //
    do {
      StsReg = (UINT8) IoIn8 (SmbusBase + 0);
    } while ((StsReg & (BIT4 | BIT3 | BIT2 | BIT1) ) == 0);

    if (StsReg & B_PCH_SMBUS_DERR) {
      Status = MMRC_FAILURE;
      break;
    } else if (StsReg & B_PCH_SMBUS_BERR) {
      //
      // Clear the Bus Error for another try
      //
      Status = MMRC_FAILURE;
      IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BERR);
      //
      // Clear Status Registers
      //
      IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
      IoOut8 (SmbusBase + R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);

      continue;
    }

    //
    // successfull completion
    // Operation Specifics (post-execution)
    //
    switch (Operation) {

    case SmbusReadByte:
      CallBuffer[0] = (UINT8) (IoIn8 (SmbusBase + R_PCH_SMBUS_HD0) );
      break;

    case SmbusWriteBlock:
      IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BYTE_DONE_STS);
      break;

    case SmbusReadBlock:
      BufferTooSmall = FALSE;
      //
      // Find out how many bytes will be in the block
      //
      BlockCount = (UINT8) (IoIn8 (SmbusBase + R_PCH_SMBUS_HD0) );
      if (*Length < BlockCount) {
        BufferTooSmall = TRUE;
      } else {
        for (Index = 0; Index < BlockCount; Index++) {
          //
          // Read the byte
          //
          CallBuffer[Index] = (UINT8) IoIn8 (SmbusBase + R_PCH_SMBUS_HBD);
        }
      }

      *Length = BlockCount;
      if (BufferTooSmall) {
        Status = MMRC_FAILURE;
      }
      break;

    default:
      break;
    };

    if ((StsReg & B_PCH_SMBUS_BERR) && (Status != MMRC_FAILURE)) {
      //
      // Clear the Bus Error for another try
      //
      Status = MMRC_FAILURE;
      IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BERR);

      continue;
    } else {
      break;
    }
  }

  //
  // Clear Status Registers and exit
  //
  IoOut8 (SmbusBase + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
  IoOut8 (SmbusBase + R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);
  IoOut8 (SmbusBase + R_PCH_SMBUS_AUXC, 0);
#endif
  return Status;
#endif
}

UINT32
ConvertLinearToPhysicalValue (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT16         DimIndex,
  IN        UINT32        LinearValue
)
{
  UINT32  PhysicalValue;
#if 0
  UINT8   TotalSize;
  UINT8   *LinearTable;
  UINT8   DummyValue;

  TotalSize     = 0;
  DummyValue    = 0;
  PhysicalValue = 0;
  LinearTable   = (UINT8 *)&DummyValue;

  if (DimIndex == RX_VREF) {
    TotalSize = MAX_ELEMENT_TABLE;
    LinearTable = (UINT8 *) LinearToPhysicalVrefCodes;
  } else if (DimIndex == TX_VREF){
    TotalSize = MrcData->OemMrcData.PhyVrefTable.SizeOfLtoPTable;
    LinearTable = (UINT8 *)MrcData->OemMrcData.PhyVrefTable.LinearToPhysicalTable;
  } else {
    PhysicalValue = LinearValue;
  }

  if (LinearValue >= TotalSize && TotalSize != 0) {
    MmrcErrorPrint ((MMRC_DBG_MIN, "Warning!! TotalSize = %d Linear = %d \n", TotalSize, LinearValue));
    MrcDeadLoop ();
  } else if (TotalSize != 0) {
    PhysicalValue = LinearTable[LinearValue];
  }
  return PhysicalValue;
#else
  if ((DimIndex >= RxVocVal0) && (DimIndex <= RxVocVal7)){
    PhysicalValue= LinearValue > 7 ? LinearValue-8 : 15-LinearValue;
  }
  else
  {
    PhysicalValue = LinearValue;
  }
  return PhysicalValue;
#endif
}

UINT32
ConvertPhysicalToLinearValue (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT16         DimIndex,
  IN        UINT32        PhysicalValue
)
{
  UINT32  LinearValue;
#if 0
  UINT8   TotalSize;
  UINT8   *PhysicalTable;
  UINT8   DummyValue;

  TotalSize     = 0;
  DummyValue    = 0;
  LinearValue   = 0;
  PhysicalTable = (UINT8 *)&DummyValue;

  if (DimIndex == RX_VREF) {
    TotalSize = MAX_ELEMENT_TABLE;
    PhysicalTable = (UINT8 *) PhysicalToLinearVrefCodes;
  } else if (DimIndex == TX_VREF){
    TotalSize = MrcData->OemMrcData.PhyVrefTable.SizeOfPtoLTable;
    PhysicalTable = (UINT8 *) MrcData->OemMrcData.PhyVrefTable.PhysicalToLinearTable;
  } else {
    LinearValue = PhysicalValue;
  }

  if (PhysicalValue >= TotalSize && TotalSize != 0) {
    MmrcErrorPrint ((MMRC_DBG_MIN, "Warning!! TotalSize = %d Physical = %d \n", TotalSize, PhysicalValue));
    MrcDeadLoop ();
  } else if (TotalSize != 0) {
    LinearValue = PhysicalTable[PhysicalValue];
  }
  return LinearValue;
#else
  if ((DimIndex >= RxVocVal0) && (DimIndex <= RxVocVal7)){
    LinearValue= PhysicalValue > 7 ? 15-PhysicalValue : PhysicalValue+8;
  }
  else
  {
    LinearValue = PhysicalValue;
  }
  return LinearValue;
#endif
}

MMRC_STATUS
PerformDQReset (
  IN OUT   MMRC_DATA      *MrcData,
  IN       UINT8          Channel,
  IN       UINT8          Rank
)
{
  UINT32 TempValue;
  UINT8  Strobe;

  for (Strobe = 0; Strobe < MAX_STROBES; Strobe++) {
    if (!IsStrobeValid (MrcData, Channel, Rank, Strobe)) {
      continue;
    }
  TempValue = 0;
    GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, DQS_CNT_RESET, CMD_SET_VAL_FC, &TempValue);
  }
    #if SIM
      MySimStall (50);
    #else
      MrcDelay (MrcData, NANO_DEL, 20);
    #endif
  for (Strobe = 0; Strobe < MAX_STROBES; Strobe++) {
    if (!IsStrobeValid (MrcData, Channel, Rank, Strobe)) {
      continue;
    }
    TempValue = 1;
    GetSetDdrioGroup2 (MrcData, Channel, Rank, Strobe, DQS_CNT_RESET, CMD_SET_VAL_FC, &TempValue);
  }

  return MMRC_SUCCESS;
}

////Rx Power Training
//typedef enum {
//  UNIQUE = 0,
//  STROBE_BASED = 1,
//  RANK_BASED = 2,
//  ALL_STROBES = 3
//} ACCESS_TYPE;
//
//typedef struct {
//  UINT8         TypeIndex;
//  ACCESS_TYPE   AccessType;
//  UINT8         RestoreOnS3Resume;
//} RESTORE_DATA_DESCRIPTOR;

//typedef enum {
//  SAVE = 0,
//  RESTORE = 1,
//} SAVE_RESTORE_TASK;

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
)
{
  if (Cmd == RD_REG) {
    *Value = MrcData->Channel[Channel].DimmVrefWrite[Rank][Strobe];
  } else {
    WriteDimmVref (MrcData, Channel, *Value, OVERRIDE);
    MrcData->Channel[Channel].DimmVrefWrite[Rank][Strobe] = (UINT8)*Value;
  }

  return MMRC_SUCCESS;
}

VOID
WriteDimmVref (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT32        vref,
  IN        UINT8         Override
)
{
  UINT32 txVref;
  UINT8  Length;

#if JTAG || SIM
  return;
#endif

  Length = 1;

  if (Override == OVERRIDE) {
    txVref = vref;
  } else {
    txVref = 0x40;
  }
  //
  // LPK has DIMM VREF control at SMBUS addresses 0x5C and 0x7C.
  // MPK has DIMM VREF control at SMBUS address 0x5C for writing and 0x5D for reading.
  // TCV has DIMM VREF control at SMBUS address 0x5C.
  // Since the platform design guide specifies 1 DIMM VREF control per platform is
  // sufficient, AVN MRC will support a single VREF per platform. This means that
  // LPK's 2 DIMM VREF circuits will be programmed to the same value.
  //
  MrcSmbusExec (
    MrcData->SmbusBar,
    0x5C,
    SmbusWriteByte,
    0,
    &Length,
    (UINT8 *) &txVref
    );

  MrcSmbusExec (
    MrcData->SmbusBar,
    0x7C,
    SmbusWriteByte,
    0,
    &Length,
    (UINT8 *) &txVref
    );
  //
  // Add some time for Vref circuit to settle
  //
  MrcDelay (MICRO_DEL, 100);
}
*/

UINT32
SendDramCmd (
  MMRC_DATA         *MrcData,
  UINT8             Channel)
/*++

Routine Description:

  Executes and prints (if enabled) the MR values

Arguments:

  Operand  - MRCData and Channel

Returns:

  Success
  Failure
--*/
{
#ifdef MRR_DEBUG
  MmrcDebugPrint ((MMRC_DBG_TRACE, "MRR chunk1:%02X chunk2:%02X chunk3:%02X chunk4:%02X\n",
                  (MrcData->DynamicVars[Channel][REG_DATA] & 0x3F),
                  ((MrcData->DynamicVars[Channel][REG_DATA] >>  6) & 0x3F),
                  ((MrcData->DynamicVars[Channel][REG_DATA] >> 12) & 0x3F),
                  ((MrcData->DynamicVars[Channel][REG_DATA] >> 18) & 0x3F)
                  ));
#endif
  MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  return MMRC_SUCCESS;
}

UINT32
  ReadMRValue (
  IN OUT MMRC_DATA *MrcData,
  IN UINT8 Channel,
  IN UINT8 MRNumber
  ) {

  REGISTER_ACCESS     Register;
  UINT32 TempValue;

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
      MrcData->DynamicVars[Channel][REG_DATA] = MRR_LP4(MRNumber);
      MrcData->DynamicVars[Channel][RDDCMD]   = 1;
      MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  } else if (MrcData->Channel[Channel].DramType == TypeWio2) {
  }

  MrcDelay (MrcData, MICRO_DEL, 10);

  Register.Offset = D_CR_DCMDDATA_DUNIT_REG;
  Register.Mask = 0xFFFFFFFF;
  Register.ShiftBit = 0;
  TempValue = ((UINT32) MemRegRead (DUNIT, Channel, 0, Register)) & 0xFF;
  return TempValue;

}

MMRC_STATUS
PostMrcDoneSettings (
  IN  OUT   MMRC_DATA   *MrcData,
  UINT16                 CapsuleStartIndex,
  UINT16                 StringIndex,
  UINT8                  Channel)
{
  SetSliceChannelHash(MrcData);
  return MMRC_SUCCESS;
}

/**
  SetSliceChannelHash

  @param[in]  MrcData  Host structure for all MRC global data.

  @retval  None
**/
VOID
SetSliceChannelHash (
  IN OUT MMRC_DATA *MrcData)
{

///
/// Set the B_CR_SLICE_CHANNEL_HASH as an MSR - this is the only way to do this
/// MSR write 0 to location 290 called B_CR_SLICE_CHANNEL_HASH
/// disable CH1 for LP3
///

  REGISTER_ACCESS                                        Register;
  A_CR_SLICE_CHANNEL_HASH_0_0_0_MCHBAR_AUNIT_STRUCT      SliceChannelReg;
  UINT32                                                 LoBits;
  UINT32                                                 HiBits;

  SliceChannelReg.Data = 0;

  SliceChannelReg.Bits.ch_1_disabled    = MrcData->MrcInputs.SlicesChannelsEnabled & BIT1;
  SliceChannelReg.Bits.slice_1_disabled = MrcData->MrcInputs.SlicesChannelsEnabled & BIT0;
  SliceChannelReg.Bits.ch_hash_mask     = MrcData->MrcInputs.ChannelHashMask;
  SliceChannelReg.Bits.slice_hash_mask  = MrcData->MrcInputs.SliceHashMask;
  SliceChannelReg.Bits.interleave_mode  = MrcData->MrcInputs.InterleavedMode;
  SliceChannelReg.Bits.lock = 1;

  LoBits = ((UINT32*)(&SliceChannelReg.Data))[0];
  HiBits = ((UINT32*)(&SliceChannelReg.Data))[1];

  Register.Offset = A_CR_SLICE_CHANNEL_HASH_0_0_0_MCHBAR_AUNIT_REG;
  Register.Mask = 0xFFFFFFFFFFFFFFFF;
  Register.ShiftBit = 0;
  MemRegWrite (AUNIT, 0, 0, Register, SliceChannelReg.Data, 0xF);
  MemRegRead(AUNIT, 0, 0, Register);

  _asm {
    push eax
    push edx
    push ecx
    mov eax, LoBits
    mov edx, HiBits
    mov ecx, 0122h
    wrmsr
    pop ecx
    pop edx
    pop eax
  }
}

MMRC_STATUS
ReadMRTQREF(
IN  OUT   MMRC_DATA   *MrcData,
UINT16            CapsuleStartIndex,
UINT16            StringIndex,
UINT8             Channel)
{

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if ((MrcData->BootMode == S5) || (MrcData->BootMode == FB)) {
      //
      // LP4_MR0VALUE and LP4_MR4VALUE are part of MRC_PARAMS_SAVE_RESTORE_DEF
      // meaning that their values will be preserved in NVRAM across resets.
      //
      MrcData->Channel[Channel].LP4_MR4VALUE = (UINT16)ReadMRValue(MrcData, Channel, 4);
      MrcData->Channel[Channel].LP4_MR0VALUE = (UINT16)ReadMRValue(MrcData, Channel, 0);
    }
    MrcData->DynamicVars[Channel][MR_VALUE] = MrcData->Channel[Channel].LP4_MR4VALUE;
    //
    // Modified refresh mode HSD1012349953
    //
    if (MrcData->Channel[Channel].LP4_MR0VALUE == 1) {
      MrcData->DynamicVars[Channel][REFWMPNC] = 3;
      MrcData->DynamicVars[Channel][REFWMHI] = 2;
    } else {
      MrcData->DynamicVars[Channel][REFWMPNC] = 7;
      MrcData->DynamicVars[Channel][REFWMHI] = 5;
    }
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    MrcData->DynamicVars[Channel][MR_VALUE] = 0;
  } else {
    //WIO
    MrcData->DynamicVars[Channel][MR_VALUE] = ReadMRValue(MrcData, Channel, 4);
  }

  MmrcDebugPrint((MMRC_DBG_MIN, "<!> FIFO Reset %d\n", Channel));
  PerformFifoReset(MrcData, Channel, 0);

  return MMRC_SUCCESS;
}

MMRC_STATUS
R2RDynamicSetup (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
  /*++

Routine Description:

  Fills in the complete parameter list for the Modular MRC.  This includes all the Dynamic entries that are listed in the Input Spreadsheet, but also
  the PFCT values.

Arguments:

  MrcData:       Host structure for all data related to MMRC

  Channel:       Channel to do the conversion on.

Returns:

  Success
  Failure

--*/
{

  UINT32 TempVar;
  UINT8 Rank;
  UINT8 Strobe;

  //
  // Loop through each supported Rank and supported Strobe reading the R2Ren flags into cache.
  // Also updtae the Rk2REn for each algo.
  //
  for (Rank=0; Rank < MAX_RANKS; Rank++) {
    if (!RunOnThisChannel (MrcData, Channel, Rank)) {
      continue;
    }
    for (Strobe=0; Strobe < MrcData->Channel[Channel].MaxDq[Rank]; Strobe++)
    {
      if (!IsStrobeValid (MrcData, Channel, Rank, Strobe)) {
        continue;
      }
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RRcvnEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.RcvEn = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RRxDqsEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.RxDqs = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RTxDqEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.TxDq = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RTxDqsEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.TxDqs = (UINT8) TempVar;
    }
  }
  MmrcDebugPrint((MMRC_DBG_MIN, "Rk2RkEn Rcv=%d TxDqs=%d TxDq=%d RxDqs=%d RxPBD=%d TxPBD=%d\n", MrcData->Rk2RkEn.RcvEn, MrcData->Rk2RkEn.TxDqs, MrcData->Rk2RkEn.TxDq, MrcData->Rk2RkEn.RxDqs, MrcData->ReadPerBitEnable, MrcData->WritePerBitEnable));
  return MMRC_SUCCESS;
}

MMRC_STATUS
MmrcConvertParams (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
  /*++

Routine Description:

  Fills in the complete parameter list for the Modular MRC.  This includes all the Dynamic entries that are listed in the Input Spreadsheet, but also
  the PFCT values.

Arguments:

  MrcData:       Host structure for all data related to MMRC

  Channel:       Channel to do the conversion on.

Returns:

  Success
  Failure

--*/
{

  UINT8  BLMode;
  UINT8  TimingParamVar;
//  UINT32 TempVar;
 // UINT8  DQBusWidth;
 // UINT8  DeviceWidth;
 // UINT16 DramCapacity;

  DEBUP0_struct  DEBUPValue;
  REGISTER_ACCESS Register;
  //
  // Convert the format of "MrcData->Channel[Channel].CurrentConfiguration"
  // from numX to bitX. This will allow us to compare agains the bit masks
  // C_PO and C_POR
  //
  UINT8 CurrentConfiguration = NUM_TO_BIT(MrcData->Channel[Channel].CurrentConfiguration);
//  UINT8 Rank;
//  UINT8 Strobe;

  //
  // Loop through each supported Rank and supported Strobe reading the R2Ren flags into cache.
  // Also updtae the Rk2REn for each algo.
  //
  /*
  for (Rank=0; Rank < MAX_RANKS; Rank++) {
    if (!RunOnThisChannel (MrcData, Channel, Rank)) {
      continue;
    }
    for (Strobe=0; Strobe < MrcData->Channel[Channel].MaxDq[Rank]; Strobe++)
    {
      if (!IsStrobeValid (MrcData, Channel, Rank, Strobe)) {
        continue;
      }
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RRcvnEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.RcvEn = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RRxDqsEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.RxDqs = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RTxDqEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.TxDq = (UINT8) TempVar;
      GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, R2RTxDqsEn, CMD_GET_REG_UC, &TempVar);
      MrcData->Rk2RkEn.TxDqs = (UINT8) TempVar;
    }
  }
 */
  //UINT8 DramDensity[5] = {4, 6, 8, 12, 16};
  MrcData->EnableVocCtle=TRUE;
  MrcData->EnableMaxPiDbselProcess=FALSE;
  Register.Offset = DEBUP0_0_0_0_PCI_CUNIT_REG;
  Register.Mask   = 0xFFFFFFFF;
  DEBUPValue.Data = (UINT32)MemRegRead (CUNIT, 0, 0, Register);

  //
  // Set the default Per Bit De-Skew configuration
  //
  MrcData->ReadPerBitEnable = TRUE;
  MrcData->WritePerBitEnable = TRUE;

#if !defined(JTAG) && !defined(SIM)
  //
  // Invert the default configuration if the debug bit is set
  //
  MrcData->ReadPerBitEnable ^= DEBUPValue.DEBUP0_bits.readPBD;
  MrcData->WritePerBitEnable ^= DEBUPValue.DEBUP0_bits.writePBD;
  MmrcDebugPrint((MMRC_DBG_MIN, "<+> Per bit deskew:\x1b[1;92m \n")); // use the color green "\x1b[1;92m"
  if (MrcData->ReadPerBitEnable) {
    MmrcDebugPrint((MMRC_DBG_MIN, "ENABLED for READS "));
  }
  if (MrcData->WritePerBitEnable) {
    MmrcDebugPrint((MMRC_DBG_MIN, "ENABLED for WRITES"));
  }
  MmrcDebugPrint((MMRC_DBG_MIN, "\x1b[0m")); // clear text formatting and color "\x1b[0m"
#endif
  memcpy((void* ) MrcData->GranularitySteps, (void *) GranularityGenerated, sizeof(GranularityGenerated));

  MrcData->DynamicVars[Channel][DDEN] = MrcData->Channel[Channel].D_Size[0];
  /*
  DramCapacity = 256; //Mb
  DeviceWidth  = 4;
  DQBusWidth   = 8;

//  for (Channel = 0; Channel < MAX_CHANNELS; Channel++ ) {
    ////MrcData->Channel[Channel].TotalMem = 0;

    if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
       DQBusWidth = 32;
       DeviceWidth = 32;//MrcData->Channel[Channel].D_DataWidth[0];
    } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
       DQBusWidth = 16;
       DeviceWidth = 16;
    } else {
       DQBusWidth = 64;
       DeviceWidth = MrcData->Channel[Channel].D_DataWidth[0];
    }
    //
    // Look at both DIMMs and fill in DIMM-specific variables. Also add up total memory
    // per channel and system-wide.
    //
    //if (MrcData->Channel[Channel].DimmPresent[0]) {
    /*if (MrcData->Channel[Channel].RankEnabled[0]) {
      //  DRAM chip density / 8 * (dq bus /data width)

      MrcData->Channel[Channel].SlotMem[0]         = ( (DramDensity[MrcData->DynamicVars[Channel][DDEN]] * 1024 / 2) / 8 * DQBusWidth) / DeviceWidth
                                                     * MrcData->Channel[Channel].D_Ranks[0];
      MrcData->Channel[Channel].TotalMem           += MrcData->Channel[Channel].SlotMem[0];
      MrcData->SystemMemorySize                    += MrcData->Channel[Channel].SlotMem[0];

   MmrcDebugPrint((MMRC_DBG_MAX, "DIMM0 Memory Size: %d, System Mem %d in MB\n", MrcData->Channel[Channel].SlotMem[0], MrcData->SystemMemorySize));
    if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
      MrcData->NumberOfChannels[TypeLpDdr3] += 1;
    }

    if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    MrcData->NumberOfChannels[TypeLpDdr4] += 1;
    }

    } //rankEnabled*/
  //}

  switch (Channel) {
  case 0://8:
    MrcData->DynamicVars[0][FM_CH0] = 1;
    MrcData->DynamicVars[0][FM_SIZE_CH8] = MrcData->Channel[Channel].TotalMem / 256;  ///multiple of 256M
  break;
  case 1://9:
    MrcData->DynamicVars[0][FM_CH1] = 1;
    MrcData->DynamicVars[0][FM_SIZE_CH9] = MrcData->Channel[Channel].TotalMem / 256;
  break;
  case 2://10:
    MrcData->DynamicVars[0][FM_CH2] = 1;
    MrcData->DynamicVars[0][FM_SIZE_CH10] = MrcData->Channel[Channel].TotalMem / 256;
  break;
  case 3://11:
    MrcData->DynamicVars[0][FM_CH3] = 1;
    MrcData->DynamicVars[0][FM_SIZE_CH11] = MrcData->Channel[Channel].TotalMem / 256;
  break;
  default:  //WIO
    MrcData->DynamicVars[0][FM_SIZE_CH11] = MrcData->Channel[Channel].TotalMem / 256;
   } //switch
  //REQ_TYPE needs to be set when memory is in SR.  That should only happen in S0 bootmode
  if (MrcData->BootMode == S0) {
    MrcData->DynamicVars[0][PUNIT_REQ_TYPE] = 0x4;
  } else {
    MrcData->DynamicVars[0][PUNIT_REQ_TYPE] = 0;
  }
  TimingParamVar = (MrcData->DVFS_FreqA - 1);
  if (MrcData->PlatformID != 0) {
    MrcData->DynamicVars[0][PUNIT_REQ_DATA] = ConfigToSpeedMultiple133[TimingParamHash (TimingParamVar, TRUE)];
  } else {
    //if (MrcData->pSmipData == NULL) {
  ////TODO[OA]: Check how to replace the previous check and populate DramPolicyData for PlatformID == 0
  if ((MrcData->PlatformID == 0) || (MrcData->DramPolicyData.MemoryDown == 1)) {
      MrcData->DynamicVars[0][PUNIT_REQ_DATA] = ConfigToSpeedMultiple133[TimingParamHash (TimingParamVar, TRUE)];
    } else {
      MrcData->DynamicVars[0][PUNIT_REQ_DATA] = ConfigToSpeedMultiple133[TimingParamHash (MrcData->DramPolicyData.Profile - 1, TRUE)];
    }
  }

  MrcData->DynamicVars[Channel][RANK0_ENABLED] = MrcData->Channel[Channel].RankEnabled[0];
  MrcData->DynamicVars[Channel][RANK1_ENABLED] = MrcData->Channel[Channel].RankEnabled[1];

  //Option needs to be 0 for PO configuration and 3 for POR.
  MrcData->DynamicVars[Channel][ADDRDEC] = (MrcData->DramPolicyData.ChDrp[Channel].Option >> 4) & 0x3;
  MrcData->DynamicVars[Channel][BAHEN] =   (MrcData->DramPolicyData.ChDrp[Channel].Option >> 1) & 0x1;
  MrcData->DynamicVars[Channel][RSIEN] =   MrcData->DramPolicyData.ChDrp[Channel].Option & 0x1;

  //MrcData->DynamicVars[0][ONE_LM_MODE] = ~(MrcData->TwoLMEnable);

  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    if (MrcData->NumberOfChannels[TypeLpDdr3] == 1) {
      MrcData->DynamicVars[Channel][FM_ACTIVE_SNWE] = 0x1;
      MrcData->DynamicVars[0][FM_ACTIVE_SNWE] = 0x1;
    } else {
      MrcData->DynamicVars[Channel][FM_ACTIVE_SNWE] = 0x9;
      MrcData->DynamicVars[0][FM_ACTIVE_SNWE] = 0x9;
    }
  }

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    MrcData->DynamicVars[Channel][FM_ACTIVE_SNWE] = 0xf;
    MrcData->DynamicVars[0][FM_ACTIVE_SNWE] = 0xf;
  }

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    BLMode = 32;
  } else {
    BLMode = 8;
  }

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if (CurrentConfiguration == C_PO) {
      MrcData->DynamicVars[Channel][MINREFRATE] = 0x3;
    } else {
      MrcData->DynamicVars[Channel][MINREFRATE] = 0x2;
    }
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    if (MrcData->CpuStepping == STEPPING_A0) {
      MrcData->DynamicVars[Channel][MINREFRATE] = 0x5;
    } else {
      if (CurrentConfiguration == C_PO) {
         MrcData->DynamicVars[Channel][MINREFRATE] = 0x3;
      } else {
        MrcData->DynamicVars[Channel][MINREFRATE] = 0x2;
      }
    }
  } else {  //ddr3L
    MrcData->DynamicVars[Channel][MINREFRATE] = 0x3;
  }

  if ((MrcData->MrcInputs.ForceMin2xRefresh) && (MrcData->DynamicVars[Channel][MINREFRATE] < 0x4)) {
     MrcData->DynamicVars[Channel][MINREFRATE] = 0x4;
  }

  if (CurrentConfiguration == C_PO) {
    MrcData->DynamicVars[Channel][REFWMPNC] = 7;
    MrcData->DynamicVars[Channel][REFWMHI] = 5;
  }

  MrcData->DynamicVars[Channel][SPIDCMDCLK_RATIO] = 2;
  MrcData->DynamicVars[Channel][SPIDDATA_RATIO_WIO2] = (MrcData->DynamicVars[Channel][SPIDCMDCLK_RATIO] * 4) + 3 ;

  MrcData->DynamicVars[Channel][SACLKBY2_RATIO] = 3;
  MrcData->DynamicVars[Channel][TCKE] = GetTimingParam(BxtTimingTcke);

  //MrcData->DynamicVars[Channel][TCKSRX] = GetTimingParam(BxtTimingTcksrx);

  //TCKCKEH -- review, lp4-4, lp3-2
  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if (CurrentConfiguration == C_PO) {
       MrcData->DynamicVars[Channel][TCKSRX] = GetTimingParam(BxtTimingTcksrx);
    } else {
      MrcData->DynamicVars[Channel][TCKSRX] = 0;
    }
  } else {
    if (CurrentConfiguration == C_PO){
      MrcData->DynamicVars[Channel][TCKSRX] = 2;
    } else {
      MrcData->DynamicVars[Channel][TCKSRX] = 0;
    }
  }

  MrcData->DynamicVars[Channel][RL] =  GetTimingParam(BxtTimingTcl);
  MrcData->DynamicVars[Channel][TRAS] = GetTimingParam(BxtTimingTras);
  MrcData->DynamicVars[Channel][TRCD] = GetTimingParam(BxtTimingTrcd);
  MrcData->DynamicVars[Channel][TRRD] = GetTimingParam(BxtTimingTrrd);
  MrcData->DynamicVars[Channel][TRTP] = GetTimingParam(BxtTimingTrtp);
  MrcData->DynamicVars[Channel][TFAW] = GetTimingParam(BxtTimingTfaw);

  ////LPDDR3 Equation: = BL/2 + tRTP - 4
  //LPDDR4 Equation: = BL/2 + Max (8, tRTP) - 8
  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
  MrcData->DynamicVars[Channel][TRTP] = (BLMode/2) + GetTimingParam(BxtTimingTrtp) - 4;
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    MrcData->DynamicVars[Channel][TRTP] = (BLMode/2) + MAX(8, GetTimingParam(BxtTimingTrtp)) - 8;
  }

  MrcData->DynamicVars[Channel][TCKESR] = GetTimingParam(BxtTimingTckesr);
  MrcData->DynamicVars[Channel][TRPPB] = GetTimingParam(BxtTimingTrppb);

  MrcData->DynamicVars[Channel][TCWL] = GetTimingParam(BxtTimingTcwl);
  MrcData->DynamicVars[Channel][WL] = MrcData->DynamicVars[Channel][TCWL];

  MrcData->DynamicVars[Channel][TRRDR] = GetTimingParam(BxtTimingTrrdr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TWWDR] = GetTimingParam(BxtTimingTwwdr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TWRDR] = GetTimingParam(BxtTimingTwrdr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TRWSR] = GetTimingParam(BxtTimingTrwsr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TRRSR] = GetTimingParam(BxtTimingTrrsr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TWRSR] = GetTimingParam(BxtTimingTwrsr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TWWSR] = GetTimingParam(BxtTimingTwwsr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TRWDR] = GetTimingParam(BxtTimingTrwdr) + GUARDBAND;
  MrcData->DynamicVars[Channel][TXP] = GetTimingParam(BxtTimingTxp);

  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    MrcData->DynamicVars[Channel][TZQCS] = GetTimingParam(BxtTimingTzqcs);
  } else {
    MrcData->DynamicVars[Channel][TZQCS] = 0;
  }
  MrcData->DynamicVars[Channel][TZQCL] = GetTimingParam(BxtTimingTzqcl);
  MrcData->DynamicVars[Channel][TZQLAT] = GetTimingParam(BxtTimingTzqlat);
  MrcData->DynamicVars[Channel][TWTP] =  BLMode/2 + GetTimingParam(BxtTimingTcwl) + GetTimingParam(BxtTimingTwr) + 1;
  MrcData->DynamicVars[Channel][NREFI] = GetTimingParam(BxtTimingTrefi);
  MrcData->DynamicVars[Channel][TRDPDEN] = GetTimingParam(BxtTimingTrdpden);
  MrcData->DynamicVars[Channel][TWRPDEN] = GetTimingParam(BxtTimingTwrpden);
  MrcData->DynamicVars[Channel][TRPAB] = GetTimingParam(BxtTimingTrpab);

  if (MrcData->Channel[Channel].DramType == TypeWio2) {
    MrcData->DynamicVars[Channel][TZQCL] = 0;
  }

 // if (MrcData->Channel[Channel].DramType == TypeDdr3L) {
    MrcData->DynamicVars[Channel][TZQCL] = GetTimingParam(BxtTimingTzqoper);
 // }


  if ((MrcData->Channel[Channel].DramType == TypeLpDdr4) || (MrcData->Channel[Channel].DramType == TypeWio2)) {
    MrcData->DynamicVars[Channel][TZQCS] = 0;
  }

  if (MrcData->Channel[Channel].DramType != TypeLpDdr4) {
    MrcData->DynamicVars[Channel][TZQLAT] = 0;
  }

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if (MrcData->BxtSeries == SERIES_BXT_1) {
      MrcData->DynamicVars[Channel][TCCDMW] = GetTimingParam(BxtTimingTccdmw) + 8;
    } else {
      MrcData->DynamicVars[Channel][TCCDMW] = GetTimingParam(BxtTimingTccdmw);
    }
  } else if (MrcData->Channel[Channel].DramType == TypeWio2) {
    MrcData->DynamicVars[Channel][TCCDMW] = GetTimingParam(BxtTimingTccdmw) + 2;
  } else {  //lpddr3 and ddr3l
    MrcData->DynamicVars[Channel][TCCDMW] = 0;
  } //if TypeWio2

  MrcData->DynamicVars[Channel][TXSR] = GetTimingParam(BxtTimingTxsr4Gb + MrcData->Channel[Channel].D_Size[0]);
  MrcData->DynamicVars[Channel][NRFCAB] = GetTimingParam(BxtTimingTrfc4Gb + MrcData->Channel[Channel].D_Size[0]);
  MrcData->DynamicVars[Channel][TDERATE_INC] = (1875) / TCK[MrcData->CurrentFrequency];
  if ((1875) % TCK[MrcData->CurrentFrequency] >= ((1875) / TCK[MrcData->CurrentFrequency])) {
    MrcData->DynamicVars[Channel][TDERATE_INC] += 1;
  }

  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if (CurrentConfiguration == C_PO) {
      MrcData->DynamicVars[Channel][TCKCKEL] = GetTimingParam(BxtTimingTcksre);
    } else {
      MrcData->DynamicVars[Channel][TCKCKEL] = GetTimingParam(BxtTimingTcksre) - 4;
    }
  } else {
    MrcData->DynamicVars[Channel][TCKCKEL] = 0;
  }

    //tRP + tRCD + RL + tCCD
  //(BL/2 + tRPpb + tRCD + 3 ) / 16
  if (CurrentConfiguration == C_PO) {
    MrcData->DynamicVars[Channel][MNTDLY] = 0;
  } else {
  if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    if(MrcData->BxtSeries == SERIES_BXT) {
      MrcData->DynamicVars[Channel][MNTDLY] = (GetTimingParam(BxtTimingTrppb) + BLMode/2 +
        GetTimingParam(BxtTimingTrcd) + 3) / 16;
    } else {
      MrcData->DynamicVars[Channel][MNTDLY] = 0;
    }
  } else {
    MrcData->DynamicVars[Channel][MNTDLY] = (GetTimingParam(BxtTimingTrppb) + BLMode/2 +
    GetTimingParam(BxtTimingTrcd) + 3) / 8;
  }
}

  MrcData->DynamicVars[Channel][TPSTMRBLK] = 0; // Updated in CP 0x0A "SetTpstmrblk"

  if (MrcData->CpuStepping != STEPPING_P0) {
    //LP4: tXP + tRCD - 8, LP3/Wio2: tXP + tRCD - 4
    if (CurrentConfiguration == C_PO) {
      MrcData->DynamicVars[Channel][TPREMRBLK] = GetTimingParam(BxtTimingTxp) + GetTimingParam(BxtTimingTrcd);
    } else {
      if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
        MrcData->DynamicVars[Channel][TPREMRBLK] = GetTimingParam(BxtTimingTxp) + GetTimingParam(BxtTimingTrcd) - 8;
      } else {
        MrcData->DynamicVars[Channel][TPREMRBLK] = GetTimingParam(BxtTimingTxp) + GetTimingParam(BxtTimingTrcd) - 4;
      }
    }
    ////TODO: Check if we should enable next write on E0
    //MrcData->DynamicVars[Channel][TPSTMRWBLK] = GetTimingParam(BxtTimingTmod);
  }

   if (MrcData->CpuStepping != STEPPING_P0) {
    // MrcData->DynamicVars[Channel][TPREMRBLK] = GetTimingParam(BxtTimingTxp) + GetTimingParam(BxtTimingTrcd) ;

     if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
       MrcData->DynamicVars[Channel][TLPCKDLY] = 0x1a;
     } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
       MrcData->DynamicVars[Channel][TLPCKDLY] = 0x2C;
     } else {
        MrcData->DynamicVars[Channel][TLPCKDLY] = 0xa;
     }

   }

  //30ns/clock
  MrcData->DynamicVars[Channel][TCKLPDLY] = (30 * 1000) / TCK[MrcData->CurrentFrequency];
  MrcData->DynamicVars[Channel][PWDDLY]   = (80 * 1000) / TCK[MrcData->CurrentFrequency];

  MrcData->DynamicVars[Channel][DQS_OSC_RT] = ((140+40) * 1000) / TCK[MrcData->CurrentFrequency] / 16;
  if ((((150+40) * 1000) % TCK[MrcData->CurrentFrequency] / 16) > (((150+40) * 1000) / TCK[MrcData->CurrentFrequency] / 16)) {
    MrcData->DynamicVars[Channel][DQS_OSC_RT] += 1;
  }
  MrcData->DynamicVars[Channel][DQS_RETRAIN_INT] = 1 * 1000000 / (TCK[MrcData->CurrentFrequency] * (UINT32) MrcData->DynamicVars[Channel][NREFI]/1000);

  //WPQCOUNT and RPQCOUNT value need to set back to default as it will change during CPGC
  MrcData->DynamicVars[Channel][WPQCOUNT] = 32;
  MrcData->DynamicVars[Channel][RPQCOUNT] = 16;

  if (MrcData->CpuStepping <= 1) {//WA 1202117097 TODO Fix stepping information
    MrcData->DynamicVars[Channel][SRXZQC] = 0x0;
  } else {
    if (CurrentConfiguration == C_PO) {
      MrcData->DynamicVars[Channel][SRXZQC] = 0x1;  //E0 safe = 1
    } else {
      MrcData->DynamicVars[Channel][SRXZQC] = 0x0;  //E0 safe = 1
    }
      if (MrcData->Channel[Channel].DramType == TypeWio2)  {
        MrcData->DynamicVars[Channel][SRXZQC] = 0x3;
      }
  }

  MrcData->DynamicVars[Channel][TQPOLLPER] = 1000000 / ( 4 *  (TCK[MrcData->CurrentFrequency] * (UINT32) MrcData->DynamicVars[Channel][NREFI]/1000));

  MrcData->DynamicVars[Channel][ECCEN] = 0;

  MrcData->DynamicVars[Channel][SSKPD0] = 1;
  MrcData->DynamicVars[Channel][SSKPD1] = 0;
  MrcData->DynamicVars[Channel][BONUS0] = 0;
  MrcData->DynamicVars[Channel][BONUS1] = 0;
  FloorPlanByteMapping(MrcData);
  //  MrcData->DynamicVars[Channel][DIMM0MIRR] = 0;

  //640ns / 16 x clock period
//    MrcData->DynamicVars[Channel][SREDLY]  = (640 * 1000) / TCK[MrcData->CurrentFrequency] / 16;
  MrcData->DynamicVars[Channel][PCLSTO]  = (150 * 1000) / TCK[MrcData->CurrentFrequency] / 16;
  if (((150 * 1000) / TCK[MrcData->CurrentFrequency]) % 16 >= (16/2)) {
    MrcData->DynamicVars[Channel][PCLSTO] += 1;
  }
  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 0x2;
    MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 0x7;
    MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0x0;
    if (Channel == 2) {
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0x4;
    } else {
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0x2;
    }
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
    switch (Channel) {
    case 0://8:
      MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 1;
      MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 6;
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0x7;
      MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0x5;
      break;
    case 1://9:
      MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 1;
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0x3;
      break;
    case 2://10:
      MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 1;
      MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 5;
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0x6;
      MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0x7;
      break;
    case 3://11:
      MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0x1;
      MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0x2;
      break;
    default:
      MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 0;
      MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 0;
    }
  } else if (MrcData->Channel[Channel].DramType == TypeWio2) {
    MrcData->DynamicVars[Channel][MR4_BYTE_SEL] = 0;
    MrcData->DynamicVars[Channel][MR4_BIT0_SEL] = 0;
    MrcData->DynamicVars[Channel][MR4_BIT1_SEL] = 1;
    MrcData->DynamicVars[Channel][MR4_BIT2_SEL] = 2;
  }

  if (CurrentConfiguration == C_PO){
    MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 0;
  } else {
    if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
      switch (Channel) {
      case 0://8:
        MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 0;
        break;
      case 1://9:
        MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 0;
        break;
      case 2://10:
        MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 1;
        break;
      case 3://11:
        MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 1;
        break;
      default:
        MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 0;
      }
    } else {
      MrcData->DynamicVars[Channel][TQDATAPUSHEN] = 1;
    }
  }

 // MrcData->DynamicVars[Channel][MCHBAR]  = MCHBAR_BASE;
 // MrcData->DynamicVars[Channel][P2SBBAR] = P2SB_BASE;

  MrcData->TwoXps = Timing2x[MrcData->Channel[Channel].CurrentFrequency];

  if (MrcData->CpuStepping < STEPPING_E_C0) {
    MrcData->DynamicVars[Channel][LPMODE_IOBUFACTOVRD_VAL] = 1;
  } else {
    MrcData->DynamicVars[Channel][LPMODE_IOBUFACTOVRD_VAL] = 0;
  }
#if defined BXT_PO && (BXT_PO == 1)
  MmrcDebugPrint ((MMRC_DBG_MIN, "\x1b[31;6m BXT_PO BUILD\n \x1b[0m"));
#endif
  if (CurrentConfiguration == C_PO) {
    MmrcDebugPrint ((MMRC_DBG_MIN, "\x1b[31;6m CurrentConfiguration == C_PO\n \x1b[0m"));
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
SetTpstmrblk (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
/*++

Routine Description:

  Sets the dynamic variable "TPSTMRBLK" based on the register value of RDCMD2RDVLD.
  Must be executed after "modmem_init_lpddrgrp3m_seq" init task

Arguments:

  MrcData:   Host structure for all MRC global data.

Returns:

  Success
  Failure

--*/
{
  // (rdcmd2rdvld + 4) * 4
  MmrcExecuteTask (MrcData, RDCMD2RDVLD_READ, NO_PRINT, Channel);
  MrcData->DynamicVars[Channel][TPSTMRBLK] = ((UINT32) MrcData->DynamicVars[Channel][RDCMD2RDVLD] + 4) * 4;
  return MMRC_SUCCESS;
}

MMRC_STATUS
OverrideTimingParams (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
/*++

Routine Description:

  Override Timing parameter based on user input/Setup option

Arguments:

  MrcData:   Host structure for all MRC global data.

Returns:

  Success
  Failure

--*/
{

  return MMRC_SUCCESS;
}


MMRC_STATUS
Channel10Only (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT8 loop;

  for (loop = 0; loop < MAX_CHANNELS; loop++) {
    if (MrcData->Channel[Channel].DramType == TypeLpDdr3 ) {
      if (loop == 2) {
        MrcData->Channel[loop].Enabled        = TRUE;
      } else {
        MrcData->Channel[loop].Enabled        = FALSE;
      }
    } else  // LPDDR4
    {
      if (loop == 1) {
        MrcData->Channel[loop].Enabled        = TRUE;
      } else {
        MrcData->Channel[loop].Enabled        = FALSE;
      }
    }
  }
  return MMRC_SUCCESS;
}

const UINT8 Mrr8DensityTableLpddr4[] = {4, 6, 8, 12, 16, 24, 32};//in Gb
const UINT8 Mrr8DensityTableLpddr3[] = {4, 8, 16, 32};//in Gb

VOID
DisplayMrr8Data (
  MMRC_DATA         *MrcData,
  UINT32            MemType,
  UINT32            Mrr8Data
)
{
  MRR8 Mrr8;

  Mrr8.Data = Mrr8Data;
  switch(MemType){
  case TypeLpDdr4:
    MmrcDebugPrint((MMRC_DBG_MIN, "Type: "));
    if (Mrr8.Bits.Type == 0) {
      MmrcDebugPrint((MMRC_DBG_MIN, "S16 SDRAM\n"));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    MmrcDebugPrint((MMRC_DBG_MIN, "Density: "));
    if (Mrr8.Bits.Density <= 0x6) {
      MmrcDebugPrint((MMRC_DBG_MIN, "%d\n", Mrr8DensityTableLpddr4[Mrr8.Bits.Density]));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    MmrcDebugPrint((MMRC_DBG_MIN, "I/O Width: "));
    if (Mrr8.Bits.IOWidth == 0) {
      MmrcDebugPrint((MMRC_DBG_MIN, "16\n"));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    break;
  case TypeLpDdr3:
    MmrcDebugPrint((MMRC_DBG_MIN, "Type: "));
    if (Mrr8.Bits.Type == 0x3) {
      MmrcDebugPrint((MMRC_DBG_MIN, "S8 SDRAM\n"));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    MmrcDebugPrint((MMRC_DBG_MIN, "Density: "));
    if ((Mrr8.Bits.Density >= 0x6) && (Mrr8.Bits.Density < 0xA)) {
      MmrcDebugPrint((MMRC_DBG_MIN, "%d\n", Mrr8DensityTableLpddr3[Mrr8.Bits.Density - 6]));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    MmrcDebugPrint((MMRC_DBG_MIN, "I/O Width: "));
    if (Mrr8.Bits.IOWidth == 0) {
      MmrcDebugPrint((MMRC_DBG_MIN, "x32\n"));
    } else if (Mrr8.Bits.IOWidth == 1) {
      MmrcDebugPrint((MMRC_DBG_MIN, "x16\n"));
    } else {
      MmrcDebugPrint((MMRC_DBG_MIN, "UNKNOWN\n"));
    }
    break;
  default:
     MmrcDebugPrint((MMRC_DBG_MIN, "Unknown memory technology %d\n", MemType));
    break;
  }
};

MMRC_STATUS
DisplayLpddrInfo (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
  UINT32 Mrr5;
  UINT32 Mrr6;
  UINT32 Mrr7;
  UINT32 Mrr8;

  Mrr5      = ReadMRValue(MrcData, Channel, 5);
  Mrr5      = DeSwizzle(MrcData, Channel, Mrr5);
  PerformFifoReset(MrcData, Channel, 0);
  Mrr6      = ReadMRValue(MrcData, Channel, 6);
  Mrr6      = DeSwizzle(MrcData, Channel, Mrr6);
  PerformFifoReset(MrcData, Channel, 0);
  Mrr7      = ReadMRValue(MrcData, Channel, 7);
  Mrr7      = DeSwizzle(MrcData, Channel, Mrr7);
  PerformFifoReset(MrcData, Channel, 0);
  Mrr8      = ReadMRValue(MrcData, Channel, 8);
  Mrr8      = DeSwizzle(MrcData, Channel, Mrr8);
  PerformFifoReset(MrcData, Channel, 0);

  MmrcDebugPrint((MMRC_DBG_MIN, "CH: %d, Manufacturer ID: 0x%x, Rev ID1: %d, Rev ID2: %d\n",Channel, Mrr5, Mrr6, Mrr7));
  DisplayMrr8Data(MrcData, MrcData->Channel[Channel].DramType, Mrr8);

  return MMRC_SUCCESS;
}

MMRC_STATUS
InitializeJedec (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
/*++

Routine Description:

  Initializes all dynamic variables used in JEDEC init.

Arguments:

  MrcData:   Host structure for all MRC global data.
  Channel:   Current Channel being examined.

Returns:

  Success
  Failure

--*/
{
  UINT8   Rank;
  UINT32  TempValue;
  UINT32  MR1;
  UINT32  MR2;
  UINT32  MR3;
  UINT8   SaveDebugMsgLevel;

  SaveDebugMsgLevel = MrcData->MrcDebugMsgLevel;
  MrcData->MrcDebugMsgLevel = MMRC_DBG_VERBOSE;

  MR1 = 0;
  MR2 = 0;
  MR3 = 0;

#if ODT_TABLE
  //
  // Set ODT Matrix
  //
  SetOdtMatrix (MrcData, 0, Channel, MrsValue);
#endif

  //
  // Fill in specifics based on the memory technology behind this DUNIT.
  //
  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
#if LPDDR3_SUPPORTED==1

    //
    // Initialize RANK0/RANK1 MR1/2/3 registers.
    //
    MR1 = LPDDR3_JEDEC_MR01;
    MR2 = LPDDR3_JEDEC_MR02;
    MR3 = LPDDR3_JEDEC_MR03;

    for (Rank = 0; Rank < MAX_RANKS; Rank++) {
      if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
        continue;
      }
      MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
      MrcData->DynamicVars[Channel][RDDCMD]      = 0;        // This is a MRW command.
      MrcData->DynamicVars[Channel][RANK]        = Rank;            // Must set the rank to the
      MrcData->DynamicVars[Channel][REG_DATA]    = LPDDR3_JEDEC_CMD_NOP;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM
      MySimStall(200);
#else
      MrcDelay (MrcData, MICRO_DEL, 200);
#endif

      MrcData->DynamicVars[Channel][REG_DATA]    = LPDDR3_JEDEC_CMD_MRW_RESET;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM
      MySimStall(50);
#else
      MrcDelay (MrcData, MICRO_DEL, 50);
#endif
      MrcData->DynamicVars[Channel][REG_DATA]    = LPDDR3_JEDEC_CMD_MRW_ZQCAL; //00FF0A0
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM
      MySimStall(150);
      MySimStall(2000);
#else
      MrcDelay (MrcData, MICRO_DEL, 150);
#endif
      //
      // LP3 is always BL8.
      //
      MR1 |= (LPDDR3_JEDEC_MR1_BL8 << 12);
      if (GetTimingParam(BxtTimingTwr)> 9) {
        MR1 |= ((GetTimingParam(BxtTimingTwr) - 10) << 17);

        //
        //Enable nWR Programming > 9
        //
        MR2 |= (LPDDR3_JEDEC_MR2_WRE_GT9 << 12);
      } else {
        MR1 |= ((GetTimingParam(BxtTimingTwr) - 2) << 17);
      }

      //
      // Subtract 2 from TCL to match LP spec.
      //
      MR2 |= ((GetTimingParam(BxtTimingTcl) - 2) << 12);
      //
      // Set the WL Select based on timing params. This matches the RTL sim test bench's implementation.
      //
      if (GetTimingParam(BxtTimingTcwl) > 8 || (GetTimingParam(BxtTimingTcl) == 10 && GetTimingParam(BxtTimingTcwl) == 8)) {

    //
      // Select WL Set B
      //
      MR2 |= (LPDDR3_JEDEC_MR2_WL_SETB << 12);
      }
      //
      // 40 ohm.
      //
      MR3 |= (LPDDR3_JEDEC_MR3_OHM_40 << 12);

      MrcData->DynamicVars[Channel][REG_DATA]    = MR2;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
#if SIM
    MySimStall(50);
#else
    MrcDelay (MrcData, MICRO_DEL, 50);
#endif

    MrcData->DynamicVars[Channel][REG_DATA]    = MR1;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM || JTAG
    MySimStall(50);
#else
    MrcDelay (MrcData, MICRO_DEL, 50);
#endif
    MrcData->DynamicVars[Channel][REG_DATA]    = MR3;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
  }
#endif
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {

    MySimStall(50);
    MySimStall(2000);

    //
    // Initialize RANK0/RANK1 MR1/2/3 registers.
    //
    MR1 = 0x16046;
    MR2 = 0x16086;
    MR3 = 0x160C6;

    for (Rank = 0; Rank < MAX_RANKS; Rank++) {
      if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }

     //BL
#if 0//SIM
      MR1 |= (2 << 18);  //on the fly, need to confirm
#else
    //Fail on LP4 Dunit Sim Run
    MR1 |= ((2 << 18) | (1 << 20) );  //set to BL32[op0:1], WPRE=2*tck[op2]
#endif

      MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
      MrcData->DynamicVars[Channel][RDDCMD]      = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK]        = Rank;            // Must set the rank to the

      //nWR [op 4,5,6] in data[22,23,17]
      //TempValue = (UINT32)(GetTimingParam(BxtTimingTwr) - 6) / 4;
      /*
      if (TempValue >= 4) {
        MR1 |= (1 << 17);     //op[6] = data[17]
      }

      if (TempValue == 4) {
      TempValue = 0;
      }

      if (TempValue > 5) {
      TempValue -= 5;
      }
      */
      //MR1 |= (TempValue << 22);  //op[4,5] = data[22,23]


    //from jedec rtl
 //   MrcData->DynamicVars[Channel][MR1_0] |= (1 << 17);
 //   MrcData->DynamicVars[Channel][MR1_0] |= (1 << 22);

      MR2 |= ((GetTimingParam(BxtTimingTcl) - 6) / 4) << 18;  //DBI-RD disable

      /*
      if (GetTimingParam(BxtTimingTcwl) > 18) {  // tCWL values over 18 require setB for WLS op[6]
      TempValue = 1;
      } else {
      TempValue = 0;
      } */
      TempValue = 1;             // WLS always set B for BXT
      MR2 |= TempValue << 17;    //WLS

      MR2 |= TempValue ? (((GetTimingParam(BxtTimingTcwl) - 3) / 4) ) << 21:  // WL Set "B" formula
      ((GetTimingParam(BxtTimingTcwl) - 4) / 2) << 21 ;  //  WL Set "A" formula

      MR3 |= 0x31 << 18;    //PUCAL op[0], PDDS op[5:3] default value

      MrcData->DynamicVars[Channel][REG_DATA]    = MR1;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
#if SIM
    MySimStall(50);
    MySimStall(2000);
#else
    MrcDelay (MrcData, MICRO_DEL, 50);
#endif

    MrcData->DynamicVars[Channel][REG_DATA]    = MR3;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM
    MySimStall(50);
      MySimStall(2000);
#else
    MrcDelay (MrcData, MICRO_DEL, 50);
#endif
    MrcData->DynamicVars[Channel][REG_DATA]    = MR2;
      SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

#if SIM
    MySimStall(50);
      MySimStall(2000);
#else
    MrcDelay (MrcData, MICRO_DEL, 50);
#endif
      /*
      // send mr23 ~500ns in 16 dram clk
      //
      TempValue = (500 * 1000 ) / TCK[MrcData->CurrentFrequency] / 16;
      MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(23, TempValue);
        SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
        MySimStall(2000); */
    }
  } else if (MrcData->Channel[Channel].DramType == TypeWio2) {
  } else {
  }

  MrcData->MrcDebugMsgLevel = SaveDebugMsgLevel;

  MrcData->MR1_Value = MR1;
  MrcData->MR2_Value = MR2;
  MrcData->MR3_Value = MR3;

  return MMRC_SUCCESS;
}

MMRC_STATUS
  LP4MRProgramming_MPCZQ (
  IN  OUT MMRC_DATA   *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel)
{
  UINT32 Rank;
  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }
    //
    // Issue the MPC-1 command ZQCal start and Latch
    //

    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;
    MrcData->DynamicVars[Channel][RDDCMD]      = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK]        = Rank;            // Must set the rank to the

    MrcData->DynamicVars[Channel][REG_DATA]    = MPC2(0, 0x4F);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    MySimStall(100);

    MrcData->DynamicVars[Channel][REG_DATA]    = MPC2(0, 0x51);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 50);

    MySimStall(100);
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
  LP4AllMRProgramming (
  IN  OUT MMRC_DATA   *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel)
{
  UINT32 MRValue;
  UINT8 Rank;

  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }

    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][RDDCMD]      = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK]        = Rank;            // Must set the rank to the

    MRValue = 0;
  MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(4, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0;
  MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(10, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    if (MrcData->CurrentFrequency == MMRC_1600) {
      MRValue = 0;
    } else {
      MRValue = 0x4;
    }
  MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(11, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0x60;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(12, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 0x30;
    } else {
      MRValue = 0x10;
    }

    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(13, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0x60;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(14, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0x00;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(16, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0x00;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(17, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 0x05;
    } else {
      MRValue = 0x04;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(22, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 0x00;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(24, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    //Restoring CommandClock values for FB path
    GetSetTxCaVref (MrcData, 0, Channel, 0, Rank, 0, 0, 0, 0, CMD_GET_REG, &MRValue);
    GetSetTxCaVref (MrcData, 0, Channel, 0, Rank, 0, 0, 0, 0, FC_WR, &MRValue);
    //MmrcDebugPrint ((MMRC_DBG_MIN, "CA VREF %d\n", Value));
    GetSetTxVref (MrcData, 0, Channel, 0, Rank, 0, 0, 0, 0, CMD_GET_REG, &MRValue);
    GetSetTxVref (MrcData, 0, Channel, 0, Rank, 0, 0, 0, 0, FC_WR, &MRValue);
    //MmrcDebugPrint ((MMRC_DBG_MIN, "DQ VREF %d\n", Value));
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
  LP4MRProgramming_ODTVREF (
  IN  OUT MMRC_DATA   *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel)
{
  UINT32 MRValue;
  UINT32 Rank;

  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }

    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][RDDCMD]      = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK]        = Rank;            // Must set the rank to the

    //
    // Step 4.0, MR3[0]=1 (set during jedecinit), MR22[2:0]=100b, MR13[6]=1, MR3[0]=1, MR22[2:0] = 100b
    //

    MRValue = 1;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(3, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    MySimStall(50);

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 5;       //bit 5: data mask disable
    } else {
      MRValue = 4;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(22, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 0x70;
    } else {
      MRValue = 0x50;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(13, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    MRValue = 1;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(3, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 5;       //bit 5: data mask disable
    } else {
      MRValue = 4;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(22, MRValue);  //586
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    MySimStall(1000);
    MySimStall(1000);
    //
    // Issue the MPC-1 command ZQCal start and Latch
    //
    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 0;            //MPC before training is BL16
    MrcData->DynamicVars[Channel][REG_DATA]    = MPC2(0, 0x4F);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 50);

    MySimStall(1000);
    MySimStall(1000);
    MySimStall(1000);


    MrcData->DynamicVars[Channel][REG_DATA]    = MPC2(0, 0x51);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 50);

    MySimStall(1000);
    MySimStall(1000);
    //
    //  Step 5.0
    //
    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 0x70;       //bit 5: data mask disable
    } else {
      MRValue = 0x50;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(13, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    MySimStall(50);

#if defined LDO_TRAINING && (LDO_TRAINING == 1)
    MRValue = 0xA6;
#else
    MRValue = 0x60; // 0x72;
#endif

    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(12, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    if (MrcData->CurrentFrequency == MMRC_1600) {
      MRValue = 0;
    } else {
      MRValue = 0x4;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(11, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
  }  //end of for rank

  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }

    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][RDDCMD] = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK] = Rank;            // Must set the rank to the

    // need to do for all ranks at the same time.
    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 5;       //bit 5: data mask disable
    } else {
      MRValue = 4;
    }

    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(22, MRValue);    //need bit 3:5
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
  } // for rank

  //
  //  Step 6.0
  //
  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }

    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][RDDCMD] = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK] = Rank;            // Must set the rank to the

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 0x30;       //bit 5: data mask disable
    } else {
      MRValue = 0x10;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(13, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
    MySimStall(50);

#if defined LDO_TRAINING && (LDO_TRAINING == 1)
    MRValue = 0xA6;
#else
    MRValue = 0x60; // 0x72;
#endif

    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(12, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);

    MySimStall(50);

    MRValue = 0;
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(11, MRValue);
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
  }

  for (Rank = 0; Rank < MAX_RANKS; Rank++) {
    if (MrcData->Channel[Channel].RankEnabled[Rank] == 0) {
      continue;
    }
    MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
    MrcData->DynamicVars[Channel][RDDCMD] = 0;        // This is a MRW command.
    MrcData->DynamicVars[Channel][RANK] = Rank;            // Must set the rank to the

    if (MrcData->CpuStepping == STEPPING_P0) {
      MRValue = 5;       //bit 5: data mask disable
    } else {
      MRValue = 4;
    }
    MrcData->DynamicVars[Channel][REG_DATA] = MRW_LP4(22, MRValue);    //need bit 3:5
    SendDramCmd(MrcData, Channel); //MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    MrcDelay(MrcData, NANO_DEL, 10);
  } // end of rank

  return MMRC_SUCCESS;
}

/**
Find the common timing for MD+SODIMM configuration

@param[in]  MrcData       Host structure for all data related to MMRC

@retval  Success
@retval  Failure
**/
MMRC_STATUS
  DetermineCommonTiming (
  IN  OUT   MMRC_DATA            *MrcData
  )
{

  return MMRC_SUCCESS;
}

/**
Find the common freq

@param[in]  MrcData       Host structure for all data related to MMRC

@retval  Success
@retval  Failure
**/
MMRC_STATUS
  DetermineCommonFreq (
  IN  OUT   MMRC_DATA            *MrcData
  )
{
  UINT8 Channel;
  UINT8 TimingParamVar;

  TimingParamVar = TimingParamHash (MrcData->DramPolicyData.Profile -1, TRUE);
  if (MrcData->DramPolicyData.MemoryDown == 1) {
    //MrcData->Channel[Channel].DimmCount = 1;
    MrcData->CurrentFrequency = ConfigToSpeed[TimingParamVar];

  } else if (MrcData->DramPolicyData.MemoryDown == 2){
    // CurrentFrequency is from dimm, DramPolicyData.Profile is MD
    // Use lower freq
    if (MrcData->CurrentFrequency > ConfigToSpeed[TimingParamVar]) {
      MrcData->CurrentFrequency = ConfigToSpeed[TimingParamVar];
    }

    //Set back because might be overwrite by the SMIP
    if (MrcData->Channel[1].SpdData[0].SpdPresent) {
      MrcData->Channel[1].Enabled = 1;
    }
  }

  for (Channel = 0; Channel < MAX_CHANNELS; Channel++) {
    if (!MrcData->Channel[Channel].Enabled ) continue;

    MrcData->Channel[Channel].CurrentFrequency = (1 << MrcData->CurrentFrequency);
  }

  return MMRC_SUCCESS;
}

/*
soc.ddr.ddrio.mcast0.dqccc1.dpic_ch0_pmswmsg = 0x25
p2sb+0xf20c48

soc.ddr.ddrio.mcast0.dqccc1.dpic_ch1_pmswmsg = 0x25
p2sb+0xf20c6c

soc.ddr.ddrio.mcast0.dqccc1.initcomplete = 3
p2sb+0xf20c00

soc.ddr.ddrio.mcast0.dqccc1.dpic_cripg_ctl.cripgen = 1
p2sb+0xf20c0c
*/
MMRC_STATUS wiogrp6(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
)
{
#if (!defined (SIM) || (SIM == 0)) && (!defined (MINIBIOS) || (MINIBIOS == 0)) && (!defined (JTAG) || (JTAG == 0))
  UINT32    BaseAddr;
  UINT8     port;

  //KWU how are we going to handle the steppings across A/P/B ?  TODO fix this
  if (MrcData->CpuStepping <= 1) {
    BaseAddr = MrcData->P2sbBar;
    MmrcDebugPrint ((MMRC_DBG_MIN, "<> Disabling WIO PM <>\n"));
    for (port = 0x70; port < 0x74; port++) {
      Mmio32Write ((BaseAddr + (port << 16) + 0xc48), 0x25);
      Mmio32Write ((BaseAddr + (port << 16) + 0xc6c), 0x25);
      Mmio32Write ((BaseAddr + (port << 16) + 0xc00), 0x3);
      Mmio32Write ((BaseAddr + (port << 16) + 0xc0c), 0x1);
    }
  }
#endif
  return MMRC_SUCCESS;
};

UINT16
MirrorAddrBits (
  IN    UINT16 data
)
/*++

Routine Description:

  Mirrors address bits

Arguments:

  data  - Data for MRS write

Returns:

  dataNew - Mirrored data

--*/
{
  UINT16  dataNew;

  dataNew = data & ~0x1F8;
  //
  // Swap of bits (3,4), (5,6), (7,8)
  //
  dataNew |= ((data & BIT3) << 1) | ((data & BIT4) >> 1) | ((data & BIT5) << 1) | ((data & BIT6) >> 1) | ((data & BIT7) << 1) | ((data & BIT8) >> 1);

  return dataNew;
} // MirrorAddrBits

UINT8
MirrorBankBits (
  UINT8 bank
)
/*++

Routine Description:

  Mirrors bank address bits

Arguments:

  bank  - Bank address to be mirrored

Returns:

  bankNew - Mirrored bank address

--*/
{
  UINT8 bankNew;

  bankNew = bank & ~0x3;
  //
  // Swap bits 0 and 1 of the bank address
  //
  bankNew = ((bank & BIT0) << 1) | ((bank & BIT1) >> 1);

  return bankNew;
} // MirrorBankBits

VOID
HandleAddressMirroring (
  IN  OUT   UINT32 *Data,
  IN        UINT8   AddressMirroringSpdByte
)
{
  DramInitMisc MiscCommand;

  MiscCommand.Data = *Data;

  if (AddressMirroringSpdByte & 1) {
    if (MiscCommand.Bits.rankSelect & BIT0) {
      MiscCommand.Bits.multAddress  = (UINT16) MirrorAddrBits ((UINT16) MiscCommand.Bits.multAddress);
      MiscCommand.Bits.bankAddress  = (UINT8) MirrorBankBits ((UINT8) MiscCommand.Bits.bankAddress);
    }
  }
  *Data = MiscCommand.Data;
}


VOID
WriteMrsCommand (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT8         Rank,
  IN        UINT32        MiscCommand
)
{
  MrcData->DynamicVars[Channel][RANK]        = Rank;
  MrcData->DynamicVars[Channel][RDDCMD]      = 0;
  MrcData->DynamicVars[Channel][DRAM_CMD_BL] = 1;
  MrcData->DynamicVars[Channel][REG_DATA]    = MiscCommand;
  SendDramCmd(MrcData, Channel);
#if 0
  HandleAddressMirroring (&MiscCommand, MrcData->Channel[Channel].SpdData[Rank / MAX_RANKS_PER_DIMM].Buffer[SPD_DDR3_ADD_MAPPING]);
  MrcData->DynamicVars[Channel][REG_DATA] = MiscCommand;
  MmrcExecuteTask (MrcData, SEND_DRAM_INIT, NO_PRINT, Channel);
#endif
}

UINT32
ReadMrsCommand (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
  IN        UINT8         Rank,
  IN        UINT8         Index
)
{
  DramInitMisc  MiscCommand;

  MiscCommand.Data = (UINT32) MrcData->DynamicVars[Channel][MrsCommandIndex[Rank][Index]];
#if 0
  HandleAddressMirroring (&MiscCommand.Data, MrcData->Channel[Channel].SpdData[Rank / MAX_RANKS_PER_DIMM].Buffer[SPD_DDR3_ADD_MAPPING]);
#endif
  return MiscCommand.Data;
}

UINT32
GetAddress (MMRC_DATA *MrcData, UINT8 Channel, UINT8 Rank)
{
  return (UINT32)(Rank << RANK_SHIFT_BIT);
}


VOID
CheckpointSet (
  IN        UINT16    Content
)
/*++

Routine Description:

  Output a progress data to port 80 for debug purpose
  Could be safely overridden to
  send checkpoints elsewhere, such as port 0x84 or a serial port

Arguments:

  Content:    Hexadecimal value to be sent thru debug port


Returns:

  None

--*/
{
  IoOut32 (0x80, (UINT8) Content);
}
/*
MMRC_STATUS
DumpDdrioRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
)
{
  UINT16 Offset;
  UINT32 Value;
  REGISTER_ACCESS Register;

  Register.Mask   = 0xFFFFFFFF;

  MmrcDebugPrint ((MMRC_DBG_MIN, "DDRIO CH%d\n", Channel));

  for (Offset = 0; Offset < 0x8000; Offset += 4) {
    Register.Offset = Offset;
    Value = (UINT32) MemRegRead (DDRDQ, Channel, 0, Register);
    MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
  }
  return MMRC_SUCCESS;
}
*/
MMRC_STATUS
DumpDunitRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
)
{
  UINT16 Offset;
  UINT32 Value;
  REGISTER_ACCESS Register;

  Register.Mask   = 0xFFFFFFFF;

  MmrcDebugPrint ((MMRC_DBG_MIN, "DUNIT%d\n", Channel));

  for (Offset = 0; Offset < 0x184; Offset+=4) {
    Register.Offset = Offset;
    Value = (UINT32) MemRegRead (DUNIT, Channel, 0, Register);
    MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
  }
  return MMRC_SUCCESS;
}
/*
MMRC_STATUS
DumpCpgcRegisters (
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT8      Channel
)
{
  UINT16 Offset;
  UINT32 Value;
  REGISTER_ACCESS Register;

  Register.Mask   = 0xFFFFFFFF;

  MmrcDebugPrint ((MMRC_DBG_MIN, "CPGC%d\n", Channel));

  for (Offset = 0; Offset < 0xFF; Offset++) {
    Register.Offset = Offset;
    Value = (UINT32) MemRegRead (CPGC, Channel, 0, Register);
    MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
DumpBunitRegisters (
  IN  OUT   MMRC_DATA *MrcData
)
{
  UINT16 Offset;
  UINT32 Value;
  REGISTER_ACCESS Register;

  Register.Mask   = 0xFFFFFFFF;

  MmrcDebugPrint ((MMRC_DBG_MIN, "BUNIT\n"));

  for (Offset = 0; Offset < 0xFF; Offset++) {
    Register.Offset = Offset;
    Value = (UINT32) MemRegRead (BUNIT, 0, 0, Register);
    MmrcDebugPrint ((MMRC_DBG_MIN, "0x%08x 0x%08x\n", Register.Offset, Value));
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
DumpRegs (
  IN  OUT   MMRC_DATA   *MrcData
)
{
  UINT8 i;
  DumpBunitRegisters (MrcData);
  for (i = 0; i < MAX_DUNITS; i++)
  {
    if (MrcData->Channel[i].Enabled)
    {
      DumpDunitRegisters (MrcData, i);
    }
  }
  for (i = 0; i < MAX_CHANNELS; i++) {
    if (MrcData->Channel[i].Enabled) {
      DumpDdrioRegisters (MrcData, i);
      DumpCpgcRegisters (MrcData, i);
    }
  }
  return MMRC_SUCCESS;
}
*/
MMRC_STATUS
BreakStrobeLoop (
  IN   MMRC_DATA    *MrcData,
  IN   UINT8         Channel,
  IN   UINT8         Rank,
  IN   UINT16        Dim1Index,
  IN   UINT8        *Strobe
){
  //
  // For the CMD, CTL group signals break right at the first strobe
  //
  if ((IsCmdSignalDelay (MrcData, Dim1Index) ||
    Dim1Index == TxVref ||
    Dim1Index == CkAll || Dim1Index == CkGrp0 || Dim1Index == CkGrp1 ||
    Dim1Index == CtlAll || Dim1Index == CtlGrp0 || Dim1Index == CtlGrp1 ||
    Dim1Index == CmdAll ||Dim1Index == CmdGrp0 || Dim1Index == CmdGrp1 ) && (INT8)(*Strobe) >= 0) {
      *Strobe = MrcData->Channel[Channel].MaxDq[Rank];
  }
  //
  // Different projects can add different conditions per delay that uses the same per strobe loop
  //
  return MMRC_SUCCESS;
}

UINT32
SignalUpperBound (
  IN         MMRC_DATA    *MrcData,
  IN         UINT8        Channel,
  IN         UINT8        Rank,
  IN         UINT8        Dim1Index
)
{
#if 0
  UINT32    MaxLimit;
  UINT32    WaitTime;

  GetDataGroupLimits (MrcData, 0, Channel, 0, Rank, 0, 0, 0, Dim1Index, &MaxLimit, &WaitTime);
  //
  // Return the linear value as maximum in case it exists
  //
  switch (Dim1Index) {
  case RX_VREF:
    MaxLimit = MAX_ELEMENT_TABLE - 1;
    break;
  case TX_VREF:
    MaxLimit = ConvertPhysicalToLinearValue (MrcData, Dim1Index, MaxLimit);
    break;
  default:
    break;
  }
  return MaxLimit;
#endif
  return 0;
}

#if PASS_GATE_TEST == 1
MMRC_STATUS
PassGateTestEntryHooks (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel,
      OUT   UINT8         *Swizzle
)
/*++

Routine Description:

  Configures steps prior to execute Pass Gate Test


Arguments:

  MrcData:          Host structure for all data related to MMRC
  Channel:          Current Channel being examined.
  Swizzle:          Pointer to define if Swizzle or Sequencial

Returns:

  Success
  Failure

--*/
{
  DRP_DUNIT_STRUCT    Drp;
  UINT8               Dimm;

  //
  // Enable the CPGC Engine.
  //
  MmrcExecuteTask (MrcData, CPGC_ENABLE, NO_PRINT, Channel);

  //
  // Determine if Swizzle or Sequential mode in each Dimm
  //
  for (Dimm = 0; Dimm < MAX_DIMMS; Dimm++) {
    Swizzle[Dimm] = MrcData->PassGateParameters.SwizzleMode;
    if (MrcData->Channel[Channel].SamsungDram[Dimm] && MrcData->PassGateParameters.SwizzleMode == 0) {
      Swizzle[Dimm] = 1;
    }
  } // Dimm loop ...

  MmrcExecuteTask (MrcData, PASS_GATE_SAVE_ENTRY, NO_PRINT, Channel);

  Drp.Data = MrcData->DynamicVars[Channel][PG_RESTORE_DRP];
  //
  // Drp setup
  //
  MrcData->DynamicVars[Channel][PG_DIMM1_SEQUENTIAL] = (~Swizzle[1]) & 1;
  MrcData->DynamicVars[Channel][PG_DIMM0_SEQUENTIAL] = (~Swizzle[0]) & 1;
  //
  // DMAP0
  //
  if (Drp.Bits.dimmdden0 == 3) {
    MrcData->DynamicVars[Channel][PG_BNKSEL]  = 3;
  } else {
    MrcData->DynamicVars[Channel][PG_BNKSEL]  = 2;
  }
  MrcData->DynamicVars[Channel][PG_DIMMSEL]   = 6 + Drp.Bits.dimmdden0;
  MrcData->DynamicVars[Channel][PG_RNKSEL]    = 5 + Drp.Bits.dimmdden0;
  //
  // DMAP1
  //
  if (Drp.Bits.dimmdden0 == 3) {
    MrcData->DynamicVars[Channel][PG_COL11SEL] = 0;
  } else {
    MrcData->DynamicVars[Channel][PG_COL11SEL] = 0x1f;
  }
  //
  // DMAP2
  //
  if (Drp.Bits.dimmdden0 == 3) {
    MrcData->DynamicVars[Channel][PG_ROW_0_7SEL] = 6;
  } else {
    MrcData->DynamicVars[Channel][PG_ROW_0_7SEL] = 5;
  }
  //
  // DMAP3
  //
  if (Drp.Bits.dimmdden0 == 3) {
    MrcData->DynamicVars[Channel][PG_ROW_8_13SEL] = 6;
  } else {
    MrcData->DynamicVars[Channel][PG_ROW_8_13SEL] = 5;
  }
  if (Drp.Bits.dimmdden0 == 0) {
    MrcData->DynamicVars[Channel][PG_ROW_14SEL] = 0xf;
  } else {
    MrcData->DynamicVars[Channel][PG_ROW_14SEL] = MrcData->DynamicVars[Channel][PG_ROW_8_13SEL];
  }
  if (Drp.Bits.dimmdden0 < 2) {
    MrcData->DynamicVars[Channel][PG_ROW_15SEL] = 0xf;
  } else {
    MrcData->DynamicVars[Channel][PG_ROW_15SEL] = MrcData->DynamicVars[Channel][PG_ROW_8_13SEL];
  }
  //
  // DRFC
  //
  if (MrcData->PassGateParameters.EnablePeriodicSelfRefresh) {
    MrcData->DynamicVars[Channel][PG_SELF_REFRESH] = 1;
  } else {
    MrcData->DynamicVars[Channel][PG_SELF_REFRESH] = 0;
  }

  MmrcExecuteTask (MrcData, PASS_GATE_SET_ENTRY, NO_PRINT, Channel);

  return MMRC_SUCCESS;
}

MMRC_STATUS
PassGateTestExitHooks (
  IN  OUT   MMRC_DATA     *MrcData,
  IN        UINT8         Channel
)
/*++

Routine Description:

  Configures steps after execute Pass Gate Test


Arguments:

  MrcData:          Host structure for all data related to MMRC
  Channel:          Current Channel being examined.

Returns:

  Success
  Failure

--*/
{

  MmrcExecuteTask (MrcData, PASS_GATE_EXIT, NO_PRINT, Channel);

  return MMRC_SUCCESS;
}
#endif

MMRC_STATUS
ProgramCKE2xControl (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
#if TRAINING_ALGOS==1 && PUSHOUT_CCC==1
  UINT8  Rank;
  UINT8  CccLoop;
  UINT32 CCCVal;
  UINT32 MinCCC = 0xffffffff;
  UINT8  CkeDiff=1; //Difference between Min CCCs vs CKE Value (Check with DE)


  //
  // Loop on all ranks incrementing those ranks that are enabled.
  //
  for (Rank = 0 ; Rank < MAX_RANKS; Rank++) {
    if (!RunOnThisChannel (MrcData, Channel, Rank)) {
      continue;
    }
    MinCCC = 0xffffffff;
    for (CccLoop = 0; CccLoop < MAX_CMDS; CccLoop++) {
      GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CmdGrp0 + CccLoop, CMD_GET_REG_UC, &CCCVal);
      if (CCCVal < MinCCC)
          MinCCC = CCCVal;
      GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CtlGrp0 + CccLoop, CMD_GET_REG_UC, &CCCVal);
      if (CCCVal < MinCCC)
          MinCCC = CCCVal;
        GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CkGrp0 + CccLoop, CMD_GET_REG_UC, &CCCVal);
      if (CCCVal < MinCCC)
          MinCCC = CCCVal;
    } // for
    MinCCC = MinCCC / GetHalfClk(MrcData, Channel);
    MinCCC += CkeDiff;
    //
    // Increment the Command knobs.
    //
  for (Rank=0; Rank < MAX_RANKS; Rank++)
  {
      for (CccLoop = 0; CccLoop < MrcData->Channel[Channel].MaxDq[Rank]; CccLoop++) {
        GetSetDdrioGroup2 (MrcData, Channel, Rank, CccLoop, CkeAll, CMD_SET_VAL_FC_UC, &MinCCC);
        MmrcDebugPrint ((MMRC_DBG_MAX, "FBs:CH%02d RK%02d ST%02d CKE_2x:0x%08x\n", Channel, Rank, CccLoop, MinCCC));
      } // for CccLoop
  }
  } // for Rank
#endif // TRAINING_ALGOS==1 && PUSHOUT_CCC==1
  return MMRC_SUCCESS;
}

/**

  This function extracts the required MRC training data from
  MmrcData->Channel_V[].CachedValues and copies it to a smaller array in the passed in
  pointer CachedValuesHobPointer

  @param[in,out]  MmrcData               Pointer to MmrcData where data will be saved
                                         from (Task == SAVE) or restored to
                                         (Task == RESTORE).
  @param[in,out]  CachedValuesHobPointer Pointer to external data HOB. This pointer will
                                         hold the restore data when Task == RESTORE and
                                         will be the output buffer when TASK == SAVE
  @param[in]      Task                   Flag indicating whether MmrcData should be
                                         saved or restored.

**/
#if 0
VOID
SaveRestoreCachedValues(
  IN OUT MMRC_DATA         *MmrcData,
  IN OUT UINT8             *CachedValuesHobPointer,
  IN     SAVE_RESTORE_TASK Task
)
{
  //UINT32(*CachedValues)[][MAX_RANKS][MAX_STROBES];
  UINT32(*CachedValues)[][MAX_STROBES];
  UINT8  Channel;
  UINT8  Element;
  UINT8  Rank;
  UINT8  Strobe;
  UINT8  Frequency;
  UINT8  RankTableIndex;
  UINT8  NumFrequencies;
  MemDebugPrint((SDBG_MIN, "Input Address = %x\n", CachedValuesHobPointer));

  RankTableIndex = 0;
  NumFrequencies = GetNumberOfFrequenciesEnabled(MmrcData);

  for (Channel = 0; Channel < MAX_CHANNELS; Channel++){
    if (MmrcData->Channel[Channel].Enabled == 0) {
      // Skip this iteration if the channel is disabled
      continue;
    }

    for (Rank = 0; Rank < MAX_RANKS; Rank++){
      if (MmrcData->Channel[Channel].RankEnabled[Rank] == 0) {
        // Skip this iteration if the channel is disabled
        continue;
      }
      CachedValues = (UINT32(*)[][MAX_STROBES]) (CachedValuesHobPointer + (GetMrcTrainingDataSize(MmrcData) * RankTableIndex));
      for (Element = 0; Element < RestoreDataDescriptorTableCount; Element++){
        for (Frequency = 0; Frequency < NumFrequencies; Frequency++) {
          for (Strobe = 0; Strobe < MAX_STROBES; Strobe++) {
            UINT8 TypeIndex = RestoreDataDescriptorTable[Element].TypeIndex + Frequency;
            if (Task == SAVE) {
              if (TypeIndex < ALGO_REG_INDEX){
                (*CachedValues)[Element*NumFrequencies + Frequency][Strobe] = MmrcData->Channel_V[Channel].CachedValues[TypeIndex][Rank][Strobe];
              } else {
                (*CachedValues)[Element*NumFrequencies + Frequency][Strobe] = MmrcData->Channel_V[Channel].Trained_Value.Values[TypeIndex - ALGO_REG_INDEX][Rank][Strobe];
              }
            } else {
              if (TypeIndex < ALGO_REG_INDEX){
                MmrcData->Channel_V[Channel].CachedValues[TypeIndex][Rank][Strobe] = (*CachedValues)[Element*NumFrequencies + Frequency][Strobe];
              } else{
                MmrcData->Channel_V[Channel].Trained_Value.Values[TypeIndex - ALGO_REG_INDEX][Rank][Strobe] = (UINT16)(*CachedValues)[Element*NumFrequencies + Frequency][Strobe];
              }
            }
          } //Strobe
        } //Frequency
      } //Element
      RankTableIndex++;
    } //Rank
  } //Channel
}


/**

  This function saves the MRC NV Data to the Dest input pointer buffer.

  @param[in]      MmrcData               Pointer to MmrcData where data will be saved
                                         from.
  @param[in,out]  Dest                   Pointer to the output buffer where data will be
                                         saved to.

**/
VOID
SaveMrcNvDataToBuffer(
  IN     MMRC_DATA *MmrcData,
  IN OUT VOID      *Dest
)
{
  Mmrcmemcpy(
    Dest,
    MmrcData,
    sizeof(MRC_PARAMS_SAVE_RESTORE)
    );

  SaveRestoreCachedValues(
    MmrcData,
    (UINT8 *)Dest + sizeof(MRC_PARAMS_SAVE_RESTORE),
    SAVE
    );
}

/**

  This function restored the MRC NV Data from the Src input pointer buffer.

  @param[in,out] MmrcData              Pointer to MmrcData where data will be restored
                                       to.
  @param[in]     Src                   Pointer to the output buffer where data will be
                                       restored from.

**/
VOID
RestoreMrcNvDataFromBuffer(
  IN OUT MMRC_DATA *MmrcData,
  IN     VOID      *Src
)
{
  Mmrcmemcpy(
    MmrcData,
    Src,
    sizeof(MRC_PARAMS_SAVE_RESTORE)
    );
  SaveRestoreCachedValues(
    MmrcData,
    (UINT8 *)Src + sizeof(MRC_PARAMS_SAVE_RESTORE),
    RESTORE
    );
}
#endif

MMRC_STATUS ResetHighCmdCtlValues(
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  UINT32 TempValue;
  UINT8 CccLoop;

  for (CccLoop = 0; CccLoop < MAX_CMDS; CccLoop++) {
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CmdGrp0 + CccLoop, CMD_GET_CACHE, &TempValue);
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CmdGrp0 + CccLoop, CMD_SET_VAL_FC_UC, &TempValue);
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CtlGrp0 + CccLoop, CMD_GET_CACHE, &TempValue);
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CtlGrp0 + CccLoop, CMD_SET_VAL_FC_UC, &TempValue);
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CkGrp0 + CccLoop, CMD_GET_CACHE, &TempValue);
    GetSetDdrioGroup2 (MrcData, Channel, 0, 0, CkGrp0 + CccLoop, CMD_SET_VAL_FC_UC, &TempValue);
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
SWSetHiFrequency (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channelx
  )
{
  int ch;
  UINT8 NewFrequency;
  //
  //  Setup High Frequency as the current Software Frequency
  //
  NewFrequency = ConfigToSpeed[TimingParamHash ((MrcData->DVFS_FreqA - 1), TRUE)];
  if ( NewFrequency == BOOT_FREQUENCY ) {
    //
    // Don't need to switch frequency if "High Frequency" is the "Boot Frequency"
    //
    return MMRC_SUCCESS;
  }
  MrcData->CurrentFrequency = NewFrequency;

  MmrcDebugPrint((MMRC_DBG_MIN, "Set Software Hi Freq:%d Speed set to:%d\n", MrcData->DVFS_FreqA, MrcData->CurrentFrequency));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MrcData->Channel[ch].CurrentFrequency = MrcData->CurrentFrequency;
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
SWSetBootFrequency (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channelx
  )
{
  int ch;
  //
  //  Setup Boot Frequency as the current Software Frequency
  //
  if (MrcData->CurrentFrequency == BOOT_FREQUENCY) {
    //
    // Don't need to switch frequency if "High Frequency" is the "Boot Frequency"
    //
    return MMRC_SUCCESS;
  }
  MrcData->CurrentFrequency = BOOT_FREQUENCY;
  MmrcDebugPrint((MMRC_DBG_MIN, "Set Software Init Freq:%d Speed set to:%d\n", BOOT_FREQUENCY, MrcData->CurrentFrequency));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MrcData->Channel[ch].CurrentFrequency = MrcData->CurrentFrequency;
  }
  return MMRC_SUCCESS;
}

MMRC_STATUS
HWSetBootFrequency (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  int ch;
  if (MrcData->Channel[Channel].DramType != TypeLpDdr4) {
    return MMRC_SUCCESS;
  }
  //
  // When the High Frequency == BOOT_FREQUENCY, we only need to call SetupMaxPI once.
  // For any other frequency, we need to call "MrcChangeFreq" to handle the PLL re-locking.
  //
  if (MrcData->CurrentFrequency == BOOT_FREQUENCY) {
    if(MrcData->SetupMaxPiDone[GetCurrentFrequencyIndex()]) {
      return MMRC_SUCCESS;
    } else {
      SetupMaxPI(MrcData, 0, 0, 0);
      return MMRC_SUCCESS;
    }
  }
  MrcData->DefaultCmdClkCtl=TRUE;
  //
  //  Setup High Frequency as the current Frequency
  //
  MrcData->CurrentFrequency = BOOT_FREQUENCY;
  MmrcDebugPrint((MMRC_DBG_MIN, "Set Boot Freq:%d Speed set to:%d\n", BOOT_FREQUENCY, MrcData->CurrentFrequency));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MrcData->Channel[ch].CurrentFrequency = MrcData->CurrentFrequency;
  }
  //
  // Perform the Frequency Change
  //
  MmrcChangeFreq(MrcData, CapsuleStartIndex, StringIndex, Channel);
  return MMRC_SUCCESS;
}

MMRC_STATUS
HWSetHiFrequency (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
  int ch;
  UINT8 NewFrequency;
  if (MrcData->Channel[Channel].DramType != TypeLpDdr4) {
    return MMRC_SUCCESS;
  }
  NewFrequency = ConfigToSpeed[TimingParamHash ((MrcData->DVFS_FreqA - 1), TRUE)];
  //
  // When the High Frequency == BOOT_FREQUENCY, we only need to call SetupMaxPI once.
  // For any other frequency, we need to call "MrcChangeFreq" to handle the PLL re-locking.
  //
  if ( NewFrequency == BOOT_FREQUENCY ) {
    if(MrcData->SetupMaxPiDone[GetCurrentFrequencyIndex()]) {
      return MMRC_SUCCESS;
    } else {
      SetupMaxPI(MrcData, 0, 0, 0);
      return MMRC_SUCCESS;
    }
  }
  //
  //  Setup High Frequency as the current Frequency
  //
  MrcData->CurrentFrequency = ConfigToSpeed[TimingParamHash ((MrcData->DVFS_FreqA - 1), TRUE)];
  MmrcDebugPrint((MMRC_DBG_MIN, "Set Hi Freq:%d Speed set to:%d\n", MrcData->DVFS_FreqA, MrcData->CurrentFrequency));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MrcData->Channel[ch].CurrentFrequency = MrcData->CurrentFrequency;
  }
  //
  // Perform the Frequency Change
  //
  MmrcChangeFreq(MrcData, CapsuleStartIndex, StringIndex, Channel);
  return MMRC_SUCCESS;
}

#ifndef modmem_lp4_freq_switch_seq1
#define modmem_lp4_freq_switch_seq1 0
#define modmem_lp4_freq_switch_seq2 0
#define modmem_lp4_freq_switch_seq3 0
#endif

MMRC_STATUS
MmrcChangeFreq (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channelx
  )
{
  UINT8 ch;
  UINT8   SaveDebugMsgLevel;
  //MrcData->MrcDebugMsgLevel |= MMRC_DBG_MAX;
  //MmrcDebugPrint((MMRC_DBG_MAX, "DunitFreqChangePart1\n"));
  //
  // Put the DUNIT into PM2
  //
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, DunitFreqChangePart1, NO_PRINT, ch);
  }
  //MmrcDebugPrint((MMRC_DBG_MAX, "modmem_lp4_freq_switch_seq1\n"));
  //
  // Put most of the Phy in Reset
  //

  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, modmem_lp4_freq_switch_seq1, NO_PRINT, ch);
  }
  //MmrcDebugPrint((MMRC_DBG_MAX, "modmem_init_lpddrgrp2m_seq\n"));
  //  Setup PLL settings for current Frequency
  //
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, modmem_init_lpddrgrp2m_seq, NO_PRINT, ch);
  }
  //  PLL Lock
  //
  //MmrcDebugPrint((MMRC_DBG_MAX, "modmem_lp4_freq_switch_seq2\n"));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, modmem_lp4_freq_switch_seq2, NO_PRINT, ch);
  }
  //MmrcDebugPrint((MMRC_DBG_MAX, "modmem_lp4_freq_switch_seq2a\n"));
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, modmem_lp4_freq_switch_seq2a, NO_PRINT, ch);
  }
  //SetupMaxPI(MrcData, 0,0,0);

  SaveDebugMsgLevel = MrcData->MrcDebugMsgLevel;
  //MrcData->MrcDebugMsgLevel=0;
  //
  // Initialize the DLL, and program the delay values to appropriate locations
  //
  SetupMaxPI(MrcData, NO_PRINT, NO_PRINT, 0);
  MrcData->MrcDebugMsgLevel = SaveDebugMsgLevel;
  //
  // Program the CMD/CTL/CLK to the New Frequency values
  //
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcDebugPrint((MMRC_DBG_MIN, "DefaultCmdClkCtl=%d\n", MrcData->DefaultCmdClkCtl));
    if (MrcData->DefaultCmdClkCtl == TRUE)
    {
      MmrcExecuteTask (MrcData, modmem_init_progcmdckctl_seq, NO_PRINT, ch);
    }
    else
    {
      ResetHighCmdCtlValues(MrcData, NO_PRINT, NO_PRINT, ch);
    }
  }
  //MrcData->MrcDebugMsgLevel=SaveDebugMsgLevel;
  //MmrcDebugPrint((MMRC_DBG_MAX, "modmem_lp4_freq_switch_seq3\n"));
  //
  // Bring Phy out of reset
  //
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, modmem_lp4_freq_switch_seq3, NO_PRINT, ch);
  }
  //MmrcDebugPrint((MMRC_DBG_MAX, "DunitFreqChangePart3\n"));
  //
  // Bring the DUNIT out of PM2
  //
  for (ch=0; ch < MAX_CHANNELS; ch++)
  {
    MmrcExecuteTask (MrcData, DunitFreqChangePart3, NO_PRINT, ch);
  }
  //MrcData->MrcDebugMsgLevel &= ~MMRC_DBG_MAX;
  return MMRC_SUCCESS;
}


/// Implementing a new wake sequence which will include toggling BGF_RUN for dunits which fail to wake
///  https://hsdes.intel.com/home/default.html/article?id=1206168975
MMRC_STATUS
  S0_DunitWake (
  IN  OUT   MMRC_DATA   *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel)
{
  UINT8           Ch;
  REGISTER_ACCESS Register;
  UINT32          Value;
  UINT16          WakeCnt = 0;
  UINT16          WakeLoopCnt = 0;
  UINT8           Results = 0xF; // Each bit represents a DUNITS WAKE bit status

  Register.Offset = D_CR_DRAM_SR_CMD_DUNIT_REG;
  Register.ShiftBit = 0;
  Register.Mask = 0xFFFFFFFF;

  for (Ch=0; Ch < MAX_CHANNELS; Ch++) {
    if (!RunOnThisChannel (MrcData, Ch, 0)) {
      Results &= ~((UINT8)1 << Ch); //Clear unused channel's bit
      continue;
    }
    MmrcExecuteTask (MrcData, InitDunitWakeS0pt1, NO_PRINT, Ch); // Set WAKE=1
  } //For Ch
  Results &= 0xF; // Paranoid; Ensure upper nibble can be safely ignored
  Ch=0;
  while (Results != 0) {
    if (!RunOnThisChannel (MrcData, Ch, 0)) {
    Ch++;
      continue;
    }
    Value = (UINT32)MemRegRead (DUNIT, Channel, 0, Register);
    if (Value & 0x1) { // If the WAKE bit is still set
      WakeCnt++;
      MrcDelay (MrcData, MICRO_DEL, 1);
    } else {
      //Success; Clear Result bit
      Results &= ~((UINT8)1 << Ch); //Clear unused channel's bit
      Ch++;                         //Inc to examine next channel
      WakeCnt = 0;                  //Reset Counters
      WakeLoopCnt = 0;
    }
    if (WakeCnt > 15) {  //~15us limit
      // Toggle BGF Run if more chances left
      WakeLoopCnt++;
      if (WakeLoopCnt > 10) {
        MmrcErrorPrint((MMRC_DBG_MIN, "<!> CH%02d Isn't waking up! Can't continue ...\n", Ch));
        MrcDeadLoop();
      } else {
#ifdef ToggleBGF_RUN
        if (MrcData->CpuStepping <= 1) {//WA 1206239220 TODO Fix stepping information
          //Toggle BGF Run for A0/A1
          MmrcExecuteTask (MrcData, ToggleBGF_RUN, NO_PRINT, Ch);
        }
#endif
      }
    }
  } //For Ch
  return MMRC_SUCCESS;
}


void FillDataSel (MMRC_DATA* MrcData, PTDATAMUXSEL_CCC_STRUCT * datamux, UINT8 ch, UINT8 bl)
{
  UINT8 *Mapping, *Mapping2;
  Mapping = MrcData->Channel[ch].Dimm2DunitReverseMapping;
  Mapping2 = MrcData->Channel[ch].Dimm2DunitMapping;

  datamux->Bits.data0sel  = Mapping[(bl*8) + 0] % 8;
  datamux->Bits.data1sel  = Mapping[(bl*8) + 1] % 8;
  datamux->Bits.data2sel  = Mapping[(bl*8) + 2] % 8;
  datamux->Bits.data3sel  = Mapping[(bl*8) + 3] % 8;
  datamux->Bits.data4sel  = Mapping[(bl*8) + 4] % 8;
  datamux->Bits.data5sel  = Mapping[(bl*8) + 5] % 8;
  datamux->Bits.data6sel  = Mapping[(bl*8) + 6] % 8;
  datamux->Bits.data7sel  = Mapping[(bl*8) + 7] % 8;
}
void SetupPeriodicDDRIOSwizzle(MMRC_DATA * MrcData, UINT8 Channel)
{
  REGISTER_ACCESS Register;
  UINTX   Value;
  PTDATAMUXSEL_CCC_STRUCT * datamux;
  PTCTL0_DPLVUG_STRUCT* ptrctl;
  UINT8 bl;

  //
  // Get BL0 Bit Swizzle Register
  //
  Register.Offset = PTDATAMUXSEL_CCC_REG;
  Register.ShiftBit=0;
  Register.Mask=0xFFFFFFFF;
  Value = MemRegRead (DQCCC, Channel, 0, Register);
  datamux = (PTDATAMUXSEL_CCC_STRUCT *) &Value;
  bl = MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][0].Strobelane;
  FillDataSel (MrcData, datamux, Channel, bl);
  //
  //  BL0
  //
  /*
  if (Channel==0)
  {
    //DQ11_d DQ10_d DQ9_d DQ8_d DQ12_d DQ15_d DQ13_d DQ14_d
    datamux->Bits.data0sel  = 3;
    datamux->Bits.data1sel  = 2;
    datamux->Bits.data2sel  = 1;
    datamux->Bits.data3sel  = 0;
    datamux->Bits.data4sel  = 4;
    datamux->Bits.data5sel  = 6;
    datamux->Bits.data6sel  = 7;
    datamux->Bits.data7sel  = 5;
  }
  else if (Channel==1)
  {
    //DQ1_c DQ0_c DQ3_c DQ2_c DQ4_c DQ5_c DQ7_c DQ6_c
    datamux->Bits.data0sel  = 1;
    datamux->Bits.data1sel  = 0;
    datamux->Bits.data2sel  = 3;
    datamux->Bits.data3sel  = 2;
    datamux->Bits.data4sel  = 4;
    datamux->Bits.data5sel  = 5;
    datamux->Bits.data6sel  = 7;
    datamux->Bits.data7sel  = 6;
  }
  else if (Channel==2)
  {
    //DQ10_a DQ9_a DQ8_a DQ11_a DQ12_a DQ15_a DQ14_a DQ13_a
    datamux->Bits.data0sel  = 2;
    datamux->Bits.data1sel  = 1;
    datamux->Bits.data2sel  = 0;
    datamux->Bits.data3sel  = 3;
    datamux->Bits.data4sel  = 4;
    datamux->Bits.data5sel  = 7;
    datamux->Bits.data6sel  = 6;
    datamux->Bits.data7sel  = 5;
  }
  else if (Channel==3)
  {
    //DQ0_b DQ1_b DQ2_b DQ3_b DQ4_b DQ5_b DQ7_b DQ6_b
    datamux->Bits.data0sel  = 0;
    datamux->Bits.data1sel  = 1;
    datamux->Bits.data2sel  = 2;
    datamux->Bits.data3sel  = 3;
    datamux->Bits.data4sel  = 4;
    datamux->Bits.data5sel  = 5;
    datamux->Bits.data6sel  = 7;
    datamux->Bits.data7sel  = 6;
  }
  */
  MemRegWrite(DQCCC, Channel, 0, Register, Value, 0xF);

  //
  // Get BL1 Bit Swizzle Register
  //
  Value = MemRegRead (DQCCC, Channel, 1, Register);
  bl = MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][1].Strobelane;
  FillDataSel (MrcData, datamux, Channel, bl);
  /*
  if (Channel==0)
  {
    //DQ5_d DQ4_d DQ6_d DQ7_d DQ3_d DQ2_d DQ0_d DQ1_d
    datamux->Bits.data0sel  = 6;
    datamux->Bits.data1sel  = 7;
    datamux->Bits.data2sel  = 5;
    datamux->Bits.data3sel  = 4;
    datamux->Bits.data4sel  = 1;
    datamux->Bits.data5sel  = 0;
    datamux->Bits.data6sel  = 2;
    datamux->Bits.data7sel  = 3;
  }
  else if (Channel==1)
  {
    //DQ15_c DQ13_c DQ14_c DQ12_c DQ10_c DQ11_c DQ8_c DQ9_c
    datamux->Bits.data0sel  = 6;
    datamux->Bits.data1sel  = 7;
    datamux->Bits.data2sel  = 4;
    datamux->Bits.data3sel  = 5;
    datamux->Bits.data4sel  = 3;
    datamux->Bits.data5sel  = 1;
    datamux->Bits.data6sel  = 2;
    datamux->Bits.data7sel  = 0;
  }
  else if (Channel==2)
  {
    //DQ5_a DQ6_a DQ7_a DQ4_a DQ3_a DQ0_a DQ1_a DQ2_a
    datamux->Bits.data0sel  = 5;
    datamux->Bits.data1sel  = 6;
    datamux->Bits.data2sel  = 7;
    datamux->Bits.data3sel  = 4;
    datamux->Bits.data4sel  = 3;
    datamux->Bits.data5sel  = 0;
    datamux->Bits.data6sel  = 1;
    datamux->Bits.data7sel  = 2;
  }
  else if (Channel==3)
  {
    //DQ15_b DQ13_b DQ14_b DQ12_b DQ11_b DQ10_b DQ9_b DQ8_b
    datamux->Bits.data0sel  = 7;
    datamux->Bits.data1sel  = 6;
    datamux->Bits.data2sel  = 5;
    datamux->Bits.data3sel  = 4;
    datamux->Bits.data4sel  = 3;
    datamux->Bits.data5sel  = 1;
    datamux->Bits.data6sel  = 2;
    datamux->Bits.data7sel  = 0;
  }
  */
  MemRegWrite(DQCCC, Channel, 1, Register, Value, 0xF);

  //
  // Get ByteSwizzle Register
  //
  bl = MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][0].Strobelane;
  Register.Offset = PTCTL0_DPLVUG_REG;
  Value = MemRegRead (DPLVUG, Channel, 0, Register);
  ptrctl = (PTCTL0_DPLVUG_STRUCT *) &Value;
  ptrctl->Bits.bytelane1datasel=bl;
/*
  if (Channel==1 || Channel == 3)
  {
    ptrctl->Bits.bytelane1datasel=0;
  }
  else if (Channel==0 || Channel == 2)
  {
    ptrctl->Bits.bytelane1datasel=1;
  }
  */
  MemRegWrite(DPLVUG, Channel, 0, Register, Value, 0xF);
}

#define ENABLE_PERIODIC_DQ_ADJUST 1
UINT32 GetManualInitialIndex(
  MMRC_DATA         *MrcData,
  UINT8             Rank,
  UINT8             Channel,
  UINT16            *ro_table,
  BOOLEAN           RestorePath
  )
{
  UINT8  MR18Data, MR19Data;//, MRRData;
  UINT16 MRCombinedData;
  UINT8  SMR18Data, SMR19Data;
  UINT16 SMRCombinedData;
  UINT8  DMR18Data, DMR19Data;
  UINT16 DMRCombinedData;
  UINT16 RO1, RO2;
  REGISTER_ACCESS Register;
  UINT32  TempValue;
  UINT8  bl=MrcData->MasterFloorPlan[MrcData->CurrentBlueprint][Channel][0].Strobelane;
  Register.Mask   = 0xFFFFFFFF;
  Register.ShiftBit = 0;

  //
  //  Read rdcmd2datavalid and set ptloadcode_dly=3
  //
  MrcData->DynamicVars[Channel][PTLDCODE]=1;
  MmrcExecuteTask (MrcData, PERIODIC_TRAINING_READ, NO_PRINT, Channel);
  MySimStall(2000);
  MrcDelay (MrcData, MICRO_DEL, 10);

  //RL+tDQSCK+4/2+1   , (rdcmd2rdvld + 4) * 4
  //+4 = PHY.ptctl0.ptloadcode_dly+2
  ////if (MrcData->DynamicVars[0][BXT_A0_WA] == 1) //This value needs to be set only for A0
  ////  MrcData->DynamicVars[Channel][TRWDR] = ((UINT32)MrcData->DynamicVars[Channel][RDCMD2RDVLD] + (UINT32)MrcData->DynamicVars[Channel][PTLDCODE] + 2  + 4) * 4;
  MrcData->DynamicVars[Channel][TPSTMRBLK] = ((UINT32) MrcData->DynamicVars[Channel][RDCMD2RDVLD] + (UINT32)MrcData->DynamicVars[Channel][PTLDCODE] + 2) * 4;
  MmrcDebugPrint((MMRC_DBG_MIN, "\x1b[1;92m RDCMD2RDVLD=%d, PTLDCODE=%d\n\x1b[0m", (UINT32) MrcData->DynamicVars[Channel][RDCMD2RDVLD], (UINT32) MrcData->DynamicVars[Channel][PTLDCODE]));
  MmrcDebugPrint((MMRC_DBG_MIN, "\x1b[1;92m TPSTMRBLK=%d, TRWDR=%d\n\x1b[0m", (UINT32) MrcData->DynamicVars[Channel][TPSTMRBLK], (UINT32) MrcData->DynamicVars[Channel][TRWDR]));
  //MrcData->DynamicVars[Channel][TPSTMRBLK] = 0x2c;
  if (RestorePath)
  {
    GetSetDdrioGroup2(MrcData, Channel, Rank, 0, INITIAL_INDEX, CMD_GET_CACHE, &TempValue);
    MmrcDebugPrint ((MMRC_DBG_MIN, "CH=%d R=%d Restoring 0x%04x\n", Channel, Rank, TempValue));
  }
  else
  {
    //
    // Issue the MPC-1 command
    //
    MrcData->DynamicVars[Channel][RANK]        = Rank;
    MrcData->DynamicVars[Channel][REG_DATA]    = MPC2(0, START_DQS_OSC);
    MrcData->DynamicVars[Channel][RDDCMD]      = 0;
    MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);

    MySimStall(200);
    MrcDelay (MrcData, MICRO_DEL, 10);
    //
    //  Issue MR18
    //
    MrcData->DynamicVars[Channel][RANK]        = Rank;
    MrcData->DynamicVars[Channel][REG_DATA]    = MRR_LP4(18);
    MrcData->DynamicVars[Channel][RDDCMD]      = 1;
    MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    // Wait a Bit
    MySimStall(200);
    MrcDelay (MrcData, MICRO_DEL, 1);
    //
    // Read MR18 value from DUNIT
    //
    Register.Offset = D_CR_DCMDDATA_DUNIT_REG;
    MR18Data = (UINT8) (MemRegRead (DUNIT, Channel, 0, Register) & 0xFF);
    SMR18Data = (UINT8) SwizzleBytelane(MrcData, Channel, bl, MR18Data);
    DMR18Data = (UINT8) DeSwizzleBytelane(MrcData, Channel, bl, MR18Data);
    //SMR18Data = (UINT8) Swizzle(MrcData, Channel, MR18Data);
    //DMR18Data = (UINT8) DeSwizzle(MrcData, Channel, MR18Data);
    //
    //  Issue MR19
    //
    MrcData->DynamicVars[Channel][REG_DATA]    = MRR_LP4(19);
    MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, Channel);
    // Wait a Bit
    MySimStall(200);
    MrcDelay (MrcData, MICRO_DEL, 1);
    //
    // Read MR19 value from DUNIT
    //
    Register.Offset = D_CR_DCMDDATA_DUNIT_REG;
    MR19Data = (UINT8) (MemRegRead (DUNIT, Channel, 0, Register) & 0xFF);
    SMR19Data = (UINT8) SwizzleBytelane(MrcData, Channel, bl, MR19Data);
    DMR19Data = (UINT8) DeSwizzleBytelane(MrcData, Channel, bl, MR19Data);
    //SMR19Data = (UINT8) Swizzle(MrcData, Channel, MR19Data);
    //DMR19Data = (UINT8) DeSwizzle(MrcData, Channel, MR19Data);
    MRCombinedData = MR18Data | (MR19Data << 8);
    SMRCombinedData = SMR18Data | (SMR19Data << 8);
    DMRCombinedData = DMR18Data | (DMR19Data << 8);

    MmrcDebugPrint ((MMRC_DBG_MIN, "CH=%d R=%d MR18=0x%02x S:0x%02x MR19=0x%02x S:0x%02x Combined=0x%04x S:%04x\n", Channel, Rank, MR18Data, SMR18Data, MR19Data, SMR19Data, MRCombinedData, SMRCombinedData));

    MrcDelay(MrcData, 1, 20);

    RO1=0xFFFF;
    for (TempValue = 0; TempValue < 70; TempValue++) {
      RO2 = RO1;
      RO1 = ro_table[TempValue];
      if ((SMRCombinedData < RO2) && (SMRCombinedData >= RO1))
      {
        break;
      }
    }
  }
  //TempValue = 0x45;
  //MmrcDebugPrint ((MMRC_DBG_MAX, "InitialIndex=%02i\n", TempValue));
  return TempValue;
}

/**
  Performs Periodic Training for LP4.
  calibration.

  @param[in, out]  MrcData         Host structure for all data related to MMRC.
  @param[in]   CapsuleStartIndex   Starting point within the CapsuleData structure
                                   where the Init should begin.
  @param[in]       StringIndex     When provided, this is the index into the string
                                   table to show the register names.
  @param[in]       Channel         Current Channel being examined.
  @retval          MMRC_SUCCESS
**/
MMRC_STATUS
PeriodicTraining (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
  )
{
#if PERIODIC_TRAINING == 1 && TRAINING_ALGOS == 1
#define PERIODIC_BUCKETS 70
#define PERIODIC_AUTOMATIC_INITIAL_INDEX 0
  if (MrcData->DynamicVars[Channel][BXT_PERIODIC_ENABLED] != 0)
  {
    UINT8   Rank;
    UINT32  TempValue, AnotherTempValue;
    REGISTER_ACCESS Register;
    UINT16 RO1;
    UINT16 RO2;
    UINT32  RunTimeInterval;
    UINT16  InitialValue;
    UINT16  StepSize;
    UINT16  Multiplier;
    UINT16  NumPi;
    UINT32 PTLENGTH = 140 * 1000; //150->300
    //INT32  i;
    UINT32 Dq2x, Drvdb;
    UINT8  Strobe;
    UINTX  Ptrctl0;
    UINT8  ch;
    UINT16 ro_table[PERIODIC_BUCKETS];
    BOOLEAN RestorePath = ((MrcData->BootMode == S0) || (MrcData->BootMode == FB)) ? TRUE : FALSE;
   // UINT8   BitSwizzle[8] = {};

    Rank = 0;
    TempValue = 0, AnotherTempValue=0;
    RunTimeInterval = 0;
    InitialValue = 190;     //excel
    StepSize = 10;          //excel
    Multiplier = 2;
    Register.Mask   = 0xFFFFFFFF;
    Register.ShiftBit = 0;
    ch = Channel;
    MrcData->MrcDebugMsgLevel=MMRC_DBG_TYPICAL;

    if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
      //FillChannelBitMapping(MrcData, 0, 0, ch);
      //CalculateDeswizzleReverseMap(MrcData, ch);
      //CalculateMasterFloorplan(MrcData, ch);
      SetupPeriodicDDRIOSwizzle(MrcData, ch);

      //
      // send mr23 ~150ns in 16 dram clk
      //
      for (Rank = 0; Rank < MAX_RANKS; Rank++) {
        if (!RunOnThisChannel (MrcData, Channel, Rank)) {
          continue;
        }
        TempValue = (PTLENGTH) / TCK[MrcData->CurrentFrequency] / 16;
        MmrcDebugPrint ((MMRC_DBG_MIN, "C=%d R=%d MR23: 0x%x\n", ch, Rank, TempValue));
        MrcData->DynamicVars[Channel][RANK]        = Rank;
        MrcData->DynamicVars[ch][REG_DATA] = MRW_LP4(23, TempValue);
        MmrcExecuteTask (MrcData, SEND_DRAM_CMD, NO_PRINT, ch);
        MySimStall(2000);
      }

      //
      // Find the Runtime Interval to the closest Clock
      //
      RunTimeInterval = (UINT32) (PTLENGTH / (GetOneClk(MrcData, ch) ));
      RunTimeInterval = RunTimeInterval * (GetOneClk(MrcData, ch));
      //
      // Get NumPi
      //
      Register.Offset = PTCTL0_DPLVUG_REG;
      Ptrctl0 = MemRegRead(DPLVUG, ch, 0, Register);
      Ptrctl0 |= BIT2;
      MemRegWrite(DPLVUG, ch, 0, Register, Ptrctl0, 0xFF);
      NumPi = (UINT16)Ptrctl0 & BIT2;
      NumPi = NumPi >> 2;

      if (NumPi == 0) {
        Multiplier = 1;
      }

      StepSize = Multiplier * ((TCK[MrcData->CurrentFrequency]/2) / (GetHalfClk(MrcData, ch)) );

      for (TempValue=0; TempValue < PERIODIC_BUCKETS; TempValue++)
      {
        ro_table[TempValue] = (UINT16) (RunTimeInterval/ ( 2* ((InitialValue) + (TempValue* StepSize)) ));
      }


      for (TempValue = 0; TempValue < 35; TempValue++) {
        RO1 = ro_table[(TempValue * 2)];     //(UINT16) (RunTimeInterval/ ( 2* ((InitialValue) + ((TempValue*2)       * StepSize))) ); //*2 added to account for full period
        RO2 = ro_table[(TempValue * 2) + 1]; //(UINT16) (RunTimeInterval/ ( 2* ((InitialValue) + (((TempValue*2) + 1) * StepSize))) );
        if (ch == 0)
          MmrcDebugPrint ((MMRC_DBG_MIN, "%02x %02x ", RO1, RO2));
        Register.Offset = PTROCNT12_DPLVUG_REG + (TempValue*4);
        AnotherTempValue = (RO1<<16)|RO2;
        MemRegWrite (DPLVUG, ch, 0, Register, AnotherTempValue, 0xF);
      }
      MmrcDebugPrint ((MMRC_DBG_MIN, "\n"));
      //MmrcExecuteTask (MrcData, PERIODIC_COPY_RK0_TO_RK1, NO_PRINT, Channel);
      for (Rank = 0; Rank < MAX_RANKS; Rank++) {
        if (!RunOnThisChannel (MrcData, Channel, Rank)) {
          continue;
        }
        //CpgcVATest (MrcData,NO_PRINT, NO_PRINT, ch);
        //
        // Get Initial Index
        //
        TempValue = GetManualInitialIndex(MrcData, Rank, Channel, ro_table, RestorePath);
        for (Strobe = 0; Strobe < MrcData->Channel[Channel].MaxDq[0]; Strobe++)
        {
          if (((MrcData->Rk2RkEn.TxDq == 0) && (Rank==0)) || (MrcData->Rk2RkEn.TxDq != 0)) {
            //
            //  Get TxDq deadband, and decide how to change the value
            //
            GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 3, CMD_GET_REG, &Drvdb);
            if (Drvdb == 3 || Drvdb == 1)  //We decrement for region 3,1
            {
              //
              // Grp0 2x increment
              //
              MmrcDebugPrint((MMRC_DBG_MIN, "C:%d R:%d S:%d DeadBand is %02d, So adding 1 2x to DQ\n", Channel, Rank, Strobe, Drvdb));
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_GET_REG, &Dq2x);
              MmrcDebugPrint((MMRC_DBG_MIN, "DQ 2x Value is %d -> ", Dq2x));
              Dq2x += 1;
              MmrcDebugPrint((MMRC_DBG_MIN, " %d\n", Dq2x));
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_SET_VAL_FC_UC, &Dq2x);
              //
              // Grp6 2x increment
              //
              //GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, TxDqDrive, CMD_GET_REG, &Dq2x);
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDrive) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_GET_REG, &Dq2x);
              MmrcDebugPrint((MMRC_DBG_MIN, "TxDqDrive Value is %d -> ", Dq2x));
              Dq2x += 1;
              MmrcDebugPrint((MMRC_DBG_MIN, "%d\n", Dq2x));
              //GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, TxDqDrive, CMD_SET_VAL_FC_UC, &Dq2x);
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDrive) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_SET_VAL_FC_UC, &Dq2x);
            }
            else
            {
              MmrcDebugPrint((MMRC_DBG_MIN, "C:%d R:%d S:%d No Adjustment necessary\n", Channel, Rank, Strobe, Drvdb));
            }
            /*
            if (Drvdb != 0)
            {
              //
              // Grp0 2x increment
              //
              MmrcDebugPrint((MMRC_DBG_MIN, "C:%d R:%d S:%d DeadBand is %02d, So adding 1 2x to DQ\n", Channel, Rank, Strobe, Drvdb));
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_GET_REG, &Dq2x);
              MmrcDebugPrint((MMRC_DBG_MIN, "DQ 2x Value is %d\n", Dq2x));
              Dq2x += 1;
              MmrcDebugPrint((MMRC_DBG_MIN, "Programming %d\n", Dq2x));
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, (GetOffsetIndex(MrcData, TxDqDelay_Actual) - ALGO_REG_INDEX) * NumberOfElementsPerAlgo + 1, CMD_SET_VAL_FC_UC, &Dq2x);
              //
              // Grp6 2x increment
              //
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, Grp6_2x, CMD_GET_REG, &Dq2x);
              MmrcDebugPrint((MMRC_DBG_MIN, "Grp6 2x Value is %d\n", Dq2x));
              Dq2x += 1;
              MmrcDebugPrint((MMRC_DBG_MIN, "Programming %d\n", Dq2x));
              GetSetDdrioGroup2(MrcData, Channel, Rank, Strobe, Grp6_2x, CMD_SET_VAL_FC_UC, &Dq2x);
            }
*/
          }
          else
          {
            if ((MrcData->Rk2RkEn.TxDq == 0) && (Rank != 0))
            {
              MmrcDebugPrint((MMRC_DBG_MIN, "C:%d R:%d Skipped adjusting DQ since Rk2Rk is disabled\n", Channel, Rank));
            }
          }
        } //Strobe
        MmrcDebugPrint((MMRC_DBG_MIN, "C=%d R=%d CurrentIndex 0x%x\n", Channel, Rank, TempValue));
        GetSetDdrioGroup2 (MrcData, Channel, Rank, 0, INITIAL_INDEX, CMD_SET_VAL_FC_UC , &TempValue);
      }
      MmrcDebugPrint((MMRC_DBG_MIN, "C=%d enabling Periodic Programming\n", Channel));
      MmrcExecuteTask (MrcData, PERIODIC_TRAINING_EN, NO_PRINT, Channel);

      MmrcDebugPrint((MMRC_DBG_MIN, "Running atleast one Maintainence cycle\n", Channel));
      MmrcExecuteTask (MrcData, PERIODIC_TRAINING_START, NO_PRINT, Channel);
      MmrcDebugPrint((MMRC_DBG_MAX, "TQPOLLEN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][TQPOLLEN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "SRXZQC_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][SRXZQC_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "ZQDIS_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][ZQDIS_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "OREFDIS_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][OREFDIS_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DYNSREN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DYNSREN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DISPWRDN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DISPWRDN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DQS_RETAIN_EN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DQS_RETAIN_EN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "Disabling Maintainence for Periodic\n", Channel));
      MmrcExecuteTask (MrcData, PERIODIC_TRAINING_MNT_DIS, NO_PRINT, Channel);
      MmrcDebugPrint((MMRC_DBG_MAX, "TQPOLLEN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][TQPOLLEN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "SRXZQC_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][SRXZQC_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "ZQDIS_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][ZQDIS_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "OREFDIS_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][OREFDIS_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DYNSREN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DYNSREN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DISPWRDN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DISPWRDN_SAVE]));
      MmrcDebugPrint((MMRC_DBG_MAX, "DQS_RETAIN_EN_SAVE=%d\n", (UINT32) MrcData->DynamicVars[Channel][DQS_RETAIN_EN_SAVE]));
    } // LPDDR4
  }
  else
  {
    MmrcDebugPrint((MMRC_DBG_MAX, "Ch %d: Disabling Periodic Training as Requested", Channel));
  }
#endif //PERIODIC_TRAINING
  return MMRC_SUCCESS;
}

/**
  Returns the corresponding clock crossing table entry for a given CcRange index.

  @param[in]   CcRange   The requested clock crossing range

  @retval      Returns the corresponding clock crossing table entry for a given CcRange
               index. If a valid entry is not found, deadloops and reutrns an entry
               populated with -1.
**/
CLOCK_CROSSINGS
ClockCrossings (
  UINT8 CcRange
  )
{
  MMRC_DATA *MrcData = GetMrcHostStructureAddress();
  return ClockCrossingsGenerated[MrcData->CurrentBlueprint][CcRange];
}

/**
  Setup internal PHY rank switching settings, required to switching picodes reliably insid ethe DDR PHY.
  These settings depend on trained settings established for ReceiveEnable and WriteTraining.  Please
    place this algorithm after any ReceiveEnable and WriteTraining that might occur.
  This function needs to be run on fast boot as well, *after* ReceiveEnable and WriteTraining values
    have been restored.

  @param[in, out]  MrcData         Host structure for all data related to MMRC.
  @param[in]   CapsuleStartIndex   Starting point within the CapsuleData structure
                                   where the Init should begin.
  @param[in]       StringIndex     When provided, this is the index into the string
                                   table to show the register names.
  @param[in]       Channel         Current Channel being examined.
  @retval          MMRC_SUCCESS
**/
MMRC_STATUS SetupPHYRankTurnaroundSettings(
  IN  OUT   MMRC_DATA *MrcData,
  IN        UINT16     CapsuleStartIndex,
  IN        UINT16     StringIndex,
  IN        UINT8      Channel
)
{
  /*
RdRk2RkLatency
RdRk2RkLatency

WrRk2RkLatency
WrRk2RkLatency

DqOffset
DqOffset

DqsOffset
DqsOffset

RcvEnOffset
RcvEnOffset
  */
  UINT8  Ch, Strobe, Rank;
  INT32 max_rcven, max_dq, max_dqs;
  UINT32 rkchg_rd_init, rkchg_wr_init;
  UINT32 rk2rklat_rd, rk2rklat_wr;
  INT32 rcven_off, dq_off, dqs_off;

  for (Ch = 0; Ch < MAX_CHANNELS; Ch++) {
    for (Strobe = 0; Strobe < MrcData->Channel[Channel].MaxDq[0]; Strobe++) {
      max_rcven=0;
      max_dq=0;
      max_dqs=0;
      for (Rank = 0; Rank < MAX_RANKS; Rank++) {
        if (!RunOnThisChannel (MrcData, Ch, Rank)) {
          continue;
        }
        GetSetDdrioGroup2 (MrcData, Ch, Rank, Strobe, RcvEnOffset, CMD_GET_REG, (UINT32 *) &rcven_off);
        max_rcven = MAX(max_rcven, rcven_off);
        GetSetDdrioGroup2 (MrcData, Ch, Rank, Strobe, DqsOffset, CMD_GET_REG, (UINT32 *) &dqs_off);
        max_dqs = MAX(max_dqs, dqs_off);
        GetSetDdrioGroup2 (MrcData, Ch, Rank, Strobe, DqOffset, CMD_GET_REG, (UINT32 *) &dq_off);
        max_dq = MAX(max_dq, dq_off);
        MmrcDebugPrint((MMRC_DBG_MIN, "Ch=%d Rk=%d St=%d rcven_off=%d dqs_off=%d dq_off=%d\n", Ch, Rank, Strobe, rcven_off, dqs_off, dq_off));
      }
      max_dqs -=3;
      max_dq -=7;
      GetSetDdrioGroup2 (MrcData, Ch, 0, Strobe, RdRk2RkLatency, CMD_GET_REG, &rk2rklat_rd);
      rkchg_rd_init = rk2rklat_rd;
      rk2rklat_rd = rk2rklat_rd + (max_rcven + 1)/2; //+1 is done to round up
      GetSetDdrioGroup2 (MrcData, Ch, 0, Strobe, RdRk2RkLatency, CMD_SET_VAL_FC_UC, &rk2rklat_rd);
      GetSetDdrioGroup2 (MrcData, Ch, 0, Strobe, WrRk2RkLatency, CMD_GET_REG, &rk2rklat_wr);
      rkchg_wr_init = rk2rklat_wr;
      rk2rklat_wr = rk2rklat_wr + (MAX(max_dq, max_dqs) + 1)/2; //+1 is done to round up
      MmrcDebugPrint((MMRC_DBG_MIN, "Ch=%d St=%d max_rxven=%d max_dqs=%d max_dq=%d rkchg_rd_init=%d rk2rklat_rd=%d rkchg_wr_init=%d rk2rklat_wr=%d\n", Ch, Strobe, max_rcven, max_dqs, max_dq, rkchg_rd_init, rk2rklat_rd, rkchg_wr_init, rk2rklat_wr));
      GetSetDdrioGroup2 (MrcData, Ch, 0, Strobe, WrRk2RkLatency, CMD_SET_VAL_FC_UC, &rk2rklat_wr);
    }
  }
  return MMRC_SUCCESS;
}

/*++

Routine Description:

  Returns the current frequency index, a value of BootFrequency or HighFrequency based
  on MrcData->CurrentFrequency

Arguments:

  None

Returns:

  BootFrequency
  HighFrequency

--*/
FREQUENCY_INDEX
GetCurrentFrequencyIndex(
  )
{
  MMRC_DATA *MrcData = GetMrcHostStructureAddress();
  if (MrcData->CurrentFrequency == BOOT_FREQUENCY) {
    return BootFrequency;
  } else {
    return HighFrequency;
  }
}
