/** @file
  Header file for the SC Init PEIM

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
#ifndef _SC_INIT_PEI_H_
#define _SC_INIT_PEI_H_

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/TimerLib.h>
#include <ScAccess.h>
#include <Library/MmPciLib.h>
#include <Ppi/ScPolicy.h>
#include <Ppi/ScPcieDeviceTable.h>
#include <Ppi/SiPolicyPpi.h>
#include <IndustryStandard/Pci30.h>
#include <Library/ScPlatformLib.h>
#include <Library/SideBandLib.h>
#include <Library/ConfigBlockLib.h>
#include <Private/Library/PeiPciExpressInitLib.h>
#include <Private/Library/PeiDxeSmmScPciExpressHelpersLib.h>

/**
  Perform Root Port Initialization.

  @param[in] PortIndex              The root port to be initialized (zero based)
  @param[in] ScPolicyPpi            The ScPolicy PPI
  @param[in] PmcBaseAddress         The Power Management Controller Base Address
  @param[in] AcpiBaseAddr           The PM I/O Base address of the SC
  @param[out] PortWithMaxPayload128 At least one Port with MaxPayload set to 128 bits
  @param[in] TempPciBusMin          The temporary minimum Bus number for root port initialization
  @param[in] TempPciBusMax          The temporary maximum Bus number for root port initialization

  @retval EFI_SUCCESS               Device found. The root port must be enabled.
  @retval EFI_NOT_FOUND             No device is found on the root port. It may be disabled.
  @exception EFI_UNSUPPORTED        Unsupported operation.
**/
EFI_STATUS
ScInitSingleRootPort (
  IN  UINT8         PortIndex,
  IN  SC_POLICY_PPI *ScPolicyPpi,
  IN  UINTN         PmcBaseAddress,
  IN  UINT16        AcpiBaseAddr,
  OUT BOOLEAN       *PortWithMaxPayload128,
  IN  UINT8         TempPciBusMin,
  IN  UINT8         TempPciBusMax
  );

#endif
