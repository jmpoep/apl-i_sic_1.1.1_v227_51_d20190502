/** @file
  Register names for Gaussian Mixture Models.

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, SC registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between SC generations are denoted by
    "_PCH_<generation_name>_" in register/bit names. e.g., "_PCH_CHV_"
  - Registers / bits that are different between SKUs are denoted by "_<SKU_name>"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a SC generation will be just named
    as "_PCH_" without <generation_name> inserted.

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
#ifndef _REGS_GMM_H_
#define _REGS_GMM_H_

///
/// GMM Config Registers (D0:F3)
///
#define PCI_DEVICE_NUMBER_GMM          0
#define PCI_FUNCTION_NUMBER_GMM        3

#define R_GMM_DEVVENID                 0x00  ///< Device / Vendor ID
#define B_GMM_DEVVENID_DEVICE_ID       0xFFFF0000 ///< Device ID
#define B_GMM_DEVVENID_VENDOR_ID       0x0000FFFF ///< Vendor ID
#define V_GMM_DEVVENID_VENDOR_ID       V_INTEL_VENDOR_ID ///< Intel Vendor ID
#define V_GMM_DEVICE_ID_0              0x1AA2

#define R_GMM_STSCMD                   0x04  ///< Status Command
#define B_GMM_STSCMD_RMA               BIT29 ///< Received Master Abort
#define B_GMM_STSCMD_RTA               BIT28 ///< Received Target Abort
#define B_GMM_STSCMD_CAP_LST           BIT20 ///< Capabilities List
#define B_GMM_STSCMD_INTR_STS          BIT19 ///< Interrupt Status
#define B_GMM_STSCMD_INTR_DIS          BIT10 ///< Interrupt Disable
#define B_GMM_STSCMD_SERR_EN           BIT8  ///< SERR Enable
#define B_GMM_STSCMD_BME               BIT2  ///< Bus Master Enable
#define B_GMM_STSCMD_MSE               BIT1  ///< Memory Space Enable

#define R_GMM_RID_CC                   0x08  ///< Revision ID and Class Code
#define B_GMM_RID_CC_BCC               0xFF000000 ///< Base Class Code
#define B_GMM_RID_CC_SCC               0x00FF0000 ///< Sub Class Code
#define B_GMM_RID_CC_PI                0x0000FF00 ///< Programming Interface
#define B_GMM_RID_CC_RID               0x000000FF ///< Revision Identification

#define R_GMM_BAR0_LO                  0x10  ///< BAR 0 Low
#define B_GMM_BAR0_BA                  0xFFFFF000 ///< Base Address
#define V_GMM_BAR0_SIZE                0x1000
#define N_GMM_BAR0_ALIGNMENT           12
#define B_GMM_BAR0_PREF                BIT3  ///< Prefetchable
#define B_GMM_BAR0_ADDRNG              (BIT2 | BIT1) ///< Address Range
#define B_GMM_BAR0_SPTYP               BIT0  ///< Space Type (Memory)

#define R_GMM_BAR0_HI                  0x14  ///< BAR 0 High

#define R_GMM_SSID                     0x2C  ///< Sub System ID
#define B_GMM_SSID_SID                 0xFFFF0000 ///< Sub System ID
#define B_GMM_SSID_SVID                0x0000FFFF ///< Sub System Vendor ID

#define R_GMM_ERBAR                    0x30  ///< Expansion ROM BAR
#define B_GMM_ERBAR_BA                 0xFFFFFFFF ///< Expansion ROM Base Address

#define R_GMM_CAPPTR                   0x34  ///< Capability Pointer
#define B_GMM_CAPPTR_CPPWR             0xFF  ///< Capability Pointer Power

#define R_GMM_INTR                     0x3C  ///< Interrupt
#define B_GMM_INTR_ML                  0xFF000000 ///< Max Latency
#define B_GMM_INTR_MG                  0x00FF0000
#define B_GMM_INTR_IP                  0x00000F00 ///< Interrupt Pin
#define B_GMM_INTR_IL                  0x000000FF ///< Interrupt Line

#define R_GMM_OVRCFGCTL                0x40  ///< Override Configuration Control
#define B_GMM_OVRCFGCTL_PGCBCGEN       BIT9  ///< Clock Gating - PGCB Clock Trunk Enable
#define B_GMM_OVRCFGCTL_SBDCGEN        BIT8  ///< Clock Gating - Sideband Enable
#define B_GMM_OVRCFGCTL_SBTCGEN        BIT7  ///< Clock Gating - Sideband Trunk Enable
#define B_GMM_OVRCFGCTL_SBPCGEN        BIT6  ///< Clock Gating - Sideband Partition Enable
#define B_GMM_OVRCFGCTL_C_DCGEN        BIT5  ///< Clock Gating - Core Enable
#define B_GMM_OVRCFGCTL_DMA_DCGEN      BIT4  ///< Clock Gating - DMA Enable
#define B_GMM_OVRCFGCTL_RA_DCGEN       BIT3  ///< Clock Gating - Register Access Enable
#define B_GMM_OVRCFGCTL_HOST_DCGEN     BIT2  ///< Clock Gating - Host Interface Enable
#define B_GMM_OVRCFGCTL_PCGEN          BIT1  ///< Clock Gating - Partition Enable
#define B_GMM_OVRCFGCTL_TCGEN          BIT0  ///< Clock Gating - Trunk Enable

#define R_GMM_D0I3_PWRCTRLEN           0xB2  ///< D0i3 Power Control Enable
#define B_GMM_D0I3_PWRCTRLEN_PG_MASK   0xFFF8
#define B_GMM_D0I3_PWRCTRLEN_HW_AUTO   BIT5  ///< Power Gating - HW Autonomous Enable
#define B_GMM_D0I3_PWRCTRLEN_D3_HOT    BIT2  ///< Power Gating - D3 Hot Enable
#define B_GMM_D0I3_PWRCTRLEN_I3        BIT1  ///< Power Gating - I3 Enable
#define B_GMM_D0I3_PWRCTRLEN_PMCR      BIT0  ///< Power Gating - PMC Request Enable

#define R_GMM_PCS                      0xE0  ///< PME Control Status
#define B_GMM_PCS_PS                   (BIT1 | BIT0) ///< Power State

///
/// GMM MMIO Registers
///
#define R_GMM_D0I3C                    0xA8
#define B_GMM_D0I3C_D0I3               BIT2
#endif
