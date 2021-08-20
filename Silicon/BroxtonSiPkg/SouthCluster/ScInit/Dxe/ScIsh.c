/** @file
  Initializes SC ISH Devices

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
#include "ScInit.h"
#include <Protocol/GlobalNvsArea.h>
#include <ScRegs/RegsIsh.h>
#include "SaAccess.h"

/**
  Hide PCI config space of ISH devices and do any final initialization.

  @param[in] ScPolicy             The SC Policy instance

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureIshAtBoot (
  IN SC_POLICY_HOB                *ScPolicy
  )
{
  UINTN                           IshPciMmBase;
  UINT32                          IshMmioBase0;
  UINT32                          IshMmioBase1;
  UINT32                          Buffer32;

  DEBUG ((DEBUG_INFO, "ConfigureIshAtBoot() Start\n"));

  ///
  /// Initialize Variables
  ///
  IshPciMmBase     = 0;
  IshMmioBase0     = 0;
  IshMmioBase1     = 0;
  Buffer32         = 0;

  DEBUG ((DEBUG_INFO, "ConfigureIshAtBoot() End\n"));

  return EFI_SUCCESS;
}
