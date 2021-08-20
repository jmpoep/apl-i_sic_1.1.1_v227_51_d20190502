/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2016 Intel Corporation.

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
#include <Ppi/CpuPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>
#include <CpuRegs.h>
#include <Library/CpuPolicyLib.h>
#include <Library/SteppingLib.h>

/**
  Get the next microcode patch pointer.

  @param[in, out] MicrocodeData - Input is a pointer to the last microcode patch address found,
                                  and output points to the next patch address found.

  @retval EFI_SUCCESS           - Patch found.
  @retval EFI_NOT_FOUND         - Patch not found.
**/
EFI_STATUS
EFIAPI
RetrieveNextMicrocode (
  IN OUT CPU_MICROCODE_HEADER **MicrocodeData
  )
{
/*todo: build error that need to fix
  UINTN                MicrocodeStart;
  UINTN                MicrocodeEnd;
  UINTN                TotalSize;

  ///
  /// Microcode binary in SEC
  ///
  MicrocodeStart = PcdGet32 (PcdFlashMicroCodeAddress) +
          ((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashMicroCodeAddress))->HeaderLength +
          sizeof (EFI_FFS_FILE_HEADER);

  MicrocodeEnd = PcdGet32 (PcdFlashMicroCodeAddress) + PcdGet32 (PcdFlashMicroCodeSize);

  if (*MicrocodeData == NULL) {
    *MicrocodeData = (CPU_MICROCODE_HEADER *) (UINTN) MicrocodeStart;
  } else {
    if (*MicrocodeData < (CPU_MICROCODE_HEADER *) (UINTN) MicrocodeStart) {
      DEBUG ((DEBUG_INFO, "[CpuPolicy]*MicrocodeData < MicrocodeStart \n"));
      return EFI_NOT_FOUND;
    }

    TotalSize = (UINTN) ((*MicrocodeData)->TotalSize);
    if (TotalSize == 0) {
      TotalSize = 2048;
    }
    //
    // Add alignment check - begin
    //
    if ((TotalSize & 0x7FF) != 0) {
      TotalSize = (TotalSize & 0xFFFFF800) + 0x800;
    }
    //
    // Add alignment check - end
    //
    *MicrocodeData += TotalSize;
    if (*MicrocodeData >= (CPU_MICROCODE_HEADER *) (UINTN) (MicrocodeEnd) || (*MicrocodeData)->TotalSize == (UINT32) -1) {
      DEBUG ((DEBUG_INFO, "[CpuPolicy]*MicrocodeData >= MicrocodeEnd \n"));
      return EFI_NOT_FOUND;
    }
  }
 */
  return EFI_SUCCESS;
}

