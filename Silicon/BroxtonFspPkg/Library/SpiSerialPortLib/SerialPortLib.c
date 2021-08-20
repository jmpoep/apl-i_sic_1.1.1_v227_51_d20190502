/*++

Copyright (c) 2012 - 2015 Intel Corporation. All rights reserved
All rights reserved. This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

Module Name:

  SerialPortLib.c

Abstract:

  Contains Platform specific implementations required to use status codes.

--*/

/** @file
  Serial Port Library for Ultra Mobile SPI based UART implementations

  This file implements the support for the Maxim
  <a href="http://datasheets.maxim-ic.com/en/ds/MAX3110E-MAX3111E.pdf">MAX311xE</a>
  SPI/UART.
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/SerialPortLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <ScRegs/RegsLpss.h>
#include <PlatformBaseAddresses.h>
#include <ScAccess.h>


STATIC UINT32 mSpi1BaseAddress = SPI_TEMP_MEM_BASE_ADDRESS;

#define    LPSS_SPI_BASE_ADDRESS            mSpi1BaseAddress
#define    SPI_REG_SPACE_ADDR_BASE          (LPSS_SPI_BASE_ADDRESS & B_LPSS_IO_BAR_BA)
#define    R_SPI_SSCR0                      (0x00)          // Control Register 0
#define     V_SPI_SSCR0_MOD                 (BIT31)         // Mode Set to 0 - Normal SSP Mode : Full Duplex Serial peripheral interface. 1 = reserved
#define     V_SPI_SSCR0_MOD_NORMAL          (0 << 31)       // Mode Set to 0 - Normal SSP Mode : Full Duplex Serial peripheral interface.
#define     V_SPI_SSCR0_ACS                 (BIT30)         // Audio Clock Select Set to 0 for Clock selection which is determined by the NCS and ECS bits in this register. 1 = reserved
#define     V_SPI_SSCR0_ACS_NCS_ECS         (0 << 30)       // Set to 0 for Clock selection which is determined by the NCS and ECS bits in this register.
#define     V_SPI_SSCR0_TIM                 (BIT23)         // Transmit FIFO Under Run Interrupt Mask
#define     V_SPI_SSCR0_RIM                 (BIT22)         // Receive FIFO Over Run Interrupt Mask
#define     V_SPI_SSCR0_NCS_MASK            (BIT21)         // Network Clock Select The SSCR0.NCS bit in conjunction with SSCR0.ECS determines which clock is used.
#define     V_SPI_SSCR0_NCS_ECS             (0 << 21)       // 0 - Clock selection is determined by ECS bit
#define     V_SPI_SSCR0_SCR_MASK            (0x3FF00)       // [19:8] Serial Clock Rate Value used to generate transmission rate of SSP.
#define     V_SPI_SSCR0_SSE                 (BIT7)          // Synchronous Serial Port Enable
#define     V_SPI_SSCR0_ECS_MASK            (BIT6)          // External Clock Select
#define     V_SPI_SSCR0_ECS_ONCHIP          (0 <<6)         // 0 = use On-chip clock (output of M/N Divider) to produce the SSP's serial clock (SSPSCLK).
#define     V_SPI_SSCR0_FRF_MASK            (BIT4 | BIT5)   // Frame Format
#define     V_SPI_SSCR0_FRF_MOTOROLA        (0 << 4)        // Set to 00 - Motorola Serial Peripheral Interface (SPI)
#define     V_SPI_SSCR0_DSS_MASK            (0x0F)          // Data Size Select With EDSS as MSB
#define     V_SPI_SSCR0_DSS_4               (0x03)          // Data Size Select With EDSS as MSB, 4 bits
#define     V_SPI_SSCR0_DSS_8               (0x07)          // Data Size Select With EDSS as MSB, 8 bits
#define     V_SPI_SSCR0_DSS_16_32           (0x0F)          // Data Size Select With EDSS as MSB, 16/32 bits
#define    R_SPI_SSCR1                      (0x04)          // Control Register 1
#define    R_SPI_SSSR                       (0x08)          // Status Register
#define     V_SPI_SSSR_RFS                  (BIT6)          // BSY (SSI Busy Flag)
#define     V_SPI_SSSR_BSY                  (BIT4)          // BSY (SSI Busy Flag)
#define     V_SPI_SSSR_RNE                  (BIT3)          // (Receive FIFO Not Empty)
#define     V_SPI_SSSR_TNF                  (BIT2)          // (Transmit FIFO Not Full)
#define    R_SPI_SSITR                      (0x0C)          // Interrupt Test register
#define    R_SPI_SSDR                       (0x10)          // Data Register
#define    R_SPI_SSTO                       (0x28)          // Time Out Register
#define    R_SPI_SITF                       (0x44)          // Transmit FIFO Threshold Register
#define    R_SPI_SIRF                       (0x48)          // Receive FIFO Threshold Register
#define    R_SPI_CLOCK_PARAMS               (0x200)         // Clock Parameter Register
#define     V_SPI_CLOCK_PARAMS_CLK_UPDATE   (BIT31)         // 0 - No clock Update; 1 - Clock gets updated.
#define     V_SPI_CLOCK_PARAMS_BIT16        (BIT16)         // N_VAL
#define     V_SPI_CLOCK_PARAMS_BIT4         (BIT4)          // M_VAL
#define     V_SPI_CLOCK_PARAMS_BIT1         (BIT1)          // M_VAL
#define     V_SPI_CLOCK_PARAMS_N_VAL_MASK   (0x7FFF0000)    // The numerator value (N) for the M over N divider logic that creates the SPI_CLK_OUT for the SSP.
#define     V_SPI_CLOCK_PARAMS_M_VAL_MASK   (0xFFFE)        // The numerator value (M) for the M over N divider logic that creates the SPI_CLK_OUT for the SSP.
#define     V_SPI_CLOCK_PARAMS_CLK_EN       (BIT0)          // clk_en
#define    R_SPI_CS_CONTROL                 (0x224)         // CS Control Register
#define     V_SPI_CS_CONTROL_MASK           (BIT8 | BIT9)
#define     V_SPI_CS_CONTROL_CS0            (0x00)
#define     V_SPI_CS_CONTROL_CS1            (BIT8)
#define     V_SPI_CS_CONTROL_CS2            (BIT9)
#define     V_SPI_CS_CONTROL_CS3            (BIT8 | BIT9)
#define    R_LPSS_IO_REMAP_ADDRESS_LOW      0x240           // Low 32 bits of BAR address read by SW from BAR Low CFG Offset 0x10

#define SPI_BASE            SPI_REG_SPACE_ADDR_BASE         // Base Address of SPI
#define SPI_SSCR0           (SPI_BASE + R_SPI_SSCR0)        // Control Register 0
#define SPI_SSCR1           (SPI_BASE + R_SPI_SSCR1)        // Control Register 1
#define SPI_SSSR            (SPI_BASE + R_SPI_SSSR)         // Status Register
#define SPI_SSITR           (SPI_BASE + R_SPI_SSITR)        // Interrupt Test register
#define SPI_SSDR            (SPI_BASE + R_SPI_SSDR)         // Data Register
#define SPI_SSTO            (SPI_BASE + R_SPI_SSTO)         // Time Out Register
#define SPI_SITF            (SPI_BASE + R_SPI_SITF)         // Transmit FIFO Threshold Register
#define SPI_SIRF            (SPI_BASE + R_SPI_SIRF)         // Receive FIFO Threshold Register
#define SPI_CLOCK_PARAMS    (SPI_BASE + R_SPI_CLOCK_PARAMS) // Clock Parameter Register
#define SPI_CS_CONTROL      (SPI_BASE + R_SPI_CS_CONTROL)   // CS Control Register

#define SPI_RESET_PARAMS    (SPI_BASE + 0x204)              // Clock Parameter Register

#define SpiRead(r)          (*((volatile UINT32*)(UINTN)(r)))
#define SpiWrite(r,d)       (*((volatile UINT32*)(UINTN)(r)) = ((UINT32)(d)))


#define MAXIM_UART_SPI_SLAVE_ID     1   // Slave ID on SPI port 0

#define WRITE_CONFIGURATION         ((UINT32)(BIT14 | BIT15))
#define READ_CONFIGURATION          ((UINT32)(BIT14))
#define WRITE_DATA                  ((UINT32)(BIT15))
#define READ_DATA                   ((UINT32)(0))

#define FIFO_ENABLED                (0 << 13)
#define FIFO_DISABLED               (BIT13)

#define ENTER_SOFTWARE_SHUTDOWN     (BIT12)
#define EXIT_SOFTWARE_SHUTDOWN      (0 << 12)

#define XMIT_BUF_EMPTY_INT_ENABLED  (BIT11)
#define XMIT_BUF_EMPTY_INT_DISABLED (0 << 11)

#define FIFO_INT_ENABLED            (BIT10)
#define FIFO_INT_DISABLED           (0 << 10)

#define PARITY_BIT_INT_ENABLED      (BIT9)
#define PARITY_BIT_INT_DISABLED     (0 << 9)

#define FRAMING_ERR_INT_ENABLED     (BIT8)
#define FRAMING_ERR_INT_DISABLED    (0 << 8)

#define IRDA_MODE_ENABLED           (BIT7)
#define IRDA_MODE_DISABLED          (0 << 7)

#define TX_TWO_STOP_BITS            (BIT6)
#define TX_ONE_STOP_BIT             (0 << 6)

#define PARITY_ENABLED              (BIT5)
#define PARITY_DISABLED             (0 << 5)

#define SEVEN_BIT_WORDS             (BIT4)
#define EIGHT_BIT_WORDS             (0 << 4)

#define BAUD_RATE_115200            (1)         // fOSC=3.684MHz
#define BAUD_RATE_57600             (2)
#define BAUD_RATE_38400             (3)
#define BAUD_RATE_19200             (6)

#define READ_DATA_READY             (BIT15)
#define XMIT_BUFFER_FULL            (0)
#define XMIT_BUFFER_EMPTY           (BIT14)
#define FRAMING_ERROR               (BIT10)

// N_VAL = 8192, Bit [30:16] = 0x2000
// M_VAL = 9, Bit [15:1] = 0x9
#define SPI_NVAL_MVAL_SETTING       0x00640002

#define SPI_TIMEOUT_MAX             100

//
// The size of the input queue.  Must be a power of 2.
//
#define INPUT_QUEUE_SIZE  16

CHAR8  InputQueue[INPUT_QUEUE_SIZE] = { 0 };
UINTN  InputQueueStart = 0;
UINTN  InputQueueEnd   = 0;


VOID SpiControllerReset(
   VOID
   ) {
  UINT32 D32;
  UINT32 D32Final;

  //
  // Program CTRLR0 & 1
  //
  // as Chap51_LPSS_090, the SSCR0.SSE bit must be cleared before reconfiguring the SSCR0, SSCR1.
  // Set all bits in SSCR0 & SSCR1 to 0 default before modifying other bits, except mask interrupts
  SpiWrite(SPI_SSCR0, 0x00c00000);
  SpiWrite(SPI_SSCR1, 0x0);

  //
  // Program CTRLR0.
  //
  // Transmit FIFO Under Run Interrupt Mask = Masked, Bit 23 = 0x1
  // Receive FIFO Over Run Interrupt Mask = Masked, Bit 22 = 0x1
  // Network Clock Select = determined by ECS bit, Bit 21 = 0x0
  // Extended Data Size Select = sets the DSS range from 4-16 bits, Bit 20 = 0x0
  // Serial Clock Rate = Use the M/N divider, Bit [19:8] = 0x000
  // Synchronous Serial Port Enable = SSP operation enabled, Bit7 = 0x1
  // External Clock Select = use On-chip clock, Bit6 = 0x0
  // Frame Format = Motorola SPI, Bit[5:4] = 0x0
  // Data Frame Size = 16-bit transfer (1111b)
  //
  //
  D32 = V_SPI_SSCR0_TIM | V_SPI_SSCR0_RIM | V_SPI_SSCR0_SSE | V_SPI_SSCR0_DSS_16_32;
  SpiWrite(SPI_SSCR0, D32);

  //
  // Program the Baud Rate to 1MHz  (100Mhz clock fed into 1/100 divider)
  //
  D32Final = SpiRead(SPI_CLOCK_PARAMS);               // get current clk value
  if (D32Final != (SPI_NVAL_MVAL_SETTING | BIT0)) {   // don't reset if M/N set & clk enabled
    D32 = SPI_NVAL_MVAL_SETTING;                      // set clk value
                                                      //Per LPSS architect, must make 3 separate writes to the CLOCK_PARAMS register to avoid a data race
    SpiWrite(SPI_CLOCK_PARAMS, D32);                 //Write the M/N values to the register
    SpiWrite(SPI_CLOCK_PARAMS, 0x80000000 | D32);    //Update M/N values into the divider (bit31)
    SpiWrite(SPI_CLOCK_PARAMS, D32 | 0x01);          //Enable the clock (bit 0)
  }
}


VOID
SpiControllerInit(
   VOID
   )
/*++

Routine Description:

 This function does the basic init work for SPI controller.

Arguments:


Returns:

 EFI_SUCCESS  SPI controller inited OK

--*/ {
//  SpiControllerReset();

  //There should be no SPI read for PEI phase
  if ((InputQueueStart != 0) || (InputQueueEnd != 0)) {
    InputQueueStart = 0;
    InputQueueEnd   = 0;
  }
}


