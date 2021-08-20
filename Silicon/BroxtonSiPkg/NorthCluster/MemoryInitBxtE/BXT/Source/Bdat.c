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

#include "McFunc.h"
#include "DetectDimms.h"
#include "Bdat.h"

///
/// Flags to control support options (to turn off, comment them out)
///
#define BDAT_REV2_SUPPORT 1

#if BDAT

/**
  This function returns the CRC16 of the provided array.

  @param[in]  Ptr    The array to be processed.
  @param[in]  Count  Number of elements in the array.

  @retval  CRC16 of the provided array.

**/
INT16
Crc16 (
  IN INT8  *Ptr,
  IN INT32 Count
)
;

extern UINT16 cmdDelayValue[C_MAXDDR];

/**
  This function fills in both v1.0 and v2.0 of the BDAT data structure per
  v0.6 of "A Compatible Method to Access BIOS Data Structures"

  @param[in,out]  Host      The Host structure for all MRC global data.
  @param[out]     HostBDAT  The BDAT structure to be initialized.

  @retval  None

**/
VOID
FillBdatStructure (
  IN  OUT   MMRC_DATA         *Host,
      OUT   BDAT_STRUCTURE    *HostBDAT
)
{
  UINT8                  node;
  UINT8                  ch;
  UINT8                  dimm;
  UINT8                  rank;
  UINT32                 i;
#if DEBUG_MSG
  UINT8                  strobe;
  UINT16                 spd;
#endif  // DEBUG_MSG
  DIMM_VOLTAGE_FLAGS     *DimmVoltageFlags;
  BDAT_CHANNEL_STRUCTURE (*channelBDAT) [MAX_CHANNELS];
  BDAT_DIMM_STRUCTURE    (*dimmBDAT) [MAX_DIMM];
  BDAT_RANK_STRUCTURE    (*rankBDAT) [MAX_RANK_DIMM];
  UINT8                  *Ddr3RelevantSpdBytes = NULL;
  UINT32                 NumElements = 0;

  MmrcMemset (HostBDAT, 0, sizeof (BDAT_STRUCTURE) );

  for (node = 0; node < MAX_NODE; node++) {

    //
    // AVN has one CPU socket
    //
    channelBDAT = &HostBDAT->bdatSys.socketList[node].channelList;

    //
    // Indicate this socket is enabled
    //
    HostBDAT->bdatSys.socketList[node].imcEnabled = 1;

    //
    // DDR frequency in MT/s * 10
    //
    switch (Host->CurrentFrequency) {
    case MMRC_800:
      HostBDAT->bdatSys.socketList[node].ddrFreq = 800 * 2;
      break;
    case MMRC_1066:
      HostBDAT->bdatSys.socketList[node].ddrFreq = 1066 * 2;
      break;
    case MMRC_1333:
      HostBDAT->bdatSys.socketList[node].ddrFreq = 1333 * 2;
      break;
    case MMRC_1600:
      HostBDAT->bdatSys.socketList[node].ddrFreq = 1600 * 2;
      break;
    case MMRC_1866:
      HostBDAT->bdatSys.socketList[node].ddrFreq = 1866 * 2;
      break;
    default:
      MmrcDebugPrint ((MMRC_DBG_MIN, "BDAT: invalid ddrFreq = %d.\n", Host->CurrentFrequency));
      break;
    } // CurrentFrequency switch

    //
    // DDR voltage in mV
    //
    DimmVoltageFlags = (DIMM_VOLTAGE_FLAGS *) &Host->DimmVoltageFlags;
    switch (DimmVoltageFlags->FLAGS.RunDimmVoltageAt) {
    case DDR_VOLTAGE_150V:
      HostBDAT->bdatSys.socketList[node].ddrVoltage = 1500;
      break;
    case DDR_VOLTAGE_135V:
      HostBDAT->bdatSys.socketList[node].ddrVoltage = 1350;
      break;
    case DDR_VOLTAGE_125V:
      HostBDAT->bdatSys.socketList[node].ddrVoltage = 1250;
      break;
    default:
      MmrcDebugPrint ((MMRC_DBG_MIN, "BDAT: invalid dimmVoltage= %d.\n", DimmVoltageFlags->FLAGS.RunDimmVoltageAt));
      break;
    } // RunDimmVoltageAt switch

    //
    // IMC device ID
    //
    HostBDAT->bdatSys.socketList[node].imcDid = Host->CpuType;

    //
    // IMC revision ID
    //
    HostBDAT->bdatSys.socketList[node].imcRid = Host->CpuStepping;

#ifdef BDAT_REV2_SUPPORT
    ///
    ///                        (1/64)* QClock * 2048      (1/64)* QClock * 2048       2048
    ///   Indicate piStep  =  ----------------------   =  ----------------------  = -------- = 16
    ///                                 tCK                       2*QClock             128
    ///
    HostBDAT->bdatSys.socketList[node].piStep  = (UINT8) 16;

    ///
    ///                            step * 100       7.81 * 100
    ///   Indicate rxVrefStep  =  ------------ =  ------------ = 520
    ///                               Vdd             1.5
    ///
    HostBDAT->bdatSys.socketList[node].rxVrefStep  = (UINT16) 520;

    ///
    ///                            step * 100       8 * 100
    ///   Indicate txVrefStep  =  ------------ =  ------------ = 533
    ///                               Vdd             1.5
    ///
    HostBDAT->bdatSys.socketList[node].txVrefStep  = (UINT16) 533;
#endif

    for (ch = 0; ch < MAX_CHANNELS; ch++) {
      //
      // Skip if channel is disabled
      //
      if (Host->Channel[ch].Enabled == 0) {
        continue;
      }

      //
      // Indicate this channel is enabled
      //
      (*channelBDAT) [ch].chEnabled = 1;

      //
      // Indicate the number of DIMM slots available on this channel
      //
      (*channelBDAT) [ch].numDimmSlot = MAX_DIMM;

      dimmBDAT = &HostBDAT->bdatSys.socketList[node].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if (!Host->Channel[ch].DimmPresent[dimm]) {
          continue;
        }

        //
        // Indicate this DIMM is enabled
        //
        (*dimmBDAT) [dimm].dimmEnabled = 1;

//review needed
#if DDR3_SUPPORT || DDR4_SUPPORT
        //
        // SPD bytes
        //
        GetDdr3RelevantSpdBytes (Host, &Ddr3RelevantSpdBytes, &NumElements);
        if ((NumElements == 0) || (Ddr3RelevantSpdBytes == NULL) ) {
          MmrcDebugPrint ((MMRC_DBG_MIN, "GetDdr3RelevantSpdBytes() failed.  Skipping fill of SPD data.\n"));
        } else {
          for (i = 0; i < NumElements; i++) {
            if (Ddr3RelevantSpdBytes[i] == 0) {
              continue;  // There are a few values in the "SPD" table that do not actually exist in the SPD.
            }
            (*dimmBDAT) [dimm].spdBytes.valid[Ddr3RelevantSpdBytes[i] / 8] |= 1 << (Ddr3RelevantSpdBytes[i] % 8);
            (*dimmBDAT) [dimm].spdBytes.spdData[Ddr3RelevantSpdBytes[i]] = Host->Channel[ch].SpdData[dimm].Buffer[Ddr3RelevantSpdBytes[i]];
          }  // for (i = 0; i < NumElements; i++)
        }  // if ((NumElements == 0) || (Ddr3RelevantSpdBytes == NULL))
#endif
        rankBDAT = &HostBDAT->bdatSys.socketList[node].channelList[ch].dimmList[dimm].rankList;

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          //
          // Skip if no rank or if it is mapped out
          //
          if (Host->Channel[ch].RankEnabled[rank + dimm * MAX_RANK_DIMM] == 0) {
            continue;
          }

          //
          // Indicate this DIMM is enabled
          //
          (*rankBDAT) [rank].rankEnabled = 1;

#ifdef BDAT_REV2_SUPPORT
          if (Host->MrcRMTSupport) {
            (*rankBDAT) [rank].rankMarginEnabled = 1;
          }

          (*rankBDAT) [rank].dqMarginEnabled     = 0;   //DQ margins are optional

          //
          // DQ Margins are optional.
          //
#endif

          (*rankBDAT) [rank].rankMargin.rxDqLeft   = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][RxDqLeft];
          (*rankBDAT) [rank].rankMargin.rxDqRight  = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][RxDqRight];
          (*rankBDAT) [rank].rankMargin.txDqLeft   = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][TxDqLeft];
          (*rankBDAT) [rank].rankMargin.txDqRight  = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][TxDqRight];
          //(*rankBDAT).[rank].rankMargin.cmdLeft  = (*rankList)[rank].cmdLeft;
          //(*rankBDAT).[rank].rankMargin.cmdRight = (*rankList)[rank].cmdRight;
          (*rankBDAT) [rank].rankMargin.rxVrefLow  = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][RxVLow];
          (*rankBDAT) [rank].rankMargin.rxVrefHigh = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][RxVHigh];
          (*rankBDAT) [rank].rankMargin.txVrefLow  = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][TxVLow];
          (*rankBDAT) [rank].rankMargin.txVrefHigh = (UINT8) Host->Channel[ch].RMT_Data[rank + dimm * MAX_RANK_DIMM][TxVHigh];

          ///
          /// Get training results
          ///
          (*rankBDAT) [rank].rankTraining.clkDelay = 255;
          (*rankBDAT) [rank].rankTraining.ctlDelay = 255;
          (*rankBDAT) [rank].rankTraining.cmdDelay[0] = 255;
          (*rankBDAT) [rank].rankTraining.cmdDelay[1] = 255;
          (*rankBDAT) [rank].rankTraining.cmdDelay[2] = 255;
          (*rankBDAT) [rank].rankTraining.ioLatency = 0;    // appears to be inapplicable to AVN
          (*rankBDAT) [rank].rankTraining.roundtrip = 0;    // appears to be inapplicable to AVN

          for (strobe = 0; strobe < MAX_STROBES; strobe++) {
            ///
            /// Skip if this is an ECC strobe when ECC is disabled
            ///
            if ((!Host->Channel[ch].EccEnabled) && ((strobe == 8) || (strobe == 17) ) ) {
              continue;
            }
//            (*rankBDAT) [rank].rankTraining.recEnDelay[strobe] = Host->Channel[ch].Trained_Value.RcvnValues[rank + dimm * MAX_RANK_DIMM].TotalDelay[strobe];
//            (*rankBDAT) [rank].rankTraining.wlDelay[strobe] = Host->Channel[ch].Trained_Value.WDqsValues[rank + dimm * MAX_RANK_DIMM].TotalDelay[strobe];
//            (*rankBDAT) [rank].rankTraining.rxDqDelay[strobe] = Host->Channel[ch].Trained_Value.RDqsValues[rank + dimm * MAX_RANK_DIMM].Pi[strobe];
            ///
            /// Based on conversation with Van Lovelace, and because the field is
            /// only 8 bit, I am filling in txDqDelay with only the PI value.
            /// This is spite of the fact that on AVN, the Twoxclk varies from
            /// 1-3 and seems like it needs to be part of the reported value.  In
            /// reality, the bdat structure seems like it should support the full
            /// linear value.
            ///
//            (*rankBDAT) [rank].rankTraining.txDqDelay[strobe] = Host->Channel[ch].Trained_Value.WDqValues[rank + dimm * MAX_RANK_DIMM].Pi[strobe];
          } // strobe loop

          //
          //  Strip off the bits for command, bank, and rank that are in the AVN structures but are unneeded for BDAT.
          //
