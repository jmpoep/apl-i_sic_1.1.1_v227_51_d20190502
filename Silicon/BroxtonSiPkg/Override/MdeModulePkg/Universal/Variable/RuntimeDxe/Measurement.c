/** @file
  Measure TrEE required variable.

Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Guid/ImageAuthentication.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Protocol/TrEEProtocol.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/TpmMeasurementLib.h>

typedef struct {
  CHAR16                                 *VariableName;
  EFI_GUID                               *VendorGuid;
} VARIABLE_TYPE;

VARIABLE_TYPE  mVariableType[] = {
  {EFI_SECURE_BOOT_MODE_NAME,    &gEfiGlobalVariableGuid},
  {EFI_PLATFORM_KEY_NAME,        &gEfiGlobalVariableGuid},
  {EFI_KEY_EXCHANGE_KEY_NAME,    &gEfiGlobalVariableGuid},
  {EFI_IMAGE_SECURITY_DATABASE,  &gEfiImageSecurityDatabaseGuid},
  {EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid},
};

/**
  This function will return if this variable is SecureBootPolicy Variable.

  @param[in]  VariableName      A Null-terminated string that is the name of the vendor's variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.

  @retval TRUE  This is SecureBootPolicy Variable
  @retval FALSE This is not SecureBootPolicy Variable
**/
BOOLEAN
IsSecureBootPolicyVariable (
  IN CHAR16                                 *VariableName,
  IN EFI_GUID                               *VendorGuid
  )
{
  UINTN   Index;

  for (Index = 0; Index < sizeof(mVariableType)/sizeof(mVariableType[0]); Index++) {
    if ((StrCmp (VariableName, mVariableType[Index].VariableName) == 0) &&
        (CompareGuid (VendorGuid, mVariableType[Index].VendorGuid))) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Measure and log an EFI variable, and extend the measurement result into a specific PCR.

  @param[in]  VarName           A Null-terminated string that is the name of the vendor's variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.
  @param[in]  VarData           The content of the variable data.
  @param[in]  VarSize           The size of the variable data.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureVariable (
  IN      CHAR16                    *VarName,
  IN      EFI_GUID                  *VendorGuid,
  IN      VOID                      *VarData,
  IN      UINTN                     VarSize
  )
{
  EFI_STATUS                        Status;
  UINTN                             VarNameLength;
  EFI_VARIABLE_DATA_TREE            *VarLog;
  UINT32                            VarLogSize;

  ASSERT ((VarSize == 0 && VarData == NULL) || (VarSize != 0 && VarData != NULL));

  VarNameLength      = StrLen (VarName);
  VarLogSize = (UINT32)(sizeof (*VarLog) + VarNameLength * sizeof (*VarName) + VarSize
                        - sizeof (VarLog->UnicodeName) - sizeof (VarLog->VariableData));

  VarLog = (EFI_VARIABLE_DATA_TREE *) AllocateZeroPool (VarLogSize);
  if (VarLog == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (&VarLog->VariableName, VendorGuid, sizeof(VarLog->VariableName));
  VarLog->UnicodeNameLength  = VarNameLength;
  VarLog->VariableDataLength = VarSize;
  CopyMem (
     VarLog->UnicodeName,
     VarName,
     VarNameLength * sizeof (*VarName)
     );
  if (VarSize != 0) {
    CopyMem (
       (CHAR16 *)VarLog->UnicodeName + VarNameLength,
       VarData,
       VarSize
       );
  }

  DEBUG ((EFI_D_INFO, "AuthVariableDxe: MeasureVariable (Pcr - %x, EventType - %x, ", (UINTN)7, (UINTN)EV_EFI_VARIABLE_AUTHORITY));
  DEBUG ((EFI_D_INFO, "VariableName - %s, VendorGuid - %g)\n", VarName, VendorGuid));

  Status = TpmMeasureAndLogData (
             7,
             EV_EFI_VARIABLE_DRIVER_CONFIG,
             VarLog,
             VarLogSize,
             VarLog,
             VarLogSize
             );
  FreePool (VarLog);
  return Status;
}

/**
  Returns the status whether get the variable success. The function retrieves
  variable  through the UEFI Runtime Service GetVariable().  The
  returned buffer is allocated using AllocatePool().  The caller is responsible
  for freeing this buffer with FreePool().

  This API is only invoked in boot time. It may NOT be invoked at runtime.

  @param[in]  Name  The pointer to a Null-terminated Unicode string.
  @param[in]  Guid  The pointer to an EFI_GUID structure
  @param[out] Value The buffer point saved the variable info.
  @param[out] Size  The buffer size of the variable.

  @return EFI_OUT_OF_RESOURCES      Allocate buffer failed.
  @return EFI_SUCCESS               Find the specified variable.
  @return Others Errors             Return errors from call to gRT->GetVariable.

**/
EFI_STATUS
InternalGetVariable (
  IN CONST CHAR16    *Name,
  IN CONST EFI_GUID  *Guid,
  OUT VOID           **Value,
  OUT UINTN          *Size
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;

  //
  // Try to get the variable size.
  //
  BufferSize = 0;
  *Value     = NULL;
  if (Size != NULL) {
    *Size  = 0;
  }

  Status = gRT->GetVariable ((CHAR16 *) Name, (EFI_GUID *) Guid, NULL, &BufferSize, *Value);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  //
  // Allocate buffer to get the variable.
  //
  *Value = AllocatePool (BufferSize);
  ASSERT (*Value != NULL);
  if (*Value == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get the variable data.
  //
  Status = gRT->GetVariable ((CHAR16 *) Name, (EFI_GUID *) Guid, NULL, &BufferSize, *Value);
  if (EFI_ERROR (Status)) {
    FreePool(*Value);
    *Value = NULL;
  }

  if (Size != NULL) {
    *Size = BufferSize;
  }

  return Status;
}

/**
  SecureBoot Hook for SetVariable.

  @param[in] VariableName                 Name of Variable to be found.
  @param[in] VendorGuid                   Variable vendor GUID.

**/
VOID
EFIAPI
SecureBootHook (
  IN CHAR16                                 *VariableName,
  IN EFI_GUID                               *VendorGuid
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableDataSize;
  VOID                              *VariableData;

  if (!IsSecureBootPolicyVariable (VariableName, VendorGuid)) {
    return ;
  }

  //
  // We should NOT use Data and DataSize here,because it may include signature,
  // or is just partial with append attributes, or is deleted.
  // We should GetVariable again, to get full variable content.
  //
  Status = InternalGetVariable (
             VariableName,
             VendorGuid,
             &VariableData,
             &VariableDataSize
             );
  if (EFI_ERROR (Status)) {
    VariableData     = NULL;
    VariableDataSize = 0;
  }

  Status = MeasureVariable (
             VariableName,
             VendorGuid,
             VariableData,
             VariableDataSize
             );
  DEBUG ((EFI_D_INFO, "MeasureBootPolicyVariable - %r\n", Status));

  if (VariableData != NULL) {
    FreePool (VariableData);
  }

  return ;
}
