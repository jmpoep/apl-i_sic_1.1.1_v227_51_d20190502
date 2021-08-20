/** @file
  PEIM to initialize both IGD and PCI graphics card.

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

#include <Library/PeiGraphicsInitLib.h>
#include <Library/DebugLib.h>

#include <ScAccess.h>
#include <Library/PeiSaPolicyLib.h>

/**
  GraphicsInit: Initialize the IGD if no other external graphics is present

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] SaPolicyPpi          Instance of _SA_POLICY_PPI

  @retval  None
**/
VOID
GraphicsPreMemInit (
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  UINT16                GMSData;
  BOOLEAN               IGfxSupported;
  UINTN                 McD0BaseAddress;
  UINTN                 McD2BaseAddress;
  SA_PRE_MEM_CONFIG     *SaPreMemConfig = NULL;
  EFI_STATUS            Status;
  SI_SA_POLICY_PPI     *SiSaPolicyPpi;

  DEBUG ((DEBUG_INFO, "iGFX initialization Start\n"));

  Status = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gSiSaPreMemPolicyPpiGuid,
                                 0,
                                 NULL,
                                 (void **)&SiSaPolicyPpi
                                 );

  Status = GetConfigBlock((VOID *)SiSaPolicyPpi, &gSaPreMemConfigGuid, (VOID *)&SaPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  ///
  /// Set the VGA Decode bits to a good known starting point where both PEG and
  /// IGD VGA Decode Bits are both disabled.
  ///
  McD0BaseAddress   = MmPciBase (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN);
  MmioOr16 (McD0BaseAddress + R_SA_GGC, B_SA_GGC_IVD_MASK);
  ///
  /// Check if IGfx is supported
  ///
  McD2BaseAddress  = MmPciBase (SA_IGD_BUS, SA_IGD_DEV, SA_IGD_FUN_0);
  IGfxSupported    = (BOOLEAN) (MmioRead16 (McD2BaseAddress + R_SA_IGD_VID) != 0xFFFF);
  if (!IGfxSupported) {
    DEBUG ((DEBUG_INFO, "iGFX not detected, skipped!\n"));
    return;
  }
  ///
  /// If primary display device is IGD or no other display detected then enable IGD
  ///
  if (IGfxSupported && SaPreMemConfig->InternalGraphics != IGD_DISABLE) {

    DEBUG ((EFI_D_INFO, "IGD enabled.\n"));
    ///
    /// Program GFX Memory by setting D0.F0.R 050h [15:8]
    ///
    GMSData = (UINT8) SaPreMemConfig->IgdDvmt50PreAlloc;
    DEBUG ((EFI_D_INFO, "GMSData: 0x%x\n",GMSData));
    MmioAnd16 (McD0BaseAddress + R_SA_GGC, (UINT16) ~(B_SA_GGC_GMS_MASK));
    MmioOr16 (McD0BaseAddress + R_SA_GGC, ((GMSData & 0x3F) << N_SA_GGC_GMS_OFFSET));
    ///
    /// Program Graphics GTT Memory D0:F0:R50h[7:6]
    ///   01b => 2MB of GTT
    ///   10b => 4MB of GTT
    ///   11b => 8MB of GTT
    ///
    if (SaPreMemConfig->GttSize != V_SA_GGC_GGMS_DIS) {
        ASSERT (SaPreMemConfig->GttSize <= 3);
        MmioAndThenOr16 (McD0BaseAddress + R_SA_GGC, (UINT16) ~(B_SA_GGC_GGMS_MASK), (SaPreMemConfig->GttSize << N_SA_GGC_GGMS_OFFSET) & B_SA_GGC_GGMS_MASK);
    }
    ///
    /// Set register D2.F0.R 062h [4:0] = `00001b' to set a 256MByte aperture.
    /// This must be done before Device 2 registers are enumerated.
    ///
    MmioAndThenOr8 (McD2BaseAddress + R_SA_IGD_MSAC_OFFSET, (UINT8) ~(BIT4 + BIT3 + BIT2 + BIT1 + BIT0), (UINT8) SaPreMemConfig->ApertureSize);
    ///
    /// Enable IGD VGA Decode.  This is needed so the Class Code will
    /// be correct for the IGD Device when determining which device
    /// should be primary.  If disabled, IGD will show up as a non VGA device.
    ///
    if (SaPreMemConfig->PrimaryDisplay != IGD) {
      ///
      /// If IGD is forced to be enabled, but is a secondary display, disable IGD VGA Decode
      ///
      MmioOr16 (McD0BaseAddress + R_SA_GGC, B_SA_GGC_IVD_MASK);
      DEBUG ((EFI_D_INFO, "IGD VGA Decode is disabled because it's not a primary display.\n"));
    } else {
      MmioAnd16 (McD0BaseAddress + R_SA_GGC, (UINT16) ~(B_SA_GGC_IVD_MASK));
    }

  } else {

    DEBUG ((EFI_D_INFO, "Disable IGD Device.\n"));

    ///
    /// Disable IGD device
    /// Set Register D0:F0 Offset 50h [15:8] (GMS) = '00000000b'.
    /// This prevents UMA memory from being pre-allocated to IGD.
    /// Set D0:F0 Offset 50h [9:8] (GGMS) = '00b'.
    /// Set GTT Graphics Memory Size to 0
    ///
    MmioAndThenOr16 (McD0BaseAddress + R_SA_GGC, (UINT16) ~(B_SA_GGC_GGMS_MASK | B_SA_GGC_GMS_MASK), B_SA_GGC_IVD_MASK);

    SaPreMemConfig->GttSize           = 0;
    SaPreMemConfig->IgdDvmt50PreAlloc = 0;
    ///
    /// Disable IGD. D0.F0.R 054h [4] = '0b'.
    ///
    MmioAnd8 (McD0BaseAddress + R_SA_DEVEN, (UINT8) ~(B_SA_DEVEN_D2F0EN_MASK));
  }
  DEBUG ((DEBUG_INFO, "iGFX initialization End\n"));
}