//          (*rankBDAT) [rank].rankMRS.mr0 = (UINT16) ((Host->Channel[ch].MrsValue[rank + dimm * MAX_RANK_DIMM][0] >> 6) & 0xffff);
//          (*rankBDAT) [rank].rankMRS.mr1 = (UINT16) ((Host->Channel[ch].MrsValue[rank + dimm * MAX_RANK_DIMM][1] >> 6) & 0xffff);
//          (*rankBDAT) [rank].rankMRS.mr2 = (UINT16) ((Host->Channel[ch].MrsValue[rank + dimm * MAX_RANK_DIMM][2] >> 6) & 0xffff);
//          (*rankBDAT) [rank].rankMRS.mr3 = (UINT16) ((Host->Channel[ch].MrsValue[rank + dimm * MAX_RANK_DIMM][3] >> 6) & 0xffff);
        } // rank loop
      } // dimm loop
    } // ch loop
  } // node loop

  //
  // Save reference code revision
  //
  HostBDAT->bdatSys.refCodeRevision = MRC_VERSION;

#ifdef BDAT_REV2_SUPPORT

  HostBDAT->bdatSys.maxNode     = MAX_NODE;
  HostBDAT->bdatSys.maxCh       = MAX_CHANNELS;
  HostBDAT->bdatSys.maxDimm     = MAX_DIMM;
  HostBDAT->bdatSys.maxRankDimm = MAX_RANK_DIMM;
  HostBDAT->bdatSys.maxStrobe   = MAX_STROBES;
  HostBDAT->bdatSys.maxDq       = MAX_STROBES * 8;

