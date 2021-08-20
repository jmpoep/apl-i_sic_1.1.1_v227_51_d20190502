/** @file

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/

#ifndef _BL_PLATFORM_DATA_H
#define _BL_PLATFORM_DATA_H

//
// @note
// All lines between tag EXTERNAL_BOOTLOADER_STRUCT_BEGIN and
// EXTERNAL_BOOTLOADER_STRUCT_END will be exported to the generated
// FspUpdVpd.h file by the tool.
//

#pragma pack(1)

/* !EXPORT FSPM EXTERNAL_BOOTLOADER_STRUCT_BEGIN */

#define MAX_CHANNELS_NUM 4
#define MAX_DIMMS_NUM    1

typedef struct {
  UINT8         DimmId;
  UINT32        SizeInMb;
  UINT16        MfgId;
  /** Module part number for DRR3 is 18 bytes
  but DRR4 is 20 bytes as per JEDEC Spec, so
  reserving 20 bytes **/
  UINT8         ModulePartNum[20];
} DIMM_INFO;

typedef struct {
  UINT8         ChannelId;
  UINT8         DimmCount;
  DIMM_INFO     DimmInfo[MAX_DIMMS_NUM];
} CHANNEL_INFO;

typedef struct {
  UINT8         Revision;
  UINT8         DataWidth;
  /** As defined in SMBIOS 3.0 spec
    Section 7.18.2 and Table 75
  **/
  UINT16         MemoryType;
  UINT16        MemoryFrequencyInMHz;
  /** As defined in SMBIOS 3.0 spec
    Section 7.17.3 and Table 72
  **/
  UINT8         ErrorCorrectionType;
  UINT8         ChannelCount;
  CHANNEL_INFO  ChannelInfo[MAX_CHANNELS_NUM];
} FSP_SMBIOS_MEMORY_INFO;

/* !EXPORT FSPM EXTERNAL_BOOTLOADER_STRUCT_END   */

#pragma pack()

#endif
