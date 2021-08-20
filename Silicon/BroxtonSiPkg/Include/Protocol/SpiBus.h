/** @file
  MMIO device protocol as defined in the UEFI 2.x.x specification.

  The MMIO device protocol defines a memory mapped I/O device
  for use by the system.

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

#ifndef __SPI_BUS_H__
#define __SPI_BUS_H__

//#include <Protocol/SpiHost.h>
//#include <Library/SpiTargetSettings.h>
//#include <Guid/EventGroup.h>
//#include <Library/SpiTargetSettings.h>

//typedef
//EFI_STATUS
//(EFIAPI *EFI_SPI_ACPI_PROTOCOL) (
//  IN CONST EFI_SPI_ACPI_PROTOCOL *This,
//  OUT CONST EFI_SPI_DEVICE **Device
//  );

///
///  EFI SPI Device
///
//typedef struct {
//  UINT8 Index;
//  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
//  SPI_TARGET_SETTINGS *targetSettings;
//} EFI_SPI_DEVICE;
//
/////
/////  SPI PLATFORM CONTEXT
/////
//typedef struct {
//  UINTN BaseAddress;
//  UINT64 InputFrequencyHertz;
//} SPI_PLATFORM_CONTEXT;

//typedef (EFI_STATUS EFIAPI) EFI_SPI_ACPI_PROTOCOL;
typedef struct _EFI_SPI_BUS_PROTOCOL EFI_SPI_BUS_PROTOCOL;

typedef
EFI_STATUS (EFIAPI* SPI_BUS_STARTREQUEST)(
  IN EFI_SPI_BUS_PROTOCOL *This,
  IN EFI_EVENT Event OPTIONAL,
  IN EFI_SPI_REQUEST_PACKET *RequestPacket,
  OUT EFI_STATUS *SpiStatus OPTIONAL
  );

typedef
UINTN
(EFIAPI* SPI_BUS_READ)(
  IN OUT UINT8   *Buffer,
  IN  UINTN   NumberOfBytes
  );


typedef
UINTN
(EFIAPI* SPI_BUS_WRITE)(
  IN OUT UINT8  *Buffer,
  IN UINTN  NumberOfBytes
  );


struct _EFI_SPI_BUS_PROTOCOL {
  SPI_BUS_STARTREQUEST StartRequest;
  SPI_BUS_READ SPIRead;
  SPI_BUS_WRITE SPIWrite;
  UINT32 MaximumReceiveBytes;
  UINT32 MaximumTransmitBytes;
  UINT32 MaximumTotalBytes;
};

extern EFI_GUID gEfiSpiBusProtocolGuid;




#endif  //  __SPI_BUS_H__
