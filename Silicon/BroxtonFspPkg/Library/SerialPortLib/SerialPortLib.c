/** @file
  Serial I/O Port library functions with no library constructor/destructor

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2016 Intel Corporation.

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/PlatformHookLib.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#include <ScAccess.h>
#include <SaCommonDefinitions.h>

//---------------------------------------------
// UART Register Offsets
//---------------------------------------------
#define STRIDE_SIZE             0x01
#define BAUD_LOW_OFFSET         (STRIDE_SIZE * 0x00)
#define BAUD_HIGH_OFFSET        (STRIDE_SIZE * 0x01)
#define IER_OFFSET              (STRIDE_SIZE * 0x01)
#define LCR_SHADOW_OFFSET       (STRIDE_SIZE * 0x01)
#define FCR_SHADOW_OFFSET       (STRIDE_SIZE * 0x02)
#define IR_CONTROL_OFFSET       (STRIDE_SIZE * 0x02)
#define FCR_OFFSET              (STRIDE_SIZE * 0x02)
#define EIR_OFFSET              (STRIDE_SIZE * 0x02)
#define BSR_OFFSET              (STRIDE_SIZE * 0x03)
#define LCR_OFFSET              (STRIDE_SIZE * 0x03)
#define MCR_OFFSET              (STRIDE_SIZE * 0x04)
#define LSR_OFFSET              (STRIDE_SIZE * 0x05)
#define MSR_OFFSET              (STRIDE_SIZE * 0x06)
#define SCR_OFFSET              (STRIDE_SIZE * 0x07)

//---------------------------------------------
// UART Register Bit Defines
//---------------------------------------------
#define LSR_TXRDY               0x20
#define LSR_RXDA                0x01
#define DLAB                    0x01
#define UART_MAGIC              0x55

UINTN   gBps      = 115200;
UINT8   gData     = 8;
UINT8   gStop     = 1;
UINT8   gParity   = 0;
UINT8   gBreakSet = 0;

UINT32
GetPciUartBase (
  IN FSPM_UPD    *FspmUpd
  )
{
  UINT32  PciUartMmBase;
  UINT16  Cmd16;

  if (FspmUpd->FspmConfig.SerialDebugPortDevice < 3) {
    PciUartMmBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_SC,
                      PCI_DEVICE_NUMBER_LPSS_HSUART,
                      (PCI_FUNCTION_NUMBER_LPSS_HSUART0 + FspmUpd->FspmConfig.SerialDebugPortDevice),
                      0
                      );
    Cmd16 = MmioRead16 (PciUartMmBase + R_LPSS_IO_STSCMD);
    if (Cmd16 == 0xFFFF) {
      //
      // Device might be hidden, assigned temp base address for it
      //
      return LPSS_UART_TEMP_BASE_ADDRESS;
    } else {
      if (MmioRead8 (PciUartMmBase + R_LPSS_IO_STSCMD) & 0x02) {
        return MmioRead32 (PciUartMmBase + R_LPSS_IO_BAR) & 0xFFFFFFF0;
      } else {
        return 0;
      }
    }
  } else {
    return FspmUpd->FspmConfig.SerialDebugPortAddress;
  }
}

UINT8
SerialPortReadRegister (
  IN UINTN    Offset
  )
{
  FSPM_UPD          *FspmUpd;
  UINT8              Data;
  UINT32             Base;
  UINT32             RealOffset;

  FspmUpd = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer();
  Base = GetPciUartBase (FspmUpd);
  if (Base == 0) {
    Data = 0;
  } else {
    RealOffset = Offset << FspmUpd->FspmConfig.SerialDebugPortStrideSize;
    if (FspmUpd->FspmConfig.SerialDebugPortType == 1) {
      Data = IoRead8 (Base + RealOffset);
    } else if (FspmUpd->FspmConfig.SerialDebugPortType == 2) {
      Data = MmioRead8 (Base + RealOffset);
    } else {
      Data = 0xFF;
    }
  }
  return Data;
}

UINT8
SerialPortWriteRegister (
  IN UINTN    Offset,
  IN UINT8    Value
  )
{
  FSPM_UPD          *FspmUpd;
  UINT32             Base;
  UINT32             RealOffset;

  FspmUpd = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer();
  Base = GetPciUartBase (FspmUpd);
  if (Base != 0) {
    RealOffset = Offset << FspmUpd->FspmConfig.SerialDebugPortStrideSize;
    if (FspmUpd->FspmConfig.SerialDebugPortType == 1) {
      return IoWrite8 (Base + RealOffset, Value);
    } else if (FspmUpd->FspmConfig.SerialDebugPortType == 2) {
      return MmioWrite8 (Base + RealOffset, Value);
    } else {
      //
      // not support
      //
    }
  }

  //
  // No data has been written.
  //
  return Value;
}

RETURN_STATUS
EFIAPI
UARTInitialize (
  VOID
  )
{
  UINTN  Divisor;
  UINT8  OutputData;
  UINT8  Data;

  if (SerialPortReadRegister (SCR_OFFSET) != UART_MAGIC) {
    //
    // Calculate divisor for baud generator
    //
    Data = (UINT8) (gData - (UINT8) 5);

    //
    // Calculate divisor for baud generator
    //
    Divisor = 115200 / gBps;

    //
    // Set communications format
    //
    OutputData = (UINT8) (0x83);
    SerialPortWriteRegister (LCR_OFFSET, OutputData);

    //
    // Configure baud rate
    //
    SerialPortWriteRegister (BAUD_HIGH_OFFSET, (UINT8) (Divisor >> 8));
    SerialPortWriteRegister (BAUD_LOW_OFFSET,  (UINT8) (Divisor & 0xff));

    //
    // Switch back to bank 0
    //
    OutputData = (UINT8) (0x03);
    SerialPortWriteRegister (LCR_OFFSET, OutputData);

    SerialPortWriteRegister (IR_CONTROL_OFFSET, 0xc1);

    SerialPortWriteRegister (SCR_OFFSET, UART_MAGIC);
  }

  return RETURN_SUCCESS;
}

/**
  Initialize the serial device hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfully initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval EFI_SUCCESS        The serial device was initialized.
  @retval EFI_DEVICE_ERROR   The serial device could not be initialized.
**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  FSP_GLOBAL_DATA  *FspData;

  //
  // Check if FSP Global Data pointer is intialized or not
  //
  FspData = GetFspGlobalDataPointer ();
  if ((FspData != NULL) && ((UINT32)FspData != 0xFFFFFFFF)) {
    PlatformHookSerialPortInitialize ();
    UARTInitialize ();
  }

  return RETURN_SUCCESS;
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Write data failed.
  @retval !0               Actual number of bytes writed to serial device.
**/
UINTN
EFIAPI
UARTDbgOut (
  IN UINT8    *Buffer,
  IN UINTN    NumberOfBytes
  )
{
  UINTN  Result;
  UINT8  Data;

  if (NULL == Buffer) {
    return 0;
  }
  if (GetPciUartBase (GetFspMemoryInitUpdDataPointer ()) == 0) {
    //
    // Device may be disabled, return to avoid looping
    //
    return 0;
  }

  Result = NumberOfBytes;

  while (NumberOfBytes--) {
    //
    // Wait for the serail port to be ready.
    //
    do {
      Data = SerialPortReadRegister (LSR_OFFSET);
    } while ((Data & LSR_TXRDY) == 0);
    SerialPortWriteRegister (0, *Buffer++);
  }

  return Result;
}

