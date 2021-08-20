/** @file
 Early SC platform initialization.

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

@par Specification Reference:
**/

#include <Library/PcdLib.h>
#include <Library/PciExpressLib.h>
#include <Library/PeiScPolicyLib.h>
#include <Library/PreSiliconLib.h>
#include <Library/SideBandLib.h>
#include <Library/DebugLib.h>
#include <ScAccess.h>
#include <SaAccess.h>
#include <Ppi/Stall.h>
#include <Ppi/ScPolicy.h>
#include <FspsUpd.h>
#include <Library/FspCommonLib.h>

#define  SYSTEM_CONFIGURATION  VOID


#ifndef __GNUC__
#pragma warning (push)
#pragma warning (disable : 4245)
#pragma warning (pop)
#endif

UINT8
ReadCmosBank1Byte (
  IN UINT8                      Address
  )
{
  UINT8                           Data;

  IoWrite8(R_RTC_EXT_INDEX, Address);
  Data = IoRead8 (R_RTC_EXT_TARGET);
  return Data;
}

VOID
WriteCmosBank1Byte (
  IN UINT8                     Address,
  IN UINT8                     Data
  )
{
  IoWrite8 (R_RTC_EXT_INDEX, Address);
  IoWrite8 (R_RTC_EXT_TARGET, Data);
}

VOID
CheckPowerOffNow (
  VOID
  )
/*++

Routine Description:

  Turn off system if needed.

Arguments:

  PeiServices Pointer to PEI Services
  CpuIo       Pointer to CPU I/O Protocol

Returns:

  None.

--*/
{
  UINT16  Pm1Sts;
  UINT16  AcpiBaseAddr;

  ///
  /// Read ACPI Base Address
  ///
  AcpiBaseAddr = (UINT16)PcdGet16 (PcdScAcpiIoPortBaseAddress);

  //
  // Read and check the ACPI registers
  //
  Pm1Sts = IoRead16 (AcpiBaseAddr + R_ACPI_PM1_STS);
  if ((Pm1Sts & B_ACPI_PM1_STS_PWRBTN) == B_ACPI_PM1_STS_PWRBTN) {
    IoWrite16 (AcpiBaseAddr + R_ACPI_PM1_STS, B_ACPI_PM1_STS_PWRBTN);
    IoWrite16 (AcpiBaseAddr + R_ACPI_PM1_CNT, V_ACPI_PM1_CNT_S5);
    IoWrite16 (AcpiBaseAddr + R_ACPI_PM1_CNT, V_ACPI_PM1_CNT_S5 + B_ACPI_PM1_CNT_SLP_EN);
  }
}

VOID
ClearPowerState (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  UINT16  Data16;
  UINT32  Data32;
  UINT16  AcpiBaseAddr;
  FSPS_UPD     *FspsUpd;

  ///
  /// Read ACPI Base Address
  ///
  AcpiBaseAddr = (UINT16)PcdGet16(PcdScAcpiIoPortBaseAddress);

  //
  // Check for PowerState option for AC power loss and program the chipset
  //

  //
  // Clear PWROK and Power Failure Bit (Set to Clear)
  // Ensure that we don't clear bits that are write-one clear in the GEN_PMCON1 register
  //
  // TODO: Check if it is OK to clear here
  Data32 = MmioRead32 (PMC_BASE_ADDRESS + R_PMC_GEN_PMCON_1);
  Data32 &= (~(B_PMC_GEN_PMCON_MIN_SLP_S4 |
               B_PMC_GEN_PMCON_SRS |
               B_PMC_GEN_PMCON_GLOBAL_RST_STS |
               B_PMC_GEN_PMCON_WARM_RST_STS |
               B_PMC_GEN_PMCON_COLD_RST_STS |
               B_PMC_GEN_PMCON_COLD_BOOT_STS
            ));
  Data32 |= (B_PMC_GEN_PMCON_PWROK_FLR | B_PMC_GEN_PMCON_SUS_PWR_FLR);
  MmioWrite32 (PMC_BASE_ADDRESS + R_PMC_GEN_PMCON_1, Data32);

  //
  // Clear the GPE and PM enable
  //
  IoWrite16 (AcpiBaseAddr + R_ACPI_PM1_EN, (UINT16) 0x00);
  IoWrite32 (AcpiBaseAddr + R_ACPI_GPE0a_EN, (UINT32) 0x00);

  //
  // Halt the TCO timer
  //
  FspsUpd = (FSPS_UPD *)GetFspSiliconInitUpdDataPointer ();
  if (FspsUpd->FspsConfig.TcoTimerHaltLock) {
    DEBUG ((DEBUG_INFO,"Halting the TCO Timer (Watchdog)\n"));

    Data16 = IoRead16 (AcpiBaseAddr + R_TCO1_CNT);
    Data16 |= B_TCO1_CNT_TMR_HLT;
    IoWrite16 (AcpiBaseAddr + R_TCO1_CNT, Data16);

    //
    // NMI NOW bit is "Write '1' to clear"
    //

    //
    // Before we clear the TO status bit here we need to save the results in a CMOS bit for later use.
    //
    Data32 = IoRead16 (AcpiBaseAddr + R_TCO_STS);
    if ((Data32 & B_TCO_STS_SECOND_TO) == B_TCO_STS_SECOND_TO)
    {
#if (defined(HW_WATCHDOG_TIMER_SUPPORT) && (HW_WATCHDOG_TIMER_SUPPORT != 0))
      WriteCmosBank1Byte (
        EFI_CMOS_PERFORMANCE_FLAGS,
        ReadCmosBank1Byte (EFI_CMOS_PERFORMANCE_FLAGS) | B_CMOS_TCO_WDT_RESET
        );
#endif
    }
    //
    // Now clear the TCO status bit (Write '1' to clear)
    //
    IoWrite32 (AcpiBaseAddr + R_TCO_STS, (UINT32) (Data32 | B_TCO_STS_SECOND_TO));
  }
}

