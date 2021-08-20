/** @file
  This file contains Initialization of the BDAT data structures for both
  v1.0 and v2.0 taken from v0.6 of "A Compatible Method to Access BIOS
  Data Structures".

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

@par Specification Reference:
**/

#ifndef _BDAT_H_
#define _BDAT_H_

///
/// BDAT definitions
///
#define MAX_NODE              1                     ///< Max processors per system
#define MAX_DIMM              2                     ///< Max DIMMs per channel
#define MAX_RANK_DIMM         2                     ///< Max ranks per DIMM
//#define MAX_BITS             72
#define MAX_SPD_BYTE        256                     ///< Number of bytes in Serial EEPROM

#define BDAT_REV2_SUPPORT  1

#pragma pack(1)

#if BDAT_REV2_SUPPORT

#define BDAT_PRIMARY_VER    0x0002
#define BDAT_SECONDARY_VER  0x0000

#else

#define BDAT_PRIMARY_VER    0x0001
#define BDAT_SECONDARY_VER  0x0000

#endif

typedef struct {
  UINT8   BiosDataSignature[8]; ///< "BDATHEAD"
  UINT32  BiosDataStructSize;   ///< sizeof BDAT_STRUCTURE
  UINT16  Crc16;                ///< 16-bit CRC of BDAT_STRUCTURE (calculated with 0 in this field)
  UINT16  Reserved;
  UINT16  PrimaryVersion;       ///< Primary version
  UINT16  SecondaryVersion;     ///< Secondary version
  UINT32  OemOffset;            ///< Optional offset to OEM-defined structure
  UINT32  Reserved1;
  UINT32  Reserved2;
} BDAT_HEADER_STRUCTURE;


#if BDAT_REV2_SUPPORT

typedef struct {
  UINT8   rxDqLeft;   ///< Units = piStep
  UINT8   rxDqRight;
  UINT8   txDqLeft;
  UINT8   txDqRight;
  UINT8   rxVrefLow;  ///< Units = rxVrefStep
  UINT8   rxVrefHigh;
  UINT8   txVrefLow;  ///< Units = txVrefStep
  UINT8   txVrefHigh;
} BDAT_DQ_MARGIN_STRUCTURE;

#endif

typedef struct {
  UINT8 rxDqLeft;               ///< Units = piStep
  UINT8 rxDqRight;
  UINT8 txDqLeft;
  UINT8 txDqRight;
  UINT8 cmdLeft;
  UINT8 cmdRight;
  UINT8 rxVrefLow;              ///< Units = rxVrefStep
  UINT8 rxVrefHigh;
  UINT8 txVrefLow;              ///< Units = txVrefStep
  UINT8 txVrefHigh;
} BDAT_RANK_MARGIN_STRUCTURE;

typedef struct {
  UINT16  recEnDelay[MAX_STROBES]; ///< Array of nibble training results per rank
  UINT16  wlDelay[MAX_STROBES];
  UINT8   rxDqDelay[MAX_STROBES];
  UINT8   txDqDelay[MAX_STROBES];
  UINT8   clkDelay;
  UINT8   ctlDelay;
  UINT8   cmdDelay[3];
  UINT8   ioLatency;
  UINT8   roundtrip;
} BDAT_RANK_TRAINING_STRUCTURE;

typedef struct {
  UINT16  mr0;                 ///< MR0 settings
  UINT16  mr1;                 ///< MR1 settings
  UINT16  mr2;                 ///< MR2 settings
  UINT16  mr3;                 ///< MR3 settings
} BDAT_RANK_MRS_STRUCTURE;

#if BDAT_REV2_SUPPORT

typedef struct {
  UINT8                         rankEnabled;         ///< 0 = Rank disabled
  UINT8                         rankMarginEnabled;   ///< 0 = Rank margin disabled
  UINT8                         dqMarginEnabled;     ///< 0 = Dq margin disabled
  BDAT_RANK_MARGIN_STRUCTURE    rankMargin;          ///< Rank margin data
  BDAT_DQ_MARGIN_STRUCTURE      dqMargin[MAX_STROBES];     ///< Array of Dq margin data per rank
  BDAT_RANK_TRAINING_STRUCTURE  rankTraining;        ///< Rank training settings
  BDAT_RANK_MRS_STRUCTURE       rankMRS;             ///< Rank MRS settings
} BDAT_RANK_STRUCTURE;

#else

