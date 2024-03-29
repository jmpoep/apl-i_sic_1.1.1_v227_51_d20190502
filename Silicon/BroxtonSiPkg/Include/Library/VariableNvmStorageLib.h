/** @file
  Variable NVM Storage Helper library

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation.

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
#ifndef _VARIABLE_NVM_STORAGE_LIB_H_
#define _VARIABLE_NVM_STORAGE_LIB_H_

#include <Uefi.h>
#include <Guid/VariableFormat.h>
#include <VariableNvmStorageFormat.h>

/**
  Returns a pointer of type VARIABLE_NVM_STORE_HEADER to a buffer containing
  the header of an empty variable store.

  @param VarStoreHeader  Pointer to the Variable Store Header.

  @retval EFI_SUCCESS    The variable store was created successfully.
  @retval Others         An error occurred creating the variable store.

**/
EFI_STATUS
CreateEmptyVariableStore (
  OUT  VARIABLE_NVM_STORE_HEADER  *VarStoreHeader
  );


/**
  Gets the pointer to the first variable header in the given variable header region.

  @param VarStoreHeader  Pointer to the Variable Store Header.

  @return Pointer to the first variable header

**/
VARIABLE_NVM_HEADER *
GetStartPointer (
  IN  CONST VARIABLE_NVM_STORE_HEADER  *VarStoreHeader
  );


/**
  This code gets the pointer to the last variable header in the variable header region.

  @param  VarStoreHeader  Pointer to the Variable Store Header.

  @return VARIABLE_NVM_HEADER* pointer to last unavailable Variable Header.

**/
VARIABLE_NVM_HEADER *
GetEndPointer (
  IN  CONST VARIABLE_NVM_STORE_HEADER  *VarStoreHeader
  );


/**
  This code checks if the variable header is valid or not.

  @param  Variable          Pointer to the Variable Header.
  @param  VariableStoreEnd  Pointer to the end of the Variable Store

  @retval TRUE      Variable header is valid.
  @retval FALSE     Variable header is not valid.

**/
BOOLEAN
IsValidVariableHeader (
  IN  CONST VARIABLE_NVM_HEADER  *Variable,
  IN  CONST VARIABLE_NVM_HEADER  *VariableStoreEnd
  );

/**
  This code checks if the variable header is valid or not.

  @param  VarStoreHeader        Pointer to the Variable Store Header.
  @param  IsAuthenticatedStore  Indicates whether this store is authenticated.

  @retval EFI_SUCCESS           The authenticated status of the store was successfully determined.
  @retval EFI_INVALID_PARAMETER An invalid parameter was passed to the function.

**/
EFI_STATUS
IsAuthenticatedVariableStore (
  IN  CONST VARIABLE_NVM_STORE_HEADER  *VarStoreHeader,
  OUT       BOOLEAN                    *IsAuthenticatedStore
  );

/**
  This code gets the size of the variable header.

  @param AuthFlag   Authenticated variable flag.

  @return Size of variable header in bytes in type UINTN.

**/
UINTN
GetVariableHeaderSize (
  IN  BOOLEAN       AuthFlag
  );


/**
  This code gets the size of the name of the variable.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
UINTN
NameSizeOfVariable (
  IN  CONST VARIABLE_NVM_HEADER  *Variable,
  IN        BOOLEAN              AuthFlag
  );


/**
  This code gets the size of the variable data.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
UINTN
DataSizeOfVariable (
  IN  CONST VARIABLE_NVM_HEADER  *Variable,
  IN        BOOLEAN              AuthFlag
  );


/**
  This code gets the pointer to the variable name.

  @param   Variable  Pointer to the Variable Header.
  @param   AuthFlag  Authenticated variable flag.

  @return  A CHAR16* pointer to Variable Name.

**/
CHAR16 *
GetVariableNamePtr (
  IN  CONST VARIABLE_NVM_HEADER  *Variable,
  IN        BOOLEAN              AuthFlag
  );


/**
  This code gets the pointer to the variable guid.

  @param Variable   Pointer to the Variable Header.
  @param AuthFlag   Authenticated variable flag.

  @return A EFI_GUID* pointer to Vendor Guid.

**/
CONST EFI_GUID *
GetVendorGuidPtr (
  IN  CONST VARIABLE_NVM_HEADER  *Variable,
  IN        BOOLEAN              AuthFlag
  );


