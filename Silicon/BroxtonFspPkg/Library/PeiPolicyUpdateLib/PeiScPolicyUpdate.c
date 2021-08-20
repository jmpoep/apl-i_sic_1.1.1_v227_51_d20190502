/** @file
  This file is SampleCode of the library for Intel SC PEI Policy initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2016 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ScPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/ConfigBlockLib.h>

/**
  This function performs PCH PEI Policy update.

  @param[in] ScPolicyPpi    The SC Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiScPolicy (
  IN OUT SC_POLICY_PPI    *ScPolicy,
  IN FSPS_UPD             *FspsUpd
  )
{
  UINTN                           Index;
  EFI_STATUS                      Status;
  SC_GENERAL_CONFIG               *GeneralConfig;
  SC_SATA_CONFIG                  *SataConfig;
  SC_PCIE_CONFIG                  *PcieConfig;
  SC_USB_CONFIG                   *UsbConfig;
  SC_HPET_CONFIG                  *HpetConfig;
  SC_IOAPIC_CONFIG                *IoApicConfig;
  SC_HDAUDIO_CONFIG               *HdaConfig;
  SC_GMM_CONFIG                   *GmmConfig;
  SC_LOCK_DOWN_CONFIG             *LockDownConfig;
  SC_LPSS_CONFIG                  *LpssConfig;
  SC_SCS_CONFIG                   *ScsConfig;
  SC_VTD_CONFIG                   *VtdConfig;
  SC_SMBUS_CONFIG                 *SmbusConfig;
  SC_ISH_CONFIG                   *IshConfig;
  SC_PM_CONFIG                    *PmConfig;
  SC_DCI_CONFIG                   *DciConfig;
  SC_FLASH_PROTECTION_CONFIG      *FlashProtectionConfig;
  SC_P2SB_CONFIG                  *P2sbConfig;
  SC_INTERRUPT_CONFIG             *InterruptConfig;
  SC_LPC_SIRQ_CONFIG              *SerialIrqConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FspUpdatePeiScPolicy\n"));

  Status = GetConfigBlock ((VOID *) ScPolicy, &gScGeneralConfigGuid, (VOID *) &GeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gPcieRpConfigGuid, (VOID *) &PcieConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gHpetConfigGuid, (VOID *) &HpetConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gHdAudioConfigGuid, (VOID *) &HdaConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gGmmConfigGuid, (VOID *) &GmmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gLpssConfigGuid, (VOID *) &LpssConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gVtdConfigGuid, (VOID *) &VtdConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gSmbusConfigGuid, (VOID *) &SmbusConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gDciConfigGuid, (VOID *) &DciConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ScPolicy, &gSerialIrqConfigGuid, (VOID *) &SerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Update ACPI and P2SB Policies
  //
  GeneralConfig->ResetSelect = FspsUpd->FspsConfig.ResetSelect;
  GeneralConfig->Crid        = FspsUpd->FspsConfig.CRIDSettings;

  //
  // Update HPET Policies
  //
  HpetConfig->Enable         = FspsUpd->FspsConfig.Hpet;
  HpetConfig->BdfValid       = FspsUpd->FspsConfig.HpetBdfValid;
  HpetConfig->BusNumber      = FspsUpd->FspsConfig.HpetBusNumber;
  HpetConfig->DeviceNumber   = FspsUpd->FspsConfig.HpetDeviceNumber;
  HpetConfig->FunctionNumber = FspsUpd->FspsConfig.HpetFunctionNumber;

  //
  // Update IOAPIC Policy
  //
  IoApicConfig->IoApicId       = FspsUpd->FspsConfig.IoApicId;
  IoApicConfig->BdfValid       = FspsUpd->FspsConfig.IoApicBdfValid;
  IoApicConfig->BusNumber      = FspsUpd->FspsConfig.IoApicBusNumber;
  IoApicConfig->DeviceNumber   = FspsUpd->FspsConfig.IoApicDeviceNumber;
  IoApicConfig->FunctionNumber = FspsUpd->FspsConfig.IoApicFunctionNumber;
  //
  // Update PCIE RP policies
  //
  PcieConfig->DisableRootPortClockGating          = FspsUpd->FspsConfig.PcieClockGatingDisabled;
  PcieConfig->EnablePort8xhDecode                 = FspsUpd->FspsConfig.PcieRootPort8xhDecode;
  PcieConfig->ScPciePort8xhDecodePortIndex        = FspsUpd->FspsConfig.Pcie8xhDecodePortIndex;
  PcieConfig->EnablePeerMemoryWrite               = FspsUpd->FspsConfig.PcieRootPortPeerMemoryWriteEnable;

  for (Index = 0; Index < SC_MAX_PCIE_ROOT_PORTS; Index++) {
    PcieConfig->RootPort[Index].Enable                            = FspsUpd->FspsConfig.PcieRootPortEn[Index];
    PcieConfig->RootPort[Index].SlotImplemented                   = FspsUpd->FspsConfig.PcieRpSlotImplemented[Index];
    PcieConfig->RootPort[Index].PhysicalSlotNumber                = FspsUpd->FspsConfig.PhysicalSlotNumber[Index];
    PcieConfig->RootPort[Index].Aspm                              = FspsUpd->FspsConfig.PcieRpAspm[Index];
    PcieConfig->RootPort[Index].L1Substates                       = FspsUpd->FspsConfig.PcieRpL1Substates[Index];
    PcieConfig->RootPort[Index].AcsEnabled                        = FspsUpd->FspsConfig.PcieRpAcsEnabled[Index];
    PcieConfig->RootPort[Index].PmSci                             = FspsUpd->FspsConfig.PcieRpPmSci[Index];
    PcieConfig->RootPort[Index].HotPlug                           = FspsUpd->FspsConfig.PcieRpHotPlug[Index];
    PcieConfig->RootPort[Index].AdvancedErrorReporting            = FspsUpd->FspsConfig.AdvancedErrorReporting[Index];
    PcieConfig->RootPort[Index].UnsupportedRequestReport          = FspsUpd->FspsConfig.UnsupportedRequestReport[Index];
    PcieConfig->RootPort[Index].FatalErrorReport                  = FspsUpd->FspsConfig.FatalErrorReport[Index];
    PcieConfig->RootPort[Index].NoFatalErrorReport                = FspsUpd->FspsConfig.NoFatalErrorReport[Index];
    PcieConfig->RootPort[Index].CorrectableErrorReport            = FspsUpd->FspsConfig.CorrectableErrorReport[Index];
    PcieConfig->RootPort[Index].PmeInterrupt                      = FspsUpd->FspsConfig.PmeInterrupt[Index];
    PcieConfig->RootPort[Index].SystemErrorOnFatalError           = FspsUpd->FspsConfig.SystemErrorOnFatalError[Index];
    PcieConfig->RootPort[Index].SystemErrorOnNonFatalError        = FspsUpd->FspsConfig.SystemErrorOnNonFatalError[Index];
    PcieConfig->RootPort[Index].SystemErrorOnCorrectableError     = FspsUpd->FspsConfig.SystemErrorOnCorrectableError[Index];
    PcieConfig->RootPort[Index].TransmitterHalfSwing              = FspsUpd->FspsConfig.PcieRpTransmitterHalfSwing[Index];
    PcieConfig->RootPort[Index].CompletionTimeout                 = FspsUpd->FspsConfig.PcieRpCompletionTimeout[Index];
    PcieConfig->RootPort[Index].PcieSpeed                         = FspsUpd->FspsConfig.PcieRpSpeed[Index];
    PcieConfig->RootPort[Index].SelectableDeemphasis              = FspsUpd->FspsConfig.PcieRpSelectableDeemphasis[Index];
   // LTR settings
    PcieConfig->RootPort[Index].LtrEnable                         = FspsUpd->FspsConfig.PcieRpLtrEnable[Index];
    PcieConfig->RootPort[Index].LtrConfigLock                     = FspsUpd->FspsConfig.PcieRpLtrConfigLock[Index];
    PcieConfig->RootPort[Index].LtrMaxSnoopLatency                = FspsUpd->FspsConfig.PcieRpLtrMaxSnoopLatency[Index];
    PcieConfig->RootPort[Index].LtrMaxNoSnoopLatency              = FspsUpd->FspsConfig.PcieRpLtrMaxNonSnoopLatency[Index];
    PcieConfig->RootPort[Index].SnoopLatencyOverrideMode          = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMode[Index];
    PcieConfig->RootPort[Index].SnoopLatencyOverrideMultiplier    = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMultiplier[Index];
    PcieConfig->RootPort[Index].SnoopLatencyOverrideValue         = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideValue[Index];
    PcieConfig->RootPort[Index].NonSnoopLatencyOverrideMode       = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMode[Index];
    PcieConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMultiplier[Index];
    PcieConfig->RootPort[Index].NonSnoopLatencyOverrideValue      = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideValue[Index];
    PcieConfig->RootPort[Index].PtmEnable                         = FspsUpd->FspsConfig.PtmEnable[Index];
    PcieConfig->RootPort[Index].ClkReqSupported                   = FspsUpd->FspsConfig.PcieRpClkReqSupported[Index];
    PcieConfig->RootPort[Index].ClkReqNumber                      = FspsUpd->FspsConfig.PcieRpClkReqNumber[Index];
  }

  //
  // Update SATA Policies
  //
  SataConfig->Enable   = FspsUpd->FspsConfig.EnableSata;
  SataConfig->SataMode = FspsUpd->FspsConfig.SataMode;
  SataConfig->SalpSupport = FspsUpd->FspsConfig.SataSalpSupport;
  for (Index = 0; Index < SC_MAX_SATA_PORTS; Index++) {
    SataConfig->PortSettings[Index].Enable           = FspsUpd->FspsConfig.SataPortsEnable[Index];
    SataConfig->PortSettings[Index].HotPlug          = FspsUpd->FspsConfig.SataPortsHotPlug[Index];
    SataConfig->PortSettings[Index].SpinUp           = FspsUpd->FspsConfig.SataPortsSpinUp[Index];
    SataConfig->PortSettings[Index].External         = FspsUpd->FspsConfig.SataPortsExternal[Index];
    SataConfig->PortSettings[Index].DevSlp           = FspsUpd->FspsConfig.SataPortsDevSlp[Index];
    SataConfig->PortSettings[Index].EnableDitoConfig = FspsUpd->FspsConfig.SataPortsEnableDitoConfig[Index];
    SataConfig->PortSettings[Index].DmVal            = FspsUpd->FspsConfig.SataPortsDmVal[Index];
    SataConfig->PortSettings[Index].DitoVal          = FspsUpd->FspsConfig.SataPortsDitoVal[Index];
    SataConfig->PortSettings[Index].SolidStateDrive  = FspsUpd->FspsConfig.SataPortsSolidStateDrive[Index];
  }
  SataConfig->TestMode                               = FspsUpd->FspsConfig.SataTestMode;

  //
  // Flash Security Recommendation,
  // Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit. Enabling this bit
  // will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
  // We always enable this as a platform policy.
  //
  LockDownConfig->BiosInterface = FspsUpd->FspsConfig.BiosInterface;
  LockDownConfig->BiosLock      = FspsUpd->FspsConfig.BiosLock;
  LockDownConfig->SpiEiss       = FspsUpd->FspsConfig.SpiEiss;
  LockDownConfig->RtcLock       = FspsUpd->FspsConfig.RtcLock;
  LockDownConfig->TcoLock       = FspsUpd->FspsConfig.TcoTimerHaltLock;

  //
  // Update HDA Policies
  //
  HdaConfig->Enable               = FspsUpd->FspsConfig.HdaEnable;
  HdaConfig->DspEnable            = FspsUpd->FspsConfig.DspEnable;
  HdaConfig->DspUaaCompliance     = FspsUpd->FspsConfig.HdAudioDspUaaCompliance;
  HdaConfig->Mmt                  = FspsUpd->FspsConfig.Mmt;
  HdaConfig->Hmt                  = FspsUpd->FspsConfig.Hmt;
  HdaConfig->IoBufferOwnership    = FspsUpd->FspsConfig.HdAudioIoBufferOwnership;
  HdaConfig->PwrGate              = FspsUpd->FspsConfig.HDAudioPwrGate;
  HdaConfig->ClkGate              = FspsUpd->FspsConfig.HDAudioClkGate;
  HdaConfig->Pme                  = FspsUpd->FspsConfig.Pme;
  if (HdaConfig->DspUaaCompliance) {
    HdaConfig->DspEndpointDmic      = FALSE;
    HdaConfig->DspEndpointBluetooth = FALSE;
  } else {
    HdaConfig->DspEndpointDmic      = FspsUpd->FspsConfig.DspEndpointDmic;
    HdaConfig->DspEndpointBluetooth = FspsUpd->FspsConfig.DspEndpointBluetooth;
  }
  if ((HdaConfig->IoBufferOwnership == ScHdaIoBufOwnerHdaLink) || (HdaConfig->DspUaaCompliance)) {
    HdaConfig->DspEndpointI2sSkp = FALSE;
    HdaConfig->DspEndpointI2sHp  = FALSE;
  } else {
    HdaConfig->DspEndpointI2sSkp   = FspsUpd->FspsConfig.DspEndpointI2sSkp;
    HdaConfig->DspEndpointI2sHp    = FspsUpd->FspsConfig.DspEndpointI2sHp;
  }
  HdaConfig->VcType                = FspsUpd->FspsConfig.HdAudioVcType;
  HdaConfig->HdAudioLinkFrequency  = FspsUpd->FspsConfig.HdAudioLinkFrequency;
  HdaConfig->IDispLinkFrequency    = FspsUpd->FspsConfig.HdAudioIDispLinkFrequency;
  HdaConfig->IDispLinkTmode        = FspsUpd->FspsConfig.HdAudioIDispLinkTmode;
  HdaConfig->DspFeatureMask        = FspsUpd->FspsConfig.DspFeatureMask;
  HdaConfig->DspPpModuleMask       = FspsUpd->FspsConfig.DspPpModuleMask;
  HdaConfig->ResetWaitTimer        = FspsUpd->FspsConfig.ResetWaitTimer;

  //
  // Install Verb table
  //
  HdaConfig->VerbTableEntryNum     = FspsUpd->FspsConfig.HdaVerbTableEntryNum;
  HdaConfig->VerbTablePtr          = FspsUpd->FspsConfig.HdaVerbTablePtr;

  //
  // Update GMM Policies
  //
  GmmConfig->Enable                  = FspsUpd->FspsConfig.Gmm;
  GmmConfig->ClkGatingPgcbClkTrunk   = FspsUpd->FspsConfig.ClkGatingPgcbClkTrunk;
  GmmConfig->ClkGatingSb             = FspsUpd->FspsConfig.ClkGatingSb;
  GmmConfig->ClkGatingSbClkTrunk     = FspsUpd->FspsConfig.ClkGatingSbClkTrunk;
  GmmConfig->ClkGatingSbClkPartition = FspsUpd->FspsConfig.ClkGatingSbClkPartition;
  GmmConfig->ClkGatingCore           = FspsUpd->FspsConfig.ClkGatingCore;
  GmmConfig->ClkGatingDma            = FspsUpd->FspsConfig.ClkGatingDma;
  GmmConfig->ClkGatingRegAccess      = FspsUpd->FspsConfig.ClkGatingRegAccess;
  GmmConfig->ClkGatingHost           = FspsUpd->FspsConfig.ClkGatingHost;
  GmmConfig->ClkGatingPartition      = FspsUpd->FspsConfig.ClkGatingPartition;
  GmmConfig->ClkGatingTrunk          = FspsUpd->FspsConfig.ClkGatingTrunk;

  //
  // Update ISH Policy
  //
  IshConfig->Enable   = FspsUpd->FspsConfig.IshEnable;

  //
  // Update USB Policies
  //
  UsbConfig->DisableComplianceMode      = FspsUpd->FspsConfig.DisableComplianceMode;

  //
  // xHCI (USB 3.0)
  //
  UsbConfig->Usb30Settings.Mode = FspsUpd->FspsConfig.Usb30Mode;

  UsbConfig->SsicConfig.SsicPort[0].Enable = FspsUpd->FspsConfig.SsicPortEnable[0];
  UsbConfig->SsicConfig.SsicPort[1].Enable = FspsUpd->FspsConfig.SsicPortEnable[1];
  UsbConfig->SsicConfig.SsicPort[0].Rate   = FspsUpd->FspsConfig.SsicRate[0];
  UsbConfig->SsicConfig.SsicPort[1].Rate   = FspsUpd->FspsConfig.SsicRate[1];
  UsbConfig->SsicConfig.DlanePwrGating     = FspsUpd->FspsConfig.DlanePwrGating;
  UsbConfig->XdciConfig.Enable             = FspsUpd->FspsConfig.UsbOtg;

  //
  // Overcurrent applies to walk-up xHCI ports only - not SSIC or HSIC
  //
  //  OC0: Used for the OTG port (port 0)
  //  OC1: Used for the 2 host walk-up ports
  //
  for (Index = 0 ; Index < SC_MAX_USB2_PORTS; Index++) {
    UsbConfig->PortUsb20[Index].OverCurrentPin             = FspsUpd->FspsConfig.PortUs20bOverCurrentPin[Index];
    UsbConfig->PortUsb20[Index].Enable                     = FspsUpd->FspsConfig.PortUsb20Enable[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.IUsbTxEmphasisEn = FspsUpd->FspsConfig.PortUsb20IUsbTxEmphasisEn[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.PerPortRXISet    = FspsUpd->FspsConfig.PortUsb20PerPortRXISet[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.HsSkewSel        = FspsUpd->FspsConfig.PortUsb20HsSkewSel[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.HsNpreDrvSel     = FspsUpd->FspsConfig.PortUsb20HsNpreDrvSel[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.PerPortTxPeHalf  = FspsUpd->FspsConfig.PortUsb20PerPortTxPeHalf[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.PerPortPeTxiSet  = FspsUpd->FspsConfig.PortUsb20PerPortPeTxiSet[Index];
    UsbConfig->PortUsb20[Index].AfeConfig.PerPortTxiSet    = FspsUpd->FspsConfig.PortUsb20PerPortTxiSet[Index];
  }
  for (Index = 0 ; Index < SC_MAX_USB3_PORTS; Index++) {
    UsbConfig->PortUsb30[Index].OverCurrentPin = FspsUpd->FspsConfig.PortUs30bOverCurrentPin[Index];
    UsbConfig->PortUsb30[Index].Enable         = FspsUpd->FspsConfig.PortUsb30Enable[Index];
  }

  //
  // Update LPSS Policies
  //
  LpssConfig->I2c0Enable    = FspsUpd->FspsConfig.I2c0Enable;
  LpssConfig->I2c1Enable    = FspsUpd->FspsConfig.I2c1Enable;
  LpssConfig->I2c2Enable    = FspsUpd->FspsConfig.I2c2Enable;
  LpssConfig->I2c3Enable    = FspsUpd->FspsConfig.I2c3Enable;
  LpssConfig->I2c4Enable    = FspsUpd->FspsConfig.I2c4Enable;
  LpssConfig->I2c5Enable    = FspsUpd->FspsConfig.I2c5Enable;
  LpssConfig->I2c6Enable    = FspsUpd->FspsConfig.I2c6Enable;
  LpssConfig->I2c7Enable    = FspsUpd->FspsConfig.I2c7Enable;
  LpssConfig->Hsuart0Enable = FspsUpd->FspsConfig.Hsuart0Enable;
  LpssConfig->Hsuart1Enable = FspsUpd->FspsConfig.Hsuart1Enable;
  LpssConfig->Hsuart2Enable = FspsUpd->FspsConfig.Hsuart2Enable;
  LpssConfig->Hsuart3Enable = FspsUpd->FspsConfig.Hsuart3Enable;
  LpssConfig->Spi0Enable    = FspsUpd->FspsConfig.Spi0Enable;
  LpssConfig->Spi1Enable    = FspsUpd->FspsConfig.Spi1Enable;
  LpssConfig->Spi2Enable    = FspsUpd->FspsConfig.Spi2Enable;
  LpssConfig->Uart2KernelDebugBaseAddress = FspsUpd->FspsConfig.Uart2KernelDebugBaseAddress;
  for (Index = 0; Index < LPSS_I2C_DEVICE_NUM; Index++) {
    LpssConfig->I2cClkGateCfg[Index] = FspsUpd->FspsConfig.I2cClkGateCfg[Index];
  }
  for (Index = 0; Index < LPSS_HSUART_DEVICE_NUM; Index++) {
    LpssConfig->HsuartClkGateCfg[Index] = FspsUpd->FspsConfig.HsuartClkGateCfg[Index];
  }
  for (Index = 0; Index < LPSS_SPI_DEVICE_NUM; Index++) {
    LpssConfig->SpiClkGateCfg[Index] = FspsUpd->FspsConfig.SpiClkGateCfg[Index];
  }

  //
  // Kernel Debugger (e.g. WinDBG) Enable
  //
  LpssConfig->OsDbgEnable = FspsUpd->FspsConfig.OsDbgEnable;
  LpssConfig->S0ixEnable  = FspsUpd->FspsConfig.LPSS_S0ixEnable;

  //
  // Update SCS Policies
  //
  ScsConfig->SdcardEnable                      = FspsUpd->FspsConfig.SdcardEnabled;
  ScsConfig->UfsEnable                         = FspsUpd->FspsConfig.UfsEnabled;
  ScsConfig->EmmcEnable                        = FspsUpd->FspsConfig.eMMCEnabled;
  ScsConfig->SdioEnable                        = FspsUpd->FspsConfig.SdioEnabled;
  ScsConfig->EmmcHostMaxSpeed                  = FspsUpd->FspsConfig.eMMCHostMaxSpeed;
  ScsConfig->GppLock                           = FspsUpd->FspsConfig.GppLock;
  ScsConfig->SdioEnable                        = FspsUpd->FspsConfig.SdioEnabled;
  ScsConfig->SdioRegDllConfig.TxCmdCntl        = FspsUpd->FspsConfig.SdioTxCmdCntl;
  ScsConfig->SdioRegDllConfig.TxDataCntl1      = FspsUpd->FspsConfig.SdioTxDataCntl1;
  ScsConfig->SdioRegDllConfig.TxDataCntl2      = FspsUpd->FspsConfig.SdioTxDataCntl2;
  ScsConfig->SdioRegDllConfig.RxCmdDataCntl1   = FspsUpd->FspsConfig.SdioRxCmdDataCntl1;
  ScsConfig->SdioRegDllConfig.RxCmdDataCntl2   = FspsUpd->FspsConfig.SdioRxCmdDataCntl2;
  ScsConfig->SdcardRegDllConfig.TxCmdCntl      = FspsUpd->FspsConfig.SdcardTxCmdCntl;
  ScsConfig->SdcardRegDllConfig.TxDataCntl1    = FspsUpd->FspsConfig.SdcardTxDataCntl1;
  ScsConfig->SdcardRegDllConfig.TxDataCntl2    = FspsUpd->FspsConfig.SdcardTxDataCntl2;
  ScsConfig->SdcardRegDllConfig.RxCmdDataCntl1 = FspsUpd->FspsConfig.SdcardRxCmdDataCntl1;
  ScsConfig->SdcardRegDllConfig.RxStrobeCntl   = FspsUpd->FspsConfig.SdcardRxStrobeCntl;
  ScsConfig->SdcardRegDllConfig.RxCmdDataCntl2 = FspsUpd->FspsConfig.SdcardRxCmdDataCntl2;
  ScsConfig->EmmcRegDllConfig.TxCmdCntl        = FspsUpd->FspsConfig.EmmcTxCmdCntl;
  ScsConfig->EmmcRegDllConfig.TxDataCntl1      = FspsUpd->FspsConfig.EmmcTxDataCntl1;
  ScsConfig->EmmcRegDllConfig.TxDataCntl2      = FspsUpd->FspsConfig.EmmcTxDataCntl2;
  ScsConfig->EmmcRegDllConfig.RxCmdDataCntl1   = FspsUpd->FspsConfig.EmmcRxCmdDataCntl1;
  ScsConfig->EmmcRegDllConfig.RxStrobeCntl     = FspsUpd->FspsConfig.EmmcRxStrobeCntl;
  ScsConfig->EmmcRegDllConfig.RxCmdDataCntl2   = FspsUpd->FspsConfig.EmmcRxCmdDataCntl2;
  ScsConfig->EmmcRegDllConfig.MasterSwCntl     = FspsUpd->FspsConfig.EmmcMasterSwCntl;

  //
  // Update VT-D Policy
  //
  VtdConfig->VtdEnable = FspsUpd->FspsConfig.VtdEnable;

  //
  // Update PM Policy
  //
  PmConfig->Timer8254ClkGateEn       = FspsUpd->FspsConfig.Timer8254ClkSetting;
  PmConfig->PwrBtnOverridePeriod     = FspsUpd->FspsConfig.PwrBtnOverridePeriod;
  PmConfig->DisableNativePowerButton = FspsUpd->FspsConfig.DisableNativePowerButton;
  PmConfig->PowerButterDebounceMode  = FspsUpd->FspsConfig.PowerButterDebounceMode;
  //
  // Update Flash Protect Policies
  //
  for (Index = 0; Index < SC_FLASH_PROTECTED_RANGES; Index++) {
    FlashProtectionConfig->ProtectRange[Index].WriteProtectionEnable  = FspsUpd->FspsConfig.WriteProtectionEnable[Index];
    FlashProtectionConfig->ProtectRange[Index].ReadProtectionEnable   = FspsUpd->FspsConfig.ReadProtectionEnable[Index];
  }

  //
  // Update FPRR ranges
  //
  FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase    =  FspsUpd->FspsConfig.ProtectedRangeBase[0];
  FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit   =  FspsUpd->FspsConfig.ProtectedRangeLimit[1];
  FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase    =  FspsUpd->FspsConfig.ProtectedRangeBase[0];
  FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit   =  FspsUpd->FspsConfig.ProtectedRangeLimit[1];

  //
  // Update DCI Policy
  //
  DciConfig->DciEn         = FspsUpd->FspsConfig.DciEn;
  DciConfig->DciAutoDetect = FspsUpd->FspsConfig.DciAutoDetect;

  //
  //
  // Update P2sbUnhide Policy
  P2sbConfig->P2sbUnhide   = FspsUpd->FspsConfig.P2sbUnhide;

  //
  // Update ITSS IRQ interrupt polarity control
  //
  for (Index = 0; Index < SC_NUM_IPC_REG; Index++) {
    InterruptConfig->IPC[Index] = FspsUpd->FspsConfig.IPC[Index];
  }

  //
  // Update Serial IRQ Policy
  //
  SerialIrqConfig->SirqEnable      = FspsUpd->FspsConfig.SirqEnable;
  SerialIrqConfig->SirqMode        = FspsUpd->FspsConfig.SirqMode;
  SerialIrqConfig->StartFramePulse = FspsUpd->FspsConfig.StartFramePulse;

  return EFI_SUCCESS;
}

