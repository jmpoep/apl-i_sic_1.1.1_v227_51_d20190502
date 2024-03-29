/** @file
  This file contains routines that get Soc Stepping

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2016 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/Pci30.h>
#include <SaAccess.h>
#include <ScRegs/RegsPmc.h>
#include <Library/SteppingLib.h>
#include <Library/MmPciLib.h>

/**
  Return SOC series type

  @retval BXT_SERIES  SOC series type
**/
BXT_SERIES
EFIAPI
GetBxtSeries (
  VOID
  )
{
  UINTN   McD0Base;
  UINT16  VenId;
  UINT16  DevId;

  McD0Base = MmPciBase (
               SA_MC_BUS,
               SA_MC_DEV,
               SA_MC_FUN
               );

  VenId = MmioRead16 (McD0Base + PCI_VENDOR_ID_OFFSET);
  DevId = MmioRead16 (McD0Base + PCI_DEVICE_ID_OFFSET);
  if (VenId == V_SA_MC_VID) {
    switch (DevId) {
      case V_SA_MC_DID1:
        return Bxt1;
        break;
      case V_SA_MC_DID3:
        return BxtP;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported BXT Series.\n"));
        return BxtSeriesMax;
        break;
    }
  }
  return BxtSeriesMax;
}

/**
  This procedure returns the FIB type on BXT E0

  @retval FIB Type
**/
BXT_FIB_TYPE
EFIAPI
GetFibType (
  VOID
  )
{
  UINT32 SsramBaseAddr;

  if (BxtStepping() != BxtB0) {
    return BxtNoFib;
  }

  SsramBaseAddr = MmioRead32 (
    MmPciAddress (0, 0, PCI_DEVICE_NUMBER_PMC, PCI_FUNCTION_NUMBER_PMC_SSRAM, 0x10)
    );

  if (((SsramBaseAddr & 0xFFFFFFF0) != 0xFFFFFFF0) && ((SsramBaseAddr & 0xFFFFFFF0) != 0)) {
    if ((MmioRead32 ((SsramBaseAddr & 0xFFFFFFF0) + BXT_FIB_DETECTION_SKPD_OFFSET) & BxtSoftFib) == BxtSoftFib) {
      return BxtSoftFib;
    } else {
      return BxtHardFib;
    }
  }

  DEBUG ((DEBUG_ERROR, "BXT E0 Detected But Could Not Read FIB Type from PMC SSRAM SKPD.\n"));

  return BxtNoFib;
}

/**
  This procedure will get Soc Stepping

  @retval Soc Stepping
**/
BXT_STEPPING
EFIAPI
BxtStepping (
  VOID
  )
{
  BXT_SERIES BxtSeries;
  UINT8      RevId;
  UINTN      McD0Base;

  McD0Base = MmPciBase (
               SA_MC_BUS,
               SA_MC_DEV,
               SA_MC_FUN
               );
  RevId = MmioRead8 (McD0Base + PCI_REVISION_ID_OFFSET);
  BxtSeries = GetBxtSeries ();

  if (BxtSeries == BxtP) {
    switch (RevId) {
      case V_SA_MC_RID_A:
        DEBUG ((DEBUG_INFO, "BXTP-B0 detected!\n"));
        return BxtPB0;
        break;
      case V_SA_MC_RID_B:
        DEBUG ((DEBUG_INFO, "BXTP-B1 detected!\n"));
        return BxtPB1;
        break;
      case V_SA_MC_RID_C:
        DEBUG ((DEBUG_INFO, "BXTP-E0 detected!\n"));
        return BxtPE0;
        break;
      case V_SA_MC_RID_D:
        DEBUG ((DEBUG_INFO, "BXTP-F1 detected!\n"));
        return BxtPF1;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported BXT-P stepping.\n"));
        return BxtSteppingMax;
        break;
    }
  } else if (BxtSeries == Bxt1) {
    switch (RevId) {
      case V_SA_MC_RID_6:
        DEBUG ((DEBUG_INFO, "BXT-B0 detected!\n"));
        return BxtB0;
        break;
      case V_SA_MC_RID_7:
        DEBUG ((DEBUG_INFO, "BXT-B1 detected!\n"));
        return BxtB1;
        break;
      case V_SA_MC_RID_8:
        DEBUG ((DEBUG_INFO, "BXT-B2 detected!\n"));
        return BxtB2;
        break;
      case V_SA_MC_RID_C:
        DEBUG ((DEBUG_INFO, "BXT-C0 detected!\n"));
        return BxtC0;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported BXT1 stepping.\n"));
        return BxtSteppingMax;
        break;
    }
  }
  return BxtSteppingMax;
}



