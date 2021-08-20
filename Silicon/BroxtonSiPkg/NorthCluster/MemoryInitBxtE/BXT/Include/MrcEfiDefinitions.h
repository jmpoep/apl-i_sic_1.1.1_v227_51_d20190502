/**@file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2016 Intel Corporation.

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
#ifndef _MRCEFIDEFINITIONS_H_
#define _MRCEFIDEFINITIONS_H_

#ifdef EFI_MEMORY_INIT
#include <Ppi/DramPolicyPpi.h>
#include <Library/SteppingLib.h>
#define DRAM_POLICY DRAM_POLICY_PPI
#else
#include "DataTypes.h"

#define DRAM_POLICY_NUMBER_CHANNELS         4
#define DRAM_POLICY_NUMBER_BITS             32
#define DRAM_POLICY_NUMBER_SPD_ADDRESSES    2

#pragma pack(push,1)

typedef struct {
  UINT8 RankEnable;
  UINT8 DeviceWidth;
  UINT8 DramDensity;
  UINT8 Option;
  UINT8 OdtConfig;
  UINT8 TristateClk1;
  UINT8 Mode2N;
} DRP_DRAM_POLICY;

typedef struct {
    UINT8               Package;
    UINT8               Profile;
    UINT8               MemoryDown;
    UINT8               DDR3LPageSize;
    UINT8               DDR3LASR;
    EFI_PHYSICAL_ADDRESS MrcTrainingDataPtr;
    UINT8               ScramblerSupport;
    UINT16              ChannelHashMask;
    UINT16              SliceHashMask;
    UINT8               InterleavedMode;
    UINT8               ChannelsSlicesEnabled;//bit0=SliceEnabled;bit1=channelEnabled
    UINT8               MinRefRate2xEnabled;
    UINT8               DualRankSupportEnabled;
    UINT16              SystemMemorySizeLimit;
    UINT16              LowMemMaxVal;
    UINT16              HighMemMaxVal;
    UINT8               DisableFastBoot;
    UINT8               RmtMode;
    UINT8               SpdAddress[DRAM_POLICY_NUMBER_SPD_ADDRESSES];
    UINT8               ChSwizzle[DRAM_POLICY_NUMBER_CHANNELS][DRAM_POLICY_NUMBER_BITS];
    DRP_DRAM_POLICY     ChDrp[DRAM_POLICY_NUMBER_CHANNELS];
    UINT8               reserved[14];
} DRAM_POLICY;

///
/// BXT Series
///
typedef enum {
  Bxt = 0x00,
  Bxt1,
  BxtP,
  BxtSeriesMax = 0xFF
} BXT_SERIES;

///
/// Silicon Steppings
///
typedef enum {
  BxtB0 = 0x10,
  BxtB1,
  BxtB2,
  BxtC0,
  BxtPB0 = 0x50,
  BxtSteppingMax = 0xFF
} BXT_STEPPING;


#pragma pack(pop)


#endif
#endif
