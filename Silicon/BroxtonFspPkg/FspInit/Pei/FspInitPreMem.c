/** @file
  Source code file for FSP Init Pre-Memory PEI module

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation.

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

#include "FspInitPreMem.h"
#include <Library/PostCodeLib.h>
#include <Library/FspCommonLib.h>

#define EFI_PHYSICAL_ADDRESS  UINT64
#include <MmrcData.h>

#pragma pack(1)
typedef struct {
  EFI_GUID          ConfigDataGuid;
  UINT32            ConfigDataSize;
  UINT32            CheckSum;
  UINT32            Reserved[2];
  UINT8             Data[16];
} PLATFORM_CONFIG_DATA_TABLE;
#pragma pack()

#define PLATFORM_CONFIG_DATA_ALIGNMENT      16
#define PLATFORM_CONFIG_DATA_TABLE_SIZE(a)  (((UINTN) ((sizeof(PLATFORM_CONFIG_DATA_TABLE) + a - 16)) + PLATFORM_CONFIG_DATA_ALIGNMENT - 1) & (~(PLATFORM_CONFIG_DATA_ALIGNMENT - 1)))

extern EFI_GUID gFspSiliconFvGuid;

EFI_STATUS
EFIAPI
GetFvNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

//
// The global indicator, the FvFileLoader callback will modify it to TRUE after loading PEIM into memory
//
BOOLEAN ImageInMemory = FALSE;
CHAR8   mGdtTable[0x40];
DRAM_POLICY_PPI mDramPolicyPpi;


GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MemoryDiscoveredPpiNotifyCallback
};

EFI_PEI_PPI_DESCRIPTOR mPeiTemporaryRamSupportPpi = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiTemporaryRamSupportPpiGuid,
    NULL
};

EFI_PEI_PPI_DESCRIPTOR mPeiDramPolicyPpi = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gDramPolicyPpiGuid,
    &mDramPolicyPpi
};

//
// Notify Callbacks for SPI and non-SPI boot devices.
// These are installed by MemoryDiscovered Callback, since they require main memory.
//
EFI_PEI_NOTIFY_DESCRIPTOR mFvNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiVirtualBlockIoPpiGuid,  //non-SPI boot
    GetFvNotifyCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gCseSpiSelectPpiGuid,   //SPI boot
    GetFvNotifyCallback
  }
};


VOID
RtcPowerFailureHandler (
  VOID
  )
{
  UINT16          Data16;
  UINT8           Data8;

  ///
  /// CHV BIOS Specification 0.5.0 - Section 29.4.3, "Power Failure Consideration"
  ///
  /// When the RTC_PWR_STS bit is set, it indicates that the RTCRST# signal went low.
  /// Software should clear this bit. For example, changing the RTC battery sets this bit.
  /// System BIOS should reset CMOS to default values if the RTC_PWR_STS bit is set.
  /// The System BIOS should execute the sequence below if the RTC_PWR_STS bit is set
  /// before memory initialization. This will ensure that the RTC state machine has been
  /// initialized.
  /// 1. If the RTC_PWR_STS bit is set which indicates a new coin-cell battery insertion or a
  ///    battery failure, steps 2 through 5 should be executed.
  /// 2.  Set RTC Register 0x0A[6:4] to 110b or 111b.
  /// 3.  Set RTC Register 0x0B[7].
  /// 4.  Set RTC Register 0x0A[6:4] to 010b.
  /// 5.  Clear RTC Register 0x0B[7].
  ///

  Data16 = MmioRead16 (PMC_BASE_ADDRESS + R_PMC_GEN_PMCON_1);

  if ((Data16 & B_PMC_GEN_PMCON_RTC_PWR_STS) != 0) {
    ///
    /// 2. Set RTC Register 0Ah[6:4] to 110b or 111b
    ///
    IoWrite8 (R_RTC_INDEX2, (UINT8) R_RTC_REGISTERA);
    Data8 = IoRead8 (R_RTC_TARGET2) & (UINT8) ~(B_RTC_REGISTERA_DV);
    Data8 |= (UINT8) (V_RTC_REGISTERA_DV_DIV_RST1);
    IoWrite8 (R_RTC_TARGET2, Data8);

    ///
    /// 3. Set RTC Register 0Bh[7].
    ///
    IoWrite8 (R_RTC_INDEX2, (UINT8) R_RTC_REGISTERB);
    IoOr8 (R_RTC_TARGET2, (UINT8) B_RTC_REGISTERB_SET);

    ///
    /// 4. Set RTC Register 0Ah[6:4] to 010b
    ///
    IoWrite8 (R_RTC_INDEX2, (UINT8) R_RTC_REGISTERA);
    Data8 = IoRead8 (R_RTC_TARGET2) & (UINT8) ~(B_RTC_REGISTERA_DV);
    Data8 |= (UINT8) (V_RTC_REGISTERA_DV_NORM_OP);
    IoWrite8 (R_RTC_TARGET2, Data8);

    ///
    /// 5. Clear RTC Register 0Bh[7].
    ///
    IoWrite8 (R_RTC_INDEX2, (UINT8) R_RTC_REGISTERB);
    IoAnd8 (R_RTC_TARGET2, (UINT8) ~B_RTC_REGISTERB_SET);
  }

  return;
}

VOID
ScBaseInit (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "ScBaseInit() - Start\n"));

  //
  // Set BARs for PMC SSRAM (0/13/3)
  // Allocation for these regions is done in MemoryCallback.c via call to BuildResourceDescriptorHob()
  MmioWrite32(
    MmPciAddress (0, 0, PCI_DEVICE_NUMBER_PMC, PCI_FUNCTION_NUMBER_PMC_SSRAM, 0x10),  //Write BAR0-lower
    PcdGet32(PcdPmcSsramBaseAddress0)
    );
  MmioWrite32(
    MmPciAddress (0, 0, PCI_DEVICE_NUMBER_PMC, PCI_FUNCTION_NUMBER_PMC_SSRAM, 0x18),  //Write BAR1-lower
    PcdGet32(PcdPmcSsramBaseAddress1)
    );
  MmioWrite16(
    MmPciAddress (0, 0, PCI_DEVICE_NUMBER_PMC, PCI_FUNCTION_NUMBER_PMC_SSRAM, 0x4),   //Set BME and MSE
    0x6
    );

  //
  // Set SPI Base Address
  //
  MmioWrite32 (
    MmPciAddress (0,
      DEFAULT_PCI_BUS_NUMBER_SC,
      PCI_DEVICE_NUMBER_SPI,
      PCI_FUNCTION_NUMBER_SPI,
      R_SPI_BASE
    ),
    (UINT32)((SPI_BASE_ADDRESS & B_SPI_BASE_BAR))
  );

  DEBUG ((EFI_D_INFO, "ScBaseInit() - End\n"));
}

/**
  Install Firmware Volume Hob's once there is main memory

  @param  PeiServices       General purpose services available to every PEIM.
  @param  NotifyDescriptor  Notify that this module published.
  @param  Ppi               PPI that was installed.

  @retval  EFI_SUCCESS     The function completed successfully.
**/
EFI_STATUS
EFIAPI
GetFvNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                     Status;
  UINT32                         Offset;
  UINT32                         FileSize;
  UINT8                          Index;
  FSPM_UPD                       *MemoryInitUpd;
  EFI_HOB_RESOURCE_DESCRIPTOR    *Descriptor;
  EFI_FIRMWARE_VOLUME_HEADER     *FvHeader;
  SUBPART_DIR_HEADER             *DirectoryHeader;
  SUBPART_DIR_ENTRY              *DirectoryEntry;
  BPDT_PAYLOAD_DATA              *BpdtPayloadPtr;
  CHAR8                          *OemFileName;
  BOOT_PARITION_SELECT            BootPart;
  BPDT_ENTRY_TYPES                EntryType;

  Status = EFI_SUCCESS;
  MemoryInitUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();

  if (MemoryInitUpd->FspmConfig.OemFileName == NULL) {
    return Status;
  }

  //
  // Read BIOS file from SPI.
  //
  OemFileName = (CHAR8 *)MemoryInitUpd->FspmConfig.OemFileName;
  DEBUG ((EFI_D_INFO, "FSP: Parsing OBB Payload from BPDT..\n"));
  if (AsciiStrnCmp (OemFileName, "BP1\\",  4) == 0) {
    OemFileName += 4;
    BootPart = BootPart1;
  } else if (AsciiStrnCmp (OemFileName, "BP2\\",  4) == 0) {
    OemFileName += 4;
    BootPart = BootPart2;
  } else {
    BootPart = BootPart2;
  }

  if (AsciiStrnCmp (OemFileName, "IBB\\",  4) == 0) {
    OemFileName += 4;
    EntryType = BpdtIbb;
  } else if (AsciiStrnCmp (OemFileName, "OBB\\",  4) == 0) {
    OemFileName += 4;
    EntryType = BpdtObb;
  } else {
    EntryType = BpdtObb;
  }

  GetBpdtPayloadData (BootPart, EntryType, &BpdtPayloadPtr);

  DirectoryHeader = (SUBPART_DIR_HEADER *)BpdtPayloadPtr->DataPtr;
  DirectoryEntry = (SUBPART_DIR_ENTRY *)((UINTN)DirectoryHeader + sizeof(SUBPART_DIR_HEADER));
  DEBUG ((EFI_D_INFO, "DirectoryHeader = %x, NumOf Dir Entries = %x\n", (UINTN)DirectoryHeader, DirectoryHeader->NumOfEntries));

  for (Index = 0; Index < DirectoryHeader->NumOfEntries; Index++, DirectoryEntry++) {
    Offset = DirectoryEntry->EntryOffset;
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)((UINTN)DirectoryHeader + Offset);
    DEBUG ((EFI_D_INFO, "%a Entry Offset = %x, FvHeader = %x\n", DirectoryEntry->EntryName, Offset, (UINTN)FvHeader));

    if ( OemFileName &&
         !AsciiStrCmp(DirectoryEntry->EntryName, OemFileName) )
    {
      FileSize = DirectoryEntry->EntrySize;
      DEBUG ((EFI_D_INFO, "Loading OBB image from 0x%08X to 0x%08X with length 0x%08X!\n", \
              FvHeader, MemoryInitUpd->FspmConfig.OemLoadingBase, FileSize));
      Descriptor = FspGetResourceDescriptorByOwner (&gFspReservedMemoryResourceHobGuid);
      CopyMem((CHAR8 *)MemoryInitUpd->FspmConfig.OemLoadingBase, FvHeader, FileSize);
    }
  }

  return Status;
}