#endif

  //
  // Save margin loop count
  //
  HostBDAT->bdatSys.marginLoopCount = Host->MrcTrainingCpgcExpLoopCnt;

  //
  // Create BIOS Data Signature
  //
  HostBDAT->bdatHeader.BiosDataSignature[0] = 'B';
  HostBDAT->bdatHeader.BiosDataSignature[1] = 'D';
  HostBDAT->bdatHeader.BiosDataSignature[2] = 'A';
  HostBDAT->bdatHeader.BiosDataSignature[3] = 'T';
  HostBDAT->bdatHeader.BiosDataSignature[4] = 'H';
  HostBDAT->bdatHeader.BiosDataSignature[5] = 'E';
  HostBDAT->bdatHeader.BiosDataSignature[6] = 'A';
  HostBDAT->bdatHeader.BiosDataSignature[7] = 'D';

  //
  // Structure size
  //
  HostBDAT->bdatHeader.BiosDataStructSize = sizeof (struct bdatStruct);

  //
  // Primary Version
  //
  HostBDAT->bdatHeader.PrimaryVersion =  BDAT_PRIMARY_VER;

  //
  // Secondary Version
  //
  HostBDAT->bdatHeader.SecondaryVersion =  BDAT_SECONDARY_VER;

  //
  // CRC16
  //
  HostBDAT->bdatHeader.Crc16 = 0;
  HostBDAT->bdatHeader.Crc16 = Crc16 ((INT8 *) HostBDAT, sizeof (struct bdatStruct) );

  //
  // Save BDAT to Scratchpad
  //

  //
  // The code to save to scratchpad and create the HOB was already in MemoryInit.
  //

  //
  // Print Compatible BIOS Data Structure
  //
  MmrcDebugPrint ((MMRC_DBG_MAX, "\nstruct HostBDAT->bdatHeader {\n"));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  BiosDataSignature          "));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[0]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[1]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[2]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[3]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[4]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[5]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c", HostBDAT->bdatHeader.BiosDataSignature[6]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "%c\n", HostBDAT->bdatHeader.BiosDataSignature[7]));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  BiosDataStructSize         0x%x\n", HostBDAT->bdatHeader.BiosDataStructSize));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  Crc16                      0x%x\n", HostBDAT->bdatHeader.Crc16));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  PrimaryVersion             0x%x\n", HostBDAT->bdatHeader.PrimaryVersion));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  SecondaryVersion           0x%x\n", HostBDAT->bdatHeader.SecondaryVersion));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  OemOffset                  0x%x\n", HostBDAT->bdatHeader.OemOffset));
  MmrcDebugPrint ((MMRC_DBG_MAX, "} // bdatheader\n"));
  MmrcDebugPrint ((MMRC_DBG_MAX, "\nstruct Host.bdat.bdatSys {\n"));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  refCodeRevision          %02x.%02x.%02x\n", (HostBDAT->bdatSys.refCodeRevision >> 24) & 0xff,
            (HostBDAT->bdatSys.refCodeRevision >> 16) & 0xff, (HostBDAT->bdatSys.refCodeRevision >> 8) & 0xff));
  MmrcDebugPrint ((MMRC_DBG_MAX, "  marginLoopCount          %d\n", HostBDAT->bdatSys.marginLoopCount));
  for (node = 0; node < MAX_NODE; node++) {
    if (( HostBDAT->bdatSys.socketList[node].imcEnabled == 0) || (HostBDAT->bdatSys.maxDimm == 0) ) {
      continue;
    }
    MmrcDebugPrint ((MMRC_DBG_MAX, "  struct socketList[%d] {\n", node));
    MmrcDebugPrint ((MMRC_DBG_MAX, "    imcEnabled             %d\n", HostBDAT->bdatSys.socketList[node].imcEnabled));
    MmrcDebugPrint ((MMRC_DBG_MAX, "    ddrFreq                %d MT/s\n", HostBDAT->bdatSys.socketList[node].ddrFreq));
    MmrcDebugPrint ((MMRC_DBG_MAX, "    ddrVoltage             %d mV\n", HostBDAT->bdatSys.socketList[node].ddrVoltage));
    MmrcDebugPrint ((MMRC_DBG_MAX, "    imcDid                 0x%4.4x\n", HostBDAT->bdatSys.socketList[node].imcDid));
    MmrcDebugPrint ((MMRC_DBG_MAX, "    imcRid                 %d\n", HostBDAT->bdatSys.socketList[node].imcRid));
    channelBDAT = &HostBDAT->bdatSys.socketList[node].channelList;
    for (ch = 0; ch < MAX_CHANNELS; ch++) {
      if ((*channelBDAT) [ch].chEnabled == 0) {
        continue;
      }
      MmrcDebugPrint ((MMRC_DBG_MAX, "    struct channelList[%d] {\n", ch));
      MmrcDebugPrint ((MMRC_DBG_MAX, "      chEnabled            %d\n", (*channelBDAT) [ch].chEnabled));
      MmrcDebugPrint ((MMRC_DBG_MAX, "      numDimmSlot          %d\n", (*channelBDAT) [ch].numDimmSlot));
      dimmBDAT = &HostBDAT->bdatSys.socketList[node].channelList[ch].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmBDAT) [dimm].dimmEnabled == 0) {
          continue;
        }
        MmrcDebugPrint ((MMRC_DBG_MAX, "      struct dimmList[%d] {\n", dimm));
        MmrcDebugPrint ((MMRC_DBG_MAX, "      dimmEnabled          %d\n", (*dimmBDAT) [dimm].dimmEnabled));
        rankBDAT = &HostBDAT->bdatSys.socketList[node].channelList[ch].dimmList[dimm].rankList;
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if ((*rankBDAT) [rank].rankEnabled == 0) {
            continue;
          }
          MmrcDebugPrint ((MMRC_DBG_MAX, "        struct rankList[%d] {\n", rank));
          MmrcDebugPrint ((MMRC_DBG_MAX, "        rankEnabled        %d\n", (*rankBDAT) [rank].rankEnabled));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          struct rankMargin {\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            rxDqLeft       %d\n", (*rankBDAT) [rank].rankMargin.rxDqLeft));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            rxDqRight      %d\n", (*rankBDAT) [rank].rankMargin.rxDqRight));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            txDqLeft       %d\n", (*rankBDAT) [rank].rankMargin.txDqLeft));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            txDqRight      %d\n", (*rankBDAT) [rank].rankMargin.txDqRight));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            cmdLeft        N/A\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            cmdRight       N/A\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            rxVrefLow      %d\n", (*rankBDAT) [rank].rankMargin.rxVrefLow));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            rxVrefHigh     %d\n", (*rankBDAT) [rank].rankMargin.rxVrefHigh));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            txVrefLow      %d\n", (*rankBDAT) [rank].rankMargin.txVrefLow));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            txVrefHigh     %d\n", (*rankBDAT) [rank].rankMargin.txVrefHigh));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          } // rankMargin\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          struct rankTraining {\n"));
          for (strobe = 0; strobe < MAX_STROBES; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!Host->Channel[ch].EccEnabled) && ((strobe == 8) || (strobe == 17) ) ) {
              continue;
            }
            MmrcDebugPrint ((MMRC_DBG_MAX, "            recEnDelay[%d]  %d\n", strobe, (*rankBDAT) [rank].rankTraining.recEnDelay[strobe]));
          } // strobe loop
          for (strobe = 0; strobe < MAX_STROBES; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!Host->Channel[ch].EccEnabled) && ((strobe == 8) || (strobe == 17) ) ) {
              continue;
            }
            MmrcDebugPrint ((MMRC_DBG_MAX, "            wlDelay[%d]     %d\n", strobe, (*rankBDAT) [rank].rankTraining.wlDelay[strobe]));
          } // strobe loop
          for (strobe = 0; strobe < MAX_STROBES; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!Host->Channel[ch].EccEnabled) && ((strobe == 8) || (strobe == 17) ) ) {
              continue;
            }
            MmrcDebugPrint ((MMRC_DBG_MAX, "            rxDqDelay[%d]   %d\n", strobe, (*rankBDAT) [rank].rankTraining.rxDqDelay[strobe]));
          } // strobe loop
          for (strobe = 0; strobe < MAX_STROBES; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!Host->Channel[ch].EccEnabled) && ((strobe == 8) || (strobe == 17) ) ) {
              continue;
            }
            MmrcDebugPrint ((MMRC_DBG_MAX, "            txDqDelay[%d]   %d\n", strobe, (*rankBDAT) [rank].rankTraining.txDqDelay[strobe]));
          } // strobe loop
          MmrcDebugPrint ((MMRC_DBG_MAX, "            clkDelay       %d\n", (*rankBDAT) [rank].rankTraining.clkDelay));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            ctlDelay       %d\n", (*rankBDAT) [rank].rankTraining.ctlDelay));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            cmdDelay[0]    %d\n", (*rankBDAT) [rank].rankTraining.cmdDelay[0]));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            cmdDelay[1]    %d\n", (*rankBDAT) [rank].rankTraining.cmdDelay[1]));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            cmdDelay[2]    %d\n", (*rankBDAT) [rank].rankTraining.cmdDelay[2]));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            ioLatency      %d\n", (*rankBDAT) [rank].rankTraining.ioLatency));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            roundtrip      %d\n", (*rankBDAT) [rank].rankTraining.roundtrip));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          } // rankTraining\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          struct rankMRS {\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            mr0            0x%x\n", (*rankBDAT) [rank].rankMRS.mr0));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            mr1            0x%x\n", (*rankBDAT) [rank].rankMRS.mr1));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            mr2            0x%x\n", (*rankBDAT) [rank].rankMRS.mr2));
          MmrcDebugPrint ((MMRC_DBG_MAX, "            mr3            0x%x\n", (*rankBDAT) [rank].rankMRS.mr3));
          MmrcDebugPrint ((MMRC_DBG_MAX, "          }\n"));
          MmrcDebugPrint ((MMRC_DBG_MAX, "        } // rankList\n"));
        } // rank loop
        MmrcDebugPrint ((MMRC_DBG_MAX, "        struct spdBytes {\n"));
        for (spd = 0; spd < MAX_SPD_BYTE; spd++) {
          if ((*dimmBDAT) [dimm].spdBytes.valid[spd / 8] & (1 << (spd % 8))) {
            MmrcDebugPrint ((MMRC_DBG_MAX, "          spdData[%d]        %d\n", spd, (*dimmBDAT) [dimm].spdBytes.spdData[spd]));
          }
        } // spd loop
        MmrcDebugPrint ((MMRC_DBG_MAX, "        } // spdBytes\n"));
        MmrcDebugPrint ((MMRC_DBG_MAX, "      } // dimmList\n"));
      } // dimm loop
      MmrcDebugPrint ((MMRC_DBG_MAX, "    } // channelList\n"));
    } // ch loop
    MmrcDebugPrint ((MMRC_DBG_MAX, "  } // node\n"));
  } // node loop
  MmrcDebugPrint ((MMRC_DBG_MAX, "} // bdat\n\n"));
} // FillBdatStructure

/**
  This function returns the CRC16 of the provided array.

  @param[in]  Ptr    The array to be processed.
  @param[in]  Count  Number of elements in the array.

  @retval  CRC16 of the provided array.
**/
INT16
Crc16 (
  IN INT8  *Ptr,
  IN INT32 Count
)
{
  INT16 crc, i;
  crc = 0;
  while (--Count >= 0) {
    crc = crc ^ (INT16) (int) * Ptr++ << 8;
    for (i = 0; i < 8; ++i) {
      if (crc & 0x8000) {
        crc = crc << 1 ^ 0x1021;
      } else {
        crc = crc << 1;
      }
    }
  }
  return (crc & 0xFFFF);
}

#endif  //  BDAT