UINT32
SpiSlaveCmd(
   IN UINT32 Command
   )
/*++

Routine Description:

 This function send specific command to SPI current slave.

Arguments:
 Command     - The command need to issue to the current Slave chosen
 WaitForData - The need to wait for slave feedback or not. TRUE, yes wait for slave data; FALSE, return instantly

Returns:

 The data returned for the command

--*/  {
  UINT32 Timeout = 0;
  //
  // Issue the command
  //
  SpiWrite(SPI_SSDR, Command);

  //
  // Wait for the command to complete when the SPI Host Controller is not busy,
  // and the transmit FIFO is empty meaning the command was sent, and the receive
  // FIFO is not empty meaning a value was returned from the sent command.
  //
  //   BSY (SSI Busy Flag)                  [BIT4] == 0
  //   RNE (Receive FIFO Not Empty)         [BIT3] == 0
  //   TNF (Transmit FIFO Not Full)         [BIT2] == 1
  //
  while ((!((SpiRead(SPI_SSSR) & (V_SPI_SSSR_BSY | V_SPI_SSSR_TNF)) == V_SPI_SSSR_TNF)) && (Timeout < SPI_TIMEOUT_MAX)) {
    Timeout++;
  }
  //
  // Wait for the FIFO to be ready. (RFNE)
  //
  Timeout = 0;
  while ((SpiRead(SPI_SIRF) == 0) && (Timeout < SPI_TIMEOUT_MAX)) {
    Timeout++;
  }

  //
  // Return the data
  //
  return SpiRead(SPI_SSDR);
}

