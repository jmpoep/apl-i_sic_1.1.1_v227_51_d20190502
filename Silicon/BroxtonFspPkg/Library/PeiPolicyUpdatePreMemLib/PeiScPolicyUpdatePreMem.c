/** @file
  This file is SampleCode of the library for Intel SC PEI Policy initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Ppi/ScPolicyPreMem.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>
#include <Library/GpioLib.h>

/**
  This function performs SC PEI Policy update in pre-memory.

  @param[in] ScPreMemPolicy   The SC PreMem Policy PPI instance
  @param[in] FspmUpd          The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiScPolicyPreMem (
  IN OUT SC_PREMEM_POLICY_PPI *ScPreMemPolicy,
  IN FSPM_UPD                 *FspmUpd
  )
{
  SC_PCIE_PREMEM_CONFIG  *PciePreMemConfig;
  EFI_STATUS             Status;

  Status = GetConfigBlock ((VOID *) ScPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PciePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  PciePreMemConfig->RootPort[0].Perst = N_GPIO_13;  // Slot2
  PciePreMemConfig->RootPort[1].Perst = N_GPIO_15;  // NGFF
  PciePreMemConfig->RootPort[2].Perst = W_GPIO_152; // Slot1
  PciePreMemConfig->RootPort[3].Perst = 0;
  PciePreMemConfig->RootPort[4].Perst = N_GPIO_37;  // LOM
  PciePreMemConfig->RootPort[5].Perst = 0;

  PciePreMemConfig->RootPort[0].Clock = W_GPIO_211;  // Slot2
  PciePreMemConfig->RootPort[1].Clock = W_GPIO_212;  // NGFF
  PciePreMemConfig->RootPort[2].Clock = W_GPIO_209; // Slot1
  PciePreMemConfig->RootPort[3].Clock = 0;
  PciePreMemConfig->RootPort[4].Clock = 0;
  PciePreMemConfig->RootPort[5].Clock = 0;
  PciePreMemConfig->StartTimerTickerOfPfetAssert = (UINTN) FspmUpd->FspmConfig.StartTimerTickerOfPfetAssert;
  PciePreMemConfig->SkipPciePowerSequence = FspmUpd->FspmConfig.SkipPciePowerSequence;

  return EFI_SUCCESS;
}
