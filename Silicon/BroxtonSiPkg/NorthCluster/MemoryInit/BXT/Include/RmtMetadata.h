/** @file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation.

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
#ifndef __AUTOGENERATED_RMTMETADATA_H__
#define __AUTOGENERATED_RMTMETADATA_H__

#include "SsaResults.h"

#define RMT_RESULT_METADATA_GUID \
{0x02CB1552,0xD659,0x4232,{0xB5,0x1F,0xCA,0xB1,0xE1,0x1F,0xCA,0x87} }

#pragma pack (push, 1)

enum SCRAMBLER_OVERRIDE_MODE{
  DisableScrambler = 0,
  EnableScrambler = 1,
  DontTouchScrambler = 2,
  SCRAMBLER_OVERRIDE_MODE_DELIM = INT32_MAX
};

typedef enum SCRAMBLER_OVERRIDE_MODE SCRAMBLER_OVERRIDE_MODE;

typedef struct RMT_RESULT_METADATA{
  BOOLEAN EnableCtlAllMargin;
  UINT16 SinglesBurstLength;
  UINT32 SinglesLoopCount;
  UINT16 TurnaroundsBurstLength;
  UINT32 TurnaroundsLoopCount;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  UINT8 PiStepUnit[2];
  UINT16 RxVrefStepUnit[2];
  UINT16 TxVrefStepUnit[2][2];
  UINT16 CmdVrefStepUnit[2][2];
  UINT8 MajorVer;
  UINT8 MinorVer;
  UINT8 RevVer;
  UINT32 BuildVer;
  UINT16 ResultEleCount;
}RMT_RESULT_METADATA;

#pragma pack (pop)

#endif
