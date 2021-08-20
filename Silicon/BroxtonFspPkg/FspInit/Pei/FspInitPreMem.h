/** @file
  Header file for FSP Init Pre-Memory PEI module

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

#ifndef _FSP_INIT_PRE_MEMORY_H_
#define _FSP_INIT_PRE_MEMORY_H_

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/CacheLib.h>
#include <Library/TimerLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspPlatformLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/Reset.h>
#include <FspEas.h>
#include <FspmUpd.h>
#include <Ppi/MasterBootMode.h>
#include <Library/PeiSiPolicyLib.h>
#include <Library/CpuPolicyLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/PeiScPolicyLib.h>
#include <Library/PreSiliconLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
#include <Library/ScPlatformLib.h>
#include <Ppi/SeCUma.h>
#include <SeCAccess.h>
#include <Ppi/TemporaryRamSupport.h>
#include <Guid/FspHeaderFile.h>
#include <Ppi/BlockIo.h>
#include <Library/BpdtLib.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Guid/MemoryConfigData.h>
#include <Ppi/SeCUma.h>
#include <Ppi/DramPolicyPpi.h>
#include "FspBoardGpiosPreMem.h"
#include <Library/HeciMsgLib.h>
#include <Library/PeiMmcMainLib.h>
#include <FspStatusCode.h>
#include <Library/ReportStatusCodeLib.h>
/**
  This function will be called when MRC is done.

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

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
  );

/**
  This function performs CPU PEI Policy initialization in PreMem.

  @param[in, out] SiCpuPolicyPpi  The Cpu Policy PPI instance
  @param[in]      FspmUpd         The pointer of FspmUpd

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicyPreMem (
  IN OUT  SI_CPU_POLICY_PPI *SiCpuPolicyPpi,
  IN FSPM_UPD               *FspmUpd
  );

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
  );

/**
    FspUpdatePeiSaPolicy performs SA Pre Mem PEI Policy initialization

  @param[in out] SiSaPolicyPpi     SI_SA_POLICY PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdateSaPolicyPreMem (
  IN OUT SI_SA_POLICY_PPI       *SiSaPolicyPpi,
  IN FSPM_UPD                   *FspmUpd
  );

/**
  This function performs Silicon PEI Policy initialization in pre-memory.

  @param[in] SiPolicy       The Silicon Policy PPI instance
  @param[in] FspmUpd        The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicyPreMem (
  IN OUT SI_POLICY_PPI *SiPolicy,
  IN FSPM_UPD          *FspmUpd
  );

/**
  This function checks the boot medium using HECI during pre-memory.

  @param[in/out] DeviceIndex       The output value: SPI, eMMC, UFS, Unknown

  @retval
**/
VOID FspCheckBootDevice(
  IN OUT INTN                  *DeviceIndex,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

EFI_STATUS
EFIAPI
SeCUmaEntry (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  );

/**
  DramInitializePolicy creates the default setting of Dram Policy.

  @param[out] DramPolicyPpi       The pointer to get Dram Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
DramInitializePolicy(
  OUT  DRAM_POLICY_PPI **DramPolicyPpi//,
  );

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
  );

EFI_STATUS
GetMrcDataFromSysCfg (
 IN  DRAM_POLICY_PPI *DramPolicy
 );

UINT32
GetMrcDataFromSys (
 IN  VOID **Buffer
);

#endif