UINT32
SpiSlaveCmd2(
   IN UINT32 Command
   )
/*++

Routine Description:

 This function send specific command to SPI current slave.

Arguments:
 Command     - The command need to issue to the current Slave chosen
 WaitForData - The need to wait for slave feedback or not. TRUE, yes wait for slave data; FALSE, return instantly

Returns:

 The data returned for the command

--*/  {
  UINT32 Timeout = 0;
  UINT32 Data32;

  //
  // Issue the command
  //
  SpiWrite(SPI_SSDR, Command);

  //
  // Wait for the command to complete when the SPI Host Controller is not busy,
  // and the transmit FIFO is empty meaning the command was sent, and the receive
  // FIFO is not empty meaning a value was returned from the sent command.
  //
  //   BSY (SSI Busy Flag)                  [BIT4] == 0
  //   RNE (Receive FIFO Not Empty)         [BIT3] == 0
  //   TNF (Transmit FIFO Not Full)         [BIT2] == 1
  //
  while ((!((SpiRead(SPI_SSSR) & (V_SPI_SSSR_BSY | V_SPI_SSSR_TNF)) == V_SPI_SSSR_TNF)) && (Timeout < SPI_TIMEOUT_MAX)) {
    Timeout++;
  }

  Data32 = SpiRead(SPI_SSDR);

  if (Data32 == 0xFFFF) { // invalid value for SPI read data, if HW SPI singal is disconnected, read value always 0x0000FFFF!
    Data32 = 0;
  }
  //
  // Return the data
  //
  return Data32;
}



