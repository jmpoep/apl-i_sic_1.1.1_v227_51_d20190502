/** @file
  Framework PEIM to initialize memory on a Memory Controller.

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

#include "MemoryInit.h"
#include "DetectDimms.h"
#include "McFunc.h"
#include "MrcFunc.h"

#include <Library/PcdLib.h>
#include <Library/PciExpressLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/AcpiVariableCompatibility.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SecLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/ReadOnlyVariable.h>
#include <Guid/MemoryConfigData.h>
#include <Ppi/SeCUma.h>
#include <Ppi/HeciPpi.h>
#include <Ppi/DramPolicyPpi.h>
#include <Guid/NpkInfoHob.h>
#include <Guid/BiosReservedMemoryHob.h>
#include <Library/TimerLib.h>

#include <Library/PreSiliconLib.h>
#include <Library/SteppingLib.h>
#if defined (RTIT_Support) && (RTIT_Support == 1)
#include "RtitMemory.h"
#endif

#ifdef APTIO_BUILD
#define VFRCOMPILE
#include <Include/Setup.h>
#else
#if SMM_SUPPORT
#include <Guid\SmramMemoryReserve.h>
#endif
#endif

#ifdef FSP_FLAG
#include <Guid/GuidHobFsp.h>
#include <Guid/GuidHobFspEas.h>
#include <Library/FspPlatformLib.h>
#include <FspApi.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#endif

#ifndef FSP_FLAG
#include <Library/CmosAccessLib.h>
#endif

#define SKPD_RSVD_WARM_RESET               0x0C000000  //26,27


#ifdef PRAM_SUPPORT
extern EFI_GUID gPramAddrDataGuid;
#endif


#ifndef FSP_FLAG
/**
  This function installs the EFI Memory when S3 path is executed

  @param[in]      PeiServices  PEI Services Table.
  @param[in,out]  MrcData      Pointer to MRC Output Data that contains MRC Parameters.

  @retval  EFI_SUCCESS         When no errors when installing memory
  @retval  Others              Errors occurred while installing memory
**/
EFI_STATUS
InstallS3Memory (
  IN        EFI_PEI_SERVICES      **PeiServices,
  IN  OUT   MMRC_DATA   *MrcData
);
#endif
/**
  CpuMemoryTest

  @param[in]  None

  @retval  EFI_STATUS
**/
EFI_STATUS CpuMemoryTest() {

  UINT32                 Address;
  INT8                   errors_found;
  UINT32                 Data = 0;
  EFI_STATUS             Status;
  EFI_PHYSICAL_ADDRESS   TSegBase;
  REGISTER_ACCESS        Register;
  INT8                   ErrorLimit=100;
  //    UINT32                 AmtMemToTest = 0x100000;
  INT8                   WhichPattern = 0;
  UINT32                 Pattern = 0;
  UINT8                  Results[2];

  Results[0]=0;
  Results[1]=0;
  errors_found=0;
  Register.Mask = 0xFFFFFFFF;
  Register.ShiftBit = 0;
  Register.Offset = TSEGMB_0_0_0_PCI_CUNIT_REG;
  TSegBase = (UINT32) MemRegRead (CUNIT, 0, 0, Register) & 0xFFF00000;

  DEBUG ((EFI_D_INFO, "Please wait while memory is spot checked... \n"));

  Status = MMRC_SUCCESS;

  do {
    Address = 0;
    switch (WhichPattern) {
    case 0: Pattern = 0xFFFFFFFF;
      break;
    case 1: Pattern = 0x00000000;
      break;
    default: Pattern = 0x11111111;
    };
    //WRITE 1MB
    do {
#ifndef __KLOCWORK__
      * ( (volatile UINT32 *) Address) = Pattern;
#endif
      Address += 4;
    } while (Address < 0x100000);

    Address = 0;

    //READ 1MB
    do {
#ifndef __KLOCWORK__
      Data = * ( (volatile UINT32 *) Address);
#endif
      if (Data != Pattern) {
        if ((Results[WhichPattern] == 1) && (errors_found < ErrorLimit))
          DEBUG ((EFI_D_ERROR, "FAILURE detected at 0x%08x : expected %08x : actual %08x\n", Address, Pattern, Data));
        errors_found += 1;
        Results[WhichPattern] += 1;
        Status = MMRC_FAILURE;
      }
      //_asm stosd
      Address += 4;
    } while ((Address < 0x100000) && (errors_found < ErrorLimit));
    if (Results[WhichPattern] == 0)
      DEBUG ((EFI_D_INFO, "%08x pattern passed!\n", Pattern));
    WhichPattern++;
  } while ((WhichPattern < 2) && (errors_found < ErrorLimit));

  if (errors_found > ErrorLimit)
    DEBUG ((EFI_D_ERROR, "Max # of memory mis-matches reached(%d)!\n", errors_found));
  else
    DEBUG ((EFI_D_INFO, ">>Number of error(s) detected after memory test: %d\n", errors_found));

  if (errors_found == 0) {
    //WRITE the rest
    Address = 0x100000;
    do {
#ifndef __KLOCWORK__
      * ( (volatile UINT32 *) Address) = 0xffffffff;
#endif
      Address += 0x400;
    } while (Address < (UINT32)TSegBase);

    //READ the rest
    Address = 0x100000;

    do {
#ifndef __KLOCWORK__
      Data = * ( (volatile UINT32 *) Address);
#endif
      if (Data != 0xffffffff) {
        Status = MMRC_FAILURE;
        if (errors_found < ErrorLimit)
          DEBUG ((EFI_D_ERROR, "FAIL 0xffffffff pattern>> Address 0x%x: Data %x\n", Address, Data));
        errors_found++;
      }
      Address += 0x400;
    } while ((Address < (UINT32)TSegBase) && (errors_found < ErrorLimit));
  }

  if (errors_found == 0)
    DEBUG ((EFI_D_INFO, "MRC:CpuMemoryTest Successful!\n"));
  else {
    DEBUG ((EFI_D_ERROR, "MRC:CpuMemoryTest Failed! <>CPU Halted<>\n"));
    CpuDeadLoop();
  }
  return Status;
}

