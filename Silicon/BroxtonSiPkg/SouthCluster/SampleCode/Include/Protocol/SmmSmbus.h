/**  @file
  SmmSmbus Protocol

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification
**/
#ifndef __EFI_SMM_SMBUS_PROTOCOL_H__
#define __EFI_SMM_SMBUS_PROTOCOL_H__

#define EFI_SMM_SMBUS_PROTOCOL_GUID \
  { \
    0x72e40094, 0x2ee1, 0x497a, \
    { \
      0x8f, 0x33, 0x4c, 0x93, 0x4a, 0x9e, 0x9c, 0xc \
    } \
  }


//
// Resuse the DXE definition, and use another GUID.
//
typedef EFI_SMBUS_HC_PROTOCOL  SMM_SMBUS_HC_PROTOCOL;

extern EFI_GUID               gEfiSmmSmbusProtocolGuid;

#endif