VOID maxUartSel(
   VOID
   )
/*++

Routine Description:

 This function does the basic init work for Max3110.

Arguments:


Returns:

 EFI_SUCCESS  Max6950 inited OK

--*/ {
  volatile UINT32 D32;
  UINT32 Timeout = 0;
  //
  // Wait for SPI port to be Idle
  //
  while (((SpiRead(SPI_SSSR) & V_SPI_SSSR_BSY) == V_SPI_SSSR_BSY) && (Timeout < SPI_TIMEOUT_MAX)) {
    Timeout++;
  }

  //
  // Select the appropriate slave device
  //
  D32 = SpiRead(SPI_CS_CONTROL);
  if ((D32 & V_SPI_CS_CONTROL_MASK) != V_SPI_CS_CONTROL_CS1) {
    // It  is the responsibility of the SW Driver to quiescence the current SPI transactions before
    // switching to another SPI_CS output/connect device.
    SpiControllerReset();
    MicroSecondDelay(25000);

    D32 &= ~V_SPI_CS_CONTROL_MASK;
    D32 |= V_SPI_CS_CONTROL_CS1;
    SpiWrite(SPI_CS_CONTROL, D32);
    while ((SpiRead(SPI_CS_CONTROL) & V_SPI_CS_CONTROL_MASK) != V_SPI_CS_CONTROL_CS1);
  }

  //
  // Wait for the controller to be not BUSY and the TX FIFO to be clear */
  //
  Timeout = 0;
  while ((!((SpiRead(SPI_SSSR) & (V_SPI_SSSR_BSY | V_SPI_SSSR_TNF)) == V_SPI_SSSR_TNF)) && (Timeout < SPI_TIMEOUT_MAX)) {
    Timeout++;
  }

  //
  // While there is anything that happens to be IN the FIFO, read it out.
  //
  Timeout = 0;
  while (((SpiRead(SPI_SSSR) & V_SPI_SSSR_RNE) != 0) &&  (Timeout < SPI_TIMEOUT_MAX)) {
    SpiRead(SPI_SSDR);
    Timeout++;
  }
}

