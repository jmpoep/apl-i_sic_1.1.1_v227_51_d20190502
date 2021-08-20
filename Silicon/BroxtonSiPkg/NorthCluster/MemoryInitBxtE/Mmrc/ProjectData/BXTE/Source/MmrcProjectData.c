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

  MmrcProjectData.c

Abstract:

  This file contains data structures for AVN silicon.
  These structures are fed to MmrcLibraries.c, which use them to initialize
  memory.

--*/

#include "MmrcProjectData.h"
#include "MmrcLibraries.h"

CONST UINT16 Timing2x[]= {
  1250,     // 800
  938,      // 1066
  750,      // 1333
  625,      // 1600
  468,      // 2133
  416,      // 2400
  375,      // 2667
  341,      // 2933
  312       // 3200
};



CONST UINT8 LinearToPhysicalVrefCodes[MAX_ELEMENT_TABLE] = { // lowest to highest
  0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 0x00, 0x01, // 00 - 09
  0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, // 10 - 19
  0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, // 20 - 29
  0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, // 30 - 39
};

CONST UINT8 PhysicalToLinearVrefCodes[MAX_ELEMENT_TABLE] = { // lowest to highest
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, // 00 - 09
  0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, // 10 - 19
  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, // 20 - 29
  0x26, 0x27, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, // 30 - 39
};

