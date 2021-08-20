/**@file
  GUID used for Platform ConfigDataGuid.
 @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2016 Intel Corporation.

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

#ifndef __PLATFORM_CONFIG_DATA_GUID_H__
#define __PLATFORM_CONFIG_DATA_GUID_H__

#define PLATFORM_CONFIG_DATA_GUID \
  {\
    0x3c029123, 0x2638, 0x43fc, { 0x85, 0x7d, 0xc3, 0xab, 0xbc, 0x1f, 0x71, 0x6c } \
  }

#define PLATFORM_SYSTEM_CONFIG_DATA_GUID \
  {\
    0xef927760, 0xa05d, 0x419c, { 0x92, 0xe1, 0x12, 0x6b, 0xa9, 0x2d, 0x8c, 0xcd } \
  }

#define PLATFORM_MEMORY_CONFIG_DATA_GUID \
  {\
    0xfce67798, 0xb347, 0x4c4c, { 0xa1, 0xcf, 0xd, 0x85, 0x90, 0xcd, 0x25, 0xc6 } \
  }

#define PLATFORM_SMIP_CONFIG_DATA_GUID \
  {\
    0x2bedcf0e, 0x633f, 0x4415, { 0xb7, 0x57, 0xad, 0x11, 0x31, 0x79, 0x92, 0x61 } \
  }

extern EFI_GUID gPlatformConfigDataGuid;
extern EFI_GUID gPlatformSystemConfigDataGuid;
extern EFI_GUID gPlatformMemoryConfigDataGuid;
extern EFI_GUID gPlatformSmipConfigDataGuid;

#endif