RETURN_STATUS
EFIAPI
maxConfig(
   VOID
   )
/**
 Initialize the serial device hardware.

 If no initialization is required, then return RETURN_SUCCESS.
 If the serial device was successfuly initialized, then return RETURN_SUCCESS.
 If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

 Initialize the MAX3110.

 This routine waits for the SPI Host Port 0 to be idle.  Once it is, it
 assigns the CS to be used to be that of the UART.  Subsequently, it then
 performs a Write Configuration command to the UART device to configure it
 for a FIXED configuration.  It will not return to the caller until the
 SPI Host Port 0 BUSY bit is zero and the TX FIFO is clear.

 @retval RETURN_SUCCESS        The serial device was initialized.
 @retval RETURN_DEVICE_ERROR   The serail device could not be initialized.

**/ {
  UINT32    D32;
  UINT32    Configuration;
  UINTN     SpiBaseMmio;
  UINT32    Spi1BaseAddress = 0;

  //Initialize the SPI BAR
  SpiBaseMmio = MmPciAddress(0, 0, PCI_DEVICE_NUMBER_LPSS_SPI, PCI_FUNCTION_NUMBER_LPSS_SPI1, 0);

  D32 = SpiRead(SpiBaseMmio + R_LPSS_IO_BAR);
  Spi1BaseAddress = D32 & B_LPSS_IO_BAR_BA;

  // Check if the BAR is programmed yet
  if ((Spi1BaseAddress == 0) || (Spi1BaseAddress == B_LPSS_IO_BAR_BA)) {
    return RETURN_NOT_READY;
  }

  //Check if the device is ready
  D32 = SpiRead(SpiBaseMmio + R_LPSS_IO_STSCMD);
  if ((D32 & B_LPSS_IO_STSCMD_MSE) == 0) {
    return RETURN_NOT_READY;
  }

  //set remap low mem base and reset SPI controller
  MmioWrite32((UINTN)(Spi1BaseAddress + R_LPSS_IO_REMAP_ADDRESS_LOW), Spi1BaseAddress);
  SpiWrite(SPI_RESET_PARAMS, 0x03);

  if (Spi1BaseAddress != mSpi1BaseAddress) {
    mSpi1BaseAddress = Spi1BaseAddress;
  }

  //
  // Initialize the SPI Controller
  //
  SpiControllerInit();

  //
  // Select UART device
  //
  maxUartSel();

  //
  // Perform a Configuration Register Write for setup.
  // Current configuration is for no interrupts, normal operation, 115200 8-n-1
  Configuration =
     FIFO_ENABLED                |
        EXIT_SOFTWARE_SHUTDOWN      |
        XMIT_BUF_EMPTY_INT_DISABLED |
        FIFO_INT_DISABLED           |
        PARITY_BIT_INT_DISABLED     |
        FRAMING_ERR_INT_DISABLED    |
        IRDA_MODE_DISABLED          |
        TX_ONE_STOP_BIT             |
        PARITY_DISABLED             |
        EIGHT_BIT_WORDS; //           |
                         //BAUD_RATE_115200;

  SpiSlaveCmd(WRITE_CONFIGURATION | Configuration);

  //
  // Since the UART may be active the transmit buffer empty bit may not
  // be set.  Instead read back the configuration and validate it ignoring
  // the receive data ready and transmit buffer empty bits.
  //
  // Determine if the SPI UART is present
  //
  D32 = SpiSlaveCmd(READ_CONFIGURATION);
  if ((D32 & 0x00003FFF) != Configuration) {
    /* Device isn't present.  Don't try to manipulate the UART. */
    return RETURN_DEVICE_ERROR;
  } else {
    return RETURN_SUCCESS;
  }
}