/**
  Migrate FSP-M UPD data before destroying CAR.

**/
VOID
EFIAPI
MigrateFspmUpdData (
  VOID
 )
{
  FSP_INFO_HEADER           *FspInfoHeaderPtr;
  VOID                      *FspmUpdPtrPostMem;
  VOID                      *FspmUpdPtrPreMem;

  FspInfoHeaderPtr = GetFspInfoHeader();
  FspmUpdPtrPostMem = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)FspInfoHeaderPtr->CfgRegionSize));
  ASSERT(FspmUpdPtrPostMem != NULL);

  FspmUpdPtrPreMem = (VOID *)GetFspMemoryInitUpdDataPointer ();
  CopyMem (FspmUpdPtrPostMem, (VOID *)FspmUpdPtrPreMem, (UINTN)FspInfoHeaderPtr->CfgRegionSize);

  //
  // Update FSP-M UPD pointer in FSP Global Data
  //
  SetFspMemoryInitUpdDataPointer((VOID *)FspmUpdPtrPostMem);

  DEBUG ((DEBUG_INFO, "Migrate FSP-M UPD from %x to %x \n", FspmUpdPtrPreMem, FspmUpdPtrPostMem));

}


/**
  This function reports and installs new FV

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
ReportAndInstallNewFv (
  VOID
  )
{
  FSP_INFO_HEADER                *FspInfoHeader;
  EFI_FIRMWARE_VOLUME_HEADER     *FvHeader;
  UINT8                          *CurPtr;
  UINT8                          *EndPtr;

  FspInfoHeader = GetFspInfoHeaderFromApiContext();
  if (FspInfoHeader->Signature != FSP_INFO_HEADER_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "The signature of FspInfoHeader getting from API context is invalid at 0x%08X.\n", (UINT32)FspInfoHeader));
    FspInfoHeader = GetFspInfoHeader();
  }

  CurPtr = (UINT8 *)FspInfoHeader->ImageBase;
  EndPtr = CurPtr + FspInfoHeader->ImageSize - 1;

  while (CurPtr < EndPtr) {
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)CurPtr;
    if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
      break;
    }
    PeiServicesInstallFvInfoPpi (
      NULL,
      (VOID *)FvHeader,
      (UINTN) FvHeader->FvLength,
      NULL,
      NULL
      );
    CurPtr += FvHeader->FvLength;
  }

  return EFI_SUCCESS;
}

/**
  This function will be called when MRC is done.

  @param[in] PeiServices         General purpose services available to every PEIM.
  @param[in] NotifyDescriptor    Information about the notify event..
  @param[in] Ppi                 The notify context.

  @retval EFI_SUCCESS            If the function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  UINT64                        LowMemoryLength;
  UINT64                        HighMemoryLength;
  UINT64                        MaxLowMemoryLength;
  FSP_GLOBAL_DATA               *FspData;
  UINT8                         PhysicalAddressBits;
  UINT32                        RegEax;
  EFI_HOB_RESOURCE_DESCRIPTOR   *Descriptor;
  VOID                          **HobListPtr;
  FSPM_UPD                      *FspmUpd = NULL;
  EFI_STATUS                    Status;
  INTN                          DeviceIndex;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "Memory Discovered Notify invoked ...\n"));

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  //
  // Create a CPU hand-off information
  //
  BuildCpuHob (PhysicalAddressBits, 16);

  //
  // Get system memory from HOB
  //
  FspGetSystemMemorySize (&LowMemoryLength, &HighMemoryLength);

  //
  // FSP reserved memory is immediately following all available system memory regions,
  // so we should add it back to ensure this reserved region is cached.
  //
  Descriptor =  FspGetResourceDescriptorByOwner (&gFspReservedMemoryResourceHobGuid);
  ASSERT (Descriptor != NULL);
  LowMemoryLength =  Descriptor->PhysicalStart + Descriptor->ResourceLength;

  Descriptor =  FspGetResourceDescriptorByOwner (&gFspBootLoaderTolumHobGuid);
  if (Descriptor) {
    LowMemoryLength += Descriptor->ResourceLength;
  }

  DEBUG ((DEBUG_INFO, "FSP TOLM = 0x%08X\n", (UINT32)LowMemoryLength));

  //
  // Migrate FSP-M UPD data before destroying CAR
  //
  MigrateFspmUpdData ();

  HobListPtr = (VOID **)GetFspApiParameter2 ();
  if (HobListPtr != NULL) {
    *HobListPtr = (VOID *)GetHobList ();
  }
  //
  // Give control back after MemoryInitApi
  //
  FspMemoryInitDone (HobListPtr);

  if (GetFspApiCallingIndex() == TempRamExitApiIndex) {
    //
    // Disable CAR
    //
    ResetCacheAttributes ();

    //
    // Set fixed MTRR values
    //
    SetCacheAttributes (
      0x00000,
      0xA0000,
      EFI_CACHE_WRITEBACK
      );

    SetCacheAttributes (
      0xA0000,
      0x20000,
      EFI_CACHE_UNCACHEABLE
      );

    SetCacheAttributes (
      0xC0000,
      0x40000,
      EFI_CACHE_WRITEPROTECTED
      );

    //
    // Set the code region as cachable for performance
    //

    FspData  = GetFspGlobalDataPointer ();

    DEBUG ((DEBUG_INFO , "MicrocodeRegionBase : 0x%08x\n", FspData->PlatformData.MicrocodeRegionBase));
    DEBUG ((DEBUG_INFO , "MicrocodeRegionSize : 0x%08x\n", FspData->PlatformData.MicrocodeRegionSize));
    DEBUG ((DEBUG_INFO , "CodeRegionBase      : 0x%08x\n", FspData->PlatformData.CodeRegionBase));
    DEBUG ((DEBUG_INFO , "CodeRegionSize      : 0x%08x\n", FspData->PlatformData.CodeRegionSize));

    //
    // Change to write combining to accelerate copy memory by using SSE4.
    //
    SetCacheAttributes (
      0xFF800000,
      0x00800000,
      EFI_CACHE_WRITECOMBINING
      );

    //
    // Set the largest range as WB and then patch smaller ranges with UC
    // It can reduce the MTRR register usage
    //
    MaxLowMemoryLength = GetPowerOfTwo64 (LowMemoryLength);
    if (LowMemoryLength != MaxLowMemoryLength) {
      MaxLowMemoryLength = LShiftU64 (MaxLowMemoryLength, 1);
    }
    if (MaxLowMemoryLength >= 0x100000000ULL) {
      MaxLowMemoryLength = (LowMemoryLength + 0x0FFFFFFF) & 0xF0000000;
    }

    SetCacheAttributes (
      0,
      MaxLowMemoryLength,
      EFI_CACHE_WRITEBACK
      );

    if (LowMemoryLength != MaxLowMemoryLength) {
      SetCacheAttributes (
        LowMemoryLength,
        MaxLowMemoryLength - LowMemoryLength,
        EFI_CACHE_UNCACHEABLE
        );
    }

    if (HighMemoryLength) {
      SetCacheAttributes (
        0x100000000,
        HighMemoryLength,
        EFI_CACHE_WRITEBACK
        );
    }

    FspmUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();

    //
    // Loading OBB
    //
    DeviceIndex = -1;
    FspCheckBootDevice(&DeviceIndex, (const EFI_PEI_SERVICES **)PeiServices);
    if (FspmUpd->FspmConfig.OemFileName[0] != 0x00) {

      //
      // Register Notify Callbacks to handle additional FV discovery from NVM.
      //
      //      SPI - Notify on gCseSpiSelectPpiGuid.
      // eMMC/UFS - Notify on gEfiPeiVirtualBlockIoPpiGuid.
      //
      Status = (*PeiServices)->NotifyPpi ((const EFI_PEI_SERVICES **)PeiServices, &mFvNotifyList[0]);
      ASSERT_EFI_ERROR (Status);

      //If boot device is eMMC then run MmcEntry, which will install gEfiPeiVirtualBlockIoPpiGuid
      if (DeviceIndex == 0) {
        Status = MmcMainEntryPoint(NULL, (const EFI_PEI_SERVICES **)PeiServices);
        ASSERT_EFI_ERROR (Status);
      }
    }

    DEBUG ((DEBUG_INFO | DEBUG_INIT, "Memory Discovered Notify completed ...\n"));

    //
    // Give control back after TempRamExitApi
    //
    FspTempRamExitDone ();
  }

  ReportAndInstallNewFv ();

  return EFI_SUCCESS;
}

VOID
CopyMemSse4 (
  IN VOID* Dst,
  IN VOID* Src,
  IN UINTN SizeInBytes
  )
{
#if defined __GNUC__  // GCC compiler
  __asm__ __volatile__ (
    // Initialize pointers to start of the USWC memory
    "\n\t mov  %1, %%esi"  //mov esi, Src
    "\n\t mov  %1, %%edx"  //mov edx, Src

    // Initialize pointer to end of the USWC memory
    "\n\t add  %2, %%edx"  //add edx, SizeInBytes

    // Initialize pointer to start of the cacheable WB buffer
    "\n\t mov  %0, %%edi"  //mov edi, Dst

    // save xmm0 ~ xmm3 to stack
    "\n\t sub  $0x40, %%esp"
    "\n\t movdqu  %%xmm0, 0x00(%%esp)"
    "\n\t movdqu  %%xmm1, 0x10(%%esp)"
    "\n\t movdqu  %%xmm2, 0x20(%%esp)"
    "\n\t movdqu  %%xmm3, 0x30(%%esp)"

    // Start of Bulk Load loop
    "\n\t inner_start:"
    // Load data from USWC Memory using Streaming Load
    "\n\t MOVNTDQA  0x00(%%esi), %%xmm0"
    "\n\t MOVNTDQA  0x10(%%esi), %%xmm1"
    "\n\t MOVNTDQA  0x20(%%esi), %%xmm2"
    "\n\t MOVNTDQA  0x30(%%esi), %%xmm3"

    // Copy data to buffer
    "\n\t MOVDQA  %%xmm0, 0x00(%%edi)"
    "\n\t MOVDQA  %%xmm1, 0x10(%%edi)"
    "\n\t MOVDQA  %%xmm2, 0x20(%%edi)"
    "\n\t MOVDQA  %%xmm3, 0x30(%%edi)"

    // Increment pointers by cache line size and test for end of loop
    "\n\t add  $0x40, %%esi"
    "\n\t add  $0x40, %%edi"
    "\n\t cmp  %%edx, %%esi"
    "\n\t jne inner_start"

    // restore xmm0 ~ xmm3
    "\n\t mfence"
    "\n\t movdqu  0x00(%%esp), %%xmm0"
    "\n\t movdqu  0x10(%%esp), %%xmm1"
    "\n\t movdqu  0x20(%%esp), %%xmm2"
    "\n\t movdqu  0x30(%%esp), %%xmm3"
    "\n\t add  $0x40, %%esp" // stack cleanup
    ::"a"(Dst),"b"(Src),"c"(SizeInBytes)
    :"%esi", "%edi", "%edx"
  );
#else //MSFT compiler
  _asm {
    // Initialize pointers to start of the USWC memory
    mov esi, Src
    mov edx, Src

    // Initialize pointer to end of the USWC memory
    add edx, SizeInBytes

    // Initialize pointer to start of the cacheable WB buffer
    mov edi, Dst

    // save xmm0 ~ xmm3 to stack
    sub     esp, 040h
    movdqu  [esp], xmm0
    movdqu  [esp + 16], xmm1
    movdqu  [esp + 32], xmm2
    movdqu  [esp + 48], xmm3

    // Start of Bulk Load loop
    inner_start:
    // Load data from USWC Memory using Streaming Load
    MOVNTDQA xmm0, xmmword ptr [esi]
    MOVNTDQA xmm1, xmmword ptr [esi + 16]
    MOVNTDQA xmm2, xmmword ptr [esi + 32]
    MOVNTDQA xmm3, xmmword ptr [esi + 48]

    // Copy data to buffer
    MOVDQA xmmword ptr [edi], xmm0
    MOVDQA xmmword ptr [edi + 16], xmm1
    MOVDQA xmmword ptr [edi + 32], xmm2
    MOVDQA xmmword ptr [edi + 48], xmm3

    // Increment pointers by cache line size and test for end of loop
    add esi, 040h
    add edi, 040h
    cmp esi, edx
    jne inner_start

    // restore xmm0 ~ xmm3
    mfence
    movdqu  xmm0, [esp]
    movdqu  xmm1, [esp + 16]
    movdqu  xmm2, [esp + 32]
    movdqu  xmm3, [esp + 48]
    add     esp, 040h // stack cleanup
  }
#endif

}

/**
  FSP Init before memory PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
FspInitPreMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS              Status;
  VOID                    *Memory;
  SI_POLICY_PPI           *SiPolicyPpi;
  SC_PREMEM_POLICY_PPI    *ScPreMemPolicyPpi;
  SI_CPU_POLICY_PPI       *SiCpuPolicyPpi;
  SI_SA_POLICY_PPI        *SiSaPolicyPpi;
  SI_SA_POLICY_PPI        *SiSaPreMemPolicyPpi;
  IA32_DESCRIPTOR         GdtDscriptor;
  EFI_BOOT_MODE           BootMode;
  FSP_INFO_HEADER         *FspInfoHeader;
  UINT32                  CopySize;
  FSPM_UPD                *FspmUpd;
  UINT32                  Temp32;
  DRAM_POLICY_PPI         *DramPolicyPpi;
  EFI_PEI_PPI_DESCRIPTOR  *PeiPpiDescriptor;
  UINT32                  FspMLoadingBase;
  UINT32                  Gap;
  EFI_PEI_HOB_POINTERS    Hob;
  UINT32                  MrcNvDataCrc[3];

  SetPhaseStatusCode (FSP_STATUS_CODE_MEMORY_INIT);
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, FSP_STATUS_CODE_MEMORY_INIT | FSP_STATUS_CODE_COMMON_CODE | FSP_STATUS_CODE_API_ENTRY);

  DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - Start\n"));

  FspmUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();
  Status = (*PeiServices)->RegisterForShadow (FileHandle);
  if (Status == EFI_ALREADY_STARTED) {
    ImageInMemory = TRUE;
    AsmReadGdtr(&GdtDscriptor);
    if (GdtDscriptor.Base >= 0xFE000000) {
      (*PeiServices)->CopyMem (
                        (VOID *) mGdtTable,
                        (VOID *) GdtDscriptor.Base,
                        GdtDscriptor.Limit + 1
                        );
      GdtDscriptor.Base = (UINT32) mGdtTable;
    }
    AsmWriteGdtr(&GdtDscriptor);

    CopySize = PcdGet32 (PcdFlashFvFspmSize);
    Memory   = AllocatePages (EFI_SIZE_TO_PAGES(CopySize));

    FspInfoHeader = GetFspInfoHeader();
    //
    // Copy IBBM to memory and fix FspInfoHeader and ImageBase
    //
    FspMLoadingBase = FspInfoHeader->ImageBase;
    CopyMemSse4(Memory , (VOID *)FspMLoadingBase, CopySize);
    Gap      = (UINT32)Memory - FspMLoadingBase;
    FspInfoHeader = (FSP_INFO_HEADER *)((UINT8 *)FspInfoHeader + Gap);
    FspInfoHeader->ImageBase = (UINT32)Memory;
    SetFspInfoHeader (FspInfoHeader);


    PeiServicesInstallFvInfoPpi (
      NULL,
      (VOID *)Memory,
      PcdGet32 (PcdFlashFvFspmSize),
      NULL,
      NULL
      );

    //
    // locate the TemporaryRamSupport PPI
    //
    Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gEfiTemporaryRamSupportPpiGuid,  // GUID
                               0,                                // INSTANCE
                               &PeiPpiDescriptor,                // EFI_PEI_PPI_DESCRIPTOR
                               NULL                              // PPI
                               );

    if (Status == EFI_SUCCESS) {
      //
      // Reinstall the Variable PPI
      //
      Status = (*PeiServices)->ReInstallPpi (
                                 PeiServices,
                                 PeiPpiDescriptor,
                                 &mPeiTemporaryRamSupportPpi
                                 );
    }


    //
    // Reinstall SecUMA
    //
    SeCUmaEntry(FileHandle, PeiServices);

    Status = PeiServicesInstallPpi (&mPpiBootMode);
    Status = CreateCpuConfigBlocks (&SiCpuPolicyPpi);
    FspUpdatePeiCpuPolicyPreMem (SiCpuPolicyPpi, FspmUpd);
    Status = CpuInstallPolicyPpi (SiCpuPolicyPpi);

    Status = CreateConfigBlocks (&SiSaPolicyPpi);
    FspUpdatePeiSaPolicyPreMem (SiSaPolicyPpi, FspmUpd);
    Status = SiSaInstallPolicyPpi (SiSaPolicyPpi);

    Status = SiCreatePolicyDefaults (&SiPolicyPpi);
    FspUpdatePeiSiPolicyPreMem (SiPolicyPpi, FspmUpd);
    Status = SiInstallPolicyPpi (SiPolicyPpi);

    //
    // Now that all of the pre-permanent memory activities have
    // been taken care of, post a call-back for the permanent-memory
    // resident services, such as HOB construction.
    // PEI Core will switch stack after this PEIM exit.  After that the MTRR
    // can be set.
    //
    Status = PeiServicesNotifyPpi (&mMemoryDiscoveredNotifyList);
    ASSERT_EFI_ERROR (Status);

  } else {

    if (Status == EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    Status = ScCreatePreMemConfigBlocks (&ScPreMemPolicyPpi);
    FspUpdatePeiScPolicyPreMem (ScPreMemPolicyPpi, FspmUpd);
    Status = ScInstallPreMemPolicyPpi (ScPreMemPolicyPpi);

    Status = SaCreatePreMemConfigBlocks (&SiSaPreMemPolicyPpi);
    FspUpdateSaPolicyPreMem (SiSaPreMemPolicyPpi, FspmUpd);
    Status = SaInstallPreMemPolicyPpi (SiSaPreMemPolicyPpi);

    //
    // Get the Boot Mode
    //
    BootMode = FspmUpd->FspmArchUpd.BootMode;
    DEBUG ((DEBUG_INFO, "BootMode : 0x%x\n", BootMode));
    PeiServicesSetBootMode (BootMode);

    //
    // Install required policy for Memory Init
    //
    SeCUmaEntry(FileHandle, PeiServices);

    //
    // Since PEI has no PCI enumerator, set the BAR & I/O space enable ourselves
    //
    ScBaseInit();

    //
    // Program GPIO in Pre-Mem stage.
    //
    FspPlatformGpioProgramPreMem (
      FspmUpd->FspmConfig.PreMemGpioTableEntryNum,
      FspmUpd->FspmConfig.PreMemGpioTablePinNum,
      FspmUpd->FspmConfig.PreMemGpioTablePtr
      );

    //
    // After ScBaseInit(), Check the PlatformID, if invalid then set to Safe_Warning_Value.
    // Safe_Warning_Value for PcdIafwPlatformInfo = Real_Silicon + Max_RevId
    //
    if (PcdGet8 (PcdPlatformIdRegisterOffset) != 0) {
      Temp32 = MmioRead32 (0xFF03A02C);  //Simics PlatId w/a for BXT
    } else {
      Temp32 = MmioRead32 (PcdGet32 (PcdPmcSsramBaseAddress0) + PcdGet8 (PcdPlatformIdRegisterOffset));
    }
    if (Temp32 == 0 || Temp32 == 0xFFFFFFFF) {
      PcdSet32 (PcdIafwPlatformInfo, 0x0000FF00);
      DEBUG ((DEBUG_WARN, "Warning: PcdIafwPlatformInfo set to Safe_Warning_Value\n"));
    } else {
      PcdSet32 (PcdIafwPlatformInfo, Temp32);
    }
    DEBUG ((DEBUG_INFO, "PcdIafwPlatformInfo:0x%X  PlatID:0x%X\n", PcdGet32(PcdIafwPlatformInfo), PLATFORM_ID));

    //
    // Setting 8254
    // Program timer 1 as refresh timer
    //
    IoWrite8 (0x43, 0x54);
    IoWrite8 (0x41, 0x12);

    //
    // RTC power failure handling
    //
    RtcPowerFailureHandler ();

    //
    // Create and install Dram Policy Ppi with DRAM UPD data.
    //
    Status = DramInitializePolicy(&DramPolicyPpi);
    ASSERT_EFI_ERROR (Status);

    MrcNvDataCrc[0] = 0;
    MrcNvDataCrc[1] = 0;
    MrcNvDataCrc[2] = 0;

    //
    // Override MrcTrainingDataPtr and MrcBootDataPtr seetings initialized in DramInitialzePolicy function based on CSE saved data.
    //
    if (FspmUpd->FspmConfig.MrcDataSaving) {
      //
      // Get saved MRC training data
      //
      MRC_NV_DATA_FRAME    *MrcNvData     = NULL;
      UINT32                MrcNvDataSize = 0;
      MrcNvDataSize = GetMrcDataFromSys ((VOID **)&MrcNvData);
      if (MrcNvDataSize > 0x100) {
        DEBUG ((EFI_D_INFO, "Use saved MRC NV data: 0x%08X,  Size: 0x%X\n", (UINT32)MrcNvData, MrcNvDataSize));
        DramPolicyPpi->MrcTrainingDataPtr = (EFI_PHYSICAL_ADDRESS)(UINTN)&MrcNvData->MrcParamsSaveRestore;
        DramPolicyPpi->MrcBootDataPtr     = (EFI_PHYSICAL_ADDRESS)(UINTN)&MrcNvData->BootVariableNvData;
        MrcNvDataCrc[0] = MrcNvData->MrcParamsSaveRestore.MrcParamsSaveRestoreCrc;
        MrcNvDataCrc[1] = MrcNvData->MrcParamsSaveRestore.SaMemCfgCrc;
        MrcNvDataCrc[2] = MrcNvData->BootVariableNvData.BootVariableNvDataCrc;
      }
    }

    Status = DramInstallPolicyPpi (DramPolicyPpi);
    ASSERT_EFI_ERROR (Status);

    Hob.Raw = BuildGuidHob (&gDramPolicyPpiGuid, sizeof(MrcNvDataCrc));
    CopyMem ((VOID *)Hob.Raw, MrcNvDataCrc, sizeof(MrcNvDataCrc));
  }

  DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - End\n"));
  return Status;
}
