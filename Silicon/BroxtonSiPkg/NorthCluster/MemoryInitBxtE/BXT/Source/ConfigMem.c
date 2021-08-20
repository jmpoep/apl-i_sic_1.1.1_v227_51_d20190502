/** @file
  Memory Configuration.

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
#include <Library/SteppingLib.h>

#include "ConfigMem.h"
#include "ConfigMemData.h"
#include "IchRegs.h"
#include "MmrcProjectData.h"
#include "OemHooks.h"
#include "McFunc.h"
#include "DetectDimms.h"
#include "Mailbox.h"
#include "MmrcHooks.h"
#include "Mailbox.h"
#include "MmrcLibraries.h"
#include "MrcFunc.h"


/**
  Initialize Punit setting

  @param[in,out]  MrcData

  @retval  None
**/
MMRC_STATUS
InitPunitSetting(
  IN  OUT   MMRC_DATA   *MrcData
)
{
  return MMRC_SUCCESS;
}


/**
  LockBarAddress

  @param[in]  MrcData

  @retval  None
**/
VOID LockBarAddress
  (MMRC_DATA         *MrcData )
{
#ifndef SIM
    MmrcExecuteTask (MrcData, LockAunitSliceChannel, NO_PRINT, 0);
  _asm {
    push eax
    push edx
    push ecx
    mov eax, 00h
    mov edx, 00h
    mov ecx, 0122h
    rdmsr
    or  edx, 80000000h
    wrmsr
    pop ecx
    pop edx
    pop eax
  }
#endif
}