/**
  This function is the Framework PEIM entry point to initialize memory on a Memory Controller.

  @param[in]  FileHandle    File Handler
  @param[in]  PeiServices   PEI Services Table.

  @retval  EFI_SUCCESS      If memory training was successful.
  @retval  Others           Errors occurred while training memory
**/
EFI_STATUS
PeimMemoryInit (
  IN          EFI_PEI_FILE_HANDLE  FileHandle,
  IN  CONST   EFI_PEI_SERVICES     **PeiServices
)
{
  EFI_STATUS                          Status;
  EFI_BOOT_MODE                       BootMode;
  MMRC_DATA                           MrcData;

#ifdef DEBUG_HOOKS
  UINT8                               cpu_delay;
  UINT32                              preRegister;
  SKPD_struct                         RegData32;
  UINT32                              pcdBaseSave;
  MrcVersion                          IafwVersion;
#endif
  SEC_UMA_PPI                         *SeCUma;
  UINT8                               InitStat;
  DRAM_POLICY_PPI                     *DramPolicyPpi;
  BOOLEAN                             SkipNvDataRestore = FALSE;

InitializePeimMemoryInit:


  //
  // Disable PCH Watchdog timer at SB_RCBA+0x3410
  //
  Mmio32Write (PBASE + 0x08, Mmio32Read (PBASE + 0x08) | 0x10);
  //
  // Determine boot mode
  //
  Status = (*PeiServices)->GetBootMode (
             PeiServices,
             &BootMode
           );

  ASSERT_EFI_ERROR (Status);

  InitStat = 0;
  //
  // Initialize params before configure memory
  //
  MrcInitializeParameters (&MrcData);

  Status = (*PeiServices)->LocatePpi (
    PeiServices,
    &gDramPolicyPpiGuid,
    0,
    NULL,
    (VOID **)&DramPolicyPpi
    );

  if (!EFI_ERROR (Status)) {
    MrcData.MrcInputs.ChannelHashMask         = DramPolicyPpi->ChannelHashMask;
    MrcData.MrcInputs.SliceHashMask           = DramPolicyPpi->SliceHashMask;
    MrcData.MrcInputs.SlicesChannelsEnabled   = DramPolicyPpi->ChannelsSlicesEnabled;
    MrcData.MrcInputs.ScramblerSupport        = DramPolicyPpi->ScramblerSupport;
    MrcData.MrcInputs.InterleavedMode         = DramPolicyPpi->InterleavedMode;
    MrcData.MrcInputs.ForceMin2xRefresh       = DramPolicyPpi->MinRefRate2xEnabled;
    MrcData.MrcInputs.DualRankSupportEnabled  = DramPolicyPpi->DualRankSupportEnabled;
    CopyMem (&(MrcData.DramPolicyData), DramPolicyPpi, sizeof (DRAM_POLICY));
    DEBUG ((EFI_D_INFO, "Profile is now set to: %d \n", MrcData.DramPolicyData.Profile));
  }

#if (MRCDEBUG_MSG == 1)
  MrcData.MrcDebugMsgLevel = MMRC_DBG_VERBOSE;
#else
  MrcData.MrcDebugMsgLevel = MMRC_DBG_MIN;
#endif
  //
  // Execute DetectDimms
  //



  //
  // Locate SeCUma PPI which will be used in the following flows: coldboot and S3Resume
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gSeCUmaPpiGuid, 0, NULL, &SeCUma);
  ASSERT_EFI_ERROR ( Status);

  //
  // SEC Stolen Size in MB units
  //
  //DEBUG ((EFI_D_INFO, "MRC getting memory size from SeC ...\n"));
  MrcData.SeCUmaSize = SeCUma->SeCSendUmaSize ((EFI_PEI_SERVICES    **)PeiServices);  //expect in MB
  DEBUG ((EFI_D_INFO, "MRC SeCUmaSize memory size from SeC ... %x \n", MrcData.SeCUmaSize));


  if (BootMode == BOOT_ON_S3_RESUME) {
    MrcData.BootMode = S3;
    DEBUG ((EFI_D_INFO, "\nResuming from S3 detected! 0x%x\n", MrcData.BootMode));
  }

  MrcData.PlatformID = PLATFORM_ID;

  MrcData.CpuStepping = BxtStepping();
  if (MrcData.CpuStepping == 1)
    MrcData.CpuStepping = STEPPING_A0;
#if BXT_PO
  MrcData.CpuStepping = STEPPING_A0;
