## @file
#  Platform description.
#
# @copyright
#  INTEL CONFIDENTIAL
#  Copyright 2008 - 2019 Intel Corporation.
#
#  The source code contained or described herein and all documents related to the
#  source code ("Material") are owned by Intel Corporation or its suppliers or
#  licensors. Title to the Material remains with Intel Corporation or its suppliers
#  and licensors. The Material may contain trade secrets and proprietary and
#  confidential information of Intel Corporation and its suppliers and licensors,
#  and is protected by worldwide copyright and trade secret laws and treaty
#  provisions. No part of the Material may be used, copied, reproduced, modified,
#  published, uploaded, posted, transmitted, distributed, or disclosed in any way
#  without Intel's prior express written permission.
#
#  No license under any patent, copyright, trade secret or other intellectual
#  property right is granted to or conferred upon you by disclosure or delivery
#  of the Materials, either expressly, by implication, inducement, estoppel or
#  otherwise. Any license under such intellectual property rights must be
#  express and approved by Intel in writing.
#
#  Unless otherwise agreed by Intel in writing, you may not remove or alter
#  this notice or any other notice embedded in Materials by Intel or
#  Intel's suppliers or licensors in any way.
#
#  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
#  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
#  the terms of your license agreement with Intel or your vendor. This file may
#  be modified by the user, subject to additional terms of the license agreement.
#
# @par Specification
##
[Defines]
  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE FSP_PACKAGE                     = BroxtonFspPkg
  DEFINE PLATFORM_SI_PACKAGE             = Silicon/BroxtonSiPkg

  DEFINE FSP_IBBL_SIZE                   = 0x2000
  DEFINE BLD_IBBL_SIZE                   = 0x6000
  DEFINE FSP_IBBM_SIZE                   = 0x59000
  DEFINE BLD_IBBM_SIZE                   = 0x20000

  DEFINE CAR_BASE_ADDRESS                = 0xFEF00000  # @PcdTemporaryRamBase
  DEFINE BLD_RAM_DATA_SIZE               = 0x22000     # BOOTLOADER temp memory size
  DEFINE FSP_RAM_DATA_SIZE               = 0x2A000     # FSP temp memory size
  DEFINE FSP_EMP_DATA_SIZE               = 0x05000     # Free space
  DEFINE BLD_RAM_CODE_SIZE               = $(BLD_IBBM_SIZE)
  DEFINE FSP_RAM_CODE_SIZE               = $(FSP_IBBM_SIZE)
  DEFINE CAR_REGION_SIZE                 = 0x100000
  DEFINE CSE_SRAM_FETCH_SIZE             = $(FSP_IBBL_SIZE) + $(BLD_IBBL_SIZE)

  DEFINE IBBM_BASE_ADDRESS               = 0xFEF71000
  DEFINE IBBR_BASE_ADDRESS               = 0x00200000

  DEFINE FSP_IMAGE_ID                    = 0x245053464C504124  # $APLFSP$

  DEFINE BXTI_PF_ENABLE     = -DBXTI_PF_ENABLE=0
  DEFINE ENBDT_PF_ENABLE    = -DENBDT_PF_ENABLE=1

  DEFINE TABLET_PF_ENABLE   = -DTABLET_PF_ENABLE=0

  DEFINE SATA_ENABLE        = TRUE
  DEFINE PCIESC_ENABLE      = TRUE
  DEFINE HG_ENABLE          = FALSE
  DEFINE PEI_DISPLAY_ENABLE = TRUE
  DEFINE PERFORMANCE_ENABLE = FALSE  # FSPO-APL-0002
  DEFINE INTEL_FPDT_ENABLE  = TRUE
  DEFINE FSP_BUILD          = TRUE

  DEFINE MRC_BUILD_OPTION_MSFT   = -Fa$* -DEFI_MEMORY_INIT -DSMM_SUPPORT -DDEBUG_HOOKS -DBXTPMRC -DMRC_DEBUG_DISABLE=0 -Oi- -GL-  -Gs81920
  DEFINE MRC_BUILD_OPTION_GCC    = -DEFI_MEMORY_INIT -DSMM_SUPPORT -DDEBUG_HOOKS -DBXTPMRC -DMRC_DEBUG_DISABLE
  #
  # Extra flags
  #
!if $(TARGET) == DEBUG
  DEFINE MRCDEBUG_ENABLE = TRUE
!else
  DEFINE MRCDEBUG_ENABLE = FALSE
!endif

!include $(FSP_PACKAGE)/BroxtonFspPkgConfig.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = BroxtonFspPkg
  PLATFORM_GUID                  = 1BEDB57A-7904-406e-8486-C89FC7FB39EE
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/BroxtonFspPkg
  SUPPORTED_ARCHITECTURES        = IA32
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = BroxtonFspPkg/BroxtonFspPkg.fdf

  #
  # UPD tool definition
  #
  FSP_T_UPD_TOOL_GUID            = 34686CA3-34F9-4901-B82A-BA630F0714C6
  FSP_M_UPD_TOOL_GUID            = 39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
  FSP_S_UPD_TOOL_GUID            = CAE3605B-5B34-4C85-B3D7-27D54273C40F
  FSP_T_UPD_FFS_GUID             = 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA
  FSP_M_UPD_FFS_GUID             = D5B86AEA-6AF7-40D4-8014-982301BC3D89
  FSP_S_UPD_FFS_GUID             = E3CD9B18-998C-4F76-B65E-98B154E5446F

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

[LibraryClasses]
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  PeiPolicyUpdatePreMemLib|$(FSP_PACKAGE)/Library/PeiPolicyUpdatePreMemLib/PeiPolicyUpdatePreMemLib.inf
  PeiPolicyUpdateLib|$(FSP_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf

  #
  # North Cluster
  #
  ConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Override/ClientSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf

  #
  # SC
  #
  SiPolicyLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
  ScPlatformLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/ScPlatformLib/ScPlatformLib.inf
  ScInitCommonLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSmmScInitCommonLib/PeiDxeSmmScInitCommonLib.inf
  UsbCommonLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeUsbCommonLib/PeiDxeUsbCommonLib.inf
  PeiScPolicyLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/PeiScPolicyLib/PeiScPolicyLib.inf
  ScInitLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiScInitLib/PeiScInitLibFsp.inf
  ScInitCommonLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSmmScInitCommonLib/PeiDxeSmmScInitCommonLib.inf
  ScSmbusCommonLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSmmScSmbusCommonLib/PeiDxeSmmScSmbusCommonLib.inf
  ScSpiCommonLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/BaseScSpiCommonLib/BaseScSpiCommonLib.inf
  I2cLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/I2CLib/I2CLib.inf
  HsioLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/PeiDxeSmmHsioLib/PeiDxeSmmHsioLib.inf
  PeiVtdLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/PeiVtdLib/PeiVtdLib.inf

  #
  # Basic
  #
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf

  #
  # UEFI & PI
  #
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  #
  # Generic Modules
  #
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf

  #
  # Misc
  #
  TimerLib | $(FSP_PACKAGE)/Library/AcpiTimerLib/AcpiTimerLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf

  #
  # Silicon initialization library
  #
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmMmPciLib/PeiDxeSmmMmPciLib.inf
  SteppingLib|$(PLATFORM_SI_PACKAGE)/Library/SteppingLib/SteppingLib.inf
  HeciInitLib|$(PLATFORM_SI_PACKAGE)/Txe/Library/Private/PeiDxeHeciInitLib/PeiDxeHeciInitLib.inf
  PreSiliconLib|$(PLATFORM_SI_PACKAGE)/Library/PreSiliconLib/PreSiliconLib.inf
  SideBandLib|$(PLATFORM_SI_PACKAGE)/Library/SideBandLib/SideBandLib.inf
  GpioLib|$(PLATFORM_SI_PACKAGE)/Library/GpioLib/GpioLib.inf
  BpdtLib|$(PLATFORM_SI_PACKAGE)/Library/BpdtLib/BpdtLib.inf

  #
  # ICH
  #
  ScAslUpdateLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/SampleCode/Library/AslUpdate/Dxe/ScAslUpdateLib.inf
  SataInitLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSataInitLib/PeiDxeSataInitLib.inf
  ScHdaLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/DxeScHdaLib/DxeScHdaLib.inf

  #
  # Platform
  #
  BaseIpcLib|$(PLATFORM_SI_PACKAGE)/Library/PmcIpcLib/BaseIpcLib.inf
  MonoStatusCodeLib|$(FSP_PACKAGE)/Library/MonoStatusCode/MonoStatusCode.inf

[LibraryClasses.IA32.PEIM,LibraryClasses.IA32.PEI_CORE,LibraryClasses.IA32.SEC]
  #
  # RC Package
  #
  !include $(PLATFORM_SI_PACKAGE)/SiPkgPeiLib.dsc

  #
  # SEC and PEI phase common
  #
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf
  CacheAsRamLib|$(FSP_PACKAGE)/Library/BaseCacheAsRamLib/BaseCacheAsRamLib.inf
  CacheLib|IntelFsp2Pkg/Library/BaseCacheLib/BaseCacheLib.inf
  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspPlatformLib|IntelFsp2Pkg/Library/BaseFspPlatformLib/BaseFspPlatformLib.inf
  SecCpuLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/SecCpuLib/SecCpuLibFsp.inf
  SecPlatformLib|$(PLATFORM_SI_PACKAGE)/SampleCode/Library/SecPlatformLib/SecPlatformLibFsp.inf
  MmcMediaDevicePeiPolicyLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/MmcMediaDevicePei/MmcMediaDevicePeiFsp.inf

  DebugDeviceLib|$(FSP_PACKAGE)/Library/DebugDeviceLib/DebugDeviceLib.inf
!if $(TARGET) == DEBUG
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  SerialPortLib|$(FSP_PACKAGE)/Library/SerialPortLib/SerialPortLib.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!if $(PEI_DEBUG_GOP_BUILD) == TRUE
  SerialPortLib|$(FSP_PACKAGE)/Library/SerialPortLib/SerialPortLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
!endif

  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PostCodeLib|$(FSP_PACKAGE)/Library/BasePostCodeLibPort80Ex/BasePostCodeLibPort80Ex.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|$(PLATFORM_SI_PACKAGE)/Override/ClientSiliconPkg/Library/PeiPerformanceLibFpdt/PeiPerformanceLibFpdt.inf  # FSPO-APL-0002
!else
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
!endif
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf

  #
  # CPU
  #
  BiosGuardLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiBiosGuardLibNull/PeiBiosGuardLibNull.inf
  CpuMailboxLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuMailboxLib/PeiDxeSmmCpuMailboxLib.inf

  SataInitLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSataInitLib/PeiDxeSataInitLib.inf
  PeiDxeSmmScPciExpressHelpersLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiDxeSmmScPciExpressHelpersLib/PeiDxeSmmScPciExpressHelpersLib.inf
  PeiPciExpressInitLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/Private/PeiPciExpressInitLib/PeiPciExpressInitLib.inf
  PeiSpiInitLib|$(PLATFORM_SI_PACKAGE)/SouthCluster/Library/PeiSpiInitLib/PeiSpiInitLib.inf
  PeiSiPolicyLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
  #
  # CPU
  #
  CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf
  AslUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  CpuInitLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuInitLib/PeiCpuInitLibFsp.inf
  MpServiceLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiMpServiceLib/PeiMpServiceLibFsp.inf
  CpuPowerMgmtLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuPowerMgmtLib/PeiCpuPowerMgmtLibFsp.inf
  CpuS3Lib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuS3Lib/PeiCpuS3LibFsp.inf
  CpuCommonLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiDxeSmmCpuCommonLib/PeiDxeSmmCpuCommonLib.inf
  SmbiosCpuLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiSmbiosCpuLib/PeiSmbiosCpuLib.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf

!if $(TXT_ENABLE) == TRUE
  PeiTxtLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiTxtLib/PeiTxtLib.inf
!else
  PeiTxtLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiTxtLibNull/PeiTxtLibNull.inf
!endif
!if $(OVERCLOCK_ENABLE) == TRUE
  CpuOcLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuOcLib/PeiCpuOcLib.inf
  CpuOcInitLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuOcInitLib/PeiCpuOcInitLib.inf
!else
  CpuOcLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuOcLibNull/PeiCpuOcLibNull.inf
  CpuOcInitLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/Private/PeiCpuOcInitLibNull/PeiCpuOcInitLibNull.inf
!endif
  SeCUmaLib|$(PLATFORM_SI_PACKAGE)/Txe/Library/Private/PeiSeCUma/SeCUmaFsp.inf

  Heci2PowerManagementLib|$(PLATFORM_SI_PACKAGE)/Txe/Library/BaseHeci2PowerManagementNullLib/BaseHeci2PowerManagementNullLib.inf
  PlatformHookLib|$(FSP_PACKAGE)/Library/PlatformHookLib/PlatformHookLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild]
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot    | TRUE
  gBroxtonFspPkgTokenSpaceGuid.PcdFspHeaderRevision       | 0x03
  gBroxtonFspPkgTokenSpaceGuid.PcdFspImageIdString        | $(FSP_IMAGE_ID)
  !if $(PERFORMANCE_ENABLE) == TRUE
    gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
    !if $(INTEL_FPDT_ENABLE) == TRUE
      gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|120 # FSPO-APL-0002
    !else
      gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|60
    !endif
  !endif

  #
  # FSP CAR Usages  (BL RAM | FSP RAM | FSP CODE)
  #
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase         | $(CAR_BASE_ADDRESS)
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize         | $(CAR_REGION_SIZE)
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize      | $(CAR_REGION_SIZE) - $(BLD_RAM_DATA_SIZE)
  gBroxtonFspPkgTokenSpaceGuid.PcdFspTemporaryRamDataSize | $(FSP_RAM_DATA_SIZE)
  gIntelFsp2PkgTokenSpaceGuid.PcdFspReservedBufferSize    | 0x0400

  # This defines how much space will be used for heap in FSP temporary memory
  # x % of FSP temporary memory will be used for heap
  # (100 - x) % of FSP temporary memory will be used for stack
  gIntelFsp2PkgTokenSpaceGuid.PcdFspHeapSizePercentage    | 65

  # This is a platform specific global pointer used by FSP
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress | 0xFED00148

!if $(VP_BIOS_ENABLE) == TRUE
  gEfiBxtTokenSpaceGuid.PcdPlatformIdRegisterOffset       | 0x2C
!endif

  gIntelFsp2PkgTokenSpaceGuid.PcdFspReservedMemoryLength  | 0x00400000

!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel   | 0x00000000
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask           | 0
!else
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel   | 0x80000047
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask           | 0x27
!endif
  gEfiBxtTokenSpaceGuid.PcdPmcGcrBaseAddress              | 0xFE043000
  gEfiBxtTokenSpaceGuid.PcdPmcIpc1BaseAddress0            | 0xFE042000
  gEfiBxtTokenSpaceGuid.PcdPmcIpc1BaseAddress1            | 0xFE044000

[PcdsPatchableInModule]
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress       | 0xE0000000
  #
  # This entry will be patched during the build process
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress        | 0x12345678

!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel        | 0
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel        | 0x80000047
!endif

[PcdsFeatureFlag]
!if ($(PEI_DEBUG_GOP_BUILD) == TRUE) AND ($(TARGET) == RELEASE)
  gBroxtonFspPkgTokenSpaceGuid.PcdMonoStatusCode          | TRUE
!else
  gBroxtonFspPkgTokenSpaceGuid.PcdMonoStatusCode          | FALSE
!endif