/**
  Configure the memory thru several training steps

  @param[in,out]  MrcData  Host structure for all MRC global data.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
ConfigureMemory (
  IN  OUT   MMRC_DATA   *MrcData
)
{
  MMRC_STATUS        Status;
  UINT32             TempRegData;

#if MINIBIOS
  UINT32             Address;
  UINT32             Data;
  UINT32             i;
#endif
  TempRegData = 0;


  Status = MMRC_SUCCESS;

  ///
  /// Some routines require the MrcData host structure but we don't pass the structure
  /// to them for several reasons:
  ///   1) To save code space - 1 less parameter per function call on routines which
  ///      are frequently called.
  ///   2) Macros such as MrcDeadLoop() are able to be used from any routine. If a
  ///      routine does not have MrcData as a parameter, it can't pass it to MrcDeadLoop().
  ///
  SaveMrcHostStructureAddress (MrcData);


  ///
  /// Initialize other components such as UART, PUNIT, HPET, etc.
  ///
  ExternalSiliconInit (MrcData);

  ///
  ///
  /// Populate all input parameters that MMRC needs to execute.
  ///
  GetInputParameters (MrcData);
  ///
  /// Convert input parameters to actual values which will be written
  /// into registers. For example, if TCL of 11 needs a value of 6
  /// written to the DUNIT, this routine will change TCL to 6.
  ///
  //ConvertInputParameters (MrcData);

  PrintMemoryConfig (MrcData);

  //
  // FALSE = Start memory initialization
  //
#if !SIM && !JTAG
  RecordMrcStatus(MrcData, FALSE);
#endif

  ///
  /// Single entry point to MMRC to execute the entire flow.
  ///
  Status = MmrcEntry (MrcData, EntireMmrc, NULL);

  if (Status != MMRC_SUCCESS) {
    return Status;
  }
  FillOutputStructure (MrcData);
  //
  // If we're in the MiniBIOS, MRC has completed. Now need to deadloop and optionally
  // display all delays and dump all registers.
  //

  //
  // Disables the High Precision Event Timer
  //
  McEnableHpet (MrcData);

#ifdef BXTPMRC
  SetSliceChannelHash (MrcData);
  LockBarAddress (MrcData);
#endif

  //
  // TRUE = Complete memory initialization
  //
#if !SIM && !JTAG
  RecordMrcStatus(MrcData, TRUE);
#endif

  return Status;
}

/**
  Mark MRC as started so that if a reset occurs during the middle of MRC,
  we know the memory subsystem is partially initialized. In this case, we
  need to force a powergood reset to get the DDRIO back into a reset state
  where the registers are at default values.

  @param[in,out]  MrcData      Host structure for all MRC global data.
  @param[in]      Finish

  @retval  Success
  @retval  Failure
**/
VOID
RecordMrcStatus (
  IN  OUT   MMRC_DATA   *MrcData,
  IN  BOOLEAN           Finish
  )
{
  UINT32 PmcMrcStartedIndication ;
  //
  // Read PMC GCR PMCON1 register and clear dram init bit when complete. Set dram init bit to indicate MRC is not complete/in progress.
  //
  PmcMrcStartedIndication  = Mmio32Read(MrcData->PmcBar + PMC_GCR_BASE_ADDRESS + PMC_GCR_GEN_PMCON1);
  if (Finish) {
    Mmio32Write (MrcData->PmcBar + PMC_GCR_BASE_ADDRESS + PMC_GCR_GEN_PMCON1, (PmcMrcStartedIndication  & ~PMC_GCR_GEN_PMCON1_DRAM_INIT_BIT_STS));
  } else {
    Mmio32Write (MrcData->PmcBar + PMC_GCR_BASE_ADDRESS + PMC_GCR_GEN_PMCON1, (PmcMrcStartedIndication  & PMC_GCR_GEN_PMCON1_DRAM_INIT_BIT_STS));
  }
}
/**
  Routine to initialize other pieces of silicon which may be required
  for MRC to successfully initialize memory. High precision event timer,
  PUNIT mailbox, etc.

  @param[in,out]  MrcData       Host structure for all MRC global data.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
ExternalSiliconInit (
  IN  OUT   MMRC_DATA   *MrcData
)
{
#ifndef SIM
#ifdef MINIBIOS
#if DEBUG_MSG
 // Init16550Uart ();
#endif
#endif
  ///
  /// Get platform setup including all BAR address
  ///
  if (GetPlatformSettings (MrcData) == MMRC_FAILURE) {
    MrcDeadLoop ();
  }
  ///
  /// Enables the High Precision Event Timer
  ///
  McEnableHpet (MrcData);
  ///
  /// Initialize Punit Setting
  ///
  InitPunitSetting(MrcData);
#endif // SIM

  return MMRC_SUCCESS;
}

//
// DramDensity[]
// Used to convert the index value of MrcData->Channel[Channel].D_Size[0] into
// a decimal value in units of gigabits (Gb)
//
UINT8 DramDensity[5] = {
  4,  // 0 = 4Gb
  6,  // 1 = 6Gb
  8,  // 2 = 8Gb
  12, // 3 = 12Gb
  16  // 4 = 16Gb
};

/**
  Calculates and returns the rank size in megabytes for a given channel

  @param[in]  MrcData       Host structure for all data related to MMRC
  @param[in]  Channel       Target channel for rank size calculation

  @retval  RankSize         Rank size in MB for a given channel
**/
UINT16 CalculateRankSize(
  IN  MMRC_DATA *MrcData,
  IN  UINT8     Channel
  )
{
  UINT8  DQBusWidth;
  // 0 = x8; 1 = x16; 2 = x32
  UINT8  DeviceWidth;
  //  0000 =  4 Gb 0001 = 6 Gb  0010 = 8 Gb  0011 = 12 Gb
  //  0100 = 16 Gb
  UINT16 DramCapacity;
  UINT16 RankSize;

#if !defined (SIM) && !defined (JTAG) && !defined (CAR)
  BXT_SERIES BxtSeries;

  BxtSeries = GetBxtSeries ();
#endif

  DQBusWidth   = 8;
  DeviceWidth  = MrcData->Channel[Channel].D_DataWidth[0];
  DramCapacity = MrcData->Channel[Channel].D_Size[0];

  if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
    DQBusWidth = 32;
  } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