#endif
  DEBUG((EFI_D_INFO, "CPU stepping = %d.\n", MrcData.CpuStepping));

  MrcData.BxtSeries = GetBxtSeries();
  switch (MrcData.BxtSeries){
  case Bxt1:
    MrcData.BxtSeries = SERIES_BXT_1;
    break;
  case BxtP:
    MrcData.BxtSeries = SERIES_BXT_P;
    break;
  }
  DEBUG((EFI_D_INFO, "BXT Series = %d.\n", MrcData.BxtSeries));

  MRC_PEI_REPORT_PROGRESS_CODE(PeiServices, (EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_PC_CONFIGURING));

  pcdBaseSave = (UINT32)PcdGet64(PcdPciExpressBaseAddress);

  preRegister = pcdBaseSave + (UINT32)DEBUP0_0_0_0_PCI_CUNIT_REG;
  RegData32.Data = Mmio32Read(preRegister);
  MrcData.DEBUP0Reg = RegData32.Data;
  preRegister = pcdBaseSave + (UINT32)SKPD_0_0_0_PCI_CUNIT_REG;
  RegData32.Data = Mmio32Read(preRegister);
  MrcData.SKPDTestReg = RegData32.Data;
  DEBUG((EFI_D_INFO, " SKPD: 0x%08x DEBUP: 0x%08x\n", MrcData.SKPDTestReg, MrcData.DEBUP0Reg));

  GetIafwVersion(&MrcData, &IafwVersion);
  RegData32.SKPD_bits.IAFW_version    = (0xffff & ((IafwVersion.Version.Minor << 8) | (IafwVersion.Version.Rev)));
  Mmio32Write(preRegister, RegData32.Data);

  //
  // Restore MRC Parameters from NVRAM if Fast Boot is not disabled.
  //
  if (SkipNvDataRestore == FALSE && !((((DEBUP0_struct*)(&(MrcData.DEBUP0Reg)))->DEBUP0_bits.NoFB == 1) || MrcData.DramPolicyData.DisableFastBoot)) {
    Status = MrcParamsRestore (
      (EFI_PEI_SERVICES **)PeiServices,
      &MrcData
      );
    if (EFI_ERROR(Status)) {
      SkipNvDataRestore = TRUE;
      DEBUG((EFI_D_INFO, "\x1b[41mParamsRestore Error\x1b[0m\n"));
      //
      // If the MrcParamsRestore returned an error, MrcData is potentially populated with
      // stale or corrupt data. Jump back to a point where we can re-initalize the data.
      //
      goto InitializePeimMemoryInit;
    }
  }
  cpu_delay = 254;
  if (RegData32.SKPD_bits.HALT_before_MRC == 1) {
    DEBUG ((EFI_D_WARN, "! Stall selected at beginning of MRC !\n"));
    DEBUG ((EFI_D_WARN, "! Clear SKPD_0_0_0_PCI_CUNIT_REG[bit16] to continue !\n"));
    do {
        do {
          CpuPause();
        } while (cpu_delay-- > 0);
        cpu_delay = 254;
        RegData32.Data = Mmio32Read(pcdBaseSave + (UINT32)SKPD_0_0_0_PCI_CUNIT_REG);
    } while (RegData32.SKPD_bits.HALT_before_MRC == 1);
  }




  Status = ConfigureMemory (&MrcData);


  if (Status != EFI_SUCCESS) {
     DEBUG ((EFI_D_ERROR, "ConfigureMemory FAILED!\n"));
     MrcDeadLoop();
  }

  RegData32.Data = MrcData.SKPDTestReg;
  cpu_delay = 254;
  if (RegData32.SKPD_bits.HALT_after_MRC == 1) {
    DEBUG ((EFI_D_WARN, "! Stall selected at end of MRC !\n"));
    DEBUG ((EFI_D_WARN, "! Clear SKPD_0_0_0_PCI_CUNIT_REG[bit17] to continue !\n"));
    do {
      do {
        CpuPause();
      } while (cpu_delay-- > 0);
      cpu_delay = 254;
      RegData32.Data = Mmio32Read(pcdBaseSave + (UINT32)SKPD_0_0_0_PCI_CUNIT_REG);
    } while (RegData32.SKPD_bits.HALT_after_MRC == 1);
  }

#if VP_BIOS_ENABLE
  DEBUG ((EFI_D_INFO, "!!Skip Memory Test for VP!!\n"));
#else
  //Skip for Simics
  if ((RegData32.SKPD_bits.SkipMemTest != 1)&&(MrcData.PlatformID == VALUE_REAL_PLATFORM)&&
      ((MrcData.BootMode != S3)&&(MrcData.BootMode != S0C)&&(MrcData.BootMode != S0W)&&(MrcData.BootMode != S0))) {
    CpuMemoryTest();
  } else {
    if ((MrcData.BootMode != S3)&&(MrcData.BootMode != S0C) && (MrcData.BootMode != S0W) && (MrcData.BootMode != S0))
      DEBUG ((EFI_D_INFO, "!!-Preserving Memory-Skipping memory test-!!\n"));
  }
#endif


  // Fix SeCUmaBase at 0x1000000 For All Memory Sizes
  MrcData.SeCUmaBase = 0x10000000;   /// revisit

  SeCUma->SeCConfigDidReg(PeiServices,MrcData.BootMode, 0, MrcData.SeCUmaBase, &MrcData.SeCUmaSize);

  //Set MEM_INIT_DONE after sending DID message
  MmrcExecuteTask (&MrcData, SetPunitMemDone, NO_PRINT, 0);

  MmrcExecuteTask (&MrcData, PollPunit, NO_PRINT, 0);

  //
  // Install memory
  //
  DEBUG ((EFI_D_INFO, "Install EFI Memory.\n"));

  Status = InstallEfiMemory ((EFI_PEI_SERVICES **) PeiServices, BootMode, &MrcData);

  if (BootMode != BOOT_ON_S3_RESUME) {
    MrcParamsSave ((EFI_PEI_SERVICES **) PeiServices, &MrcData);
    //DEBUG ((EFI_D_INFO, "Save MRC params.\n"));
  }

  DEBUG ((EFI_D_INFO, "MemoryInit "));
  if (Status == EFI_SUCCESS) {
    MRC_PEI_REPORT_PROGRESS_CODE(PeiServices, (EFI_SOFTWARE_PEI_SERVICE | EFI_SW_PS_PC_INSTALL_PEI_MEMORY));
  } else {
    MRC_PEI_REPORT_ERROR_CODE(PeiServices, (EFI_SOFTWARE_PEI_SERVICE | EFI_SW_EC_NON_SPECIFIC), EFI_ERROR_MAJOR);
    DEBUG ((EFI_D_INFO, "Not "));
  }
  DEBUG ((EFI_D_INFO, "Installed.\n"));

  return EFI_SUCCESS;
}

