/** @file
  Null instance of Sec Platform Hook Lib.

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

//
// The package level header files this module uses
//
#include "PlatformSecLib.h"
#include <CpuRegs.h>


/**
  Perform those platform specific operations that are requried to be executed as early as possibile.

  @return TRUE always return true.
**/
EFI_STATUS
EFIAPI
PlatformSecLibConstructor (
  )
{
  return EFI_SUCCESS;
}

/**
  Backup the FIT table into stack top reserved range.

  @return TRUE always return true.
**/
VOID
EFIAPI
CopyFitTable (
  VOID
)
{
  UINT32            *Src;
  UINT32            *Dst;
  UINT32             Len;
  UINT32             Base;

  /* Copy FIT table */
  Base     = *(UINT32 *)0xFFFFFFC0;
  Len      = (*(UINT32 *)(Base + 8) & 0x00FFFFFF) << 4;
  if (Len < (PcdGet32 (PcdFspReservedBufferSize)- DATA_LEN_AT_TOP_OF_CAR)) {
    Src = (UINT32 *)Base;
    Dst = (UINT32 *)(PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize) - \
                     PcdGet32 (PcdFspReservedBufferSize));
    Len >>= 2;
    while (Len--) {
      *Dst++ = *Src++;
    }
  }
}

/**
  This function check the signture of UPD

  @param[in]  ApiIdx           Internal index of the FSP API.
  @param[in]  ApiParam         Parameter of the FSP API.

**/

EFI_STATUS
EFIAPI
FspUpdSignatureCheck (
  IN UINT32   ApiIdx,
  IN VOID     *ApiParam
  )
{
  EFI_STATUS    Status;
  FSPM_UPD      *FspmUpd;
  FSPS_UPD      *FspsUpd;

  Status = EFI_SUCCESS;
  FspmUpd = NULL;
  FspsUpd = NULL;

  if (ApiIdx == FspMemoryInitApiIndex) {
    //
    // FspMemoryInit check
    //
    FspmUpd = (FSPM_UPD *)ApiParam;
    if (FspmUpd != NULL) {
      if ((FspmUpd->FspUpdHeader.Signature != FSPM_UPD_SIGNATURE)
        || ((UINTN)FspmUpd->FspmArchUpd.StackBase == 0 )
        || (FspmUpd->FspmArchUpd.StackSize < PcdGet32(PcdFspTemporaryRamDataSize))
        || (((UINTN)FspmUpd->FspmArchUpd.StackBase + FspmUpd->FspmArchUpd.StackSize) > BASE_4GB)
        || ((FspmUpd->FspmArchUpd.BootLoaderTolumSize % EFI_PAGE_SIZE) != 0)) {
        Status = EFI_INVALID_PARAMETER;
      }
    }
  } else if (ApiIdx == FspSiliconInitApiIndex) {
    //
    // FspSiliconInit check
    //
    FspsUpd = (FSPS_UPD *)ApiParam;
    if (FspsUpd != NULL) {
      if (FspsUpd->FspUpdHeader.Signature != FSPS_UPD_SIGNATURE) {
        Status = EFI_INVALID_PARAMETER;
      }
    }
  }

  return Status;
}