/**
  Get the microcode patch pointer.

  @retval EFI_PHYSICAL_ADDRESS - Address of the microcode patch, or NULL if not found.
**/
EFI_PHYSICAL_ADDRESS
PlatformCpuLocateMicrocodePatch (
  VOID
  )
{
  EFI_STATUS              Status;
  CPU_MICROCODE_HEADER    *MicrocodeData;
  EFI_CPUID_REGISTER      Cpuid;
  UINT32                  UcodeRevision;

  AsmCpuid (
    CPUID_VERSION_INFO,
    &Cpuid.RegEax,
    &Cpuid.RegEbx,
    &Cpuid.RegEcx,
    &Cpuid.RegEdx
    );

  UcodeRevision = GetCpuUcodeRevision ();
  MicrocodeData = NULL;
  while (TRUE) {
    ///
    /// Find the next patch address
    ///
    Status = RetrieveNextMicrocode (&MicrocodeData);
    DEBUG ((DEBUG_INFO, "MicrocodeData =  %x\n", MicrocodeData));

    if (Status != EFI_SUCCESS) {
      break;
    } else if (CheckMicrocode (Cpuid.RegEax, MicrocodeData, &UcodeRevision)) {
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    return (EFI_PHYSICAL_ADDRESS) 0;
  } else {
    return (EFI_PHYSICAL_ADDRESS) (UINTN) MicrocodeData;
  }
}

/**
  This function performs CPU PEI Policy initialization.

  @param[in] SiCpuPolicyPpi        The Cpu Policy PPI instance
  @param[in] FspsUpd               The pointer of FspsUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicy (
  IN OUT SI_CPU_POLICY_PPI    *SiCpuPolicyPpi,
  IN FSPS_UPD                 *FspsUpd
  )
{
  EFI_STATUS                  Status;
  CPU_CONFIG                  *CpuConfig;
  POWER_MGMT_CONFIG           *PowerMgmtConfig;

  DEBUG ((DEBUG_INFO, "Update CPU policy from FspsUpd\n"));

  Status = GetConfigBlock((CONFIG_BLOCK_TABLE_HEADER*)SiCpuPolicyPpi,&gCpuConfigGuid,(VOID *)&CpuConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, " Get config block for CpuConfig \n"));

  Status = GetConfigBlock((CONFIG_BLOCK_TABLE_HEADER*)SiCpuPolicyPpi,&gPowerMgmtConfigGuid,(VOID *)&PowerMgmtConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, " Get config block for PowerMgmtConfig \n"));

  CpuConfig->ActiveProcessorCores     = FspsUpd->FspsConfig.ActiveProcessorCores;
  CpuConfig->DisableCore1             = FspsUpd->FspsConfig.DisableCore1;
  CpuConfig->DisableCore2             = FspsUpd->FspsConfig.DisableCore2;
  CpuConfig->DisableCore3             = FspsUpd->FspsConfig.DisableCore3;
  CpuConfig->VmxEnable                = FspsUpd->FspsConfig.VmxEnable;
  CpuConfig->ProcTraceMemSize         = FspsUpd->FspsConfig.ProcTraceMemSize;
  CpuConfig->ProcTraceEnable          = FspsUpd->FspsConfig.ProcTraceEnable;
  CpuConfig->SkipMpInit               = FspsUpd->FspsConfig.SkipMpInit;
  CpuConfig->MonitorMwaitEnable       = FspsUpd->FspsConfig.MonitorMwaitEnable;
  PowerMgmtConfig->Eist               = FspsUpd->FspsConfig.Eist;
  PowerMgmtConfig->BootPState         = FspsUpd->FspsConfig.BootPState;
  PowerMgmtConfig->Cx                 = FspsUpd->FspsConfig.EnableCx;
  PowerMgmtConfig->C1e                = FspsUpd->FspsConfig.C1e;
  PowerMgmtConfig->BiProcHot          = FspsUpd->FspsConfig.BiProcHot;
  PowerMgmtConfig->PkgCStateLimit     = (MAX_PKG_C_STATE)FspsUpd->FspsConfig.PkgCStateLimit;
  PowerMgmtConfig->TurboMode          = FspsUpd->FspsConfig.TurboMode;

  if (CpuConfig->MonitorMwaitEnable == 0) {
    PowerMgmtConfig->PmgCstCfgCtrIoMwaitRedirection  = 1;
  }


  if (FspsUpd->FspsConfig.EnableCx) {
    //
    // Clear C1 & C3 Auto demotion policy
    //
    PowerMgmtConfig->C1AutoDemotion = 0;
    PowerMgmtConfig->C3AutoDemotion = 0;

    switch (FspsUpd->FspsConfig.CStateAutoDemotion) {
      case 0:
        //
        // Disable C1 and C3 Auto-demotion
        //
        break;

      case 1:
        //
        // Enable C3/C6/C7 Auto-demotion to C1
        //
        PowerMgmtConfig->C1AutoDemotion = 1;
        break;

      case 2:
        //
        // Enable C6/C7 Auto-demotion to C3
        //
        PowerMgmtConfig->C3AutoDemotion = 1;
        break;

      default:
      case 3:
        //
        // Enable C6/C7 Auto-demotion to C1 and C3
        //
        PowerMgmtConfig->C1AutoDemotion = 1;
        PowerMgmtConfig->C3AutoDemotion = 1;
        break;
    }
    //
    // Configure Un-demotion.
    //
    PowerMgmtConfig->C1UnDemotion = 0;
    PowerMgmtConfig->C3UnDemotion = 0;

    switch (FspsUpd->FspsConfig.CStateUnDemotion) {
      case 0:
        //
        // Disable C1 and C3 Un-demotion
        //
        break;

      case 1:
        //
        // Enable C1 Un-demotion
        //
        PowerMgmtConfig->C1UnDemotion = 1;
        break;

      case 2:
        //
        // Enable C3 Un-demotion
        //
        PowerMgmtConfig->C3UnDemotion = 1;
        break;

      case 3:
        //
        // Enable C1 and C3 Un-demotion
        //
        PowerMgmtConfig->C1UnDemotion = 1;
        PowerMgmtConfig->C3UnDemotion = 1;
        break;

      default:
        break;
    }
    switch (FspsUpd->FspsConfig.MaxCoreCState) {
      case 0:
        PowerMgmtConfig->UnlimitedCstate = 1;
        break;

      case 1:
        PowerMgmtConfig->EnableC1 = 1;
        break;

      case 2:
        PowerMgmtConfig->EnableC3 = 1;
        break;

      case 3:
        PowerMgmtConfig->EnableC6 = 1;
        break;

      case 4:
        PowerMgmtConfig->EnableC7 = 1;
        break;

      case 5:
        PowerMgmtConfig->EnableC8 = 1;
        break;

      case 6:
        PowerMgmtConfig->EnableC9 = 1;
        break;

      case 7:
        PowerMgmtConfig->EnableC10 = 1;
        break;

      case 8:
        PowerMgmtConfig->EnableCCx = 1;
        break;

      default:
        break;
    }

    //
    // Pkg C-state Demotion/Un Demotion
    //
    PowerMgmtConfig->PkgCStateDemotion   = FspsUpd->FspsConfig.PkgCStateDemotion;
    PowerMgmtConfig->PkgCStateUnDemotion = FspsUpd->FspsConfig.PkgCStateUnDemotion;
  }

  return EFI_SUCCESS;
}