VOID
ClearSmiAndWake (
  VOID
  )
/*++

Routine Description:

  Clear any SMI status or wake status left over from boot.

Arguments:

Returns:

  None.

--*/
{
  UINT16  Pm1Sts;
  UINT32  Gpe0Sts;
  UINT32  SmiSts;
  UINT16  AcpiBaseAddr;

  ///
  /// Read ACPI Base Address
  ///
  AcpiBaseAddr = (UINT16)PcdGet16(PcdScAcpiIoPortBaseAddress);

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (AcpiBaseAddr + R_ACPI_PM1_STS);
  Gpe0Sts = IoRead32 (AcpiBaseAddr + R_ACPI_GPE0a_STS);
  SmiSts  = IoRead32 (AcpiBaseAddr + R_SMI_STS);

  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts = (B_ACPI_PM1_STS_PRBTNOR | B_ACPI_PM1_STS_PWRBTN);

  Gpe0Sts |=
    (
      B_ACPI_GPE0a_STS_CORE_GPIO |
      B_ACPI_GPE0a_STS_SUS_GPIO |
      B_ACPI_GPE0a_STS_PME_B0 |
      B_ACPI_GPE0a_STS_BATLOW |
      B_ACPI_GPE0a_STS_PCI_EXP |
      B_ACPI_GPE0a_STS_GUNIT_SCI |
      B_ACPI_GPE0a_STS_PUNIT_SCI |
      B_ACPI_GPE0a_STS_SWGPE |
      B_ACPI_GPE0a_STS_HOT_PLUG
    );

  SmiSts |=
    (
      B_SMI_STS_PERIODIC |
      B_SMI_STS_TCO |
      B_SMI_STS_SWSMI_TMR |
      B_SMI_STS_APM |
      B_SMI_STS_ON_SLP_EN |
      B_SMI_STS_BIOS
    );

  //
  // Write them back
  //
  IoWrite16 (AcpiBaseAddr + R_ACPI_PM1_STS, Pm1Sts);
  IoWrite32 (AcpiBaseAddr + R_ACPI_GPE0a_STS, Gpe0Sts);
  IoWrite32 (AcpiBaseAddr + R_SMI_STS, SmiSts);
}

EFI_STATUS
EFIAPI
IchReset (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Provide hard reset PPI service.
  To generate full hard reset, write 0x0E to ICH RESET_GENERATOR_PORT (0xCF9).

Arguments:

  PeiServices       General purpose services available to every PEIM.

Returns:

  Not return        System reset occurred.
  EFI_DEVICE_ERROR  Device error, could not reset the system.

--*/
{
  IoWrite8 (
    R_RST_CNT,
    V_RST_CNT_HARDSTARTSTATE
    );

  IoWrite8 (
    R_RST_CNT,
    V_RST_CNT_HARDRESET
    );

  //
  // System reset occurred, should never reach at this line.
  //
  ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);

  return EFI_DEVICE_ERROR;
}

VOID
ScPlatformIntInit (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN SYSTEM_CONFIGURATION       *SystemConfiguration
  )
{
  EFI_BOOT_MODE BootMode;
  UINT8         Data8;

  (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  if ((BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Clear all pending SMI. On S3 clear power button enable so it wll not generate an SMI
    //
    ClearSmiAndWake ();
  }

  ClearPowerState (SystemConfiguration);

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  Data8 = IoRead8 (R_NMI_SC);
  IoWrite8(R_NMI_SC, (UINT8) (Data8 | B_NMI_SC_PCI_SERR_EN | B_NMI_SC_IOCHK_NMI_EN));

}

VOID
IchRcrbInit (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  //
  // Disable the Watchdog timer expiration from causing a system reset
  //
  MmioOr8 (PMC_BASE_ADDRESS + R_PMC_PM_CFG, B_PMC_PM_CFG_NO_REBOOT);

  //
  // HPET is enabled in ScInitPeim.c
  //
}

//
// Returns a pointer to the memory-mapped capability
//
UINTN
MmPciCapability (
  IN UINT16           Segment,
  IN UINT16           Bus,
  IN UINT16           Device,
  IN UINT16           Function,
  IN UINT8            CapabilityId
  )
{
  UINT8 Capability;
  UINTN PciDeviceBase = PCI_EXPRESS_LIB_ADDRESS (Bus, Device, Function, 0x0);

  // Check that capabilities are flagged as enabled in the PCI status register
  ASSERT(EFI_PCI_STATUS_CAPABILITY ==
    (EFI_PCI_STATUS_CAPABILITY & PciExpressRead8(PciDeviceBase + PCI_PRIMARY_STATUS_OFFSET)));

  // Get the config space-relative offset of the capabilities list from the PCI capabilities pointer
  Capability = PciExpressRead8(PciDeviceBase + 0x34);  // Pointer to the first element of the capability linked list

  // Walk the linked list of capabilities
  while (Capability) {  // List is terminated by a next ptr == 0x0
    if (CapabilityId == PciExpressRead8(PciDeviceBase + Capability + 0x0)) { // ID is at offset 0
      return (PciDeviceBase + Capability);
    }
    Capability = PciExpressRead8(PciDeviceBase + Capability + 0x1); // Next pointer is at offset 1
  }

  return EFI_NOT_FOUND;
}


EFI_STATUS
PlatformScInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN UINT16                      PlatformType
  )
{
  IchRcrbInit (PeiServices, SystemConfiguration);

  ScPlatformIntInit (PeiServices, SystemConfiguration);

  return EFI_SUCCESS;
}