/**
  This function installs the EFI Memory

  @param[in]      PeiServices   PEI Services Table.
  @param[in]      BootMode      Indicates the boot mode selected
  @param[in,out]  MrcData       Pointer to MRC Output Data that contains MRC Parameters

  @retval  EFI_SUCCESS      When no errors when installing memory
  @retval  Others           Errors occurred while installing memory
**/
EFI_STATUS
InstallEfiMemory (
  IN        EFI_PEI_SERVICES      **PeiServices,
  IN        EFI_BOOT_MODE         BootMode,
  IN  OUT   MMRC_DATA   *MrcData
)
{
  EFI_STATUS                      Status;

  UINT8                           RemapEnable;
  EFI_PHYSICAL_ADDRESS            UpperTotalMemory;
  UINT32                          MmioAllocation;
  EFI_PHYSICAL_ADDRESS            HostIOBoundaryHi;     //HostIOBoundaryHi = TOUUD = BMBOUNDHI
  EFI_PHYSICAL_ADDRESS            HostIOBoundary;       //HostIOBoundary = TOLUD = BMBOUND

  EFI_PHYSICAL_ADDRESS            TSegBase;
  UINT64                          TSegSize;

  EFI_PHYSICAL_ADDRESS            dPeiMemBase;
  UINT64                          PeiMemSize;

  EFI_PHYSICAL_ADDRESS            LowReservedMemoryBase; //Low Reserved Memory Base Address
  EFI_PHYSICAL_ADDRESS            LowSystemMemoryBase;
  UINT64                          LowSystemMemorySize;
  EFI_PHYSICAL_ADDRESS            LowSystemMemoryBase1;
  UINT64                          LowSystemMemorySize1;
  EFI_PHYSICAL_ADDRESS            LowSystemMemoryBase2;
  UINT64                          LowSystemMemorySize2;
  EFI_PHYSICAL_ADDRESS            HighSystemMemoryBase;
  UINT64                          HighSystemMemorySize;
  REGISTER_ACCESS                 Register;


#ifdef FSP_FLAG
  FSPM_UPD                        *FspmUpd;
  UINT32                          BootLoaderTolumSize;
  UINT32                          RemainingSize;
  EFI_PHYSICAL_ADDRESS            BootLoaderTolumBase;
#endif
#if SMM_SUPPORT
  //EFI_PEI_HOB_POINTERS            Hob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK  *SmramHobDescriptorBlock;
#endif
#if BDAT_ENABLED
  BDAT_STRUCTURE                  HostBDAT;
#endif
#ifdef PRAM_SUPPORT
  EFI_PHYSICAL_ADDRESS            PramMemBase = 0;
  UINT32                          PramMemSize = 0;
  EFI_PHYSICAL_ADDRESS            *PramBaseAddrHubData = NULL;
  UINTN                           BufferSize = 0;
  UINT8                           PramConfigVariable;
  BIOS_RESERVED_MEMORY_HOB        *BiosReservedMemoryHob = NULL;
#endif

  EFI_PEI_HOB_POINTERS            GuidHob;


  CopyMem (&GuidHob, 0, sizeof(EFI_PEI_HOB_POINTERS));

  Status =  MMRC_SUCCESS;
  RemapEnable = FALSE;
  UpperTotalMemory = 0;
  MmioAllocation = MrcData->OemMrcData.MmioSize;

#ifdef FSP_FLAG
  PeiMemSize = PcdGet32(PcdFspReservedMemoryLength);

  //Get the OemMemory to be reserved
  FspmUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();
  BootLoaderTolumSize = FspmUpd->FspmArchUpd.BootLoaderTolumSize;

  DEBUG ((EFI_D_INFO, "BootLoaderTolumSize:   0x%x\n", BootLoaderTolumSize));
  //
  // BootLoaderTolumSize needs to be page aligned.
  //
  if (BootLoaderTolumSize > 0) {
    RemainingSize = BootLoaderTolumSize % EFI_PAGE_SIZE;
    if (RemainingSize != 0) {
      BootLoaderTolumSize = BootLoaderTolumSize - RemainingSize + EFI_PAGE_SIZE;
    }
  }
#else
  PeiMemSize = 0x10000000; //256MB
#endif

  LowReservedMemoryBase = 0;
  LowSystemMemoryBase = 0;
  LowSystemMemorySize = 0;
  HighSystemMemoryBase = 0;
  HighSystemMemorySize = 0;

  UpperTotalMemory  = (EFI_PHYSICAL_ADDRESS) LShiftU64 (MrcData->SystemMemorySize, 20);

  Register.Mask = 0xFFFFFFFF;
  Register.ShiftBit = 0;

  Register.Offset = TOUUD_HI_0_0_0_PCI_CUNIT_REG;
  HostIOBoundaryHi = (EFI_PHYSICAL_ADDRESS)LShiftU64 (MemRegRead (CUNIT, 0, 0, Register), 32);
  Register.Offset = TOUUD_LO_0_0_0_PCI_CUNIT_REG;
  HostIOBoundaryHi = (EFI_PHYSICAL_ADDRESS) (HostIOBoundaryHi + (MemRegRead (CUNIT, 0, 0, Register) & 0xFFF00000));

  Register.Offset = TOLUD_0_0_0_PCI_CUNIT_REG;
  HostIOBoundary= MemRegRead (CUNIT, 0, 0, Register) & 0xFFF00000;

  Register.Offset = TSEGMB_0_0_0_PCI_CUNIT_REG;
  TSegBase = MemRegRead (CUNIT, 0, 0, Register) & 0xFFF00000;

//  Register.Offset = BGSM_0_0_0_PCI_CUNIT_REG;
//  TSegSize = (MemRegRead (CUNIT, 0, 0, Register) & 0xFFF00000) - TSegBase;
  TSegSize = (UINT64)(MrcData->OemMrcData.TsegSize * 0x100000);
/*
  DEBUG ((EFI_D_INFO, "MmioAllocation:   0x%08X\n", MmioAllocation));
  DEBUG ((EFI_D_INFO, "UpperTotalMemory: 0x%08X",   UpperTotalMemory >> 32));
  DEBUG ((EFI_D_INFO, "%08X\n",                     UpperTotalMemory));
  DEBUG ((EFI_D_INFO, "HostIOBoundary/TOUUD/BMBOUNDHI: 0x%08X\n", HostIOBoundary));
  DEBUG ((EFI_D_INFO, "HostIOBoundaryHi/TOLUD/BMBOUND: 0x%08X",   HostIOBoundaryHi >> 32));
  DEBUG ((EFI_D_INFO, "%08X\n", HostIOBoundaryHi));
  DEBUG ((EFI_D_INFO, "TSegBase: 0x%08X\n", TSegBase));
  DEBUG ((EFI_D_INFO, "TSegSize: 0x%08X\n", TSegSize));
*/
#if (VP_BIOS_ENABLE == 0) || (TABLET_PF_ENABLE == 0)
  if (UpperTotalMemory > HostIOBoundary) {
      RemapEnable = TRUE;
      DEBUG ((EFI_D_INFO, "RemapEnable is TRUE\n"));
  }
#endif
  DEBUG ((EFI_D_INFO, "-------------------Memory Mapping----------------\n"));

  //
  // Report first 640KB of memory
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    MEM_DET_COMMON_MEM_ATTR,
    (EFI_PHYSICAL_ADDRESS) (0),
    (UINT64) (0xA0000)
  );

 // DEBUG ((EFI_D_INFO, "System Memory:   0x%lx -- 0x%lx\n", (UINT64) 0x0, (UINT64) (0xA0000 - 1) ));

  //
  // Report first 0A0000h - 0FFFFFh as RESERVED memory
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    MEM_DET_COMMON_MEM_ATTR,
    (EFI_PHYSICAL_ADDRESS) (0xA0000),
    (UINT64) (0x60000)
  );

  //DEBUG ((EFI_D_INFO, "Reserved Memory: 0x%lx -- 0x%lx\n", (UINT64) 0xA0000, (UINT64) (0xA0000 + 0x60000 - 1)));

  // *------------+ HostIOBoundary
  // *            +
  // *------------+ GMS Base
  // *            +
  // *------------+ GTT Base
  // *            +
  // *------------+ RTIT Base
  // *            +
  // *------------+ TSEG Base
  // *            +




