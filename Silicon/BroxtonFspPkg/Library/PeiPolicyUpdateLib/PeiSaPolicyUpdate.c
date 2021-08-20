/** @file
  Do Platform Stage System Agent initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2016 Intel Corporation.

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

#include <SaAccess.h>
#include <Ppi/SaPolicy.h>
#include <Ppi/ScPolicy.h>
#include <Library/DebugLib.h>
#include <FspEas.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <FspsUpd.h>
#include <Library/SteppingLib.h>

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization

  @param[in out] SiSaPolicyPpi     SI_SA_POLICY PPI
  @param[in out] ScPolicyPpi       The SC Policy PPI instance
  @param[in]     FspsUpd           The pointer of FspsUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicy (
  IN OUT SI_SA_POLICY_PPI       *SiSaPolicyPpi,
  IN OUT SC_POLICY_PPI          *ScPolicyPpi,
  IN FSPS_UPD                   *FspsUpd
  )
{
  EFI_STATUS                    Status;
  SA_MISC_CONFIG                *MiscConfig  = NULL;
  GRAPHICS_CONFIG               *GtConfig    = NULL;
  IPU_CONFIG                    *IpuPolicy = NULL;
  SC_VTD_CONFIG                 *VtdConfig = NULL;
  BXT_SERIES                    SocSeries;
  BXT_STEPPING                  SocStepping;

  DEBUG ((DEBUG_INFO, "FspUpdatePeiSaPolicy() - start\n"));

  Status = GetConfigBlock ((VOID *)SiSaPolicyPpi, &gSaMiscConfigGuid, (VOID *)&MiscConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiSaPolicyPpi, &gGraphicsConfigGuid, (VOID *)&GtConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiSaPolicyPpi, &gIpuConfigGuid, (VOID *)&IpuPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) ScPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Get the Platform Configuration from FspUpd
  //
  GtConfig->GttMmAdr                  = GTTMM_BASE_ADDRESS;
  GtConfig->GmAdr                     = GMADR_BASE_ADDRESS;

  GtConfig->PmSupport                 = FspsUpd->FspsConfig.PmSupport;
  GtConfig->EnableRenderStandby       = FspsUpd->FspsConfig.EnableRenderStandby;
  GtConfig->CdClock                   = FspsUpd->FspsConfig.CdClock;
  GtConfig->PavpEnable                = FspsUpd->FspsConfig.PavpEnable;

  GtConfig->GraphicsConfigPtr         = (VOID *)FspsUpd->FspsConfig.GraphicsConfigPtr;
  GtConfig->LogoPtr                   = (VOID *)FspsUpd->FspsConfig.LogoPtr;
  GtConfig->LogoSize                  = FspsUpd->FspsConfig.LogoSize;
  GtConfig->PeiGraphicsPeimInit       = FspsUpd->FspsConfig.PeiGraphicsPeimInit;
  GtConfig->PavpPr3                   = FspsUpd->FspsConfig.PavpPr3;

  //

  MiscConfig->FastBoot                = 1;
  MiscConfig->DynSR                   = 0;

  IpuPolicy->SaIpuEnable              = FspsUpd->FspsConfig.IpuEn;

  //
  // Workaround for [1504141196][[APL RCR] IPU and VT-D dependency implementation] -fix in C0 stepping
  //
  SocSeries   = GetBxtSeries ();
  SocStepping = BxtStepping ();
  if ((SocSeries == BxtP && SocStepping <= BxtPB1) || (SocSeries != BxtP && SocStepping < BxtC0)) {
    if (IpuPolicy->SaIpuEnable == 1) {
      VtdConfig->VtdEnable = 0;
    }
  }

  IpuPolicy->IpuAcpiMode              = FspsUpd->FspsConfig.IpuAcpiMode;
  IpuPolicy->IpuMmAdr                 = IPUMM_BASE_ADDRESS;

  DEBUG ((DEBUG_INFO, "FspUpdatePeiSaPolicy() - end\n"));
  return EFI_SUCCESS;
}
