/** @file
  Hob guid for Information about the FSP performance measurement data.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

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

@par Specification
**/

#ifndef _PERF_INFO_HOB_GUID_H_
#define _PERF_INFO_HOB_GUID_H_

#define EFI_PEI_PERF_INFO_HOB_GUID \
  { \
    0xeefd49d9, 0x64dc, 0x4a35, { 0xa9, 0xfe, 0x88, 0xfd, 0xe7, 0x8b, 0x94, 0xa2 } \
  }

#pragma pack(1)

typedef struct {
  UINT32             PerfEntryNum;
  UINT64             PerfData[];
} EFI_PEI_PERF_INFO_HOB;

#pragma pack()

extern EFI_GUID gEfiPerformanceInfoHobGuid;

#endif