[PcdsDynamicVpd.Upd]
  #
  # This section is not used by the normal build process
  # However, FSP will use dedicated tool to handle it and generate a
  # VPD similar binary block (User Configuration Data). This block will
  # be accessed through a generated data structure directly rather than
  # PCD services. This is for size consideration.
  # Format:
  #   gBroxtonFspPkgTokenSpaceGuid.Updxxxxxxxxxxxxn      | OFFSET | LENGTH | VALUE
  # Only simple data type is supported
  #

  #
  # Comments with !BSF will be used to generate BSF file
  # Comments with !HDR will be used to generate H header file
  #

  # Global definitions in BSF
  # !BSF PAGES:{MEM:"FSP MemoryInit Settings", SIL:"FSP SiliconInit Settings"}
  # !BSF BLOCK:{NAME:"Apollo Lake Platform", VER:"0.1"}

  # !BSF FIND:{APLUPD_T}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FsptUpdSignature: {APLUPD_T}
  gBroxtonFspPkgTokenSpaceGuid.Signature                   | 0x0000 | 0x08 | 0x545F4450554C5041
  # !BSF NAME:{FsptUpdRevision}  TYPE:{None}
  gBroxtonFspPkgTokenSpaceGuid.Revision                    | 0x0008 | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gBroxtonFspPkgTokenSpaceGuid.Reserved                    | 0x0009 | 0x17 | {0x00}

  # !HDR COMMENT:{FSPT_COMMON_UPD:Fsp T Common UPD}
  # !HDR EMBED:{FSPT_COMMON_UPD:FsptCommonUpd:START}
  gBroxtonFspPkgTokenSpaceGuid.Revision                    | 0x0020 | 0x01 | 0x01
  gBroxtonFspPkgTokenSpaceGuid.Reserved                    | 0x0021 | 0x03 | {0x00}

  # Base address of the microcode region.
  gBroxtonFspPkgTokenSpaceGuid.MicrocodeRegionBase         | 0x0024 | 0x04 | 0x00000000

  # Length of the microcode region.
  gBroxtonFspPkgTokenSpaceGuid.MicrocodeRegionLength       | 0x0028 | 0x04 | 0x00000000

  # Base address of the cacheable flash region.
  gBroxtonFspPkgTokenSpaceGuid.CodeRegionBase              | 0x002C | 0x04 | 0x00000000

  # Length of the cacheable flash region.
  gBroxtonFspPkgTokenSpaceGuid.CodeRegionLength            | 0x0030 | 0x04 | 0x00000000

  # !HDR EMBED:{FSPT_COMMON_UPD:FsptCommonUpd:END}
  gBroxtonFspPkgTokenSpaceGuid.Reserved1                   | 0x0034 | 0x0C | {0x00}

  # !HDR COMMENT:{FSP_T_CONFIG:Fsp T Configuration}
  # !HDR EMBED:{FSP_T_CONFIG:FsptConfig:START}

  # !HDR EMBED:{FSP_T_CONFIG:FsptConfig:END}
  gBroxtonFspPkgTokenSpaceGuid.ReservedFsptUpd1             | 0x0040 | 0x10 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.
  gBroxtonFspPkgTokenSpaceGuid.UpdTerminator               | 0x0056 | 0x02 | 0x55AA

  ################################################################################
  #
  # UPDs consumed in FspMemoryInit Api
  #
  ################################################################################
  # !BSF FIND:{APLUPD_M}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspmUpdSignature: {APLUPD_M}
  gBroxtonFspPkgTokenSpaceGuid.Signature                   | 0x0000 | 0x08 | 0x4D5F4450554C5041
  # !BSF NAME:{FspmUpdRevision}  TYPE:{None}
  gBroxtonFspPkgTokenSpaceGuid.Revision                    | 0x0008 | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gBroxtonFspPkgTokenSpaceGuid.Reserved                    | 0x0009 | 0x17 | {0x00}

  # !HDR COMMENT:{FSPM_ARCH_UPD:Fsp M Architectural UPD}
  # !HDR EMBED:{FSPM_ARCH_UPD:FspmArchUpd:START}
  # !BSF PAGE:{MEM}

  gBroxtonFspPkgTokenSpaceGuid.Revision                    | 0x0020 | 0x01 | 0x01

  gBroxtonFspPkgTokenSpaceGuid.Reserved                    | 0x0021 | 0x03 | {0x00}

  # !HDR STRUCT:{VOID*}
  gBroxtonFspPkgTokenSpaceGuid.NvsBufferPtr                | 0x0024 | 0x04 | 0x00000000

  # !HDR STRUCT:{VOID*}
  # !BSF NAME:{StackBase}
  # !BSF HELP:{Stack base for FSP use. Default: 0xFEF16000}
  gBroxtonFspPkgTokenSpaceGuid.StackBase                   | 0x0028 | 0x04 | 0xFEF16000

  # !BSF NAME:{StackSize}
  # !BSF HELP:{To pass the stack size for FSP use. Bootloader can programmatically get the FSP requested StackSize by using the defaults in the FSP-M component. This is the minimum stack size expected by this revision of FSP. Default: 0x2A000}
  gBroxtonFspPkgTokenSpaceGuid.StackSize                   | 0x002C | 0x04 | 0x0002A000

  # !BSF NAME:{BootLoaderTolumSize}
  # !BSF HELP:{To pass Bootloader Tolum size.}
  gBroxtonFspPkgTokenSpaceGuid.BootLoaderTolumSize         | 0x0030 | 0x04 | 0x00000000

  # !BSF NAME:{Bootmode}
  # !BSF HELP:{To maintain Bootmode details.}
  gPlatformFspPkgTokenSpaceGuid.Bootmode                   | 0x0034 | 0x04 | 0x00000000

  # !HDR EMBED:{FSPM_ARCH_UPD:FspmArchUpd:END}
  gBroxtonFspPkgTokenSpaceGuid.Reserved1                   | 0x0038 | 0x08 | {0x00}

  # !HDR COMMENT:{FSP_M_CONFIG:Fsp M Configuration}
  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:START}
  # !BSF NAME:{Debug Serial Port Base address}
  # !BSF TYPE:{EditNum, HEX, (0x00000000,0xFFFFFFFF)}
  # !BSF HELP:{Debug serial port base address. This option will be used only when the 'Serial Port Debug Device'}
  # !BSF HELP:{+ option is set to 'External Device'. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SerialDebugPortAddress      | 0x0040 | 0x04 | 0x00000000

  # !BSF NAME:{Debug Serial Port Type} TYPE:{Combo}
  # !BSF OPTION:{0:NONE, 1:I/O, 2:MMIO}
  # !BSF HELP:{16550 compatible debug serial port resource type. NONE means no serial port support. 0x02:MMIO(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SerialDebugPortType         | 0x0044 | 0x01 | 0x02

  # !BSF NAME:{Serial Port Debug Device} TYPE:{Combo}
  # !BSF OPTION:{0:SOC UART0, 1:SOC UART1, 2:SOC UART2, 3:External Device}
  # !BSF HELP:{Select active serial port device for debug. }
  # !BSF HELP:{+For SOC UART devices,'Debug Serial Port Base' options will be ignored. 0x02:SOC UART2(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SerialDebugPortDevice       | 0x0045 | 0x01 | 0x02

  # !BSF NAME:{Debug Serial Port Stride Size} TYPE:{Combo}
  # !BSF OPTION:{0:1, 2:4}
  # !BSF HELP:{Debug serial port register map stride size in bytes. 0x00:1, 0x02:4(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SerialDebugPortStrideSize   | 0x0046 | 0x01 | 0x02

  # !BSF NAME:{Memory Fast Boot}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable MRC fast boot support. 0x00:Disable, 0x01:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.MrcFastBoot                 | 0x0047 | 0x01 | 0x01

  # !BSF NAME:{Integrated Graphics Device} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable : Enable Integrated Graphics Device (IGD) when selected as the Primary Video Adaptor. Disable: Always disable IGD. 0x00:Disable, 0x01:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Igd                         | 0x0048 | 0x01 | 0x01

  # !BSF NAME:{DVMT Pre-Allocated} TYPE:{Combo}
  # !BSF OPTION:{0x02:64 MB, 0x03:96 MB, 0x04:128 MB, 0x05:160 MB, 0x06:192 MB, 0x07:224 MB, 0x08:256 MB, 0x09:288 MB, 0x0A:320 MB, 0x0B:352 MB, 0x0C:384 MB, 0x0D:416 MB, 0x0E:448 MB, 0x0F:480 MB, 0x10:512 MB}
  # !BSF HELP:{Select DVMT 5.0 Pre-Allocated (Fixed) Graphics Memory size used by the Internal Graphics Device. 0x02:64 MB(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IgdDvmt50PreAlloc           | 0x0049 | 0x01 | 0x02

  # !BSF NAME:{Aperture Size} TYPE:{Combo}
  # !BSF OPTION:{0x1:128 MB, 0x2:256 MB, 0x3:512 MB}
  # !BSF HELP:{Select the Aperture Size used by the Internal Graphics Device. 0x1:128 MB(Default), 0x2:256 MB, 0x3:512 MB.}
  gBroxtonFspPkgTokenSpaceGuid.IgdApertureSize             | 0x004A | 0x01 | 0x01

  # !BSF NAME:{GTT Size} TYPE:{Combo}
  # !BSF OPTION:{0x1:2 MB, 0x2:4 MB, 0x3:8 MB}
  # !BSF HELP:{Select the GTT Size used by the Internal Graphics Device. 0x1:2 MB, 0x2:4 MB, 0x3:8 MB(Default).}
  gBroxtonFspPkgTokenSpaceGuid.GttSize                     | 0x004B | 0x01 | 0x03

  # !BSF NAME:{Primary Display} TYPE:{Combo}
  # !BSF OPTION:{0x0:AUTO, 0x2:IGD, 0x3:PCI}
  # !BSF HELP:{Select which of IGD/PCI Graphics device should be Primary Display. 0x0:AUTO(Default), 0x2:IGD, 0x3:PCI}
  gBroxtonFspPkgTokenSpaceGuid.PrimaryVideoAdaptor         | 0x004C | 0x01 | 0x00

  # !BSF NAME:{Package} TYPE:{Combo}
  # !BSF OPTION:{0x0:SODIMM, 0x1:BGA, 0x2:BGA mirrored (LPDDR3 only), 0x3:SODIMM/UDIMM with Rank 1 Mirrored (DDR3L)}
  # !BSF HELP:{NOTE: Specifies CA Mapping for all technologies. Supported CA Mappings: 0 - SODIMM(Default); 1 - BGA; 2 - BGA mirrored (LPDDR3 only); 3 - SODIMM/UDIMM with Rank 1 Mirrored (DDR3L); Refer to the IAFW spec for specific details about each CA mapping.}
  gBroxtonFspPkgTokenSpaceGuid.Package                     | 0x004D | 0x01 | 0x00

  # !BSF NAME:{Profile} TYPE:{Combo}
  # !BSF OPTION:{0x1:WIO2_800_7_8_8, 0x2:WIO2_1066_9_10_10, 0x3:LPDDR3_1066_8_10_10, 0x4:LPDDR3_1333_10_12_12, 0x5:LPDDR3_1600_12_15_15, 0x6:LPDDR3_1866_14_17_17, 0x7:LPDDR3_2133_16_20_20, 0x8:LPDDR4_1066_10_10_10, 0x9:LPDDR4_1600_14_15_15, 0xA:LPDDR4_2133_20_20_20, 0xB:LPDDR4_2400_24_22_22, 0xC:LPDDR4_2666_24_24_24, 0xD:LPDDR4_2933_28_27_27, 0xE:LPDDR4_3200_28_29_29, 0xF:DDR3_1066_6_6_6, 0x10:DDR3_1066_7_7_7, 0x11:DDR3_1066_8_8_8, 0x12:DDR3_1333_7_7_7, 0x13:DDR3_1333_8_8_8, 0x14:DDR3_1333_9_9_9, 0x15:DDR3_1333_10_10_10, 0x16:DDR3_1600_8_8_8, 0x17:DDR3_1600_9_9_9, 0x18:DDR3_1600_10_10_10, 0x19:DDR3_1600_11_11_11, 0x1A:DDR3_1866_10_10_10, 0x1B:DDR3_1866_11_11_11, 0x1C:DDR3_1866_12_12_12, 0x1D:DDR3_1866_13_13_13, 0x1E:DDR3_2133_11_11_11, 0x1F:DDR3_2133_12_12_12, 0x20:DDR3_2133_13_13_13, 0x21:DDR3_2133_14_14_14, 0x22:DDR4_1333_10_10_10, 0x23:DDR4_1600_10_10_10, 0x24:DDR4_1600_11_11_11, 0x25:DDR4_1600_12_12_12, 0x26:DDR4_1866_12_12_12, 0x27:DDR4_1866_13_13_13, 0x28:DDR4_1866_14_14_14, 0x29:DDR4_2133_14_14_14, 0x2A:DDR4_2133_15_15_15, 0x2B:DDR4_2133_16_16_16, 0x2C:DDR4_2400_15_15_15, 0x2D:DDR4_2400_16_16_16, 0x2E:DDR4_2400_17_17_17, 0x2F:DDR4_2400_18_18_18}
  # !BSF HELP:{Profile list. 0x19(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Profile                     | 0x004E | 0x01 | 0x19

  # !BSF NAME:{MemoryDown} TYPE:{Combo}
  # !BSF OPTION:{0x0:No, 0x1:Yes, 0x2:1MD+SODIMM (for DDR3L only) ACRD, 0x3:1x32 LPDDR4}
  # !BSF HELP:{Memory Down. 0x0(Default).}
  gBroxtonFspPkgTokenSpaceGuid.MemoryDown                  | 0x004F | 0x01 | 0x00

  # !BSF NAME:{DDR3LPageSize} TYPE:{Combo}
  # !BSF OPTION:{0x1:1KB, 0x2:2KB}
  # !BSF HELP:{NOTE: Only for memory down (soldered down memory with no SPD). 0x01:1KB(Default), 0x02:2KB.}
  gBroxtonFspPkgTokenSpaceGuid.DDR3LPageSize               | 0x0050 | 0x01 | 0x01

  # !BSF NAME:{DDR3LASR} TYPE:{Combo}
  # !BSF OPTION:{0x0:Not Supported, 0x1:Supported}
  # !BSF HELP:{NOTE: Only for memory down. This is specific to ddr3l and used for refresh adjustment in Self Refresh, does not affect LP4. 0x00:Not Supported(Default), 0x01:Supported.}
  gBroxtonFspPkgTokenSpaceGuid.DDR3LASR                    | 0x0051 | 0x01 | 0x00

  # !BSF NAME:{ScramblerSupport} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Scrambler Support -  Enable or disable the memory scrambler. Data scrambling is provided as a means to increase signal integrity/reduce RFI generated by the DRAM interface. This is achieved by randomizing seed that encodes/decodes memory data so repeating a worse case pattern is hard to repeat. 00: Disable Scrambler Support, 01: Enable Scrambler Support}
  gBroxtonFspPkgTokenSpaceGuid.ScramblerSupport            | 0x0052 | 0x01 | 0x01

  # !BSF NAME:{InterleavedMode} TYPE:{Combo} OPTION:{0x0:Disable, 0x2:Enable}
  # !BSF HELP:{This field is ignored if one of the PnP channel configurations is used. If the memory configuration is different, then the field is used directly to populate. 0x00:Disable(Default), 0x02:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.InterleavedMode             | 0x0053 | 0x01 | 0x00

  # !BSF NAME:{ChannelHashMask} TYPE:{EditNum, HEX, (0x0,0xF)}
  # !BSF HELP:{ChannelHashMask and SliceHashMask allow for the channel hashing algorithm to be modified. These inputs are not used for configurations where an optimized ChannelHashMask has been provided by the PnP validation teams. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ChannelHashMask             | 0x0054 | 0x02 | 0x00

  # !BSF NAME:{SliceHashMask} TYPE:{EditNum, HEX, (0x0,0xF)}
  # !BSF HELP:{ChannelHashMask and SliceHashMask allow for the channel hashing algorithm to be modified. These inputs are not used for configurations where an optimized ChannelHashMask has been provided by the PnP validation teams. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SliceHashMask               | 0x0056 | 0x02 | 0x00

  # !BSF NAME:{ChannelsSlicesEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{ChannelSlicesEnable field is not used at all on BXTP. The Channel Slice Configuration is calculated internally based on the enabled channel configuration. 0x00:Disable(Default), 0x01:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.ChannelsSlicesEnable        | 0x0058 | 0x01 | 0x00

  # !BSF NAME:{MinRefRate2xEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Provided as a means to defend against Row-Hammer attacks. 0x00:Disable(Default), 0x01:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.MinRefRate2xEnable          | 0x0059 | 0x01 | 0x00

  # !BSF NAME:{DualRankSupportEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Dual Rank Support Enable. 0x00:Disable, 0x01:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DualRankSupportEnable       | 0x005A | 0x01 | 0x01

  # !BSF NAME:{RmtMode} TYPE:{Combo}
  # !BSF OPTION:{0x0:Disabled, 0x3:Enabled}
  # !BSF HELP:{Rank Margin Tool Mode. 0x00(Default), 0x3(Enabled).}
  gBroxtonFspPkgTokenSpaceGuid.RmtMode                     | 0x005B | 0x01 | 0x00

  # !BSF NAME:{MemorySizeLimit} TYPE:{EditNum, HEX, (0x0000,0xFFFF)}
  # !BSF HELP:{Memory Size Limit: This value is used to restrict the total amount of memory and the calculations based on it. Value is in MB. Example encodings are: 0x400 = 1GB, 0x800 = 2GB, 0x1000 = 4GB, 0x2000 8GB. 0x0000(Default)}
  gBroxtonFspPkgTokenSpaceGuid.MemorySizeLimit             | 0x005C | 0x02 | 0x0000

  # !BSF NAME:{LowMemoryMaxValue} TYPE:{EditNum, HEX, (0x0000,0xFFFF)}
  # !BSF HELP:{Low Memory Max Value: This value is used to restrict the amount of memory below 4GB and the calculations based on it. Value is in MB.Example encodings are: 0x400 = 1GB, 0x800 = 2GB, 0x1000 = 4GB, 0x2000 8GB. 0x0000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.LowMemoryMaxValue           | 0x005E | 0x02 | 0x0000

  # !BSF NAME:{HighMemoryMaxValue} TYPE:{EditNum, HEX, (0x0000,0xFFFF)}
  # !BSF HELP:{High Memory Max Value: This value is used to restrict the amount of memory above 4GB and the calculations based on it. Value is in MB. Example encodings are: 0x0400:1GB, 0x0800:2GB, 0x1000:4GB, 0x2000:8GB. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HighMemoryMaxValue          | 0x0060 | 0x02 | 0x0000

  # !BSF NAME:{DisableFastBoot} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{00:Disabled; Use saved training data (if valid) after first boot(Default), 01:Enabled; Full re-train of memory on every boot.}
  gBroxtonFspPkgTokenSpaceGuid.DisableFastBoot             | 0x0062 | 0x01 | 0x00

  # !BSF NAME:{DIMM0SPDAddress} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DIMM0 SPD Address (NOTE: Only for DDR3L only. Please put 0 for MemoryDown. 0xA0(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DIMM0SPDAddress             | 0x0063 | 0x01 | 0xA0

  # !BSF NAME:{DIMM1SPDAddress} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DIMM1 SPD Address (NOTE: Only for DDR3L only. Please put 0 for MemoryDown. 0xA4(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DIMM1SPDAddress             | 0x0064 | 0x01 | 0xA4

  # !BSF NAME:{Ch0_RankEnable} TYPE:{EditNum, HEX, (0x00,0xF)}
  # !BSF HELP:{NOTE: Only for memory down. This is a bit mask which specifies what ranks are enabled. NOTE: Only for memory down (soldered down memory with no SPD): BIT[0] Enable Rank 0:  Must be set to 1 to enable use of this rank; BIT1[1] Enable Rank 1:  Must be set to 1 to enable use of this rank.}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_RankEnable              | 0x0065 | 0x01 | 0x00

  # !BSF NAME:{Ch0_DeviceWidth} TYPE:{Combo}
  # !BSF OPTION:{0b0000:x8, 0b0001:x16, 0b0010:x32, 0b0011:x64}
  # !BSF HELP:{NOTE: Only for memory down. Must specify the DRAM device width per DRAM channel (not to be confused with the SoC Memory Channel width which is always x32 for LPDDR3\LPDDR4 and x64 for DDR3L). LPDDR4 devices typically have two channels per die and a x16 device width: 00 - x8; 01 - x16; 10 - x32; 11 - x64}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_DeviceWidth             | 0x0066 | 0x01 | 0x00

  # !BSF NAME:{Ch0_DramDensity} TYPE:{Combo}
  # !BSF OPTION:{0b0000:4Gb, 0b0001:6Gb, 0b0010:8Gb, 0b0011:12Gb, 0b0100:16Gb}
  # !BSF HELP:{NOTE: Only for memory down. For LPDDR3 and LPDDR4: Must specify the DRAM device density per rank (per Chip Select). The simplest way of identifying the density per rank is to divide the total SoC memory channel density by the number of ranks. For DDR3L: Must specify the DRAM device density per DRAM device. For example, an 8GB 2Rx8 configuration will utilize sixteen 4Gb density DRAMS. In this configuration, a 4Gb density setting would be selected in the MRC: 000 - 4Gb; 001 - 6Gb; 010 - 8Gb; 011 - 12Gb; 100 - 16Gb; 101 - 2Gb; 110-111 - Reserved}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_DramDensity             | 0x0067 | 0x01 | 0x00

  # !BSF NAME:{Ch0_Option} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{BIT[0] Rank Select Interleaving Enable.  See Address Mapping section for full description: 0 - Rank Select Interleaving disabled; 1 - Rank Select Interleaving enabled. BIT[1] Bank Address Hashing Enable. See Address Mapping section for full description: 0 - Bank Address Hashing disabled; 1 - Bank Address Hashing enabled. BIT[2] CH1 CLK Disable. Disables the CH1 CLK PHY Signal when set to 1. This is used on board designs where the CH1 CLK is not routed and left floating or stubbed out: 0 - CH1 CLK is enabled; 1 -  CH1 CLK is disabled. BIT[3] Reserved; BIT[5:4] This register specifies the address mapping to be used: 00 - 1KB (A); 01 - 2KB (B)}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_Option                  | 0x0068 | 0x01 | 0x03

  # !BSF NAME:{Ch0_OdtConfig} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{[0] RX ODT - DDR3L & LPDDR3 only: Change the READ ODT strength , for SOC termination during a READ transaction, ON DQ BITs. STRONG ==> 60 OHMS roughly, WEAK ==> 120 OHMS or so roughly. Purpose: Save power on these technologies which burn power directly proportional to ODT strength, because ODT looks like a PU and PD (e.g. a resistor divider, which always burns power when ODT is ON).   0 - WEAK_ODT_CONFIG,   1 - STRONG_ODT_CONFIG. LPDDR4:   X - Don't Care. [1] CA ODT - LPDDR4 Only: The customer needs to choose this based on their actual board strapping (how they tie the DRAM's ODT PINs). Effect: LPDDR4 MR11 will be set based on this setting. CAODT_A_B_HIGH_LOW ==> MR11 = 0x34, which is CA ODT = 80 ohms. CAODT_A_B_HIGH_HIGH ==> MR11 = 0x24, which is CA ODT = 120 ohms (results in 60 ohm final   effective impedance on CA/CLK/CS signals). Purpose: To improve signal integrity and provide a much more optimized CA VREF value during training.  Not to save power.  0 - ODT_AB_HIGH_LOW (default),  1 - ODT_AB_HIGH_HIGH. DDR3L & LPDDR3:  X - Don't Care. [4] TX ODT. DDR3L only:   0 = RZQ/4 (60 Ohms)  = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_60_OHMS,   1 = RZQ/2 (120 Ohms) = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_120_OHMS. LPDDR3 & LPDDR4:   X = Don't Care}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_OdtConfig               | 0x0069 | 0x01 | 0x00

  # !BSF NAME:{Ch0_TristateClk1} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Not used}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_TristateClk1            | 0x006A | 0x01 | 0x00

  # !BSF NAME:{Ch0_Mode2N} TYPE:{Combo}
  # !BSF OPTION:{0x0:Auto, 0x1:Force 2N CMD Timing Mode}
  # !BSF HELP:{DDR3L Only: Configures the DDR3L command timing mode. 2N Mode is a stretched command mode that provides more setup and hold time for DRAM commands on the DRAM command bus. This is useful for platforms with unusual CMD bus routing or marginal signal integrity: 0 - Auto (1N or 2N mode is automatically selected during Command and Control training), 1 - Force 2N Mode}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_Mode2N                  | 0x006B | 0x01 | 0x00

  # !BSF NAME:{Ch0_OdtLevels} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Parameter used to determine if ODT will be held high or low: 0 - ODT Connected to SoC, 1 - ODT held high}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_OdtLevels               | 0x006C | 0x01 | 0x00

  # !BSF NAME:{Ch1_RankEnable} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{NOTE: Only for memory down. This is a bit mask which specifies what ranks are enabled. NOTE: Only for memory down (soldered down memory with no SPD): BIT[0] Enable Rank 0:  Must be set to 1 to enable use of this rank; BIT1[1] Enable Rank 1:  Must be set to 1 to enable use of this rank.}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_RankEnable              | 0x006D | 0x01 | 0x00

  # !BSF NAME:{Ch1_DeviceWidth} TYPE:{Combo}
  # !BSF OPTION:{0b0000:x8, 0b0001:x16, 0b0010:x32, 0b0011:x64}
  # !BSF HELP:{NOTE: Only for memory down. Must specify the DRAM device width per DRAM channel (not to be confused with the SoC Memory Channel width which is always x32 for LPDDR3\LPDDR4 and x64 for DDR3L). LPDDR4 devices typically have two channels per die and a x16 device width: 00 - x8; 01 - x16; 10 - x32; 11 - x64}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_DeviceWidth             | 0x006E | 0x01 | 0x00

  # !BSF NAME:{Ch1_DramDensity} TYPE:{Combo}
  # !BSF OPTION:{0b0000:4Gb, 0b0001:6Gb, 0b0010:8Gb, 0b0011:12Gb, 0b0100:16Gb}
  # !BSF HELP:{NOTE: Only for memory down. For LPDDR3 and LPDDR4: Must specify the DRAM device density per rank (per Chip Select). The simplest way of identifying the density per rank is to divide the total SoC memory channel density by the number of ranks. For DDR3L: Must specify the DRAM device density per DRAM device. For example, an 8GB 2Rx8 configuration will utilize sixteen 4Gb density DRAMS. In this configuration, a 4Gb density setting would be selected in the MRC: 000 - 4Gb; 001 - 6Gb; 010 - 8Gb; 011 - 12Gb; 100 - 16Gb; 101 - 2Gb; 110-111 - Reserved}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_DramDensity             | 0x006F | 0x01 | 0x00

  # !BSF NAME:{Ch1_Option} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{BIT[0] Rank Select Interleaving Enable.  See Address Mapping section for full description: 0 - Rank Select Interleaving disabled; 1 - Rank Select Interleaving enabled. BIT[1] Bank Address Hashing Enable. See Address Mapping section for full description: 0 - Bank Address Hashing disabled; 1 - Bank Address Hashing enabled. BIT[2] CH1 CLK Disable. Disables the CH1 CLK PHY Signal when set to 1. This is used on board designs where the CH1 CLK is not routed and left floating or stubbed out: 0 - CH1 CLK is enabled; 1 -  CH1 CLK is disabled. BIT[3] Reserved; BIT[5:4] This register specifies the address mapping to be used: 00 - 1KB (A); 01 - 2KB (B)}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_Option                  | 0x0070 | 0x01 | 0x03

  # !BSF NAME:{Ch1_OdtConfig} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{[0] RX ODT - DDR3L & LPDDR3 only: Change the READ ODT strength , for SOC termination during a READ transaction, ON DQ BITs. STRONG ==> 60 OHMS roughly, WEAK ==> 120 OHMS or so roughly. Purpose: Save power on these technologies which burn power directly proportional to ODT strength, because ODT looks like a PU and PD (e.g. a resistor divider, which always burns power when ODT is ON).   0 - WEAK_ODT_CONFIG,   1 - STRONG_ODT_CONFIG. LPDDR4:   X - Don't Care. [1] CA ODT - LPDDR4 Only: The customer needs to choose this based on their actual board strapping (how they tie the DRAM's ODT PINs). Effect: LPDDR4 MR11 will be set based on this setting. CAODT_A_B_HIGH_LOW ==> MR11 = 0x34, which is CA ODT = 80 ohms. CAODT_A_B_HIGH_HIGH ==> MR11 = 0x24, which is CA ODT = 120 ohms (results in 60 ohm final   effective impedance on CA/CLK/CS signals). Purpose: To improve signal integrity and provide a much more optimized CA VREF value during training.  Not to save power.  0 - ODT_AB_HIGH_LOW (default),  1 - ODT_AB_HIGH_HIGH. DDR3L & LPDDR3:  X - Don't Care. [4] TX ODT. DDR3L only:   0 = RZQ/4 (60 Ohms)  = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_60_OHMS,   1 = RZQ/2 (120 Ohms) = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_120_OHMS. LPDDR3 & LPDDR4:   X = Don't Care}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_OdtConfig               | 0x0071 | 0x01 | 0x00

  # !BSF NAME:{Ch1_TristateClk1} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Not used}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_TristateClk1            | 0x0072 | 0x01 | 0x00

  # !BSF NAME:{Ch1_Mode2N} TYPE:{Combo}
  # !BSF OPTION:{0x0:Auto, 0x1:Force 2N CMD Timing Mode}
  # !BSF HELP:{DDR3L Only: Configures the DDR3L command timing mode. 2N Mode is a stretched command mode that provides more setup and hold time for DRAM commands on the DRAM command bus. This is useful for platforms with unusual CMD bus routing or marginal signal integrity: 0 - Auto (1N or 2N mode is automatically selected during Command and Control training), 1 - Force 2N Mode}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_Mode2N                  | 0x0073 | 0x01 | 0x00

   # !BSF NAME:{Ch1_OdtLevels} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DDR3L Only: Parameter used to determine if ODT will be held high or low: 0 - ODT_AB_HIGH_LOW (default), 1 - ODT_AB_HIGH_HIGH}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_OdtLevels               | 0x0074 | 0x01 | 0x00

  # !BSF NAME:{Ch2_RankEnable} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{NOTE: Only for memory down. This is a bit mask which specifies what ranks are enabled. NOTE: Only for memory down (soldered down memory with no SPD): BIT[0] Enable Rank 0:  Must be set to 1 to enable use of this rank; BIT1[1] Enable Rank 1:  Must be set to 1 to enable use of this rank.}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_RankEnable              | 0x0075 | 0x01 | 0x00

  # !BSF NAME:{Ch2_DeviceWidth} TYPE:{Combo}
  # !BSF OPTION:{0b0000:x8, 0b0001:x16, 0b0010:x32, 0b0011:x64}
  # !BSF HELP:{NOTE: Only for memory down. Must specify the DRAM device width per DRAM channel (not to be confused with the SoC Memory Channel width which is always x32 for LPDDR3\LPDDR4 and x64 for DDR3L). LPDDR4 devices typically have two channels per die and a x16 device width: 00 - x8; 01 - x16; 10 - x32; 11 - x64}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_DeviceWidth             | 0x0076 | 0x01 | 0x00

  # !BSF NAME:{Ch2_DramDensity} TYPE:{Combo}
  # !BSF OPTION:{0b0000:4Gb, 0b0001:6Gb, 0b0010:8Gb, 0b0011:12Gb, 0b0100:16Gb}
  # !BSF HELP:{NOTE: Only for memory down. For LPDDR3 and LPDDR4: Must specify the DRAM device density per rank (per Chip Select). The simplest way of identifying the density per rank is to divide the total SoC memory channel density by the number of ranks. For DDR3L: Must specify the DRAM device density per DRAM device. For example, an 8GB 2Rx8 configuration will utilize sixteen 4Gb density DRAMS. In this configuration, a 4Gb density setting would be selected in the MRC: 000 - 4Gb; 001 - 6Gb; 010 - 8Gb; 011 - 12Gb; 100 - 16Gb; 101 - 2Gb; 110-111 - Reserved}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_DramDensity             | 0x0077 | 0x01 | 0x00

  # !BSF NAME:{Ch2_Option} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{BIT[0] Rank Select Interleaving Enable.  See Address Mapping section for full description: 0 - Rank Select Interleaving disabled; 1 - Rank Select Interleaving enabled. BIT[1] Bank Address Hashing Enable. See Address Mapping section for full description: 0 - Bank Address Hashing disabled; 1 - Bank Address Hashing enabled. BIT[2] CH1 CLK Disable. Disables the CH1 CLK PHY Signal when set to 1. This is used on board designs where the CH1 CLK is not routed and left floating or stubbed out: 0 - CH1 CLK is enabled; 1 -  CH1 CLK is disabled. BIT[3] Reserved; BIT[5:4] This register specifies the address mapping to be used: 00 - 1KB (A); 01 - 2KB (B)}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_Option                  | 0x0078 | 0x01 | 0x00

  # !BSF NAME:{Ch2_OdtConfig} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{[0] RX ODT - DDR3L & LPDDR3 only: Change the READ ODT strength , for SOC termination during a READ transaction, ON DQ BITs. STRONG ==> 60 OHMS roughly, WEAK ==> 120 OHMS or so roughly. Purpose: Save power on these technologies which burn power directly proportional to ODT strength, because ODT looks like a PU and PD (e.g. a resistor divider, which always burns power when ODT is ON).   0 - WEAK_ODT_CONFIG,   1 - STRONG_ODT_CONFIG. LPDDR4:   X - Don't Care. [1] CA ODT - LPDDR4 Only: The customer needs to choose this based on their actual board strapping (how they tie the DRAM's ODT PINs). Effect: LPDDR4 MR11 will be set based on this setting. CAODT_A_B_HIGH_LOW ==> MR11 = 0x34, which is CA ODT = 80 ohms. CAODT_A_B_HIGH_HIGH ==> MR11 = 0x24, which is CA ODT = 120 ohms (results in 60 ohm final   effective impedance on CA/CLK/CS signals). Purpose: To improve signal integrity and provide a much more optimized CA VREF value during training.  Not to save power.  0 - ODT_AB_HIGH_LOW (default),  1 - ODT_AB_HIGH_HIGH. DDR3L & LPDDR3:  X - Don't Care. [4] TX ODT. DDR3L only:   0 = RZQ/4 (60 Ohms)  = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_60_OHMS,   1 = RZQ/2 (120 Ohms) = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_120_OHMS. LPDDR3 & LPDDR4:   X = Don't Care}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_OdtConfig               | 0x0079 | 0x01 | 0x00

  # !BSF NAME:{Ch2_TristateClk1} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Not used}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_TristateClk1            | 0x007A | 0x01 | 0x00

  # !BSF NAME:{Ch2_Mode2N} TYPE:{Combo}
  # !BSF OPTION:{0x0:Auto, 0x1:Force 2N CMD Timing Mode}
  # !BSF HELP:{DDR3L Only: Configures the DDR3L command timing mode. 2N Mode is a stretched command mode that provides more setup and hold time for DRAM commands on the DRAM command bus. This is useful for platforms with unusual CMD bus routing or marginal signal integrity: 0 - Auto (1N or 2N mode is automatically selected during Command and Control training), 1 - Force 2N Mode}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_Mode2N                  | 0x007B | 0x01 | 0x00

  # !BSF NAME:{Ch2_OdtLevels} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{DDR3L Only: Parameter used to determine if ODT will be held high or low: 0 - ODT_AB_HIGH_LOW (default), 1 - ODT_AB_HIGH_HIGH}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_OdtLevels               | 0x007C | 0x01 | 0x00

  # !BSF NAME:{Ch3_RankEnable} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{NOTE: Only for memory down. This is a bit mask which specifies what ranks are enabled. NOTE: Only for memory down (soldered down memory with no SPD): BIT[0] Enable Rank 0:  Must be set to 1 to enable use of this rank; BIT1[1] Enable Rank 1:  Must be set to 1 to enable use of this rank.}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_RankEnable              | 0x007D | 0x01 | 0x00

  # !BSF NAME:{Ch3_DeviceWidth} TYPE:{Combo}
  # !BSF OPTION:{0b0000:x8, 0b0001:x16, 0b0010:x32, 0b0011:x64}
  # !BSF HELP:{NOTE: Only for memory down. Must specify the DRAM device width per DRAM channel (not to be confused with the SoC Memory Channel width which is always x32 for LPDDR3\LPDDR4 and x64 for DDR3L). LPDDR4 devices typically have two channels per die and a x16 device width: 00 - x8; 01 - x16; 10 - x32; 11 - x64}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_DeviceWidth             | 0x007E | 0x01 | 0x00

  # !BSF NAME:{Ch3_DramDensity} TYPE:{Combo}
  # !BSF OPTION:{0b0000:4Gb, 0b0001:6Gb, 0b0010:8Gb, 0b0011:12Gb, 0b0100:16Gb}
  # !BSF HELP:{NOTE: Only for memory down. For LPDDR3 and LPDDR4: Must specify the DRAM device density per rank (per Chip Select). The simplest way of identifying the density per rank is to divide the total SoC memory channel density by the number of ranks. For DDR3L: Must specify the DRAM device density per DRAM device. For example, an 8GB 2Rx8 configuration will utilize sixteen 4Gb density DRAMS. In this configuration, a 4Gb density setting would be selected in the MRC: 000 - 4Gb; 001 - 6Gb; 010 - 8Gb; 011 - 12Gb; 100 - 16Gb; 101 - 2Gb; 110-111 - Reserved}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_DramDensity             | 0x007F | 0x01 | 0x00

  # !BSF NAME:{Ch3_Option} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{BIT[0] Rank Select Interleaving Enable.  See Address Mapping section for full description: 0 - Rank Select Interleaving disabled; 1 - Rank Select Interleaving enabled. BIT[1] Bank Address Hashing Enable. See Address Mapping section for full description: 0 - Bank Address Hashing disabled; 1 - Bank Address Hashing enabled. BIT[2] CH1 CLK Disable. Disables the CH1 CLK PHY Signal when set to 1. This is used on board designs where the CH1 CLK is not routed and left floating or stubbed out: 0 - CH1 CLK is enabled; 1 -  CH1 CLK is disabled. BIT[3] Reserved; BIT[5:4] This register specifies the address mapping to be used: 00 - 1KB (A); 01 - 2KB (B)}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_Option                  | 0x0080 | 0x01 | 0x00

  # !BSF NAME:{Ch3_OdtConfig} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{[0] RX ODT - DDR3L & LPDDR3 only: Change the READ ODT strength , for SOC termination during a READ transaction, ON DQ BITs. STRONG ==> 60 OHMS roughly, WEAK ==> 120 OHMS or so roughly. Purpose: Save power on these technologies which burn power directly proportional to ODT strength, because ODT looks like a PU and PD (e.g. a resistor divider, which always burns power when ODT is ON).   0 - WEAK_ODT_CONFIG,   1 - STRONG_ODT_CONFIG. LPDDR4:   X - Don't Care. [1] CA ODT - LPDDR4 Only: The customer needs to choose this based on their actual board strapping (how they tie the DRAM's ODT PINs). Effect: LPDDR4 MR11 will be set based on this setting. CAODT_A_B_HIGH_LOW ==> MR11 = 0x34, which is CA ODT = 80 ohms. CAODT_A_B_HIGH_HIGH ==> MR11 = 0x24, which is CA ODT = 120 ohms (results in 60 ohm final   effective impedance on CA/CLK/CS signals). Purpose: To improve signal integrity and provide a much more optimized CA VREF value during training.  Not to save power.  0 - ODT_AB_HIGH_LOW (default),  1 - ODT_AB_HIGH_HIGH. DDR3L & LPDDR3:  X - Don't Care. [4] TX ODT. DDR3L only:   0 = RZQ/4 (60 Ohms)  = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_60_OHMS,   1 = RZQ/2 (120 Ohms) = MRC_SMIP_DDR3L_TX_ODT_RTT_WR_120_OHMS. LPDDR3 & LPDDR4:   X = Don't Care}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_OdtConfig               | 0x0081 | 0x01 | 0x00

  # !BSF NAME:{Ch3_TristateClk1} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Not used}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_TristateClk1            | 0x0082 | 0x01 | 0x00

  # !BSF NAME:{Ch3_Mode2N} TYPE:{Combo}
  # !BSF OPTION:{0x0:Auto, 0x1:Force 2N CMD Timing Mode}
  # !BSF HELP:{DDR3L Only: Configures the DDR3L command timing mode. 2N Mode is a stretched command mode that provides more setup and hold time for DRAM commands on the DRAM command bus. This is useful for platforms with unusual CMD bus routing or marginal signal integrity: 0 - Auto (1N or 2N mode is automatically selected during Command and Control training), 1 - Force 2N Mode}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_Mode2N                  | 0x0083 | 0x01 | 0x00

  # !BSF NAME:{Ch3_OdtLevels} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{DDR3L Only: Parameter used to determine if ODT will be held high or low: 0 - ODT_AB_HIGH_LOW (default), 1 - ODT_AB_HIGH_HIGH}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_OdtLevels               | 0x0084 | 0x01 | 0x00

  # !BSF NAME:{RmtCheckRun} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Parameter used to determine whether to run the margin check. Bit 0 is used for MINIMUM MARGIN CHECK and bit 1 is used for DEGRADE MARGIN CHECK}
  gBroxtonFspPkgTokenSpaceGuid.RmtCheckRun                 | 0x0085 | 0x01 | 0x00

  # !BSF NAME:{RmtMarginCheckScaleHighThreshold} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Percentage used to determine the margin tolerances over the failing margin.}
  gBroxtonFspPkgTokenSpaceGuid.RmtMarginCheckScaleHighThreshold | 0x0086 | 0x02 | 0x0000

  # !BSF NAME:{Ch0_Bit_swizzling} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Channel 0 PHY to DUnit DQ mapping (only used if not 1-1 mapping)Range: 0-32. Frequently asked questions: Q: The DQS (strobes) need to go with the corresponding byte lanes on the DDR module. Are the DQS being swapped around as well? Ans: Yes, DQ strobes need to follow the DQ byte lane they correspond too. So for example if you have DQ[7:0] swapped with DQ[15:8], DQS0 pair also need to be swapped with DQS1 pair. Also, the spreadsheet used for Amenia is essentially a swizzle value lookup that specifies what DRAM DQ bit a particular SoC DQ bit is connected to. Some confusion can arrise from the fact that the indexes to the array do not necessarily map 1:1 to an SoC DQ pin. For example, the CH0 array at index 0 maps to SoC DQB8. The value of 9 at index 0 tells us that SoC DQB8 is connected to DRAM DQA9. Q: The PDG indicates a 2 physical channels need to be stuffed and operated together. Are the CHx_A and CHx_B physical channels operated in tandem or completely separate? If separate, why requirement of pairing them? Ans: We have 2 PHY instances on the SoC each supporting up to 2 x32 LP4 channels. If you have 4 channels both PHYs are active, but if you have 2 channels in order to power gate one PHY, those two channel populated must be on one PHY instance. So yes all channels are independent of each other, but there are some restrictions on how they need to be populated. Q: How is it that an LPDDR4 device is identified as having a x16 width when all 32-bits are used at the same time with a single chip select? That's effectively a x32 device. Ans:LPDDR4 DRAM devices are x16. Each die has 2 x16 devices on them. To make a x32 channel the CS of the two devices in the same die are connected together to make a single rank of one x32 channel (SDP). The second die in the DDP package makes the second rank.}
  gBroxtonFspPkgTokenSpaceGuid.Ch0_Bit_swizzling           | 0x0088 | 0x20 | {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F}

  # !BSF NAME:{Ch1_Bit_swizzling} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Channel 1 PHY to DUnit DQ mapping (only used if not 1-1 mapping)Range: 0-32.}
  gBroxtonFspPkgTokenSpaceGuid.Ch1_Bit_swizzling           | 0x00A8 | 0x20 | {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F}

  # !BSF NAME:{Ch2_Bit_swizzling} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Channel 2 PHY to DUnit DQ mapping (only used if not 1-1 mapping)Range: 0-32.}
  gBroxtonFspPkgTokenSpaceGuid.Ch2_Bit_swizzling           | 0x00C8 | 0x20 | {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F}

  # !BSF NAME:{Ch3_Bit_swizzling} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Channel 3 PHY to DUnit DQ mapping (only used if not 1-1 mapping)Range: 0-32.}
  gBroxtonFspPkgTokenSpaceGuid.Ch3_Bit_swizzling           | 0x00E8 | 0x20 | {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F}

  # !BSF NAME:{MsgLevelMask} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{32 bits used to mask out debug messages. Masking out bit 0 mask all other messages.}
  gBroxtonFspPkgTokenSpaceGuid.MsgLevelMask                | 0x0108 | 0x04 | 0x00000000

  # !BSF NAME:{PreMem GPIO Pin Number for each table} TYPE:{EditNum, HEX, (0x0, 0xFFFFFFFF)}
  # !BSF HELP:{Number of Pins in each PreMem GPIO Table. 0(Default).}
  gPlatformFspPkgTokenSpaceGuid.PreMemGpioTablePinNum      | 0x0110 | 0x04 | {0x00}

  # !BSF NAME:{PreMem GPIO Table Pointer} TYPE:{EditNum, HEX, (0x0, 0xFFFFFFFF)}
  # !BSF HELP:{Pointer to Array of pointers to PreMem GPIO Table. 0x00000000(Default).}
  gPlatformFspPkgTokenSpaceGuid.PreMemGpioTablePtr         | 0x0114 | 0x04 | 0x00000000

  # !BSF NAME:{PreMem GPIO Table Entry Number. Currently maximum entry number is 4} TYPE:{EditNum, HEX, (0x0, 0x04)}
  # !BSF HELP:{Number of Entries in PreMem GPIO Table. 0(Default).}
  gPlatformFspPkgTokenSpaceGuid.PreMemGpioTableEntryNum    | 0x0118 | 0x01 | 0x00

  #
  # LPC Configurations
  #

  # !BSF NAME:{Enhance the port 8xh decoding} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Enhance the port 8xh decoding. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EnhancePort8xhDecoding      | 0x0119 | 0x01 | 0x01

  # !BSF NAME:{SPD Data Write}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SPD data write on the SMBUS. 0x00:Disable(Default), 0x01:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SpdWriteEnable              | 0x011A | 0x01 | 0x00

  # !BSF NAME:{MRC Training Data Saving}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable MRC training data saving in FSP. 0x00:Disable(Default), 0x01:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.MrcDataSaving               | 0x011B | 0x01 | 0x00

  # !BSF NAME:{OEM File Loading Address}
  # !BSF TYPE:{EditNum, HEX, (0x00000000,0xFFFFFFFF)}
  # !BSF HELP:{Determine the memory base address to load a specified file from CSE file system after memory is available.}
  gBroxtonFspPkgTokenSpaceGuid.OemLoadingBase              | 0x011C | 0x04 | 0x7AD00000

  # !BSF NAME:{OEM File Name to Load}
  # !BSF TYPE:{EditText}
  # !BSF HELP:{Specify a file name to load from CSE file system after memory is available. Empty indicates no file needs to be loaded.}
  gBroxtonFspPkgTokenSpaceGuid.OemFileName                 | 0x0120 | 0x10 | ""

  # !HDR STRUCT:{VOID*}
  gBroxtonFspPkgTokenSpaceGuid.MrcBootDataPtr              | 0x0130 | 0x04 | 0x00000000

  # !BSF NAME:{eMMC Trace Length}  TYPE:{Combo}
  # !BSF OPTION:{0x0:Long, 0x1:Short}
  # !BSF HELP:{Select eMMC trace length to load OEM file from when loading OEM file name is specified. 0x0:Long(Default), 0x1:Short.}
  gBroxtonFspPkgTokenSpaceGuid.eMMCTraceLen                | 0x0134 | 0x01 | 0x00

  # !BSF NAME:{Skip CSE RBP to support zero sized IBB}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable skip CSE RBP for bootloader which loads IBB without assistance of CSE. 0x00:Disable(Default), 0x01:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SkipCseRbp                  | 0x0135 | 0x01 | 0x00

  # !BSF NAME:{Npk Enable}  TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Debugger, 3:Auto}
  # !BSF HELP:{Enable/Disable Npk. 0:Disable, 1:Enable, 2:Debugger, 3:Auto(Default).}
  gBroxtonFspPkgTokenSpaceGuid.NpkEn                       | 0x0136 | 0x01 | 0x03

  # !BSF NAME:{FW Trace Enable}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable FW Trace. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.FwTraceEn                   | 0x0137 | 0x01 | 0x01

  # !BSF NAME:{FW Trace Destination}  TYPE:{EditNum, HEX, (0x1, 0x04)}
  # !BSF HELP:{FW Trace Destination. 1-NPK_TRACE_TO_MEMORY, 2-NPK_TRACE_TO_DCI, 3-NPK_TRACE_TO_BSSB, 4-NPK_TRACE_TO_PTI(Default).}
  gBroxtonFspPkgTokenSpaceGuid.FwTraceDestination          | 0x0138 | 0x01 | 0x04

  # !BSF NAME:{NPK Recovery Dump}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable NPK Recovery Dump. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.RecoverDump                 | 0x0139 | 0x01 | 0x00

  # !BSF NAME:{Memory Region 0 Buffer WrapAround}  TYPE:{EditNum, HEX, (0x0, 0x01)}
  # !BSF HELP:{Memory Region 0 Buffer WrapAround. 0-n0-warp, 1-warp(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Msc0Wrap                    | 0x013A | 0x01 | 0x01

  # !BSF NAME:{Memory Region 1 Buffer WrapAround}  TYPE:{EditNum, HEX, (0x0, 0x01)}
  # !BSF HELP:{Memory Region 1 Buffer WrapAround. 0-n0-warp, 1-warp(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Msc1Wrap                    | 0x013B | 0x01 | 0x01

  # !BSF NAME:{Memory Region 0 Buffer Size}  TYPE:{EditNum, HEX, (0x0, 0x07)}
  # !BSF HELP:{Memory Region 0 Buffer Size. 0-0MB(Default), 1-1MB, 2-8MB, 3-64MB, 4-128MB, 5-256MB, 6-512MB, 7-1GB.}
  gBroxtonFspPkgTokenSpaceGuid.Msc0Size                    | 0x013C | 0x04 | 0x00

  # !BSF NAME:{Memory Region 1 Buffer Size}  TYPE:{EditNum, HEX, (0x0, 0x07)}
  # !BSF HELP:{Memory Region 1 Buffer Size, 0-0MB(Default), 1-1MB, 2-8MB, 3-64MB, 4-128MB, 5-256MB, 6-512MB, 7-1GB.}
  gBroxtonFspPkgTokenSpaceGuid.Msc1Size                    | 0x0140 | 0x04 | 0x00

  # !BSF NAME:{PTI Mode}  TYPE:{EditNum, HEX, (0x0, 0x04)}
  # !BSF HELP:{PTI Mode. 0-0ff, 1-x4(Default), 2-x8, 3-x12, 4-x16.}
  gBroxtonFspPkgTokenSpaceGuid.PtiMode                     | 0x0144 | 0x01 | 0x01

  # !BSF NAME:{PTI Training}  TYPE:{EditNum, HEX, (0x0, 0x06)}
  # !BSF HELP:{PTI Training. 0-off(Default), 1-6=1-6.}
  gBroxtonFspPkgTokenSpaceGuid.PtiTraining                 | 0x0145 | 0x01 | 0x00

  # !BSF NAME:{PTI Speed}  TYPE:{EditNum, HEX, (0x0, 0x02)}
  # !BSF HELP:{PTI Speed. 0-full, 1-half, 2-quarter(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PtiSpeed                    | 0x0146 | 0x01 | 0x02

  # !BSF NAME:{Punit Message Level}  TYPE:{EditNum, HEX, (0x0, 0x04)}
  # !BSF HELP:{Punit Message Output Verbosity Level. 0, 1(Default), 2-4=2-4.}
  gBroxtonFspPkgTokenSpaceGuid.PunitMlvl                   | 0x0147 | 0x01 | 0x01

  # !BSF NAME:{PMC Message Level}  TYPE:{EditNum, HEX, (0x0, 0x04)}
  # !BSF HELP:{PMC Message Output Verbosity Level. 0, 1(Default), 2-4=2-4.}
  gBroxtonFspPkgTokenSpaceGuid.PmcMlvl                     | 0x0148 | 0x01 | 0x01

  # !BSF NAME:{SW Trace Enable}  TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SW Trace. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SwTraceEn                   | 0x0149 | 0x01 | 0x00

  # !BSF NAME:{Periodic Retraining Disable}  TYPE:{Combo}
  # !BSF OPTION:{0x0:Enabled, 0x1:Disabled}
  # !BSF HELP:{Periodic Retraining Disable - This option allows customers to disable LPDDR4 Periodic Retraining for debug purposes. Periodic Retraining should be enabled in production. Periodic retraining allows the platform to operate reliably over a larger voltage and temperature range. This field has no effect for DDR3L and LPDDR3 memory type configurations. 0x00: Enable Periodic Retraining (default); 0x01: Disable Periodic Retraining (debug configuration only)}
  gBroxtonFspPkgTokenSpaceGuid.PeriodicRetrainingDisable   | 0x014A | 0x01 | 0x00

  # !BSF NAME:{Enable Reset System}  TYPE:{Combo}
  # !BSF OPTION:{0x0:Disabled, 0x1:Eabled}
  # !BSF HELP:{ Enable FSP to trigger reset instead of returning reset request. 0x00: Return the Return Status from FSP if a reset is required. (default); 0x01: Perform Reset inside FSP instead of returning from the API.}
  gBroxtonFspPkgTokenSpaceGuid.EnableResetSystem           | 0x014B | 0x01 | 0x00

  # !BSF NAME:{Enable HECI2 in S3 resume path}  TYPE:{Combo}
  # !BSF OPTION:{0x0:Disabled, 0x1:Enabled}
  # !BSF HELP:{ Enable HECI2 in S3 resume path. 0x00: Skip HECI2 initialization in S3 resume. ; 0x01: Enable HECI2 in S3 resume path.(Default)}
  gBroxtonFspPkgTokenSpaceGuid.EnableS3Heci2               | 0x014C | 0x01 | 0x01

  # !HDR STRUCT:{VOID*}
  gBroxtonFspPkgTokenSpaceGuid.VariableNvsBufferPtr        | 0x0150 | 0x04 | 0x00000000

  # !BSF NAME:{PCIE SLOT Power Enable Assert Time - PFET.} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{ACPI Timer Ticker to measure when PCIE Slot Power is enabled through PFET. FSP will wait for 100ms for the power to be stable, before de-asserting PERST bin. Customer who designed the board PCIE slot Power automatically enabled, can pass value of zero here.}
  gBroxtonFspPkgTokenSpaceGuid.StartTimerTickerOfPfetAssert | 0x0154 | 0x08 | 0x00000000

  # !BSF NAME:{Real Time Enabling} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Real-Time Feature Configuration Bits settings. 0x0:Disabled (default), 0x1:Enabled}
  gBroxtonFspPkgTokenSpaceGuid.RtEn                        | 0x015C | 0x01 | 0x00

  # !BSF NAME:{Skip Pcie Power Sequence}
  # !BSF OPTION:{0x0:Disabled, 0x1:Skip}
  # !BSF HELP:{UPD To Skip PciePowerSequence in FSP if set this UPD is set to 1. 0x0:Disabled (default), 0x1:Skip}
  gBroxtonFspPkgTokenSpaceGuid.SkipPciePowerSequence       | 0x015D | 0x01 | 0x00

  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:END}
  gBroxtonFspPkgTokenSpaceGuid.ReservedFspmUpd             | 0x015E | 0x02 | {0x00}



  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.
  gBroxtonFspPkgTokenSpaceGuid.UpdTerminator               | 0x01FE | 0x02 | 0x55AA

  ################################################################################
  #
  # UPDs consumed in FspSiliconInit Api
  #
  ################################################################################
  # !BSF FIND:{APLUPD_S}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspsUpdSignature: {APLUPD_S}
  gBroxtonFspPkgTokenSpaceGuid.Signature                   | 0x0000 | 0x08 | 0x535F4450554C5041
  # !BSF NAME:{FspsUpdRevision}  TYPE:{None}
  gBroxtonFspPkgTokenSpaceGuid.Revision                    | 0x0008 | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gBroxtonFspPkgTokenSpaceGuid.Reserved                    | 0x0009 | 0x17 | {0x00}

  # !HDR COMMENT:{FSP_S_CONFIG:Fsp S Configuration}
  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:START}
  # !BSF PAGE:{SIL}

  # !BSF NAME:{ActiveProcessorCores} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Number of active cores. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.ActiveProcessorCores        | 0x0020 | 0x01 | 0x00

  # !BSF NAME:{Disable Core1} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable Core1. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DisableCore1                | 0x0021 | 0x01 | 0x01

  # !BSF NAME:{Disable Core2} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable Core2. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DisableCore2                | 0x0022 | 0x01 | 0x01

  # !BSF NAME:{Disable Core3} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable Core3. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DisableCore3                | 0x0023 | 0x01 | 0x01

  # !BSF NAME:{VMX Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable VMX. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.VmxEnable                   | 0x0024 | 0x01 | 0x01

  # !BSF NAME:{Memory region allocation for Processor Trace} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Memory region allocation for Processor Trace, allowed range is from 4K (0x0) to 128MB (0xF); <b>0xFF: Disable. 0xFF:Disable(Default)}
  gBroxtonFspPkgTokenSpaceGuid.ProcTraceMemSize            | 0x0025 | 0x01 | 0xFF

  # !BSF NAME:{Enable Processor Trace} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Processor Trace feature.  0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.ProcTraceEnable             | 0x0026 | 0x01 | 0x00

  # !BSF NAME:{Eist} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Intel SpeedStep Technology. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Eist                        | 0x0027 | 0x01 | 0x01

  # !BSF NAME:{Boot PState} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Boot PState with HFM or LFM. 0:HFM(Default), 1:LFM.}
  gBroxtonFspPkgTokenSpaceGuid.BootPState                  | 0x0028 | 0x01 | 0x00

  # !BSF NAME:{CPU power states (C-states)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable CPU power states (C-states). 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EnableCx                    | 0x0029 | 0x01 | 0x01

  # !BSF NAME:{Enhanced C-states} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Enhanced C-states. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.C1e                         | 0x002A | 0x01 | 0x00

  # !BSF NAME:{Bi-Directional PROCHOT#} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Bi-Directional PROCHOT#. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.BiProcHot                   | 0x002B | 0x01 | 0x01

  # !BSF NAME:{Max Pkg Cstate} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Max Pkg Cstate. 0:PkgC0C1, 1:PkgC2, 2:PkgC3(Default), 3:PkgC6, 4:PkgC7, 5:PkgC7s, 6:PkgC8, 7:PkgC9, 8:PkgC10, 9:PkgCMax, 254:PkgCpuDefault, 255:PkgAuto.}
  gBroxtonFspPkgTokenSpaceGuid.PkgCStateLimit              | 0x002C | 0x01 | 0x02

  # !BSF NAME:{C-State auto-demotion} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{C-State Auto Demotion. 0:Disable(Default) C1 and C3 Auto-demotion, 1:Enable C3/C6/C7 Auto-demotion to C1, 2:Enable C6/C7 Auto-demotion to C3, 3:Enable C6/C7 Auto-demotion to C1 and C3.}
  gBroxtonFspPkgTokenSpaceGuid.CStateAutoDemotion          | 0x002D | 0x01 | 0x00

  # !BSF NAME:{C-State un-demotion} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{C-State un-demotion. 0:Disable(Default) C1 and C3 Un-demotion, 1:Enable C1 Un-demotion, 2:Enable C3 Un-demotion, 3:Enable C1 and C3 Un-demotion.}
  gBroxtonFspPkgTokenSpaceGuid.CStateUnDemotion            | 0x002E | 0x01 | 0x00

  # !BSF NAME:{Max Core C-State} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Max Core C-State. 0:Unlimited, 1:C1, 2:C3, 3:C6, 4:C7, 5:C8, 6:C9, 7:C10, 8:CCx(Default).}
  gBroxtonFspPkgTokenSpaceGuid.MaxCoreCState               | 0x002F | 0x01 | 0x08

  # !BSF NAME:{Package C-State Demotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Package Cstate Demotion. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PkgCStateDemotion           | 0x0030 | 0x01 | 0x00

  # !BSF NAME:{Package C-State Un-demotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Package Cstate UnDemotion. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PkgCStateUnDemotion         | 0x0031 | 0x01 | 0x00

  # !BSF NAME:{Turbo Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable long duration Turbo Mode. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.TurboMode                   | 0x0032 | 0x01 | 0x01

  # !BSF NAME:{SC HDA Verb Table Entry Number} TYPE:{EditNum, HEX, (0x0, 0xFF)}
  # !BSF HELP:{Number of Entries in Verb Table. 0(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HdaVerbTableEntryNum        | 0x0033 | 0x01 | 0x00

  # !BSF NAME:{SC HDA Verb Table Pointer} TYPE:{EditNum, HEX, (0x0, 0xFFFFFFFF)}
  # !BSF HELP:{Pointer to Array of pointers to Verb Table. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HdaVerbTablePtr             | 0x0034 | 0x04 | 0x00000000

  # !BSF NAME:{Enable/Disable P2SB device hidden.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable P2SB device hidden. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.P2sbUnhide                  | 0x0038 | 0x01 | 0x00

  # !BSF NAME:{IPU Enable/Disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable IPU Device. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IpuEn                       | 0x0039 | 0x01 | 0x01

  # !BSF NAME:{IMGU ACPI mode selection} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:IGFX Child device, 2:ACPI device}
  # !BSF HELP:{0:Auto, 1:IGFX Child device(Default), 2:ACPI device.}
  gBroxtonFspPkgTokenSpaceGuid.IpuAcpiMode                 | 0x003A | 0x1 | 0x01

  # !BSF NAME:{Enable ForceWake} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable ForceWake Models. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.ForceWake                   | 0x003B | 0x01 | 0x00

  # !BSF TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF NAME:{GttMmAdr}
  # !BSF HELP:{GttMmAdr structure for initialization. 0xBF000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.GttMmAdr                    | 0x003C | 0x04 | 0xBF000000

  # !BSF TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF NAME:{GmAdr}
  # !BSF HELP:{GmAdr structure for initialization. 0xA0000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.GmAdr                       | 0x0040 | 0x04 | 0xA0000000

  # !BSF NAME:{Enable PavpLock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PavpLock. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PavpLock                    | 0x0044 | 0x01 | 0x00

  # !BSF NAME:{Enable GraphicsFreqModify} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable GraphicsFreqModify. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.GraphicsFreqModify          | 0x0045 | 0x01 | 0x00

  # !BSF NAME:{Enable GraphicsFreqReq} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable GraphicsFreqReq. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.GraphicsFreqReq             | 0x0046 | 0x01 | 0x00

  # !BSF NAME:{Enable GraphicsVideoFreq} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable GraphicsVideoFreq. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.GraphicsVideoFreq           | 0x0047 | 0x01 | 0x00

  # !BSF NAME:{Enable PmLock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PmLock. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PmLock                      | 0x0048 | 0x01 | 0x00

  # !BSF NAME:{Enable DopClockGating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DopClockGating. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DopClockGating              | 0x0049 | 0x01 | 0x00

  # !BSF NAME:{Enable UnsolicitedAttackOverride} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable UnsolicitedAttackOverride. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.UnsolicitedAttackOverride   | 0x004A | 0x01 | 0x00

  # !BSF NAME:{Enable WOPCMSupport} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable WOPCMSupport. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.WOPCMSupport                | 0x004B | 0x01 | 0x00

  # !BSF NAME:{Enable WOPCMSize} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable WOPCMSize. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.WOPCMSize                   | 0x004C | 0x01 | 0x00

  # !BSF NAME:{Enable PowerGating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PowerGating. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PowerGating                 | 0x004D | 0x01 | 0x00

  # !BSF NAME:{Enable UnitLevelClockGating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable UnitLevelClockGating. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.UnitLevelClockGating        | 0x004E | 0x01 | 0x00

  # !BSF NAME:{Enable FastBoot} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable FastBoot. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.FastBoot                    | 0x004F | 0x01 | 0x00

  # !BSF NAME:{Enable DynSR} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DynSR. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DynSR                       | 0x0050 | 0x01 | 0x00

  # !BSF NAME:{Enable SaIpuEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable SaIpuEnable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SaIpuEnable                 | 0x0051 | 0x01 | 0x00

  # !BSF NAME:{GT PM Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable GT power management support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PmSupport                   | 0x0052 | 0x01 | 0x01

  # !BSF NAME:{RC6(Render Standby)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable render standby support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EnableRenderStandby         | 0x0053 | 0x01 | 0x01

  # !BSF NAME:{BMP Logo Data Size}
  # !BSF TYPE:{Reserved}
  # !BSF HELP:{BMP logo data buffer size. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.LogoSize                    | 0x0054 | 0x04 | 0x00000000

  # !BSF NAME:{BMP Logo Data Pointer}
  # !BSF TYPE:{Reserved}
  # !BSF HELP:{BMP logo data pointer to a BMP format buffer. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.LogoPtr                     | 0x0058 | 0x04 | 0x00000000

  # !BSF NAME:{Graphics Configuration Data Pointer}
  # !BSF TYPE:{Reserved}
  # !BSF HELP:{Graphics configuration data used for initialization. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.GraphicsConfigPtr           | 0x005C | 0x04 | 0x00000000

  # !BSF NAME:{PAVP Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Protected Audio Visual Path (PAVP). 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PavpEnable                  | 0x0060 | 0x01 | 0x01

  # !BSF NAME:{PAVP PR3} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PAVP PR3 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PavpPr3                     | 0x0061 | 0x01 | 0x01

  # !BSF NAME:{CdClock Frequency selection} TYPE:{Combo}
  # !BSF OPTION:{0: 144 MHz, 1: 288 MHz, 2: 384 MHz, 3: 576 MHz, 4: 624 MHz}
  # !BSF HELP:{0:144MHz, 1:288MHz, 2:384MHz, 3:576MHz, 4:624MHz(Default).}
  gBroxtonFspPkgTokenSpaceGuid.CdClock                     | 0x0062 | 0x01 | 0x04

  # !BSF NAME:{Enable/Disable PeiGraphicsPeimInit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PeiGraphicsPeimInit 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PeiGraphicsPeimInit         | 0x0063 | 0x01 | 0x01

  #
  # SouthCluster (PCH) Configurations
  #

  #
  # Flash Protection Configurations
  #

  # !BSF NAME:{Write Protection Support} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable Write Protection. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.WriteProtectionEnable                            | 0x0064 | 0x05 | { 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Read Protection Support} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable Read Protection. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ReadProtectionEnable                             | 0x0069 | 0x05 | { 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Protected Range Limitation} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{The address of the upper limit of protection, 0x0FFFh(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ProtectedRangeLimit                              | 0x006E | 0x0A | { 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF }

  # !BSF NAME:{Protected Range Base} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{The base address of the upper limit of protection. 0x0000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ProtectedRangeBase                               | 0x0078 | 0x0A | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  #
  # GMM Configurations
  #

  # !BSF NAME:{Enable SC Gaussian Mixture Models} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable SC Gaussian Mixture Models. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Gmm                                              | 0x0082 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating - PGCB Clock Trunk} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PGCB Clock Trunk. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingPgcbClkTrunk                            | 0x0083 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Sideband} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Sideband. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingSb                                      | 0x0084 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Sideband} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Sideband. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingSbClkTrunk                              | 0x0085 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Sideband Clock Partition} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Sideband Clock Partition. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingSbClkPartition                          | 0x0086 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Core} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Core. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingCore                                    | 0x0087 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - DMA} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DMA. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingDma                                     | 0x0088 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Register Access} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Register Access. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingRegAccess                               | 0x0089 | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Host} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Host. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingHost                                    | 0x008A | 0x01 | 0x01

  # !BSF NAME:{GMM Clock Gating  - Partition} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Partition. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingPartition                               | 0x008B | 0x01 | 0x01

  # !BSF NAME:{Clock Gating  - Trunk} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Trunk. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ClkGatingTrunk                                   | 0x008C | 0x01 | 0x01

  #
  # High Definition Audio (HDA) Configurations
  #

  # !BSF NAME:{HD Audio Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable HDA Audio Feature. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HdaEnable                                        | 0x008D | 0x01 | 0x01

  # !BSF NAME:{HD Audio DSP Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable HDA Audio DSP Feature. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DspEnable                                        | 0x008E | 0x01 | 0x01

  # !BSF NAME:{Azalia wake-on-ring} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Azalia wake-on-ring. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.Pme                                              | 0x008F | 0x01 | 0x00

  # !BSF NAME:{HD-Audio I/O Buffer Ownership} TYPE:{Combo}
  # !BSF OPTION:{0:HD-Audio link owns all the I/O buffers, 1:HD-Audio link owns 4 I/O buffers and I2S port owns 4 I/O buffers, 3:I2S port owns all the I/O buffers}
  # !BSF HELP:{Set HD-Audio I/O Buffer Ownership. 0:HD-Audio link owns all the I/O buffers(Default)}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioIoBufferOwnership                         | 0x0090 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio I/O Buffer Voltage} TYPE:{Combo} OPTION:{0: 3.3V, 1: 1.8V}
  # !BSF HELP:{HD-Audio I/O Buffer Voltage Mode Selectiton . 0:3.3V(Default), 1:1.8V.}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioIoBufferVoltage                           | 0x0091 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Virtual Channel Type} TYPE:{Combo} OPTION:{0: VC0, 1: VC1}
  # !BSF HELP:{HD-Audio Virtual Channel Type Selectiton. 0:VC0(Default), 1:VC1.}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioVcType                                    | 0x0092 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Link Frequency} TYPE:{Combo} OPTION:{0: 6MHz, 1: 12MHz, 2: 24MHz, 3: 48MHz, 4: 96MHz, 5: Invalid}
  # !BSF HELP:{HD-Audio Virtual Channel Type Selectiton. 0:6MHz(Default), 1:12MHz, 2:24MHz, 3:48MHz, 4:96MHz, 5:Invalid.}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioLinkFrequency                             | 0x0093 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio iDisp-Link Frequency} TYPE:{Combo} OPTION:{0: 6MHz, 1: 12MHz, 2: 24MHz, 3: 48MHz, 4: 96MHz, 5: Invalid}
  # !BSF HELP:{HD-Audio iDisp-Link Frequency Selectiton. 0:6MHz(Default), 1:12MHz, 2:24MHz, 3:48MHz, 4:96MHz, 5:Invalid.}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioIDispLinkFrequency                        | 0x0094 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio iDisp-Link T-Mode} TYPE:{Combo} OPTION:{0: 2T, 1: 1T}
  # !BSF HELP:{HD-Audio iDisp-Link T-Mode Selectiton. 0:2T(Default), 1:1T.}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioIDispLinkTmode                            | 0x0095 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Disp DMIC} TYPE:{Combo} OPTION:{0: Disable, 1: 2ch array, 2: 4ch array}
  # !BSF HELP:{HD-Audio Disp DMIC Selectiton. 0:Disable, 1:2ch array(Default), 2:4ch array.}
  gBroxtonFspPkgTokenSpaceGuid.DspEndpointDmic                                  | 0x0096 | 0x01 | 0x01

  # !BSF NAME:{HD-Audio Bluetooth} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio bluetooth. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DspEndpointBluetooth                             | 0x0097 | 0x01 | 0x01

  # !BSF NAME:{HD-Audio I2S SHK} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio I2S SHK. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DspEndpointI2sSkp                                | 0x0098 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio I2S HP} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio I2S HP. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DspEndpointI2sHp                                 | 0x0099 | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Controller Power Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio Controller Power Gating. This option is deprecated.}
  gBroxtonFspPkgTokenSpaceGuid.AudioCtlPwrGate                                  | 0x009A | 0x01 | 0x00

  # !BSF NAME:{HD-Audio ADSP Power Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio ADSP Power Gating. This option is deprecated.}
  gBroxtonFspPkgTokenSpaceGuid.AudioDspPwrGate                                  | 0x009B | 0x01 | 0x00

  # !BSF NAME:{HD-Audio CSME Memory Transfers} TYPE:{Combo} OPTION:{0: VC0, 1: VC2}
  # !BSF HELP:{Enable/Disable HD-Audio CSME Memory Transfers. 0:VC0(Default), 1:VC2.}
  gBroxtonFspPkgTokenSpaceGuid.Mmt                                              | 0x009C | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Host Memory Transfers} TYPE:{Combo} OPTION:{0: VC0, 1: VC2}
  # !BSF HELP:{Enable/Disable HD-Audio Host Memory Transfers. 0:VC0(Default), 1:VC2.}
  gBroxtonFspPkgTokenSpaceGuid.Hmt                                              | 0x009D | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Power Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio BIOS Configuration Lock Down. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.HDAudioPwrGate                                   | 0x009E | 0x01 | 0x00

  # !BSF NAME:{HD-Audio Clock Gatingn} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio Clock Gating. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.HDAudioClkGate                                   | 0x009F | 0x01 | 0x00

  # !BSF NAME:{Bitmask of DSP Feature}  TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF OPTION:{[BIT0] - WoV, [BIT1] - BT Sideband, [BIT2] - Codec VAD, [BIT5] - BT Intel HFP, [BIT6] - BT Intel A2DP, [BIT7] - DSP based speech pre-processing disabled, [BIT8] - 0: Intel WoV, 1: Windows Voice Activation}
  # !BSF HELP:{Set Bitmask of HD-Audio DSP Feature. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DspFeatureMask                                   | 0x00A0 | 0x04 | 0x00000000

  # !BSF NAME:{Bitmask of supported DSP Post-Processing Modules} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF OPTION:{[BIT0] - WoV, [BIT1] - BT Sideband, [BIT2] - Codec VAD, [BIT5] - BT Intel HFP, [BIT6] - BT Intel A2DP, [BIT7] - DSP based speech pre-processing disabled, [BIT8] - 0: Intel WoV, 1: Windows Voice Activation}
  # !BSF HELP:{Set HD-Audio Bitmask of supported DSP Post-Processing Modules. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DspPpModuleMask                                  | 0x00A4 | 0x04 | 0x00000000

  # !BSF NAME:{HD-Audio BIOS Configuration Lock Down} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HD-Audio BIOS Configuration Lock Down. 0:Disable(Default), 1:Enable. This option is deprecated}
  gBroxtonFspPkgTokenSpaceGuid.BiosCfgLockDown                                  | 0x00A8 | 0x01 | 0x00

  #
  # High Precision Timer (Hpet) Configurations
  #

  # !BSF NAME:{Enable High Precision Timer} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Hpet. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Hpet                                             | 0x00A9 | 0x01 | 0x01

  # !BSF NAME:{Hpet Valid BDF Value} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Hpet Valid BDF Value. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.HpetBdfValid                                     | 0x00AA | 0x01 | 0x00

  # !BSF NAME:{Bus Number of Hpet} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Bus Number of Hpet. Default = 0xFA(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HpetBusNumber                                    | 0x00AB | 0x01 | 0xFA

  # !BSF NAME:{Device Number of Hpet} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Device Number of Hpet. 0x1F(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HpetDeviceNumber                                 | 0x00AC | 0x01 | 0x1F

  # !BSF NAME:{Function Number of Hpet} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Function Number of Hpet. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HpetFunctionNumber                               | 0x00AD | 0x01 | 0x00

  #
  # Io Apic Configurations
  #

  # !BSF NAME:{IoApic Valid BDF Value} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable IoApic Valid BDF Value. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.IoApicBdfValid                                   | 0x00AE | 0x01 | 0x00

  # !BSF NAME:{Bus Number of IoApic} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Bus Number of IoApic. 0xFA(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicBusNumber                                  | 0x00AF | 0x01 | 0xFA

  # !BSF NAME:{Device Number of IoApic} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Device Number of IoApic. 0x0F(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicDeviceNumber                               | 0x00B0 | 0x01 | 0x0F

  # !BSF NAME:{Function Number of IoApic} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Completer ID of Function Number of IoApic. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicFunctionNumber                             | 0x00B1 | 0x01 | 0x00

  # !BSF NAME:{IOAPIC Entry 24-119} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable IOAPIC Entry 24-119. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicEntry24_119                                | 0x00B2 | 0x01 | 0x01

  # !BSF NAME:{IO APIC ID} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{This member determines IOAPIC ID. 0x01(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicId                                         | 0x00B3 | 0x01 | 0x01

  # !BSF NAME:{IoApic Range} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Define address bits 19:12 for the IOxAPIC range. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IoApicRangeSelect                                | 0x00B4 | 0x01 | 0x00

  #
  # Integrated Sensor Hub (ISH) Configurations
  #

  # !BSF NAME:{ISH Controller} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable ISH Controller. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.IshEnable                                        | 0x00B5 | 0x01 | 0x01

  #
  # Lock Down Configurations
  #

  # !BSF NAME:{BIOS Interface Lock Down} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable BIOS Interface Lock Down bit to prevent writes to the Backup Control Register. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.BiosInterface                                    | 0x00B6 | 0x01 | 0x01

  # !BSF NAME:{Bios LockDown Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable the BIOS Lock Enable (BLE) feature and set EISS bit. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.BiosLock                                         | 0x00B7 | 0x01 | 0x00

  # !BSF NAME:{SPI EISS Status} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable InSMM.STS (EISS) in SPI. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SpiEiss                                          | 0x00B8 | 0x01 | 0x01

  # !BSF NAME:{BiosLock SWSMI Number} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{This member describes the SwSmi value for Bios Lock. 0xA9(Default).}
  gBroxtonFspPkgTokenSpaceGuid.BiosLockSwSmiNumber                              | 0x00B9 | 0x01 | 0xA9

  #
  # LPSS Configurations
  #

  # !BSF NAME:{LPSS IOSF PMCTL S0ix Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable LPSS IOSF Bridge PMCTL Register S0ix Bits. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.LPSS_S0ixEnable                                  | 0x00BA | 0x01 | 0x00

  # !BSF NAME:{LPSS I2C Clock Gating Configuration} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable LPSS I2C Clock Gating. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.I2cClkGateCfg                                    | 0x00BC | 0x08 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PSS HSUART Clock Gating Configuration} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable LPSS HSUART Clock Gating. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.HsuartClkGateCfg                                 | 0x00C4 | 0x04 | { 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{LPSS SPI Clock Gating Configuration} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFF)}
  # !BSF HELP:{Enable/Disable LPSS SPI Clock Gating. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SpiClkGateCfg                                    | 0x00C8 | 0x03 | { 0x01, 0x01, 0x01 }

  # !BSF NAME:{I2C Device 0} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 0. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c0Enable                                       | 0x00CB | 0x01 | 0x01

  # !BSF NAME:{I2C Device 1} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 1. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c1Enable                                       | 0x00CC | 0x01 | 0x01

  # !BSF NAME:{I2C Device 2} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 2. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c2Enable                                       | 0x00CD | 0x01 | 0x01

  # !BSF NAME:{I2C Device 3} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 3. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c3Enable                                       | 0x00CE | 0x01 | 0x01

  # !BSF NAME:{I2C Device 4} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 4. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c4Enable                                       | 0x00CF | 0x01 | 0x01

  # !BSF NAME:{I2C Device 5} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 5. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c5Enable                                       | 0x00D0 | 0x01 | 0x01

  # !BSF NAME:{I2C Device 6} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 6. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c6Enable                                       | 0x00D1 | 0x01 | 0x01

  # !BSF NAME:{I2C Device 7} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable I2C Device 7. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.I2c7Enable                                       | 0x00D2 | 0x01 | 0x01

  # !BSF NAME:{UART Device 0} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable UART Device 0. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Hsuart0Enable                                    | 0x00D3 | 0x01 | 0x01

  # !BSF NAME:{UART Device 1} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable UART Device 1. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Hsuart1Enable                                    | 0x00D4 | 0x01 | 0x01

  # !BSF NAME:{UART Device 2} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable UART Device 2. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Hsuart2Enable                                    | 0x00D5 | 0x01 | 0x01

  # !BSF NAME:{UART Device 3} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable UART Device 3. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Hsuart3Enable                                    | 0x00D6 | 0x01 | 0x01

  # !BSF NAME:{SPI UART Device 0} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable SPI Device 0. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Spi0Enable                                       | 0x00D7 | 0x01 | 0x01

  # !BSF NAME:{SPI UART Device 1} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable SPI Device 1. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Spi1Enable                                       | 0x00D8 | 0x01 | 0x01

  # !BSF NAME:{SPI UART Device 2} TYPE:{Combo} OPTION:{0: Disabled, 1: PCI Mode, 2: ACPI Mode}
  # !BSF HELP:{Enable/Disable SPI Device 2. 0:Disabled, 1:PCI Mode(Default), 2:ACPI Mode.}
  gBroxtonFspPkgTokenSpaceGuid.Spi2Enable                                       | 0x00D9 | 0x01 | 0x01

  # !BSF NAME:{OS Debug Feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable OS Debug Feature. 0:Disable(Default), 1: Enable.}
  gBroxtonFspPkgTokenSpaceGuid.OsDbgEnable                                      | 0x00DA | 0x01 | 0x00

  # !BSF NAME:{DCI Feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DCI Feature. 0:Disable(Default), 1: Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DciEn                                            | 0x00DB | 0x01 | 0x00

  # !BSF NAME:{UART Debug Base Address} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{UART Debug Base Address. 0x00000000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Uart2KernelDebugBaseAddress                      | 0x00DC | 0x04 | 0x0000000

  #
  # PCI Express (PCIE) Configurations
  #

  # !BSF NAME:{Enable PCIE Clock Gating} TYPE:{Combo}
  # !BSF OPTION:{0:Enable, 1:Disable}
  # !BSF HELP:{Enable/disable PCIE Clock Gating. 0:Enable, 1:Disable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieClockGatingDisabled                          | 0x00E0 | 0x01 | 0x00

  # !BSF NAME:{Enable PCIE Root Port 8xh Decode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PCIE Root Port 8xh Decode. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRootPort8xhDecode                            | 0x00E1 | 0x01 | 0x01

  # !BSF NAME:{PCIE 8xh Decode Port Index} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{PCIE 8xh Decode Port Index. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.Pcie8xhDecodePortIndex                           | 0x00E2 | 0x01 | 0x00

  # !BSF NAME:{Enable PCIE Root Port Peer Memory Write} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PCIE root port peer memory write. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRootPortPeerMemoryWriteEnable                | 0x00E3 | 0x01 | 0x00

  # !BSF NAME:{PCIE SWSMI Number} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{This member describes the SwSmi value for override PCIe ASPM table. 0xAA(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieAspmSwSmiNumber                              | 0x00E4 | 0x01 | 0xAA

  #
  # PCIE RP (PCIE RP) Configurations
  #

  # !BSF NAME:{PCI Express Root Port} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Control the PCI Express Root Port . 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRootPortEn                                   | 0x00E6 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Hide PCIE Root Port Configuration Space} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable Hide PCIE Root Port Configuration Space. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpHide                                       | 0x00EC | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE Root Port Slot Implement} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}}
  # !BSF HELP:{Enable/disable PCIE Root Port Slot Implement. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSlotImplemented                            | 0x00F2 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Hot Plug} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Hot Plug Enable/Disable. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpHotPlug                                    | 0x00F8 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE PM SCI} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PCI Express PME SCI. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpPmSci                                      | 0x00FE | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE Root Port Extended Sync} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PCIE Root Port Extended Sync. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpExtSync                                    | 0x0104 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Transmitter Half Swing} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Transmitter Half Swing Enable/Disable. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpTransmitterHalfSwing                       | 0x010A | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{ACS} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable Access Control Services Extended Capability. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpAcsEnabled                                 | 0x0110 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Clock Request Support} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable CLKREQ# Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpClkReqSupported                            | 0x0116 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Configure CLKREQ Number} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Configure Root Port CLKREQ Number if CLKREQ is supported. Default=0x04, 0x05, 0x00, 0x01, 0x02, 0x03.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpClkReqNumber                               | 0x011C | 0x06 | { 0x04, 0x05, 0x00, 0x01, 0x02, 0x03 }

  # !BSF NAME:{CLKREQ# Detection} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable CLKREQ# Detection Probe. 0: Disable(Default), 1: Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpClkReqDetect                               | 0x0122 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Advanced Error Reporting} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable Advanced Error Reporting. 0: Disable(Default), 1: Enable.}
  gBroxtonFspPkgTokenSpaceGuid.AdvancedErrorReporting                           | 0x0128 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PME Interrupt} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PME Interrupt. 0: Disable(Default), 1: Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PmeInterrupt                                     | 0x012E | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  URR} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Unsupported Request Reporting Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.UnsupportedRequestReport                         | 0x0134 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  FER} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Device Fatal Error Reporting Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.FatalErrorReport                                 | 0x013A | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  NFER} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Device Non-Fatal Error Reporting Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.NoFatalErrorReport                               | 0x0140 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  CER} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Device Correctable Error Reporting Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.CorrectableErrorReport                           | 0x0146 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  SEFE} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Root PCI Express System Error on Fatal Error Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SystemErrorOnFatalError                          | 0x014C | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  SENFE} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Root PCI Express System Error on Non-Fatal Error Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SystemErrorOnNonFatalError                       | 0x0152 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{  SECE} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Root PCI Express System Error on Correctable Error Enable/Disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SystemErrorOnCorrectableError                    | 0x0158 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIe Speed} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Configure PCIe Speed. 0:Auto(Default), 1:Gen1, 2:Gen2, 3:Gen3.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSpeed                                      | 0x015E | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Physical Slot Number} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Physical Slot Number for PCIE Root Port. Default=0x00, 0x01, 0x02, 0x03, 0x04, 0x05.}
  gBroxtonFspPkgTokenSpaceGuid.PhysicalSlotNumber                               | 0x0164 | 0x06 | { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }

  # !BSF NAME:{CTO} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PCI Express Completion Timer TO . 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpCompletionTimeout                          | 0x016A | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PTM Support} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PTM Support. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PtmEnable                                        | 0x0170 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{ASPM} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express Active State Power Management settings. 0:Disable, 1:L0s, 2:L1, 3:L0sL1, 4:Auto(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpAspm                                       | 0x0176 | 0x06 | { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 }

  # !BSF NAME:{L1 Substates} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCI Express L1 Substates settings. 0:Disable, 1:L1.1, 2:L1.2, 3:L1.1 & L1.2(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpL1Substates                                | 0x017C | 0x06 | { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }

  # !BSF NAME:{PCH PCIe LTR} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIE Latency Reporting Enable/Disable. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpLtrEnable                                  | 0x0182 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE LTR Lock} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{PCIE LTR Configuration Lock. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpLtrConfigLock                              | 0x0188 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  #
  # Power Management Configurations
  #

  # !BSF NAME:{PME_B0_S5 Disable bit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{PME_B0_S5_DIS bit in the General PM Configuration B (GEN_PMCON_B) register. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PmeB0S5Dis                                       | 0x018E | 0x01 | 0x00

  # !BSF NAME:{PCI Clock Run} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This member describes whether or not the PCI ClockRun feature of SC should be enabled. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.PciClockRun                                      | 0x018F | 0x01 | 0x00

  # !BSF NAME:{Enable/Disable Timer 8254 Clock Setting} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Timer 8254 Clock. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.Timer8254ClkSetting                              | 0x0190 | 0x01 | 0x00

  #
  # SATA Configurations
  #

  # !BSF NAME:{Chipset SATA} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables or Disables the Chipset SATA Controller. The Chipset SATA controller supports the 2 black internal SATA ports (up to 3Gb/s supported per port). 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EnableSata                                       | 0x0191 | 0x01 | 0x01

  # !BSF NAME:{SATA Mode Selection} TYPE:{Combo} OPTION:{0:AHCI, 1:RAID}
  # !BSF HELP:{Determines how SATA controller(s) operate. 0:AHCI(Default), 1:RAID.}
  gBroxtonFspPkgTokenSpaceGuid.SataMode                                         | 0x0192 | 0x01 | 0x00

  # !BSF NAME:{Aggressive LPM Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable PCH to aggressively enter link power state.  0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SataSalpSupport                                  | 0x0193 | 0x01 | 0x01

  # !BSF NAME:{SATA Power Optimization} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable SATA Power Optimizer on SC side.  0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPwrOptEnable                                 | 0x0194 | 0x01 | 0x00

  # !BSF NAME:{eSATA Speed Limit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable eSATA Speed Limit.  0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.eSATASpeedLimit                                  | 0x0195 | 0x01 | 0x00

  # !BSF NAME:{SATA Speed Limit} TYPE:{Combo} OPTION:{0:Default, 1: 1.5 Gb/s (Gen 1), 2: 3 Gb/s(Gen 2), 3: 6 Gb/s (Gen 1)}
  # !BSF HELP:{SATA Speed Limit. 0h:ScSataSpeed(Default), 1h:1.5Gb/s(Gen 1), 2h:3Gb/s(Gen 2), 3h:6Gb/s(Gen 3).}
  gBroxtonFspPkgTokenSpaceGuid.SpeedLimit                                       | 0x0196 | 0x01 | 0x00

  # !BSF NAME:{SATA Port} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Enable or Disable SATA Port.  0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsEnable                                  | 0x0198 | 0x02 | { 0x01, 0x01 }

  # !BSF NAME:{SATA Port DevSlp} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/Disable SATA Port  DevSlp. Board rework for LP needed before enable.  0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsDevSlp                                  | 0x019A | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{SATA Port HotPlug} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/Disable SATA Port Hotplug . 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsHotPlug                                 | 0x019C | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{Mechanical Presence Switch} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Controls reporting if this port has an Mechanical Presence Switch.\nNote:Requires hardware support.  0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsInterlockSw                             | 0x019E | 0x02 | { 0x01, 0x01 }

  # !BSF NAME:{External SATA Ports} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/Disable External SATA Ports. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsExternal                                | 0x01A0 | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{Spin Up Device} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/Disable device spin up at boot on selected Sata Ports. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsSpinUp                                  | 0x01A2 | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{SATA Solid State} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Identify the SATA port is connected to Solid State Drive or Hard Disk Drive.  0:Hard Disk Drive(Default), 1:Solid State Drive.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsSolidStateDrive                         | 0x01A4 | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{  DITO Configuration} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/Disable DITO Configuration.  0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsEnableDitoConfig                        | 0x01A6 | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{DM Value} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{DM Value.  0:Minimum, 0x0F:Maximum(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsDmVal                                   | 0x01A8 | 0x02 | { 0x0F, 0x0F }

  # !BSF NAME:{DITO Value} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{DEVSLP Idle Timeout Value. 0:Minimum, 0x03FF:Maximum, 0x0271(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsDitoVal                                 | 0x01AC | 0x04 | { 0x0271, 0x0271 }

  #
  # General Configurations
  #

  # !BSF NAME:{Subsystem Vendor ID} TYPE:{EditNum, HEX, (0x0000, 0xFFFF)}
  # !BSF HELP:{Subsystem Vendor ID. 0x8086(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SubSystemVendorId                                | 0x01B0 | 0x02 |  0x8086

  # !BSF NAME:{Subsystem ID} TYPE:{EditNum, HEX, (0x0000, 0xFFFF)}
  # !BSF HELP:{Subsystem ID. 0x7270(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SubSystemId                                      | 0x01B2 | 0x02 |  0x7270

  # !BSF NAME:{CRIDSettings} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{PMC CRID setting. 0:Disable(Default), 1:CRID_1, 2:CRID_2, 3:CRID_3.}
  gBroxtonFspPkgTokenSpaceGuid.CRIDSettings                                     | 0x01B4 | 0x01 | 0x00

  # !BSF NAME:{ResetSelect} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{ResetSelect. 0x6:warm reset(Default), 0xE:cold reset.}
  gBroxtonFspPkgTokenSpaceGuid.ResetSelect                                      | 0x01B5 | 0x01 | 0x00

  #
  # Storage and Communication Subsystem (SCS) Configurations
  #

  # !BSF NAME:{SD Card Support (D27:F0)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SD Card Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardEnabled                                    | 0x01B6 | 0x01 | 0x01

  # !BSF NAME:{SeMMC Support (D28:F0)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable eMMC Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.eMMCEnabled                                      | 0x01B7 | 0x01 | 0x01

  # !BSF NAME:{eMMC Max Speed} TYPE:{Combo} OPTION:{0:HS400, 1: HS200, 2:DDR50}
  # !BSF HELP:{Select the eMMC max Speed allowed. 0:HS400(Default), 1:HS200, 2:DDR50.}
  gBroxtonFspPkgTokenSpaceGuid.eMMCHostMaxSpeed                                 | 0x01B8 | 0x01 | 0x00

  # !BSF NAME:{UFS Support (D29:F0)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SDIO Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.UfsEnabled                                       | 0x01B9 | 0x01 | 0x01

  # !BSF NAME:{SDIO Support (D30:F0)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SDIO Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioEnabled                                      | 0x01BA | 0x01 | 0x01

  # !BSF NAME:{GPP Lock Feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable GPP lock. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.GppLock                                          | 0x01BB | 0x01 | 0x00

  #
  # Seial IRQ Configurations
  #

  # !BSF NAME:{Serial IRQ} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Serial IRQ. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SirqEnable                                       | 0x01BC | 0x01 | 0x01

  # !BSF NAME:{Serial IRQ Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Serial IRQ Mode Selection. 0:Quiet mode(Default), 1:Continuous mode.}
  gBroxtonFspPkgTokenSpaceGuid.SirqMode                                         | 0x01BD | 0x01 | 0x00

  # !BSF NAME:{Start Frame Pulse Width} TYPE:{Combo}
  # !BSF HELP:{Start Frame Pulse Width Value. 0:ScSfpw4Clk(Default), 1: ScSfpw6Clk, 2:ScSfpw8Clk.}
  # !BSF OPTION:{0:ScSfpw4Clk, 1:ScSfpw6Clk, 2:ScSfpw8Clk}
  gBroxtonFspPkgTokenSpaceGuid.StartFramePulse                                  | 0x01BE | 0x01 | 0x00

  #
  # SMBus Configurations
  #

  # !BSF NAME:{Enable SMBus} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable SMBus controller. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SmbusEnable                                      | 0x01BF | 0x01 | 0x01

  # !BSF NAME:{SMBus ARP Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable SMBus ARP Support. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ArpEnable                                        | 0x01C0 | 0x01 | 0x01

  # !BSF NAME:{SMBus Table Elements} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{The number of elements in the Reserved SMBus Address Table. 0x0080(Default).}
  gBroxtonFspPkgTokenSpaceGuid.NumRsvdSmbusAddresses                            | 0x01C2 | 0x02 | 0x0080

  # !BSF NAME:{Reserved SMBus Address Table} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Array of addresses reserved for non-ARP-capable SMBus devices. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.RsvdSmbusAddressTable                            | 0x01C4 | 0x80 | { 0x00 }

  #
  # USB Configurations
  #

  # !BSF NAME:{XHCI Disable Compliance Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Options to disable XHCI Link Compliance Mode. Default is FALSE to not disable Compliance Mode. Set TRUE to disable Compliance Mode. 0:FALSE(Default), 1:True.}
  gBroxtonFspPkgTokenSpaceGuid.DisableComplianceMode                            | 0x0244 | 0x01 | 0x00

  # !BSF NAME:{USB Per-Port Control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Control each of the USB ports enable/disable. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.UsbPerPortCtl                                    | 0x0245 | 0x01 | 0x00

  # !BSF NAME:{xHCI Mode} TYPE:{Combo}
  # !BSF HELP:{Mode of operation of xHCI controller. 0:Disable, 1:Enable, 2:Auto(Default)}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  gBroxtonFspPkgTokenSpaceGuid.Usb30Mode                                        | 0x0246 | 0x01 | 0x02

  # !BSF NAME:{Enable USB2 ports} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable per USB2 ports. One byte for each port, byte0 for port0, byte1 for port1, and so on. 0x01(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20Enable                                  | 0x0248 | 0x08 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{USB20 Over Current Pin} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Over Current Pin number of USB 2.0 Port. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PortUs20bOverCurrentPin                          | 0x0250 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{XDCI Support} TYPE:{Combo}
  # !BSF HELP:{Enable/Disable XDCI. 0:Disable, 1:PCI_Mode(Default), 2:ACPI_mode.}
  # !BSF OPTION:{0:Disable, 1:PCI_Mode, 2:ACPI_mode}
  gBroxtonFspPkgTokenSpaceGuid.UsbOtg                                           | 0x0258 | 0x01 | 0x01

  # !BSF NAME:{Enable XHCI HSIC Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable USB HSIC1. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.HsicSupportEnable                                | 0x0259 | 0x01 | 0x00

  # !BSF NAME:{Enable USB3 ports} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable per USB3 ports. One byte for each port, byte0 for port0, byte1 for port1, and so on. 0x01(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb30Enable                                  | 0x025A | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{USB20 Over Current Pin} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Over Current Pin number of USB 3.0 Port. 0x01(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PortUs30bOverCurrentPin                          | 0x0260 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Enable XHCI SSIC Support} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Enable/disable XHCI SSIC ports. One byte for each port, byte0 for port0, byte1 for port1. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SsicPortEnable                                   | 0x0266 | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{SSIC Dlane PowerGating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SSIC Data lane Power Gating. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DlanePwrGating                                   | 0x0268 | 0x02 |  0x01

  #
  # VT-d Configurations
  #

  # !BSF NAME:{VT-d} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable VT-d. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.VtdEnable                                        | 0x026A | 0x01 |  0x00

  #
  # Other Configurations
  #

  # !BSF NAME:{SMI Lock bit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SMI_LOCK bit to prevent writes to the Global SMI Enable bit. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.LockDownGlobalSmi                                | 0x026B | 0x01 | 0x01

  # !BSF NAME:{HDAudio Delay Timer} TYPE:{EditNum, HEX, (0x0000, 0xFFFF)}
  # !BSF HELP:{The delay timer after Azalia reset. 0x012C(Default).}
  gBroxtonFspPkgTokenSpaceGuid.ResetWaitTimer                                   | 0x026C | 0x02 | 0x012C

  # !BSF NAME:{RTC Lock Bits} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable RTC Lock Bits. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.RtcLock                                          | 0x026E | 0x01 | 0x01

  # !BSF NAME:{SATA Test Mode Selection} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SATA Test Mode. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.SataTestMode                                     | 0x026F | 0x01 | 0x00

  # !BSF NAME:{XHCI SSIC RATE} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Set XHCI SSIC1 Rate to A Series or B Series. 1:A Series(Default), 2:B Series.}
  gBroxtonFspPkgTokenSpaceGuid.SsicRate                                         | 0x0270 | 0x02 | { 0x01, 0x01 }

  # !BSF NAME:{SMBus Dynamic Power Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SMBus dynamic power gating. 0:Disable(Default), 1:Enable.}
  gBroxtonFspPkgTokenSpaceGuid.DynamicPowerGating                               | 0x0272 | 0x02 | 0x00

  # !BSF NAME:{Max Snoop Latency} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting Max Snoop Latency. 0x0000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpLtrMaxSnoopLatency                         | 0x0274 | 0x0C | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  # !BSF NAME:{  Snoop Latency Override} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Snoop Latency Override for PCH PCIE. \nDisabled:Disable override.\nManual:Manually enter override values.\nAuto:Maintain default BIOS flow. 0:Disable, 1:Enable, 2:Auto(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideMode                   | 0x0280 | 0x06 | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{  Snoop Latency Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{LTR Snoop Latency value of PCH PCIE. 0:Minimum, 0x03FF:Maximum, 0x003C(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideValue                  | 0x0288 | 0x0C | { 0x003C, 0x003C, 0x0000, 0x003C, 0x003C, 0x003C }

  # !BSF NAME:{  Snoop Latency Multiplier} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{LTR Snoop Latency Multiplier of PCH PCIE. 0:1ns, 1:32ns, 2:1024ns(Default), 3:32768ns, 4:1048576ns, 5:33554432ns.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideMultiplier             | 0x0294 | 0x06 | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{Skip Multi-Processor Initialization} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When this is skipped, boot loader must initialize processors before SilicionInit API. 0: Initialize(Default), <b>1: Skip}
  gBroxtonFspPkgTokenSpaceGuid.SkipMpInit                                       | 0x029A | 0x01 | 0

  # !BSF NAME:{DCI Auto Detect} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Deprecated: Enable/disable DCI AUTO mode. Enabled(Default).}
  gBroxtonFspPkgTokenSpaceGuid.DciAutoDetect                                    | 0x029B | 0x01 | 0x01

  # !BSF NAME:{Max Non-Snoop Latency} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Max Non-Snoop Latency. 0x0000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpLtrMaxNonSnoopLatency                      | 0x029C | 0x0C | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  # !BSF NAME:{  Non Snoop Latency Override} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Non Snoop Latency Override for PCH PCIE. \nDisabled:Disable override.\nManual:Manually enter override values.\nAuto: Maintain default BIOS flow. 0:Disable, 1:Enable, 2:Auto(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideMode                | 0x02A8 | 0x06 | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{Halt and Lock TCO Timer}  TYPE:{Combo}
  # !BSF HELP:{Halt and Lock the TCO Timer (Watchdog).}
  # !BSF OPTION:{0:No, 1:Yes (default)}
  gBroxtonFspPkgTokenSpaceGuid.TcoTimerHaltLock                                 | 0x02AE | 0x01 | 0x01

  # !BSF NAME:{Power Button Override Period}  TYPE:{Combo} OPTION:{0x0:4s, 0x1:6s, 0x2:8s, 0x3:10s, 0x4:12s, 0x5:14s}
  # !BSF HELP:{specifies how long will PMC wait before initiating a global reset. 000b-4s(default), 001b-6s, 010b-8s, 011b-10s, 100b-12s, 101b-14s.)}
  gBroxtonFspPkgTokenSpaceGuid.PwrBtnOverridePeriod                             | 0x02AF | 0x01 | 0x00

  # !BSF NAME:{  Non Snoop Latency Value}  TYPE:{EditNum, HEX, (0x0000,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{LTR Non Snoop Latency value of PCH PCIE. 0:Minimum, 0x03FF:Maximum, 0x003C(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideValue               | 0x02B0 | 0x0C | { 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C }

  # !BSF NAME:{  Non Snoop Latency Multiplier} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{LTR Non Snoop Latency Multiplier of PCH PCIE. 0:1ns, 1:32ns, 2:1024ns(Default), 3:32768ns, 4:1048576ns, 5:33554432ns.}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideMultiplier          | 0x02BC | 0x06 | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE Root Port Slot Power Limit Scale}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFF)}
  # !BSF HELP:{Specifies scale used for slot power limit value. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSlotPowerLimitScale                        | 0x02C2 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE Root Port Slot Power Limit Value}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFF)}
  # !BSF HELP:{Specifies upper limit on power supplie by slot. 0x00(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSlotPowerLimitValue                        | 0x02C8 | 0x06 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Power Button Native Mode Disable}  TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable power button native mode, when 1, this will result in the PMC logic constantly seeing the power button as de-asserted. 0 (default))}
  gBroxtonFspPkgTokenSpaceGuid.DisableNativePowerButton    | 0x02CE | 0x01 | 0x00

  # !BSF NAME:{Power Button Debounce Mode}  TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable interrupt when PWRBTN# is asserted. 0:Disabled, 1:Enabled(default)}
  gBroxtonFspPkgTokenSpaceGuid.PowerButterDebounceMode     | 0x02CF | 0x01 | 0x01

  #
  # SDIO/SDCARD/EMMC Registers DLL default Configuration
  #
  # !BSF NAME:{SDIO_TX_CMD_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDIO_TX_CMD_DLL_CNTL. 0x505(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioTxCmdCntl               | 0x02D0 | 0x04 | 0x00000505

  # !BSF NAME:{SDIO_TX_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDIO_TX_DATA_DLL_CNTL1. 0xE(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioTxDataCntl1             | 0x02D4 | 0x04 | 0x0000000E

  # !BSF NAME:{SDIO_TX_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDIO_TX_DATA_DLL_CNTL2. 0x22272828(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioTxDataCntl2             | 0x02D8 | 0x04 | 0x22272828

  # !BSF NAME:{SDIO_RX_CMD_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDIO_RX_CMD_DATA_DLL_CNTL1. 0x16161616(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioRxCmdDataCntl1          | 0x02DC | 0x04 | 0x16161616

  # !BSF NAME:{SDIO_RX_CMD_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDIO_RX_CMD_DATA_DLL_CNTL2. 0x10000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdioRxCmdDataCntl2          | 0x02E0 | 0x04 | 0x00010000

  # !BSF NAME:{SDCARD_TX_CMD_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_TX_CMD_DLL_CNTL. 0x505(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardTxCmdCntl             | 0x02E4 | 0x04 | 0x00000505

  # !BSF NAME:{SDCARD_TX_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_TX_DATA_DLL_CNTL1. 0xA13(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardTxDataCntl1           | 0x02E8 | 0x04 | 0x00000A13

  # !BSF NAME:{SDCARD_TX_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_TX_DATA_DLL_CNTL2. 0x24242828(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardTxDataCntl2           | 0x02EC | 0x04 | 0x24242828

  # !BSF NAME:{SDCARD_RX_CMD_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_RX_CMD_DATA_DLL_CNTL1. 0x73A3637(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardRxCmdDataCntl1        | 0x02F0 | 0x04 | 0x073A3637

  # !BSF NAME:{SDCARD_RX_STROBE_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_RX_STROBE_DLL_CNTL. 0x0(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardRxStrobeCntl          | 0x02F4 | 0x04 | 0x00000000

  # !BSF NAME:{SDCARD_RX_CMD_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{SDCARD_RX_CMD_DATA_DLL_CNTL2. 0x10000(Default).}
  gBroxtonFspPkgTokenSpaceGuid.SdcardRxCmdDataCntl2        | 0x02F8 | 0x04 | 0x00010000

  # !BSF NAME:{EMMC_TX_CMD_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_TX_CMD_DLL_CNTL. 0x505(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcTxCmdCntl               | 0x02FC | 0x04 | 0x00000505

  # !BSF NAME:{EMMC_TX_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_TX_DATA_DLL_CNTL1. 0xC11(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcTxDataCntl1             | 0x0300 | 0x04 | 0x00000C11

  # !BSF NAME:{EMMC_TX_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_TX_DATA_DLL_CNTL2. 0x1C2A2927(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcTxDataCntl2             | 0x0304 | 0x04 | 0x1C2A2927

  # !BSF NAME:{EMMC_RX_CMD_DATA_DLL_CNTL1} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_RX_CMD_DATA_DLL_CNTL1. 0x000D162F(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcRxCmdDataCntl1          | 0x0308 | 0x04 | 0x000D162F

  # !BSF NAME:{EMMC_RX_STROBE_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_RX_STROBE_DLL_CNTL. 0x0a0a(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcRxStrobeCntl            | 0x030C | 0x04 | 0x00000a0a

  # !BSF NAME:{EMMC_RX_CMD_DATA_DLL_CNTL2} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_RX_CMD_DATA_DLL_CNTL2. 0x1003b(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcRxCmdDataCntl2          | 0x0310 | 0x04 | 0x0001003b

  # !BSF NAME:{EMMC_MASTER_DLL_CNTL} TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFF)}
  # !BSF HELP:{EMMC_MASTER_DLL_CNTL. 0x001(Default).}
  gBroxtonFspPkgTokenSpaceGuid.EmmcMasterSwCntl            | 0x0314 | 0x04 | 0x00000001

  # !BSF NAME:{PCIe Selectable De-emphasis} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{When the Link is operating at 5.0 GT/s speed, this bit selects the level of de-emphasis for an Upstream component. 1b:-3.5 dB 0b:-6 dB. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.PcieRpSelectableDeemphasis  | 0x0318 | 0x06 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Monitor Mwait Enable}  TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Monitor Mwait. For Windows* OS, this should be Enabled. For Linux based OS, this should be Disabled. 0:Disable, 1:Enable(Default).}
  gBroxtonFspPkgTokenSpaceGuid.MonitorMwaitEnable          | 0x031E | 0x01 | 0x01

  # !BSF NAME:{Universal Audio Architecture compliance for DSP enabled system} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Not-UAA Compliant (Intel SST driver supported only), 1: UAA Compliant (HDA Inbox driver or SST driver supported).}
  gBroxtonFspPkgTokenSpaceGuid.HdAudioDspUaaCompliance     | 0x031F | 0x01 | 0x00

  # !BSF NAME:{IRQ Interrupt Polarity Control}  TYPE:{EditNum, HEX, (0x00000000, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Set IRQ Interrupt Polarity Control to ITSS.IPC[0]~IPC[3]. 0:Active High, 1:Active Low}
  gBroxtonFspPkgTokenSpaceGuid.IPC                         | 0x0320 | 0x10 | {0xFFFFEEF8, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}

  # !BSF NAME:{Disable ModPHY dynamic power gate} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Disable ModPHY dynamic power gate for the specific SATA port.}
  gBroxtonFspPkgTokenSpaceGuid.SataPortsDisableDynamicPg   | 0x0330 | 0x02 | { 0x00, 0x00}

  # !BSF NAME:{Init CPU during S3 resume} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Do not initialize CPU during S3 resume. 1: Initialize CPU during S3 resume.}
  gBroxtonFspPkgTokenSpaceGuid.InitS3Cpu                   | 0x0332 | 0x01 | 0x00

  # !BSF NAME:{Skip P-unit Initialization} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When this is skipped, boot loader must initialize P-unit before SilicionInit API. 0: Initialize(Default), 1: Skip}
  gBroxtonFspPkgTokenSpaceGuid.SkipPunitInit                  | 0x0333 | 0x01 | 0x00

  # !BSF NAME:{PerPort Half Bit Pre-emphasis} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PerPort Half Bit Pre-emphasis. Value of register USB2_PER_PORT_PPX [14]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20PerPortTxPeHalf      | 0x0338 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PerPort HS Pre-emphasis Bias} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PerPort HS Pre-emphasis Bias. Value of register USB2_PER_PORT_PPX [13:11]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20PerPortPeTxiSet      | 0x0340 | 0x08 | { 0x07, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x01 }

  # !BSF NAME:{PerPort HS Transmitter Bias} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PerPort HS Transmitter Bias. Value of register USB2_PER_PORT_PPX [10:8]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20PerPortTxiSet      | 0x0348 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 }

  # !BSF NAME:{Select the skew direction for HS transition} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Select the skew direction for HS transition. Value of register USB2_PER_PORT_2_PPX [25]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20HsSkewSel           | 0x0350 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

  # !BSF NAME:{Per Port HS Transmitter Emphasis} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Per Port HS Transmitter Emphasis. Value of register USB2_PER_PORT_2_PPX [24:23]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20IUsbTxEmphasisEn      | 0x0358 | 0x08 | { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01 }

  # !BSF NAME:{PerPort HS Receiver Bias} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PerPort HS Receiver Bias. Value of register  USB2_PER_PORT_PPX [19:17]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20PerPortRXISet      | 0x0360 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Delay/skew's strength control for HS driver} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Delay/skew's strength control for HS driver. Value of register USB2_PER_PORT_2_PPX [1:0]}
  gBroxtonFspPkgTokenSpaceGuid.PortUsb20HsNpreDrvSel      | 0x0368 | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 }

  # !BSF NAME:{OS Selection} TYPE:{Combo}
  # !BSF OPTION:{0x0:Windows, 0x1:Android, 0x3:Linux}
  # !BSF HELP:{Windows or Android or Linux OS selection to be used by HDA, USB Common, PWM and PEI Graphics modules. Windows (default), Android, Linux}
  gBroxtonFspPkgTokenSpaceGuid.OsSelection                 | 0x0370 | 0x01 | 0x00

  # !BSF NAME:{DPTF} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Intel® Dynamic Platform and Thermal Framework. 0x0:Disabled (default), 0x1:Enabled}
  gBroxtonFspPkgTokenSpaceGuid.DptfEnabled                 | 0x0371 | 0x01 | 0x00

  # !BSF NAME:{PWM Enabled} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{PWM Device Enabling. Windows needs this to be disabled, while Android needs this to be enabled. 0x0:Disabled (default), 0x1:Enabled}
  gBroxtonFspPkgTokenSpaceGuid.PWMEnabled                  | 0x0372 | 0x01 | 0x00

  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:END}
  gBroxtonFspPkgTokenSpaceGuid.ReservedFspsUpd             | 0x0373 | 0x0D | {0x00}


  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.
  gBroxtonFspPkgTokenSpaceGuid.UpdTerminator               | 0x03AE | 0x02 | 0x55AA

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components.IA32]
  #
  # SEC
  #
  IntelFsp2Pkg/FspSecCore/FspSecCoreT.inf {
    <LibraryClasses>
      FspSecPlatformLib|$(FSP_PACKAGE)/Library/PlatformSecLib/Vtf0PlatformSecTLib.inf
  }

  IntelFsp2Pkg/FspSecCore/FspSecCoreM.inf {
    <LibraryClasses>
      FspSecPlatformLib|$(FSP_PACKAGE)/Library/PlatformSecLib/Vtf0PlatformSecMLib.inf
  }

  IntelFsp2Pkg/FspSecCore/FspSecCoreS.inf {
    <LibraryClasses>
      FspSecPlatformLib|$(FSP_PACKAGE)/Library/PlatformSecLib/Vtf0PlatformSecSLib.inf
  }

  #
  # PEI Core
  #
  $(FSP_PACKAGE)/Override/MdeModulePkg/Core/Pei/PeiMain.inf

  #
  # PCD
  #
  $(FSP_PACKAGE)/Override/MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  #
  # FSP Binary Components
  #
  $(FSP_PACKAGE)/FspHeader/FspHeader.inf
  $(FSP_PACKAGE)/FspInit/Pei/FspInitPreMem.inf {
    <LibraryClasses>
      CpuPolicyLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiCpuPolicyLibPreMem/PeiCpuPolicyLibPreMem.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80000002
  }

  $(PLATFORM_SI_PACKAGE)/NorthCluster/MemoryInit/MemoryInitFsp.inf {
    <BuildOptions>
    !if $(VP_BIOS_ENABLE) == TRUE
      *_*_IA32_CC_FLAGS       = -D_VP_=1
    !endif
    !if $(MRCDEBUG_ENABLE) == TRUE
      MSFT:*_*_IA32_CC_FLAGS       = -D EFI_DEBUG -D DEBUG_MODE=1 $(MRC_BUILD_OPTION_MSFT)
      GCC:*_*_IA32_CC_FLAGS       = -D EFI_DEBUG -D DEBUG_MODE=1 -O0 $(MRC_BUILD_OPTION_GCC)
    !else
      MSFT:*_*_IA32_CC_FLAGS  = -DDEBUG_MSG=1 $(MRC_BUILD_OPTION_MSFT)
      INTEL:*_*_IA32_CC_FLAGS = /GL- $(MRC_BUILD_OPTION_MSFT)
      GCC:*_*_IA32_CC_FLAGS   = -O0  $(MRC_BUILD_OPTION_GCC)
    !endif
    !if $(SEC_ENABLE)==TRUE
      *_*_IA32_CC_FLAGS = -D SEC_SUPPORT_FLAG
    !endif
  }

  $(PLATFORM_SI_PACKAGE)/SiInit/Pei/SiInitPreMem.inf
  $(PLATFORM_SI_PACKAGE)/SiInit/Pei/SiInitFsp.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80000000
  }
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
      ResetSystemLib|MdeModulePkg/Library/BaseResetSystemLibNull/BaseResetSystemLibNull.inf
  }
  $(FSP_PACKAGE)/FspInit/Pei/FspInit.inf {
    <LibraryClasses>
      CpuPolicyLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiCpuPolicyLib/PeiCpuPolicyLib.inf
  }

  $(PLATFORM_SI_PACKAGE)/SouthCluster/ScInit/Dxe/ScInitDxeFsp.inf
  IntelFsp2Pkg/FspNotifyPhase/FspNotifyPhasePeim.inf

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions]
# Append build options for EDK and EDKII drivers (= is Append, == is Replace)

