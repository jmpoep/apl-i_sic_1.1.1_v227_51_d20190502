;; @file
;   Platform Specific Definitions
;
; @copyright
;  INTEL CONFIDENTIAL
;  Copyright 2008 - 2016 Intel Corporation.
;
;  The source code contained or described herein and all documents related to the
;  source code ("Material") are owned by Intel Corporation or its suppliers or
;  licensors. Title to the Material remains with Intel Corporation or its suppliers
;  and licensors. The Material may contain trade secrets and proprietary and
;  confidential information of Intel Corporation and its suppliers and licensors,
;  and is protected by worldwide copyright and trade secret laws and treaty
;  provisions. No part of the Material may be used, copied, reproduced, modified,
;  published, uploaded, posted, transmitted, distributed, or disclosed in any way
;  without Intel's prior express written permission.
;
;  No license under any patent, copyright, trade secret or other intellectual
;  property right is granted to or conferred upon you by disclosure or delivery
;  of the Materials, either expressly, by implication, inducement, estoppel or
;  otherwise. Any license under such intellectual property rights must be
;  express and approved by Intel in writing.
;
;  Unless otherwise agreed by Intel in writing, you may not remove or alter
;  this notice or any other notice embedded in Materials by Intel or
;  Intel's suppliers or licensors in any way.
;
;  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
;  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
;  the terms of your license agreement with Intel or your vendor. This file may
;  be modified by the user, subject to additional terms of the license agreement.
;
; @par Specification
;;

;
;------------------------------------------------------------------------------
;INCLUDE FlashMap.inc ;ECP porting

MKF_SOFTSDV_FLAG              EQU   0

; Set "MINIMUM_BOOT_SUPPORT" flag allows BIOS boot as minimum feature in SEC phase.
MINIMUM_BOOT_SUPPORT          EQU 0               ; ="0", Normal Boot;
                                                  ; ="1", Minimum Feature Boot
; "RESET_IN_SEC" flag allows BIOS doing RESET in SEC phase
RESET_IN_SEC                  EQU 0               ; ="0", RESET occurs in OemIohInit.c
                                                  ; ="1", RESET occurs in SEC phase

EARLY_MICROCODE_SUPPORT       EQU 1
DETERMINISTIC_BSP_SUPPORT     EQU 0
DEBUG                         EQU 1

;
; IO port to access the upper 128-byte of RTC RAM
;
RTC_UPPER_INDEX               EQU 072h
RTC_UPPER_DATA                EQU 073h

;
; Offset of data stored in the upper 128-byte of RTC RAM.
;
CMOS_CPU_BSP_SELECT           EQU 010h            ; BspSelection
CMOS_CPU_UP_MODE              EQU 011h            ; UpBootSelection

;
; Cpu Ratio and Vid stored in the upper 128-byte of RTC RAM.
;
CMOS_CPU_RATIO_OFFSET         EQU 012h            ; ProcessorFlexibleRatio
CMOS_CPU_CORE_HT_OFFSET       EQU 013h            ; ProcessorHyperThreadingEnable & EnableCoresInSbsp & EnableCoresInNbsp

;
; CPU Feature
;
CMOS_CPU_BIST_OFFSET          EQU 015h            ; ProcessorBistEnable
CMOS_CPU_VMX_OFFSET           EQU 016h            ; ProcessorVmxEnable

;
; Port80 Selection
;
CMOS_PORT80_OFFSET         	  EQU 017h            ; Port80Route

;
;Flash layout map
;
PEICODE_REGION_BASE_ADDRESS   EQU FLASH_BASE
PEICODE_REGION_SIZE           EQU FLASH_SIZE
PEICODE_REGION_SIZE_MASK      EQU (NOT (PEICODE_REGION_SIZE - 1))

BIOS_REGION_UPDATABLE_STATUS  EQU 0058h            ; Offset
;----------------------------------------------------------------------------------------
; "Merlin" support used equates
;----------------------------------------------------------------------------------------
MAGIC_ADDRESS_IN_SEG          EQU  0FFF0h
MAGIC_SEG                     EQU  0F000h

;
; -- Equates for CAR initialization
;    TileSize (must be a power of 2)
;
; Define the tile size
; The tile size and tile placement are critical to ensuring that no data loss occurs
; See BWG - chapter "Determining Tile Size"
;
TILE_SIZE                     EQU 000000000h

;
; See BWG - chapter "Determining Cacheable Code Region Base Addresses and Ranges".
;
; Now FvRecovery base address and size is determined by PcdFlashFvIBBMBase and PcdFlashFvIBBMSize.
;

WDB_REGION_BASE_ADDRESS             EQU 040000000h
WDB_REGION_SIZE                     EQU 01000h
WDB_REGION_SIZE_MASK                EQU (NOT (WDB_REGION_SIZE - 1))

;
; See BWG - chapter "Determining Data Stack Base Address and Range"
;
; Now Data Stack base address and size is determined by PcdTemporaryRamBase and PcdTemporaryRamSize.
;

;
; Cache init and test values
; These are inverted to flip each bit at least once
;
CACHE_INIT_VALUE              EQU 0A5A5A5A5h

;ECP porting
;CACHE_TEST_VALUE             EQU (NOT CACHE_INIT_VALUE)
CACHE_TEST_VALUE              EQU 05aa55aa5h

PEI_CORE_ENTRY_BASE           EQU 0FFFFFFE0h
FV_MAIN_BASE                  EQU 0FFFFFFFCh

MAX_NR_BUS                    EQU 0FFh
MAX_NR_CPU_SOCKETS            EQU 2               ; DP example, MP may have 4 or more

BIT0                          EQU 01h
BIT1                          EQU 02h
BIT2                          EQU 04h
BIT3                          EQU 08h
BIT4                          EQU 10h
BIT5                          EQU 20h
BIT6                          EQU 40h
BIT7                          EQU 80h
BIT8                          EQU 100h
BIT9                          EQU 200h
BIT10                         EQU 400h
BIT11                         EQU 800h
BIT12                         EQU 1000h
BIT13                         EQU 2000h
BIT14                         EQU 4000h
BIT15                         EQU 8000h
BIT16                         EQU 10000h
BIT17                         EQU 20000h
BIT18                         EQU 40000h
BIT19                         EQU 80000h
BIT20                         EQU 0100000h
BIT23                         EQU 0800000h
BIT31                         EQU 080000000h
; Bit definition in MM1
BadCMOSDetected               EQU (BIT0 shl 17)
BSPApicIDSaveStart            EQU 24