#if !defined (SIM) && !defined (JTAG) && !defined (CAR)
    if ( BxtSeries == Bxt1 ) {
#else
    if ( MrcData->CpuStepping == STEPPING_E0 || MrcData->CpuStepping == STEPPING_A0) {
#endif
      DQBusWidth = 16;
    } else {
      DQBusWidth = 32; //  32bit DQ bus (DRAMdevice_PR * device width)
    }
    //
    // JEDEC SPEC labels LPDDR4 DRAM Density per die. LPDDR4 will have
    // two DRAMs per die meaning that the actual device width will be
    // the sum of the width of each DRAM in each die. Here, we take this
    // into account by treating a x16 dual channel LPDDR4 device as a
    // x32 DRAM device.
    //
    DeviceWidth = 2; // increment from x16 to x32
  } else {
    if (MrcData->DramPolicyData.MemoryDown != MD_MEMORYDOWN) {
      if (MrcData->Channel[Channel].D_Size[0] == 0) {   //4Gb
        DramCapacity = 0;
      } else {
        DramCapacity = 2;            //8Gb
      }
    }
    DQBusWidth = 64;
  }

  RankSize = (DramDensity[DramCapacity] * 1024 ) / 8 *
        (DQBusWidth) /
        (8 << DeviceWidth );

  //MmrcDebugPrint((MMRC_DBG_MIN, "RankSize = %d, DQBusWidth = %d, DeviceWidth = %d, DramDensity[%d] = %d | ", RankSize, DQBusWidth, DeviceWidth, DramCapacity, DramDensity[DramCapacity]));
  return RankSize;
}

