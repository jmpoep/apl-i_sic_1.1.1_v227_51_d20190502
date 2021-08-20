/**@file

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

#include <PiPei.h>
#include <SmipGenerated.h>
#include <Ppi/DramPolicyPpi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>

//
// ************************************************************
//
// (@todo): This needs to be updated!
//
// FSP should not directly read SMIP. This breaks with the new
// UEFI variable solution.
//
// ************************************************************
//

#define COUNT_LIMIT 0x13   //reasonable limit for smipblock loop

/* Define CSE registers */
#define  CSE2HOST               0x60
#define  PCIE_MMCFG_BASE        0xE0000000
#define  CSE_CFG_BASE           (PCIE_MMCFG_BASE + 0x78000)
#define  CSE_IVD                (1 << 10)
#define  CSE_CSE2HOST           (*(volatile UINT32 *)(CSE_CFG_BASE + CSE2HOST))
#pragma pack(push, 1)
typedef struct {
  UINT64  Address;
  UINT32  Size:24;
  UINT32  Rsvd:8;
  UINT16  Version;
  UINT8   Type:7;
  UINT8   C_V:1;
  UINT8   Checksum;
} FitEntry;

typedef struct {
  UINT16  BlockCount;
  UINT16  TotalSize;
} SmipHeader;

typedef struct {
  UINT16  BlockType;   //0=CSE, 1=PMC, 2=IAFW
  UINT16  BlockOffset;
  UINT16  BlockLength;
  UINT16  Reserved;
} SmipBlockHeader;
#pragma pack(pop)

UINT32
GetMrcDataFromSys (
 IN  VOID **Buffer
)
{
  UINT16                            index;
  EFI_STATUS                        Status;
  SmipHeader                       *SmipHeadPointer;
  SmipBlockHeader                  *SmipBlockPtr;
  VOID                             *MrcTrainingDataPtr = NULL;
  FitEntry                         *FitHeader, *FitEntries;
  VOID                             *SystemConfigDataPtr;
  UINTN                             SystemConfigDataSize;
  UINTN                             MrcTrainingDataSize;


  DEBUG ((EFI_D_INFO, "GetMrcDataFromSysCfg() - Start\n"));

  Status = EFI_SUCCESS;
  FitHeader = (FitEntry *)(PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize) - PcdGet32 (PcdFspReservedBufferSize));

  DEBUG ((EFI_D_INFO, "PcdTemporaryRamBase          = 0x%x\n", PcdGet32 (PcdTemporaryRamBase) ));
  DEBUG ((EFI_D_INFO, "PcdTemporaryRamSize          = 0x%x\n", PcdGet32 (PcdTemporaryRamSize) ));
  DEBUG ((EFI_D_INFO, "PcdFspReservedBufferSize     = 0x%x\n", PcdGet32 (PcdFspReservedBufferSize)));
  FitEntries = FitHeader;

  DEBUG ((EFI_D_INFO, "Checking FitHeader at 0x%08x\n", FitHeader ));
  if (FitHeader->Address != SIGNATURE_64 ('_', 'F', 'I', 'T', '_', ' ', ' ', ' ')) {
    DEBUG ((EFI_D_ERROR, "FitHeader signature was invalid.\n"));
    return EFI_NOT_FOUND;
  }
  DEBUG ((EFI_D_INFO, "FitHeader signature verified.\n"));

  //
  //Loop through FIT Entries until we find SMIP (start at 1 to skip header)
  //
  DEBUG ((EFI_D_INFO, "Searching for SMIP Entry in FIT...\n" ));

  //
  // Poll IBB Verification Done (IVD) bit to wait for the SMIP data is loaded by CSE
  //
  DEBUG ((EFI_D_INFO, "Waiting for SMIP is loaded, CSE2HOST = %x\n", CSE_CSE2HOST));
  while ((CSE_CSE2HOST & CSE_IVD) != CSE_IVD);

  SmipHeadPointer     = NULL;
  SmipBlockPtr        = NULL;
  SystemConfigDataPtr = NULL;
  SystemConfigDataSize = 0;
  for (index=1; index < FitHeader->Size; index++) {
    if ( FitEntries[index].Type == 0x10 && FitEntries[index].Rsvd == 0x5) {
      DEBUG ((EFI_D_INFO, "Found SMIP Entry in FIT.\n" ));
      SmipHeadPointer = (SmipHeader*)(UINTN)(FitEntries[index].Address);
      SmipBlockPtr = (SmipBlockHeader*)((UINT8*)SmipHeadPointer + sizeof(SmipHeadPointer));
      DEBUG ((EFI_D_INFO, "SMIP table located at: 0x%08x\n", SmipHeadPointer));
      DEBUG ((EFI_D_INFO, "SMIP table size = 0x%08x bytes\n", SmipHeadPointer->TotalSize));
    } else if ( FitEntries[index].Type == 0x10 && FitEntries[index].Rsvd == 0x6) {
      DEBUG ((EFI_D_INFO, "Found System Configuration Data Entry in FIT.\n" ));
      SystemConfigDataPtr = (VOID*)(UINTN)(FitEntries[index].Address);
      SystemConfigDataSize = FitEntries[index].Size;
    }
  }

  if (SmipHeadPointer == NULL) {
    DEBUG ((EFI_D_ERROR, "No SMIP Entry found in FIT\n"));
  } else {
    //
    // Loop through SMIP Header and look for IAFW SMIP entry
    //
    if (SystemConfigDataPtr == NULL) {
      DEBUG ((EFI_D_INFO, "Couldn't get memory config data\n"));
    } else {
      MrcTrainingDataSize = SystemConfigDataSize;
      MrcTrainingDataPtr  = SystemConfigDataPtr;
      DEBUG ((EFI_D_INFO, "Found memory config data at 0x%08X with size 0x%x\n", (UINT32)MrcTrainingDataPtr, MrcTrainingDataSize));
    }
  }

  if (Buffer) {
    *Buffer = MrcTrainingDataPtr;
  }

  return MrcTrainingDataSize;
}

EFI_STATUS
GetMrcDataFromSysCfg (
 IN  DRAM_POLICY_PPI *DramPolicy
 )
{
  EFI_STATUS  Status;
  VOID       *MrcTrainingDataPtr;
  UINT32      MrcTrainingDataSize;

  DEBUG ((EFI_D_INFO, "GetMrcDataFromSysCfg() - Start\n"));

  MrcTrainingDataSize = GetMrcDataFromSys ((VOID **)&MrcTrainingDataPtr);

  if (DramPolicy && MrcTrainingDataSize) {
    DramPolicy->MrcTrainingDataPtr = (EFI_PHYSICAL_ADDRESS)(UINT32) MrcTrainingDataPtr;
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_NOT_FOUND;
  }
  DEBUG ((EFI_D_INFO, "GetMrcDataFromSysCfg() - End\n"));
  return Status;
}