/**
  This code gets the pointer to the variable data.

  This is an "offset" that is interpreted as needed based on the caller's requirements.

  @param   Variable         Pointer to the Variable Header.
  @param   StoreInfo        Pointer to a Variable Store Info structure for this variable.

  @return  A UINT32 value representing the offset to the variable data.

**/
UINT32
GetVariableDataPtr (
  IN  CONST VARIABLE_NVM_HEADER     *Variable,
  IN        VARIABLE_NVM_STORE_INFO *StoreInfo
  );


/**
  This code gets the pointer to the next variable header.

  @param  StoreInfo         Pointer to variable store info structure.
  @param  VariableHeader    Pointer to the Variable Header.

  @return  A VARIABLE_NVM_HEADER* pointer to next variable header.

**/
VARIABLE_NVM_HEADER *
GetNextVariablePtr (
  IN  CONST VARIABLE_NVM_STORE_INFO  *StoreInfo,
  IN  CONST VARIABLE_NVM_HEADER      *VariableHeader
  );


/**
  Get variable store status.

  @param  VarStoreHeader  Pointer to the Variable Store Header.

  @retval  EfiRaw      Variable store is raw
  @retval  EfiValid    Variable store is valid
  @retval  EfiInvalid  Variable store is invalid

**/
VARIABLE_STORE_STATUS
GetVariableStoreStatus (
  IN  CONST VARIABLE_NVM_STORE_HEADER  *VarStoreHeader
  );


/**
  Compare two variable names.

  @param StoreInfo      Pointer to variable store info structure.
  @param Name1          Pointer to one variable name.
  @param Name2          Pointer to another variable name.
  @param NameSize       Variable name size.

  @retval TRUE          Name1 and Name2 are identical.
  @retval FALSE         Name1 and Name2 are not identical.

**/
BOOLEAN
CompareVariableName (
  IN  CONST VARIABLE_NVM_STORE_INFO  *StoreInfo,
  IN  CONST CHAR16                   *Name1,
  IN  CONST CHAR16                   *Name2,
  IN        UINTN                    NameSize
  );


/**
  This function compares a variable with the variable entry in the database.

  @param  StoreInfo      Pointer to variable store info structure.
  @param  Variable       Pointer to the variable in our database
  @param  VariableHeader Pointer to the Variable Header that has consecutive content.
  @param  VariableName   Name of the variable to compare to 'Variable'
  @param  VendorGuid     GUID of the variable to compare to 'Variable'
  @param  PtrTrack       Variable Track Pointer structure that contains Variable Information.

  @retval EFI_SUCCESS    Found match variable
  @retval EFI_NOT_FOUND  Variable not found

**/
EFI_STATUS
CompareWithValidVariable (
  IN  CONST VARIABLE_NVM_STORE_INFO     *StoreInfo,
  IN        VARIABLE_NVM_HEADER         *Variable,
  IN  CONST VARIABLE_NVM_HEADER         *VariableHeader,
  IN  CONST CHAR16                      *VariableName,
  IN  CONST EFI_GUID                    *VendorGuid,
  OUT       VARIABLE_NVM_POINTER_TRACK  *PtrTrack
  );


/**
  Validate the provided variable header.

  @param StoreInfo      Pointer to variable store info structure.
  @param Variable       Pointer to the Variable Header.
  @param VariableHeader Pointer to Pointer to the Variable Header that has consecutive content.

  @retval TRUE          Variable header is valid.
  @retval FALSE         Variable header is not valid.

**/
BOOLEAN
GetVariableHeader (
  IN  CONST VARIABLE_NVM_STORE_INFO   *StoreInfo,
  IN        VARIABLE_NVM_HEADER       *Variable,
  OUT       VARIABLE_NVM_HEADER       **VariableHeader
  );

/**
  Compares two EFI_TIME data structures.

  @param   FirstTime   A pointer to the first EFI_TIME data.
  @param   SecondTime  A pointer to the second EFI_TIME data.

  @retval  TRUE        The FirstTime is not later than the SecondTime.
  @retval  FALSE       The FirstTime is later than the SecondTime.

**/
BOOLEAN
VariableNvmCompareTimeStamp (
  IN  CONST EFI_TIME  *FirstTime,
  IN  CONST EFI_TIME  *SecondTime
  );

#endif
