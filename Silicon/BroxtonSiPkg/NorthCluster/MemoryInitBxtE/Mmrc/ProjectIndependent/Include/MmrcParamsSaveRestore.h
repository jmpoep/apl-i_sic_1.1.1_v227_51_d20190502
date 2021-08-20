/** @file
  MmrcParamsSaveRestore.h
  Defines the MRC Parameters Save and Restore Structure.

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
#ifndef _MMRC_PARAMS_SAVE_RESTORE_H_
#define _MMRC_PARAMS_SAVE_RESTORE_H_

#include "MmrcProjectDefinitions.h"
#include "Spd.h"

typedef enum {
  BootFrequency,
  HighFrequency,
  MaxFrequencyIndex
} FREQUENCY_INDEX;

/**
  Final training values stored on a per blueprint level. Needs to be per blueprint
  in case of a system with more than 1 level of memory per channel.
**/
typedef struct {
  UINT16    Values[MAX_BLUEPRINTS][MAX_NUM_ALGOS][MAX_RANKS][MAX_STROBES];
} TRAINING_SETTING;

typedef struct {
  UINT8   SpdPresent;
  UINT8   Buffer[MAX_SPD_ADDR + 1];
} SPD_DATA;

typedef struct {
  UINT8 ManuIDlo;
  UINT8 ManuIDhi;
  UINT8 ManuLoc;
  UINT8 ManuDateLO;
  UINT8 ManuDateHI;
  UINT8 SerialNumber1;
  UINT8 SerialNumber2;
  UINT8 SerialNumber3;
  UINT8 SerialNumber4;
  UINT8 DimmCount;
} FASTBOOTDATA;

#if FAULTY_PART_TRACKING
typedef struct {
  UINT32   Byte0to3;                  ///> On the fly tracking of the minimum bit failure used by all trainings
  UINT32   Byte4to7;                  ///> On the fly tracking of the minimum failure bit
  UINT8    ByteEcc;                   ///> On the fly tracking of the minimum failure bit
  UINT16   TrainingFail;              ///> Training that failed due to faulty part
  UINT32   DataBitFailCumulativeLow;  ///> Field to cumulate bit failures found as training progresses for data bits
  UINT32   DataBitFailCumulativeHigh; ///> Field to cumulate bit failures found as training progresses for data bits
  UINT8    EccBitFailCumulative;      ///> Field to cumulate bit failures found as training progresses for ECC byte
  UINT8    ErrorType;                 ///> 0- No error, 1-Correctable, 2-Uncorrectable
} FAULTY_PART;
#endif

typedef struct {
  TRAINING_SETTING          Trained_Value;
  BOOLEAN                   Enabled;
  UINT32                    TotalMem;
  UINT8                     DimmCount;
  UINT8                     DimmPresent[MAX_DIMMS];
  UINT32                    SlotMem[MAX_DIMMS];
  SPD_DATA                  SpdData[MAX_DIMMS];
  UINT8                     D_Ranks[MAX_DIMMS];
  UINT8                     D_DataWidth[MAX_DIMMS];
  UINT8                     D_Banks[MAX_DIMMS];
  UINT8                     D_BusWidth[MAX_DIMMS];
  UINT8                     D_Size[MAX_DIMMS];
  UINT8                     D_Type[MAX_DIMMS];
  UINT16                    DimmFrequency[MAX_DIMMS];
  BOOLEAN                   RankEnabled[MAX_RANKS];
  BOOLEAN                   EccEnabled;
  UINT32                    EccStrobes;
  UINT8                     MaxDq[MAX_RANKS];
  FASTBOOTDATA              FastBootData[MAX_DIMMS];
  UINT16                    DramType;
  UINT16                    CurrentPlatform;
  UINT16                    CurrentFrequency;
  UINT16                    CurrentConfiguration;
  UINT16                    CurrentDdrType;
  BOOLEAN                   ExecuteOnThisChannel;
  UINT16                    ScramblerSeed;
  UINT16                    TimingData[16];
  //UINT32                  CachedValues[MAX_NUM_CACHE_ELEMENTS][MAX_RANKS][MAX_STROBES];
  //UINT8                   DimmVrefWrite[MAX_RANKS][MAX_STROBES];
  INT16                      RMT_Data[MAX_RANKS + PSEUDO_RANKS][MAX_RMT_ELEMENTS][2];
#if RMT_PER_BIT_MARGIN
  INT16                      RMT_DQData[MAX_RANKS + PSEUDO_RANKS][MAX_RMT_ELEMENTS][MAX_BITS*MAX_STROBES][2];
#endif
  UINT8                     Tcl;
  //UINT8                   VrefSmbAddress;
  //UINT8                   TwoClkRdPreamble;
  //UINT8                   TwoClkWrPreamble;
  UINT8                     Dimm2DunitMapping[32];
  UINT8                     Dimm2DunitReverseMapping[32];
  //MRS_COMMAND_SX          MrsCommandSx[MAX_RANKS];
#if SIM && DDR4_SUPPORT
  UINT16                    TbMr5;
#endif
#if FAULTY_PART_TRACKING
  FAULTY_PART               FaultyPart[MAX_RANKS];
#endif
  UINT32                    WrVref[MAX_RANKS][MAX_STROBES];
  UINT32                    CaVref[MAX_RANKS];
  UINT32                    Storage[18];
  UINT8                     ASR_Supported;
  UINT8                     OdtHigh;
  UINT16                    LP4_MR0VALUE;
  UINT16                    LP4_MR4VALUE;
} CHANNEL;

#define MRC_PARAMS_SAVE_RESTORE_DEF        \
  UINT32        Signature;                 \
  UINT8         MrcParamsValidFlag;        \
  UINT16        BootMode;                  \
  UINT16        CurrentFrequency;          \
  UINT8         DVFSFreqIndex;             \
  UINT8         TotalDimm;                 \
  UINT16        DDR3L_MRVALUE;             \
  UINT8         DDR3L_PageSize;            \
  UINT32        SystemMemorySize;          \
  UINT32        MrcVer;                    \
  UINT32        CachedValues[MAX_CHANNELS][MAX_NUM_CACHE_ELEMENTS][MAX_RANKS][MAX_STROBES]; \
  UINT32        CachedValuesPB[MAX_CHANNELS][MAX_NUM_CACHE_ELEMENTS_PB][MAX_RANKS][MAX_STROBES][MAX_BITS]; \
  UINT8         BusWidth;                  \
  DRAM_POLICY   DramPolicyData;            \
  CHANNEL       Channel[MAX_CHANNELS];     \
  UINT32        MarginCheckResultState;    \
  UINT32        MuxcodeNv[MaxFrequencyIndex]; \
  BOOLEAN       SetupMaxPiDone[MaxFrequencyIndex]; \
  UINT32        SaMemCfgCrc;               \
  UINT32        MrcParamsSaveRestoreCrc;

//
// Do not add or change this structure.
//
typedef struct {
  MRC_PARAMS_SAVE_RESTORE_DEF
} MRC_PARAMS_SAVE_RESTORE;

#endif