/**
  Fills in the complete parameter list for the Modular MRC.
  This includes all the Dynamic entries that are listed in
  the Input Spreadsheet, but also the PFCT values.
  Want to

  @param[in,out]  MrcData       Host structure for all data related to MMRC

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
  GatherSocConfigs (
  IN  OUT   MMRC_DATA            *MrcData
  )
{
  UINT8  Channel;
  DRP_DRAM_POLICY DrpPolicy;
  DRAM_POLICY DramPolicyData;
  UINT8 i;


  MmrcMemset (&DrpPolicy, 0, sizeof (DRP_DRAM_POLICY));
  MmrcMemset (&DramPolicyData, 0, sizeof (DRAM_POLICY));

  //
  // Get SMIP setting and convert it to MRC Data
  //
  if (MrcData->DramPolicyData.MemoryDown) {
    DramPolicyData.Profile  = MrcData->DramPolicyData.Profile;  // Add after smip available
    DramPolicyData.ChDrp[0] = MrcData->DramPolicyData.ChDrp[0];
    DramPolicyData.ChDrp[1] = MrcData->DramPolicyData.ChDrp[1];
    DramPolicyData.ChDrp[2] = MrcData->DramPolicyData.ChDrp[2];
    DramPolicyData.ChDrp[3] = MrcData->DramPolicyData.ChDrp[3];
  } else {
    // Set DDR3L profile for SODIMM

    switch (MrcData->CurrentFrequency) {
    case MMRC_1333:
      DramPolicyData.Profile = DDR3_1333_7_7_7;
      i = 8;
      break;
    case MMRC_1600:
      DramPolicyData.Profile = DDR3_1600_8_8_8;
      i = 9;
      break;
    case MMRC_1866:
      DramPolicyData.Profile = DDR3_1866_10_10_10;
      i = 11;
      break;
    default:
      DramPolicyData.Profile = DDR3_1333_7_7_7;
      i = 8;
      break;
    }

    while (i++ <= MrcData->Channel[0].Tcl)  {
      DramPolicyData.Profile += 1;
    }

    MmrcDebugPrint ((MMRC_DBG_MIN, "profile is %d freq is %d\n", DramPolicyData.Profile, MrcData->CurrentFrequency ));
  }
  MrcData->DVFS_FreqA = DramPolicyData.Profile;//  LPDDR4_3200_28_29_29;//LPDDR3_1600_12_12_12;
  MrcData->TimingParamsConfig = DramPolicyData.Profile;

  for (Channel = 0; Channel < MAX_CHANNELS; Channel++) {

    #ifdef BXT_PERIODIC_ENABLED
#if !defined (SIM) && !defined (JTAG) && !defined (CAR)
    if ( GetBxtSeries() == Bxt1 ) {
#else
    if ( MrcData->CpuStepping == STEPPING_E0 ) {
#endif
      MrcData->DynamicVars[Channel][BXT_PERIODIC_ENABLED]=1;
    } else {
      MrcData->DynamicVars[Channel][BXT_PERIODIC_ENABLED]=1;
    }
    #endif

    if (MrcData->DramPolicyData.MemoryDown != 0) {
      switch (Channel) {
      case 0:
        Mmrcmemcpy(&DrpPolicy, &(DramPolicyData.ChDrp[0]), sizeof(DRP_DRAM_POLICY));
        break;
      case 1:
        Mmrcmemcpy(&DrpPolicy, &(DramPolicyData.ChDrp[1]), sizeof(DRP_DRAM_POLICY));
        break;
      case 2:
        Mmrcmemcpy(&DrpPolicy, &(DramPolicyData.ChDrp[2]), sizeof(DRP_DRAM_POLICY));
        break;
      case 3:
        Mmrcmemcpy(&DrpPolicy, &(DramPolicyData.ChDrp[3]), sizeof(DRP_DRAM_POLICY));
        break;
      default:
        Mmrcmemcpy(&DrpPolicy, &(DramPolicyData.ChDrp[0]), sizeof(DRP_DRAM_POLICY));
        break;
      }

      if (DrpPolicy.RankEnable != 0) {
        MrcData->Channel[Channel].Enabled = TRUE;
        MrcData->Channel[Channel].DimmPresent[0] = 1;
        MrcData->Channel[Channel].RankEnabled[0] = (BOOLEAN) DrpPolicy.RankEnable & 0x1;
        MrcData->Channel[Channel].RankEnabled[1] = (BOOLEAN) ((DrpPolicy.RankEnable >> 1) & 0x1);
        MrcData->Channel[Channel].D_Size[0] = (UINT8) (DrpPolicy.DramDensity & 0x7); // DeviceDensity16Gb;
        MrcData->Channel[Channel].D_BusWidth[0] = 1;
        MrcData->Channel[Channel].D_DataWidth[0] = (UINT8) (DrpPolicy.DeviceWidth & 0x3);// 1;

        MrcData->Channel[Channel].DramType =(UINT8)  ConfigToDdrType[TimingParamHash (DramPolicyData.Profile - 1, TRUE)];
        MrcData->Channel[Channel].ASR_Supported = MrcData->DramPolicyData.DDR3LASR;
      }
    }

    if ((MrcData->Channel[Channel].RankEnabled[0] || MrcData->Channel[Channel].RankEnabled[1]) == 0){
      MrcData->Channel[Channel].Enabled = FALSE;
    }

    if (MrcData->Channel[Channel].Enabled) {
      if (MrcData->Channel[Channel].RankEnabled[0] && MrcData->Channel[Channel].RankEnabled[1]) {
        MrcData->Channel[Channel].D_Ranks[0] = 2;
      } else {
        MrcData->Channel[Channel].D_Ranks[0] = 1;
      }
      MmrcDebugPrint((MMRC_DBG_MIN, "Ch[%d] %d rank\n", Channel, MrcData->Channel[Channel].D_Ranks[0]));
    }
  }

  MrcData->DigitalDll = DIGITALDLL;

  // Enable 2nd channel to configure PHY for 1x32 lpddr4 config
  if (MrcData->Channel[0].DramType == TypeLpDdr4) {
    if (MrcData->DramPolicyData.MemoryDown == 3) {
      MrcData->Channel[1].Enabled = TRUE;
    }
  }




  for (Channel = 0; Channel < MAX_CHANNELS; Channel++) {

    if (!MrcData->Channel[Channel].Enabled) {
      continue;
    }

    MrcData->Channel[Channel].TotalMem = 0;


    //
    // Look at both DIMMs and fill in DIMM-specific variables. Also add up total memory
    // per channel and system-wide.
    //

    //if (MrcData->Channel[Channel].DimmPresent[0]) {
    if (MrcData->Channel[Channel].RankEnabled[0]) {

      MrcData->Channel[Channel].SlotMem[0] = CalculateRankSize(MrcData, Channel) * MrcData->Channel[Channel].D_Ranks[0];

      MrcData->Channel[Channel].TotalMem  += MrcData->Channel[Channel].SlotMem[0];
      MrcData->SystemMemorySize           += MrcData->Channel[Channel].SlotMem[0];

      MmrcDebugPrint((MMRC_DBG_MIN, "DIMM%d Memory Size: %5d, System Mem %5d in MB\n", Channel, MrcData->Channel[Channel].SlotMem[0], MrcData->SystemMemorySize));
      if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
        MrcData->NumberOfChannels[TypeLpDdr3] += 1;
      }
      if (MrcData->Channel[Channel].DramType == TypeDdr3L) {
        MrcData->NumberOfChannels[TypeDdr3L] += 1;
      }
      if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
        MrcData->NumberOfChannels[TypeLpDdr4] += 1;
      }
    } //rankEnabled

    // MrcData->CurrentFrequency = MrcData->Channel[Channel].CurrentFrequency;

    switch (MrcData->Channel[Channel].DramType) {
    case TypeLpDdr3:
      MrcData->Channel[Channel].CurrentDdrType = T_LPDDR3;// >> 1;
      MrcData->CurrentBlueprint = 0;   //Lp3 : 0; LP4: 1
      MrcData->Channel[Channel].MaxDq[0] = 4;
      MrcData->Channel[Channel].MaxDq[1] = 4;
      break;
    case TypeLpDdr4:
      MrcData->Channel[Channel].CurrentDdrType = T_LPDDR4;// >> 1;
      if (MrcData->BxtSeries == SERIES_BXT || MrcData->BxtSeries == SERIES_BXT_1 ) {
        MrcData->CurrentBlueprint = 1;   //Lp3 : 0; LP4: 1
        MrcData->Channel[Channel].MaxDq[0] = 2;
        MrcData->Channel[Channel].MaxDq[1] = 2;
      }
      if (MrcData->BxtSeries == SERIES_BXT_P) {
        MrcData->CurrentBlueprint = 0;   //Lp3 : 0; LP4: 1
        MrcData->Channel[Channel].MaxDq[0] = 4;
        MrcData->Channel[Channel].MaxDq[1] = 4;
      }
      break;
    case TypeDdr3L:
      MrcData->Channel[Channel].CurrentDdrType = T_DDR3L;// >> 1;
      MrcData->CurrentBlueprint = 1;  //Lp3 : 0; LP4: 1
      MrcData->Channel[Channel].MaxDq[0] = 8;
      MrcData->Channel[Channel].MaxDq[1] = 8;
      break;
    default:
      MrcData->Channel[Channel].CurrentDdrType = T_LPDDR3;// >> 1;
      break;
    }
    MrcData->MaxElements = MrcData->Channel[Channel].MaxDq[0];
    MrcData->MaxBits =  MAX_BITS;  //assign to maxDq initially
  }

  return MMRC_SUCCESS;
}

/**
  PrintMemoryConfig

  @param[in]  MrcData

  @retval  None
**/
void
  PrintMemoryConfig(
  IN  MMRC_DATA   *MrcData
  )
{
  UINT8  Channel;

  MmrcDebugPrint ((MMRC_DBG_MAX, "MRCDATA Size: %x\n", sizeof(MrcData)));
  MmrcDebugPrint ((MMRC_DBG_MIN, "BootMode set to: %x (FB=0x10; S5=8; S0=1,2,4; S3=0x20)\n",MrcData->BootMode));
  MmrcDebugPrint ((MMRC_DBG_MIN, "CurrentFreq = %d    (1600 = %d, 1866  = %d, 2133 = %d, 2666 = %d, 3200 = %d)           \n", MrcData->CurrentFrequency, MMRC_1600, MMRC_1866, MMRC_2133, MMRC_2666, MMRC_3200));
  MmrcDebugPrint ((MMRC_DBG_MIN, "SystemMemSize = %x\n", MrcData->SystemMemorySize));

  for (Channel = 0; Channel < MAX_CHANNELS; Channel++) {
    if (MrcData->Channel[Channel].Enabled) {
        MmrcDebugPrint ((MMRC_DBG_MIN, "CH %d Enabled ", Channel));
      if (MrcData->Channel[Channel].DramType == TypeLpDdr3) {
        MmrcDebugPrint ((MMRC_DBG_MIN, "LPDDR3\n"));
      } else if (MrcData->Channel[Channel].DramType == TypeDdr3L) {
        MmrcDebugPrint ((MMRC_DBG_MIN, "DDR3L\n"));
      } else if (MrcData->Channel[Channel].DramType == TypeLpDdr4) {
        MmrcDebugPrint ((MMRC_DBG_MIN, "LPDDR4\n"));
      } else {
        MmrcDebugPrint ((MMRC_DBG_MIN, "Unknown mem type\n"));
      }
    }
  }

}

