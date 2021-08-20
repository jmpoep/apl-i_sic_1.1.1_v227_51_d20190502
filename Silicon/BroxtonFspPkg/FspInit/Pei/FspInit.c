/** @file
  Source file for FSP Init PEI module

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2016 Intel Corporation.

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

#include "FspInit.h"

#define EFI_PHYSICAL_ADDRESS  UINT64
#define MRC_DATA_REQUIRED_FROM_OUTSIDE
#define EFI_MEMORY_INIT
#include <MmrcData.h>

#define PLATFORM_CONFIG_STORE_FILE_NAME     "critical/mrc_trg_data"

static EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiEndOfPeiSignalPpiGuid,
    FspInitEndOfPeiCallback
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gFspEventEndOfFirmwareGuid,
    FspEndOfFirmwareCallback
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiEventReadyToBootGuid,
    FspReadyToBootCallback
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPciEnumerationCompleteProtocolGuid,
    FspInitAfterPciEnumerationCallback
  }
};

// .Read the DEVIDLEC register value
// .If CIP=1, wait until cleared (if host is not using interrupts - poll on the bit value until cleared)
#define  WaitForDevIdleRegCipToClear(Addr, Data)   \
  do {                                             \
    Data = Mmio32(Addr, R_HECI_DEVIDLEC);          \
    if ((Data & B_HECI_DEVIDLEC_CIP) == 0) break;  \
  }while(TRUE)


EFI_STATUS
EFIAPI
FspInitEndOfPeiCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:

  FSP initialization after PCI enumeration

Arguments:

  Event             A pointer to the Event that triggered the callback.
  Context           A pointer to private data registered with the callback function.

Returns:

  EFI_SUCCESS       The function completed successfully

  --*/
{
  FSPS_UPD                        *FspsUpd;
  EFI_BOOT_MODE                    BootMode;


  BootMode = 0;
  PeiServicesGetBootMode(&BootMode);

  DEBUG ((EFI_D_INFO, "FspInitEndOfPeiCallback++\n"));

  FspsUpd = (FSPS_UPD *)GetFspSiliconInitUpdDataPointer();
  if (FspsUpd->FspsConfig.P2sbUnhide == 0) {
    DEBUG ((EFI_D_INFO, "Hide P2SB device.\n"));
    PchHideP2sb (MmPciBase (DEFAULT_PCI_BUS_NUMBER_SC, PCI_DEVICE_NUMBER_P2SB, PCI_FUNCTION_NUMBER_P2SB));
  }

  DEBUG ((EFI_D_INFO, "FspInitEndOfPeiCallback--\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FspEndOfFirmwareCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:

  FSP initialization triggered by ExitBootService event for Fsp

Arguments:

  Event             A pointer to the Event that triggered the callback.
  Context           A pointer to private data registered with the callback function.

Returns:

  EFI_SUCCESS       The function completed successfully

  --*/
{
  UINT32        HeciBar;
  UINT32        Data;
  UINTN         P2sbBase;
  BOOLEAN       P2sbOrgStatus;
  EFI_BOOT_MODE BootMode;

  DEBUG ((EFI_D_INFO, "FspEndOfFirmwareCallback++\n"));


  BootMode = 0;
  PeiServicesGetBootMode(&BootMode);
  if ((BootMode != BOOT_ON_S3_RESUME)) {
    DEBUG ((EFI_D_INFO, "Send EOS Heci Message start...\n"));
    HeciEndOfServices();
    DEBUG ((EFI_D_INFO, "Send EOS Heci Message end...\n"));
  }

  DEBUG((EFI_D_INFO, "Put all HECI devices into D0I3 and hide HECI2 & 3 \n"));

  HeciBar = HeciPciRead32 (R_HECIMBAR0) & 0xFFFFFFF0 ;
  WaitForDevIdleRegCipToClear(HeciBar, Data);
  Mmio32Or(HeciBar, R_HECI_DEVIDLEC, B_HECI_DEVIDLEC_DEVIDLE);

  HeciBar = Heci2PciRead32 (R_HECIMBAR0) & 0xFFFFFFF0 ;
  WaitForDevIdleRegCipToClear(HeciBar, Data);
  Mmio32Or(HeciBar, R_HECI_DEVIDLEC, B_HECI_DEVIDLEC_DEVIDLE);

  HeciBar = Heci3PciRead32(R_HECIMBAR0) & 0xFFFFFFF0;
  WaitForDevIdleRegCipToClear(HeciBar, Data);
  Mmio32Or(HeciBar, R_HECI_DEVIDLEC, B_HECI_DEVIDLEC_DEVIDLE);

  P2sbOrgStatus = FALSE;
  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_SC,
                    PCI_DEVICE_NUMBER_P2SB,
                    PCI_FUNCTION_NUMBER_P2SB
                    );
  PchRevealP2sb (P2sbBase, &P2sbOrgStatus);

  //
  // To hide the HECI 2 in PCI configuraiton space
  //
  SideBandAndThenOr32(
    SB_PORTID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_CSE_D15F1_FUN_DIS,
    0xFFFFFFFF,
    (UINT32) BIT0
    );

  //
  // To hide the HECI 3 in PCI configuraiton space
  //
  SideBandAndThenOr32(
    SB_PORTID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_CSE_D15F2_FUN_DIS,
    0xFFFFFFFF,
    (UINT32) BIT0
    );

  if (!P2sbOrgStatus) {
    PchHideP2sb (P2sbBase);
  }

  DEBUG ((EFI_D_INFO, "FspEndOfFirmwareCallback--\n"));

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FspReadyToBootCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:

  FSP initialization after PCI enumeration

Arguments:

  Event             A pointer to the Event that triggered the callback.
  Context           A pointer to private data registered with the callback function.

Returns:

  EFI_SUCCESS       The function completed successfully

  --*/
{
  EFI_BOOT_MODE BootMode;

  DEBUG ((EFI_D_INFO, "FspReadyToBootCallback++\n"));

  BootMode = 0;
  PeiServicesGetBootMode(&BootMode);
  if ((BootMode != BOOT_ON_S3_RESUME)) {
    DEBUG ((EFI_D_INFO, "Start Send EOP Heci Message\n"));
    HeciEndOfPost();
    DEBUG ((EFI_D_INFO, "End Of Send EOP and EOS Heci Message\n"));
  }

  DEBUG ((EFI_D_INFO, "FspReadyToBootCallback--\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FspInitAfterPciEnumerationCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:

  FSP initialization after PCI enumeration

Arguments:

  Event             A pointer to the Event that triggered the callback.
  Context           A pointer to private data registered with the callback function.

Returns:

  EFI_SUCCESS       The function completed successfully

  --*/
{

  DEBUG ((EFI_D_INFO, "FspInitAfterPciEnumerationCallback++\n"));

  DEBUG ((EFI_D_INFO, "FspInitAfterPciEnumerationCallback--\n"));

  return EFI_SUCCESS;
}


VOID
InitPlatformDirectIrqs (
  VOID
  )
{

  UINT16                                AcpiBaseAddr = 0;
  UINT32                                PmcBaseAddr = 0;

  DEBUG((EFI_D_INFO, "InitPlatformDirectIrqs() - Start\n"));

  AcpiBaseAddr = (UINT16)PcdGet16(PcdScAcpiIoPortBaseAddress);
  PmcBaseAddr  = (UINT32)PcdGet32(PcdPmcIpc1BaseAddress0);

  IoAnd32 (
    AcpiBaseAddr + R_DIRECT_IRQ_EN,
    (UINT32)(~(B_DIRECT_IRQ_EN_XHCI_EN | B_DIRECT_IRQ_EN_XDCI_EN))
    );

  IoOr32 (
    AcpiBaseAddr + R_ACPI_GPE0a_EN,
    (B_ACPI_GPE0a_EN_XHCI_PME_EN | B_ACPI_GPE0a_EN_XDCI_PME_EN)
    );

  //
  // Write the SCI IRQ to ACPI Bridge in PMC
  //
  MmioAndThenOr32 (
    PmcBaseAddr + R_PMC_IRQ_SEL_2,
    0x0000FFFF,
    (V_PMC_IRQ_SEL_2_SCI_IRQ_9 << N_PMC_IRQ_SEL_2_SCIS) +
    (41 << N_PMC_IRQ_SEL_2_DIR_IRQ_SEL_PMIC)
    );

   DEBUG((EFI_D_INFO, "InitPlatformDirectIrqs() - End\n"));
}

EFI_STATUS
SaveMrcData (FSPM_UPD  *FspmUpd)
{
  EFI_STATUS                    Status;
  EFI_HOB_GUID_TYPE            *HobList;
  EFI_HOB_GUID_TYPE            *HobList1;
  MRC_PARAMS_SAVE_RESTORE      *MrcDataBuf;
  UINTN                         MrcDataSize;
  UINT32                       *MrcNvDataCrc;

  BOOT_VARIABLE_NV_DATA        *VarDataBuf;
  UINTN                         VarDataSize;

  UINTN                         fileSize;

  Status = EFI_NOT_FOUND;
  if (FspmUpd->FspmConfig.MrcFastBoot && FspmUpd->FspmConfig.MrcDataSaving) {
    DEBUG ((EFI_D_INFO, "Try to find MRC training data HOB.\n"));
    if (((HobList = GetFirstGuidHob (&gFspNonVolatileStorageHobGuid)) != NULL) &&
        ((HobList1 = GetFirstGuidHob (&gFspVariableNvDataHobGuid)) != NULL)) {
      //
      // Get MRC training data
      //
      MrcDataBuf   = GET_GUID_HOB_DATA (HobList);
      MrcDataSize  = sizeof(MRC_PARAMS_SAVE_RESTORE);

      HobList = GetFirstGuidHob (&gDramPolicyPpiGuid);
      ASSERT_EFI_ERROR (HobList != NULL);
      MrcNvDataCrc = (UINT32 *)GET_GUID_HOB_DATA (HobList);

      //
      // Get variable boot data
      //
      VarDataBuf   = GET_GUID_HOB_DATA (HobList1);
      VarDataSize  = sizeof(BOOT_VARIABLE_NV_DATA);

      fileSize = 0;
      HeciGetNVMFileSize(PLATFORM_CONFIG_STORE_FILE_NAME, &fileSize);
      if (fileSize != (MrcDataSize + VarDataSize)) {
        DEBUG ((EFI_D_INFO, "No MRC training data found, perform data save via HECI.\n"));
        Status = HeciWriteNVMFile(PLATFORM_CONFIG_STORE_FILE_NAME, 0, (UINT8*)MrcDataBuf, MrcDataSize, FALSE);
        DEBUG ((EFI_D_INFO, "Saved MRC training data with status (0x%08X)\n", Status));

        Status = HeciWriteNVMFile(PLATFORM_CONFIG_STORE_FILE_NAME, MrcDataSize, (UINT8*)VarDataBuf, VarDataSize, FALSE);
        DEBUG ((EFI_D_INFO, "Saved variable data with status (0x%08X)\n", Status));
      } else {
        if ((MrcNvDataCrc[0] == MrcDataBuf->MrcParamsSaveRestoreCrc) && (MrcNvDataCrc[1] == MrcDataBuf->SaMemCfgCrc)) {
          DEBUG ((EFI_D_INFO, "MRC training data is identical, skip saving\n"));
          Status = EFI_SUCCESS;
        } else {
          Status = HeciWriteNVMFile(PLATFORM_CONFIG_STORE_FILE_NAME, 0, (UINT8*)MrcDataBuf, MrcDataSize, FALSE);
          DEBUG ((EFI_D_INFO, "Saved MRC training data with status (0x%08X)\n", Status));
        }

        if (MrcNvDataCrc[2] == VarDataBuf->BootVariableNvDataCrc) {
          DEBUG ((EFI_D_INFO, "MRC variable boot data is identical, skip saving\n"));
          Status = EFI_SUCCESS;
        } else {
          Status = HeciWriteNVMFile(PLATFORM_CONFIG_STORE_FILE_NAME, MrcDataSize, (UINT8*)VarDataBuf, VarDataSize, FALSE);
          DEBUG ((EFI_D_INFO, "Saved variable data with status (0x%08X)\n", Status));
        }
        Status = EFI_SUCCESS;
      }
    }
  }
  return Status;
}

/**
  FSP Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
FspInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS          Status;
  FSPS_UPD            *FspsUpd;
  SI_POLICY_PPI       *SiPolicyPpi;
  SC_POLICY_PPI       *ScPolicyPpi;
  SI_CPU_POLICY_PPI   *SiCpuPolicyPpi;
  SI_SA_POLICY_PPI    *SiSaPolicyPpi;
  FSP_INFO_HEADER     *FspInfoHeader;
  FSPM_UPD            *FspmUpd;

  Status = EFI_SUCCESS;
  FspsUpd = NULL;

  DEBUG ((DEBUG_INFO, "FspInitEntryPoint() - start\n"));
  FspInfoHeader = GetFspInfoHeaderFromApiContext ();
  SetFspInfoHeader (FspInfoHeader);

  FspsUpd = (FSPS_UPD *)GetFspApiParameter();
  if (FspsUpd == NULL) {
    //
    // Use the UpdRegion as default
    //
    FspsUpd = (FSPS_UPD *) (FspInfoHeader->ImageBase + FspInfoHeader->CfgRegionOffset);
  }
  SetFspSiliconInitUpdDataPointer (FspsUpd);

  FspmUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();
  DEBUG ((DEBUG_INFO, "Saving MRC data using CSE through HECI interface\n"));
  SaveMrcData (FspmUpd);
  DEBUG((DEBUG_INFO, "Updating UPDs consumed in FspSiliconInitApi...\n"));

  //
  // Call ScCreateConfigBlocks to initialize platform policy structure
  // and get all Intel default policy settings.
  //
  ScCreateConfigBlocks (&ScPolicyPpi);
  //
  // Update policies which are related to UPDs
  //
  FspUpdatePeiScPolicy (ScPolicyPpi, FspsUpd);
  //
  // Install SC Policy PPI
  //
  Status = ScInstallPolicyPpi (ScPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Call CreateCpuConfigBlocks to initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreateCpuConfigBlocks (&SiCpuPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  FspUpdatePeiCpuPolicy (SiCpuPolicyPpi, FspsUpd);

  //
  // Install SiCpuPolicyPpi.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = CpuInstallPolicyPpi (SiCpuPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  SiSaPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiSaPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiSaPolicyPpi
             );
  if (!EFI_ERROR (Status)) {
    FspUpdatePeiSaPolicy (SiSaPolicyPpi, ScPolicyPpi, FspsUpd);
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to locate gSiSaPolicyPpiGuid\n"));
    ASSERT_EFI_ERROR (Status);
  }

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
            &gSiPolicyPpiGuid,
            0,
            NULL,
            (VOID **) &SiPolicyPpi
            );
  if (!EFI_ERROR (Status)) {
    FspUpdatePeiSiPolicy (SiPolicyPpi, FspsUpd);
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to locate gSiPolicyPpiGuid\n"));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Do basic SC init
  //
  Status = PlatformScInit (NULL, PeiServices, 0);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesNotifyPpi (&mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  InitPlatformDirectIrqs ();
  //
  // Installs the Report Status Code PPI
  //
  if (PcdGetBool(PcdMonoStatusCode)) {
    InstallMonoStatusCode (FileHandle, PeiServices);
  }

  DEBUG ((DEBUG_INFO, "FspInitEntryPoint() - end\n"));
  return Status;
}