#ifdef FSP_FLAG
  if (TSegSize) {

    BuildResourceDescriptorWithOwnerHob (
      EFI_RESOURCE_MEMORY_RESERVED,
      (
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_TESTED |
        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE
      ),
      TSegBase,
      TSegSize,
      &gFspReservedMemoryResourceHobTsegGuid
    );

    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_RESERVED,
      MEM_DET_COMMON_MEM_ATTR,
      (EFI_PHYSICAL_ADDRESS)(TSegBase + TSegSize),
      (UINT64)(HostIOBoundary - TSegBase - TSegSize)
    );
  }
#else
  //
  //Update Memory Region from TSEG Base to HostIOBoundary as reserved memory
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    MEM_DET_COMMON_MEM_ATTR,
    (EFI_PHYSICAL_ADDRESS)(TSegBase),
    (UINT64)(HostIOBoundary - TSegBase)
    );
#endif
  //DEBUG ((EFI_D_INFO, "Reserved Memory: 0x%lx -- 0x%lx\n", (UINT64) TSegBase, (UINT64)(HostIOBoundary-1) ));

  //
  // LowReservedMemoryBase set to TsegBase which is the lowest Reserved memory base at this point
  //
  LowReservedMemoryBase = TSegBase;


  //
  // Report (TOM-TSEG_SIZE) to TSEG_SIZE as reserved memory (SMRAM TSEG)
  //
  if (TSegSize) {
    SmramHobDescriptorBlock = BuildGuidHob (
                &gEfiSmmPeiSmramMemoryReserveGuid,
                sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK) + sizeof (EFI_SMRAM_DESCRIPTOR)
              );
    SmramHobDescriptorBlock->NumberOfSmmReservedRegions = (UINT32) (1);
    SmramHobDescriptorBlock->Descriptor[0].PhysicalStart = (EFI_PHYSICAL_ADDRESS) (TSegBase);
    SmramHobDescriptorBlock->Descriptor[0].CpuStart      = (EFI_PHYSICAL_ADDRESS) (TSegBase);
    SmramHobDescriptorBlock->Descriptor[0].PhysicalSize  = (UINT64) TSegSize;
    SmramHobDescriptorBlock->Descriptor[0].RegionState   = (UINT64) (EFI_SMRAM_CLOSED);

    DEBUG ((EFI_D_INFO, "dTSeg:           0x%lx -- 0x%lx\n", (UINT64) TSegBase, (UINT64) TSegBase + TSegSize - 1));
  }// TSegSize

#ifdef PRAM_SUPPORT
  GuidHob.Raw = GetHobList ();
  if (GuidHob.Raw != NULL) {
    if ((GuidHob.Raw = GetNextGuidHob (&gBiosReserveMemoryHobGuid, GuidHob.Raw)) != NULL) {
      BiosReservedMemoryHob = GET_GUID_HOB_DATA (GuidHob.Guid);
    }
  }

  if (BiosReservedMemoryHob != NULL) {
    PramConfigVariable = BiosReservedMemoryHob->Pram;

    if (PramConfigVariable != 0x30)
    {
      switch (PramConfigVariable)
      {
      case 0x31: //4MB
        PramMemSize = 0x400000;
        break;

      case 0x32: //16MB
        PramMemSize = 0x1000000;
        break;

      case 0x33: //64MB
        PramMemSize = 0x4000000;
        break;

      default:
        PramMemSize = 0;
        break;
      }

      PramMemBase = LowReservedMemoryBase - PramMemSize;

      BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        MEM_DET_COMMON_MEM_ATTR,
        (EFI_PHYSICAL_ADDRESS)(PramMemBase),
        (UINT64)(PramMemSize)
        );

      BufferSize = sizeof (EFI_PHYSICAL_ADDRESS);
      PramBaseAddrHubData = BuildGuidHob (&gPramAddrDataGuid, BufferSize);
      *PramBaseAddrHubData = PramMemBase;

      LowReservedMemoryBase = PramMemBase;
    }
  }
#endif



  //
  // Change PeiMemory location for EFI-complaint Grub Bootloader, from LowReservedMemoryBase with length 64M
  //
#ifndef FSP_FLAG
  dPeiMemBase = LowReservedMemoryBase - PeiMemSize;
  if (dPeiMemBase <= 0x100000) {
    dPeiMemBase = 0x100000;
  }
#else
  BootLoaderTolumBase = LowReservedMemoryBase - BootLoaderTolumSize;
  dPeiMemBase = BootLoaderTolumBase - PeiMemSize;
  if (dPeiMemBase <= 0x100000) {
     dPeiMemBase = 0x100000;
  }
  LowReservedMemoryBase = dPeiMemBase;
#endif

#ifdef FSP_FLAG
  //Oem Reserved Memory
  if(BootLoaderTolumSize)
  {
    BuildResourceDescriptorWithOwnerHob (
       EFI_RESOURCE_MEMORY_RESERVED,
       (
         EFI_RESOURCE_ATTRIBUTE_PRESENT |
         EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
         EFI_RESOURCE_ATTRIBUTE_TESTED |
         EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
       ),
       BootLoaderTolumBase,
       BootLoaderTolumSize,
       &gFspBootLoaderTolumHobGuid
     );
    DEBUG ((
      EFI_D_INFO,
      "BootLoader Reserved Memory:   0x%lx -- 0x%lx\n",
      (UINT64) BootLoaderTolumBase,
      (UINT64) BootLoaderTolumBase + BootLoaderTolumSize - 1
    ));
  }

  BuildResourceDescriptorWithOwnerHob (
     EFI_RESOURCE_MEMORY_RESERVED,
     (
       EFI_RESOURCE_ATTRIBUTE_PRESENT |
       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
       EFI_RESOURCE_ATTRIBUTE_TESTED |
       EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
       EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
       EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
       EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
     ),
     dPeiMemBase,
     PeiMemSize,
     &gFspReservedMemoryResourceHobGuid
   );