RETURN_STATUS
EFIAPI
SerialPortInitialize(
   VOID
   ) {
  UINT32    SpiBar0;
  UINT32    D32;
  UINTN     SpiBaseMmio;

  //Initialize the SPI Controller BAR0 in PEI phase
  SpiBaseMmio = MmPciAddress (0, 0, PCI_DEVICE_NUMBER_LPSS_SPI, PCI_FUNCTION_NUMBER_LPSS_SPI1, 0);

  D32 = MmioRead32 (SpiBaseMmio + R_LPSS_IO_STSCMD);
  D32 &= ~(UINT32)(B_LPSS_IO_STSCMD_MSE | B_LPSS_IO_STSCMD_BME);
  MmioWrite32 (SpiBaseMmio + R_LPSS_IO_STSCMD, D32);

  SpiBar0 = mSpi1BaseAddress;
  MmioWrite32(SpiBaseMmio + R_LPSS_IO_BAR, SpiBar0);

  D32 = MmioRead32 (SpiBaseMmio + R_LPSS_IO_STSCMD);
  D32 |= (B_LPSS_IO_STSCMD_MSE | B_LPSS_IO_STSCMD_BME);
  MmioWrite32 (SpiBaseMmio + R_LPSS_IO_STSCMD, D32);
  return RETURN_SUCCESS;
}

UINTN
EFIAPI
SerialPortWrite(
   IN UINT8     *Buffer,
   IN UINTN     NumberOfBytes
   )
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

**/ {
  UINTN    Index;
  UINT32   Data;
  BOOLEAN  State;
  UINT32   Timeout = 0;

  //
  // If serial port initialization is not successful, return.
  //
  if (EFI_ERROR(maxConfig())) {
    return 0;
  }

  //
  // Make sure interrupts are disabled
  //
  State = SaveAndDisableInterrupts();

  for (Index = 0; Index < NumberOfBytes; Index++) {

    //
    // Wait for the transmit buffer to be empty so there is room in the
    // transmit buffer to send the next byte from the buffer.  While
    // waiting, look to see if there is any data ready to be read from the
    // UART and queue any valid bytes received.
    //
    do {
      //
      // Read data from the UART.  This also returns the status of the TX FIFO and RX FIFO
      //
      Data = SpiSlaveCmd(READ_DATA);

      if (Data == 0) {
        break;
      }

#if 0
      //
      // If a character was received from the UART then queue it
      //
      if (((Data & READ_DATA_READY) != 0) && (Data != 0xffff)) { // data & data value not equal 0xFFFF
        InputQueue[InputQueueStart] = (CHAR8)(Data & 0xff);
        InputQueueStart = (InputQueueStart + 1) & (INPUT_QUEUE_SIZE - 1);
      }
#endif
    }
    while (((Data & XMIT_BUFFER_EMPTY) != XMIT_BUFFER_EMPTY) || (SpiRead(SPI_SSSR) & V_SPI_SSSR_RNE) != 0);

    while ((!((SpiRead(SPI_SSSR) & (V_SPI_SSSR_BSY | V_SPI_SSSR_TNF)) == V_SPI_SSSR_TNF)) && (Timeout < SPI_TIMEOUT_MAX)) {
      Timeout++;
    }

    //
    // Send a character to the UART
    //
    Data = SpiSlaveCmd(WRITE_DATA | Buffer[Index]);
    MicroSecondDelay(100);

#if 0
    //
    // If a character was received from the UART then queue it
    //
    if ((Data & READ_DATA_READY) != 0) {
      InputQueue[InputQueueStart] = (CHAR8)(Data & 0xff);
      InputQueueStart = (InputQueueStart + 1) & (INPUT_QUEUE_SIZE - 1);
    }
#endif
  }

  //
  // Restore the original interrupt state
  //
  SetInterruptState(State);

  return Index;
}