/**
  Write data from buffer to serial device.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.

  If Buffer is NULL, then ASSERT().

  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8    *Buffer,
  IN UINTN    NumberOfBytes
  )
{
  return UARTDbgOut (Buffer, NumberOfBytes);
}

UINTN
EFIAPI
UARTDbgIn (
  OUT UINT8    *Buffer,
  IN  UINTN    NumberOfBytes
  )
{
  UINTN Result;
  UINT8 Data;

  if (NULL == Buffer) {
    return 0;
  }
  if (GetPciUartBase (GetFspMemoryInitUpdDataPointer ()) == 0) {
    //
    // Device may be disabled, return to avoid looping
    //
    return 0;
  }

  Result = NumberOfBytes;

  while (NumberOfBytes--) {
    //
    // Wait for the serial port to be ready.
    //
    do {
      Data = SerialPortReadRegister (LSR_OFFSET);
    } while ((Data & LSR_RXDA) == 0);

    *Buffer++ = SerialPortReadRegister (0);
  }

  return Result;
}

/**
  Reads data from a serial device into a buffer.

  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
  @param  NumberOfBytes    Number of bytes to read from the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8 *Buffer,
  IN UINTN  NumberOfBytes
  )
{
  return UARTDbgIn (Buffer, NumberOfBytes);
}

BOOLEAN
EFIAPI
UARTDbgPoll (
  VOID
  )
{
  //
  // Read the serial port status
  //
  if ((SerialPortReadRegister (LSR_OFFSET) & LSR_RXDA) != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls aserial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.
**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  BOOLEAN Status;

  Status = FALSE;
  Status |= UARTDbgPoll ();

  return Status;
}