#endif

  //
  // Report the memory to EFI
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = (*PeiServices)->InstallPeiMemory (PeiServices, dPeiMemBase, PeiMemSize);
    if (Status != EFI_SUCCESS) {
      MrcDeadLoop ();
    }
#ifndef FSP_FLAG
  } else {
    Status = InstallS3Memory(PeiServices, MrcData);
    ASSERT_EFI_ERROR (Status);
#endif
  }

  //
  // Memory Mapping
  //
  // *------------+ HostIOBoundaryHi (If RemapEnable == TRUE)
  // *            +
  // *------------+ 4G = HighSystemMemoryBase
  // *            +
  // *            +   [MMIO Memory]
  // *            +
  // *------------+ HostIOBoundary
  // *            +
  // *------------+ GMS Base
  // *            +
  // *------------+ GTT Base
  // *            +
  // *------------+ TSEG Base
  // *            +
  // *------------+ PRAM Base = LowReservedMemoryBase  (if PRMA enable)
  // *            +
  // *            + BootLoaderTolumBase (if BootLoaderTolumSize!=0)
  // *            +
  // *------------+ dPeiMemBase Base
  // *            +
  // *            +
  // *            +   [System Memory]
  // *            +
  // *------------+ 0x1000 (4K)    [SV S3 memory test region]
  // *            +   [SEC UMA]
  // *------------+ 0x400000 (256M)
  // *            +
  // *------------+ 0x100000 (1M) = LowSystemMemoryBase
  // *            +   [Reserved Memory]
  // *------------+ 0xA0000
  // *            +   [System Memory]
  // *------------+ 0

  LowSystemMemoryBase = 0x100000;
  LowSystemMemorySize = LowReservedMemoryBase - 0x100000;

  if (RemapEnable == TRUE) {
    HighSystemMemoryBase = 0x100000000;
    HighSystemMemorySize = HostIOBoundaryHi - 0x100000000;
  }
  /*
  DEBUG ((
    EFI_D_INFO,
    "PeiMemory:       0x%lx -- 0x%lx\n",
    (UINT64) dPeiMemBase,
    (UINT64) (dPeiMemBase + PeiMemSize - 1)
  ));  */


  //
  // Report 1MB to LowReservedMemoryBase as system memory
  //
  LowSystemMemoryBase1 = LowSystemMemoryBase;
  LowSystemMemorySize1 = MrcData->SeCUmaBase - LowSystemMemoryBase1;
  LowSystemMemoryBase2 = MrcData->SeCUmaBase + MrcData->SeCUmaSize;
  LowSystemMemorySize2 = LowSystemMemoryBase + LowSystemMemorySize - LowSystemMemoryBase2;

  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    MEM_DET_COMMON_MEM_ATTR,
    LowSystemMemoryBase1,
    LowSystemMemorySize1
  );

  DEBUG ((
    EFI_D_INFO,
    "System Memory:   0x%lx -- 0x%lx\n",
    (UINT64) LowSystemMemoryBase1,
    (UINT64) LowSystemMemoryBase1 + LowSystemMemorySize1 - 1
  ));

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    MEM_DET_COMMON_MEM_ATTR,
    MrcData->SeCUmaBase,
    MrcData->SeCUmaSize
  );

  DEBUG ((
    EFI_D_INFO,
    "CSEUma Memory:   0x%lx -- 0x%lx\n",
    (UINT64) MrcData->SeCUmaBase,
    (UINT64) MrcData->SeCUmaBase + MrcData->SeCUmaSize - 1
  ));


BuildResourceDescriptorHob (
  EFI_RESOURCE_SYSTEM_MEMORY,
  MEM_DET_COMMON_MEM_ATTR,
  LowSystemMemoryBase2,
  LowSystemMemorySize2
);
/*
  DEBUG ((
    EFI_D_INFO,
    "System Memory:   0x%lx -- 0x%lx\n",
    (UINT64) LowSystemMemoryBase2,
    (UINT64) LowSystemMemoryBase2 + LowSystemMemorySize2 - 1
  ));
*/

  if (RemapEnable == TRUE) {
    if (HighSystemMemorySize > 0) {
      BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        MEM_DET_COMMON_MEM_ATTR1,
        HighSystemMemoryBase,
        HighSystemMemorySize
      );
      DEBUG ((
        EFI_D_INFO,
        "UpperMemory:     0x%lx -- 0x%lx\n",
        (UINT64) HighSystemMemoryBase,
        (UINT64) (HighSystemMemoryBase) + HighSystemMemorySize - 1
      ));
    }
  } // RemapEnable

#if BDAT_ENABLED
  //
  // BDAT 2.0 STRUCT Test
  //
  DEBUG ((EFI_D_INFO, "Filling BDAT 2.0 Structure and saving it to HOB\n"));
  FillBdatStructure (MrcData, &HostBDAT);

  //
  //  Save the BDAT structure to a Hob.
  //
  MrcBdatSaveHob (PeiServices, MrcData, &HostBDAT);
#endif

#ifdef FSP_FLAG

  {
    UINT64              LowMemoryLength;
    UINT64              HighMemoryLength;
    FspGetSystemMemorySize (&LowMemoryLength,&HighMemoryLength);
  }
#endif
  return EFI_SUCCESS;
}