#if RMT
RMT_SIGNAL RmtTable[MAX_RMT_ELEMENTS] = {
  //{Signal,    SkipInitialProgrammming,  CpgcConfig,       StepSize,           RankChoices,               JedecrestType  }
  {RxDqsPDelay,       FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {RxDqsNDelay,       FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {TxDqDelay,         FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {RxVref,            FALSE,          CPGC_CONFIG_VA,        1,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
//  {TxVref,            FALSE,          CPGC_CONFIG_VA,        5,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {CmdAll,            TRUE,           CPGC_CONFIG_CMD_STRESS,4,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_FAIL_RESTORE_PI_CENTER|RMT_FAIL_JEDEC_RESET |RMT_FAIL_RESTORE_PI_LAST_VALUE    },
  //{CkAll,             TRUE,           CPGC_CONFIG_VA,        5,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_FAIL_RESTORE_PI_CENTER|RMT_FAIL_JEDEC_RESET|RMT_FAIL_RESTORE_PI_LAST_VALUE     },
  //{CtlAll,          TRUE,           CPGC_CONFIG_VA,        5,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_FAIL_RESTORE_PI_CENTER|RMT_FAIL_JEDEC_RESET|RMT_FAIL_RESTORE_PI_LAST_VALUE     },
  {MMRC_NA,           MMRC_NA,        MMRC_NA,               MMRC_NA,     MMRC_NA,                    MMRC_NA } ,
};

// In the production RMT table, the "StepSize" is actually the limit.
RMT_SIGNAL RmtTableProduction[MAX_PRODUCTION_RMT_ELEMENTS] = {
  {RxDqsPDelay,       FALSE,          CPGC_CONFIG_VA,        7,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {RxDqsNDelay,       FALSE,          CPGC_CONFIG_VA,        7,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {TxDqDelay,         FALSE,          CPGC_CONFIG_VA,        8,   RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,             RMT_NO_ACTION     },
  {MMRC_NA,           MMRC_NA,        MMRC_NA,            MMRC_NA,     MMRC_NA,                           MMRC_NA }  ,
};

RMT_SIGNAL RmtTablePpv[MAX_PPV_RMT_ELEMENTS] = {
  //{Signal,    SkipInitialProgrammming,  CpgcConfig,       StepSize,           RankChoices,               JedecrestType  }
  {RxDqsPDelay,       FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {RxDqsNDelay,       FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {TxDqDelay,         FALSE,          CPGC_CONFIG_VA,        2,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {RxVref,            FALSE,          CPGC_CONFIG_VA,        1,    RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {TxVref,            FALSE,          CPGC_CONFIG_VA,        20,   RMT_PR_EACH_RANK|RMT_PR_LFSR|RMT_PR_TA,            RMT_NO_ACTION     },
  {MMRC_NA,           MMRC_NA,        MMRC_NA,               MMRC_NA,     MMRC_NA,                    MMRC_NA } ,
};
#endif

//tCK in ps
UINT16 TCK[C_MAXDDR] = {
//800   1066  1333  1600  1866  2133 2400 2600  3200
  2500, 1876, 1500, 1250, 1071, 937, 833, 750,  625
};

//
// Depending on the configuration of memory, this table returns the speed associated with that config.
//

UINT8 ConfigToSpeed[MaxMemoryTypeFreqConfig - 1] = {
  MMRC_1600, MMRC_2133, MMRC_2666, MMRC_3200
};

//
// Depending on the configuration of memory, this table returns the speed associated with that config.
//
UINT8 ConfigToSpeedMultiple133[MaxMemoryTypeFreqConfig - 1] = {
  12, 16, 20, 24
};


//
// Depending on the configuration of memory, this table returns the speed associated with that config.
//
UINT8 ConfigToDdrType[MaxMemoryTypeFreqConfig - 1] = {
  TypeLpDdr4, TypeLpDdr4, TypeLpDdr4, TypeLpDdr4
};

//
// TimingParamsUnder256[][MaxMemoryTypeFreqConfig]
// - This table holds timing paramters that can be described with a UINT8 data type.
//   This means that values must be less than 256
//
UINT8 TimingParamsUnder256[][MaxMemoryTypeFreqConfig] = {
//Remark:
//13=4-03-2014: Txp LPDDR4 14-15-15 Excel:6 Sim Model: 8
//13=4-03-2014: Tfaw LPDDR4 14-15-15 Excel:32 Sim Model: 40
//13=4-03-2014: Trwdr LPDDR4 14-15-15 Excel:23 Sim Model: 24
  //
  // Configs supported:
  //                      L     L     L     L
  //                      P     P     P     P
  //                      D     D     D     D
  //                      D     D     D     D
  //                      R     R     R     R
  //                      4     4     4     4
  //                      -     -     -     -
  //                      1     2     2     3
  //                      6     1     6     2
  //                      0     3     6     0
  //                      0     3     6     0
  //
  //                      14    20    24    28
  //                      -     -     -     -
  //                      15    20    24    29
  //                      -     -     -     -
  //                      15    20    24    29
  //
  {  BxtTimingTcl,        14,   20,   24,   28,    },
  {  BxtTimingTcwl,       12,   18,   22,   26,    },
  {  BxtTimingTrcd,       15,   20,   24,   29,    },
  {  BxtTimingTrpab,      17,   23,   28,   34,    },
  {  BxtTimingTras,       34,   45,   56,   68,    },
  {  BxtTimingTrrd,        8,   11,   14,   16,    },
  {  BxtTimingTfaw,       32,   43,   54,   64,    },
  {  BxtTimingTccd,       16,   16,   16,   16,    },
  {  BxtTimingTwtr,        8,   11,   14,   16,    },
  {  BxtTimingTwr,        15,   20,   24,   29,    },
  {  BxtTimingTrtp,        8,    8,   10,   12,    },
  {  BxtTimingTwtp,       44,   55,   63,   72,    },
  {  BxtTimingTckesr,      6,    8,   10,   12,    },
  {  BxtTimingTxp,         6,    8,   10,   12,    },
  {  BxtTimingTcke,        6,    8,   10,   12,    },
  {  BxtTimingTrdpden,    34,   41,   46,   51,    },
  {  BxtTimingTwrpden,    45,   55,   63,   73,    },
  {  BxtTimingTrrsr,      16,   16,   16,   16,    },
  {  BxtTimingTrrdr,      20,   21,   21,   23,    },
  {  BxtTimingTwwsr,      16,   16,   16,   16,    },
  {  BxtTimingTwwdr,      24,   24,   25,   27,    },
  {  BxtTimingTwrsr,      37,   46,   53,   59,    },
  {  BxtTimingTwrdr,      20,   21,   21,   22,    },
  {  BxtTimingTrwsr,      26,   27,   29,   30,    },
  {  BxtTimingTrwdr,      26,   27,   29,   30,    },
  {  BxtTimingTzqcs,       0,    0,    0,    0,    },
  {  BxtTimingTzqlat,     24,   32,   40,   48,    },
  {  BxtTimingTccdmw,     32,   32,   32,   32,    },
  {  BxtTimingTcksrx,      3,    3,    3,    0,    },
  {  BxtTimingTcksre,      5,    6,    7,    8,    },
  {  BxtTimingTrppb,      15,   20,   24,   29,    },
  {  BxtTimingTmod ,      12,   15,   19,   23,    },
  {  BxtTimingTrfc6Gb,    96,   96,   96,   96,    },
  {  BxtTimingTxsr6Gb,   102,  104,  106,  108,    },
//Please add new timing param at the end of table and make sure new timing param name is define in sequence under TIMING_DATA_TYPE
};

//
// TimingParamsUnder256[][MaxMemoryTypeFreqConfig]
// - This table holds timing paramters that can be described with a UINT16 data type but not a UINT8
//   Onlt use this table for timings that are larger than 255.
//
UINT16 TimingParamsOver255[][MaxMemoryTypeFreqConfig] = {
  {  BxtTimingTrfc,     144,  216,  240,  288,   },
  {  BxtTimingTrefi,   3125, 4167, 5208, 6250,   },
  {  BxtTimingTzqcl,      0,    0,    0,    0,   },
  { BxtTimingTzqoper,   800, 1067, 1334, 1600,   },
  {  BxtTimingTrfc4Gb,  104,  139,  174,  208,   },
  {  BxtTimingTrfc8Gb,  144,  192,  240,  288,   },
  {  BxtTimingTrfc12Gb, 224,  299,  374,  448,   },
  {  BxtTimingTrfc16Gb, 224,  299,  374,  448,   },
  {  BxtTimingTxsr4Gb,  110,  147,  184,  220,   },
  {  BxtTimingTxsr8Gb,  150,  200,  250,  300,   },
  {  BxtTimingTxsr12Gb, 230,  307,  348,  460,   },
  {  BxtTimingTxsr16Gb, 230,  307,  348,  460,   },
};

UINT8 TimingParamsUnder256Size = (sizeof(TimingParamsUnder256) / sizeof(TimingParamsUnder256[0]));
UINT8 TimingParamsOver255Size = (sizeof(TimingParamsOver255) / sizeof(TimingParamsOver255[0]));

UINT8 NumberEyeMaskRead = RT_NUM_EYEMASKS;

EYEMASK_T    EyeMaskRead[] = {
  {RT_EYEMASK_VREF, RT_EYEMASK0_PI},
  {RT_EYEMASK_VREF, RT_EYEMASK1_PI}
};

UINT8 NumberEyeMaskWrite = WT_NUM_EYEMASKS;

EYEMASK_T    EyeMaskWrite[] = {
  {WT_EYEMASK_VREF, WT_EYEMASK0_PI},
  {WT_EYEMASK_VREF, WT_EYEMASK1_PI}
};

UINT8 NumberEyeMaskCmd = CMD_NUM_EYEMASKS;

EYEMASK_T    EyeMaskCmd[] = {
  {3, 40},
  {3, 25}
};

#ifdef RT_PN_ENABLED
UINT8 NumberControlKnobsRead = 2 + RT_PN_ENABLED + RT_CTLE_ENABLE * 2;
#else
UINT8 NumberControlKnobsRead = 2 + RT_CTLE_ENABLE * 2;
#endif

UINT8 ControlVrefsRead[] = {RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP3_VREFSAMPLES};
UINT8 ControlVrefsWrite[] = {RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP3_VREFSAMPLES};
UINT8 ControlVrefsCmd[] = {RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP2_VREFMIN, RT_STEP3_VREFSAMPLES};

#if RT_PN_ENABLED==1
CONTROLKNOBS ControlKnobsReadLP4[] = {
  // Control                   Start       Range      Step     Rel/Abs     Byte Shared                        Bit Shared
  //  Knob                     Value       Value      Value    Flags         Flags                              Flag
  {RxDqsPDelay, RxDqsDelay, RxDqBitDelay, {0 , _12X},                                0xf, {0,  _12X}                           , { RT_STEP1_3_PISTEPLARGE, RT_STEP1_3_PISTEPMEDIUM, RT_STEP1_3_PISTEPSMALL}, MMRC_ABS, RT_BYTE_SHARING                      , RT_BIT_SHARING|TIMING_SHARED, "PI \0", 2+2*RT_CTLE_ENABLE-1, 0,  0, 0, -1, READTRAINING_ENTRY, FALSE, 2, 5, GenericMemoryTest},
  {RxDqsNDelay, RxDqsDelay, RxDqBitDelay, {0 , _12X},                                0xf, {0,  _12X}                           , { RT_STEP1_3_PISTEPLARGE, RT_STEP1_3_PISTEPMEDIUM, RT_STEP1_3_PISTEPSMALL}, MMRC_ABS, RT_BYTE_SHARING                      , RT_BIT_SHARING|TIMING_SHARED, "PI \0", 2+2*RT_CTLE_ENABLE-1, 0,  0, 0, -1, READTRAINING_ENTRY, FALSE, 2, 5, GenericMemoryTest},
  {RxVref    , -1,  -1          , {RT_STEP1_VREFSTART_LP4 ,  RT_STEP1_VREFSTART_LP4}, -1, {RT_STEP1_VREFMIN,  RT_STEP1_VREFMAX}, { RT_STEP2_VREFSTEP, 0, 0}                                                , MMRC_ABS, RANK_SHARED | KNOB_SHARED | BIT_SHARED | TIMING_SHARED , -1           , "VRF\0", 2+2*RT_CTLE_ENABLE-1, 5, 50, 5, RT_STEP1_VREFSTEP,READTRAINING_ENTRY, FALSE, 1, 3, GenericMemoryTest},
#if RT_CTLE_ENABLE==1
  {Ctle_Res  , -1          , {0                  ,  3}                 , -1, { 0              ,  3                }, { 1, 0, 0}, MMRC_ABS, RT_CTLE_SHARED, -1, "Cc", 2+2*RT_CTLE_ENABLE-1, 0, 0, 0, 1,READTRAINING_ENTRY, FALSE, GenericMemoryTest},
  {Ctle_Cap  , -1          , {0                  ,  3}                 , -1, { 0              ,  3                }, { 1, 0, 0}, MMRC_ABS, RT_CTLE_SHARED, -1, "Cr", 2+2*RT_CTLE_ENABLE-1, 0, 0, 0, 1,READTRAINING_ENTRY, FALSE, GenericMemoryTest},
#endif
};
#else
CONTROLKNOBS ControlKnobsReadLP4[] = {
  // Control                   Start       Range      Step     Rel/Abs     Byte Shared                        Bit Shared
  //  Knob                     Value       Value      Value    Flags         Flags                              Flag
  {RxDqsDelay, -1, RxDqBitDelay, {0 , _12X},                                0xf, {0,  _12X}                           , { RT_STEP1_3_PISTEPLARGE, RT_STEP1_3_PISTEPMEDIUM, RT_STEP1_3_PISTEPSMALL}, MMRC_ABS, RT_BYTE_SHARING                      , RT_BIT_SHARING|TIMING_SHARED, "PI \0", 2+2*RT_CTLE_ENABLE-1, 0,  0, 0, -1, READTRAINING_ENTRY, FALSE, 1, 5, GenericMemoryTest},
  {RxVref    , -1,  -1          , {RT_STEP1_VREFSTART_LP4 ,  RT_STEP1_VREFSTART_LP4}, -1, {RT_STEP1_VREFMIN,  RT_STEP1_VREFMAX}, { RT_STEP2_VREFSTEP, 0, 0}                                                , MMRC_ABS, RANK_SHARED | KNOB_SHARED | BIT_SHARED | TIMING_SHARED , -1           , "VRF\0", 2+2*RT_CTLE_ENABLE-1, 5, 50, 5, RT_STEP1_VREFSTEP,READTRAINING_ENTRY, FALSE, 1, 3, GenericMemoryTest},
#if RT_CTLE_ENABLE==1
  {Ctle_Res  , -1          , {0                  ,  3}                 , -1, { 0              ,  3                }, { 1, 0, 0}, MMRC_ABS, RT_CTLE_SHARED, -1, "Cc", 2+2*RT_CTLE_ENABLE-1, 0, 0, 0, 1,READTRAINING_ENTRY, FALSE, GenericMemoryTest},
  {Ctle_Cap  , -1          , {0                  ,  3}                 , -1, { 0              ,  3                }, { 1, 0, 0}, MMRC_ABS, RT_CTLE_SHARED, -1, "Cr", 2+2*RT_CTLE_ENABLE-1, 0, 0, 0, 1,READTRAINING_ENTRY, FALSE, GenericMemoryTest},
#endif
};
#endif

UINT8 NumberControlKnobsWrite = 2;

CONTROLKNOBS ControlKnobsWriteLP4[] = {
  // Control                   Start                                       Range              Step                                                                      Rel/Abs      Shared
  //  Knob                     Value             Value                     Value    Flags                                                            Flags
  {TxDqDelay,   -1, TxDqBitDelay, {_n12X,_12X}                             , 31, {_n12X,  _12X}                       , { WT_STEP1_3_PISTEPLARGE, WT_STEP1_3_PISTEPMEDIUM, WT_STEP1_3_PISTEPSMALL}, REL     , WT_BYTE_SHARING     , WT_BIT_SHARING, "PI \0", 2-1, 0,  0,                 0, -1,READTRAINING_ENTRY, TRUE, 1, 5, GenericMemoryTest},
  {TxVref,      -1,         -1, {WT_STEP1_VREFSTART_LP4 ,  WT_STEP1_VREFSTART_LP4}, -1, {WT_STEP1_VREFMIN_LP4,  WT_STEP1_VREFMAX_LP4}, { WT_STEP2_VREFSTEP_LP4, 0, 0}                                                , MMRC_ABS, STROBE_SHARED | KNOB_SHARED | BIT_SHARED , -1          , "VRF\0", 2-1, 5, 50, WT_STEP1_VREFSTEP_LP4, WT_STEP1_VREFSTEP_LP4,READTRAINING_ENTRY, TRUE, 1, 3, GenericMemoryTest},
};

UINT8 NumberControlKnobsCmdCtl = 3;

CONTROLKNOBS ControlKnobsCmdCtl[] = {
  // Control                   Start       Range      Step     Rel/Abs      Shared
  //  Knob                     Value       Value      Value    Flags         Flags
  {CmdAll ,               -1, -1, {0 , 0}, -1, {  _n12X,  _12X}, { 10, 0, 0}, REL, RANK_SHARED | BIT_SHARED | STROBE_SHARED | KNOB_SHARED, 0xff,"Cmd",2, 0, 0, 0,  0xFF, READTRAINING_ENTRY, TRUE,2, 5, GenericCmdMemoryTest},
  {CtlAll ,               -1, -1, {0 , 0}, -1, {  _n12X,  _12X}, { 10, 0, 0}, REL, RANK_SHARED | BIT_SHARED | STROBE_SHARED | KNOB_SHARED, 0xff,"Ctl",2, 0, 0, 0, 0xFF, READTRAINING_ENTRY, TRUE,2, 5, GenericCmdMemoryTest},
  {TxCaVref,              -1, -1, {400 , 400}, -1, {100,  400}, { 401, 0, 0}, MMRC_ABS, STROBE_SHARED | KNOB_SHARED | BIT_SHARED,-1,"Vre",2, 5, 50, 5, 5, READTRAINING_ENTRY, TRUE,2, 3, GenericCmdMemoryTest},
};

UINT8 EyeMaskPctPiRd[NUM_TYPE][NUM_FREQ][RT_NUM_EYEMASKS] =
{
  { //LPDDR3
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //LPDDR4
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //DDR3L
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //WIO2
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
};

UINT8 EyeMaskPctPiWr[NUM_TYPE][NUM_FREQ][WT_NUM_EYEMASKS] =
{
  { //LPDDR3
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //LPDDR4
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //DDR3L
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //WIO2
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
};

UINT8 EyeMaskPctPiCmdCtl[NUM_TYPE][NUM_FREQ][CMD_NUM_EYEMASKS] =
{
  { //LPDDR3
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //LPDDR4 (*2 since its this value is a percentage of HalfClk)
    {100,60}, //800
    {100,60}, //1066
    {100,60}, //1333
    {100,60}, //1600
    {100,60}, //1866
    {100,60}, //2133
    {100,60}, //2400
    {100,60}, //2667
    {100,60}, //3200
    {100,60}  //2933
  },
  { //DDR3L
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
  { //WIO2
    {40,20}, //800
    {40,20}, //1066
    {40,20}, //1333
    {40,20}, //1600
    {30,15}, //1866
    {20,10}, //2133
    {20,10}, //2400
    {20,10}, //2667
    {20,10}, //3200
    {20,10}  //2933
  },
};

UINT32 DramOdt[NUM_TYPE][NUM_FREQ] =
{
  { //LPDDR3
    99999, //800
    99999, //1066
    99999, //1333
    99999, //1600
    60,    //1866
    60,    //2133
    60,    //2400
    60,    //2667
    60,    //3200
    60     //2933
  },
  { //LPDDR4
    99999, //800
    99999, //1066
    99999, //1333
    99999, //1600
    60,    //1866
    60,    //2133
    60,    //2400
    60,    //2667
    60,    //3200
    60     //2933
  },
  { //DDR3L
    99999, //800
    99999, //1066
    99999, //1333
    99999, //1600
    60,    //1866
    60,    //2133
    60,    //2400
    60,    //2667
    60,    //3200
    60     //2933
  },
  { //WIO2
    99999, //800
    99999, //1066
    99999, //1333
    99999, //1600
    60,    //1866
    60,    //2133
    60,    //2400
    60,    //2667
    60,    //3200
    60     //2933
  },
};

UINT16 DramVrefOvr[NUM_TYPE][NUM_FREQ] =
{
  { //LPDDR3
    37, //800
    37, //1066
    37, //1333
    37, //1600
    27, //1866
    27, //2133
    27, //2400
    27, //2667
    27, //3200
    27  //2933
  },
  { //LPDDR4
    37, //800
    37, //1066
    37, //1333
    37, //1600
    27, //1866
    27, //2133
    27, //2400
    27, //2667
    27, //3200
    27  //2933
  },
  { //DDR3L
    37, //800
    37, //1066
    37, //1333
    37, //1600
    27, //1866
    27, //2133
    27, //2400
    27, //2667
    27, //3200
    27  //2933
  },
  { //WIO2
    37, //800
    37, //1066
    37, //1333
    37, //1600
    27, //1866
    27, //2133
    27, //2400
    27, //2667
    27, //3200
    27  //2933
  },
};

//UINT8 SimData[MAX_CHANNELS][MAX_RANKS][MAX_KNOBS][MAX_STROBES][MAX_BITS][2];
typedef struct {
  UINT8 Left;
  UINT8 Right;
} RANGE_T;


CONST UINT8 AddressDecoding[3][2][3] = {
  //LP3
    //1KB,         2KB,           4KB
  {{AddMapping4KB, AddMapping4KB, AddMapping4KB},  //1 channel
   {AddMapping2KB, AddMapping2KB, AddMapping4KB},  //2 channels
  },
  //LP4
  {{AddMapping4KB, AddMapping4KB, AddMapping4KB},  //2 channel
   {AddMapping2KB, AddMapping2KB, AddMapping4KB},  //4 channels
  },
  //WIO
    {{AddMapping4KB, AddMapping4KB, AddMapping4KB},  //1 channel
   {AddMapping2KB, AddMapping2KB, AddMapping4KB},  //2 channels
  },
};

#if PASS_GATE_TEST == 1
CONST UINT32 MaxDelayFor4x[NUM_FREQ] = {
  319000,  // 800  - Never Tested
  426000,  // 1066
  532000,  // 1333
  639000,  // 1600
};

CONST DELAY_PASS_GATE DelayPassGate [2][NUM_FREQ] = {
  { // 1x Speed
  { 75,  0, 0, 0}, // 800  - 100ns - Never Tested
  {100, 53, 0, 0}, // 1066 - 75ns
  { 50, 58, 0, 0}, // 1333 - 60ns
  { 50, 64, 0, 0}, // 1600 - 50ns
  },
  { // 4x Speed
  {150,   0, 0, 0}, // 800  - Never Tested
  {200,  26, 0, 0}, // 1066 - 150ns
  {100,  29, 0, 0}, // 1333 - 120ns
  {100,  32, 0, 0}, // 1600 - 100ns
  },
};
#endif

#if EARLY_COMMAND_TRAINING == 1 && TRAINING_ALGOS == 1
  //
  // These are the variables required for early command training.
  //
  UINT16 EarlyCommandPatternsRise[]   = {0x2AA,      0x0cc,      0x30f,  0x38b};
  UINT16 EarlyCommandPatternsFall[]   = {0x155,      0x333,      0x0f0,  0x074};
  UINT32 EarlyCommandResults41[MAX_CHANNELS][4]      = {{0x9966,     0x965a,     0x6A55, 0x5A65},
                                                        {0x9966,     0x965a,     0x6A55, 0x5A65},
                                                        {0x9966,     0x965a,     0x6A55, 0x5A65},
                                                        {0x9966,     0x965a,     0x6A55, 0x5A65}
                                                       };
  UINT32 EarlyCommandResults48[MAX_CHANNELS][4]      = {{0x0102,     0x0202,     0x0102, 0x0102},
                                                        {0x0102,     0x0202,     0x0102, 0x0102},
                                                        {0x0102,     0x0202,     0x0102, 0x0102},
                                                        {0x0102,     0x0202,     0x0102, 0x0102}
                                                       };

  UINT16 EarlyCommandPatterns1RiseLP4[]   = {0x31,      0x0D,      0x06, 0x30};
  UINT16 EarlyCommandPatterns1FallLP4[]   = {0x0C,      0x30,      0x29, 0x0F};
  UINT16 EarlyCommandPatterns2RiseLP4[]   = {0x33,      0x0F,      0x16, 0x00};
  UINT16 EarlyCommandPatterns2FallLP4[]   = {0x0C,      0x30,      0x29, 0x00};
  UINT32 EarlyCommandResultsSIM[]      = {0x31003100, 0x0d000d00, 0x06000600, 0x30003000};
  UINT32 EarlyCommandResultsSIL[MAX_CHANNELS][4]      =
     {
        {0x005c005c, 0x000f000f, 0x16001600, 0x00000000}, //{0x33003300, 0x03003000, 0x16001600, 0x00000000};
        {0xca00ca00, 0xf000f000, 0x16001600, 0x00000000}, //{0x33003300, 0x03003000, 0x16001600, 0x00000000};
        {0x00960096, 0x000f000f, 0x16001600, 0x00000000}, //{0x33003300, 0x03003000, 0x16001600, 0x00000000};
        {0xca00ca00, 0xf000f000, 0x16001600, 0x00000000} //{0x33003300, 0x03003000, 0x16001600, 0x00000000};
  };

  UINT8 EarlyCommandWeights[3][3] = {
    { 1, 1, 1 },
    { 1, 2, 1 },
    { 1, 1, 1 }
  };

#endif
/**
  TimingParamHash - BXTE
  This hash function converts old index values to new valid ones.
  We are removing indices for the following arrays:
     ConfigToSpeed
     ConfigToSpeedMultiple133
     ConfigToDdrType
     TimingParamsUnder256
     TimingParamsOver256
  The TimingParams arrays are 1 based, whereas the Config is 0 based.
  For Config, we are removing indices
     0, 1, 2, 6, 7, 11, 12, 13, 14, 15, 16, and 29+
  For Timing, we are removing indices
     1, 2, 3, 7, 8, 12, 13, 14, 15, 16, 17, and 30+
  These indices are being removed because DDR4 and select frequencies are not POR.
  This also saves on code size.

  @param[in, out] TimingParamInput  The requested index value.
  @param[in]      Config            TRUE if the array is a "Config" array. FALSE if it is a "Timing" array.

  @retval         TimingParamInput  The new index value.

**/
extern
UINT8
TimingParamHash (
  UINT8   TimingParamInput,
  BOOLEAN Config
  )
{
  UINT8 Offset;
  Offset = 0;

  if (Config) {
    TimingParamInput++;
  }
  //
  // Take valid index and determine offset.
  // Obtain offset by comparing MaxMemoryTypeFreqConfigSmip (original index)
  // to MaxMemoryTypeFreqConfig (correct index).
  //
  if ((LPDDR4_1600_14_15_15 <= TimingParamInput) && (TimingParamInput < LPDDR4_2400_24_22_22)) {
    Offset += (LPDDR4_1600_14_15_15 - LPDDR4_1600);
  }
  else if ((LPDDR4_2666_24_24_24 <= TimingParamInput) && (TimingParamInput < DDR3_1066_6_6_6)) {
    Offset += (LPDDR4_2666_24_24_24 - LPDDR4_2666);
  }
  else {
    // If the function was asked for a index with data that has been removed, return an 0xFF to indicate error.
    MmrcErrorPrint((MMRC_DBG_MIN, "The index does not exist. Please consult TimingParams.\n"));
    return 0xFF;
  }
  //
  // Return the new index by subtracting offset from original value.
  //
  if (Config) {
    TimingParamInput--;
  }
  return (TimingParamInput - Offset);
}