typedef struct {
  UINT8                         rankEnabled;
  BDAT_RANK_MARGIN_STRUCTURE    rankMargin;
  BDAT_RANK_TRAINING_STRUCTURE  rankTraining;
  BDAT_RANK_MRS_STRUCTURE       rankMRS;
} BDAT_RANK_STRUCTURE;

#endif

typedef struct {
  UINT8 valid[MAX_SPD_BYTE / 8];                    ///< Each valid bit maps to SPD byte
  UINT8 spdData[MAX_SPD_BYTE];                      ///< Array of raw SPD data bytes
} BDAT_SPD_STRUCTURE;

typedef struct {
  UINT8               dimmEnabled;                  ///< 0 = DIMM disabled
  BDAT_RANK_STRUCTURE rankList[MAX_RANK_DIMM];      ///< Array of ranks per DIMM
  BDAT_SPD_STRUCTURE  spdBytes;                     ///< SPD data per DIMM
} BDAT_DIMM_STRUCTURE;

typedef struct {
  UINT8               chEnabled;                    ///< 0 = Channel disabled
  UINT8               numDimmSlot;                  ///< Number of slots per channel on the board
  BDAT_DIMM_STRUCTURE dimmList[MAX_DIMM];           ///< Array of DIMMs per channel
} BDAT_CHANNEL_STRUCTURE;

#if BDAT_REV2_SUPPORT

typedef struct {
  UINT8                   imcEnabled;                ///< 0 = MC disabled
  UINT16                  imcDid;                    ///< MC device Id
  UINT8                   imcRid;                    ///< MC revision Id
  UINT16                  ddrFreq;                   ///< DDR frequency in units of MHz/10
                                                     ///< e.g. ddrFreq = 13333 for tCK = 1.5 ns
  UINT16                  ddrVoltage;                ///< Vdd in units of mV
                                                     ///< e.g. ddrVoltage = 1350 for Vdd = 1.35 V
  UINT8                   piStep;                    ///< Step unit   = piStep * tCK/2048
                                                     // e.g. piStep = 16 for step = 11.7ps (1/128 tCK)
  UINT16                  rxVrefStep;                ///< Step unit = rxVrefStep * Vdd / 100
                                                     ///< e.g. rxVrefStep = 520 for step = 7.02 mV
  UINT16                  txVrefStep;                ///< Step unit = txVrefStep * Vdd / 100
  BDAT_CHANNEL_STRUCTURE  channelList[MAX_CHANNELS]; ///< Array of channels per socket
} BDAT_SOCKET_STRUCTURE;

#else

typedef struct {
  UINT8                   imcEnabled;           ///< 0 = MC disabled
  UINT16                  ddrFreq;              ///< Units of MT/s
  UINT16                  ddrVoltage;           ///< Units of mV
  UINT16                  imcDid;               ///< MC device Id
  UINT8                   imcRid;               ///< MC revision Id
  BDAT_CHANNEL_STRUCTURE  channelList[MAX_CHANNELS];
} BDAT_SOCKET_STRUCTURE;

#endif

#if BDAT_REV2_SUPPORT

typedef struct {
  UINT32                  refCodeRevision;        ///< Matches JKT scratchpad definition
  UINT8                   maxNode;                ///< Max processors per system, e.g. 4
  UINT8                   maxCh;                  ///< Max channels per socket, e.g.  4
  UINT8                   maxDimm;                ///< Max DIMM per channel, e.g.    3
  UINT8                   maxRankDimm;            ///< Max ranks per DIMM, e.g.     4
  UINT8                   maxStrobe;              ///< Number of Dqs used by the rank, e.g. 18
  UINT8                   maxDq;                  ///< Number of Dq bits used by the rank, e.g. 72
  UINT32                  marginLoopCount;        ///< Units of cache line
  BDAT_SOCKET_STRUCTURE   socketList[MAX_NODE];   ///< Array of sockets per system
} BDAT_SYSTEM_STRUCTURE;

#else

typedef struct {
  UINT32                  refCodeRevision;
  UINT32                  marginLoopCount;      ///< Units of cache line
  BDAT_SOCKET_STRUCTURE   socketList[MAX_NODE];
} BDAT_SYSTEM_STRUCTURE;

#endif

typedef struct bdatStruct {
  BDAT_HEADER_STRUCTURE bdatHeader;
  BDAT_SYSTEM_STRUCTURE bdatSys;
} BDAT_STRUCTURE;

#pragma pack()

#endif // _BDAT_H_