#ifndef FSP_FLAG
#if !TEMP_DISABLED
/**
  This function installs the EFI Memory when S3 path is executed

  @param[in]      PeiServices  PEI Services Table.
  @param[in,out]  MrcData      Pointer to MRC Output Data that contains MRC Parameters.

  @retval  EFI_SUCCESS         When no errors when installing memory
  @retval  Others              Errors occurred while installing memory
**/
EFI_STATUS
InstallS3Memory (
  IN        EFI_PEI_SERVICES      **PeiServices,
  IN  OUT   MMRC_DATA   *MrcData
)
{
  EFI_STATUS                      Status;
//   UINTN                           VarSize;
//   UINTN                           VarAttrib;
//   EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
//   UINT64                          AcpiVariableSet64;
  ACPI_VARIABLE_SET               *AcpiVariableSet;
  UINTN                           S3MemoryBase;
  UINTN                           S3MemorySize;

  Status          = EFI_SUCCESS;
  AcpiVariableSet = NULL;
//   VarSize         = sizeof (AcpiVariableSet64);
//   VarAttrib       = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;

  DEBUG ((EFI_D_INFO, "InstallS3Memory()\n"));
/*
//   //
//   // Get necessary PPI
//   //
//   // Need to locate DRAM Policy PPI for this
//   //
//   Status = (*PeiServices)->LocatePpi (
//              PeiServices,
//              &gEfiPeiReadOnlyVariable2PpiGuid,   //Getsystemconfigdata, varsize
//              0,
//              NULL,
//              &PeiVariable
//            );
//   ASSERT_EFI_ERROR (Status);
//
//   Status = PeiVariable->GetVariable (
//              PeiVariable,
//              ACPI_GLOBAL_VARIABLE,
//              &gEfiAcpiVariableGuid,
//              &VarAttrib,
//              &VarSize,
//              &AcpiVariableSet64
//            );
//
//   AcpiVariableSet = (ACPI_VARIABLE_SET *) (UINTN) AcpiVariableSet64;
//
//   if (EFI_ERROR (Status) || (AcpiVariableSet == NULL) ) {
//     return EFI_OUT_OF_RESOURCES;
//   }

//   AcpiVariableSet = (ACPI_VARIABLE_SET *) (UINTN)( \
//                        (UINT32)ReadCmos(0x58) + \
//                        ((UINT32)ReadCmos(0x59) << 8)+ \
//                        ((UINT32)ReadCmos(0x5A) << 16)+ \
//                        ((UINT32)ReadCmos(0x5B) << 24)
//                        );
*/
  AcpiVariableSet = (ACPI_VARIABLE_SET *) (UINTN)ReadCmos32(0x58);
  DEBUG ((EFI_D_INFO, "InstallS3Memory() read CMOS address 0x%08x\n",(UINT32)((UINTN)AcpiVariableSet)));
  S3MemoryBase = (UINTN) (AcpiVariableSet->AcpiReservedMemoryBase);
  S3MemorySize = (UINTN) (AcpiVariableSet->AcpiReservedMemorySize);

  //
  // Report Memory to EFI
  //
  Status = (*PeiServices)->InstallPeiMemory (PeiServices, S3MemoryBase, S3MemorySize);
  ASSERT_EFI_ERROR (Status);


  return Status;
}
#endif
#endif

/**
  This function restores the MRC parameters previously saved and configured in the Setup Options

  @param  PeiServices   - PEI Services Table.
  @param  MrcData       - Pointer to MRC Output Data that contains MRC Parameters.

  @retval EFI_SUCCESS   - MRC parameters restored correctly
  @retval Others        - Otherwise
**/
EFI_STATUS
MrcParamsRestore (
  IN        EFI_PEI_SERVICES    **PeiServices,
  IN  OUT   MMRC_DATA           *MrcData
  )
{
  EFI_STATUS                      Status;
  DRAM_POLICY_PPI                 *DramPolicyPpi;
  MRC_PARAMS_SAVE_RESTORE         *MrcParamsHob;
  UINT32                           MrcParamsSaveRestoreCrc;

  Status = (*PeiServices)->LocatePpi (
                             (CONST EFI_PEI_SERVICES **)PeiServices,
                             &gDramPolicyPpiGuid,
                             0,
                             NULL,
                             (VOID **)&DramPolicyPpi
                             );
  if ((EFI_ERROR (Status)) || ((VOID *)(UINT32)DramPolicyPpi->MrcTrainingDataPtr == NULL)) {
    DEBUG ((EFI_D_ERROR, "MRC Parameters not valid. Status is %r\n", Status));
  } else {
    MrcParamsHob = (MRC_PARAMS_SAVE_RESTORE*)((UINT32)DramPolicyPpi->MrcTrainingDataPtr);

    DEBUG((EFI_D_INFO, "Recovering MRC Parameters from DRAM_POLICY_PPI...\n"));
    // Copy the data locally before computing the CRC checksum.
    // This avoids unecessary accesses to the CSE SRAM
    // Note: the MRC_PARAMS_SAVE_RESTORE struct describes all of the NV data in the
    //       MMRC_DATA MrcData variable. This struct is a copy of the top of the
    //       MMRC_DATA struct which is why we can copy it directly from the HOB
    //       using sizeof(MRC_PARAMS_SAVE_RESTORE)
    CopyMem(MrcData, MrcParamsHob, sizeof (MRC_PARAMS_SAVE_RESTORE));

    DEBUG ((EFI_D_INFO, "CRC Changes:\n" ));
    //
    // Ignore MrcTrainingDataPtr in CRC check since this may change from boot to boot:
    //
    DramPolicyPpi->MrcTrainingDataPtr = (EFI_PHYSICAL_ADDRESS) 0;
    DEBUG((EFI_D_INFO, "MrcTrainingDataPtr = %x\n", DramPolicyPpi->MrcTrainingDataPtr));
    if (MrcData->SaMemCfgCrc != MrcCalculateCrc32((UINT8 *)DramPolicyPpi, sizeof (DRAM_POLICY_PPI))) {
      DEBUG ((EFI_D_INFO, "DPP\n" ));
      MrcData->MrcParamsValidFlag = 0;
      return EFI_NOT_READY;
    }
    MrcParamsSaveRestoreCrc = MrcData->MrcParamsSaveRestoreCrc;
    MrcData->MrcParamsSaveRestoreCrc = 0;
    DEBUG((EFI_D_INFO, "MrcParamsSaveRestoreCrc = %x\n", MrcData->MrcParamsSaveRestoreCrc));
    if (MrcParamsSaveRestoreCrc != MrcCalculateCrc32((UINT8 *)MrcData, sizeof (MRC_PARAMS_SAVE_RESTORE))) {
      DEBUG ((EFI_D_INFO, "MrcParams\n" ));
      MrcData->MrcParamsValidFlag = 0;
      return EFI_NOT_READY;
    }
    if (MrcData->MrcVer != GetMrcVersions(MrcData)) {
      DEBUG ((EFI_D_INFO, "MrcVer: %8xh != %8xh\n", MrcData->MrcVer, GetMrcVersions(MrcData)));
      MrcData->MrcParamsValidFlag = 0;
      return EFI_NOT_READY;
    }
    if ( MrcData->Signature != SIGNATURE_32 ('T', 'R', 'N', 'D') ) {
      MrcData->MrcParamsValidFlag = 0;
      return EFI_NOT_FOUND;
    }

    MrcData->FastBootEnable = 1;
    MrcData->SystemMemorySize = 0;
  }

  return EFI_SUCCESS;
}