UINTN
EFIAPI
SerialPortRead(
   OUT UINT8     *Buffer,
   IN  UINTN     NumberOfBytes
   )
/**
 Reads data from a serial device into a buffer.
 SerialPortRead should only be used after PEI phase

 @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
 @param  NumberOfBytes    Number of bytes to read from the serial device.

 @retval 0                NumberOfBytes is 0.
 @retval >0               The number of bytes read from the serial device.
                          If this value is less than NumberOfBytes, then the read operation failed.

**/ {
  UINTN    Index = 0;
  UINT32   Data;
  BOOLEAN  State;
  UINTN    TimeCount = 0;
  STATIC   BOOLEAN SerialPortInitialized = FALSE;
  EFI_STATUS Status;

  // SerialPortRead should be called in DXE phase.
  if (!SerialPortInitialized) {
    Status = maxConfig();
    if (!EFI_ERROR(maxConfig())) {
      SerialPortInitialized = TRUE;
    }
  } else {
    maxUartSel();  //chip select serial uart
  }

  //
  // Make sure interrupts are disabled
  //
  State = SaveAndDisableInterrupts();

  //
  // First read any characters that are in the input queue
  //
  for (Index = 0; (Index < NumberOfBytes) && (InputQueueStart != InputQueueEnd); Index++) {
    Buffer[Index] = InputQueue[InputQueueEnd];
    InputQueueEnd = (InputQueueEnd + 1) & (INPUT_QUEUE_SIZE - 1);
  }
  do {
    Data = SpiSlaveCmd2(READ_DATA);
    if (((Data & READ_DATA_READY) != 0) && (Data != 0xffff)) { // data & data value not equal 0xFFFF
      Buffer[Index++] = (CHAR8)(Data & 0xff);
      TimeCount = 0;
    } else {
      MicroSecondDelay(1);
      TimeCount++;
    }
  }
  while (Index < NumberOfBytes && TimeCount <= SPI_TIMEOUT_MAX);


  //
  // Restore the original interrupt state
  //
  SetInterruptState(State);

  //
  // Return the number of characters read from the UART
  //
  return Index;
}


BOOLEAN
EFIAPI
SerialPortPoll(
   VOID
   )
/**
 Polls a serial device to see if there is any data waiting to be read.

 Polls aserial device to see if there is any data waiting to be read.
 If there is data waiting to be read from the serial device, then TRUE is returned.
 If there is no data waiting to be read from the serial device, then FALSE is returned.

 @retval TRUE             Data is waiting to be read from the serial device.
 @retval FALSE            There is no data waiting to be read from the serial device.

**/ {
  UINT32   Data;
  BOOLEAN  State;

  //
  // If serial port initialization is not successful, return.
  //
  if (EFI_ERROR(maxConfig())) {
    return 0;
  }

  //
  // Make sure interrupts are disabled
  //
  State = SaveAndDisableInterrupts();

  //
  // If there is data in the input queue then return TRUE
  //
  if (InputQueueStart != InputQueueEnd) {
    //
    // Restore the original interrupt state
    //
    SetInterruptState(State);

    return TRUE;
  }

  //
  // Select UART device
  //
  maxUartSel();

  //
  // Read the UART configuration register
  //
  Data = SpiSlaveCmd(READ_CONFIGURATION);

  //
  // Restore the original interrupt state
  //
  SetInterruptState(State);

  //
  // If there is a character waiting to be read from the UART then return TRUE
  //
  if ((Data & READ_DATA_READY) != 0) {
    return TRUE;
  }

  //
  // There is no data in the input queue and there is no data waiting to be read from the UART
  //
  return FALSE;
}

