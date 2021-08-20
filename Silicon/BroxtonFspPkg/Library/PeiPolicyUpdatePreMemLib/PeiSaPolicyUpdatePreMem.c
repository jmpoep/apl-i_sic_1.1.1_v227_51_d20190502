/** @file
  Do Platform Stage System Agent initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation.

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

#include <FspEas.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <FspmUpd.h>
#include <PlatformBaseAddresses.h>

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization in pre-memory

  @param[in out] SiSaPolicyPpi     SI_SA_POLICY PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicyPreMem (
  IN OUT SI_SA_POLICY_PPI       *SiSaPolicyPpi,
  IN FSPM_UPD                   *FspmUpd
  )
{
  EFI_STATUS                      Status;
  SA_MISC_CONFIG                  *MiscConfig = NULL;

  DEBUG ((DEBUG_INFO, "FspUpdatePeiSaPolicyPreMem() - start\n"));

  Status = GetConfigBlock ((VOID *)SiSaPolicyPpi, &gSaMiscConfigGuid , (VOID *)&MiscConfig);
  ASSERT_EFI_ERROR (Status);

  MiscConfig->FastBoot = FspmUpd->FspmConfig.MrcFastBoot;
  MiscConfig->DynSR = 1;

  DEBUG ((DEBUG_INFO, "FspUpdatePeiSaPolicyPreMem() - end\n"));
  return EFI_SUCCESS;
}

/*++

Routine Description:

FspUpdatePeiSaPolicy performs SA Pre Mem PEI Policy initialization

  @param[in out] SiSaPolicyPpi     SI_SA_POLICY PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.

--*/

EFI_STATUS
EFIAPI
FspUpdateSaPolicyPreMem (
  IN OUT SI_SA_POLICY_PPI       *SiSaPolicyPpi,
  IN FSPM_UPD                   *FspmUpd
  )
{
  SA_PRE_MEM_CONFIG             *SaPreMemConfig = NULL;
  NPK_PRE_MEM_CONFIG            *NpkPreMemConfig = NULL;
  EFI_STATUS                    Status;

  DEBUG ((EFI_D_INFO, "FspUpdateSaPolicyPreMem() - Start\n"));

  Status = GetConfigBlock ((VOID *) SiSaPolicyPpi, &gSaPreMemConfigGuid, (VOID *) &SaPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiSaPolicyPpi, &gNpkPreMemConfigGuid, (VOID *) &NpkPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  SaPreMemConfig->PrimaryDisplay      = FspmUpd->FspmConfig.PrimaryVideoAdaptor;
  SaPreMemConfig->IgdDvmt50PreAlloc   = FspmUpd->FspmConfig.IgdDvmt50PreAlloc;
  SaPreMemConfig->ApertureSize        = FspmUpd->FspmConfig.IgdApertureSize;
  SaPreMemConfig->GttSize             = FspmUpd->FspmConfig.GttSize;
  SaPreMemConfig->InternalGraphics    = FspmUpd->FspmConfig.Igd;
  SaPreMemConfig->RtEn                = FspmUpd->FspmConfig.RtEn;

  NpkPreMemConfig->NpkEn              = FspmUpd->FspmConfig.NpkEn;
  NpkPreMemConfig->FwTraceEn          = FspmUpd->FspmConfig.FwTraceEn;
  NpkPreMemConfig->FwTraceDestination = FspmUpd->FspmConfig.FwTraceDestination;
  NpkPreMemConfig->RecoverDump        = FspmUpd->FspmConfig.RecoverDump;
  NpkPreMemConfig->Msc0Size           = FspmUpd->FspmConfig.Msc0Size;
  NpkPreMemConfig->Msc0Wrap           = FspmUpd->FspmConfig.Msc0Wrap;
  NpkPreMemConfig->Msc1Size           = FspmUpd->FspmConfig.Msc1Size;
  NpkPreMemConfig->Msc1Wrap           = FspmUpd->FspmConfig.Msc1Wrap;
  NpkPreMemConfig->PtiMode            = FspmUpd->FspmConfig.PtiMode;
  NpkPreMemConfig->PtiTraining        = FspmUpd->FspmConfig.PtiTraining;
  NpkPreMemConfig->PtiSpeed           = FspmUpd->FspmConfig.PtiSpeed;
  NpkPreMemConfig->PunitMlvl          = FspmUpd->FspmConfig.PunitMlvl;
  NpkPreMemConfig->PmcMlvl            = FspmUpd->FspmConfig.PmcMlvl;
  NpkPreMemConfig->SwTraceEn          = FspmUpd->FspmConfig.SwTraceEn;

  return EFI_SUCCESS;
}