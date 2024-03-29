/** @file
  Spd.h
  This file include all the spd data defination.

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

@par Specification Reference:
**/

#ifndef _SPD_H_
#define _SPD_H_

#define MAX_SPD_ADDR                125//512              // SPD Index of highest byte used by this reference code
#define MAX_SPD_BYTE                256              // Number of bytes in Serial EEPROM
//
// Manufacturer ID's
//
#define MICRON_ID           0x2C
#define SAMSUNG_ID          0xCE
#define TRANSCEND_ID        0x4F
#define KINGSTON_ID         0x98
#define ELPIDA_ID           0xFE
#define HYNIX_ID            0xAD
#define INFINEON_ID         0xC1
#define SMART_ID            0x57
#define QIMONDA_ID          0x51
#define MAX_MANUFACTURER_ID 9
//
// SPD register offsets
//
#define SPD_DDR3_TYPE             2       // Memory Type
#define SPD_DDR3_MODULE           3       // Module type (3:0)
#define SPD_DDR3_DENBANK          4       // Density (6:4), Banks (3:0)
#define SPD_DDR3_ADDRESS          5       // Row (5:3), Column (2:0) address
#define SPD_DDR3_VDD              6       // Module Nominal Voltage
#define SPD_DDR3_ORG              7       // Ranks (5:3),device width (2:0)
#define SPD_DDR3_WIDTH            8       // Bus width ext (4:3), Bus width (2:0)
#define SPD_DDR3_MTBDD            10      // Medium Timebase (MTB) Dividend
#define SPD_DDR3_MTBDS            11      // Medium Timebase (MTB) Divisor
#define SPD_DDR3_TCLK             12      // Minimum cycle time (tCKmin)
#define SPD_DDR3_CLL              14      // CAS latency supported, low byte
#define SPD_DDR3_CLH              15      // CAS latency supported, high byte
#define SPD_DDR3_TAA              16      // Minimum CAS latency time (tAA)
#define SPD_DDR3_TWR              17      // Minimum write recovery time (tWR)
#define SPD_DDR3_TRCD             18      // Minimum RAS to CAS time (tRCD)
#define SPD_DDR3_TRRD             19      // Minimum RA to RA time (tRRD)
#define SPD_DDR3_TRP              20      // Minimum precharge time (tRP)
#define SPD_DDR3_TRASRC           21      // Upper nibbles for tRAS (7:4), tRC (3:0)
#define SPD_DDR3_TRAS             22      // Minimum active to precharge (tRAS)
#define SPD_DDR3_TRC              23      // Minimum active to active/refresh (tRC)
#define SPD_DDR3_TRFCL            24      // Minimum refresh recovery (tRFC), low byte
#define SPD_DDR3_TRFCH            25      // Minimum refresh recovery (tRFC), high byte
#define SPD_DDR3_TWTR             26      // Minimum internal wr to rd cmd (tWTR)
#define SPD_DDR3_TRTP             27      // Minimum internal rd to pc cmd (tRTP)
#define SPD_DDR3_TFAWH            28      // Upper Nibble for tFAW
#define SPD_DDR3_TFAWL            29      // Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte
#define SPD_DDR3_STROPT           31      // SDRAM Thermal and Refresh Options
#define SPD_DDR3_TCK_FTB          34      // Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
#define SPD_DDR3_TAA_FTB          35      // Fine Offset for Minimum CAS Latency Time (tAAmin)
#define SPD_DDR3_TRCD_FTB         36      // Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
#define SPD_DDR3_TRP_FTB          37      // Minimum Row Precharge Delay Time (tRPmin)
#define SPD_DDR3_TRC_FTB          38      // Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
#define SPD_DDR3_MAC              41      // MAC
#define SPD_DDR3_TCCD             0       // Register doesn't exist in SPD byte
#define SPD_DDR3_TWTP             0       // Register doesn't exist in SPD byte
#define SPD_DDR3_TWCL             0       // Register doesn't exist in SPD byte
#define SPD_DDR3_TCMD             0       // Register doesn't exist in SPD byte

