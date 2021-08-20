/** @file
  Header file for FSP Init PEI module

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

#ifndef _FSP_INIT_PEI_H_
#define _FSP_INIT_PEI_H_

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/SiPolicyLib.h>
#include <Library/CpuPolicyLib.h>
#include <Library/MmPciLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/PeiScPolicyLib.h>
#include <Library/ScPlatformLib.h>
#include <Library/HeciMsgLib.h>
#include <Library/HobLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/ScPolicy.h>
#include <ScAccess.h>
#include <SaAccess.h>
#include <Library/SideBandLib.h>
#include <SeCAccess.h>
#include <HeciRegs.h>
#include <FspmUpd.h>

#define END_OF_GPIO_TABLE   0xFFFFFFFF
#define MAX_GPIO_COUNT 300

#define SYSTEM_CONFIGURATION  VOID

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
  );

/**
  This function performs PCH PEI Policy update.

  @param[in] ScPolicyPpi    The SC Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiScPolicy (
  IN OUT SC_POLICY_PPI    *ScPolicyPpi,
  IN FSPS_UPD             *FspsUpd
  );

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
  );

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPolicy       The Silicon Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicy (
  IN OUT SI_POLICY_PPI *SiPolicy,
  IN FSPS_UPD          *FspsUpd
  );

EFI_STATUS
EFIAPI
FspInitAfterPciEnumerationCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
FspInitEndOfPeiCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
FspReadyToBootCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
FspEndOfFirmwareCallback (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
PlatformScInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN UINT16                      PlatformType
  );

EFI_STATUS
EFIAPI
InstallMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

#endif
