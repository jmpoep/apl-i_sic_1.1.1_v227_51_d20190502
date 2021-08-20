/** @file
  The platform hook library.

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

@par Specification
**/

#include <PiPei.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/FspCommonLib.h>
#include <Library/GpioLib.h>
#include <FspmUpd.h>
#include <PlatformBaseAddresses.h>
#include <ScAccess.h>
#include <SaCommonDefinitions.h>

#define  V_FSP_LPSS_UART_PPR_CLK_N_DIV   32767
#define  V_FSP_LPSS_UART_PPR_CLK_M_DIV   602

BXT_GPIO_PAD_INIT  mGpioInitTbl[] =
{
  // GPIO_38 LPSS_UART0_RXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,   HizRx1I,DisPuPd,    GPIO_PADBAR+0x0130,  NORTH),
  // GPIO_39 LPSS_UART0_TXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,Last_Value,DisPuPd,    GPIO_PADBAR+0x0138,  NORTH),

  // GPIO_42 LPSS_UART1_RXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,   HizRx1I,DisPuPd,    GPIO_PADBAR+0x0150,  NORTH),
  // GPIO_43 LPSS_UART1_TXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,Last_Value,DisPuPd,    GPIO_PADBAR+0x0158,  NORTH),

  // GPIO_46 LPSS_UART2_RXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,   HizRx1I,DisPuPd,    GPIO_PADBAR+0x0170,  NORTH),
  // GPIO_47 LPSS_UART2_TXD
  BXT_GPIO_PAD_CONF(L"",   M1   ,    NA    ,  NA   ,  NA    ,   NA       , Wake_Disabled, P_20K_H,   NA    ,    NA,Last_Value,DisPuPd,    GPIO_PADBAR+0x0178,  NORTH),
};

RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (
  VOID
  )
{
  FSP_GLOBAL_DATA   *FspData;
  FSPM_UPD          *FspmUpd;
  UINT32            PciUartMmBase;
  UINT32            PciBar;
  UINT32            Data32;
  UINT8             DeviceIdx;

  //
  // FSP Global Data pointer must be valid
  //
  FspData  = GetFspGlobalDataPointer ();
  if ((FspData == NULL) || ((UINT32)FspData == 0xFFFFFFFF)) {
    ASSERT (FALSE);
  }

  //
  // For external serial port no need to do anything
  //
  FspmUpd = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer();
  if ((FspmUpd->FspmConfig.SerialDebugPortDevice >= 3) || (FspmUpd->FspmConfig.SerialDebugPortType != 2)) {
    return RETURN_SUCCESS;
  }

  //
  // Set GPIO for LPSS UART
  //
  DeviceIdx = FspmUpd->FspmConfig.SerialDebugPortDevice & 3;
  GpioPadConfigTable (2, &mGpioInitTbl[DeviceIdx * 2]);

  //
  // Program the UART PCI resource
  //
  PciUartMmBase = MmPciAddress (
                    0,
                    DEFAULT_PCI_BUS_NUMBER_SC,
                    PCI_DEVICE_NUMBER_LPSS_HSUART,
                    (PCI_FUNCTION_NUMBER_LPSS_HSUART0 + DeviceIdx),
                    0
                    );
  if (MmioRead8 (PciUartMmBase + R_LPSS_IO_STSCMD) & 0x02) {
    PciBar = MmioRead32 (PciUartMmBase + R_LPSS_IO_BAR) & 0xFFFFFFF0;
  } else {
    PciBar = LPSS_UART_TEMP_BASE_ADDRESS;
    MmioWrite32 (PciUartMmBase + R_LPSS_IO_BAR,  PciBar);
    MmioWrite32 (PciUartMmBase + R_LPSS_IO_BAR + 4, 0);
    MmioWrite32 (PciUartMmBase + R_LPSS_IO_BAR1, PciBar + 0x1000);
    MmioWrite32 (PciUartMmBase + R_LPSS_IO_BAR1 + 4, 0);
    MmioWrite8  (PciUartMmBase + R_LPSS_IO_STSCMD, 0x06);
  }

  //
  // Bring UART out of reset
  //
  MmioWrite32 (PciBar + R_LPSS_IO_MEM_RESETS, B_LPSS_IO_MEM_iDMA_RESET_REL | B_LPSS_IO_MEM_HC_RESET_REL);

  //
  // Set clock
  //
  Data32 = (V_FSP_LPSS_UART_PPR_CLK_N_DIV << 16) | (V_FSP_LPSS_UART_PPR_CLK_M_DIV << 1) | B_LPSS_IO_MEM_PCP_CLK_EN;
  MmioWrite32 (PciBar + R_LPSS_IO_MEM_PCP, Data32);
  MmioWrite32 (PciBar + R_LPSS_IO_MEM_PCP, Data32 | B_LPSS_IO_MEM_PCP_CLK_UPDATE);
  MmioRead32  (PciBar + R_LPSS_IO_MEM_PCP);

  return RETURN_SUCCESS;
}