#define SPD_DDR3_RAW_CARD_ID                62      // Raw Card Identification
#define SPD_DDR3_ADD_MAPPING                63      // Address Mapping (Odd Rank Mirror)
#define SPD_DDR3_MANUFACTURER_ID_LO         117
#define SPD_DDR3_MANUFACTURER_ID_HI         118
#define SPD_DDR3_MANUFACTURE_LOCATION       119
#define SPD_DDR3_MANUFACTURE_DATE_LO        120
#define SPD_DDR3_MANUFACTURE_DATE_HI        121
#define SPD_DDR3_SERIAL_NUMBER_1            122
#define SPD_DDR3_SERIAL_NUMBER_2            123
#define SPD_DDR3_SERIAL_NUMBER_3            124
#define SPD_DDR3_SERIAL_NUMBER_4            125
#define SPD_DDR3_DRAM_MANUFACTURER_ID_LOW   148   // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DDR3_DRAM_MANUFACTURER_ID_HIGH  149   // DRAM Manufacturer ID Code, Most Significant Byte
//
// SPD DDR3 register definitions
//
#define SPD_DDR_RDIMM             0x01    // DDR3/4 Registered Memory Type value
#define SPD_DDR3_UNBUFFERED       0x02    // DDR3/4 Unbuffered Memory Type value
#define SPD_DDR3_SODIMM           0x03    // SO-DIMM with no ECC
#define SPD_DDR3                  0x0B    // DDR3 Memory type
#define SPD_DDR4                  0x0C    // DDR4 Memory type
#define SPD_DDR3_SODIMM_ECC       0x08    // SO-DIMM with ECC
#define SPD_DDR3_MTYPE_MASK       0x0F    // DDR3 Memory Type mask
#define SPD_DDR3_BWE_MASK         0x18    // DDR3 Bus Width Extension mask
#define SPD_DDR3_S                0x01    // DDR3
#define SPD_DDR3_L                0x02    // DDR3L
#define SPD_DDR3_U                0x04    // DDR3U
//
// SPD Values for VDD
// Nominal Voltage
//
#define SPD_DDRS                  1
#define SPD_DDRL                  2
#define SPD_DDRL_DDRS             3
#define SPD_DDRU                  4
#define SPD_DDRU_DDRL             6
//
// MAC Values
//
#define SPD_DDR3_PTTR_MASK       (BIT7)
#define SPD_DDR3_UNLIMITED_MAC   (BIT3)

#define SPD_DDR3_MAC_MASK        (7)
#define SPD_DDR3_MAC_300k        (BIT2|BIT0)
#define SPD_DDR3_MAC_400k        (BIT2)
#define SPD_DDR3_MAC_500k        (BIT1|BIT0)
#define SPD_DDR3_MAC_600k        (BIT1)
#define SPD_DDR3_MAC_700k        (BIT0)
#define SPD_DDR3_MAC_UNKNOWN     (0)