/**
  This routine is responsible to initialize ALL data required for
  MMRC to initialize memory. Typical flow for server would be:

  1) DIMM detection
  2) Timing calculations
  3) Variable initialization
  4) BIOS Setup overrides

  Typical phone/tablet flow would be:

  1) Timing variable population via lookup table
  2) BIOS Setup overrides

  This routine can be very complex in server segments or extremely
  lightweight in phone/tablet segments, depending on code size and
  MRC execution time requirements.

  @param[in,out]  MrcData  Host structure for all MRC global data.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
GetInputParameters (
  IN  OUT   MMRC_DATA   *MrcData
)
{
  OemGetPlatformType (MrcData);
  //
  // Sets the OEM specific information
  //
  OemSetMrcData (MrcData);
#if !defined (CAR) && !defined (JTAG)
  if (GetFibType() == BxtSoftFib) {  // BIT2 = BXT E0 Soft FIB
#else
  if (TRUE) {
#endif

    MmrcDebugPrint ((MMRC_DBG_MIN, "BXT E0 Soft FIB Detected. Forcing S5 Boot in MRC.\n"));

    // This should never be checked in.
    MrcData->BootMode = S5;
  } else {
    if (MrcData->BootMode != S3) {
      DetermineBootMode (MrcData);
    } else {
      if (MrcData->MrcParamsValidFlag == 0) {   //if S3 and restored data not valid.
        MmrcDebugPrint ((MMRC_DBG_MIN, "Forced powergood reset. Vars not valid in S3 resume\n"));
        IoOut8 (0xCF9, 0x0);
        IoOut8 (0xCF9, 0xE);
#if !defined SUSSW
        _asm hlt
#endif
        return MMRC_FAILURE;
      }
    }
  }


  ///
  /// Select to use the restore path here we decide here after DetermineBootMode
  /// and DetectDimms, this may change for memory down we decide here in case a define
  /// prevents DetermineBootMode or DetectDimms functions to execute
  ///
//  if ((MrcData->BootMode != S5) && (MrcData->CpuStepping != STEPPING_A0)) {
  MmrcDebugPrint ((MMRC_DBG_MIN, "CurrentBootMode -> %d ValidFlag -> %d\n", MrcData->BootMode, MrcData->MrcParamsValidFlag));  //KW REMOVE
  if ((MrcData->BootMode != S5) && (MrcData->MrcParamsValidFlag == 1)) {
    MrcData->RestorePath = TRUE;
    MmrcDebugPrint ((MMRC_DBG_MIN, "Restore Path\n"));
  } else {
    MrcData->RestorePath = FALSE;
    MmrcDebugPrint ((MMRC_DBG_MIN, "NOT Restore Path\n"));
  }
#if DDR3_SUPPORT || DDR4_SUPPORT
  if ((MrcData->BootMode == S5) && (MrcData->DramPolicyData.MemoryDown != 1)) {
    DetectDimms (MrcData);
  }
#endif

  GatherSocConfigs (MrcData);

  DetermineCommonFreq (MrcData);

  if (MrcData->DramPolicyData.MemoryDown == 2) {
    if (MrcData->DDR3L_PageSize < MrcData->DramPolicyData.DDR3LPageSize) {
      MrcData->DDR3L_PageSize = MrcData->DramPolicyData.DDR3LPageSize;
    }
    /*
    MrcData->Channel[0].CurrentPlatform = P_SIL_MD;
    if (MrcData->Channel[1].Enabled) {
      MrcData->Channel[1].CurrentPlatform = P_SIL_SD;
    }*/

    //determine the common freq and timing
    //DetermineCommonTiming (MrcData);

  }

  //
  // Save DIMM information for fast boot path comparison
  //