/**
  This function saves the MRC Parameters to Hob for later use.

  @param[in]   PeiServices   PEI Services Table.
  @param[out]  MrcData       Pointer to MRC Output Data that contains MRC Parameters

  @retval  EFI_SUCCESS  - Hob is successfully built.
  @retval  Others       - Errors occur while creating new Hob
**/
EFI_STATUS
MrcParamsSave (
  IN        EFI_PEI_SERVICES      **PeiServices,
  IN  OUT   MMRC_DATA   *MrcData
  )
{
  MRC_PARAMS_SAVE_RESTORE   *MrcParamsHob;
  EFI_PEI_HOB_POINTERS      Hob;
  UINTN                     BufferSize;
  EFI_STATUS                Status;
  DRAM_POLICY_PPI           *DramPolicyPpi;

  BufferSize = sizeof (MRC_PARAMS_SAVE_RESTORE);

#ifndef FSP_FLAG
  Hob.Raw = BuildGuidHob(&gEfiMemoryConfigDataGuid, BufferSize);
#else
  //
  // FSP build the hob for boot laoder.
  //
  Hob.Raw = BuildGuidHob(&gFspNonVolatileStorageHobGuid, BufferSize);
#endif
  MrcParamsHob = (MRC_PARAMS_SAVE_RESTORE *) Hob.Raw;


  CopyMem (MrcParamsHob, MrcData, sizeof (MRC_PARAMS_SAVE_RESTORE));
  //Validate the saved data
  MrcParamsHob->Signature = SIGNATURE_32 ('T', 'R', 'N', 'D');
  MrcParamsHob->MrcParamsValidFlag = 1;

  //
  // Save compile time information to allow Fast Boot Data compatibility check
  //

  // MrcVer is also used for the BIOS set-up screen
  MrcParamsHob->MrcVer = GetMrcVersions(MrcData);

  //Update policy CRC
  Status = (*PeiServices)->LocatePpi(
    (CONST EFI_PEI_SERVICES **)PeiServices,
    &gDramPolicyPpiGuid,
    0,
    NULL,
    (VOID **)&DramPolicyPpi
    );

  ASSERT_EFI_ERROR (Status);

  //
  // Ignore MrcTrainingDataPtr in CRC calculation since this may change from boot to boot:
  //
  DramPolicyPpi->MrcTrainingDataPtr = (EFI_PHYSICAL_ADDRESS) (UINTN) NULL;
  MrcParamsHob->SaMemCfgCrc = MrcCalculateCrc32((UINT8 *)DramPolicyPpi, sizeof (DRAM_POLICY_PPI));

  //
  // Also calculate CRC on the MRC NV data which is used to restore the trained
  // register settings on S0/FB/S3 flows
  //
  MrcParamsHob->MrcParamsSaveRestoreCrc = MrcCalculateCrc32((UINT8 *)MrcParamsHob, sizeof (*MrcParamsHob));

  //DEBUG ((EFI_D_INFO, "MRC Parameters: MRC_PARAMS_SAVE_RESTORE size is %x\n", sizeof (MRC_PARAMS_SAVE_RESTORE)));
  //DEBUG ((EFI_D_INFO, "MRC Parameters: MMRC_DATA size is %x\n", sizeof (MMRC_DATA)));
  //DEBUG ((EFI_D_INFO, "MRC Parameters: CHANNEL size is %x\n", sizeof (CHANNEL)));

  return EFI_SUCCESS;
}


/**
  This function saves the BDAT structure to Hob for later use.

  @param[in]  PeiServices  -  PEI Services Table.
  @param[in]  MrcData      -  Pointer to MRC Output Data that contains MRC Parameters
  @param[in]  HostBDAT     -  Pointer to the BDAT data structure.

  @retval  EFI_SUCCESS     - Hob is successfully built.
  @retval  Others          - Errors occur while creating new Hob
**/
#if BDAT_ENABLED
EFI_STATUS
MrcBdatSaveHob (
  IN        EFI_PEI_SERVICES      **PeiServices,
  IN        MMRC_DATA             *MrcData,
  IN        BDAT_STRUCTURE        *HostBDAT
  )
{
  BDAT_STRUCTURE        *BdatHob;
  EFI_PEI_HOB_POINTERS  Hob;
  UINTN                 BufferSize;

  BufferSize = sizeof (BDAT_STRUCTURE);

  Hob.Raw = BuildGuidHob (
              &gEfiMrcBDATGuid,
              BufferSize
            );
  ASSERT (Hob.Raw);

  BdatHob = (BDAT_STRUCTURE *) Hob.Raw;

  CopyMem (
    BdatHob,
    HostBDAT,
    sizeof (BDAT_STRUCTURE)
  );

  ///
  /// Saving the pointer to the HOB to dunit channel 0 (only) sticky scratchpad 1.
  /// This scratchpad is also used for MRC status codes but MRC is done
  /// with status codes by the time we get here. Use of this scratchpad
  /// makes it possible to have BDAT on both AVN and VLV.
  ///
  MemRegWrite (DUNIT, 0, 0, SSKPD1_DUNIT_REG, (UINT32) BdatHob, 0xF);

  DEBUG ((EFI_D_INFO, "Wrote BDAT pointer, %p, into scratchpad.\n", BdatHob));

  return EFI_SUCCESS;
}
#endif



#if NORTHPEAK_SUPPORT
/**
  This function configures the NorthPeak

  @param[in] MrcData   Host structure for all MRC global data.

  @retval  Success
  @retval  Failure
**/
MMRC_STATUS
NorthPeakConfiguration (
  IN        MMRC_DATA   *MrcData,
)
{

}
#endif // #if NORTHPEAK_SUPPORT