[BuildOptions.Common.EDKII]

!if $(PCIESC_ENABLE) == TRUE
  DEFINE PCIESC_SUPPORT_BUILD_OPTION = -DPCIESC_SUPPORT=1
!endif

!if $(SATA_ENABLE) == TRUE
  DEFINE SATA_SUPPORT_BUILD_OPTION = -DSATA_SUPPORT=1
!endif

!if $(TARGET) == DEBUG
  DEFINE DBG_FLAG = -DEFI_DEBUG
!endif

  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS_GCC = -DFSP_FLAG $(DBG_FLAG) -Wno-error=unused-function  -Wno-error=overflow  -Wno-error=unknown-pragmas  -Wno-pointer-sign -Werror $(TABLET_PF_ENABLE) $(ENBDT_PF_ENABLE) $(BXTI_PF_ENABLE)
  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS_MSFT = /wd4819 /DFSP_FLAG $(DBG_FLAG) $(TABLET_PF_ENABLE) $(ENBDT_PF_ENABLE) $(BXTI_PF_ENABLE)

  MSFT:*_*_*_CC_FLAGS    = $(EDKII_DSC_FEATURE_BUILD_OPTIONS_MSFT)
  GCC:*_*_*_CC_FLAGS    =  $(EDKII_DSC_FEATURE_BUILD_OPTIONS_GCC)

  *_*_IA32_CC_FLAGS    = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(HDAUDIO_SUPPORT_BUILD_OPTION) $(SATA_SUPPORT_BUILD_OPTION) $(PCIESC_SUPPORT_BUILD_OPTION) $(MISC_BUILD_OPTION)
  *_*_IA32_VFRPP_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(HDAUDIO_SUPPORT_BUILD_OPTION) $(SATA_SUPPORT_BUILD_OPTION) $(PCIESC_SUPPORT_BUILD_OPTION) $(MISC_BUILD_OPTION)
  *_*_IA32_APP_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(HDAUDIO_SUPPORT_BUILD_OPTION) $(SATA_SUPPORT_BUILD_OPTION) $(PCIESC_SUPPORT_BUILD_OPTION) $(MISC_BUILD_OPTION)
  *_*_IA32_PP_FLAGS    = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DHDAUDIO_SUPPORT_BUILD_OPTION) $(SATA_SUPPORT_BUILD_OPTION) $(PCIESC_SUPPORT_BUILD_OPTION) $(MISC_BUILD_OPTION)
