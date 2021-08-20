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

@par Specification Reference:
**/

#include <PiPei.h>
#include <Ppi/DramPolicyPpi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#include "MmrcData.h"


/**
  DramInitializePolicy creates the default setting of Dram Policy.

  @param[out] DramPolicyPpi       The pointer to get Dram Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
DramInitializePolicy (
  OUT  DRAM_POLICY_PPI **DramPolicyPpi
  )
{
  DRAM_POLICY_PPI    *DramPolicy;
  FSPM_UPD           *FspmUpd;
  DRP_DRAM_POLICY    *DrpPtr;
  UINT8              *ChSwizlePtr;

  FspmUpd = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer();

  DramPolicy = (DRAM_POLICY_PPI *) AllocateZeroPool (sizeof (DRAM_POLICY_PPI));
  if (DramPolicy == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }


  DramPolicy->Package                            = FspmUpd->FspmConfig.Package;
  DramPolicy->Profile                            = FspmUpd->FspmConfig.Profile;
  DramPolicy->MemoryDown                         = FspmUpd->FspmConfig.MemoryDown;
  DramPolicy->DDR3LPageSize                      = FspmUpd->FspmConfig.DDR3LPageSize;
  DramPolicy->DDR3LASR                           = FspmUpd->FspmConfig.DDR3LASR;
  DramPolicy->SystemMemorySizeLimit              = FspmUpd->FspmConfig.MemorySizeLimit;
  DramPolicy->SpdAddress[0]                      = FspmUpd->FspmConfig.DIMM0SPDAddress;
  DramPolicy->SpdAddress[1]                      = FspmUpd->FspmConfig.DIMM1SPDAddress;
  DramPolicy->DDR3LPageSize                      = FspmUpd->FspmConfig.DDR3LPageSize;
  DramPolicy->DDR3LASR                           = FspmUpd->FspmConfig.DDR3LASR;
  DramPolicy->HighMemMaxVal                      = FspmUpd->FspmConfig.HighMemoryMaxValue;
  DramPolicy->LowMemMaxVal                       = FspmUpd->FspmConfig.LowMemoryMaxValue;
  DramPolicy->DisableFastBoot                    = FspmUpd->FspmConfig.DisableFastBoot;
  DramPolicy->RmtMode                            = FspmUpd->FspmConfig.RmtMode;
  DramPolicy->RmtCheckRun                        = FspmUpd->FspmConfig.RmtCheckRun;
  DramPolicy->RmtMarginCheckScaleHighThreshold   = FspmUpd->FspmConfig.RmtMarginCheckScaleHighThreshold;

  DramPolicy->MsgLevelMask                       = 0;

  DrpPtr                                         = (DRP_DRAM_POLICY*)(&(FspmUpd->FspmConfig.Ch0_RankEnable));
  ChSwizlePtr                                    = (UINT8 *)(FspmUpd->FspmConfig.Ch0_Bit_swizzling);


  DramPolicy->ChannelHashMask         = FspmUpd->FspmConfig.ChannelHashMask;
  DramPolicy->SliceHashMask           = FspmUpd->FspmConfig.SliceHashMask;
  DramPolicy->ChannelsSlicesEnabled   = FspmUpd->FspmConfig.ChannelsSlicesEnable;
  DramPolicy->InterleavedMode         = FspmUpd->FspmConfig.InterleavedMode;
  DramPolicy->MinRefRate2xEnabled     = FspmUpd->FspmConfig.MinRefRate2xEnable;
  DramPolicy->DualRankSupportEnabled  = FspmUpd->FspmConfig.DualRankSupportEnable;

  ((SMIP_SCRAMBLERSUPPORT_OPTION *) &DramPolicy->ScramblerSupport)->Bits.ScramblerSupport          = (UINT32) FspmUpd->FspmConfig.ScramblerSupport;
  ((SMIP_SCRAMBLERSUPPORT_OPTION *) &DramPolicy->ScramblerSupport)->Bits.PeriodicRetrainingDisable = (UINT32) FspmUpd->FspmConfig.PeriodicRetrainingDisable;

  if (DrpPtr != NULL) {
    CopyMem (DramPolicy->ChDrp, DrpPtr, sizeof(DramPolicy->ChDrp));
  }
  if (ChSwizlePtr != NULL) {
    CopyMem (DramPolicy->ChSwizzle, ChSwizlePtr, sizeof (DramPolicy->ChSwizzle));
  }

  DramPolicy->MrcTrainingDataPtr = (EFI_PHYSICAL_ADDRESS) (UINTN)FspmUpd->FspmArchUpd.NvsBufferPtr;
  DramPolicy->MrcBootDataPtr = (EFI_PHYSICAL_ADDRESS) (UINTN) FspmUpd->FspmConfig.VariableNvsBufferPtr;
  *DramPolicyPpi                 = DramPolicy;

  return EFI_SUCCESS;
}


/**
  DramInstallPolicyPpi installs Dram Policy Ppi.

  @param[in] DramPolicyPpi       The pointer to Dram Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
DramInstallPolicyPpi (
  IN  DRAM_POLICY_PPI *DramPolicyPpi
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *DramPolicyPpiDesc;

  DramPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (DramPolicyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  DramPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  DramPolicyPpiDesc->Guid  = &gDramPolicyPpiGuid;
  DramPolicyPpiDesc->Ppi   = DramPolicyPpi;

  //
  // Install Silicon Policy PPI
  //
  Status = PeiServicesInstallPpi (DramPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