#if DDR3_SUPPORT || DDR4_SUPPORT
  if ((MrcData->Channel[0].DramType == TypeDdr3L) && (MrcData->DramPolicyData.MemoryDown != 1)) {
    FillInputStructure (MrcData);
  }
#endif

  return MMRC_SUCCESS;
}

/**
  This routine assumes all input parameters to the MMRC have been
  populated by GetInputParameters(). Converting all timing variables
  to the correct values for DUNIT programming is the responsibility
  of this function.

  @param[in,out]  MrcData  Host structure for all MRC global data.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
ConvertInputParameters (
  IN  OUT   MMRC_DATA   *MrcData
)
{



  return MMRC_SUCCESS;
}

/**
  ProgramMemoryMap

  @param[in]  MrcData  Host structure for all MRC global data.
  @param[in]  CapsuleStartIndex
  @param[in]  StringIndex
  @param[in]  Channel

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
ProgramMemoryMap (
  MMRC_DATA         *MrcData,
  UINT16            CapsuleStartIndex,
  UINT16            StringIndex,
  UINT8             Channel
)
{
  UINT32        MmioAllocation;
  UINT32        TOM;
  UINT32        LowMemory;
  BOOLEAN       ReclaimEn;
  UINT32        HighMemory;
  UINT16        TSegSize;
  UINT32        TSegBase;
  UINT16        GMSsize;
  UINT16        GTTsize;
  UINT32        GMSbase;
  UINT32        GTTbase;
  UINT32        SeCUmaBase;
  UINT32        TempTSEGbase;
//  UINT32    SeCfTPMUmaBase;
  REGISTER_ACCESS               Register;
  GGC_0_0_0_PCI_CUNIT_STRUCT    Cuint_GGC;
  UINT32                        Buffer32;

  SeCUmaBase = 0;
#ifdef FTPM_ENABLE
  UINT32  SeCfTPMUmaBase;
#endif

#ifdef ISH_ENABLE
  UINT32  IshBase;
#endif

  // BXT_B0_FIB: Booted to Windows With TsegSize=0x4
  MrcData->OemMrcData.TsegSize = 0x4;

  MrcData->OemMrcData.MmioSize = 0x400;


  GMSsize = 0;
  GTTsize = 0;
  TSegSize = 0;
  Buffer32 = 0;

  Register.Mask = 0xFFFFFFFF;
  Register.ShiftBit = 0;

  Register.Offset = GGC_0_0_0_PCI_CUNIT_REG;
  Cuint_GGC.Data = MemRegRead (CUNIT, 0, 0, Register);


  //All unit in 1MB granularity
  if(Cuint_GGC.Bits.gms < 0x40){
    GMSsize = (UINT16) Cuint_GGC.Bits.gms * 32;
  } else {
    MmrcDebugPrint ((MMRC_DBG_MIN, "Warning GMSsize is zero\n"));
    //return MMRC_FAILURE;
  }

  if(Cuint_GGC.Bits.ggms > 0x0){
    GTTsize = 1 << Cuint_GGC.Bits.ggms;
  }


  if ((MrcData->DramPolicyData.SystemMemorySizeLimit != 0) &&
      (MrcData->DramPolicyData.SystemMemorySizeLimit < MrcData->SystemMemorySize)) {
        MrcData->SystemMemorySize = MrcData->DramPolicyData.SystemMemorySizeLimit;
  }

  TSegSize       = MrcData->OemMrcData.TsegSize;
  MmioAllocation = MrcData->OemMrcData.MmioSize;
  TOM            = MrcData->SystemMemorySize;

  // LowMemory not = 0x800 could reintroduce x64 bit addressing issues in some of the IAFW drivers
  //LowMemory       = MIN (0x1000 - MmioAllocation, TOM);//LowMemory       = 0x800;  //LowMemory       = MIN (0x1000 - MmioAllocation, TOM);
  LowMemory       = 0x800;

  if ((MrcData->DramPolicyData.LowMemMaxVal != 0) &&
      (MrcData->DramPolicyData.LowMemMaxVal < LowMemory)) {
    LowMemory = MrcData->DramPolicyData.LowMemMaxVal;
  }

  MmrcDebugPrint ((MMRC_DBG_MIN, "LowMemory:      0x%08x\n", LowMemory));

  ReclaimEn = FALSE;

  if (TOM > LowMemory) {
    ReclaimEn = TRUE;
  }

  if (ReclaimEn) {
    HighMemory = TOM - LowMemory + 0x1000;
  } else {
    HighMemory = TOM;
  }

  if ((MrcData->DramPolicyData.HighMemMaxVal != 0) &&
      (MrcData->DramPolicyData.HighMemMaxVal < HighMemory)) {
    HighMemory = MrcData->DramPolicyData.HighMemMaxVal;
  }
  MmrcDebugPrint ((MMRC_DBG_MIN, "HighMemory:     0x%08x\n", HighMemory));

#if 0  // allocate in memoryinit.c
#ifdef SEC_SUPPORT_FLAG

#ifdef FTPM_ENABLE
  SeCfTPMUmaBase = LowMemory -  MrcData->SeCfTPMUmaSize;
  CurrentMrcData->SeCfTPMUmaBase = SeCfTPMUmaBase;
  SeCUmaBase = SeCfTPMUmaBase - CurrentMrcData->SeCUmaSize;
  CurrentMrcData->SeCUmaBase = SeCUmaBase;
#else
//  SeCUmaBase = LowMemory - CurrentMrcData->SeCUmaSize;
  //Temp
  SeCUmaBase = SeCfTPMUmaBase = LowMemory -  MrcData->SeCUmaSize;
   MrcData->SeCUmaBase = SeCUmaBase;
#endif

#else
  SeCUmaBase = SeCfTPMUmaBase = LowMemory;
  MrcData->SeCUmaBase = SeCUmaBase;
#endif
#endif

#ifdef ISH_ENABLE
  //
  // 1MB alloc for ISH
  //
  #ifdef FTPM_ENABLE
  IshBase = SeCfTPMUmaBase - CurrentMrcData->IshUmaSize;
  #else
  IshBase = SeCUmaBase - CurrentMrcData->IshUmaSize;
  #endif
  //
  //9. Determine GFX Memory base, which is calculated by the value of TOLUD minus GFX size.
  //
  GMSbase = IshBase - GMSsize;
#else
  //
  //9. Determine GFX Memory base, which is calculated by the value of TOLUD minus GFX size.
  //
  GMSbase = LowMemory - GMSsize;
#endif

  GTTbase = GMSbase - GTTsize;

  MmrcDebugPrint ((MMRC_DBG_MIN, "SeCUmaBase 0x%x GMSsize 0x%x GMSbase 0x%x GTTbase 0x%x\n", SeCUmaBase,GMSsize,GMSbase,GTTbase));

  ///
  ///11. Determine TSEG base, which is calculated by the value of TOLUD minus GFX size minus TSEG size.
  ///
  TempTSEGbase = GTTbase - (GTTbase & (TSegSize - 1));
  TSegBase = TempTSEGbase - TSegSize;

///////////////////////////////////////////////////////
///Program the registers
///////////////////////////////////////////////////////


  MmrcDebugPrint ((MMRC_DBG_MIN, "TSegSize:       0x%08x\n", TSegSize));
  MmrcDebugPrint ((MMRC_DBG_MIN, "TSegBase:       0x%08x\n", TSegBase));
  MmrcDebugPrint ((MMRC_DBG_MIN, "MmioAllocation: 0x%08x\n", MmioAllocation));
  MmrcDebugPrint ((MMRC_DBG_MIN, "TOM:            0x%08x\n", TOM));
  MmrcDebugPrint ((MMRC_DBG_MIN, "TOLUD:          0x%08x\n", LowMemory << 20));
  MmrcDebugPrint ((MMRC_DBG_MIN, "TOUUD_LO:       0x%08x\n", HighMemory << 20));
  MmrcDebugPrint ((MMRC_DBG_MIN, "TOUUD_HI:       0x%08x\n", HighMemory >> 12));

  Register.Mask = 0xFFFFFFFF;
  Register.ShiftBit = 0;

  Register.Offset = TOLUD_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (LowMemory << 20) + BIT0, 0xF);
  Register.Offset = TOUUD_LO_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (HighMemory << 20) & 0xFFFFFFFF, 0xF);
  Register.Offset = TOUUD_HI_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (HighMemory >> 12), 0xF);
  Register.Offset = BDSM_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (GMSbase << 20) + BIT0, 0xF);
  Register.Offset = BGSM_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (GTTbase << 20) + BIT0, 0xF);
  Register.Offset = TSEGMB_0_0_0_PCI_CUNIT_REG;
  MemRegWrite (CUNIT, 0, 0, Register, (TSegBase << 20) + BIT0, 0xF);

  return MMRC_SUCCESS;
}