//#if DDR4_SUPPORT
//
// DDR4 Specific SPD
//
#define SPD_DDR4_ADDRESS          5       // Row (5:3), Column (2:0) address
#define SPD_DDR4_TYPE             6       // SDRAM Device Type (DDR4)
#define SPD_DDR4_OPTFEAT          7       // SDRAM Optional Features (DDR4)
#define SPD_DDR4_TRFSH            8       // SDRAM Thermal Refresh Options (DDR4)
#define SPD_DDR4_VDD              11      // Vdd DIMM supports (DDR4)
#define SPD_DDR4_ORG              12      // Number of Ranks and SDRAM device width (DDR4)
#define SPD_DDR4_WIDTH            13      // Width of SDRAM memory bus
#define SPD_DDR4_TS               14      // Module Thermal Sensor
#define SPD_DDR4_TB               17      // Timebase [3:2] MTB, [1:0] FTB
#define SPD_DDR4_MIN_TCLK         18      // Minimum cycle time (tCKmin)
#define SPD_DDR4_MAX_TCLK         19      // Maximum cycle time
#define SPD_DDR4_CAS1             20      // CAS Latencies Supported, first byte
#define SPD_DDR4_CAS2             21      // CAS Latencies Supported, second byte
#define SPD_DDR4_CAS3             22      // CAS Latencies Supported, third byte
#define SPD_DDR4_CAS4             23      // CAS Latencies Supported, fourth byte
#define SPD_DDR4_TAA              24      // Minimum CAS Latency Time (tAAmin)
#define SPD_DDR4_TRCD             25      // Minimum RAS to CAS delay
#define SPD_DDR4_TRP              26      // Minimum Row Precharge time
#define SPD_DDR4_TRASRC           27      // Upper nibbles for min tRAS and tRC
#define SPD_DDR4_TRAS             28      // Minimum Active to Precharge time
#define SPD_DDR4_TRC              29      // Minimum Active to Active/Refresh time
#define SPD_DDR4_TRFCL            30      // Minimum Refresh Recovery time least-significant byte
#define SPD_DDR4_TRFCH            31      // Minimum Refresh Recovery time most-significant byte
#define SPD_DDR4_TFAWH            36      // Upper Nibble for tFAW
#define SPD_DDR4_TFAWL            37      // Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte
#define SPD_DDR4_TRRDS            38      // Minimum RA to RA time (tRRDs) different bank
#define SPD_DDR4_TRRDL            39      // Minimum RA to RA time (tRRDl) same bank
#define SPD_DDR4_TCCD             40      // Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group
#define SPD_DDR4_TCCD_FTB        117      // Fine offset for TCCD
#define SPD_DDR4_TRC_FTB         120      // Fine offset for TRC
#define SPD_DDR4_TRP_FTB         121      // Fine offset for TRP
#define SPD_DDR4_TRCD_FTB        122      // Fine offset for TRCD
#define SPD_DDR4_TAA_FTB         123      // Fine offset for TAA
#define SPD_DDR4_MAX_TCLK_FTB    124      // Fine offset for max TCK
#define SPD_DDR4_MIN_TCLK_FTB    125      // Fine offset for min TCK
#define SPD_DDR4_ADD_MAPPING     131      // Address Mapping (Odd Rank Mirror)
#define SPD_DDR4_MMIDL           320      // Module Manufacturer ID Code, Least Significant Byte
#define SPD_DDR4_MMIDH           321      // Module Manufacturer ID Code, Mostst Significant Byte
#define SPD_DDR4_MMLOC           322      // Module Manufacturing Location
#define SPD_DDR4_MMDATE          323      // Module Manufacturing Date 323-324
#define SPD_DDR4_MODULESN        325      // Module Serial Number 325-328
#define SPD_DDR4_MODULEPN        329      // Module Part Number 329-348
#define SPD_DDR4_MODULERC        349      // Module Revision Code
#define SPD_DDR4_MIDCL           350      // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DDR4_MIDCH           351      // DRAM Manufacturer ID Code, Most Significant Byte
#define SPD_DDR4_REV             352      // DRAM Stepping
#define SPD_DDR4_CRCL            382      // LSB of 16-bit CRC
#define SPD_DDR4_CRCH            383      // MSB of 16-bit CRC
//
// DDR4 Medium Time Base 125 ps
//
#define SPD_DDR4_MTB             125    // 125 ps
//
// DDR4 module specific section
//
#define SPD_DDR4_RAWCARD         130    // Reference Raw Card Used
//
// UDIMM specific bytes
//
#define SPD_DDR4_UADDRMAP        131    // Address Mapping from Edge Connector to DRAM
//
// RDIMM specific bytes
//
#define SPD_DDR4_RADDMAP         136    // Address mapping from Reg to DRAM

//#endif // DDR4_SUPPORT

//
// MR Commands JEDEC Related
//
#define DDR3_MRS2_RTTWR_0        (0)
#define DDR3_MRS2_RTTWR_60       (1)
#define DDR3_MRS2_RTTWR_120      (2)
#define DDR3_MRS2_RTTWR_RESERVED (3)

#define DDR3_MRS1_RTTNOM_DIS     (0)
#define DDR3_MRS1_RTTNOM_60      (BIT2)
#define DDR3_MRS1_RTTNOM_120     (BIT6)
#define DDR3_MRS1_RTTNOM_40      (BIT6|BIT2)

#endif

